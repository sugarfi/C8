#ifndef _C8_OPS
#define _C8_OPS

/*
ops.h - Common file operations.
*/

#include <mem/page.h>
#include <lib/math.h>
#include <mem/malloc.h>
#include <lib/kstdio.h>
#include <sys/elf.h>
#include <fs/dev.h>

typedef int (*file_exec_t)(void); // An executable file

int file_open(char* name, int addr) {
    /*
    Opens a file and returns it's file descriptor.
    */
    while (1) {
        file_t file = file_get(addr); // Get a binary file entry
        if (file.name[0] == '\0') { // If the first character of the name is '\0', we have reached the end of the files
            log_error("Attempted to open nonexistent file\n");
            return 1023; // 1023 is the file descriptor for nonexist
        }
        if (strncmp(name, file.name, strlen(name))) {  // Check if the name is the same
            open[file_next_fd] = addr; // Store the file
            file_next_fd++;
            return file_next_fd - 1;
        }
        int size = oct2bin(file.size, 11);
        addr += (((size + 511) / 512) + 1) * 512; // Otherwise, go to the next file
    }
}

void file_read(int fd, char* out) {
    /*
    Given a file descriptor, reads a file into a buffer
    */

    int in;
    for (in = 0; in < FILE_LIMIT; in++) {
        if (devs[in].fd == fd) {
            devs[fd].read(out);
            return;
        }
    }

    int addr = open[fd]; // Get a file object
    file_t file = file_get(addr);
    if (file.name[0] == '\0') { // Don't read nonexistent files
        log_error("Attempted to read nonexistent file\n");
        return;
    }
    out[oct2bin(file.size, 11)] = '\0';
    memcpy(out, (char*) addr + FILE_SIZE, oct2bin(file.size, 11));
}

void file_ls(int fd, char* out[]) {
    int addr = open[fd]; // Get a file object
    file_t file = file_get(addr);
    addr += FILE_SIZE;
    file_t this;
    int i = 0;

    while (1) {
        this = file_get(addr);
        if (this.name[0] == '\0') {
            return;
        }
        if (!strcmp(file.name, this.name)) {
            return;
        }
        out[i] = kmalloc(100);
        memcpy(out[i], this.name, 100);
        int size = oct2bin(this.size, 11);
        addr += (((size + 511) / 512) + 1) * 512;
        i++;
    }
}

void file_write(int fd, char* data) {
    int in;
    for (in = 0; in  < FILE_LIMIT; in++) {
        if (devs[in].fd == fd) {
            devs[in].write(data);
            return;
        }
    }

    int addr = open[fd]; // Get a file object
    char* p = (char*) addr;
    file_t file = file_get(addr);
    memcpy(p + FILE_SIZE, data, strlen(data));

    int i = strlen(data);
    int this;
    char* digits = "01234567";
    int index;
    for (index = 124; index < 124 + 10; index++) {
        p[index] = '0';
    }
    index = 124 + 10;

    while (i) {
        this = i % 8;
        p[index] = digits[this];
        index--;
        i /= 8;
    }
}

int file_exec(int fd) {
    char buffer[0x10000];
    file_read(fd, buffer);
    elf_t* elf = (elf_t*) buffer;
    elf_shent_t* sections = (elf_shent_t*) (((int) buffer) + elf->shoff);
    elf_shent_t code;
    elf_shent_t data;
    int i;

    file_exec_t func = (file_exec_t) (((int) buffer) + sections[1].offset);

    return func();
}

#endif
