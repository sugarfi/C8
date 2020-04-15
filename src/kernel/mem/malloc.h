#ifndef _C8_MALLOC
#define _C8_MALLOC

#include <stdbool.h>

typedef struct {
    unsigned int addr;
    unsigned int size;
    bool free;
} malloc_block_t;

void* kmalloc(int size);

void kfree(void* ptr);

#endif
