#include <int/idt.h>
#include <int/isr.h>
#include <lib/kstdio.h>

void kmain(void) {
    idt_desc.base = (unsigned int) &idt;
    idt_desc.limit = 256 * sizeof(idt_entry_t) - 1;
    idt_load(&idt_desc);

    isr_init();
    isr_install();

    syscall_init();

    return;
}
