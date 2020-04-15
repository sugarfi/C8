#ifndef _C8_PAGEFRAME
#define _C8_PAGEFRAME

#include <stdbool.h>

#define PAGEFRAME_NUMBER 1024

bool PAGEFRAMES[PAGEFRAME_NUMBER];

void pageframe_init(void);

int pageframe_alloc(void);
void pageframe_free(int);

#endif
