#ifndef _C8_FAT16
#define _C8_FAT16
#include <types.h>

typedef struct __attribute__((packed)) {
    u8 jmp[3];
    u8 oem[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved;
    u8 number_of_fats;
    u16 root_dir_entries;
    u16 total_sectors;
    u8 media_byte;
    u16 sectors_per_fat;
    u16 sectors_per_track;
    u16 number_of_heads;
    u32 hidden_sectors;
    u32 large_sectors;
    u8 drive_number;
    u8 nt_flags;
    u8 signature;
    u32 serial;
    u8 volume_label[11];
    u8 fs_type[8];
    u8 boot_code[448];
    u16 sig;
} fat16_bpb_t;

typedef struct __attribute__((packed)) {
    char name[8];
    char ext[3];
    u8 attrs;
    u8 nt_reserved;
    u8 create_time;
    u16 create_date;
    u16 last_accessed_date;
    u16 cluster_hi;
    u16 last_mod_time;
    u16 last_mod_date;
    u16 cluster_lo;
    u32 size;
} fat16_83_entry_t;

#endif
