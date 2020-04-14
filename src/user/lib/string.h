#ifndef _C8_USER_STRING
#define _C8_USER_STRING

#include <stdbool.h>

bool strcmp(char* a, char* b, int n) {
    int i = 0;
    while (a[i]) {
        if (b[i] == '\0') {
            return true;
        }
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return true;
}

void memcpy(char* a, char* b, int n) {
    int i = 0;
    while (i < n) {
        a[i] = b[i];
        i++;
    }
}

int strlen(char* s) {
    int i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

#endif
