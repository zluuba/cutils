#ifndef INCLUDE_LOCATION_H
#define INCLUDE_LOCATION_H

#include <stddef.h>

typedef struct Location {
    const char *file;
    size_t line;
    const char *func;
} Location;

#define location_here() (Location) { \
    .file = __FILE_NAME__,           \
    .line = __LINE__,                \
    .func = __func__                 \
}

#endif // INCLUDE_LOCATION_H
