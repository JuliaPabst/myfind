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
    // define arguments struct with default values
    Arguments arguments = {NULL, NULL, 0, 0, 0};

    // allocate memory for filenames (one fewer than arguments because of path)
    arguments.filenames = malloc((argc -1) * sizeof(char *));
    for (int i = 1; i < argc; i++) {
        // allocate different words to right values in arguments struct
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

void search_file(const char *searchpath, const char *filename, Arguments* arguments) {
    // try to open directory and check if it could be opened 
    DIR *dir = opendir(searchpath);
    if (dir == NULL) {
        perror("Could not open directory");
        exit(1);
    }

    // predefine an entry and loop over all entries in directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        // ignore . and .. directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // choose the right comparison function based on case_insensitive flag
        int match = arguments->case_insensitive ? strcasecmp(entry->d_name, filename) == 0 : strcmp(entry->d_name, filename) == 0;

        if (match) {
            // combine directory path and filename to one string
            char relative_path[PATH_MAX];
            snprintf(relative_path, sizeof(relative_path), "%s/%s", searchpath, entry->d_name);

            // print the pid, filename and absolute path is realpath exists
            char absolute_path[PATH_MAX];
            if (realpath(relative_path, absolute_path) != NULL) {
                printf("%d: %s: %s\n", getpid(), filename, absolute_path);
            } else {
                perror("Error resolving absolute path");
            }
        }

        // recursively search subdirectories if recursive flag is set
        if (arguments->recursive && entry->d_type == DT_DIR) {
            // combine directory path and filename to one string
            char subdir_path[PATH_MAX];
            snprintf(subdir_path, sizeof(subdir_path), "%s/%s", searchpath, entry->d_name);
            search_file(subdir_path, filename, arguments);
        }
    }
    
    closedir(dir);
}

void find_files(Arguments arguments) {
     for (int i = 0; i < arguments.numberOfFiles; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // if fork is a child process: search file 
            search_file(arguments.searchpath, arguments.filenames[i], &arguments);
            exit(0);
        }
    }

    // parent process waits for all children to finish 
    for (int i = 0; i < arguments.numberOfFiles; i++) {
        wait(NULL);
    }

    free(arguments.filenames);
}
