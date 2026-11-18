#ifndef INCLUDE_LOCATION_H
#define INCLUDE_LOCATION_H

#include <stddef.h>

/**
 * Source location information for a code point.
 */
typedef struct Location {
    const char * const file;
    const size_t line;
    const char * const func;
} Location;

/**
 * Create a Location describing the current source position.
 * @return A Location initialized with the current source position.
 */
#define location_here() (Location) { \
    .file = __FILE_NAME__,           \
    .line = __LINE__,                \
    .func = __func__                 \
}

/**
 * @brief Format a Location for printing.
 * @param[in] loc Pointer to the Location to format.
 */
#define PriLocation(loc) \
    PriFmt("%s:%zu %s()", (loc)->file, (loc)->line, (loc)->func)

#endif // INCLUDE_LOCATION_H
