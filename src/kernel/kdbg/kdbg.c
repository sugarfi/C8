#include <kdbg/kdbg.h>

void kdbg_init(void) {
    /*
     * Initializes the serial kernel debugger. All serial code taken from here: https://wiki.osdev.org/Serial_Port
     */
    port_outb(0x00, KDBG_COM1 + 1);
    port_outb(0x80, KDBG_COM1 + 3);
    port_outb(0x03, KDBG_COM1);
    port_outb(0x00, KDBG_COM1 + 1);
    port_outb(0x03, KDBG_COM1 + 3);
    port_outb(0xc7, KDBG_COM1 + 2);
    port_outb(0x0b, KDBG_COM1 + 4); 
}

void kdbg_send_char(char c) {
    /*
     * This code sends a single character to the serial port. See the link above.
     */
    while ((port_inb(KDBG_COM1 + 5) & 0x20) == 0);
    port_outb(c, KDBG_COM1);
}

void kdbg(char *s) {
    /*
     * Sends a string to the serial port.
     */
    char i;
    while (i = *(s++)) {
        kdbg_send_char(i);
    }
}

void kdbg_info(char *s) {
    /*
     * Outputs an info level debug to the serial port.
     */
    kdbg("[INFO]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_warn(char *s) {
    /*
     * Outputs a warning to the serial port.
     */
    kdbg("[WARN]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_error(char *s) {
    /*
     * Outputs an error the serial port.
     */
    kdbg("[ERROR]: ");
    kdbg(s);
    kdbg("\n\r");
}

void kdbg_death(char *s) {
    /*
     * Outputs a fatal error, with the message 'DEATH AND SUFFERING' to the serial port. Should
     * usually be followed up with a halt or an exit.
     */
    kdbg("[DEATH AND SUFFERING]: ");
    kdbg(s);
    kdbg("\n\r");
}
