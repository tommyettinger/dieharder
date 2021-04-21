/*
 *  rng_tsundere.c
 *
 * Tsundere is a Japanese term for someone who seems cold and hard to outsiders, but is internally warm and caring.
 * From outside the tsundere generator, its inner processes may seem impenetrable, but the internal initialization is very forgiving.
 *
 * Any relation to Pelle Evensen's Konadare generator is purely coincidental, and so by no means should any similarity between that
 * generator and this one be seen as affecting the licensing, ownership, or copyrights of Konadare.
 */

#include <dieharder/libdieharder.h>

static unsigned long int tsundere_get (void *vstate);
static double tsundere_get_double (void *vstate);
static void tsundere_set (void *vstate, unsigned long int s);

typedef struct {
 unsigned long long a;
 unsigned long long b;
 unsigned long long c;
} tsundere_state_t;

#define TSUNDERE_INC 0xBB67AE8584CAA73BULL

static inline unsigned long long ror64(unsigned long long v, int r) {
#ifdef __has_builtin_rotateright64 
  return __builtin_rotateright64(v, r);
#else
  r &= 63;
  return (v >> r) | (v << (64 - r));
#endif
}

static unsigned long int tsundere_get (void *vstate)
{
  tsundere_state_t *s = vstate;
  unsigned long long out = (s->b + s->c);
  s->a += TSUNDERE_INC;
  s->b = ror64(s->b + s->a, 45);
  s->c = ror64(s->c ^ s->b, 37);
  return (unsigned int)(out);
}

static double tsundere_get_double (void *vstate)
{
  return tsundere_get (vstate) / (double) UINT_MAX;
}

static void
tsundere_set (void *vstate, unsigned long int s)
{

 /* Initialize automaton using specified seed. */
 tsundere_state_t *state = (tsundere_state_t *) vstate;
 state->a = s;
 state->b = 0xFA346CBFD5890825ULL ^ s;
 state->c = (~0xFA346CBFD5890825ULL) ^ s;

 return;
}

static const gsl_rng_type tsundere_type =
{"tsundere",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (tsundere_state_t),
 &tsundere_set,
 &tsundere_get,
 &tsundere_get_double};

const gsl_rng_type *gsl_rng_tsundere = &tsundere_type;
