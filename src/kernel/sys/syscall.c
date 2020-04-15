#include <sys/syscall.h>

void syscall_init()
{
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
