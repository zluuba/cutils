#ifndef CUTIL_H
#define CUTIL_H

#include "errors.h"
#include "log.h"
#include "mem.h"
#include "random.h"
#include "unused.h"

#define MATH_MOD(a, b) ((((a) % (b)) + (b)) % (b))
#define MAX(a, b) (((a) > (b))? (a): (b))
#define MIN(a, b) (((a) < (b))? (a): (b))

#endif // CUTIL_H
