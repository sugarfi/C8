#ifndef _C8_DEV
#define _C8_DEV

#include <lib/kstdio.h>

typedef enum {
    DEV_STDIN,
    DEV_STDOUT,
} dev_t;

void dev_read(dev_t num, char* buffer);
void dev_write(dev_t num, char* data);

#endif
