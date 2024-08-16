#include "headers.h"

void handlePipes(const char *input) {
    char inputCopy[1024];
    strcpy(inputCopy, input);

    char *command;
    char *commands[32];  
    int commandCount = 0;

    command = strtok(inputCopy, "|");
    while (command != NULL) {
        commands[commandCount++] = command;
        command = strtok(NULL, "|");
    }

    int pipes[commandCount - 1][2]; 

    for (int i = 0; i < commandCount - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    for (int i = 0; i < commandCount; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {

            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }

            if (i < commandCount - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }

            for (int j = 0; j < commandCount - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execlp("/bin/sh", "sh", "-c", commands[i], NULL);
            perror("Exec failed");
            exit(1);
        }
    }

    for (int i = 0; i < commandCount - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < commandCount; i++) {
        wait(NULL);
    }
}
