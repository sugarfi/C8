#ifndef _C8_SYSCALL
#define _C8_SYSCALL

#include <fs/file.h>
#include <fs/rw.h>
#include <sys/log.h>
#include <fs/dev.h>

typedef int (*syscall_t)(int, int, int, int, int, int);

typedef enum {
    READ_REGULAR,
    READ_DEVICE,
    WRITE_REGULAR,
    WRITE_DEVICE,
} syscall_file_op_t;

void syscall_init();

int syscall_read(int a, int b, int c, int d, int si, int di);

int syscall_write(int a, int b, int c, int d, int si, int di);

int syscall_open(int a, int b, int c, int d, int si, int di);

syscall_t SYSCALLS[6];

int syscall_entry(int a);

#endif
