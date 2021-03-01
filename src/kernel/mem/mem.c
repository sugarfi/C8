#include <mem/mem.h>

void mem_cpy(char *a, char *b, u32 n) {
    /*
     * A simple memcpy.
     */
    u32 i;
    for (i = 0; i < n; ++i) {
        a[i] = b[i];
    }
    //a[n] = 0;
}

bool mem_cmp(char *a, char *b, u32 n) {
    /*
     * strcmp but it returns a boolean (fight me). Probably closer to strncmp?
     */
    u32 i;
    for (i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

u32 mem_len(char *s) {
    /*
     * strlen with a different name.
     */
    u32 i = 0;
    while (s[i++]);
    return i;
}

void mem_cat(char *a, char *b) {
    /*
     * Concatenate two strings.
     */
    u32 i;
    for (i = 0; i < mem_len(b); ++i) {
        a[i + mem_len(a)] = b[i];
    }
}

void mem_set(char *s, char x, u32 n) {
    /*
     * Set the first n bytes of s to x.
     */
    u32 i;
    for (i = 0; i < n; i++) {
        s[i] = x;
    }
}
