#include <fs/exec.h>
#include <lib/kstdio.h>

void file_exec(file_t* file, unsigned int page_table[1024]) {
    char buffer[oct2bin(file->size, 11)];
    file_read(file, buffer);

    elf_header_t* header = (elf_header_t*) ((int) buffer);

    elf_section_t* text;
    for (text = (elf_section_t*) ((int) (buffer + header->shoff)); text->type != 1; text++);
    page_map(page_table, 0x1000, (int) buffer, 0x03);
    file_exec_t func = (file_exec_t) ((int) (buffer + text->offset));
    page_map(page_table, 0x1000, 0x1000, 0x03);
    return func();
}
