#include <types.h>
#include <info/ib.h>
#include <info/mbr.h>
#include <sys/systable.h>
#include <sys/sysint.h>
#include <mem/mem.h>
#include <mem/page.h>
#include <kdbg/kdbg.h>
#include <dev/int.h>
#include <dev/atapio.h>
#include <mem/alloc.h>
#include <dev/vga.h>
#include <fs/tar.h>

void kmain(void) {
    kdbg_init();
    kdbg_info("Intialized kernel debugger"); // Initialize kdbg and print a message

    mbr_t *mbr = (mbr_t *) (0x7c00 + 440);

    u32 __attribute__((aligned(4096))) pt[1024];
    u32 __attribute__((aligned(4096))) pd[1024];

    kdbg_info("Identity paging the first 8 MiB"); // Identity page 8 MiB of memory
    u32 i;
    for (i = 0; i < 1024; i++) {
        pt[i] = (i << 12) | 3;
    }
    pd[0] = ((u32) pt) | 3;
    pd[1] = ((u32) pt) | 3;

    page_enable((u32 **) &pd); // Enable paging

    kdbg_info("Setting up interrupts");
    idt_entry_t idt[256];
    idt_fill(idt); // Set up an IDT and fill it with ISRs

    irq_f_t sys_irq_f = &sys_irq;
    idt_entry_t sys_irq_entry = { // Add the syscall IRQ as entry 128 in the IDT.
        .offset_lo = ((u32) sys_irq_f) & 0xffff,
        .selector = 0x08,
        .zero = 0,
        .type = 0b10001110,
        .offset_hi = ((u32) sys_irq_f) >> 16
    };
    idt[128] = sys_irq_entry;

    idt_desc_t idt_desc = { // Create the IDT descriptor and load it.
        .size = 256 * sizeof(idt_entry_t) - 1,
        .ptr = (u32) &idt
    };
    idt_load(&idt_desc);


    kdbg_info("Filling system call table with invalid entries"); // We want the whole syscall table to be filled with invalid entries
    systable_entry_t systable[1024];
    systable_entry_t sys_nop = {
        .func = NULL,
        .flags = 0
    };
    for (i = 0; i < 1024; i++) {
        systable[i] = sys_nop;
    }
    
    ib_t ib = { // Create the information block with all relevant things and copy it to memory.
        .mbr = (u32) mbr,
        .pd = (u32) pd,
        .idt = (u32) idt,
        .systable = (u32) systable,
        .res = 0,
    };

    kdbg_info("Writing information block to address 0x000003ff");

    mem_cpy((char *) 0x3ff, (char *) &ib, sizeof(ib_t));

    kdbg_info("Initializing ATAPIO driver");
    atapio_setup(); // Might not be necessary but whatever :/

    kdbg_info("Reading and printing motd.txt");
    char motd[512];
    if (!tar_read("motd.txt", mbr->part2.start_lba + 1, motd)) { // Ideally this can later be done with an fread function or something
        vga_put("Error reading file motd.txt, please ensure it exists.", 0x07);
    } else {
        vga_put(motd, 0x07);
    }

    kdbg_info("Finished, hanging"); // Prob a better way to do this, even just a return could work ok

    for (;;);
}
