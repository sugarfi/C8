#ifndef _C8_SYS_INT
#define _C8_SYS_INT

#include <info/ib.h>
#include <sys/systable.h>
#include <kdbg/kdbg.h>
#include <dev/pic.h>

__attribute__((interrupt)) void sys_irq(void* frame);

#endif
