#ifndef USQRT_H
#define USQRT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function calculates the integer square root of a number.
 *
 * This function uses the binary search method to calculate the integer square root of a number.
 * It is optimized for systems without a floating point unit, such as the ARM Cortex-M3.
 *
 * @param x The number for which the integer square root will be calculated.
 * 
 * @return The integer square root of the input number.
 *
 * @note The algorithm used in this function is referenced from:
 * http://www.embedded.com/electronics-blogs/programmer-s-toolbox/4219659/Integer-Square-Roots
 *
 * @code{.c}
 * uint32_t result = Algo_uSqrt(9);  // result will be 3
 * @endcode
 */
uint32_t usqrt32(uint32_t x);

#ifdef __cplusplus
}
#endif

#endif /* USQRT_H */