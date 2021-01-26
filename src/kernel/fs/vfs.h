#ifndef _C8_VFS
#define _C8_VFS

#include <fs/tar.h>
#include <types.h>
#include <mem/alloc.h>
#include <mem/mem.h>

#define VFS_SECTOR 65 // Add a method or smth for this later

struct vfs_dir;

typedef struct __attribute__((packed)) vfs_file  {
    char *name;
    char *content;
    u32 size;
    struct vfs_file *sibling;
    struct vfs_dir* parent;
} vfs_file_t;

typedef struct __attribute__((packed)) vfs_dir {
    char *name;
    vfs_file_t *first_file;
    struct vfs_dir *first_dir;
    struct vfs_dir *sibling;
    struct vfs_dir* parent;
} vfs_dir_t;

vfs_file_t *vfs_get_handle(char *, vfs_dir_t *);
bool vfs_read(vfs_file_t *, char *);
bool vfs_write(char *, vfs_file_t *, u32);
void vfs_populate(vfs_dir_t *);
void vfs_flush(vfs_dir_t *);
void vfs_free(vfs_dir_t *);
void vfs_add_file(vfs_file_t *, vfs_dir_t *);
vfs_dir_t *vfs_make_dir(char *);
vfs_file_t *vfs_make_file(char *);

#endif
