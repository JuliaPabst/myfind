#define _GNU_SOURCE
#include "myfind.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

Arguments parse_arguments(int argc, char *argv[]) {
    Arguments arguments = {NULL, NULL, 0, 0, 0};

    arguments.filenames = malloc(argc * sizeof(char *));
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] == '.' && argv[i][1] == '/'){
            arguments.searchpath = argv[i];  
        } else if(argv[i][0] == '-' && argv[i][1] == 'R' && argv[i][2] == '\0'){
            arguments.recursive = 1;  
        } else if(argv[i][0] == '-' && argv[i][1] == 'i' && argv[i][2] == '\0'){
            arguments.case_insensitive = 1;  
        } else {
            arguments.filenames[arguments.numberOfFiles] = argv[i];
            arguments.numberOfFiles++;
        }
    }

    return arguments;
}

void search_file(const char *searchpath, const char *filename) {
    DIR *dir = opendir(searchpath);
    if (dir == NULL) {
        perror("Could not open directory");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Check if the filename matches
        if (strcmp(entry->d_name, filename) == 0) {
            // Construct the relative path to the found file
            char relative_path[PATH_MAX];
            snprintf(relative_path, sizeof(relative_path), "%s/%s", searchpath, entry->d_name);

            // Get the absolute path
            char absolute_path[PATH_MAX];
            if (realpath(relative_path, absolute_path) != NULL) {
                // Output the PID and absolute path
                printf("%d: %s: %s\n", getpid(), filename, absolute_path);
            } else {
                perror("Error resolving absolute path");
            }
            closedir(dir);
            exit(0);
        }
    }
    closedir(dir);
    exit(1); 
}

void find_files(Arguments arguments) {
     for (int i = 0; i < arguments.numberOfFiles; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // child process
            search_file(arguments.searchpath, arguments.filenames[i]);
        }
    }

    // parent process waits for all children to finish 
    for (int i = 0; i < arguments.numberOfFiles; i++) {
        wait(NULL);
    }

    free(arguments.filenames);
}
