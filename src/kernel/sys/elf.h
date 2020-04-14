#ifndef _C8_ELF
#define _C8_ELF

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
    elf_word entry;
    elf_word phoff;
    elf_word shoff;
    elf_word flags;
    elf_half ehsize;
    elf_half phentsize;
    elf_half phnum;
    elf_half shentsize;
    elf_half shnum;
    elf_half shstrndx;
} __attribute__((packed)) elf_t;

typedef struct {
    elf_word name;
    elf_word type;
    elf_word flags;
    elf_word addr;
    elf_word offset;
    elf_word size;
    elf_word link;
    elf_word info;
    elf_word addralign;
    elf_word entsize;
} __attribute__((packed)) elf_shent_t;

#endif
