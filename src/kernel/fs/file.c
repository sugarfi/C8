#include <fs/file.h>

file_t file_get(int addr)
{
    /*
    Converts the specified address to a file binary entry. Assumes it points to a valid entry.
    */
    file_t* file = (file_t*) addr; // Make a pointer
    return *file; // Return it
}
