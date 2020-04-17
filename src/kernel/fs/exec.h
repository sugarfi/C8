#ifndef _C8_EXEC
#define _C8_EXEC

#include <sys/elf.h>
#include <fs/rw.h>
#include <mem/page.h>

void file_exec(file_t*, unsigned int*);

typedef void (*file_exec_t)(void);

#endif
