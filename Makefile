
BIN = ruler sad_psadbw sad_naive ssd_naive ham_psadbw ham_naive ham_popcnt_gcc
CFLAGS = -Wall -Wno-parentheses -O3 -fforce-addr -fomit-frame-pointer -finline-functions -funroll-loops

all: $(BIN)

ruler: ruler.cpp tictoc.cpp tictoc.h
	g++ ruler.cpp tictoc.cpp -o ruler

sad_psadbw: sad_psadbw.cpp tictoc.cpp tictoc.h
	g++ $(CFLAGS) sad_psadbw.cpp tictoc.cpp -o sad_psadbw

sad_naive: sad_naive.cpp tictoc.cpp tictoc.h
	g++ $(CFLAGS) sad_naive.cpp tictoc.cpp -o sad_naive

ssd_naive: ssd_naive.cpp tictoc.cpp tictoc.h
	g++ $(CFLAGS) ssd_naive.cpp tictoc.cpp -o ssd_naive

ham_psadbw: ham_psadbw.cpp tictoc.cpp tictoc.h
	g++ $(CFLAGS) ham_psadbw.cpp tictoc.cpp -o ham_psadbw

ham_naive: ham_naive.cpp tictoc.cpp tictoc.h
	g++ $(CFLAGS) ham_naive.cpp tictoc.cpp -o ham_naive

ham_popcnt_icc: ham_popcnt_icc.c
	/opt/intel/cce/10.1.015/bin/icc ham_popcnt_icc.c -o ham_popcnt_icc

ham_popcnt_gcc: ham_popcnt_gcc.cpp tictoc.cpp tictoc.h driver.cpp
	g++ -msse4.1 -msse4.2 $(CFLAGS) ham_popcnt_gcc.cpp tictoc.cpp -o ham_popcnt_gcc


clean:
	rm -rf $(BIN)

.PHONY: clean

