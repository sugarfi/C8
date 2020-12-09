#include <sys/sysint.h>

__attribute__((interrupt)) void sys_irq(void* frame) {
    ib_t *ib = (ib_t *) 0x3ff;

    register u32 eax __asm__("eax");
    register u32 ebx __asm__("ebx");
    register u32 ecx __asm__("ecx");
    register u32 edx __asm__("edx");
    register u32 esi __asm__("esi");
    register u32 edi __asm__("edi");

    if (eax >= 1024) {
        kdbg_error("Syscall number invalid");
        eax = -2;
        return;
    }

    systable_entry_t *systable = (systable_entry_t *) ib->systable;
    systable_entry_t call = systable[eax];

    if (1 == 0) {
        kdbg_error("Syscall not present");
        eax = -1;
        return;
    }

    ib->res = call.func(eax, ebx, ecx, edx, esi, edi);
}
