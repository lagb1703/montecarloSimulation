#include "stdint.h"
#include "malloc.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#define fl double
#define ARGSNUM 2
#define UI32 uint32_t

typedef struct
{
    fl x, y;
} Point;

Point *makePoint()
{
    Point *p = (Point *)malloc(sizeof(Point));
    p->x = ((double)rand()) / RAND_MAX;
    p->y = ((double)rand()) / RAND_MAX;
    // printf("%.6f %.6f\n", p->x, p->y);
    return p;
}

fl montecarloPi(UI32 n)
{
    UI32 circle = 0;
    UI32 total = n;
    while (n--)
    {
        Point *p = makePoint();
        fl r = (p->x*p->x + p->y*p->y);
        // printf("%.6f\n", r);
        free(p);
        if (r <= 1.0)
        {
            // printf("circle\n");
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
    srand(time(NULL));
    UI32 n = (UI32)atoi(argv[1]);
    printf("%.6f", montecarloPi(n));
    return 0;
}