#ifndef _C8_FILE
#define _C8_FILE

#include <lib/math.h>

#define FILE_SIZE(f) (oct2bin(f->size, 11))

typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char time[12];
    char checksum[8];
    char type;
    char linked[100];
    char ustar[6];
    char version[2];
    char owner[32];
    char user[32];
    char major[8];
    char minor[8];
    char prefix[155];
} __attribute__((packed)) file_raw_t;

typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char time[12];
    char checksum[8];
    char type;
    char linked[100];
    char ustar[6];
    char version[2];
    char owner[32];
    char user[32];
    char major[8];
    char minor[8];
    char prefix[155];
    char* content;
} __attribute__((packed)) file_t;

#define FILE_ADDR 0x2fe00

#endif
