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
        // printf("%.6f %.6f\n", angle, x);
        if (x - angle <= 0 || x + angle >= l)
        {
            intersects++;
        }
    }
    // printf("intersects: %i\n", intersects);
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
    srand(time(NULL));
    UI32 n = (UI32)atoi(argv[1]);
    printf("%.6f", needlePi(n));
    return 0;
}