#include "vfs.h"

vfs_file_t *vfs_get_handle(char *name, vfs_dir_t *dir) {
    /*
     * Given a name and a directory, returns the first child of that directory with that file name.
     */
    vfs_file_t *file = dir->first_file;
    while (!mem_cmp(file->name, name, mem_len(file->name))) {
        file = file->sibling;
        if (file == NULL) {
            return NULL;
        }
    }
    return file;
}

bool vfs_internal_read(vfs_file_t *file) {
    /*
     * Used to read a file from the disk if its contents is not already cached in memory.
     */
    tar_file_t* ifile = tar_get_file(file->name, VFS_SECTOR);
    if (ifile == NULL) {
        return false;
    }
    u32 size = oct2bin(ifile->size, 11);
    file->content = alloc_alloc(size);
    file->size = size;
    char *buf = alloc_alloc(size + 512); // Quick and dirty round up
    tar_read(file->name, VFS_SECTOR, buf);
    mem_cpy(file->content, buf, size);

    return true;
}

bool vfs_read(vfs_file_t *file, char *buf) {
    /*
     * Reads a file from the VFS. If the file contents has been cached in memory, it simply returns
     * that - otherwise it uses ATAPIO to read it from the disk.
     */
    if (file->size == 0) {
        if (!vfs_internal_read(file)) {
            kdbg_error("Internal read failed");
            return false;
        };
        mem_cpy(buf, file->content, file->size);
        buf[file->size] = 0;
    } else {
        mem_cpy(buf, file->content, file->size);
        buf[file->size] = 0;
    }

    return true;
}

bool vfs_write(char *data, vfs_file_t *file, u32 size) {
    /*
     * Writes data to the file's in-memory contents buffer. vfs_flush() can be used to write the
     * data to the disk.
     */
    if (file->size != 0) {
        alloc_free(file->content);
    }
    file->content = alloc_alloc(size + 1);
    file->size = size;
    mem_cpy(file->content, data, size);
    file->content[size] = 0;
    return true;
}

bool vfs_internal_write(vfs_file_t *file) {
    /*
     * Writes data from memory to the disk.
     */
    if (file->size > 0) {
        return tar_write(file->name, file->content, file->size, VFS_SECTOR);
    } else {
        return true;
    }
}

void vfs_flush(vfs_dir_t *dir) {
    /*
     * Flushes all in-memory data to the disk, and resets all data buffers.
     * (at the moment everything breaks if you all this so)
     */
    vfs_file_t *file;
    for (file = dir->first_file; file != NULL; file = file->sibling) {
        if (!vfs_internal_write(file)) {
            kdbg_error("Internal write failed");
        };
    }

    vfs_dir_t *dir2 = dir->first_dir;
    while (dir2 != NULL) {
        vfs_flush(dir2);
        dir2 = dir2->sibling;
    }
}   

void vfs_add_file(vfs_file_t *add, vfs_dir_t *dir) {
    /*
     * Adds a file as a child of a directory.
     */
    if (dir->first_file == NULL) { // If the directory has no files we'll just write the file
        dir->first_file = add;
        add->parent = dir;
        return;
    }        
    vfs_file_t *file = dir->first_file;
    while (file->sibling != NULL) {
        file = file->sibling;
    }
    file->sibling = add;
    add->parent = dir;
}

void vfs_add_dir(vfs_dir_t *add, vfs_dir_t *dir) {
    /*
     * Adds a directory as a child of another directory.
     */
    if (dir->first_dir == NULL) { // See above
        dir->first_dir = add;
        add->parent = dir;
        return;
    }        
    vfs_dir_t *dir2 = dir->first_dir;
    while (dir2->sibling != NULL) {
        dir2 = dir2->sibling;
    }
    dir2->sibling = add;
    add->parent = dir;
}

void vfs_free(vfs_dir_t *dir) {
    /*
     * Frees all the memory used by files in a vfs structure.
     */
    vfs_file_t *file = dir->first_file;
    while (file != NULL) {
        file->size = 0;
        alloc_free(file->name); // Assumes all files were created with vfs_make_file or similar
        alloc_free(file->content);
        file = file->sibling;
    }
    
    vfs_dir_t *dir2 = dir->first_dir;
    while (dir2 != NULL) {
        vfs_free(dir2);
        dir2 = dir2->sibling;
    }
}

void vfs_populate(vfs_dir_t *dir) {
    /*
     * Given a directory, finds all files on the disk that are a child of it and adds them to its VFS structure.
     */
    /*
    char *path = alloc_alloc(mem_len(dir->name));
    mem_cpy(path, dir->name, mem_len(dir->name)); // Get the directory name in a spot where we can edit it
    char *path2;

    vfs_dir_t *dir2 = dir->parent;
    while (dir2 != NULL) { // We want to prefix the path with every parent of the directory
        path2 = alloc_alloc(mem_len(path) + 1 + mem_len(dir2->name)); // Enough space for <x>/<y>
        mem_cat(path2, dir2->name);
        mem_cat(path2, "/");
        mem_cat(path2, path); // Add all the parts
        path = alloc_alloc(mem_len(path2) + 2); // I forget why the + 2 is here but it's probably important
        mem_cpy(path, path2, mem_len(path2));
        dir2 = dir2->parent;
    }
    path2 = alloc_alloc(mem_len(path) + 1); // This code looks complicated but it's just adding a / to the end of the path
    mem_cpy(path2, path, mem_len(path));
    path2[mem_len(path) - 1] = '/';
    path2[mem_len(path)] = 0;
    path = alloc_alloc(mem_len(path2));
    mem_cpy(path, path2, mem_len(path2));
    alloc_free(path2);
    */

    u32 size = tar_count(dir->name, VFS_SECTOR); // Get the number of children of the file
    tar_file_t **files = alloc_alloc(sizeof(tar_file_t *) * size);
    tar_read_dir(dir->name, VFS_SECTOR, files); // Read every child

    u32 i;
    for (i = 0; i < size; i++) {
        if (files[i]->type == '5') { // Directory
            vfs_dir_t *add = vfs_make_dir(files[i]->name);
            vfs_add_dir(add, dir);
        } else { // Something else
            vfs_file_t *file = vfs_make_file(files[i]->name);
            vfs_add_file(file, dir);
        }
        alloc_free(files[i]);
    }

    alloc_free(files); // Clean up
}

vfs_dir_t *vfs_make_dir(char *name) {
    /*
     * Creates a directory struct
     */
    vfs_dir_t *out = alloc_alloc(sizeof(vfs_dir_t));
    out->name = alloc_alloc(mem_len(name));
    out->first_file = NULL;
    out->first_dir = NULL;
    out->sibling = NULL;
    out->parent = NULL;
    mem_cpy(out->name, name, mem_len(name));
    return out;
}

vfs_file_t *vfs_make_file(char *name) {
    /*
     * Creates a file struct
     */
    vfs_file_t *out = alloc_alloc(sizeof(vfs_file_t));
    out->name = alloc_alloc(mem_len(name) + 1);
    out->sibling = NULL;
    out->parent = NULL;
    mem_cpy(out->name, name, mem_len(name));
    out->name[mem_len(name)] = 0;
    out->size = 0;
    return out;
}
