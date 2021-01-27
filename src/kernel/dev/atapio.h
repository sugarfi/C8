#ifndef _C8_ATAPIO
#define _C8_ATAPIO

#include <dev/port.h>
#include <types.h>
#include <kdbg/kdbg.h>

#define ATAPIO_BUS 0x1f0

void atapio_setup(void);
void atapio_read(u32, u8, char *);
void atapio_write(u32, u8, char *);

#endif
