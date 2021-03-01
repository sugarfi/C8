#include <sys/sysint.h>

__attribute__((interrupt)) void sys_irq(void* frame) {
    /*
     * This function is invoked whenever a syscall happens, ie. int 0x80.
     */
    ib_t *ib = (ib_t *) 0x500; // Get the information block

    register u32 eax __asm__("eax"); // Get all the registers, to pass to the handler.
    register u32 ebx __asm__("ebx");
    register u32 ecx __asm__("ecx");
    register u32 edx __asm__("edx");
    register u32 esi __asm__("esi");
    register u32 edi __asm__("edi");

    if (eax >= 1024) { // Syscalls above 1024 are invalid.
        kdbg_error("Syscall number invalid");
        eax = -2;
        return;
    }

    systable_entry_t *systable = (systable_entry_t *) ib->systable;
    systable_entry_t call = systable[eax]; // We want to extract the correct syscall to invoke

    if (!(call.flags & 1)) {
        kdbg_error("Syscall not present"); // We will error if we the syscall doesn't exist, and return -1
        ib->res = -1;
        return;
    }

    ib->res = call.func(eax, ebx, ecx, edx, esi, edi); // The result of the system call is stored in ib->res
    pic_eoi(0x80);
}
