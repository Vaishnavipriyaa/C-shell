#include "headers.h"


typedef struct {
    int pid;
    char command[256];
    int state;
} BackgroundProcess;


static BackgroundProcess backgroundProcesses[100]; 
static int processCount = 0;

BackgroundProcess *getBackgroundProcesses() {
    return backgroundProcesses;
}

int getBackgroundProcessCount() {
    return processCount;
}

void addShellProcess(int pid, const char *command, int state) {
    for (int i = 0; i < processCount; i++) {
        if (backgroundProcesses[i].pid == pid) {
            
            backgroundProcesses[i].state = state;
            return; 
        }
    }
    

    if (processCount < sizeof(backgroundProcesses) / sizeof(backgroundProcesses[0])) {
        backgroundProcesses[processCount].pid = pid;
        snprintf(backgroundProcesses[processCount].command, sizeof(backgroundProcesses[0].command), "%s", command);
        backgroundProcesses[processCount].state = state;
        processCount++;
    } else {
        fprintf(stderr, "Background process limit exceeded.\n");
    }
}

int checkpid(int pid){
    int t;
    for (int i = 0; i < processCount; i++) {
        printf("%d\n",backgroundProcesses[i].pid);
        printf("pid-%d\n",pid);
        if (backgroundProcesses[i].pid == pid){
            t=i;
            }
            else{
                printf("pid not found\n");
            }
            }
            printf("%d\n",backgroundProcesses[t].state);
        if(backgroundProcesses[t].state==0){
                return 0;
        }
        if(backgroundProcesses[t].state==1){
            return 1;
        }
return 0;
    }


void updateBackgroundState(int pid, int state) {
    printf("inf\n");
 
    int t;
    for (int i = 0; i < processCount; i++) {
        printf("%d\n",backgroundProcesses[i].pid);
        printf("pid-%d\n",pid);
        if (backgroundProcesses[i].pid == pid){    
    backgroundProcesses[i].state =state; 
        }
    }
    printf("%d\n",state);
}

void listActivities() {
    BackgroundProcess *processes = getBackgroundProcesses();
    int processCount = getBackgroundProcessCount();

  
printf("Command Name\tPID\tState\n");
    for (int i = 0; i < processCount; i++) {
        if(backgroundProcesses[i].state!=-1){
    printf("%s\t%d\t", processes[i].command, processes[i].pid);
    
    if (backgroundProcesses[i].state==1){
        printf("Running\n");
    } else if(backgroundProcesses[i].state==0){
        printf("Stopped\n");
    }
 
    }
    }
}
