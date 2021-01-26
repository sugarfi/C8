#include "vfs.h"

vfs_file_t *vfs_get_handle(char *name, vfs_dir_t *dir) {
    vfs_file_t *file;
    for (file = dir->first_file; file == NULL || mem_cmp(name, file->name, mem_len(name)) == 0; file = file->sibling);
    return file;
}

bool vfs_internal_read(vfs_file_t *file) {
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
    kdbg_error("writing is not implemented yet.");
    return false;
}

void vfs_flush(vfs_dir_t *dir) {
    vfs_file_t *file;
    for (file = dir->first_file; file == NULL; file = file->sibling) {
        vfs_internal_write(file);
    }
}   

void vfs_add_file(vfs_file_t *add, vfs_dir_t *dir) {
    if (dir->first_file == NULL) {
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

void vfs_free(vfs_dir_t *dir) {
    vfs_file_t *file;
    for (file = dir->first_file; file == NULL; file = file->sibling) {
        file->size = 0;
        alloc_free(file->content);
        file->content = "\0";
    }
}

void vfs_populate(vfs_dir_t *dir) {
    char *path = alloc_alloc(mem_len(dir->name));
    mem_cpy(path, dir->name, mem_len(dir->name));
    char *path2;

    vfs_dir_t *dir2 = dir->parent;
    while (dir2 != NULL) {
        path2 = alloc_alloc(mem_len(path) + 1 + mem_len(dir2->name));
        mem_cat(path2, dir2->name);
        mem_cat(path2, "/");
        mem_cat(path2, path);
        path = alloc_alloc(mem_len(path2) + 2);
        mem_cpy(path, path2, mem_len(path2));
        dir2 = dir2->parent;
    }
    path2 = alloc_alloc(mem_len(path) + 1);
    mem_cpy(path2, path, mem_len(path));
    path2[mem_len(path) - 1] = '/';
    path2[mem_len(path)] = 0;
    path = alloc_alloc(mem_len(path2));
    mem_cpy(path, path2, mem_len(path2));
    alloc_free(path2);

    u32 size = tar_count(path, VFS_SECTOR);
    char **files = alloc_alloc(sizeof(char *) * size);
    tar_read_dir(path, VFS_SECTOR, files);

    u32 i;
    for (i = 0; i < size; i++) {
        vfs_file_t *file = vfs_make_file(files[i]);
        alloc_free(files[i]);
        vfs_add_file(file, dir);
    }

    alloc_free(files);
    alloc_free(path);
}

vfs_dir_t *vfs_make_dir(char *name) {
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
    vfs_file_t *out = alloc_alloc(sizeof(vfs_file_t));
    out->name = alloc_alloc(mem_len(name) + 1);
    out->sibling = NULL;
    out->parent = NULL;
    mem_cpy(out->name, name, mem_len(name));
    out->name[mem_len(name)] = 0;
    return out;
}