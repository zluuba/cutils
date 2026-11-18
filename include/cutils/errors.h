#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

#include "log.h"
#include "cutils/macro/arg_count.h"

#include <stdlib.h>

/**
 * Error-handling helpers with a consistent macro naming scheme.
 *
 * Naming convention:
 * - `ret_*` - macros return from the current function.
 * - `break_*` - macros break from the current loop.
 * - `continue_*` - macros skip the rest of the current loop iteration.
 * - `abort_*` - macros terminate the program.
 *
 * The names follow a compact pattern:
 * - `<action>_do_if(cond, code, ...)` - evaluate condition, execute `code`,
 *       then run the action (`return`, `break`, `continue`, `abort`).
 * - `<action>_do(code, ...)` - execute `code`, and then run the action.
 * - `<action>_if(cond, ...)` - execute only the action for the true condition.
 * - `<action>_(...)` - shorthand for the unconditional form.
 *
 * For return macros, the suffix defines the return value:
 * - `ret_val_*` - return a specific value.
 * - `ret_err_*` - return `-1`.
 * - `ret_*` - return without a value.
 */

/**
 * Execute `code` when `condition` is true and optionally log an error.
 * @param condition Condition to check.
 * @param code Statement(s) to run when the condition is true.
 * @param ... Optional log format string and arguments passed to `log_error`.
 */
#define do_if(condition, code, ...) {                            \
    if (condition) {                                             \
        ARG_COUNT_IS_0(, log_error(__VA_ARGS__), ##__VA_ARGS__); \
        code;                                                    \
    }                                                            \
}

/**
 * Convenience wrapper around `do_if(cond, , ...)` for logging-only checks.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments passed to `log_error`.
 */
#define log_if(cond, ...) do_if(cond,,##__VA_ARGS__)

/**
 * Return `val` after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before returning.
 * @param val Value to return.
 * @param ... Optional log format string and arguments.
 */
#define ret_val_do_if(cond, code, val, ...) \
    do_if(cond, { code; return val; }, ##__VA_ARGS__)

/**
 * Return `val` after executing `code` unconditionally.
 * @param code Statement(s) to run before returning.
 * @param val Value to return.
 * @param ... Optional log format string and arguments.
 */
#define ret_val_do(code, val, ...) ret_val_do_if(1, code, val, ##__VA_ARGS__)

/**
 * Return `-1` after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before returning.
 * @param ... Optional log format string and arguments.
 */
#define ret_err_do_if(cond, code, ...) \
    ret_val_do_if(cond, code, -1, ##__VA_ARGS__)

/**
 * Return `-1` after executing `code` unconditionally.
 * @param code Statement(s) to run before returning.
 * @param ... Optional log format string and arguments.
 */
#define ret_err_do(code, ...) ret_val_do(code, -1, ##__VA_ARGS__)

/**
 * Return without a value after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before returning.
 * @param ... Optional log format string and arguments.
 */
#define ret_do_if(cond, code, ...) ret_val_do_if(cond, code,, ##__VA_ARGS__)

/**
 * Return without a value after executing `code` unconditionally.
 * @param code Statement(s) to run before returning.
 * @param ... Optional log format string and arguments.
 */
#define ret_do(code, ...) ret_val_do(code,, ##__VA_ARGS__)

/**
 * Return `val` when `cond` is true.
 * @param cond Condition to check.
 * @param val Value to return.
 * @param ... Optional log format string and arguments.
 */
#define ret_val_if(cond, val, ...) ret_val_do_if(cond,, val, ##__VA_ARGS__)

/**
 * Return `val` unconditionally.
 * @param val Value to return.
 * @param ... Optional log format string and arguments.
 */
#define ret_val(val, ...) ret_val_do(, val, ##__VA_ARGS__)

/**
 * Return `-1` when `cond` is true.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments.
 */
#define ret_err_if(cond, ...) ret_err_do_if(cond,, ##__VA_ARGS__)

/**
 * Return `-1` unconditionally.
 * @param ... Optional log format string and arguments.
 */
#define ret_err(...) ret_err_do(, ##__VA_ARGS__)

/**
 * Return without a value when `cond` is true.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments.
 */
#define ret_if(cond, ...) ret_do_if(cond,, ##__VA_ARGS__)

/**
 * Return without a value unconditionally.
 * @param ... Optional log format string and arguments.
 */
#define ret_(...) ret_do(, ##__VA_ARGS__)

/**
 * Break from the current loop after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before breaking.
 * @param ... Optional log format string and arguments.
 */
#define break_do_if(cond, code, ...) \
    do_if(cond, { code; break; }, ##__VA_ARGS__)

/**
 * Break from the current loop after executing `code` unconditionally.
 * @param code Statement(s) to run before breaking.
 * @param ... Optional log format string and arguments.
 */
#define break_do(code, ...) break_do_if(1, code, ##__VA_ARGS__)

/**
 * Break from the current loop when `cond` is true.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments.
 */
#define break_if(cond, ...) break_do_if(cond,, ##__VA_ARGS__)

/**
 * Break from the current loop unconditionally.
 * @param ... Optional log format string and arguments.
 */
#define break_(...) break_if(1, ##__VA_ARGS__)

/**
 * Continue the current loop after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before continuing.
 * @param ... Optional log format string and arguments.
 */
#define continue_do_if(cond, code, ...) \
    do_if(cond, { code; continue; }, ##__VA_ARGS__)

/**
 * Continue the current loop after executing `code` unconditionally.
 * @param code Statement(s) to run before continuing.
 * @param ... Optional log format string and arguments.
 */
#define continue_do(code, ...) continue_do_if(1, code, ##__VA_ARGS__)

/**
 * Continue the current loop when `cond` is true.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments.
 */
#define continue_if(cond, ...) continue_do_if(cond,, ##__VA_ARGS__)

/**
 * Continue the current loop unconditionally.
 * @param ... Optional log format string and arguments.
 */
#define continue_(...) continue_if(1, ##__VA_ARGS__)

#define abort_progarm_func() abort()

/**
 * Abort the process after executing `code` when `cond` is true.
 * @param cond Condition to check.
 * @param code Statement(s) to run before aborting.
 * @param ... Optional log format string and arguments.
 */
#define abort_do_if(cond, code, ...) \
    do_if(cond, { code; abort_progarm_func(); }, ##__VA_ARGS__)

/**
 * Abort the process after executing `code` unconditionally.
 * @param code Statement(s) to run before aborting.
 * @param ... Optional log format string and arguments.
 */
#define abort_do(code, ...) abort_do_if(1, code, ##__VA_ARGS__)

/**
 * Abort the process when `cond` is true.
 * @param cond Condition to check.
 * @param ... Optional log format string and arguments.
 */
#define abort_if(cond, ...) abort_do_if(cond,, ##__VA_ARGS__)

/**
 * Abort the process unconditionally.
 * @param ... Optional log format string and arguments.
 */
#define abort_(...) abort_if(1, ##__VA_ARGS__)

#endif // INCLUDE_ERROR_H
