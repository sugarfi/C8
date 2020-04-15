#include <fs/dev.h>

void dev_nop(char* data)
{
    return;
}

void dev_init(int fd, dev_t dev)
{
    devs[fd] = dev;
    open[fd] = (int) &dev;
    file_next_fd = fd + 1;
}
