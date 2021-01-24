#include <sys/syscall.h>

u32 syscall_read(u32 a, u32 b, u32 c, u32 d, u32 si, u32 di) {
    /*
     * Invoked on int $0x80, eax=0
     */
    ib_t *ib = (ib_t *) 0x3ff;
    mbr_t *mbr = (mbr_t *) ib->mbr; // Read the IB from 0x3ff and extract the MBR struct
    return (u32) tar_read((char *) si, mbr->part2.start_lba + 1, (char *) di); // This value is stored in IB_RES, so it will be 1 (ie., true) if the read was successful
}
