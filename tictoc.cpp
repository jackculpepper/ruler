#include "tictoc.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

struct timeval s_time;


void tic(struct timeval &s_time) {
    if (gettimeofday(&s_time, NULL)) {
        cout << "unable to get millis" << endl;
        exit(1);
    }
}
void tic() { return tic(s_time); }
 
 
double toc(struct timeval &s_time) {
    struct timeval e_time;
    if (gettimeofday(&e_time, NULL)) {
        cout << "unable to get millis" << endl;
        exit(1);
    }
    return (e_time.tv_sec - s_time.tv_sec) +
        0.000001 * (e_time.tv_usec - s_time.tv_usec);
}
double toc() { return toc(s_time); }
 
 
