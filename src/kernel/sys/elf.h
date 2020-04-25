#ifndef _C8_ELF
#define _C8_ELF

typedef unsigned int elf_addr;
typedef unsigned int elf_off;
typedef unsigned short elf_section;
typedef unsigned short elf_versym;
typedef unsigned char elf_byte;
typedef unsigned short elf_half;
typedef int elf_sword;
typedef unsigned int elf_word;
typedef long elf_sxword;
typedef unsigned long elf_xword;

typedef struct {
    unsigned char magic[16];
    elf_half type;
    elf_half machine;
    elf_word version;
    elf_addr entry;
    elf_off phoff;
    elf_off shoff;
    elf_word flags;
    elf_half ehsize;
    elf_half phentsize;
    elf_half phnum;
    elf_half shentsize;
    elf_half shnum;
    elf_half shstrndx;
} elf_header_t;

typedef struct {
    elf_word name;
    elf_word type;
    elf_word flags;
    elf_addr addr;
    elf_off offset;
    elf_word size;
    elf_word link;
    elf_word info;
    elf_word addralign;
    elf_word entsize;
} elf_section_t;

typedef struct {
    elf_addr offset;
    elf_word info;
} elf_rel_t;

typedef struct {
    elf_word name;
    elf_addr value;
    elf_word size;
    unsigned char info;
    unsigned char other;
    elf_half shndx;
} elf_sym_t;

#endif
