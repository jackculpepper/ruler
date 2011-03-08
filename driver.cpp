// local
#include "tictoc.h"

// c++
#include <iostream>
#include <fstream>

// c
#include <stdlib.h>
#include <stdio.h>

using namespace std;

inline unsigned int measure(uint8_t *buf1, uint8_t *buf2, int bufLen);

int main(int argc, char **argv) {
    unsigned long long dist = 0;
    double time_total = 0;
    int i, j;

    int numLoops = atoi(argv[2]);
    int bufLen = atoi(argv[3]);

    cout << "Running " << numLoops << " loops" << endl;

    for (j = 0 ; j < numLoops ; j++) {
        ifstream inFile(argv[1]);

        if (!inFile.is_open()) {
            cerr << "Could not open input file." << endl;
            exit(1);
        }

        unsigned char buf1[64 * bufLen], buf2[64 * bufLen];

        for (i = 0 ; i < 64 * bufLen ; i++) {
            inFile >> buf1[i];
            inFile >> buf2[i];
        }  

        inFile.close();

        tic();
        dist += measure(buf1, buf2, bufLen);
        time_total += toc();
    }

    cout << dist << endl;
    cout << time_total << " s" << endl;
}


