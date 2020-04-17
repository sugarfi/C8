#ifndef _C8_PAGE
#define _C8_PAGE

extern void page_enable(void);
extern void page_load_dir(unsigned int*);
extern void page_flush_tlb(void);

void page_map(unsigned int*, int, int, int);

#endif
