#ifndef _C8_TRACE
#define _C8_TRACE

#include <kdbg/kdbg.h>
#include <types.h>
#include <mem/mem.h>

typedef struct __attribute__((packed)) trace_frame_t {
    struct trace_frame_t *ebp;
    u32 eip;
} trace_frame_t;

void trace_hex_print(u32);
void trace_print(void);

#endif
