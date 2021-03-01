#ifndef _C8_MEM
#define _C8_MEM

#include <types.h>

void mem_cpy(char *, char *, u32);
bool mem_cmp(char *, char *, u32);
u32 mem_len(char *);
void mem_cat(char *, char *);
void mem_set(char *, char, u32);

#endif
