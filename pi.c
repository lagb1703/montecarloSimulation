#include "stdint.h"
#include "malloc.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#define fl double
#define ARGSNUM 2
#define UI32 uint32_t

fl montecarloPi(UI32 n)
{
    UI32 circle = 0;
    UI32 total = n;
    fl x, y;
    while (n--)
    {
        y = ((double)rand()) / RAND_MAX;
        x = ((double)rand()) / RAND_MAX;
        fl r = (x*x + y*y);
        if (r <= 1)
        {
            circle++;
        }
    }
    return 4.0 * ((fl) circle) / total;
}

int main(int argc, char **argv)
{
    if (argc != ARGSNUM)
    {
        return 1;
    }
    struct timespec start, end;
    srand(time(NULL));
    UI32 n = (UI32)atoi(argv[1]);
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("%.6f\n", montecarloPi(n));
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f", elapsed);
    return 0;
}