#ifndef _C8_USER_FILE
#define _C8_USER_FILE

#include <mem.h>

void file_read(int fd, char* out) {
    __asm__ __volatile__ ("int $0x80" : : "a" (0), "S" (fd), "D" ((unsigned int) out));
}

int file_open(char* name) {
    int fd;
    __asm__ __volatile__ ("int $0x80" : "=a" (fd) : "a" (2), "S" ((unsigned int) name));
    return fd;
}

#endif
