#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "cutils/macro/for_each.h"

enum log_level {
    LOG_LVL_TRACE,
    LOG_LVL_DEBUG,
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL
};

#ifndef LOG_LVL
#define LOG_LVL LOG_LVL_WARN
#endif

static enum log_level log_unit_log_level = LOG_LVL;

#define log_trace(...) __log_generic(LOG_LVL_TRACE, ##__VA_ARGS__)
#define log_debug(...) __log_generic(LOG_LVL_DEBUG, ##__VA_ARGS__)
#define log_info(...)  __log_generic(LOG_LVL_INFO,  ##__VA_ARGS__)
#define log_warn(...)  __log_generic(LOG_LVL_WARN,  ##__VA_ARGS__)
#define log_error(...) __log_generic(LOG_LVL_ERROR, ##__VA_ARGS__)
#define log_fatal(...) __log_generic(LOG_LVL_FATAL, ##__VA_ARGS__)

#define PriFmt(fmt, ...) __PRIFmt_impl(fmt, ##__VA_ARGS__)
#define PriChar(val)             PriFmt("%c",   (val))
#define PriSignedChar(val)       PriFmt("%hhd", (val))
#define PriUnsignedChar(val)     PriFmt("%hhu", (val))
#define PriShort(val)            PriFmt("%hd",  (val))
#define PriUnsignedShort(val)    PriFmt("%hu",  (val))
#define PriInt(val)              PriFmt("%d",   (val))
#define PriUnsignedInt(val)      PriFmt("%u",   (val))
#define PriLong(val)             PriFmt("%ld",  (val))
#define PriUnsignedLong(val)     PriFmt("%lu",  (val))
#define PriLongLong(val)         PriFmt("%lld", (val))
#define PriUnsignedLongLong(val) PriFmt("%llu", (val))
#define PriFloat(val)            PriFmt("%f",   (val))
#define PriDouble(val)           PriFmt("%f",   (val))
#define PriLongDouble(val)       PriFmt("%Lf",  (val))
#define PriStr(val)              PriFmt("%s",   (val))
#define PriPtr(val)              PriFmt("%p",   (val))
#define PriBool(val)             log_entry_bool(log_entry_context, (val))
#define PriCall(func, ...)       func(log_entry_context, ##__VA_ARGS__)

