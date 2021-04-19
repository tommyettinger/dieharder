/*
 *  rng_harpo.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int harpo_get (void *vstate);
static double harpo_get_double (void *vstate);
static void harpo_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
 unsigned long long c;
} harpo_state_t;


static unsigned long int harpo_get (void *vstate)
{
 harpo_state_t *state = vstate;
 const unsigned long long a0 = state->a;
 const unsigned long long b0 = state->b;
 const unsigned long long c0 = state->c;
 state->a = b0 + ~c0;
 state->b = (a0 << 46 | a0 >> 18) ^ c0;
 state->c = (b0 << 23 | b0 >> 41) - a0;
 return (unsigned int)(a0 + b0);
}

static double harpo_get_double (void *vstate)
{
  return harpo_get (vstate) / (double) UINT_MAX;
}

static void
harpo_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 harpo_state_t *state = (harpo_state_t *) vstate;
 state->a = s;
 state->b = 0xFA346CBFD5890825ULL ^ s;
 state->c = (~0xFA346CBFD5890825ULL) ^ s;

 return;
}

static const gsl_rng_type harpo_type =
{"harpo",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (harpo_state_t),
 &harpo_set,
 &harpo_get,
 &harpo_get_double};

const gsl_rng_type *gsl_rng_harpo = &harpo_type;
