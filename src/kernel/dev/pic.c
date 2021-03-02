#include <dev/pic.h>

void pic_eoi(u32 irq) {
    port_outb(0x20, PIC1);
    if (irq >= PIC2_OFF) {
        port_outb(0x20, PIC2);
    }
}

void pic_init(void) {
    port_outb(0x11, PIC1);
    port_outb(0x11, PIC2);

    port_outb(PIC1_OFF, PIC1 + 1);
    port_outb(PIC2_OFF, PIC2 + 1);

    port_outb(4, PIC1 + 1);
    port_outb(2, PIC2 + 1);

    port_outb(1, PIC1 + 1);
    port_outb(1, PIC2 + 1);

    port_outb(0, PIC1 + 1);
    port_outb(0, PIC2 + 1);
}

void pic_mask(u8 mask) {
    u16 port;
    if (mask < PIC2_OFF) {
        port = PIC1 + 1;
    } else {
        port = PIC2 + 1;
        mask -= 8;
    }

    port_outb(mask, port);
}
