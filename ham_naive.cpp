#include "driver.cpp"

inline unsigned int measure(uint8_t *buf1, uint8_t *buf2, int bufLen) {
    unsigned long long dist = 0;
    unsigned long long *u, *v, w;

    u = (unsigned long long *)buf1;
    v = (unsigned long long *)buf2;

    for (int i = 0 ; i < 8 * bufLen ; i++) {
        w = *u ^ *v;

        for (int j = 0 ; j < 64 ; j++) {
            dist += w & 1;
            w >>= 1;
        }

        u++; v++;
    }

    return dist;
}

