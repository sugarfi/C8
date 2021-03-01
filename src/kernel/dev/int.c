#include <dev/int.h>
#include <kdbg/trace.h>

// Generate an ISR handler
#define MAKE_ISR(N) \
    __attribute__((interrupt)) void isr_##N(void* frame) {\
        kdbg_error("Recieved ISR " #N); \
        trace_print(); \
        for(;;);\
    }

#define MAKE_ISR_CODE(N) \
    __attribute__((interrupt)) void isr_##N(void* frame, u32 code) {\
        kdbg_error("Recieved ISR " #N); \
        u32 a; \
        __asm__ volatile ("movl %%eax, %0" : "=r" (a) : : ); \
        trace_hex_print(a); \
        trace_print(); \
        for(;;);\
    }

// Stores an ISR handler in the IDT
#define STORE_ISR(N) \
    isr_f_t isr_f_##N = &isr_##N; \
    idt_entry_t isr_entry_##N = { \
        .offset_lo = ((u32) isr_f_##N) & 0xffff, \
        .selector = 0x08, \
        .zero = 0, \
        .type = 0b10001110, \
        .offset_hi = ((u32) isr_f_##N) >> 16 \
    }; \
    idt[N] = isr_entry_##N;

#define STORE_ISR_CODE(N) \
    isr_f_code_t isr_f_##N = &isr_##N; \
    idt_entry_t isr_entry_##N = { \
        .offset_lo = ((u32) isr_f_##N) & 0xffff, \
        .selector = 0x08, \
        .zero = 0, \
        .type = 0b10001110, \
        .offset_hi = ((u32) isr_f_##N) >> 16 \
    }; \
    idt[N] = isr_entry_##N;

MAKE_ISR(0)
MAKE_ISR(1)
MAKE_ISR(2)
MAKE_ISR(3)
MAKE_ISR(4)
MAKE_ISR(5)
MAKE_ISR(6)
MAKE_ISR(7)
MAKE_ISR_CODE(8)
MAKE_ISR(9)
MAKE_ISR_CODE(10)
MAKE_ISR_CODE(11)
MAKE_ISR_CODE(12)
MAKE_ISR_CODE(13)
MAKE_ISR_CODE(14)
MAKE_ISR(15)
MAKE_ISR(16)
MAKE_ISR_CODE(17)
MAKE_ISR(18)
MAKE_ISR(19)
MAKE_ISR(20)
MAKE_ISR(21)
MAKE_ISR(22)
MAKE_ISR(23)
MAKE_ISR(24)
MAKE_ISR(25)
MAKE_ISR(26)
MAKE_ISR(27)
MAKE_ISR(28)
MAKE_ISR(29)
MAKE_ISR_CODE(30)
MAKE_ISR(31)

void idt_load(idt_desc_t *desc) {
    /*
     * Uses assembly to load the IDT.
     */
    __asm__ volatile(
        "lidt (%0)\n"
        : : "r" (desc)
        );    
}

void idt_fill(idt_entry_t idt[256]) {
    /*
     * Fills the IDT with the generated ISRs.
     */
    STORE_ISR(0)
    STORE_ISR(1)
    STORE_ISR(2)
    STORE_ISR(3)
    STORE_ISR(4)
    STORE_ISR(5)
    STORE_ISR(6)
    STORE_ISR(7)
    STORE_ISR_CODE(8)
    STORE_ISR(9)
    STORE_ISR_CODE(10)
    STORE_ISR_CODE(11)
    STORE_ISR_CODE(12)
    STORE_ISR_CODE(13)
    STORE_ISR_CODE(14)
    STORE_ISR(15)
    STORE_ISR(16)
    STORE_ISR_CODE(17)
    STORE_ISR(18)
    STORE_ISR(19)
    STORE_ISR(20)
    STORE_ISR(21)
    STORE_ISR(22)
    STORE_ISR(23)
    STORE_ISR(24)
    STORE_ISR(25)
    STORE_ISR(26)
    STORE_ISR(27)
    STORE_ISR(28)
    STORE_ISR(29)
    STORE_ISR_CODE(30)
    STORE_ISR(31)
}
