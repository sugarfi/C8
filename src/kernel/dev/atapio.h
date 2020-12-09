#ifndef _C8_ATAPIO
#define _C8_ATAPIO

#include <dev/port.h>
#include <types.h>

#define ATAPIO_BUS 0x1f0

void atapio_wait(void);
void atapio_read(u32, u8, char *);

#endif
