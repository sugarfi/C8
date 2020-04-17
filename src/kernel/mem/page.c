#include <mem/page.h>

void page_map(unsigned int page_table[1024], int real, int virtual, int flags) {
    page_table[virtual >> 12 & 0x03FF] = real | flags;
    page_flush_tlb();
}
