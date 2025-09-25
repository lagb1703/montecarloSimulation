#include "stdint.h"
#include "malloc.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#include "thread.c"
#include "rng.c"
#define fl double
#define ARGSNUM 3
#define UI32 uint32_t

typedef struct
{
    UI32 n;
    UI32 result;
    rng128_t rng;
} Data;

void montecarloPi(void *d)
{
    srand(time(NULL));
    Data *data = (Data *)d;
    UI32 circle = 0;
    fl x, y;
    for (UI32 i = 0; i < data->n; i++)
    {
        fl x = rng_next_double01(&data->rng);
        fl y = rng_next_double01(&data->rng);
        fl r = x * x + y * y;
        if (r <= 1.0) circle++;
    }
    data->result = circle;
    killMyThread(NULL);
}

fl pi(UI32 n, UI32 nThreads)
{
    UI32 result = 0;
    Data *data = (Data *)malloc(sizeof(Data) * nThreads);
    Thread *t = (Thread *)malloc(sizeof(Thread) * nThreads);
    UI32 space = n / nThreads;
    uint64_t base_seed = (uint64_t)time(NULL) ^ (uintptr_t)data ^ (uintptr_t)&n;
    data[0].n = space + n % nThreads;
    rng_seed(&data[0].rng, base_seed + 0x9E3779B97F4A7C15ULL * (uint64_t)(0 + 1));
    createThread(montecarloPi, &data[0], &t[0]);
    for (UI32 i = 1; i < nThreads; i++)
    {
        data[i].n = space;
        rng_seed(&data[i].rng, base_seed + 0x9E3779B97F4A7C15ULL * (uint64_t)(i + 1));
        createThread(montecarloPi, &data[i], &t[i]);
    }
    for (UI32 i = 0; i < nThreads; i++)
    {
        joinThread(t[i]);
        result += data[i].result;
    }
    free(data);
    free(t);
    return 4.0 * ((fl) result)/n;
}

int main(int argc, char **argv)
{
    if (argc != ARGSNUM)
    {
        return 1;
    }
    UI32 n = (UI32)atoi(argv[1]);
    UI32 nThreads = (UI32)atoi(argv[2]);
    // UI32 n = 10;
    // UI32 nThreads = 1;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("%.6f", pi(n, nThreads));
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f", elapsed);
    return 0;
}