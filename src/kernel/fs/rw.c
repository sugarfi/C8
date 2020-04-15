#include <fs/rw.h>
#include <lib/kstdio.h>

void file_read(char* name, char* out) {
    char* addr = (char*) FILE_ADDR;
    file_t* file;
    int size;
    while (1) {
        file = (file_t*) ((int) addr);
        size = oct2bin(file->size, 11);
        if (file->name[0] == '\0') {
            log_error("Attempted to read nonexistent file");
            return;
        }
        if (strcmp(file->name, name) == 0) {
            memcpy(out, addr + 512, size);
            out[size] = '\0';
            return;
        }
        addr += (((size + 511) / 512) + 1) * 512;
    }
}

void file_write(char* name, char* data) {
    char* addr = (char*) FILE_ADDR;
    file_t* file;
    int size;
    while (1) {
        file = (file_t*) ((int) addr);
        size = oct2bin(file->size, 11);
        if (file->name[0] == '\0') {
            log_error("Attempted to write nonexistent file");
            return;
        }
        if (strcmp(file->name, name) == 0) {
            size = strlen(data);
            int i;
            for (i = 0; i < 11; i++) {
                file->size[i] = '0';
            }
            int index = 10;
            while (size) {
                file->size[index] = (size % 8) + '0';
                size /= 8;
                index--;
            }
            addr[512 + oct2bin(file->size, 11)] = '\0';
            memcpy(addr + 512, data, oct2bin(file->size, 11));
            return;
        }
        addr += (((size + 511) / 512) + 1) * 512;
    }
}
