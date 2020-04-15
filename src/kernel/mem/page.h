#ifndef _C8_PAGE
#define _C8_PAGE

/*
page.h - Paging utilities
*/

unsigned int page_dir[1024] __attribute__((aligned(4096))); // The page directory
unsigned int page_table[1024] __attribute__((aligned(4096))); // One page table

extern void page_enable(); // These are defined in kernel.asm
extern void page_load_dir(unsigned int*);

void page_fill(unsigned int item[1024], char flags);

void page_id(unsigned int table[1024], char flags);

void page_map(unsigned int table[1024], int virtual, int real, char flags);

void page_unmap(unsigned int table[1024], int virtual);

#endif
