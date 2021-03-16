/*
 *  rng_lxm.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int lxm_get (void *vstate);
static double lxm_get_double (void *vstate);
static void lxm_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long s;
 unsigned long long x0;
 unsigned long long x1;
} lxm_state_t;

static unsigned long long lxm_mix(unsigned long long z) {
 z = (z ^ (z >> 32)) * 0xdaba0b6eb09322e3ULL;
 z = (z ^ (z >> 32)) * 0xdaba0b6eb09322e3ULL;
 return z ^ (z >> 32);
}

static unsigned long int lxm_get (void *vstate)
{
 lxm_state_t *state = vstate;
 unsigned int result = lxm_mix(state->s + state->x0);
 state->s = state->s * 0xd1342543de82ef95ULL + state->a;
 unsigned long long q0 = state->x0, q1 = state->x1;
 q1 ^= q0;
 q0 = (q0 << 24) | (q0 >> 40);
 q0 = q0 ^ q1 ^ (q1 << 16);
 q1 = (q1 << 37) | (q1 >> 27);
 state->x0 = q0;
 state->x1 = q1;
 return result;
}

static double lxm_get_double (void *vstate)
{
  return lxm_get (vstate) / (double) UINT_MAX;
}

static void
lxm_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 lxm_state_t *state = (lxm_state_t *) vstate;
 state->a = 1ULL;
 state->s = s;
 state->x0 = 0x9E3779B97F4A7C15ULL + s;
 state->x1 = 0x9E3779B97F4A7C15ULL - s;
 return;
}

static const gsl_rng_type lxm_type =
{"lxm",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (lxm_state_t),
 &lxm_set,
 &lxm_get,
 &lxm_get_double};

const gsl_rng_type *gsl_rng_lxm = &lxm_type;
