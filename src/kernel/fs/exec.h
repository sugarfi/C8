#ifndef _C8_EXEC
#define _C8_EXEC

#include <sys/elf.h>
#include <fs/rw.h>

void file_exec(file_t*);

typedef void (*file_exec_t)(void);

#endif
