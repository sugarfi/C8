#include <dev/atapio.h>
#include <kdbg/kdbg.h>

void atapio_wait(void) {
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
}

void atapio_read(u32 lba, u8 count, char *buf) {
    u8 i;
    u8 j;
    u32 pos;

    port_outb(0xe0 | ((lba >> 24) & 0xff), ATAPIO_BUS + 6);
    port_outb(0, ATAPIO_BUS + 1);
    port_outb(count, ATAPIO_BUS + 2);
    port_outb(lba & 0xff, ATAPIO_BUS + 3);
    port_outb((lba >> 8) & 0xff, ATAPIO_BUS + 4);
    port_outb((lba >> 16) & 0xff, ATAPIO_BUS + 5);
    port_outb(0x20, ATAPIO_BUS + 7);

    for (i = pos = 0; i < count; i++) {
        atapio_wait();
    
        while(port_inb(ATAPIO_BUS + 7) & 128);
        u8 status;
        while (1) {
            status = port_inb(ATAPIO_BUS + 7);
            if(status & 1) {
                kdbg_error("ERR bit set in PATA status, halting");
                return;
            }
            if(status & 8) break;
        }

        for (j = 0; j < 255; j++) {
            u16 val = port_inw(ATAPIO_BUS);
            buf[pos] = val & 0xff;
            buf[pos + 1] = (val >> 8) & 0xff;
            pos += 2;
        }
    }
}
