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
#include <ctype.h>

void print_usage(void);
void process_file(FILE *input, int *lines, int *words, int *chars);

int main(int argc, char *argv[]) {
    // Handle help flag
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        print_usage();
        return 0;
    }

    int lines = 0, words = 0, chars = 0;

    // Opens the file if provided
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        // Checks if the file is opened
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
        process_file(file, &lines, &words, &chars);
        fclose(file);
    } else {
        process_file(stdin, &lines, &words, &chars);
    }

    // Prints the number of lines, words, and characters
    printf("%d %d %d\n", lines, words, chars);
    return 0;
} 

/**
 * Prints the usage message for the program
 * @return void
 */
void print_usage(void) {
    printf("Usage: wc [-h] [filename]\n");
    printf("  -h    Show this help message\n");
    printf("  filename  Input file (if not specified, read from stdin)\n");
    printf("Counts lines, words, and characters in the input\n");
}

/**
 * Processes the input file and counts the lines, words, and characters
 * @param input - The input file
 * @param lines - The number of lines in the input file
 * @param words - The number of words in the input file
 * @param chars - The number of characters in the input file
 * @return void
 */
void process_file(FILE *input, int *lines, int *words, int *chars) {
    char buffer[1024];
    int in_word = 0;
    
    while (fgets(buffer, sizeof(buffer), input) != NULL) {
        (*lines)++;
        for (int i = 0; buffer[i] != '\0'; i++) {
            (*chars)++;
            if (isspace(buffer[i])) {
                if (in_word) {
                    (*words)++;
                    in_word = 0;
                }
            } else {
                in_word = 1;
            }
        }
    }
    // Count the last word
    if (in_word) {
        (*words)++;
    }
}