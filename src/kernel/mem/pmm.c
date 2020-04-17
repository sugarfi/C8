#include <mem/pmm.h>

void* pmm_alloc(int size) {
    int addr;
    int pos;
    char* ptr;
    bool found;
    for (addr = pmm_start; 1; addr++) {
        found = true;
        ptr = (char*) addr;
        for (pos = 0; pos < size; pos++) {
            if (*ptr != 0) {
                found = false;
            }
        }
        if (found) {
            pmm_start += size;
            return (void*) addr;
        }
    }
    return (void*) 0;
}

void pmm_free(void* ptr) { // TODO
    return;
}
