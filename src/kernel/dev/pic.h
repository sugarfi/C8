#ifndef _C8_PIC
#define _C8_PIC

#include <dev/port.h>
#include <types.h>

#define PIC1 0x20
#define PIC2 0xa0

// Begin OSDev wiki copypasta
#define ICW1_ICW4   0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL  0x08        /* Level triggered (edge) mode */
#define ICW1_INIT   0x10        /* Initialization - required! */
 
#define ICW4_8086   0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO   0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C        /* Buffered mode/master */
#define ICW4_SFNM   0x10        /* Special fully nested (not) */
// End copypasta

void pic_eoi(u32);
void pic_init(u32, u32);

#endif
