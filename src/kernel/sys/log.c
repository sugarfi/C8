#include <sys/log.h>

typedef struct
{
    char* name;
    short color;
} log_level_t;

static const log_level_t LOG_LEVELS[] =
{
    { "INFO", 0x07 },
    { "WARN", 0x0e },
    { "ERROR", 0x04 },
    { "FATAL", 0x47 },
};

void log(char* s, int level)
{
    char* name = LOG_LEVELS[level].name;
    short color = LOG_LEVELS[level].color;
    vga_puts(name, color);
    vga_puts(": ", color);
    vga_puts(s, color);
}
