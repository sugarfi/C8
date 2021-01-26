#include <sys/syscall.h>

u32 syscall_read(u32 a, u32 b, u32 c, u32 d, u32 si, u32 di) {
    /*
     * Invoked on int $0x80, eax=0
     */
    ib_t *ib = (ib_t *) 0x3ff;
    vfs_dir_t *root = (vfs_dir_t *) ib->root;
    vfs_file_t *file = vfs_get_handle((char *) si, root);
    if (file == NULL) {
        return 0;
    }
    return (u32) vfs_read(file, (char *) di);
}
