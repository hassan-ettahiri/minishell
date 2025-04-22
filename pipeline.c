#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
    char *input;

    printf("Simple Readline Shell\n");
    printf("Type 'clear' to clear history, 'replace' to change input line, 'exit' to quit.\n");

    while (1) {
        // Prompt user
        input = readline(">> ");

        if (!input) {
            // User pressed Ctrl+D
            printf("\nExiting...\n");
            break;
        }

        if (strlen(input) > 0) {
            add_history(input);  // Add command to history
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        } else if (strcmp(input, "clear") == 0) {
            printf("History cleared.\n");
            rl_clear_history();  // Clear all previous commands
        } else if (strcmp(input, "replace") == 0) {
            // Replace current line with something new
            rl_replace_line("This line was replaced!", 1);
            rl_on_new_line();  // Notify readline that we’re starting a new line
            rl_redisplay();    // Redisplay the updated line
        } else {
            printf("You typed: %s\n", input);
        }

        free(input); // Always free what readline allocates
    }

    return 0;
}