#include <int/idt.h>

void idt_load(idt_desc_t* table)
{
    /*
    Loads the IDT given its descriptor.
    */
    __asm__ __volatile__("lidtl (%0)" : : "r" (table)); // Load the IDT
}

idt_entry_t idt_make_entry(int addr, short selector, char type)
{
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