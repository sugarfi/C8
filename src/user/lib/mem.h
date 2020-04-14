#ifndef _C8_USER_MEM
#define _C8_USER_MEM

void mem_map(int real, int virtual, char flags) {
    __asm__ __volatile__ ("int $0x80" : : "a" (4), "S" (real), "D" (virtual), "d" (flags));
}

int malloc(int size) {
    int addr;
    __asm__ __volatile__ ("int $0x80" : "=a" (addr) : "a" (5), "c" (size));
    return addr;
}

void free(void* ptr) {
    __asm__ __volatile__ ("int $0x80" : : "a" (6), "b" ((unsigned int) ptr));
}

#endif
