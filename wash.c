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
#include <unistd.h>
#include <ctype.h>

#define MAX_LINE 256
#define MAX_ARGS 10
#define MAX_PATH 100

// Global variables
char *path_dirs[MAX_PATH];  // Array to store PATH directories
int path_count = 1;         // Initially contains only /bin

void initialize_shell(void);
void parse_command(char *line, char **args);
void execute_command(char **args);
void handle_builtin(char **args);
void print_help(void);

// Built-in commands
int is_builtin(char *cmd);
void do_exit(void);
void do_echo(char **args);
void do_pwd(void);
void do_cd(char **args);
void do_setpath(char **args);
void do_help(void);

int main(int argc, char *argv[]) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    
    // Handle -h flag
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }
    
    initialize_shell();
    
    while (1) {
        printf("wash> ");
        /*
         * fgets reads a line from stdin into the buffer 'line'
         * Parameters:
         *   - line: buffer to store the input
         *   - MAX_LINE: maximum number of characters to read
         *   - stdin: input stream to read from
         * Returns:
         *   - On success: pointer to the buffer
         *   - On EOF or error: NULL
         * We check for NULL to handle EOF (Ctrl+D) gracefully
         */
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;
        }
        
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse and execute command
        parse_command(line, args);
        if (args[0] != NULL) {
            execute_command(args);
        }
    }
    
    return 0;
}

/**
 * Initializes the shell with the default path
 * @return void
 */
void initialize_shell(void) {
    /*
     * strdup creates a duplicate of the string "/bin"
     * Parameters:
     *   - "/bin": string to duplicate
     * Returns:
     *   - On success: pointer to new string
     *   - On failure: NULL
     * We store this in path_dirs[0] as the initial PATH
     */
    path_dirs[0] = strdup("/bin");
    path_count = 1;
}

/**
 * Parses the command provided with its arguments
 * @param line - The command to parse
 * @param args - The arguments to the command
 * @return void
 */
void parse_command(char *line, char **args) {
    char *token;
    int i = 0;
    
    // Skip leading whitespace
    while (isspace(*line)) line++;
    
    /*
     * strtok splits the input line into tokens
     * Parameters:
     *   - line: string to tokenize
     *   - " \t": delimiter characters (space and tab)
     * Returns:
     *   - On success: pointer to next token
     *   - When no more tokens: NULL
     * We use this to separate command and arguments
     */
    token = strtok(line, " \t");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        // Gets next string until whitespace or tab
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
}

/**
 * Executes the command provided with its arguments
 * @param args - The arguments to the command
 * @return void
 */
void execute_command(char **args) {
    if (is_builtin(args[0])) {
        handle_builtin(args);
    } else {
        /*
         * fork creates a new process
         * Returns:
         *   - On success: 
         *     * Parent process: PID of child
         *     * Child process: 0
         *   - On failure: -1
         * We use this to execute external commands
         */
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process
            char cmd_path[MAX_LINE];
            int found = 0;
            
            // Search for command in path
            for (int i = 0; i < path_count; i++) {
                /*
                 * snprintf safely formats the command path
                 * Parameters:
                 *   - cmd_path: destination buffer
                 *   - MAX_LINE: maximum number of characters to write
                 *   - "%s/%s": format string
                 *   - path_dirs[i]: directory path
                 *   - args[0]: command name
                 * Returns:
                 *   - On success: number of characters that would be written
                 *   - On failure: negative value
                 * We use this to construct the full path to the command
                 */
                snprintf(cmd_path, MAX_LINE, "%s/%s", path_dirs[i], args[0]);
                
                /*
                 * access checks if the file exists and is executable
                 * Parameters:
                 *   - cmd_path: path to check
                 *   - X_OK: check for execute permission
                 * Returns:
                 *   - On success: 0
                 *   - On failure: -1
                 * We use this to find the first executable command in PATH
                 */
                if (access(cmd_path, X_OK) == 0) {
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                printf("%s: command not found\n", args[0]);
                exit(1);
            }
            
            /*
             * execv replaces the current process with a new program
             * Parameters:
             *   - cmd_path: path to the executable
             *   - args: array of arguments
             * Returns:
             *   - On success: does not return
             *   - On failure: -1
             * We use this to execute the external command
             */
            execv(cmd_path, args);
            perror("execv failed");
            exit(1);
        } else {
            /*
             * wait waits for the child process to finish
             * Parameters:
             *   - NULL: don't store exit status
             * Returns:
             *   - On success: PID of the terminated child
             *   - On failure: -1
             * We use this to wait for the command to complete
             */
            wait(NULL);
        }
    }
}

/**
 * Checks if the command is a built-in command
 * @param cmd - The command to check
 * @return 1 if the command is a built-in command, 0 otherwise
 */
int is_builtin(char *cmd) {
    return (strcmp(cmd, "exit") == 0 ||
            strcmp(cmd, "echo") == 0 ||
            strcmp(cmd, "pwd") == 0 ||
            strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "setpath") == 0 ||
            strcmp(cmd, "help") == 0);
}

/**
 * Handles the built-in commands
 * @param args - The arguments to the command
 * @return void
 */
void handle_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        do_exit();
    } else if (strcmp(args[0], "echo") == 0) {
        do_echo(args);
    } else if (strcmp(args[0], "pwd") == 0) {
        do_pwd();
    } else if (strcmp(args[0], "cd") == 0) {
        do_cd(args);
    } else if (strcmp(args[0], "setpath") == 0) {
        do_setpath(args);
    } else if (strcmp(args[0], "help") == 0) {
        do_help();
    }
}

/**
 * Exits the shell with exit code 0
 * @return void
 */
void do_exit(void) {
    exit(0);
}

/**
 * Prints the arguments to stdout
 * @param args - The arguments to the echo command
 * @return void
 */
void do_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s%s", args[i], args[i + 1] == NULL ? "\n" : " ");
    }
}

/**
 * Prints the current working directory to stdout
 * @return void
 */
void do_pwd(void) {
    char cwd[MAX_LINE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

/**
 * Changes the current working directory to the directory specified in args[1]
 * If no argument is provided, changes to the home directory
 * @param args - The arguments to the cd command
 * @return void
 */
void do_cd(char **args) {
    const char *dir = args[1];
    if (dir == NULL) {
        dir = getenv("HOME");
    }
    if (chdir(dir) != 0) {
        perror("cd");
    }
}

/**
 * Sets the PATH for command lookup
 * @param args - The arguments to the setpath command
 * @return void
 */
void do_setpath(char **args) {
    if (args[1] == NULL) {
        printf("setpath: requires at least one directory\n");
        return;
    }
    // Free existing path
    for (int i = 0; i < path_count; i++) {
        free(path_dirs[i]);
    }
    // Set new path
    path_count = 0;
    for (int i = 1; args[i] != NULL && path_count < MAX_PATH; i++) {
        path_dirs[path_count++] = strdup(args[i]);
    }
}

/**
 * Prints the help message for the built-in commands
 * @return void
 */
void do_help(void) {
    printf("Built-in commands:\n");
    printf("  exit    - Exit the shell\n");
    printf("  echo    - Print arguments to console\n");
    printf("  pwd     - Print current working directory\n");
    printf("  cd      - Change directory (defaults to home if no argument)\n");
    printf("  setpath - Set the PATH for command lookup\n");
    printf("  help    - Show this help message\n");
}

/**
 * Prints the help message for the shell
 * @return void
 */
void print_help(void) {
    printf("Usage: wash [-h]\n");
    printf("  -h    Show this help message\n");
    do_help();
} 