#ifndef _C8_ISR
#define _C8_ISR

#include <int/idt.h>
#include <int/isrdef.h>
#include <sys/syscall.h>
#include <lib/kstdio.h>

typedef void (*isr_t)(void); // A type for ISRs
isr_t ISR_LIST[256];

void isr_install(void);

int isr_handler(int ds, int old, int num);

void isr_init(void);

#endif
