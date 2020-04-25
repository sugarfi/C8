#include <sys/syscall.h>

int syscall_read(int a, int b, int c, int d, int si, int di) {
    if (d == READ_REGULAR) {
        file_t* file = (file_t*) si;
        file_read(file, (char*) di);
        return 0;
    } else if (d == READ_DEVICE) {
        dev_read(si, (char*) di);
        return 0;
    } else {
        log_error("Invalid read operation");
        return -1;
    }
}

int syscall_write(int a, int b, int c, int d, int si, int di) {
    if (d == WRITE_REGULAR) {
        file_t* file = (file_t*) di;
        file_read(file, (char*) si);
        return 0;
    } else if (d == WRITE_DEVICE) {
        dev_write(di, (char*) si);
        return 0;
    } else {
        log_error("Invalid read operation");
        return -1;
    }
}

void syscall_init()
{
    SYSCALLS[0] = &syscall_read;
    SYSCALLS[1] = &syscall_write;
    return;
}

int syscall_entry(int a)
{
    register int b __asm__("ebx");
    register int c __asm__("ecx");
    register int d __asm__("edx");
    register int si __asm__("esi");
    register int di __asm__("edi");

    return SYSCALLS[a](a, b, c, d, si, di);
}
