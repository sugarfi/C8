#include <dev/int.h>
#include <kdbg/trace.h>

extern void idt_isr_int_handler(void);

void idt_isr_handler(u32 err, u32 code) {
    kdbg_error("Got ISR");
    trace_hex_print(err);
    trace_hex_print(code);
    trace_print();
    for(;;);
}

// Stores an ISR handler in the IDT
#define STORE_ISR(N) \
    extern void idt_isr##N(void); \
    idt_entry_t isr_entry_##N = { \
        .offset_lo = ((u32) idt_isr##N) & 0xffff, \
        .selector = 0x08, \
        .zero = 0, \
        .type = 0b10001110, \
        .offset_hi = ((u32) idt_isr##N) >> 16 \
    }; \
    idt[N] = isr_entry_##N;

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
    STORE_ISR(8)
    STORE_ISR(9)
    STORE_ISR(10)
    STORE_ISR(11)
    STORE_ISR(12)
    STORE_ISR(13)
    STORE_ISR(14)
    STORE_ISR(15)
    STORE_ISR(16)
    STORE_ISR(17)
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
    STORE_ISR(30)
    STORE_ISR(31)
}
