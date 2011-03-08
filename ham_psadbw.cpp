#include "driver.cpp"


#define GOFAST


static uint32_t popcount64(uint64_t v) {
    static const uint64_t C55 = 0x5555555555555555;
    static const uint64_t C33 = 0x3333333333333333;
    static const uint64_t C0F = 0x0F0F0F0F0F0F0F0F;

    uint64_t w, x, y;
    uint32_t count = 0;

    w = v - ((v >> 1) & C55);
    x = (w & C33) + ((w >> 2) & C33);
    y = (x + (x >> 4) & C0F);

    /* this loop can be replaced with a few instructions */
#ifndef GOFAST

    {
        uint8_t *c;
        int i;

        c = (uint8_t *)&y;
        for (i = 0 ; i < 8 ; i++) { count += c[i]; }

        printf("v %ld w %ld x %ld y %ld count %d\n", v, w, x, y, count);
    }

#else

    __asm__ __volatile__ (
        "  .balign 16                   \n\t"
        "  movq %1, %%mm1               \n\t"       /* move y into mm1 */
        "  pxor %%mm0, %%mm0            \n\t"       /* zero out mm0 */
        "  psadbw %%mm1, %%mm0          \n\t"
        "  movd %%mm0, %0               \n\t"

         : "=r" (count)
         : "r" ((uint64_t)y)
         : "memory"
    );

#endif

    return count;
}



inline unsigned int measure(uint8_t *buf1, uint8_t *buf2, int bufLen) {         
    uint32_t dist = 0;
    uint64_t *u, *v, x;

    u = (uint64_t *)buf1;
    v = (uint64_t *)buf2;

    for (int i = 0 ; i < 8 * bufLen ; i++) {
        x = *u ^ *v;
        dist += popcount64(x);

        u++; v++;
    }

    return dist;
}

