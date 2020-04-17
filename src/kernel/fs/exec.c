#include <fs/exec.h>
#include <lib/kstdio.h>

void file_exec(file_t* file) {
    char buffer[oct2bin(file->size, 11)];
    file_read(file, buffer);

    elf_header_t* header = (elf_header_t*) ((int) buffer);

    elf_section_t* text;
    for (text = (elf_section_t*) ((int) (buffer + header->shoff)); text->type != 1; text++);
    file_exec_t func = (file_exec_t) ((int) (buffer + text->offset));
    return func();
}
