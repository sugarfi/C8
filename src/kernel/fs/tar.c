#include <fs/tar.h>

u32 oct2bin(char *str, u32 size) {
    /*
     * Converts an octal string to binary. Taken from here: https://wiki.osdev.org/USTAR
     */
    u32 n = 0;
    char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

tar_file_t *tar_get_file(char *name, u32 sector) {
    /*
     * Given a filename, gets the file header from the TAR archive.
     * At some point, tar_read should use this, but it works for
     * now.
     */
    tar_file_t *file = alloc_alloc(sizeof(tar_file_t));
    while (true) {
        atapio_read(sector, 1, (u8 *) file);
        if (!(mem_cmp(file->ustar, "ustar", 5))) {
            return NULL;
        }

        u32 size = (oct2bin(file->size, 12) + 256) / 512;
        if (mem_cmp(file->name, name, mem_len(file->name))) {
            return file;
        }

        sector += 1 + size;
    }
}

bool tar_read(char *name, u32 sector, char* buf) {
    /*
     * Reads a single file into a buffer, given the starting sector of the TAR archive.
     */
    tar_file_t *file;
    while (true) {
        atapio_read(sector, 1, (u8 *) file); // Read the file header
        if (!(mem_cmp(file->ustar, "ustar", 5))) { // If the file has no USTAR signature, we have reached the end of the archive
            return false;
        }
        u32 size = (oct2bin(file->size, 12) + 256) / 512; // Get the size in bytes and convert it to sectors.
        if (mem_cmp(file->name, name, mem_len(file->name))) { // The name matches so we have found our file
            if (file->type == '1' || file->type == '2') { // If the type byte is 1 or 2 the file is a link
                return tar_read(file->linked, sector, buf);
            } else if (file->type == '5') { // If the file type is 5 it is a directory so we should fail automatically
                return false;
            } // Otherwise the file is a normal file
            u32 i;
            ++sector; // Skip over the file header sector
            for (i = 0; i < size; i++) {
                atapio_read(sector++, 1, buf); // Read each sector of the file, one by one, into the buffer.
                /*
                 * /!\ IMPORTANT: /!\
                 * This is *definitely* a terrible way to do this. ATAPIO mode is slow and locks up the CPU,
                 * so if this is used in multitasking mode it will halt all other processes. But, that's a long
                 * way off and I hope we can have a better TAR implementation by then.
                 */
                buf += 512;
            }
            return true;
        }
        sector += 1 + size; // Skip the file if it wasn't a match.
    }
}

bool tar_read_dir(char *name, u32 sector, char **buf) {
    tar_file_t *file = alloc_alloc(sizeof(tar_file_t));
    u32 i = 0;
    while (true) {
        atapio_read(sector, 1, (u8 *) file);
        if (!(mem_cmp(file->ustar, "ustar", 5))) {
            return true;
        }

        u32 size = (oct2bin(file->size, 12) + 256) / 512;

        if (mem_cmp(name, file->name, mem_len(name) - 1) && (mem_len(file->name) > mem_len(name))) {
            buf[i] = alloc_alloc(mem_len(file->name) + 1);
            mem_cpy(buf[i], file->name, mem_len(file->name));
            buf[i][mem_len(file->name)] = 0;
            ++i;
        }
        sector += 1 + size;
    }
}

u32 tar_count(char *name, u32 sector) {
    tar_file_t *file = alloc_alloc(sizeof(tar_file_t));
    u32 i = 0;
    while (true) {
        atapio_read(sector, 1, (u8 *) file);
        if (!(mem_cmp(file->ustar, "ustar", 5))) {
            alloc_free(file);
            return i;
        }

        u32 size = (oct2bin(file->size, 12) + 256) / 512;
        
        if (mem_cmp(name, file->name, mem_len(name) - 1) && (mem_len(file->name) > mem_len(name))) {
            ++i;
        }
        sector += 1 + size;
    }
}
