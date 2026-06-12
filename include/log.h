#ifndef LOG_H
#define LOG_H

#define LOG_COLOR_RESET   "\x1b[0m"
#define LOG_COLOR_GRAY    "\x1b[30m"
#define LOG_COLOR_RED     "\x1b[31m"
#define LOG_COLOR_GREEN   "\x1b[32m"
#define LOG_COLOR_YELLOW  "\x1b[33m"
#define LOG_COLOR_BLUE    "\x1b[34m"

typedef enum LogLevel {
    LOG_ALL,
    LOG_DBG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_DISABLE,
} LogLevel;

#ifndef LOG_LVL
#define LOG_LVL LOG_INFO
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define FORMAT_ATTRIBUTE(string_index, first_to_check) \
        __attribute__((__format__(__printf__, string_index, first_to_check)))
#else
    #define FORMAT_ATTRIBUTE(string_index, first_to_check)
#endif

void generic_log(LogLevel min_lvl, LogLevel lvl, const char *fmt, ...) FORMAT_ATTRIBUTE(3, 4);

#if 1
#define LOG_FMT(lvl, fmt) \
    LOG_COLOR_GRAY "%s:%d %s() " lvl LOG_COLOR_RESET ": " fmt "\n", __FILE__, __LINE__, __func__
#else
#define LOG_FMT(lvl, fmt) lvl LOG_COLOR_RESET ": " fmt "\n"
#endif

#define _generic_log(lvl, lvl_name, fmt, ...) \
    generic_log(LOG_LVL, lvl, LOG_FMT(lvl_name, fmt), ##__VA_ARGS__)

#define logd(fmt, ...) _generic_log(LOG_DBG,  LOG_COLOR_GREEN "[Dbg]", fmt, ##__VA_ARGS__)
#define logi(fmt, ...) _generic_log(LOG_INFO, LOG_COLOR_BLUE  "[Inf]", fmt, ##__VA_ARGS__)
#define logw(fmt, ...) _generic_log(LOG_WARN, LOG_COLOR_YELLOW"[Wrn]", fmt, ##__VA_ARGS__)
#define loge(fmt, ...) _generic_log(LOG_ERR,  LOG_COLOR_RED   "[Err]", fmt, ##__VA_ARGS__)

#define logenter() logd("Enter")

#endif // LOG_H
