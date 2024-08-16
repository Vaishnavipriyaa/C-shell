#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h> 
#include <ctype.h>
#include <signal.h>
#include <termios.h>
#include <dirent.h> 
#include <unistd.h>
#include <sys/ptrace.h>
#include <netdb.h>
#include <arpa/inet.h>


#define MAX_LINE 80 
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 64
#define MAX_BG_PROCESSES 100 
#define MAX_PROCESSES 100
#define PTRACE_ATTACH 16
#define RUNNING 1
#define STOPPED 0
#define MAX_BACKGROUND_PROCESSES 100
#define STATE_RUNNING 1
#define STATE_STOPPED 2
#define MAX_BUFFER_SIZE 1024



typedef struct {
    char command[MAX_COMMAND_LENGTH];
    int index;
} HistoryEntry;

//activities
int getBackgroundProcessCount();
void addShellProcess(int pid, const char *command, int state);
int checkpid(int pid);
void updateBackgroundState(int pid, int state);
void listActivities();

//prompt
char* get_relative_path(const char* home, const char* path);
void prompt_2(char* home, time_t command_start_time, char* command) ;

//semicolon
void support_semicolon(char* command);

//background

void checkBackgroundProcesses();
void executeBackground(char *command);

//signal
void launchBackgroundProcess(char *args[], char *input);

//ping

int move_to_stopfromrun(int pid);
int pingProcess(int pid, int signal_number);

//execute

void printPermissions(mode_t mode);
void runCommand(char *command);
void executeCommands(const char *input);

//iman

void die(const char* message);
int connectToServer(const char* ip_address, int port);
void fetchManPage(const char* command_name);

//in_out redirection
void handleRedirection(char *input);

//neonate

void handleError(const char *message);
void disable();
void enable();
int getMostRecentProcessPID();
void monitorAndHandleInput(int time_arg);

//pastevents
void addHistory(HistoryEntry history[], int *historyCount, const char *command);
void printHistory(HistoryEntry history[], int historyCount);

//peek
char getFileType(mode_t mode);
void printColored(const char *text, char type);
void executePeek(const char *path, int a_flag, int l_flag);

//pipes
void handlePipes(const char *input);

//proclore

void executeProclore(int pid);

//redirection&pipes
void handleInputRedirection(char *filename);
void handleOutputRedirection(char *filename, int append);
void handlePipesAndRedirection(char *input);

//seek
void executeSeek(const char *search, const char *target_directory, char *current_dir);

//system
void printTimeTakenInPrompt(time_t start_time);
void printTimeTaken(time_t start_time, const char *command);

//warp
void executeWarp(char* home, const char* command_i);
void executeWarpCommands(char* home, char* commands[], int count);

#endif