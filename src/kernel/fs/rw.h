#ifndef _C8_RW
#define _C8_RW

#include <lib/math.h>
#include <sys/log.h>
#include <fs/file.h>
#include <lib/string.h>

void file_read(file_t*, char*);
void file_write(file_t*, char*);

#endif
