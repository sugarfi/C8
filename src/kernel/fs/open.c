#include <fs/open.h>
#include <lib/kstdio.h>

void file_open(char* name, file_t** out) {
   char* addr = (char*) FILE_ADDR;
   while (1) {
       file_raw_t* file = (file_raw_t*) addr;
       if (file->name[0] == 0) {
           log_error("Error: file does not exist");
           *out = (file_t*) 0;
           return;
       }
       if (strcmp(file->name, name) == 0) {
           memcpy((*out)->name, file->name, strlen(file->name));
           memcpy((*out)->mode, file->mode, strlen(file->mode));
           memcpy((*out)->uid, file->uid, strlen(file->uid));
           memcpy((*out)->gid, file->gid, strlen(file->gid));
           memcpy((*out)->size, file->size, strlen(file->size));
           memcpy((*out)->time, file->time, strlen(file->time));
           memcpy((*out)->checksum, file->checksum, strlen(file->checksum));
           (*out)->type = file->type;
           memcpy((*out)->linked, file->linked, strlen(file->linked));
           memcpy((*out)->ustar, file->ustar, strlen(file->ustar));
           memcpy((*out)->version, file->version, strlen(file->version));
           memcpy((*out)->owner, file->owner, strlen(file->owner));
           memcpy((*out)->user, file->user, strlen(file->user));
           memcpy((*out)->major, file->major, strlen(file->major));
           memcpy((*out)->minor, file->minor, strlen(file->minor));
           memcpy((*out)->prefix, file->prefix, strlen(file->prefix));
           (*out)->content = addr + 512;
           return;
       }
       addr += (((oct2bin(file->size, 11) + 511) / 512) + 1) * 512;
   }
}
