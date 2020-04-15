#ifndef _C8_PORT
#define _C8_PORT

void outb(short port, char val);

char inb(short port);

void outw(short port, short val);

short inw(short port);

#endif
