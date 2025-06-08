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

#define MAX_LINES 1024
#define DEFAULT_LINES 10

void print_usage(void);
int is_number(const char *str);
void process_input(FILE *input, int num_lines);

int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_LINES;
    char *filename = NULL;
    int i;
    
    // Parse arguments
    for (i = 1; i < argc; i++) {
        // Prints usage if -h is provided
        if (strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        // Updates the number of lines to print if -n is provided
        } else if (strcmp(argv[i], "-n") == 0) {
            // Checks if -n is followed by a number
            if (i + 1 >= argc || !is_number(argv[i + 1])) {
                fprintf(stderr, "Error: -n requires a number\n");
                print_usage();
                return 1;
            }
            num_lines = atoi(argv[i + 1]);
            // Skip the number argument
            i++;
        // Checks if the argument is a file
        } else if (argv[i][0] != '-') {
            // Checks if filename is already set
            if (filename != NULL) {
                fprintf(stderr, "Error: Only one file argument allowed\n");
                print_usage();
                return 1;
            }
            filename = argv[i];
        } else {
            // Prints usage if an unknown option is provided
            fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
            print_usage();
            return 1;
        }
    }
    
    if (filename != NULL) {
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
            return 1;
        }
        process_input(file, num_lines);
        fclose(file);
    } else {
        process_input(stdin, num_lines);
    }
    
    return 0;
} 

/**
 * Prints the usage message for the program
 * @return void
 */
void print_usage(void) {
    printf("Usage: tail [-h] [-n N] [filename]\n");
    printf("  -h    Show this help message\n");
    printf("  -n N  Print last N lines (default: 10)\n");
    printf("  filename  Input file (if not specified, read from stdin)\n");
}

/**
 * Helper function to check if the string is a number
 * @param str - The string to check
 * @return 1 if the string is a number, 0 otherwise
 */
int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

/**
 * Prints the last num_lines lines of the input file
 * @param input - The input file
 * @param num_lines - The number of lines to print
 * @return void
 */
void process_input(FILE *input, int num_lines) {
    char *lines[MAX_LINES];
    int count = 0;
    char buffer[1024];
    
    // Read all lines into memory
    while (count < MAX_LINES && fgets(buffer, sizeof(buffer), input) != NULL) {
        lines[count] = strdup(buffer);
        if (lines[count] == NULL) {
            perror("Error reading from file");
            exit(1);
        }
        count++;
    }
    // Calculate the start line
    int start = (count > num_lines) ? count - num_lines : 0;
    // Print the last n lines
    for (int i = start; i < count; i++) {
        printf("%s", lines[i]);
        // Free the memory allocated for the line
        free(lines[i]);
    }
}