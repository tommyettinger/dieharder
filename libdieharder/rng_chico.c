/*
 *  rng_chico.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int chico_get (void *vstate);
static double chico_get_double (void *vstate);
static void chico_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
 unsigned long long c;
} chico_state_t;


static unsigned long int chico_get (void *vstate)
{
 chico_state_t *state = vstate;
 const unsigned long long a0 = state->a;
 const unsigned long long b0 = state->b;
 const unsigned long long c0 = state->c;
 state->a = b0 ^ c0 + 0xC6BC279692B5C323UL;
 state->b = (a0 << 46 | a0 >> 18) + c0;
 state->c = (b0 << 23 | b0 >> 41) - a0;
 return (unsigned int)(a0);
}

static double chico_get_double (void *vstate)
{
  return chico_get (vstate) / (double) UINT_MAX;
}

static void
chico_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 chico_state_t *state = (chico_state_t *) vstate;
 state->a = s;
 state->b = 0xFA346CBFD5890825ULL ^ s;
 state->c = (~0xFA346CBFD5890825ULL) ^ s;

 return;
}

static const gsl_rng_type chico_type =
{"chico",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (chico_state_t),
 &chico_set,
 &chico_get,
 &chico_get_double};

const gsl_rng_type *gsl_rng_chico = &chico_type;
