#ifndef _C8_ENTRY
#define _C8_ENTRY

#include <fs/file.h>
#include <fs/ops.h>
#include <lib/kstdio.h>
#include <mem/page.h>
#include <mem/malloc.h>

typedef int (*syscall_t)(int, int, int, int, int, int);

int syscall_read(int a, int b, int c, int d, int si, int di);

int syscall_write(int a, int b, int c, int d, int si, int di);

int syscall_open(int a, int b, int c, int d, int si, int di);

int syscall_map(int a, int b, int c, int d, int si, int di);

int syscall_malloc(int a, int b, int c, int d, int si, int di);

int syscall_free(int a, int b, int c, int d, int si, int di);

syscall_t SYSCALLS[] = {
    &syscall_read,
    &syscall_write,
    &syscall_open,
    &syscall_map,
    &syscall_malloc,
    &syscall_free,
};

int syscall_entry(int a);

#endif
