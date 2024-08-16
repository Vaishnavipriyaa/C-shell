#include "headers.h"

int move_to_stopfromrun(int pid) {
    if (kill(pid, SIGSTOP) == -1) {
        return 0;
    } else {
        return 1;
    }
}

int pingProcess(int pid, int signal_number) {
    int signalToSend = signal_number % 32; 

    if (kill(pid, 0) == 0) {
        if (kill(pid, signalToSend) == 0) {
            return 1;
        } else {
            perror("kill");
        }
    } else {
        return 0;
    }
}