/*
 * (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/select.h>
#include <getopt.h>

#define VERSION 1

#define STACKSIZE 1024
#define HEAPSIZE  1024
#define RETURNSTACKSIZE 1024

void
print_usage(char ** argv)
{
    printf( "VVhitespace Interpreter v%d (www.subgeniuskitty.com)\n"
            "Usage: %s -i <file>\n"
            "  -h         Help (prints this message)\n"
            "  -i <file>  Specify a VVhitespace source file to interpret.\n"
            , VERSION, argv[0]
    );
}

int
stdin_empty(void)
{
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int retval = select(1, &read_fds, NULL, NULL, &timeout);
    /* retval could be -1. Ignoring that for now. */
    if (retval > 0) return 0;
    return 1;
}

void
ws_die(size_t * pc, char * msg)
{
    printf("SIM_ERROR @ PC %lu: %s\n", *pc, msg);
    fflush(stdout);
    exit(EXIT_FAILURE);
}

void
stack_push(int32_t ** sp, int32_t word)
{
    *((*sp)++) = word;
}

int32_t
stack_pop(int32_t ** sp)
{
    return *(--(*sp));
}

int32_t
stack_peek(int32_t ** sp, size_t offset)
/* offset=0 peeks TOS, offset=1 peeks NOS, etc. */
{
    return *((*sp)-offset-1);
}

uint8_t
next_code_byte(uint8_t * code, size_t * pc)
{
    return code[(*pc)++];
}

/*
 * In addition to returning the parsed label, this function advances the PC to
 * the next instruction.
 */
uint16_t
parse_label(uint8_t * code, size_t * pc)
{
    uint16_t label = 0;
    uint8_t c;
    while ((c = code[(*pc)++]) != '\n') {
        label = label << 1;
        if (c == ' ') label++;
    }
    // TODO: Where should I handle attempts to access an unitialized label?
    //       For now, leave it undefined in a nasal demon sense.
    return label;
}

void
populate_labels(uint32_t * labels, uint8_t * code, size_t code_size)
{
    size_t cp = 0;
    while (cp <= code_size) {
        if (code[cp] == '\v') {
            uint16_t temp_label = parse_label(code, &cp);
            labels[temp_label] = cp;
        }
        cp++;
    }
}

void
process_imp_stack(uint8_t * code, size_t * pc, int32_t ** sp)
{
    switch (next_code_byte(code,pc)) {
        case ' ':
            /* Push number onto TOS. */
            {
                /* First, pick off the sign */
                int32_t sign = 0;
                switch (next_code_byte(code,pc)) {
                    case ' ' :  sign = 1;                       break;
                    case '\t':  sign = -1;                      break;
                    default  :  ws_die(pc, "expected sign");    break;
                }

                /* Now, construct the number and push to TOS. */
                /* I'm assuming the numbers are read MSb first. */
                int32_t temp, number = 0;
                while ((temp = next_code_byte(code,pc)) != '\n') {
                    if (temp == '\v') ws_die(pc, "non-binary digit in number");
                    number <<= 1;
                    if (temp == '\t') number++;
                }
                stack_push(sp, number*sign);
            }
            break;
        case '\n':
            /* Stack sub-command */
            {
                switch (next_code_byte(code,pc)) {
                        /* Duplicate the TOS. */
                    case ' ':
                        stack_push(sp, stack_peek(sp,0));
                        break;
                        /* Swap TOS and NOS. */
                    case '\t':
                        {
                            int32_t t1 = stack_pop(sp);
                            int32_t t2 = stack_pop(sp);
                            stack_push(sp, t1);
                            stack_push(sp, t2);
                        }
                        break;
                        /* Discard TOS. */
                    case '\n':
                        stack_pop(sp);
                        break;
                    default:
                        ws_die(pc, "malformed stack IMP");
                        break;
                }
            }
            break;
        default: ws_die(pc, "malformed stack IMP"); break;
    }
}

