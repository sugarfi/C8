#ifndef _C8_IB
#define _C8_IB

#include <types.h>
#define IB_RES (((u32 *) 0x3ff)[4])

typedef struct __attribute__((packed)) {
    u32 mbr;
    u32 pd;
    u32 idt;
    u32 systable;
    u32 res;
    u32 allocated[32];
} ib_t;

#endif
