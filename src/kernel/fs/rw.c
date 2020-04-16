#include <fs/rw.h>

void file_read(file_t* file, char* buffer) {
    memcpy(buffer, file->content, oct2bin(file->size, 11));
    buffer[oct2bin(file->size, 11)] = 0;
}

void file_write(file_t* file, char* data) {
    int size = strlen(data);
    int i = 10;
    while (size) {
        file->size[i] = size % 8 + '0';
        size /= 8;
        i--;
    }
    memcpy(file->content, data, strlen(data));
    file->content[strlen(data)] = 0;
}
