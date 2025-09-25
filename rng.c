#include <stdint.h>
//Nota: este codigo lo genero una IA, el trabajo es sobre paralelismo no RNG
typedef struct
{
    uint64_t s[2];
} rng128_t;

uint64_t rotl64(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

uint64_t splitmix64_next(uint64_t *x)
{
    uint64_t z = (*x += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

void rng_seed(rng128_t *r, uint64_t seed)
{
    uint64_t x = seed;
    r->s[0] = splitmix64_next(&x);
    r->s[1] = splitmix64_next(&x);
    if (r->s[0] == 0 && r->s[1] == 0)
        r->s[1] = 1; // evitar estado nulo
}

uint64_t xorshift128plus_next64(rng128_t *r)
{
    uint64_t s1 = r->s[0];
    const uint64_t s0 = r->s[1];
    r->s[0] = s0;
    s1 ^= s1 << 23;
    r->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return r->s[1] + s0;
}

double rng_next_double01(rng128_t *r)
{
    return (xorshift128plus_next64(r) >> 11) * (1.0 / 9007199254740992.0);
}