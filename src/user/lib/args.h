#ifndef _C8_USER_ARGS
#define _C8_USER_ARGS

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

char* args_nth(char* argv[], int argc, int index) {
    int i;
    int opts = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            opts++;
        } else {
            if ((i - opts) == index) {
                return argv[i];
            }
        }
    }
    return "\0";
}

bool args_option(char* argv[], int argc, char* opt) {
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            char* arg = argv[i];
            while (arg[0] == '-') {
                arg++;
            }
            return strcmp(arg, opt, strlen(opt));
        }
    }
    return false;
}

#endif
