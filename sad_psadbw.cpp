#include "driver.cpp"


static uint32_t sad8x8__mmxext(unsigned char *ptr1, uint32_t stride1,
                               unsigned char *ptr2, uint32_t stride2) {
    uint32_t  dist;

    __asm__ __volatile__ (
        "  .balign 16                   \n\t"
        "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

        ".rept 7                        \n\t"
        "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
        "  movq (%2), %%mm1             \n\t"
        "  psadbw %%mm1, %%mm0          \n\t"
        "  add %3, %1                   \n\t"	/* Inc pointer into the new data */
        "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
        "  add %4, %2                   \n\t"	/* Inc pointer into ref data */
        ".endr                          \n\t"

        "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
        "  movq (%2), %%mm1             \n\t"
        "  psadbw %%mm1, %%mm0          \n\t"
        "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
        "  movd %%mm7, %0               \n\t"

         : "=r" (dist),
           "+r" (ptr1), 
           "+r" (ptr2) 
         : "r" ((uint64_t)stride1),
           "r" ((uint64_t)stride2)
         : "memory"
    );

    return dist;
}


inline unsigned int measure(uint8_t *buf1, uint8_t *buf2, int bufLen) {
    uint32_t dist = 0;

    for (int i = 0 ; i < bufLen ; i++) {
        dist += sad8x8__mmxext(buf1,8,buf2,8);
        buf1 += 64; buf2 += 64;
    }

    return dist;
}

