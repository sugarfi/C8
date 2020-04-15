#include <int/isr.h>

void isr_install(void)
{
    /*
    Puts each of the ISRs in the IDT.
    */
    int i;
    for (i = 0; i < 32; i++) {
        idt[i] = idt_make_entry((unsigned int) ISR_LIST[i], 0x08, 0x8e); // Set up the first 32 ISRs
    }
    idt[128] = idt_make_entry((unsigned int) isr_128, 0x08, 0x8e); // Also add the syscall interrupt
}

int isr_handler(int ds, int old, int num)
{
    /*
    Called when an ISR is caught
    */
    if (num == 0x80) { // Do a syscall if we need to
        return syscall_entry(old);
    }
    return 0;
}
