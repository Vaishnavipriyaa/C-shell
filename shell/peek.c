#include "headers.h"

char getFileType(mode_t mode) {
    if (S_ISREG(mode)) {
        return 'F'; 
    } else if (S_ISDIR(mode)) {
        return 'D'; 
    } else if (mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH) {
        return 'E'; 
    }
    return '?';
}

void printColored(const char *text, char type) {
    switch (type) {
        case 'D':
            printf("\x1b[34m%s\x1b[0m\n", text);
            break;
        case 'E':
            printf("\x1b[32m%s\x1b[0m\n", text);
            break;
        default:
            printf("%s\n", text);
    }
}

void executePeek(const char *path, int a_flag, int l_flag) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !a_flag) {
            continue;
        }

        struct stat st;
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        lstat(full_path, &st);

        char type = getFileType(st.st_mode);
        if (l_flag) {
            struct passwd *user_info = getpwuid(st.st_uid);
            struct group *group_info = getgrgid(st.st_gid);
            printPermissions(st.st_mode);
            printf(" %ld %s %s %lld\n", (long)st.st_nlink, user_info->pw_name, group_info->gr_name, (long long)st.st_size);
            printColored(entry->d_name, type);
        } else {
            printColored(entry->d_name, type);
        }
    }
    closedir(dir);
}