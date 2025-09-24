#include "stdint.h"
#include "malloc.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#include <math.h>
#define fl double
#define ARGSNUM 2
#define UI32 uint32_t
#define MAXANGLE 3.141592653589793
#define L 1
#define l 2

fl needlePi(UI32 n)
{
    UI32 total = n;
    UI32 intersects = 0;
    while (n--)
    {
        fl angle = 0.5 * sin(MAXANGLE * ((fl)rand()) / RAND_MAX);
        fl x = l * ((fl)rand()) / RAND_MAX;
        if (x - angle <= 0 || x + angle >= l)
        {
            intersects++;
        }
    }
    if (intersects == 0)
        return 0;
    return ((fl)total) / intersects;
}

int main(int argc, char **argv)
{
    if (argc != ARGSNUM)
    {
        return 1;
    }
    struct timespec start, end;
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    UI32 n = (UI32)atoi(argv[1]);
    needlePi(n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    return 0;
}