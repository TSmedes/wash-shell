/**
 * Author: Toby Smedes
 * CSC3350
 * Wash Shell
 * Professor: Andy Cameron
 * Date: 2025-06-07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void);
void process_file(FILE *input);

int main(int argc, char *argv[]) {
    // Handle help flag
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        print_usage();
        return 0;
    }

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
        process_file(file);
        fclose(file);
    } else {
        process_file(stdin);
    }

    return 0;
} 

/**
 * Prints the usage message for the program
 * @return void
 */
void print_usage(void) {
    printf("Usage: cat [-h] [filename]\n");
    printf("  -h    Show this help message\n");
    printf("  filename  Input file (if not specified, read from stdin)\n");
}

/**
 * Prints the contents of the file to stdout
 * @param input - The file to print
 * @return void
 */
void process_file(FILE *input) {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), input) != NULL) {
        printf("%s", buffer);
    }
}