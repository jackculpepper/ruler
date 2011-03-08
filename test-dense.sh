#!/bin/bash -x

perl gentest.pl 64000 >test_1000x64_rand.in

./ham_naive test_1000x64_rand.in 1000 1000

./ham_psadbw test_1000x64_rand.in 1000 1000

./ham_popcnt_gcc test_1000x64_rand.in 1000 1000

./ssd_naive test_1000x64_rand.in 1000 1000

./sad_psadbw test_1000x64_rand.in 1000 1000

./sad_naive test_1000x64_rand.in 1000 1000

