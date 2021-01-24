#ifndef _C8_ALLOC
#define _C8_ALLOC

#include <info/ib.h>
#include <types.h>
#include <mem/mem.h>

static u16 allocated = 1; // I hate static variables with a passion but there is no better solution
static u32 alloc_base; // We allocate this in the kernel startup code
static bool alloc_lock = false;

typedef struct __attribute__((packed)) {
    u16 size : 12;
    u8 flags : 4;
} alloc_header_t;

void *alloc_alloc_page(void);
void alloc_free_page(void *);
void *alloc_alloc(u32);
void alloc_free(void *);

#endif
