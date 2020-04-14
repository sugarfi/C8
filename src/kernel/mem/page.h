#ifndef _C8_PAGE
#define _C8_PAGE

/*
page.h - Paging utilities
*/

extern void page_enable(); // These are defined in kernel.asm
extern void page_load_dir(unsigned int*);

static unsigned int page_dir[1024] __attribute__((aligned(4096))); // The page directory
static unsigned int page_table[1024] __attribute__((aligned(4096))); // One page table

void page_fill(unsigned int item[1024], char flags) {
    /*
    Fills a page table or directory with entries
    */
    int i;
    for (i = 0; i < 1024; i++) {
        item[i] = flags;
    }
}

void page_id(unsigned int table[1024], char flags) {
    /*
    Sets up identity paging in a table
    */
    int i;
    for (i = 0; i < 1024; i++) {
        table[i] = (i << 12) | flags;
    }
}

void page_map(unsigned int table[1024], int virtual, int real, char flags) {
    /*
    Maps a virtual address to a physical one
    */
    table[virtual >> 12 & 0x03ff] = (real) | flags;
    page_load_dir(page_dir); // Flush the TLB
}

void page_unmap(unsigned int table[1024], int virtual) {
    /*
    Unmaps a virtual address
    */
    table[virtual >> 12] = 0;
    page_load_dir(page_dir); // Flush the TLB
}

#endif
