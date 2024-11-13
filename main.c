#include <stdio.h>
#include <limits.h>
#include "myfind.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-R] [-i] searchpath filename1 [filename2 ...]\n", argv[0]);
        return 1;
    }
    const Arguments arguments = parse_arguments(argc, argv);
    find_files(arguments);
    

    return 0;
}
