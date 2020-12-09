#ifndef _C8_PORT
#define _C8_PORt

#include <types.h>

u8 port_inb(u16);
void port_outb(u8, u16);
u16 port_inw(u16);
void port_outw(u16, u16);

#endif
