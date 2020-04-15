#include <int/isr.h>

void isr_init(void) {
    ISR_LIST[0] = &isr_0;
    ISR_LIST[1] = &isr_1;
    ISR_LIST[2] = &isr_2;
    ISR_LIST[3] = &isr_3;
    ISR_LIST[4] = &isr_4;
    ISR_LIST[5] = &isr_5;
    ISR_LIST[6] = &isr_6;
    ISR_LIST[7] = &isr_7;
    ISR_LIST[8] = &isr_8;
    ISR_LIST[9] = &isr_9;
    ISR_LIST[10] = &isr_10;
    ISR_LIST[11] = &isr_11;
    ISR_LIST[12] = &isr_12;
    ISR_LIST[13] = &isr_13;
    ISR_LIST[14] = &isr_14;
    ISR_LIST[15] = &isr_15;
    ISR_LIST[16] = &isr_16;
    ISR_LIST[17] = &isr_17;
    ISR_LIST[18] = &isr_18;
    ISR_LIST[19] = &isr_19;
    ISR_LIST[20] = &isr_20;
    ISR_LIST[21] = &isr_21;
    ISR_LIST[22] = &isr_22;
    ISR_LIST[23] = &isr_23;
    ISR_LIST[24] = &isr_24;
    ISR_LIST[25] = &isr_25;
    ISR_LIST[26] = &isr_26;
    ISR_LIST[27] = &isr_27;
    ISR_LIST[28] = &isr_28;
    ISR_LIST[29] = &isr_29;
    ISR_LIST[30] = &isr_30;
    ISR_LIST[31] = &isr_31;
}

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
