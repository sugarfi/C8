#include <mem/page.h>

void page_fill(unsigned int item[1024], char flags)
{
    /*
    Fills a page table or directory with entries
    */
    int i;
    for (i = 0; i < 1024; i++) {
        item[i] = flags;
    }
}

void page_id(unsigned int table[1024], char flags)
{
    /*
    Sets up identity paging in a table
    */
    int i;
    for (i = 0; i < 1024; i++) {
        table[i] = (i << 12) | flags;
    }
}

void page_map(unsigned int table[1024], int virtual, int real, char flags)
{
    /*
    Maps a virtual address to a physical one
    */
    table[virtual >> 12 & 0x03ff] = (real) | flags;
    page_load_dir(page_dir); // Flush the TLB
}

void page_unmap(unsigned int table[1024], int virtual)
{
    /*
    Unmaps a virtual address
    */
    table[virtual >> 12] = 0;
    page_load_dir(page_dir); // Flush the TLB
}
