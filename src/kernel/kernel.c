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
#include <mod/fat16.h>

void kmain(void) {
    kdbg_init();
    kdbg_info("Intialized kernel debugger");

    mbr_t *mbr = (mbr_t *) (0x7c00 + 440);

    u32 __attribute__((aligned(4096))) pt[1024];
    u32 __attribute__((aligned(4096))) pd[1024];

    kdbg_info("Identity paging the first 8 MiB");
    u32 i;
    for (i = 0; i < 1024; i++) {
        pt[i] = (i << 12) | 3;
    }
    pd[0] = ((u32) pt) | 3;
    pd[1] = ((u32) pt) | 3;

    page_enable((u32 **) &pd);

    kdbg_info("Setting up interrupts");
    idt_entry_t idt[256];
    idt_fill(idt);

    irq_f_t sys_irq_f = &sys_irq;
    idt_entry_t sys_irq_entry = {
        .offset_lo = ((u32) sys_irq_f) & 0xffff,
        .selector = 0x08,
        .zero = 0,
        .type = 0b10001110,
        .offset_hi = ((u32) sys_irq_f) >> 16
    };
    idt[128] = sys_irq_entry;

    idt_desc_t idt_desc = {
        .size = 256 * sizeof(idt_entry_t) - 1,
        .ptr = (u32) &idt
    };
    idt_load(&idt_desc);


    kdbg_info("Filling system call table with invalid entries");
    systable_entry_t systable[1024];
    systable_entry_t sys_nop = {
        .func = NULL,
        .flags = 0
    };
    for (i = 0; i < 1024; i++) {
        systable[i] = sys_nop;
    }
    
    ib_t ib = {
        .mbr = (u32) mbr,
        .pd = (u32) pd,
        .idt = (u32) idt,
        .systable = (u32) systable,
        .res = 0,
    };

    kdbg_info("Writing information block to address 0x000003ff");
    mem_cpy((char *) 0x3ff, (char *) &ib, sizeof(ib_t));

    kdbg_info("Reading second partition on disk (modules) to memory");
    char *buf = (char *) alloc_alloc_page();
    atapio_read(mbr->part2.start_lba, 1, buf);

    fat16_bpb_t *bpb = (fat16_bpb_t *) buf;

    char *root_buf = (char *) alloc_alloc_page();
    atapio_read(mbr->part2.start_lba + bpb->reserved + (bpb->sectors_per_fat * bpb->number_of_fats), bpb->root_dir_entries * 32 / 512, root_buf); // should work if the drive is formatted correctly - i probably shouldn't leave it up to the user to fix their drive, but hey. whatever works
    // although i suppose it doesn't really work - this allows arbitrary memory access if the root directory entries is set to anything over 8 sectors, ie. over 128
    // idk will do smth about it later
    // maybe add an ASSERT macro that does a simple conditional check + a call to death() or fatal()?

    fat16_83_entry_t *root = (fat16_83_entry_t *) root_buf;
    fat16_83_entry_t entry;
    for (i = 0, entry = root[i]; (i < 128) && (entry.name[0] != 0); entry = root[i++]) {
        kdbg_info(entry.name);
    }

    kdbg_info("Finished, hanging"); // Prob a better way to do this, even just a return could work ok

    for (;;);
}
