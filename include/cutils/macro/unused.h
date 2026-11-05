#ifndef CUTILS_MACRO_UNUSED_H
#define CUTILS_MACRO_UNUSED_H

#include "cutils/macro/for_each.h"

#define _UNUSED(arg) (void)arg;
#define UNUSED(...) FOR_EACH(_UNUSED, ##__VA_ARGS__)

#endif // CUTILS_MACRO_UNUSED_H
