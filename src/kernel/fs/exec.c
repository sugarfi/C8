#include <fs/exec.h>
#include <lib/kstdio.h>

void file_exec(file_t* file, unsigned int page_table[1024]) {
    char buffer[oct2bin(file->size, 11)];
    file_read(file, buffer);

    elf_header_t* header = (elf_header_t*) ((int) buffer);

    elf_section_t* text;
    for (text = (elf_section_t*) ((int) (buffer + header->shoff)); text->type != 1; text++);
    elf_section_t* data = (elf_section_t*) ((int) (buffer + header->shoff));
    data += 2;
    for (; data->type != 1; data++);

    memcpy((char*) 0x300000, (buffer + data->offset), data->size);
    page_map(page_table, 0, 0x300000, 0x03);
    file_exec_t func = (file_exec_t) ((int) (buffer + text->offset));
    func();
    page_map(page_table, 0, 0, 0x03);
}
