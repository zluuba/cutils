#ifndef CUTILS_MACRO_UNUSED_H
#define CUTILS_MACRO_UNUSED_H

#include "cutils/macro/for_each.h"

#define _UNUSED(arg) (void)arg;

/**
 * Marks one or more variables as intentionally unused.
 *
 * Expands to a void cast for each argument, suppressing unused-variable
 * warnings without evaluating the variables.
 *
 * @param ... One or more variables to mark as unused.
 */
#define UNUSED(...) FOR_EACH(_UNUSED, ##__VA_ARGS__)

#endif // CUTILS_MACRO_UNUSED_H
