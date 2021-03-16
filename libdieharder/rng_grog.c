/*
 *  rng_grog.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int grog_get (void *vstate);
static double grog_get_double (void *vstate);
static void grog_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
} grog_state_t;


static unsigned long int grog_get (void *vstate)
{
 grog_state_t *state = vstate;
 //0xD1342543DE82EF95ULL 0xC6BC279692B5C323ULL 0x9E3779B97F4A7C15ULL 0xD1342543DE82EF95ULL 0xFA346CBFD5890825ULL
 unsigned long long s = (state->a += 0xD1342543DE82EF95ULL);
 s = (s ^ s >> 31 ^ s >> 13) * ((s ? (state->b += 0xFA346CBFD5890825ULL) : state->b) | 1ULL);
 // testing shifts above, 31, n, then 28 below
 // n = 21 gets 4 "weak", 5 gets 2 weak, 13 gets 1 weak, 17 gets 1 weak, 19 gets 1 weak, 25 gets 2 weak
 // 31,5,28 gets no weak sometimes, but 1-4 other times.
 // s = (s ^ s >> 31 ^ s >> 21) * ((s ? (state->b += 0xC6BC279692B5C323ULL) : state->b) | 1ULL);
 //unsigned long long s = (state->a += 0xD1342543DE82EF95ULL);
 //s = (s ^ s >> 31 ^ s >> 20) * ((state->b += 0xFA346CBFD5890825ULL)|1ULL);
// unsigned long long s = (state->a += 0xC6BC279692B5C323ULL);
// s = (s ^ s >> 31) * (state->b += 0x9E3779B97F4A7C16ULL);
 return (unsigned int)(s ^ s >> 28);
}

static double grog_get_double (void *vstate)
{
  return grog_get (vstate) / (double) UINT_MAX;
}

static void
grog_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 grog_state_t *state = (grog_state_t *) vstate;
 state->a = s;
 //state->b = 1ULL | s;
 state->b = 0x9E3779B97F4A7C15ULL ^ s;
 return;
}

static const gsl_rng_type grog_type =
{"grog",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (grog_state_t),
 &grog_set,
 &grog_get,
 &grog_get_double};

const gsl_rng_type *gsl_rng_grog = &grog_type;
