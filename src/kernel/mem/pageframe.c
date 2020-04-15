#include <mem/pageframe.h>

void pageframe_init(void) {
    int i;
    for (i = 0; i < PAGEFRAME_NUMBER; i++) {
        PAGEFRAMES[i] = true;
    }
}

int pageframe_alloc(void) {
    int i;
    for (i = 0; i < PAGEFRAME_NUMBER; i++) {
        if (PAGEFRAMES[i]) {
            PAGEFRAMES[i] = false;
            return i;
        }
    }
    return -1;
}

void pageframe_free(int i) {
    PAGEFRAMES[i] = true;
}
