/*
 *  rng_tangle.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int tangle_get (void *vstate);
static double tangle_get_double (void *vstate);
static void tangle_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
} tangle_state_t;


static unsigned long int tangle_get (void *vstate)
{
 tangle_state_t *state = vstate;
 const unsigned long long s = (state->a += 0xC6BC279692B5C323ULL);
 const unsigned long long z = (s ^ s >> 31) * (state->b += 0x9E3779B97F4A7C16ULL);
 return (unsigned int)(z ^ z >> 26 ^ z >> 6);
}

static double tangle_get_double (void *vstate)
{
  return tangle_get (vstate) / (double) UINT_MAX;
}

static void
tangle_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 tangle_state_t *state = (tangle_state_t *) vstate;
 state->a = s;
 state->b = s * 0x9E3779B97F4A7C16ULL + 123456789ULL;
 return;
}

static const gsl_rng_type tangle_type =
{"tangle",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (tangle_state_t),
 &tangle_set,
 &tangle_get,
 &tangle_get_double};

const gsl_rng_type *gsl_rng_tangle = &tangle_type;
