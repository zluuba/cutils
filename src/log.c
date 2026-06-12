#include "log.h"

#include <stdarg.h>
#include <stdio.h>

void generic_log(LogLevel min_lvl, LogLevel lvl, const char *fmt, ...) {
    if (lvl < min_lvl) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
