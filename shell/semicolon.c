#include"headers.h"

void support_semicolon(char *command) {
    char *token = strtok(command, ";\n");
    while (token != NULL) {
        // Trim leading and trailing spaces and tabs from the token
        char *trimmed_token = token;
        while (*trimmed_token == ' ' || *trimmed_token == '\t') {
            trimmed_token++;
        }
        char *end = trimmed_token + strlen(trimmed_token) - 1;
        while (end > trimmed_token && (*end == ' ' || *end == '\t')) {
            end--;
        }
        *(end + 1) = '\0';

        // Execute the trimmed token as a command
        // Add your logic here to execute the command
        printf("Executing command: %s\n", trimmed_token);

        // Get the next token
        token = strtok(NULL, ";\n");
    }
}
