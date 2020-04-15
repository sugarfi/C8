#ifndef _C8_SYSCALL
#define _C8_SYSCALL

typedef int (*syscall_t)(int, int, int, int, int, int);

void syscall_init();

int syscall_read(int a, int b, int c, int d, int si, int di);

int syscall_write(int a, int b, int c, int d, int si, int di);

int syscall_open(int a, int b, int c, int d, int si, int di);

int syscall_map(int a, int b, int c, int d, int si, int di);

int syscall_malloc(int a, int b, int c, int d, int si, int di);

int syscall_free(int a, int b, int c, int d, int si, int di);

syscall_t SYSCALLS[6];

int syscall_entry(int a);

#endif
