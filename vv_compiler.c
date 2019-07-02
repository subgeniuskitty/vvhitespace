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
            case '\n':
                /* Intentionally empty */
                break;
            default:
                /* The first non-[tTsSnNvV] character begins a comment lasting until end of line. */
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

    printf("Successfully converted source code.\n");

    exit(EXIT_SUCCESS);
}
