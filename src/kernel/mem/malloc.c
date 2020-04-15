#include <mem/malloc.h>

static int malloc_addr = 0x1000;
static malloc_block_t malloc_blocks[] = {};

void* kmalloc(int size)
{
    int i;
    int out = 0;
    malloc_block_t this;
    for (i = 0; i < sizeof(malloc_blocks) / sizeof(malloc_block_t); i++) {
        this = malloc_blocks[i];
        if (this.size >= size && this.free) {
            out = this.addr;
            this.free = false;
            malloc_blocks[i] = this;
        }
    }
    if (out) {
        return (void*) out;
    }
    malloc_block_t new = { malloc_addr, size, false };
    malloc_blocks[sizeof(malloc_blocks) / sizeof(malloc_block_t)] = new;
    malloc_addr += size;
    return (void*) new.addr;
}

void kfree(void* ptr)
{
    int i;
    int addr = (unsigned int) ptr;
    int index;
    malloc_block_t this;
    for (i = 0; i < sizeof(malloc_blocks) / sizeof(malloc_block_t); i++) {
        this = malloc_blocks[i];
        if (this.addr == addr && !this.free) {
            this.free = true;
            malloc_blocks[i] = this;
        }
    }
    return;
}
