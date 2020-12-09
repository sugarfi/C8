#include <mem/alloc.h>

void *alloc_alloc_page(void) {
    ib_t *ib = (ib_t *) 0x3ff; 

    u32 i, j;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            if ((ib->allocated[i] & (1 << j)) == 0) {
                ib->allocated[i] |= (1 << j);
                u8 *ptr = (u8 *) (((i * 32) + j + 1024) << 12);
                return (void *) ptr;
            }
        }
    }

    return NULL;
}

void alloc_free_page(void *ptr) {
    ib_t *ib = (ib_t *) 0x3ff;

    u32 addr = (u32) ptr;
    addr >> 12;
    u32 i = addr / 32;
    u32 j = addr % 32;

    ib->allocated[i] &= ~(1 << j);
}
