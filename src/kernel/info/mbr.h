#ifndef _C8_MBR
#define _C8_MBR
#include <types.h>

typedef struct __attribute__((packed)) {
    u8 active;
    u8 start_h;
    u16 start_cs;
    u8 id;
    u8 end_h;
    u16 end_cs;
    u32 start_lba;
    u32 count_lba;
} mbr_entry_t;

typedef struct __attribute__((packed)) {
    u32 sig;
    u16 zero;
    mbr_entry_t part1;
    mbr_entry_t part2;
    mbr_entry_t part3;
    mbr_entry_t part4;
    u16 boot_sig;
} mbr_t;

#endif
