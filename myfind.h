#ifndef MYFIND_H
#define MYFIND_H

void parse_arguments(int argc, char *argv[]);
void find_files(const char *searchpath, const char *filename, int recursive, int case_insensitive);

#endif // MYFIND_H
