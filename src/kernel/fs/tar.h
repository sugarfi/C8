#ifndef _C8_TAR
#define _C8_TAR

#include <types.h>
#include <dev/atapio.h>
#include <mem/alloc.h>
#include <mem/mem.h>

typedef struct __attribute__((packed)) {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mod_time[12];
    char checksum[8];
    char type;
    char linked[100];
    char ustar[6];
    char version[2];
    char uname[32];
    char gname[32];
    char major[8];
    char minor[8];
    char prefix[155];
} tar_file_t;

u32 oct2bin(char *, u32);
tar_file_t *tar_get_file(char *, u32);
bool tar_read(char *, u32, char *);
bool tar_write(char *, char *, u32, u32);
bool tar_read_dir(char *, u32, tar_file_t **);
u32 tar_count(char *, u32);

#endif
