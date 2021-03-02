#include <kdbg/trace.h>

const char HEX_CHARS[16] = "0123456789abcdef";

void trace_hex_print(u32 num) {
    u32 i = 0;
    char out[9];
    mem_set(out, '0', 8);
    out[8] = 0;
    while (num) {
        out[7 - i] = HEX_CHARS[num & 0x0f];
        num >>= 4;
        ++i;
    }
    kdbg_error(out);
}

void trace_print(void) {
    trace_frame_t *stack;
    __asm__ volatile ("movl %%ebp, %0;" : "=r" (stack) : : );
    kdbg_error("Stack trace: ");

    while (stack) {
        trace_hex_print(stack->eip);

        stack = stack->ebp;
    }
}
