#include "headers.h"
 pid_t bg_processes[MAX_LINE / 2];
void launchBackgroundProcess(char *args[], char *input) {
    // Background process
    int bg_process_count=0;
    pid_t bg_pid = fork(); /* Fork a child process for background command */
    if (bg_pid < 0) {
        perror("Background fork");
    } else if (bg_pid == 0) {
        // Child process for background command
        handleRedirection(input);
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        // Parent process
        bg_processes[bg_process_count++] = bg_pid; /* Store the PID of the background process */
        printf("%d\n", bg_pid);
    }
}