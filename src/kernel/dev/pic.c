#include <dev/pic.h>

void pic_eoi(u32 irq) {
    port_outb(0x20, irq >= 8 ? PIC2 : PIC1);
}

void pic_init(u32 off1, u32 off2) {
    u8 a1 = port_inb(PIC1), a2 = port_inb(PIC2);

    port_outb(ICW1_INIT | ICW1_ICW4, PIC1);
    port_outb(ICW1_INIT | ICW1_ICW4, PIC2);
    port_outb(off1, PIC1 + 1);
    port_outb(off1, PIC1 + 1);
    port_outb(4, PIC1 + 1);
    port_outb(2, PIC2 + 1);
    port_outb(ICW4_8086, PIC1 + 1);
    port_outb(ICW4_8086, PIC2 + 1);

    port_outb(a1, PIC1 + 1);
    port_outb(a2, PIC2 + 1);
}
