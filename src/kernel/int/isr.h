#ifndef _C8_ISR
#define _C8_ISR

#include <int/idt.h>
#include <int/isrdef.h>
#include <sys/syscall.h>
#include <lib/kstdio.h>

typedef void (*isr_t)(void); // A type for ISRs
isr_t ISR_LIST[] = { // A list of the first 32 ISRs
    isr_0,
    isr_1,
    isr_2,
    isr_3,
    isr_4,
    isr_5,
    isr_6,
    isr_7,
    isr_8,
    isr_9,
    isr_10,
    isr_11,
    isr_12,
    isr_13,
    isr_14,
    isr_15,
    isr_16,
    isr_17,
    isr_18,
    isr_19,
    isr_20,
    isr_21,
    isr_22,
    isr_23,
    isr_24,
    isr_25,
    isr_26,
    isr_27,
    isr_28,
    isr_29,
    isr_30,
    isr_31,
};

void isr_install(void);

int isr_handler(int ds, int old, int num);

#endif
