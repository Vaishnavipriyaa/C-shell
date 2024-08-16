#include "headers.h"

int main()
{
    int isCtrlZPressed = 0;
    int isCtrlCPressed = 0;
    int HISTORY_SIZE = 15;
    char home[1024];

    char username[100];
    char hostname[100];
    char current_dir[100];
    HistoryEntry history[HISTORY_SIZE];
    int historyCount = 0;
    getcwd(home, sizeof(home));
    time_t command_start_time;

    int foregroundProcessPid = -1;
    int foregroundProcessState = -1;

    int background_process = 0;
    int should_run = 1;

    int backgroundPIDs[MAX_BACKGROUND_PROCESSES];
    memset(backgroundPIDs, -1, sizeof(backgroundPIDs));

    while (should_run)
    {

        char *command1 = NULL;
        getlogin_r(username, sizeof(username));
        gethostname(hostname, sizeof(hostname));
        getcwd(current_dir, sizeof(current_dir));
        prompt_2(home, command_start_time, command1);
        command_start_time = 0;
        char input[100];
        char temp[100];
        char temp1[100];
        char temp2[100];
        char *result =
            fgets(input, sizeof(input), stdin);
        if (result == NULL)
        {
            exit(0);
        }
        if (input[0] == '\n')
        {
            continue;
        }

        input[strlen(input) - 1] = '\0';
        strcpy(temp, input);
        strcpy(temp1, input);
        strcpy(temp2, input);

        for (int i = 0; input[i]; i++)
        {
            if (input[i] == '"')
            {
                input[i] = ' ';
            }
        }
        
        int background = 0;
        char *background_symbol = strstr(temp1, "&");
        if (background_symbol != NULL)
        {
            background = 1;
            *background_symbol = ' ';
        }

        if (background == 1)
        {
            checkBackgroundProcesses();
        }
        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
            continue;
        }

        if (strncmp(input, "neonate -n ", 11) == 0)
        {
            int time_arg;
            if (sscanf(input + 11, "%d", &time_arg) == 1)
            {
                if (time_arg <= 0)
                {
                    printf("Invalid Time\n");
                }
                else
                {
                       monitorAndHandleInput(time_arg); 
                }
            }
            else
            {
                fprintf(stderr, "Usage: neonate -n <time_arg>\n");
            }
        }

        char *command = strtok(temp, ";");
        if (command != NULL)
        {
            printf("here\n");
            support_semicolon(temp);
        }

        char *pipe_token = strchr(temp, '|');

        if (pipe_token != NULL)
        {
            handlePipes(temp);
        }
        if (strstr(input, "|") != NULL && (strstr(input, "<") != NULL || strstr(input, ">") != NULL || strstr(input, ">>") != NULL))
        {
            handlePipesAndRedirection(input);
        }
        if (strcmp(input, "activities") == 0)
        {
            listActivities();
        }
        else
        {

            if (strcmp(input, "pastevents") != 0 && strncmp(input, "pastevents execute", 18) != 0)
            {
                addHistory(history, &historyCount, input);
            }

            char *args[32];
            char *arg = strtok(input, " ");
            int arg_count = 0;
            while (arg != NULL)
            {
                args[arg_count++] = arg;
                arg = strtok(NULL, " ");
            }
            args[arg_count] = NULL;


            if (arg_count > 1 && strcmp(args[arg_count - 1], "&") == 0)
            {
                background = 1;
                args[arg_count - 1] = NULL;
            }

            if (strcmp(args[0], "warp") == 0)
            {
                command_start_time = 0;
                if (arg_count == 1)
                {
                    continue;
                }
                if (arg_count >= 2)
                {
                    executeWarpCommands(home, args, arg_count);
                }
                else
                {
                    fprintf(stderr, "Usage: warp <path>\n");
                }
            }

            else if (strcmp(args[0], "iMan") == 0)
            {
                fetchManPage(args[1]);
            }

            else if (strcmp(args[0], "peek") == 0)
            {
                command_start_time = 0;
                int a_flag = 0;
                int l_flag = 0;
                const char *path = current_dir;
                for (int i = 1; i < arg_count; i++)
                {
                    if (args[i][0] == '-')
                    {
                        if (strcmp(args[i], "-a") == 0)
                        {
                            a_flag = 1;
                        }
                        if (strcmp(args[i], "-l") == 0)
                        {
                            l_flag = 1;
                        }
                        if (strcmp(args[i], "-al") == 0)
                        {
                            l_flag = 1;
                            a_flag = 1;
                        }
                        if (strcmp(args[i], "-la") == 0)
                        {
                            l_flag = 1;
                            a_flag = 1;
                        }
                        if (strcmp(args[i], "-l-a") == 0)
                        {
                            l_flag = 1;
                            a_flag = 1;
                        }
                        if (strcmp(args[i], "-a-l") == 0)
                        {
                            l_flag = 1;
                            a_flag = 1;
                        }
                    }
                    else
                    {
                        path = args[i];
                    }
                }

                executePeek(path, a_flag, l_flag);
            }

            else if (strcmp(args[0], "pastevents") == 0)
            {

                if (arg_count == 1)
                {
                    for (int i = historyCount - 1; i >= 0; i--)
                    {
                        printf("[%d] %s\n", history[i].index, history[i].command);
                    }
                }
                else if (strcmp(args[1], "execute") == 0 && arg_count >= 3)
                {

                    int index = atoi(args[2]);
                    if (index > 0 && index <= historyCount)
                    {
                        printf("Executing: %s\n", history[index - 1].command);
                        runCommand(history[index - 1].command);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid history index\n");
                    }
                }
                else if (strcmp(args[1], "purge") == 0)
                {
                    historyCount = 0;
                }
                else
                {
                    fprintf(stderr, "Invalid pastevents command\n");
                }
            }

            if (strncmp(input, "ping", 4) == 0)
            {
                char *arguments = temp1 + 4;
                char *toki = strtok(arguments, " ");
                char *argst[4];
                int num_to = 0;
                while (toki != NULL)
                {
                    argst[num_to++] = toki;
                    toki = strtok(NULL, " ");
                }

                if (num_to != 2)
                {
                    printf("Error: Missing Arguments\n");
                }
                else
                {
                    int pid2 = fork();
                    int pidof = atoi(argst[0]);
                    int sig = atoi(argst[1]);
                    sig = sig % 32;
                    if (sig == 9)
                    {
                        addShellProcess(pidof, args[0], -1);
                    }
                    if (sig == 19)
                    {
                        addShellProcess(pidof, args[0], 0);
                        if (move_to_stopfromrun(pidof) == 1)
                        {
                            printf("Moved process with PID %d to Stopped state.\n", pidof);
                        }
                        else
                        {
                            perror("Failed to move process to Stopped state");
                        }
                    }
                    if (sig == 18)
                    {
                        addShellProcess(pidof, args[0], 1);
                    }
                    if (pingProcess(pidof, sig) == 1)
                    {
                        printf("Sent signal %d to process with pid %d\n", sig, pidof);
                    }
                    else
                    {
                        printf("No such process found\n");
                    }
                }
                continue;
            }

            else if (strcmp(args[0], "proclore") == 0)
            {
                if (arg_count == 1)
                {
                    executeProclore(-1);
                }
                else if (arg_count == 2)
                {
                    int pid = atoi(args[1]);
                    if (pid > 0)
                    {
                        executeProclore(pid);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid pid\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Usage: proclore [pid]\n");
                }
            }
            else if (strcmp(args[0], "seek") == 0)
            {
                int d_flag = 0;
                int f_flag = 0;
                int e_flag = 0;
                const char *search = args[1];
                const char *target_directory = current_dir;
                for (int i = 2; i < arg_count; i++)
                {
                    if (strcmp(args[i], "-d") == 0)
                    {
                        d_flag = 1;
                    }
                    else if (strcmp(args[i], "-f") == 0)
                    {
                        f_flag = 1;
                    }
                    else if (strcmp(args[i], "-e") == 0)
                    {
                        e_flag = 1;
                    }
                    else
                    {
                        target_directory = args[i];
                    }
                }
            }

            if (strcmp(input, "pastevents") == 0 || strcmp(input, "peek") == 0 || strcmp(input, "warp") == 0 || strcmp(input, "iMan") == 0 || strcmp(input, "neonate") == 0)
            {
                continue;
            }
            else
            {
                int pid = fork();

                time_t start_time;
                time(&start_time);

                if (strstr(temp2, "&") != NULL && pid > 0)
                {
                    int pid1;
                    background = 1;

                    int flag;
                    if (background_process <= 2)
                    {
                        if (background_process < 1)
                        {
                            flag = 0;
                            pid1 = pid;
                            addShellProcess(pid, args[0], 1);
                        }
                        background_process++;

                        command_start_time = 0;

                        if (background_process == 2)
                        {

                            flag = 1;

                            addShellProcess(pid1, args[0], -1);

                            addShellProcess(pid, args[0], 1);

                            printf("%d\n", pid);
                            printf("%s exited normally(%d)\n", input, pid1);
                            pid1 = pid;
                            background_process--;
                        }
                        if (flag == 0)
                        {
                            printf("%d\n", pid);
                        }
                    }
                }
                else
                {
                    if (pid == 0)
                    {
                        handleRedirection(temp);
                        execvp(args[0], args);
                        perror("execvp");
                        exit(1);
                    }
                    else if (pid > 0)
                    {
                        if (!background)
                        {
                            waitpid(pid, NULL, 0);
                            command_start_time = start_time;
                            command1 = input;
                        }
                        else
                        {
                            command_start_time = 0;
                            addShellProcess(pid, args[0], 1);
                        }
                    }
                    else
                    {
                        perror("fork");
                    }
                }
                if (strcmp(args[0], "bg") == 0)
                {
                    int targetPid = atoi(args[1]);
                    printf("2\n");
                    if (checkpid(targetPid) == 0)
                    {
                        printf("1\n");
                        updateBackgroundState(targetPid, 1);
                        listActivities();
                        printf("updated\n");
                    }
                    else
                    {
                        printf("Usage: bg <pid>\n");
                    }
                }
            }
        }
    }
    return 0;
}
