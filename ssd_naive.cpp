#include "driver.cpp"


inline unsigned int measure(uint8_t *buf1, uint8_t *buf2, int bufLen) {
    uint32_t dist = 0;

    for (int i = 0 ; i < 64 * bufLen ; i++) {
        int diff = buf1[i] - buf2[i];
        dist += diff * diff;
    }

    return dist;
}

