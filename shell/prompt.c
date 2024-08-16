#include "headers.h"


char* get_relative_path(const char* home, const char* path) 
{
    if (strncmp(path, home, strlen(home)) == 0) 
    {
        char* relative_path = (char*)malloc(sizeof(char)*512);
        snprintf(relative_path, 512, "~%s", path + strlen(home));
        return relative_path;
    } 
        return strdup(path);
}


void prompt_2(char* home, time_t command_start_time, char* command) {
    uid_t userid = getuid();
    struct passwd *info = getpwuid(userid);
    const char *username = info->pw_name; 

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    printf("<%s@%s:", username, hostname);
    
    char* cwd = (char*)malloc(sizeof(char) * 512);             
    getcwd(cwd, 512);

    char* new1 = get_relative_path(home, cwd);
    printf("%s", new1);

    if (command_start_time != 0) {
        time_t end_time;
        time(&end_time);
        double time_taken = difftime(end_time, command_start_time);

        if (time_taken>2 && time_taken<15) {
            int rounded_time = (int)time_taken; 
            printf(" sleep: (%ds)",rounded_time);
        }
    }
    
    printf(">");
}
