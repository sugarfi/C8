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
#include <sys/syscall.h>
#include <dev/pic.h>

#define MAP_PAGE(N) \
    u32 __attribute__((aligned(4096))) pt##N[1024]; \
    for (i = 0; i < 1024; ++i) { \
        pt##N[i] = ((i * 4096) + (N * 4096 * 1024)) | 3; \
    } \
    pd[N] = ((u32) pt##N) | 3;

void kmain(void) {
    kdbg_init();
    kdbg_info("Initialized kernel debugger"); // Initialize kdbg and print a message

    u32 __attribute__((aligned(4096))) pd[1024];

    kdbg_info("Identity paging the first some memory");
    u32 i;
    MAP_PAGE(0);
    MAP_PAGE(1);
    MAP_PAGE(2);
    MAP_PAGE(3);
    MAP_PAGE(4);
    MAP_PAGE(5);
    MAP_PAGE(6);
    MAP_PAGE(7);
    MAP_PAGE(8);
    MAP_PAGE(9);
    MAP_PAGE(10);
    MAP_PAGE(11);
    MAP_PAGE(12);
    MAP_PAGE(13);
    MAP_PAGE(14);
    MAP_PAGE(15);

    page_enable((u32) pd); // Enable paging

    //mbr_t *mbr = (mbr_t *) (0x7c00 + 440);

    kdbg_info("Setting up interrupts");
    idt_entry_t idt[256];
    idt_fill(idt); // Set up an IDT and fill it with ISRs

    irq_f_t sys_irq_f = &sys_irq;
    idt_entry_t sys_irq_entry = { // Add the syscall IRQ as entry 128 in the IDT.
        .offset_lo = ((u32) sys_irq_f) & 0xffff,
        .selector = 0x08,
        .zero = 0,
        .type = 0x8e,
        .offset_hi = ((u32) sys_irq_f) >> 16
    };
    //idt[128] = sys_irq_entry;

    idt_desc_t idt_desc = { // Create the IDT descriptor and load it.
        .size = 256 * sizeof(idt_entry_t) - 1,
        .ptr = (u32) &idt
    };
    idt_load(&idt_desc);

    kdbg_info("Remapping the PIC");
    pic_init();

    /*
    kdbg_info("Initializing allocator");
    alloc_base = (u32) alloc_alloc_page();

    kdbg_info("Filling system call table with invalid entries"); // We want the whole syscall table to be filled with invalid entries
    systable_entry_t systable[1024];
    systable_entry_t sys_nop = {
        .func = NULL,
        .flags = 0
    };
    for (i = 0; i < 1024; i++) {
        systable[i] = sys_nop;
    }
    
    kdbg_info("Writing actual syscalls");
    // Any real syscalls go here
    systable_entry_t sys_open = {
        .func = &syscall_open,
        .flags = 1 // Present
    };
    systable[0] = sys_open;
    systable_entry_t sys_read = {
        .func = &syscall_read,
        .flags = 1 // Present
    };
    systable[1] = sys_read;
    systable_entry_t sys_write = {
        .func = &syscall_write,
        .flags = 1 // Present
    };
    systable[2] = sys_write;

    kdbg_info("Initializing ATAPIO driver");
    atapio_setup(); // Might not be necessary but whatever :/

    kdbg_info("Populating VFS root");
    vfs_dir_t *root = vfs_make_dir("files/"); // Naming the root directory "files" is probably not good
    vfs_populate(root);                       // Later I will change the name to "root" and allow reads without a
                                              // prefix

    ib_t ib = { // Create the information block with all relevant things and copy it to memory.
        .mbr = (u32) mbr,
        .pd = (u32) pd,
        .idt = (u32) idt,
        .systable = (u32) systable,
        .root = (u32) root,
        .next_fd = 0,
        .res = 0
    };

    kdbg_info("Writing information block to address 0x00000500");
    mem_cpy((char *) 0x500, (char *) &ib, sizeof(ib_t));
    */

    kdbg_info("ok!");
    vga_put("Hello, world!", 0x07);
    for(;;);
    /*

    kdbg_info("Reading and printing motd.txt");
    char *motd = alloc_alloc(512);
    char *filename = "files/motd.txt";
    char *buf = (char *) alloc_alloc(1024);
    mem_set(buf, 'a', 1024);
    buf[1023] = 0;

    __asm__ volatile ("int $0x80" : : "a" (0), "S" ((u32) filename));

    u32 fd = IB_RES;
    if (fd == 0) { // 0 = false = failure
        kdbg_error("Could not open motd.txt.");
    } else {
        __asm__ volatile ("int $0x80" : : "a" (2), "D" (fd), "S" ((u32) buf), "c" (mem_len(buf)));
        __asm__ volatile ("int $0x80" : : "a" (1), "S" (fd), "D" ((u32) motd));
        vga_put(motd, 0x07);
    }

    alloc_free(motd);
    alloc_free(buf);

    vfs_flush(root);
    //vfs_free(root); // Clean up
    alloc_free(root);

    kdbg_info("Finished, hanging"); // Probably a better way to do this, even just a return could work ok

    for (;;);
    */
}
