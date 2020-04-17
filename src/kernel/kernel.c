#include <int/idt.h>
#include <int/isr.h>
#include <sys/syscall.h>
#include <mem/pageframe.h>
#include <lib/kstdio.h>
#include <fs/open.h>
#include <fs/exec.h>

void kmain(void) {
    idt_desc.base = (unsigned int) &idt;
    idt_desc.limit = 256 * sizeof(idt_entry_t) - 1;
    idt_load(&idt_desc);

    isr_init();
    isr_install();
    syscall_init();
    pageframe_init();

    file_t* file;
    file_open("bin/test.o", &file);
    file_exec(file);

    return;
}