#define log_custom_log_entry(func, ...) \
    log_entry_result func(log_entry_context *log_entry_context, ##__VA_ARGS__)

#define LOG_COLOR_TRACE    "\033[37m"
#define LOG_COLOR_DEBUG    "\033[32m"
#define LOG_COLOR_INFO     "\033[34m"
#define LOG_COLOR_WARN     "\033[33m"
#define LOG_COLOR_ERROR    "\033[31m"
#define LOG_COLOR_FATAL    "\033[41m"
#define LOG_COLOR_LOCATION "\033[90m"
#define LOG_COLOR_RESET    "\033[0m"

static inline const char* log_level_to_str(enum log_level lvl) {
    switch (lvl) {
        case LOG_LVL_TRACE: return LOG_COLOR_TRACE"[TRACE]"LOG_COLOR_RESET;
        case LOG_LVL_DEBUG: return LOG_COLOR_DEBUG"[DEBUG]"LOG_COLOR_RESET;
        case LOG_LVL_INFO:  return LOG_COLOR_INFO"[INFO]"LOG_COLOR_RESET;
        case LOG_LVL_WARN:  return LOG_COLOR_WARN"[WARN]"LOG_COLOR_RESET;
        case LOG_LVL_ERROR: return LOG_COLOR_ERROR"[ERROR]"LOG_COLOR_RESET;
        case LOG_LVL_FATAL: return LOG_COLOR_FATAL"[FATAL]"LOG_COLOR_RESET;
    }
    return LOG_COLOR_FATAL"[UNKNOWN]"LOG_COLOR_RESET;
}

#define __PRIFmt_impl(fmt, ...) \
    log_entry(log_entry_context, fmt, ##__VA_ARGS__)

typedef struct _log_entry_result {} log_entry_result;
typedef struct _log_entry_context {
    char *buf;
    size_t size;
    size_t capacity;
} log_entry_context;

#ifndef LOG_GET_CONTEXT_FUNC
#define LOG_BUFFER_LEN 1024
inline static log_entry_context* log_get_context() {
    static char buf[LOG_BUFFER_LEN];
    static log_entry_context log_entry_context = {};
    log_entry_context.buf = buf;
    log_entry_context.size = 0;
    log_entry_context.capacity = LOG_BUFFER_LEN;
    return &log_entry_context;
}
#define LOG_GET_CONTEXT_FUNC log_get_context
#endif

#ifndef LOG_FLUSH_FUNC
inline static void log_flush(log_entry_context *ctx) {
    (void)fprintf(stderr, "%s\n", ctx->buf);
}
#define LOG_FLUSH_FUNC log_flush
#endif

#ifndef LOG_PRI_PREFIX
#define LOG_PRI_PREFIX(lvl)                                         \
    PriFmt("%s "LOG_COLOR_LOCATION"%s:%zu %s():"LOG_COLOR_RESET" ", \
           log_level_to_str(lvl), __FILE__, __LINE__, __func__)
#endif

#define __log_generic(lvl, ...) do {          \
    if (lvl < log_unit_log_level) break;      \
    log_entry_context *log_entry_context =    \
        LOG_GET_CONTEXT_FUNC();               \
    LOG_PRI_PREFIX(lvl);                      \
    FOR_EACH(__log_entry, ##__VA_ARGS__)      \
    LOG_FLUSH_FUNC(log_entry_context);        \
} while (0)

#define __log_entry(entry) do {                         \
    _Generic((entry),                                   \
        _Bool:              PriBool(entry),             \
        char:               PriChar(entry),             \
        signed char:        PriSignedChar(entry),       \
        unsigned char:      PriUnsignedChar(entry),     \
        short:              PriShort(entry),            \
        unsigned short:     PriUnsignedShort(entry),    \
        int:                PriInt(entry),              \
        unsigned int:       PriUnsignedInt(entry),      \
        long:               PriLong(entry),             \
        unsigned long:      PriUnsignedLong(entry),     \
        long long:          PriLongLong(entry),         \
        unsigned long long: PriUnsignedLongLong(entry), \
        float:              PriFloat(entry),            \
        double:             PriDouble(entry),           \
        long double:        PriLongDouble(entry),       \
        const char *:       PriStr(entry),              \
        char *:             PriStr(entry),              \
        log_entry_result: entry                         \
    );                                                  \
} while(0);

#define LOG_TRUNCATED_LOG_SUFFIX "...trunc"
#define LOG_TRUNCATED_LOG_SUFFIX_SIZE sizeof(LOG_TRUNCATED_LOG_SUFFIX)
static inline log_entry_result log_entry(
    log_entry_context *ctx, const char *format, ...
) {
    if (ctx->size > 0 && ctx->buf[ctx->size - 1] == '\0') {
        ctx->size--;
    }
    size_t remain_size = ctx->capacity - ctx->size;
    if (!remain_size) {
        return (log_entry_result){};
    }
    char *buf_prt = ctx->buf + ctx->size;
    va_list args;
    va_start(args, format);
    ssize_t len = vsnprintf(ctx->buf + ctx->size, remain_size, format, args);
    va_end(args);
    if (len < 0) {
        ctx->buf[ctx->size] = '\0';
        return (log_entry_result){};
    }
    len++; // include '\0'
    if (len > remain_size) {
        len = remain_size;
        char *buf_end = ctx->buf + ctx->capacity;
        *buf_end = '\0';
        if (ctx->capacity >= LOG_TRUNCATED_LOG_SUFFIX_SIZE) {
            const char *suffix = LOG_TRUNCATED_LOG_SUFFIX;
            const size_t suffix_size = LOG_TRUNCATED_LOG_SUFFIX_SIZE;
            memcpy(buf_end - suffix_size, suffix, suffix_size);
        }
    }
    ctx->size += len;
    return (log_entry_result){};
}

static inline log_custom_log_entry(log_entry_bool, ...) {
    va_list args;
    va_start(args, log_entry_context);
    int value = va_arg(args, int);
    va_end(args);
    return PriStr(value? "true": "false");
}

#endif // INCLUDE_LOG_H
