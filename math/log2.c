#include <stdint.h>
#include <stdio.h>
#include <stddef.h>


uint32_t ones32(uint32_t x) {
    /* 32-bit recursive reduction using SWAR...
     * but first step is mapping 2-bit values
     * into sum of 2 1-bit values in sneaky way
     */
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return (x & 0x0000003f);
}

uint32_t log2_(uint32_t x) {
    int y = (x & (x - 1));
    y |= -y;
    y >>= 31;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return(ones32(x) - 1 - y);
}

int main() {
    int x = 33;
    printf("ones32(32) ->%u\n", ones32(x));
    printf("log2_(32)   ->%u\n", log2_(x));
    return 0;
}



