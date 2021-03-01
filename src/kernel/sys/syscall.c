#include <sys/syscall.h>

u32 __attribute__((no_caller_saved_registers)) syscall_open (u32 a, u32 b, u32 c, u32 d, u32 si, u32 di) {
    /*
     * Invoked on int $0x80, eax=0
     */
    ib_t *ib = (ib_t *) 0x500;
    vfs_dir_t *root = (vfs_dir_t *) ib->root;
    vfs_file_t *file = vfs_get_handle((char *) si, root);
    u32 fd = ib->next_fd++;
    if (file == NULL) {
        return 0;
    }
    ib->open[fd] = (u32) file;
    return fd + 1;
}
 
u32 __attribute__((no_caller_saved_registers)) syscall_read(u32 a, u32 b, u32 c, u32 d, u32 si, u32 di) {
    /*
     * Invoked on int $0x80, eax=1
     */
    ib_t *ib = (ib_t *) 0x500;
    vfs_file_t *file = (vfs_file_t *) ib->open[si - 1];
    return (u32) vfs_read(file, (char *) di);
}

u32 __attribute__((no_caller_saved_registers)) syscall_write(u32 a, u32 b, u32 c, u32 d, u32 si, u32 di) {
    /*
     * Invoked on int $0x80, eax=2
     */
    ib_t *ib = (ib_t *) 0x500;
    vfs_dir_t *root = (vfs_dir_t *) ib->root;
    vfs_file_t *file = (vfs_file_t *) ib->open[di - 1];
    vfs_write((char *) si, file, c);
    return 1;
}
