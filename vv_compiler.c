/*
 * (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>

#define VERSION 1

void
print_usage(char ** argv)
{
    printf( "VVhitespace Interpreter v%d (www.subgeniuskitty.com)\n"
            "Usage: %s -i <file> -o <file>\n"
            "  -h         Help (prints this message)\n"
            "  -i <file>  Specify a pseudo-VVhitespace source file as input.\n"
            "  -o <file>  Specify location for VVhitespace output.\n"
            , VERSION, argv[0]
    );
}

/* Builds an ASCII string on the stack using VVS PUSH_IMMEDIATE commands.    */
/* The syntax: A"test" results in five PUSH_IMMEDIATE commands for the four  */
/*   letters, and null-terminator.                                           */
/* Expects 'input' to present a double-quoted ('"') ASCII string.            */
/* The 'A' has already been chomped.                                         */
void
parse_ascii_string(FILE * input, FILE * output)
{
    uint8_t temp_byte;
    fread(&temp_byte, 1, 1, input);
    if (temp_byte != '"') {
        /* Die here since we walk the string backward and look for '"' as a terminator. */
        fprintf(stderr, "ERROR: Expected double-quote to begin string.\n");
        exit(EXIT_FAILURE);
    }

    /* Put the letters on the stack in reverse. Don't forget to put a null-terminator first. */
    for (fread(&temp_byte,1,1,input); temp_byte != '"'; fread(&temp_byte,1,1,input)) continue;
    temp_byte = '\0';

    while (temp_byte != '"') {
        /* First, push three spaces to start a PUSH_IMMEDIATE command for a positive number. */
        uint8_t temp_output = ' ';
        for(int i=0;i<3;i++) fwrite(&temp_output, 1, 1, output);
        /* Second, push the ASCII character, 7 bits total, most significant bit first. */
        /* Remember, [Tab]=1 and [Space]=0. */
        uint8_t index = 7; /* 7 bits needed per ASCII character. */
        while (index) {
            ((temp_byte >> (index-1)) & 1) ? (temp_output = '\t') : (temp_output = ' ');
            fwrite(&temp_output, 1, 1, output);
            index--;
        }
        /* Third, close the number with a newline. */
        temp_output = '\n';
        fwrite(&temp_output, 1, 1, output);

        /* Read the next byte to prepare for the loop test. */
        fseek(input, -2, SEEK_CUR);
        fread(&temp_byte, 1, 1, input);
    }

    /* Advance the stream pointer to the end of the string before returning. */
    for (fread(&temp_byte,1,1,input); temp_byte != '"'; fread(&temp_byte,1,1,input)) continue;
}

int
main(int argc, char ** argv)
{
    /*
     * Process command line arguments
     */
    int c;
    FILE * input = NULL, * output = NULL;
    while ((c = getopt(argc,argv,"i:o:h")) != -1) {
        switch (c) {
            case 'i':
                if ((input = fopen(optarg, "r")) == NULL) {
                    fprintf(stderr, "ERROR: %s: %s\n", optarg, strerror(errno));
                }
                break;
            case 'o':
                if ((output = fopen(optarg, "w+")) == NULL) {
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
        fprintf(stderr, "ERROR: Must specify a pseudo-VVhitespace source file with -i flag.\n");
        print_usage(argv);
        exit(EXIT_FAILURE);
    }
    if (output == NULL) {
        fprintf(stderr, "ERROR: Must specify destination for VVhitespace source file with -o flag.\n");
        print_usage(argv);
        exit(EXIT_FAILURE);
    }

    /*
     * Main Loop
     */
    uint8_t temp_byte;
    while (fread(&temp_byte, 1, 1, input)) {
        switch (temp_byte) {
            case 't':
            case 'T':
                temp_byte = '\t';
                fwrite(&temp_byte, 1, 1, output);
                break;
            case 's':
            case 'S':
                temp_byte = ' ';
                fwrite(&temp_byte, 1, 1, output);
                break;
            case 'n':
            case 'N':
                temp_byte = '\n';
                fwrite(&temp_byte, 1, 1, output);
                break;
            case 'v':
            case 'V':
                temp_byte = '\v';
                fwrite(&temp_byte, 1, 1, output);
                break;
            case 'a':
            case 'A':
                parse_ascii_string(input, output);
                break;
            case '\n':
                /* Intentionally empty */
                break;
            default:
                /* The first non-[tTsSnNvVaA] character begins a comment lasting until end of line. */
                while (fread(&temp_byte, 1, 1, input)) {
                    if (temp_byte == '\n') break;
                }
                break;
        }
    }

    /*
     * Cleanup and exit
     */
    fclose(input);
    fclose(output);

    exit(EXIT_SUCCESS);
}
