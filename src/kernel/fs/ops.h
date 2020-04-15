/*
 * ops.h - Common file operations.
*/
#ifndef _C8_OPS
#define _C8_OPS

#include <mem/page.h>
#include <lib/math.h>
#include <mem/malloc.h>
#include <lib/kstdio.h>
#include <sys/elf.h>
#include <fs/dev.h>

typedef int (*file_exec_t)(void); // An executable file

int file_open(char* name, int addr);

void file_read(int fd, char* out);

void file_ls(int fd, char* out[]);

void file_write(int fd, char* data);

int file_exec(int fd);

#endif
