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
#define SHM_NAME "/array"
#define fl double
#define ARGSNUM 3
#define UI32 uint64_t
#define MAXANGLE 3.141592653589793
#define L 1
#define l 2

typedef struct
{
    UI32 i;
    rng128_t rng;
} Data;

size_t round_up(size_t x, size_t a) { return (x + a - 1) & ~(a - 1); }

UI32 *createSharedMemory(UI32 n, UI32 nProcess)
{
    int shm_fd;
    UI32 *ptr;
    size_t bytesA = (nProcess + 1) * sizeof(UI32);
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, bytesA);
    ptr = mmap(NULL, bytesA, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    return ptr;
}

void deleteSharedMemory(UI32 *d, UI32 nProcess)
{
    size_t bytesA = (nProcess + 1) * sizeof(UI32);
    munmap(d, bytesA);
    shm_unlink(SHM_NAME);
}

void needlePi(Data *data, UI32 *arrayResults)
{
    UI32 n = arrayResults[0];
    UI32 intersects = 0;
    for(UI32 i = 0; i < n; i++)
    {
        fl angle = 0.5 * sin(MAXANGLE * rng_next_double01(&data->rng));
        fl x = l * rng_next_double01(&data->rng);
        if (x - angle <= 0 || x + angle >= l)
        {
            intersects++;
        }
    }
    arrayResults[data->i] = intersects;
}

fl pi(UI32 n, UI32 nProcess)
{
    UI32 *array = createSharedMemory(n, nProcess);
    pid_t *p = (pid_t *)malloc(sizeof(pid_t) * nProcess);
    int parentId = getpid();
    uint64_t base_seed = (uint64_t)time(NULL) ^ (uintptr_t)&n;
    array[0] = n / nProcess;
    UI32 i = 0;
    for (; i < nProcess && parentId == getpid(); i++)
        p[i] = fork();
    Data *data = (Data *)malloc(sizeof(Data));
    if (parentId == getpid())
    {
        UI32 result = 0;
        UI32 j = 0;
        for (; j < nProcess; j++)
        {
            waitpid(p[j], NULL, 0);
            printf("%u\n", (unsigned)array[j + 1]);
            result += array[j + 1];
        }
        array[0] = n % nProcess;
        data->i = 0;
        needlePi(data, array);
        result += array[0];
        free(p);
        deleteSharedMemory(array, nProcess);
        return ((fl)n) / result;
    }
    data->i = i;
    rng_seed(&data->rng, base_seed + 0x9E3779B97F4A7C15ULL * (uint64_t)(i));
    needlePi(data, array);
    free(data);
    exit(0);
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