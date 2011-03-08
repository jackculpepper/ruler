
#include <sys/time.h>

extern struct timeval s_time;

void tic(struct timeval &s_time);
void tic();
double toc(struct timeval &s_time);
double toc();

