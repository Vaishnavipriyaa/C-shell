#include "headers.h"

void addHistory(HistoryEntry history[], int *historyCount, const char *command) {
    int HISTORY_SIZE = 15;

    if (*historyCount > 0 && strcmp(command, history[*historyCount - 1].command) == 0) {
        return; 
    }

    HistoryEntry newEntry;
    strncpy(newEntry.command, command, sizeof(newEntry.command) - 1);
    newEntry.command[sizeof(newEntry.command) - 1] = '\0'; // Ensure null-terminated
    newEntry.index = *historyCount + 1;

    if (*historyCount == HISTORY_SIZE) {
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strncpy(history[i].command, history[i + 1].command, sizeof(history[i].command) - 1);
            history[i].command[sizeof(history[i].command) - 1] = '\0'; // Ensure null-terminated
            history[i].index = history[i + 1].index - 1;
        }
        (*historyCount)--;
    }

    strncpy(history[*historyCount].command, newEntry.command, sizeof(history[*historyCount].command) - 1);
    history[*historyCount].command[sizeof(history[*historyCount].command) - 1] = '\0'; // Ensure null-terminated
    history[*historyCount].index = newEntry.index;

    (*historyCount)++;
}



void printHistory(HistoryEntry history[], int historyCount) {
    for (int i = historyCount - 1; i >= 0; i--) {
        printf("[%d] %s\n", history[i].index, history[i].command);
    }
}

