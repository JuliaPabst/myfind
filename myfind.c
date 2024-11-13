#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "myfind.h"

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

void find_files(Arguments arguments) {
    if (arguments.searchpath == NULL || arguments.numberOfFiles == 0) {
        printf("Searchpath or filename is NULL\n");
        return;
    }
    
    DIR *dir = opendir(arguments.searchpath);
    if (dir == NULL) {
        perror("Could not open directory");
        return;
    }
    
    closedir(dir);
}
