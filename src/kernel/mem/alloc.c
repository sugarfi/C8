#include <mem/alloc.h>

/*
 * /!\ IMPORTANT /!\: always prefer alloc_alloc() to alloc_alloc_page()
 * alloc_alloc_page can allocate pages that had to be contiguous in order for alloc_alloc to work,
 * so even with the alloc_lock mechanism (which seems kinda useless now that I think about it ><),
 * it should be avoided.
 */

void *alloc_alloc_page(void) {
    /*
     * Allocates a single page and returns its address.
     */
    ib_t *ib = (ib_t *) 0x500; 
    while (alloc_lock); // A simple locking mechanism in case we ever need to prevent access to the page bitmap

    u32 i, j;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) { // We iterate over all the 1024 possible pages.
            /*
             * ib->allocated is a bitarray, with 32 u32s in it. Each bit represents
             * a single page.
             */
            if ((ib->allocated[i] & (1 << j)) == 0) { // Check for a freed page
                ib->allocated[i] |= (1 << j); // If it is freed, we mark it as used
                u8 *ptr = (u8 *) (((i * 32) + j + 1024) << 12) + (8192 << 12); // Get its address
                return (void *) ptr;
            }
        }
    }

    // Might wanna put a kdbg here?
    return NULL; // Return NULL if all 1024 pages are allocated.
}

void alloc_free_page(void *ptr) {
    /*
     * Frees a page, assuming it is already allocated.
     */
    ib_t *ib = (ib_t *) 0x500;

    u32 addr = (u32) ptr; // Get the address from the pointer
    addr >> 12;
    u32 i = addr / 32;
    u32 j = addr % 32;

    ib->allocated[i] &= ~(1 << j); // If the page is freed, this will allocate it, so there should probably be a check or something here
}

void *alloc_alloc(u32 size) { 
    /*
     * Given a size, finds or creates a block of the appropriate size and returns it.
     */
    alloc_lock = true; // We want to lock page allocations here to ensure any allocated pages are contiguous

    u32 i = alloc_base + ((allocated - 1) << 12); // Starting from the base allocation address *every time* is ridiculously inefficient for the record
    alloc_header_t *header = (alloc_header_t *) i;
    for (; i - alloc_base < (allocated << 12); i += header->size + sizeof(alloc_header_t), header = (alloc_header_t *) i) { // Might be done better with *i* as the alloc_header_t *?
        if (header->size >= size && (header->flags & 1)) { // flags & 1 is the free bit
            header->flags &= ~1;
            u32 addr = i - alloc_base;
            while (addr > (allocated << 12)) {
                alloc_lock = false;
                alloc_alloc_page(); // Race conditions aaaaah
                alloc_lock = true;
                allocated++;
            }
            u32 new = header->size - size;
            if (new > 2) { // If there is enough space for a new header and at least 1 byte of data, we will create a new block
                new -= 2;
                alloc_header_t new_header = {
                    .size = new,
                    .flags = 0
                };
                mem_cpy((u8 *) i + sizeof(alloc_header_t) + size, (u8 *) &new_header, sizeof(alloc_header_t));
            }
            /*
            u32 j;
            for (j = 0; j < size; j++) {
                ((u8 *) i)[sizeof(alloc_header_t) + j] = 0;
            }
            */
            return (void *) (i + sizeof(alloc_header_t));
        }
    }

    // If we get to here then we need to allocate more space since no blocks were big enough
    alloc_lock = false; // Works I guess
    u32 addr = (u32) alloc_alloc_page(); // Allocate at least one page
    allocated++;
    for (i = 0; i < (size >> 12); i++) { // (size >> 12) will be rounded down hence the above
        alloc_alloc_page();
        allocated++;
    }
    alloc_header_t new_header = { // Write a new allocation header
        .size = size,
        .flags = 0
    };
    mem_cpy((u8 *) addr, (u8 *) &new_header, sizeof(alloc_header_t));
    /*
    u32 j;
    for (j = 0; j < size; j++) {
        ((u8 *) addr)[sizeof(alloc_header_t) + j] = 0;
    }
    */
    return (void *) (addr + sizeof(alloc_header_t)); // And return the new pointer
}

void alloc_free(void *ptr) {
    /*
     * Given an address, marks its header as free.
     */
    alloc_header_t *header = (alloc_header_t *) (ptr - sizeof(alloc_header_t));
    header->flags |= 1; // Big security flaw - this will write the memory unconditionally, allowing arbitrary bit setting
    // Definitely need a check here
}
