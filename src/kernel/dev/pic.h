#ifndef _C8_PIC
#define _C8_PIC

#include <dev/port.h>
#include <types.h>

#define PIC1 0x20
#define PIC2 0xa0

#define PIC1_OFF 0x20
#define PIC2_OFF 0x28

void pic_eoi(u32);
void pic_init(void);
void pic_mask(u8);

#endif
