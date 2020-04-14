#ifndef _C8_ENTRY
#define _C8_ENTRY

#include <fs/file.h>
#include <fs/ops.h>
#include <lib/kstdio.h>
#include <mem/page.h>
#include <mem/malloc.h>

typedef int (*syscall_t)(int, int, int, int, int, int);

int syscall_read(int a, int b, int c, int d, int si, int di) {
    file_read(si, (char*) di);
    return 0;
}

int syscall_write(int a, int b, int c, int d, int si, int di) {
    file_write(di, (char*) si);
    return 0;
}

int syscall_open(int a, int b, int c, int d, int si, int di) {
    char* name = (char*) si;
    return file_open(name, FILE_ADDR);
}

int syscall_map(int a, int b, int c, int d, int si, int di) {
    page_map(page_table, si, di, d);
    return 0;
}

int syscall_malloc(int a, int b, int c, int d, int si, int di) {
    int addr = (unsigned int) kmalloc(c);
    return addr;
}

int syscall_free(int a, int b, int c, int d, int si, int di) {
    kfree((void*) b);
    return 0;
}

syscall_t SYSCALLS[] = {
    &syscall_read,
    &syscall_write,
    &syscall_open,
    &syscall_map,
    &syscall_malloc,
    &syscall_free,
};

int syscall_entry(int a) {
    register int b __asm__("ebx");
    register int c __asm__("ecx");
    register int d __asm__("edx");
    register int si __asm__("esi");
    register int di __asm__("edi");

    return SYSCALLS[a](a, b, c, d, si, di);
}

#endif
