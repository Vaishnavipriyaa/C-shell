#include "headers.h"

void handleError(const char *message)
{
    perror(message);
    exit(1);
}

struct termios originalTermios;

void disable()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1)
        handleError("tcsetattr");
}

void enable()
{
    if (tcgetattr(STDIN_FILENO, &originalTermios) == -1)
        handleError("tcgetattr");
    atexit(disable);
    struct termios rawTermios = originalTermios;
    rawTermios.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTermios) == -1)
        handleError("tcsetattr");
}

int getMostRecentProcessPID()
{
    FILE *loadavgFile = fopen("/proc/loadavg", "r");
    if (loadavgFile == NULL)
        handleError("fopen");

    float avg1, avg5, avg15;
    int runnableProcesses, totalProcesses, lastProcessPID;
    fscanf(loadavgFile, "%f %f %f %d/%d %d", &avg1, &avg5, &avg15, &runnableProcesses, &totalProcesses, &lastProcessPID);

    fclose(loadavgFile);
    return lastProcessPID;
}


void monitorAndHandleInput(int time_arg)
{
    if (time_arg <= 0)
    {
        fprintf(stderr, "Invalid time interval argument. Please provide a positive integer.\n");
        return;
    }

    int pipeFD[2];
    pid_t childProcessID = fork();
    pid_t recentProcessID = getMostRecentProcessPID();

    if (childProcessID == -1)
    {
        handleError("fork");
    }

    if (pipe(pipeFD) == -1)
    {
        handleError("pipe");
    }

    if (recentProcessID == 0)
        handleError("No processes found.");
    else if (childProcessID == 0)
    {
        close(pipeFD[0]);
        for (;;)
        {
            recentProcessID = getMostRecentProcessPID();
            printf("%d\n", recentProcessID);
            sleep(time_arg);
        }
    }
    else
    {
        close(pipeFD[1]);
        char inputChar;
        enable();
        for (;;)
        {
            if (read(STDIN_FILENO, &inputChar, 1) != 1)
                break;

            if (inputChar == 'x')
            {
                kill(childProcessID, SIGTERM);
                break;
            }
        }
        disable();
        close(pipeFD[0]);
    }
}
