#include "headers.h"

void executeProclore(int pid) {
    char path[100];
    char status;
    char pstatus[10];
    int pgroup;
    long int vm;
    
    if (pid == -1) {
        pid = getpid();
        strcpy(path, "/proc/self/exe");
    } else {
        snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    }

    FILE *statusFile;
    char statusPath[100];
    snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", pid);
    statusFile = fopen(statusPath, "r");
    if (statusFile != NULL) {
        while (fgets(pstatus, sizeof(pstatus), statusFile)) {
            if (strncmp(pstatus, "State:", 6) == 0) {
                status = pstatus[7];
                break;
            }
        }
        fclose(statusFile);
    } else {
        perror("fopen");
        return;
    }

    printf("pid : %d\n", pid);
    printf("process status : %c", status);
    if (status == 'R' || status == 'S' || status == 'Z') {
        if (pid == getpid()) {
            printf("+");
        }
        printf("\n");
    }
    
    snprintf(statusPath, sizeof(statusPath), "/proc/%d/stat", pid);
    statusFile = fopen(statusPath, "r");
    if (statusFile != NULL) {
        fscanf(statusFile, "%*d %*s %*c %d", &pgroup);
        fclose(statusFile);
        printf("Process Group : %d\n", pgroup);
    } else {
        perror("fopen");
    }

    snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", pid);
    statusFile = fopen(statusPath, "r");
    if (statusFile != NULL) {
        while (fgets(pstatus, sizeof(pstatus), statusFile)) {
            if (strncmp(pstatus, "VmSize:", 7) == 0) {
                sscanf(pstatus + 8, "%ld", &vm);
                break;
            }
        }
        fclose(statusFile);
        printf("Virtual memory : %ld\n", vm);
    } else {
        perror("fopen");
    }

    char exe_path[512];
    if (readlink(path, exe_path, sizeof(exe_path)) != -1) {
        printf("executable path : %s\n", exe_path);
    } else {
        perror("readlink");
    }
}