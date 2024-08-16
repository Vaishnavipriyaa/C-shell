#include "headers.h"

void printPermissions(mode_t mode) {
    char permissions[11]; 
    
    permissions[0] = (S_ISDIR(mode)) ? 'd' : '-';
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
    
    printf("%s", permissions);
}


void runCommand(char *command) {
    int pid = fork();
    if (pid == 0) {
        char *args[32]; 
        int argc = 0;

        char *arg = strtok(command, " \t");
        while (arg != NULL) {
            args[argc++] = arg;
            arg = strtok(NULL, " \t");
        }
        args[argc] = NULL;

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
    }
}

void executeCommands(const char *input) {
    char inputCopy[100]; 
    strcpy(inputCopy, input);

    char *token = strtok(inputCopy, ";");
    while (token != NULL) {
        char *command = token;
        while (*command == ' ' || *command == '\t') {
            command++;
        }
        char *end = command + strlen(command) - 1;
        while (end > command && (*end == ' ' || *end == '\t')) {
            end--;
        }
        *(end + 1) = '\0';

        // Execute the command
        if (strlen(command) > 0) {
            int background = 0;

            if (command[strlen(command) - 1] == '&') {
                background = 1;
                command[strlen(command) - 1] = '\0';
            }

            int pid = fork();
            if (pid == 0) {
                char *args[32]; 
                int argc = 0;

                char *arg = strtok(command, " \t");
                while (arg != NULL) {
                    args[argc++] = arg;
                    arg = strtok(NULL, " \t");
                }
                args[argc] = NULL;

                execvp(args[0], args);
                perror("execvp");
                exit(1);
            } else if (pid > 0) {
                if (!background) {
                    waitpid(pid, NULL, 0);
                }
            } else {
                perror("fork");
            }
        }

        token = strtok(NULL, ";");
    }
}

