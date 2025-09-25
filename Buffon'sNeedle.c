#include "stdint.h"
#include "malloc.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#include <math.h>
#include "thread.c"
#include "rng.c"
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define fl double
#define ARGSNUM 3
#define UI32 uint32_t
#define MAXANGLE 3.141592653589793
#define L 1
#define l 2

typedef struct
{
    UI32 n;
    UI32 result;
    rng128_t rng;
} Data;

void needlePi(void *d)
{
    Data *data = (Data *)d;
    UI32 n = data->n;
    UI32 intersects = 0;
    while (n--)
    {
        fl angle = 0.5 * sin(MAXANGLE * rng_next_double01(&data->rng));
        fl x = l * rng_next_double01(&data->rng);
        if (x - angle <= 0 || x + angle >= l)
        {
            intersects++;
        }
    }
    data->result = intersects;
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
    createThread(needlePi, &data[0], &t[0]);
    for (UI32 i = 1; i < nThreads; i++)
    {
        data[i].n = space;
        rng_seed(&data[i].rng, base_seed + 0x9E3779B97F4A7C15ULL * (uint64_t)(i + 1));
        createThread(needlePi, &data[i], &t[i]);
    }
    for (UI32 i = 0; i < nThreads; i++)
    {
        joinThread(t[i]);
        result += data[i].result;
    }
    free(data);
    free(t);
    if (result == 0)
        return 0;
    return ((fl)n) / result;
}

int main(int argc, char **argv)
{
    if (argc != ARGSNUM)
    {
        return 1;
    }
    UI32 n = (UI32)atoi(argv[1]);
    UI32 nThreads = (UI32)atoi(argv[2]);
    // UI32 n = 10000;
    // UI32 nThreads = 1;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pi(n, nThreads);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f", elapsed);
    return 0;
}