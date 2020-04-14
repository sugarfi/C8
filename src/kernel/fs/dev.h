#ifndef _C8_DEV
#define _C8_DEV

#include <fs/file.h>
#include <lib/kstdio.h>

typedef void(*dev_write_t)(char*);
typedef void(*dev_read_t)(char*);

typedef struct {
    int fd;
    dev_write_t write;
    dev_read_t read;
} dev_t;

static dev_t devs[FILE_LIMIT];

void dev_nop(char* data) {
    return;
}

void dev_init(int fd, dev_t dev) {
    devs[fd] = dev;
    open[fd] = (int) &dev;
    file_next_fd = fd + 1;
}

#endif
