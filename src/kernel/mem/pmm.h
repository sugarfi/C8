#ifndef _C8_PMM
#define _C8_PMM

#include <stdbool.h>

static int pmm_start = 0x100000;
void* pmm_alloc(int);
void pmm_free(void*);

#endif