void
process_imp_arithmetic(uint8_t * code, size_t * pc, int32_t ** sp)
{
    int32_t temp;
    switch (next_code_byte(code,pc)) {
        case ' ':
            {
                switch (next_code_byte(code,pc)) {
                    case ' ':
                        /* Addition */
                        stack_push(sp, stack_pop(sp)+stack_pop(sp));
                        break;
                    case '\t':
                        /* Subtraction */
                        temp = stack_pop(sp);
                        stack_push(sp, stack_pop(sp)-temp);
                        break;
                    case '\n':
                        /* Multiplication */
                        stack_push(sp, stack_pop(sp)*stack_pop(sp));
                        break;
                    default:
                        ws_die(pc, "malformed arithmetic IMP");
                        break;
                }
            }
            break;
        case '\t':
            {
                switch (next_code_byte(code,pc)) {
                    case ' ':
                        /* Division */
                        temp = stack_pop(sp);
                        stack_push(sp, stack_pop(sp)/temp);
                        break;
                    case '\t':
                        /* Modulo */
                        temp = stack_pop(sp);
                        stack_push(sp, stack_pop(sp)%temp);
                        break;
                    default: ws_die(pc, "malformed arithmetic IMP"); break;
                }
            }
            break;
        default: ws_die(pc, "malformed arithmetic IMP"); break;
    }
}

void
process_imp_flowcontrol(uint8_t * code, size_t * pc, int32_t ** sp, uint32_t * labels,
                        uint32_t ** rsp)
{
    switch (next_code_byte(code,pc)) {
        case '\n':
            /* Technically another LF is required but we ignore it. */
            fflush(stdout);
            exit(EXIT_SUCCESS);
        case ' ':
            {
                switch (next_code_byte(code,pc)) {
                    case ' ':
                        /* Mark a location in the program. */
                        if (next_code_byte(code,pc) != '\v') ws_die(pc,"expected vtab, "
                                "perhaps a whitespace program, rather than vvhitespace?");
                        /* Jump to next instruction since labels were parsed during startup. */
                        parse_label( code, pc);
                        break;
                    case '\t':
                        /* Call a subroutine. */
                        *((*rsp)++) = *pc;
                        *pc = labels[parse_label(code, pc)];
                        break;
                    case '\n':
                        /* Jump unconditionally to a label. */
                        *pc = labels[parse_label(code, pc)];
                        break;
                    default:
                        ws_die(pc, "malformed flow control IMP");
                        break;
                }
            }
            break;
        case '\t':
            {
                switch (next_code_byte(code,pc)) {
                    case ' ':
                        /* Jump to a label if TOS == 0 */
                        if (stack_peek(sp,0) == 0) *pc = labels[parse_label(code, pc)];
                        break;
                    case '\t':
                        /* Jump to a label if TOS < 0. */
                        if (stack_peek(sp,0) < 0) *pc = labels[parse_label(code, pc)];
                        break;
                    case '\n':
                        /* Return from subroutine. */
                        *pc = *(--(*rsp));
                        break;
                    default:
                        ws_die(pc, "malformed flow control IMP");
                        break;
                }
            }
            break;
        default:
            ws_die(pc, "malformed flow control IMP");
            break;
    }
}

void
process_imp_heap(uint8_t * code, size_t * pc, int32_t ** sp, int32_t ** hp)
{
    switch (next_code_byte(code,pc)) {
        case ' ' : /* Store to heap      */ *(*hp + *((*sp)-1)) = **sp; *sp -= 2; break;
        case '\t': /* Retrieve from heap */ **sp = *(*hp + **sp);                 break;
        default  : ws_die(pc, "malformed heap IMP");                              break;
    }
}

void
process_imp_io(uint8_t * code, size_t * pc, int32_t ** sp, int32_t ** hp)
{
    switch (next_code_byte(code,pc)) {
        case ' ':
            /* Output */
            {
                switch (next_code_byte(code,pc)) {
                    case ' ' : /* Output character from TOS */ printf("%c", stack_pop(sp)); break;
                    case '\t': /* Output number from TOS    */ printf("%d", stack_pop(sp)); break;
                    default  : ws_die(pc, "malformed output IMP");                          break;
                }
                fflush(stdout);
            }
            break;
        case '\t':
            /* Input */
            {
                while (stdin_empty()) continue;
                char c = getchar();
                switch (next_code_byte(code,pc)) {
                    case '\t': /* Input digit     */ c -= '0';                /* fallthrough */
                    case ' ' : /* Input character */ *(*hp + *((*sp)--)) = c; break;
                    default  : ws_die(pc, "malformed input IMP");             break;
                }
            }
            break;
        default: ws_die(pc, "malformed i/o IMP"); break;
    }
}

