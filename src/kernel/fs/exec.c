#include <fs/exec.h>
#include <lib/kstdio.h>

void file_exec(file_t* file, unsigned int page_table[1024]) {
    char buffer[oct2bin(file->size, 11)];
    file_read(file, buffer);

    elf_header_t* header = (elf_header_t*) ((int) buffer);

    elf_section_t* sections = (elf_section_t*) ((int) (buffer + header->shoff));
    elf_section_t text;
    elf_section_t rel;
    elf_section_t symtab;
    int index;

    index = 0;
    while (text.type != 1) {
        text = sections[index];
        index++;
    }

    index = 0;
    while (rel.type != 9) {
        rel = sections[index];
        index++;
    }

    index = 0;
    while (symtab.type != 2) {
        symtab = sections[index];
        index++;
    }

    elf_sym_t* symbols = (elf_sym_t*) ((int) (buffer + symtab.offset));

    int reln;
    elf_rel_t* rels = (elf_rel_t*) ((int) (buffer + rel.offset));

    for (reln = 0; reln < rel.size / rel.entsize; reln++) {
        elf_rel_t this = rels[reln];

        char symn = this.info >> 8;
        char type = this.info & 0xff;
        elf_sym_t sym = symbols[symn];
        int val;
        int* rel = (int*) ((int) buffer + this.offset + text.offset);

        if ((sym.info & 0x0f) == 3) {
            elf_section_t section = sections[sym.shndx];
            if (type == 1) {
                val = section.offset;
            } else {
                val = section.offset + *rel - (((int) rel) - ((int) buffer));
            }
            *rel = val;
        } else {
            if (type == 1) {
                val = text.offset + sym.value;
            } else {
                val = (text.offset + sym.value) + *rel - (((int) rel) - ((int) buffer));
            }
            *rel = val;
        }
    }

    int entry = symbols[(symtab.size / symtab.entsize) - 1].value;

    memcpy((char*) 0x300000, buffer, oct2bin(file->size, 11));
    page_map(page_table, 0, 0x300000, 0x03);
    page_map(page_table, 0x1000, 0x301000, 0x03);
    file_exec_t func = (file_exec_t) ((int) (text.offset + entry));
    func();
    page_map(page_table, 0, 0, 0x03);
    page_map(page_table, 0x1000, 0x1000, 0x03);
}
