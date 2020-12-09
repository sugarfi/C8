#include <kdbg/kdbg.h>

void kdbg_init(void) {
    port_outb(0x00, KDBG_COM1 + 1);
    port_outb(0x80, KDBG_COM1 + 3);
    port_outb(0x03, KDBG_COM1);
    port_outb(0x00, KDBG_COM1 + 1);
    port_outb(0x03, KDBG_COM1 + 3);
    port_outb(0xc7, KDBG_COM1 + 2);
    port_outb(0x0b, KDBG_COM1 + 4); 
}

void kdbg_send_char(char c) {
    while ((port_inb(KDBG_COM1 + 5) & 0x20) == 0);
    port_outb(c, KDBG_COM1);
}

void kdbg(char *s) {
    char i;
    while (i = *(s++)) {
        kdbg_send_char(i);
    }
}

void kdbg_info(char *s) {
    kdbg("[INFO]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_warn(char *s) {
    kdbg("[WARN]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_error(char *s) {
    kdbg("[ERROR]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_death(char *s) {
    kdbg("[DEATH AND SUFFERING]: ");
    kdbg(s);
    kdbg("\n\r");
}
