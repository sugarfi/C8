#ifndef _C8_PORT
#define _C8_PORT

void outb(short port, char val) {
    __asm__ volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

char inb(short port) {
    char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(short port, short val) {
    __asm__ volatile ("outw %0, %1" : : "a" (val), "Nd" (port));
}

short inw(short port) {
    short ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif
