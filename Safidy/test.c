#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    // Initialize readline and history
    using_history();

    while (1) {
        // Prompt for user input
        input = readline("Enter a command: ");
        
        if (input == NULL) {
            break;  // Exit if no input (Ctrl+D)
        }

        // If input is not empty, add it to history
        if (*input) {
            add_history(input);
        }

        // Print the input to the console
        printf("You entered: %s\n", input);

        // Free the memory allocated by readline
        free(input);
    }

    return 0;
}
