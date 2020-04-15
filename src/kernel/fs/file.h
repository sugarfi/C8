/*
 * file.h - declarations for file types and basic operations.
*/
#ifndef _C8_FILE
#define _C8_FILE

#include <lib/string.h>
#include <sys/log.h>

#define FILE_READONLY 1<<0 // File flags
#define FILE_EXEC     1<<1
#define FILE_ISDIR    1<<2
#define FILE_ISDEV    1<<3

#define FILE_LIMIT 1024 // Only 1024 files can be open at once

#define FILE_SIZE 512 // The size of one file metadata entry - 32 bytes
#define FILE_ADDR 0x2fe00 // The base address where files are loaded

typedef struct
{
    unsigned char name[100];
    unsigned char mode[8];
    unsigned char uid[8];
    unsigned char gid[8];
    unsigned char size[12];
    unsigned char time[12];
    unsigned char checksum[8];
    unsigned char flag;
    unsigned char linked[100];
    unsigned char ustar[6];
    unsigned char version[2];
    unsigned char owner[32];
    unsigned char group[32];
    unsigned char major[8];
    unsigned char minor[8];
    unsigned char prefix[155];
} __attribute__((packed)) file_t; // A raw file metadata entry


int file_next_fd = 0; // The next file descriptor to assign

int open[FILE_LIMIT]; // The array of open files


file_t file_get(int addr);

#endif
