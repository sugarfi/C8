#include <mem/page.h>

void page_enable(u32 **pd) {
    __asm__ volatile(
            "mov %0, %%eax\n"
            "mov %%eax, %%cr3\n"
            "mov %%cr0, %%eax\n"
            "or $0x80000001, %%eax\n"
            "mov %%eax, %%cr0\n"
            : : "Nd" (pd));
}

void page_tlb_flush(void) {
    __asm__ volatile(
            "mov %cr3, %eax\n"
            "mov %eax, %cr3\n"
            );
}
