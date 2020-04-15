#include <sys/log.h>

void log(char* s, int level)
{
    char* name = LOG_LEVELS[level].name;
    short color = LOG_LEVELS[level].color;
    vga_puts(name, color);
    vga_puts(": ", color);
    vga_puts(s, color);
}
