#ifndef MYFIND_H
#define MYFIND_H

typedef struct {
    char *searchpath;
    char **filenames;
    int recursive;
    int case_insensitive;
    int numberOfFiles;
} Arguments;

Arguments parse_arguments(int argc, char *argv[]);
void find_files(Arguments arguments);

#endif // MYFIND_H
