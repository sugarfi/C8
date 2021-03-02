#include <dev/atapio.h>

void insw(u16 port, u16 *dest, u32 count) {
    // Many thanks to this SO post: https://stackoverflow.com/questions/64945691/how-to-use-ins-instruction-with-gnu-assembler
    __asm__ volatile("rep ins%z2" : "+D" (dest), "+c" (count), "=m" (*dest) : "d" (port) : "memory"); // Invoke the rep insw instruction
}

void outsw(u16 port, u16 *buf, u32 count) {
    // Many thanks to this SO post: https://stackoverflow.com/questions/64945691/how-to-use-ins-instruction-with-gnu-assembler
    __asm__ volatile("rep outs%z2" : "+S" (buf), "+c" (count) : "d" (port) : "memory"); // Invoke the rep outsw instruction
}

void atapio_wait(void) {
    /*
     * Some ATAPIO operations require a 500 nanosecond delay to allow the drive time to process; the delay created by
     * 5 inb operations is about enough to provide this.
     */
    port_inb(ATAPIO_BUS + 7);
    port_inb(ATAPIO_BUS + 7);
    port_inb(ATAPIO_BUS + 7);
    port_inb(ATAPIO_BUS + 7);
    port_inb(ATAPIO_BUS + 7);
}

void atapio_poll(void) {
    /*
     * After sending a read command and reading one sector (and later, a write command), we have to poll the device in a loop,
     * checking if there were any errors in the operation and halting if so, and then when the drive is ready exiting the loop.
     */
    u8 status;

    while (true) {
        status = port_inb(ATAPIO_BUS + 7); // ATAPIO_BUS + 7 is the ATAPIO status register
        if (!(status & 128) && (status & 8)) { // status & 128 is the BSY bit and status & 8 is DRQ, so when BSY is 0 and DRQ is 1
                                               // we can exit the loop.
            break;
        }
    }
    if ((status & 32) || (status & 1)) {
        kdbg_death("ERR bit set in PATA status, cannot continue disk operation");
        for(;;);
    }
}

void atapio_setup(void) {
    port_outb(0b00000010, 0x3f6); // I forget why I wrote this but we probably don't need it
}

void atapio_read(u32 lba, u8 count, char *buf) {
    while (port_inb(ATAPIO_BUS + 7) & 128); // '^^ Again, no idea what this is for
    port_outb(0xe0 | ((lba >> 24) & 0x0f), ATAPIO_BUS + 6); // We output 0xe0 for the master drive, and the top few bits of the LBA
                                                            // value to ATAPIO_BUS + 6, the drive select register
    port_outb(count, ATAPIO_BUS + 2); // We output the count to ATAPIO_BUS + 2, the sector count register
    port_outb((u8) lba, ATAPIO_BUS + 3); // We output the low 8 bits of the LBA to the first LBA register
    port_outb((u8) (lba >> 8), ATAPIO_BUS + 4); // Second 8 to the second LBA register
    port_outb((u8) (lba >> 16), ATAPIO_BUS + 5); // Third 8 to the third LBA register
    port_outb(0x20, ATAPIO_BUS + 7); // We output 0x20, the read command to ATAPIO_BUS + 7, the command register

    u16 val;
    u8 i, j;

    u16 *target = (u16 *) buf;

    for (i = 0; i < count; ++i) {   
        atapio_poll(); // Wait for the drive to be ready
        insw(ATAPIO_BUS, target, 256); // Read 256 16-bit values into the buffer
        target += 256; // Increment
    }
    atapio_wait(); // Small delay
}

void atapio_write(u32 lba, u8 count, char *buf) {
    while (port_inb(ATAPIO_BUS + 7) & 128); // '^^ Again, no idea what this is for
    port_outb(0xe0 | ((lba >> 24) & 0x0f), ATAPIO_BUS + 6); // We output 0xe0 for the master drive, and the top few bits of the LBA
                                                            // value to ATAPIO_BUS + 6, the drive select register
    port_outb(0, ATAPIO_BUS + 1);
    port_outb(count, ATAPIO_BUS + 2); // We output the count to ATAPIO_BUS + 2, the sector count register
    port_outb((u8) lba, ATAPIO_BUS + 3); // We output the low 8 bits of the LBA to the first LBA register
    port_outb((u8) (lba >> 8), ATAPIO_BUS + 4); // Second 8 to the second LBA register
    port_outb((u8) (lba >> 16), ATAPIO_BUS + 5); // Third 8 to the third LBA register
    port_outb(0x30, ATAPIO_BUS + 7); // We output 0x30, the write command to ATAPIO_BUS + 7, the command register
    
    u16 val;
    u8 i, j;

    u16 *target = (u16 *) buf;

    for (i = 0; i < count; ++i) {   
        atapio_poll(); // Wait for the drive to be ready
        outsw(ATAPIO_BUS, target, 256);
        target += 256;
    }
    port_outb(0x7e, ATAPIO_BUS + 7);
    atapio_wait(); // Small delay
}
