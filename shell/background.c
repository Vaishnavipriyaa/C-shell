#include "headers.h"

typedef struct {
    pid_t pid;
    char name[100];
    int status; 
} BackgroundProcess;

BackgroundProcess backgroundProcesses[32]; 
int numBackgroundProcesses = 0;


void checkBackgroundProcesses() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < numBackgroundProcesses; i++) {
            if (backgroundProcesses[i].pid == pid) {
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    backgroundProcesses[i].status = 0; 
                } else {
                    backgroundProcesses[i].status = 1; 
                }
                break;
            }
        }
    }
}



void executeBackground(char *command) {
    char *args[MAX_LINE / 2 + 1];
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    int runInBackground = 0;
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        args[i - 1] = NULL; 
        runInBackground = 1;
    }

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        execvp(args[0], args); 
        perror("Exec failed"); 
        exit(1);
    } else {
        if (runInBackground) {
            printf("[%d] %s (PID: %d) &\n", numBackgroundProcesses + 1, args[0], pid);
            strcpy(backgroundProcesses[numBackgroundProcesses].name, args[0]);
            backgroundProcesses[numBackgroundProcesses].pid = pid;
            backgroundProcesses[numBackgroundProcesses].status = -1; // Status not known yet
            numBackgroundProcesses++;
        }
    }
}


