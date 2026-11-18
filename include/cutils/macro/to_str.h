#ifndef CUTILS_MACRO_TO_STR_H
#define CUTILS_MACRO_TO_STR_H

#define _TO_STR_2(x) #x

/**
 * @brief Converts a macro argument to a string literal after expansion.
 * @param x The macro argument to stringify.
 */
#define TO_STR(x) _TO_STR_2(x)

#endif // CUTILS_MACRO_TO_STR_H
