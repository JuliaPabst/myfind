#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "myfind.h"

void parse_arguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
}

void find_files(const char *searchpath, const char *filename, int recursive, int case_insensitive) {
    DIR *dir = opendir(searchpath);
    if (dir == NULL) {
        perror("Could not open directory");
        return;
    }
    closedir(dir);
}
