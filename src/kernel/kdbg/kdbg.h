#ifndef _C8_KDBG
#define _C8_KDBG

#include <dev/port.h>

#define KDBG_COM1 0x3f8
#define KDBG_COM2 0x2f8

void kdbg_init(void);
void kdbg_info(char *);
void kdbg_warn(char *);
void kdbg_error(char *);
void kdbg_death(char *);

#endif
