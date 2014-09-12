#ifndef TIME_H
#define TIME_H

typedef long long int clock_t;
typedef long long int time_t;

time_t time(time_t* tm);
clock_t clock();

#define CLOCKS_PER_SEC 1 // TODO: determine CLOCKS_PER_SEC

#endif