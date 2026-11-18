#ifndef CUTILS_MACRO_MATH_H
#define CUTILS_MACRO_MATH_H

/**
 * Computes the math modulo of a value.
 *
 * Example:
 * ```c
 * assert(MATH_MOD(10, 3), 1);
 * assert(MATH_MOD( 0, 3), 2);
 * assert(MATH_MOD(-1, 3), 2);
 * ```
 * @param a The value to reduce.
 * @param b The modulus (b>0).
 * @return The normalized modulo.
 */
#define MATH_MOD(a, b) ((((a) % (b)) + (b)) % (b))

/**
 * Returns the greater of two values.
 * @param a The first value.
 * @param b The second value.
 * @return The greater value.
 */
#define MAX(a, b) (((a) > (b))? (a): (b))

/**
 * Returns the lesser of two values.
 * @param a The first value.
 * @param b The second value.
 * @return The lesser value.
 */
#define MIN(a, b) (((a) < (b))? (a): (b))

#endif // CUTILS_MACRO_MATH_H
