#include <dev/port.h>

u8 port_inb(u16 port) {
    /*
     * Uses assembly to read one byte from an I/O port.
     */
    u8 out;
    
    __asm__ volatile ("inb %1, %0;" : "=a" (out) : "Nd" (port));

    return out;
}

void port_outb(u8 byte, u16 port) {
    /*
     * Uses assembly to write one byte to an I/O port.
     */
    __asm__ volatile("outb %0, %1;" : : "a" (byte), "Nd" (port));
}


u16 port_inw(u16 port) {
    u16 out;
    
    __asm__ volatile ("inw %1, %0;" : "=a" (out) : "Nd" (port));

    return out;
}

void port_outw(u16 word, u16 port) {
    __asm__ volatile("outw %0, %1;" : : "a" (word), "Nd" (port));
}

