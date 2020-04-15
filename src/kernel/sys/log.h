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

void log(char* s, int level);

#endif
