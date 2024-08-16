#include "headers.h"

void handleRedirection(char *input) {
    int input_redirection = 0;
    int output_redirection = 0;
    int append_redirection = 0;
    
    char *input_file = NULL;
    char *output_file = NULL;
    
    if (strstr(input, "<") != NULL) {
        input_redirection = 1;
        input_file = strtok(input, "<");
    }

    if (strstr(input, ">>") != NULL) {
        append_redirection = 1;
        output_file = strtok(input, ">>");
    } else if (strstr(input, ">") != NULL) {
        output_redirection = 1;
        output_file = strtok(input, ">");
    }

    if (input_file != NULL) {
        input_file = strtok(NULL, "<");
        if (input_file != NULL) {
            input_file = strtok(input_file, " ");
            input_file = strtok(input_file, "\t");
            input_file = strtok(input_file, "\n");
        }
    }

    if (output_file != NULL) {
        output_file = strtok(NULL, ">");
        if (output_file != NULL) {
            output_file = strtok(output_file, " ");
            output_file = strtok(output_file, "\t");
            output_file = strtok(output_file, "\n");
        }
    }

    if (input_redirection && input_file != NULL) {
        int fd = open(input_file, O_RDONLY);
        if (fd == -1) {
            perror("Input redirection error");
        } else {
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
    }

    if (output_redirection && output_file != NULL) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Output redirection error");
        } else {
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
    }

    if (append_redirection && output_file != NULL) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            perror("Append redirection error");
        } else {
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
    }
}
