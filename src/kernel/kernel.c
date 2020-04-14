#include <mem/page.h>
#include <mem/malloc.h>
#include <int/idt.h>
#include <int/isr.h>
#include <lib/kstdio.h>
#include <fs/file.h>
#include <fs/ops.h>

void kmain(void) {
    page_fill(page_dir, 0x02);
    page_id(page_table, 0x03);

    page_dir[0] = ((unsigned int) page_table) | 3;
    page_dir[1023] = ((unsigned int) page_dir) | 3;

    page_load_dir(page_dir);
    page_enable();

    idt_desc.base = (unsigned int) &idt;
    idt_desc.limit = 256 * sizeof(idt_entry_t) - 1;
    idt_load(&idt_desc);
    isr_install();

    dev_t stdin = {
        0,
        &dev_nop,
        &kgets,
    };
    dev_init(0, stdin);
    dev_t stdout = {
        1,
        &kputs,
        &dev_nop,
    };
    dev_init(1, stdout);

    int motd = file_open("sys/motd.c8", FILE_ADDR);
    char buffer[1024];
    file_read(motd, buffer);
    kputs(buffer);

    int csh = file_open("bin/csh.o", FILE_ADDR);
    file_exec(csh);
    return;
}
