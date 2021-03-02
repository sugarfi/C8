#ifndef _C8_INT
#define _C8_INT

#include <types.h>
#include <kdbg/kdbg.h>
#include <dev/pic.h>
#include <kdbg/trace.h>

typedef struct __attribute__((packed)) {
    u32 ip;
    u32 cs;
    u32 flags;
    u32 sp;
    u32 ss;
} idt_frame_t;

typedef void (*isr_f_t)(idt_frame_t *);
typedef void (*isr_f_code_t)(idt_frame_t *, u32);
typedef void (*irq_f_t)(void *);

typedef struct __attribute__((packed)) {
    u16 offset_lo;
    u16 selector;
    u8 zero;
    u8 type;
    u16 offset_hi;
} idt_entry_t;

typedef struct __attribute__((packed)) {
    u16 size;
    u32 ptr;
} idt_desc_t;

void idt_load(idt_desc_t *desc);
void idt_fill(idt_entry_t[256]);

#endif
