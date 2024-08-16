#include "headers.h"

void printTimeTakenInPrompt(time_t start_time) {
    time_t end_time;
    time(&end_time);
    double time_taken = difftime(end_time, start_time);

    if (time_taken > 2 && time_taken<15) {
        int rounded_time = (int)time_taken; // Round down the time
        printf("%d", rounded_time);
    }
}

void printTimeTaken(time_t start_time, const char *command) {
    time_t end_time;
    time(&end_time);
    double time_taken = difftime(end_time, start_time);

    if (time_taken > 2 && time_taken<100) {
        int rounded_time = (int)time_taken; // Round down the time
        printf("%s : %ds\n", command, rounded_time);
    }
}
