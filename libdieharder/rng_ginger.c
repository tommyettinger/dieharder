/*
 *  rng_ginger.c
 */

#include <dieharder/libdieharder.h>

static unsigned long int ginger_get (void *vstate);
static double ginger_get_double (void *vstate);
static void ginger_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
} ginger_state_t;


static unsigned long int ginger_get (void *vstate)
{
 ginger_state_t *state = vstate;
// const unsigned long long s = (state->a += ~(state->b = (state->b >> 1ULL ^ (0ULL - (state->b & 1ULL) & 0xD800000000000000ULL)))) * 0xCC62FCEB9202FAADULL;
 unsigned long long s = (state->a += 0xC6BC279692B5C323ULL ^ (state->b = (state->b >> 1ULL ^ (0ULL - (state->b & 1ULL) & 0xD800000000000000ULL))));//0xAF251AF3B0F025B5ULL;// + 0xC6BC279692B5C323ULL;
// s = (s ^ s >> 31) * 0xCC62FCEB9202FAADULL; //0xC6BC279692B5C323ULL
// unsigned long long s = (state->a = (state->a >> 1ULL ^ (0ULL - (state->a & 1ULL) & 0xD800000000000000ULL)));
// s = (s ^ s >> 31) * (state->b += 0x50DAE50C4F0FDA4AULL);
 s = (s ^ s >> 31) * 0xCC62FCEB9202FAADUL;
 return (unsigned int)(s ^ s >> 28);
}

static double ginger_get_double (void *vstate)
{
  return ginger_get (vstate) / (double) UINT_MAX;
}

static void
ginger_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 ginger_state_t *state = (ginger_state_t *) vstate;
 state->a = s;
 state->b = s == 0 ? 1ULL : s;
 return;
}

static const gsl_rng_type ginger_type =
{"ginger",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (ginger_state_t),
 &ginger_set,
 &ginger_get,
 &ginger_get_double};

const gsl_rng_type *gsl_rng_ginger = &ginger_type;
