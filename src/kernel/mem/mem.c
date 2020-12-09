#include <mem/mem.h>

void mem_cpy(char *a, char *b, u32 n) {
    u32 i;
    for (i = 0; i < n; ++i) {
        a[i] = b[i];
    }
}
