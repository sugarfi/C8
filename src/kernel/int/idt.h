/*
 * idt.h - utilities for managing the IDT
*/
#ifndef _C8_IDT
#define _C8_IDT

#include <sys/port.h>


typedef struct
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type;
    unsigned short offset_high;
} __attribute__((packed)) idt_entry_t; // One entry in the IDT

typedef struct
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_desc_t; // The IDT descriptor

void idt_load(idt_desc_t* table);

idt_entry_t idt_make_entry(int addr, short selector, char type);

#endif
