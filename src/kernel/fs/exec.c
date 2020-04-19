#include <fs/exec.h>
#include <lib/kstdio.h>

void file_exec(file_t* file, unsigned int page_table[1024]) {
    char buffer[oct2bin(file->size, 11)];
    file_read(file, buffer);

    elf_header_t* header = (elf_header_t*) ((int) buffer);

    elf_section_t* sections = (elf_section_t*) ((int) (buffer + header->shoff));
    elf_section_t text;
    int textn = 0;
    while (text.type != 1) {
        text = sections[textn];
        textn++;
    }
    int datan = textn;
    elf_section_t data;
    while (data.type != 1) {
        data = sections[datan];
        datan++;
    }
    int rodatan = datan;
    elf_section_t rodata;
    while (rodata.type != 1) {
        rodata = sections[rodatan];
        rodatan++;
    }
    memcpy((char*) 0x300000, (buffer + data.offset), data.size);
    memcpy((char*) 0x300000 + data.size, (buffer + rodata.offset), rodata.size);
    page_map(page_table, 0, 0x300000, 0x03);
    file_exec_t func = (file_exec_t) ((int) (buffer + text.offset));
    func();
    page_map(page_table, 0, 0, 0x03);
}
