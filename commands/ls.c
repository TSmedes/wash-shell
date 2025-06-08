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
#include <dirent.h>

void print_usage(void);

int main(int argc, char *argv[]) {
    // Handle help flag
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        print_usage();
        return 0;
    }

    // Open current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    // Read and print directory entries
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
    return 0;
} 

/**
 * Prints the usage message for the program
 * @return void
 */
void print_usage(void) {
    printf("Usage: ls [-h]\n");
    printf("  -h    Show this help message\n");
    printf("Lists files in the current directory\n");
}