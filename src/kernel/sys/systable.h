#ifndef _C8_SYSTABLE
#define _C8_SYSTABLE

#include <types.h>

typedef u32 (*systable_func_t)(u32, u32, u32, u32, u32, u32);

typedef struct {
    systable_func_t func;
    u8 flags;
    /*
     * -------P
     * - P: Present
     */
} systable_entry_t;

#endif
