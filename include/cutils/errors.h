#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

#include "log.h"
#include "cutils/macro/arg_count.h"

#define do_if(condition, code, ...) do {                         \
    if (condition) {                                             \
        ARG_COUNT_IS_0(, log_error(__VA_ARGS__), ##__VA_ARGS__); \
        code;                                                    \
    }                                                            \
} while (0)

#define ret_val_do_if(condition, code, val, ...) \
    do_if(condition, { code; return val; }, ##__VA_ARGS__)
#define ret_val_do(code, val, ...) \
    ret_val_do_if(1, code, val, ##__VA_ARGS__)
#define ret_err_do_if(condition, code, ...) \
    ret_val_do_if(condition, code, -1, ##__VA_ARGS__)
#define ret_err_do(code, ...) \
    ret_val_do(code, -1, ##__VA_ARGS__)
#define ret_do_if(condition, code, ...) \
    ret_val_do_if(condition, code,, ##__VA_ARGS__)
#define ret_do(code, ...) \
     ret_val_do(code,, ##__VA_ARGS__)
#define ret_val_if(condition, val, ...) \
    ret_val_do_if(condition,, val, ##__VA_ARGS__)
#define ret_val(val, ...) \
    ret_val_do(, val, ##__VA_ARGS__)
#define ret_err_if(condition, ...) \
    ret_err_do_if(condition,, ##__VA_ARGS__)
#define ret_err(...) \
    ret_err_do(, ##__VA_ARGS__)
#define ret_if(condition, ...) \
    ret_do_if(condition,, ##__VA_ARGS__)
#define ret(...) \
    ret_do(, ##__VA_ARGS__)

#define break_do_if(condition, code, ...) \
    do_if(condition, { code; break; }, ##__VA_ARGS__)
#define break_do(code, ...) \
    break_do_if(1, code, ##__VA_ARGS__)
#define break_if(condition, ...) \
    break_do_if(condition,, ##__VA_ARGS__)
#define break(...) \
    break_if(1, ##__VA_ARGS__)

#define abort_do_if(condition, code, ...) \
    do_if(condition, { code; abort(); }, ##__VA_ARGS__)
#define abort_do(code, ...) \
    abort_do_if(1, code, ##__VA_ARGS__)
#define abort_if(condition, ...) \
    abort_do_if(1,, ##__VA_ARGS__)
#define abort(...) \
    abort_if(1, ##__VA_ARGS__)

#endif // INCLUDE_ERROR_H
