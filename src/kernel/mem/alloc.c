#include <mem/alloc.h>

void *alloc_alloc_page(void) {
    /*
     * Allocates a single page and returns its address.
     */
    ib_t *ib = (ib_t *) 0x3ff; 

    u32 i, j;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) { // We iterate over all the 1024 possible pages.
            /*
             * ib->allocated is a bitarray, which 32 u32s in it. Each bit represents
             * a single page.
             */
            if ((ib->allocated[i] & (1 << j)) == 0) { // Check for a freed page
                ib->allocated[i] |= (1 << j); // If it is freed, we mark it as used
                u8 *ptr = (u8 *) (((i * 32) + j + 1024) << 12); // Get its address
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
    ib_t *ib = (ib_t *) 0x3ff;

    u32 addr = (u32) ptr; // Get the address from the pointer
    addr >> 12;
    u32 i = addr / 32;
    u32 j = addr % 32;

    ib->allocated[i] &= ~(1 << j); // If the page is freed, this will allocate it, so there should probably be a check or something here
}
