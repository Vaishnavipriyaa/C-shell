#include "headers.h"

void executeWarp(char* home, const char* command_i) {
    char previousDirectory[1024];
    char path[1024];

    if ((strcmp(command_i, "") == 0) && (strncmp(command_i, "/", 2) != 0)) {
        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);

        strcpy(previousDirectory, currentDir);

        chdir(home);
        printf("%s\n", home);
    }
    else if (strcmp(command_i, "-") == 0) {
        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);

        char* temp = (char*)malloc(sizeof(char) * 512);

        strcpy(temp, previousDirectory);
        strcpy(previousDirectory, currentDir);
        strcpy(currentDir, temp);

        chdir(currentDir);

        printf("%s\n", currentDir);
    }
    else if (strcmp(command_i, "..") == 0) {
        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);
        strcpy(previousDirectory, currentDir);

        chdir("..");
        getcwd(currentDir, 512);
        printf(" %s\n", currentDir);
    }
    else if (strcmp(command_i, ".") == 0) {
        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);
        strcpy(previousDirectory, currentDir);
        chdir(currentDir);

        printf("%s\n", currentDir);
    }
    else if (strncmp(command_i, "~/", 2) == 0) {
        char *dir = malloc(sizeof(char) * 512);
        strcpy(dir, command_i + 2);
        char *expandedDir = malloc(sizeof(char) * 512);
        snprintf(expandedDir, 512, "%s/%s", home, dir);
        printf("%s\n", expandedDir);

        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);

        strcpy(previousDirectory, currentDir);
        chdir(expandedDir);
    }
    else {
        char *dir = malloc(sizeof(char) * 512);
        strcpy(dir, command_i);
        char* currentDir = (char*)malloc(sizeof(char) * 512);
        getcwd(currentDir, 512);
        strcpy(previousDirectory, currentDir);
        char *expandedDir = malloc(sizeof(char) * 512);
        snprintf(expandedDir, 512, "%s/%s", currentDir, dir);

        printf("%s\n", expandedDir);
        chdir(expandedDir);
    }
}

void executeWarpCommands(char* home, char* commands[], int count) {
    if (count == 1) {
        chdir(home);
        printf("%s\n", home);
    }
    else if (count > 1) {
        for (int i = 1; i < count; i++) {
            executeWarp(home, commands[i]);
        }
    }
}
