#ifndef _C8_SYSCALL
#define _C8_SYSCALL

#include <types.h>
#include <fs/tar.h>
#include <info/ib.h>
#include <info/mbr.h>

u32 syscall_read(u32, u32, u32, u32, u32, u32);

#endif
