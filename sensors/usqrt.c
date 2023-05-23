#include <stdint.h>

uint32_t usqrt32(uint32_t x) 
{
    int8_t pos;
    uint32_t root = 0;
    uint32_t accum = 0;

    for (pos = 15; pos >= 0; pos--) {
        uint32_t tempAccum = accum + (UINT32_C(1) << (pos << 1)) + (root << (pos + 1));
        if (x >= tempAccum) {
            accum = tempAccum;
            root += (UINT32_C(1) << pos);
        }
    }

    return root;
}