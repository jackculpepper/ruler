
#include <stdlib.h>
#include <stdio.h>
#include <nmmintrin.h>

int main(int argc, char **argv) {
    unsigned __int64 u, v, w, x;

    u = atoi(argv[1]);
    v = atoi(argv[2]);

    w = u ^ v;

    x = _mm_popcnt_u64(w);

    printf("%ld\n", x);
}

