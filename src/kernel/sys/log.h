#ifndef _C8_LOG
#define _C8_LOG

#include <dev/vga.h>

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_FATAL 3

#define log_info(s) log(s, LOG_INFO)
#define log_warn(s) log(s, LOG_WARN)
#define log_error(s) log(s, LOG_ERROR)
#define log_fatal(s) log(s, LOG_FATAL)

typedef struct
{
    char* name;
    short color;
} log_level_t;

const log_level_t LOG_LEVELS[] =
{
    { "INFO", 0x07 },
    { "WARN", 0x0e },
    { "ERROR", 0x04 },
    { "FATAL", 0x47 },
};

void log(char* s, int level);

#endif