int
main(int argc, char ** argv)
{
    /*
     * Process command line arguments
     */
    int c;
    FILE * input = NULL;
    while ((c = getopt(argc,argv,"i:h")) != -1) {
        switch (c) {
            case 'i':
                if ((input = fopen(optarg, "r")) == NULL) {
                    fprintf(stderr, "ERROR: %s: %s\n", optarg, strerror(errno));
                }
                break;
            case 'h':
                print_usage(argv);
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    if (input == NULL) {
        fprintf(stderr, "ERROR: Must specify a VVhitespace source file with -f flag.\n");
        print_usage(argv);
        exit(EXIT_FAILURE);
    }

    /*
     * Read just the VVhitespace source code into memory.
     * We will use the array indices as addresses for the virtual PC when jumping to labels.
     */
    size_t ws_code_size = 0;
    uint8_t temp_byte;
    while (fread(&temp_byte, 1, 1, input)) {
        if (temp_byte == ' ' || temp_byte == '\t' || temp_byte == '\n' || temp_byte == '\v') {
            ws_code_size++;
        }
    }
    rewind(input);
    uint8_t * ws_code_space = malloc(ws_code_size);
    ws_code_size = 0;
    while (fread(&temp_byte, 1, 1, input)) {
        if (temp_byte == ' ' || temp_byte == '\t' || temp_byte == '\n' || temp_byte == '\v') {
            ws_code_space[ws_code_size++] = temp_byte;
        }
    }
    fclose(input);

    /*
     * Setup a stack and heap.
     * Assume a 32-bit word size.
     */
    // TODO: Make everything 64-bit.
    int32_t * hp = malloc(HEAPSIZE*4);
    int32_t * sp = malloc(STACKSIZE*4);

    /* 
     * Setup the return stack and the label array.
     */
    uint32_t * rsp = malloc(RETURNSTACKSIZE*4);
    uint32_t labels[65536] = {0};
    populate_labels(labels, ws_code_space, ws_code_size);

    /*
     * Main Loop
     */

    size_t pc = 0; /* Virtual program counter. Operates in the ws_code_space[] address space. */
    while (1) {
        if (pc >= ws_code_size) {
            fprintf(stderr, "SIM_ERROR: PC Overrun\n    Requested PC: %lu\n    Max Address: %lu\n",
                pc, ws_code_size-1);
            exit(EXIT_FAILURE);
        }
// TODO: Have the SIGTERM signal handler and normal term point return the value
// on TOS so I can do rudimentary automated tests.

        /* Decode the IMPs */
        switch (ws_code_space[pc++]) {
            case ' ':
                /* Stack Manipulation */
                process_imp_stack(ws_code_space, &pc, &sp);
                break;
            case '\n':
                /* Flow Control */
                process_imp_flowcontrol(ws_code_space, &pc, &sp, labels, &rsp);
                break;
            case '\t':
                /* Arithmetic, Heap Access, or I/O */
                {
                    switch (ws_code_space[pc++]) {
                        case ' ':
                            /* Arithmetic */
                            process_imp_arithmetic(ws_code_space, &pc, &sp);
                            break;
                        case '\t':
                            /* Heap Access */
                            process_imp_heap(ws_code_space, &pc, &sp, &hp);
                            break;
                        case '\n':
                            /* I/O */
                            process_imp_io(ws_code_space, &pc, &sp, &hp);
                            break;
                    }
                }
                break;
            default: ws_die(&pc, "unexpected VTab"); break;
        }
    }

    printf("\n");
    printf("Program executed.\n");

    exit(EXIT_SUCCESS);
}
