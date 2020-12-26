#include <dev/atapio.h>

void atapio_wait(void) {
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
    port_inb(ATAPIO_BUS + 12);
}

void atapio_poll(void) {
    atapio_wait();
    u8 status;
retry: 
    status = port_inb(ATAPIO_BUS + 7);
    if (status & 128) goto retry;
retry2: 
    status = port_inb(ATAPIO_BUS + 7);
    if(status & 1) {
        kdbg_error("ERR bit set in PATA status, halting");
        for(;;);
    }
    if(!(status & 8)) goto retry2;
}   

void atapio_read_one(u32 lba, char *buf) {
    port_outb(0xe0 | ((lba >> 24) & 0x0f), ATAPIO_BUS + 6);
    port_outb(0, ATAPIO_BUS + 1);
    port_outb(1, ATAPIO_BUS + 2);
    port_outb(lba & 0xff, ATAPIO_BUS + 3);
    port_outb((lba >> 8) & 0xff, ATAPIO_BUS + 4);
    port_outb((lba >> 16) & 0xff, ATAPIO_BUS + 5);
    port_outb(0x20, ATAPIO_BUS + 7);

    atapio_poll();
    u16 val;
    u8 i;
    for (i = 0; i < 256; i++) {
        val = port_inw(ATAPIO_BUS);
        *((u16 *)(buf + i * 2)) = val;
    }
    atapio_wait();
}

void atapio_read(u32 lba, u8 count, char* buf) {
    //__asm__ volatile ("cli");

    u32 i;
    for (i = 0; i < count; ++i, buf += 512) {
        atapio_read_one(lba + i, buf);
    }

    //__asm__ volatile ("sti");
}
