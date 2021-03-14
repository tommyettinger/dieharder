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
 unsigned long long s = (state->a += 0xD1342543DE82EF95ULL);
 s = (s ^ s >> 31 ^ s >> 21) * ((s ? (state->b += 0xC6BC279692B5C323ULL) : state->b) | 1ULL);
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
 state->b = s;
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
