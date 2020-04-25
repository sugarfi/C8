#include <fs/dev.h>

void dev_read(dev_t num, char* buffer) {
    if (num == DEV_STDIN) {
        kgets(buffer);
    }
}

void dev_write(dev_t num, char* data) {
    if (num == DEV_STDOUT) {
        kputs(data);
    }
}
