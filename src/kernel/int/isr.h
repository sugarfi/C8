#ifndef _C8_ISR
#define _C8_ISR

#include <int/idt.h>
#include <int/isrdef.h>
#include <sys/syscall.h>
#include <lib/kstdio.h>

void isr_install(void);

int isr_handler(int ds, int old, int num);

#endif
