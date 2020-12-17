/*
 *  rng_splitmix.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int splitmix_get (void *vstate);
static double splitmix_get_double (void *vstate);
static void splitmix_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
} splitmix_state_t;


static unsigned long int splitmix_get (void *vstate)
{
 splitmix_state_t *state = vstate;
 unsigned long long z = (state->a += 0x9E3779B97F4A7C15ULL);
 z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
 z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
 return (unsigned int)(z ^ (z >> 31));
}

static double splitmix_get_double (void *vstate)
{
  return splitmix_get(vstate) / (double) UINT_MAX;
}

static void
splitmix_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 splitmix_state_t *state = (splitmix_state_t *) vstate;
 state->a = s;
 return;
}

static const gsl_rng_type splitmix_type =
{"splitmix",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (splitmix_state_t),
 &splitmix_set,
 &splitmix_get,
 &splitmix_get_double};

const gsl_rng_type *gsl_rng_splitmix = &splitmix_type;
