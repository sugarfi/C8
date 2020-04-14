#ifndef _C8_IDT
#define _C8_IDT

/*
idt.h - utilities for managing the IDT
*/

#include <sys/port.h>

typedef struct {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type;
    unsigned short offset_high;
} __attribute__((packed)) idt_entry_t; // One entry in the IDT

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_desc_t; // The IDT descriptor

static idt_entry_t idt[256]; // The actual IDT
static idt_desc_t idt_desc; // The descriptor

void idt_load(idt_desc_t* table) {
    /*
    Loads the IDT given its descriptor.
    */
    __asm__ __volatile__("lidtl (%0)" : : "r" (table)); // Load the IDT
}

idt_entry_t idt_make_entry(int addr, short selector, char type) {
    /*
    Creates an IDT entry.
    */
    idt_entry_t entry;
    entry.offset_low = addr & 0x0000ffff; // Lower 16 bits of the address
    entry.selector = selector; // Selector
    entry.zero = 0;
    entry.type = type; // Type
    entry.offset_high = (addr & 0xffff0000) >> 16; // High 16 bits
    return entry;
}

#endif
