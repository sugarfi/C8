#include <int/idt.h>
#include <int/isr.h>
#include <sys/syscall.h>
#include <mem/pageframe.h>
#include <lib/kstdio.h>
#include <fs/open.h>
#include <fs/exec.h>
#include <lib/kstdio.h>

void kmain(void) {
    static unsigned int page_dir[1024] __attribute__((aligned(4096)));
    static unsigned int page_table[1024] __attribute__((aligned(4096)));

    page_fill(page_dir, 0x02);
    page_id(page_table, 0x03);

    page_dir[0] = ((unsigned int) page_table) | 3;
    page_dir[1023] = ((unsigned int) page_dir) | 3;

    page_load_dir(page_dir);
    page_enable();

    syscall_init();
    pageframe_init();

    idt_entry_t idt[256];
    isr_init();
    isr_install(idt);
    idt_desc_t idt_desc;
    idt_desc.base = (unsigned int) &idt;
    idt_desc.limit = 256 * sizeof(idt_entry_t) - 1;
    idt_load(&idt_desc);

    file_t* test;
    file_open("bin/test.o", &test);
    file_exec(test, page_table);

    return;
}
