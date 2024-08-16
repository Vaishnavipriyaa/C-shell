#include "headers.h"

void handleInputRedirection(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Input redirection error");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void handleOutputRedirection(char *filename, int append) {
    int flags = O_WRONLY | O_CREAT;
    if (append) {
        flags |= O_APPEND;
    } else {
        flags |= O_TRUNC;
    }

    int fd = open(filename, flags, 0644);
    if (fd == -1) {
        perror("Output redirection error");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handlePipesAndRedirection(char *input) {
    char *outputRedirect = strstr(input, ">>");
    char *inputRedirect = strstr(input, "<");

    char *pipeCommands[32];  
    int pipeCommandCount = 0;
    char *command = strtok(input, "|");
    while (command != NULL) {
        pipeCommands[pipeCommandCount++] = command;
        command = strtok(NULL, "|");
    }

    if (inputRedirect != NULL) {
        strtok(inputRedirect, "<");
        char *filename = strtok(NULL, " \t\n"); 
        handleInputRedirection(filename);
    }

    if (outputRedirect != NULL) {
        strtok(outputRedirect, ">");
        char *filename = strtok(NULL, " \t\n"); 
        int append = strstr(outputRedirect, ">>") != NULL; 
        handleOutputRedirection(filename, append);
    }

    int pipes[pipeCommandCount - 1][2];
    for (int i = 0; i < pipeCommandCount - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    for (int i = 0; i < pipeCommandCount; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {

            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }

            if (i < pipeCommandCount - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }

            for (int j = 0; j < pipeCommandCount - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            char *token;
            char *argv[32]; 
            int argCount = 0;
            token = strtok(pipeCommands[i], " \t\n");
            while (token != NULL) {
                if (token[0] == '"' && token[strlen(token) - 1] == '"') {
                    token[strlen(token) - 1] = '\0';
                    argv[argCount++] = &token[1];
                } else {
                    argv[argCount++] = token;
                }
                token = strtok(NULL, " \t\n");
            }
            argv[argCount] = NULL;

            execvp(argv[0], argv);
            perror("Exec failed");
            exit(1);
        }
    }

    for (int i = 0; i < pipeCommandCount - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < pipeCommandCount; i++) {
        wait(NULL);
    }
}