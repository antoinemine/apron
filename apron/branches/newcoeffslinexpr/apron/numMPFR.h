/* ********************************************************************** */
/* numMPFR.h */
/* ********************************************************************** */

#ifndef _NUMMPFR_H_
#define _NUMMPFR_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "num_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTES:
   - use mpfr_set_default_prec to change the precision (global)
   - rounding is always towards +oo
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numMPFR_set(numMPFR_t a, numMPFR_t b)
{ mpfr_set(a,b,GMP_RNDU); }
static inline void numMPFR_set_array(numMPFR_t* a, numMPFR_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_set(a[i],b[i],GMP_RNDU);
}
static inline void numMPFR_set_int(numMPFR_t a, long int i)
{ mpfr_set_si(a,i,GMP_RNDU); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numMPFR_init(numMPFR_t a)
{ mpfr_init(a); }
static inline void numMPFR_init_array(numMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_init(a[i]);
}
static inline void numMPFR_init_set(numMPFR_t a, numMPFR_t b)
{ mpfr_init_set(a,b,GMP_RNDU); }
static inline void numMPFR_init_set_int(numMPFR_t a, long int i)
{ mpfr_init_set_si(a,i,GMP_RNDU); }

static inline void numMPFR_clear(numMPFR_t a)
{ mpfr_clear(a); }
static inline void numMPFR_clear_array(numMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_clear(a[i]);
}

static inline void numMPFR_swap(numMPFR_t a, numMPFR_t b)
{ mpfr_swap(a,b); }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numMPFR_neg(numMPFR_t a, numMPFR_t b)
{ mpfr_neg(a,b,GMP_RNDU); }
static inline void numMPFR_add(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_add(a,b,c,GMP_RNDU); }
static inline void numMPFR_add_uint(numMPFR_t a, numMPFR_t b, unsigned long int c)
{ mpfr_add_ui(a,b,c,GMP_RNDU); }
static inline void numMPFR_sub(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_sub(a,b,c,GMP_RNDU); }
static inline void numMPFR_sub_uint(numMPFR_t a, numMPFR_t b, unsigned long int c)
{ mpfr_sub_ui(a,b,c,GMP_RNDU); }
static inline void numMPFR_mul(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_mul(a,b,c,GMP_RNDU); }
static inline void numMPFR_div(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_div(a,b,c,GMP_RNDU); }

static inline void numMPFR_abs(numMPFR_t a, numMPFR_t b)
{ mpfr_abs(a,b,GMP_RNDU); }
static inline void numMPFR_min(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_min(a,b,c,GMP_RNDU); }
static inline void numMPFR_max(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_max(a,b,c,GMP_RNDU); }
static inline void numMPFR_floor(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_floor(a,b,GMP_RNDU); }
static inline void numMPFR_ceil(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_ceil(a,b,GMP_RNDU); }
static inline void numMPFR_trunc(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_trunc(a,b,GMP_RNDU); }
static inline void numMPFR_sqrt(numMPFR_t up, numMPFR_t down, numMPFR_t b)
{
  mpfr_sqrt(up,b,GMP_RNDU);
  mpfr_sqrt(down,b,GMP_RNDD);
}
static inline void numMPFR_mul_2exp(numMPFR_t a, numMPFR_t b, int c)
{ mpfr_mul_2si(a,b,c,GMP_RNDU); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numMPFR_sgn(numMPFR_t a)
{ int sgn = mpfr_sgn(a);
  if (sgn) sgn = sgn>0 ? 1 : (-1);
  return sgn;
}
static inline int numMPFR_cmp(numMPFR_t a, numMPFR_t b)
{ return mpfr_cmp(a,b); }
static inline int numMPFR_cmp_int(numMPFR_t a, long int b)
{ return mpfr_cmp_si(a,b); }
static inline bool numMPFR_equal(numMPFR_t a, numMPFR_t b)
{ return mpfr_equal_p(a,b); }
static inline bool numMPFR_integer(numMPFR_t a)
{ return mpfr_integer_p(a); }
static inline int numMPFR_hash(numMPFR_t a)
{ return mpfr_get_si(a,GMP_RNDU); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

int numMPFR_snprint(char* s, size_t size, numMPFR_t a);
void numMPFR_fprint(FILE* stream, numMPFR_t a);
static inline void numMPFR_print(numMPFR_t a)
{ numMPFR_fprint(stdout,a); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numMPFR_infty(numMPFR_t a)
{ return mpfr_inf_p(a); }
static inline void numMPFR_set_infty(numMPFR_t a, int sgn)
{ mpfr_set_inf(a,sgn); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t numMPFR_serialize(void* dst, numMPFR_t src);
size_t numMPFR_deserialize(numMPFR_t dst, const void* src);
/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numMPFR_serialized_size(numMPFR_t a)
{ return mpfr_get_prec(a)/8+9+sizeof(mp_limb_t); }

size_t numMPFR_serialize_array(void* dst, numMPFR_t* src, size_t size);
size_t numMPFR_deserialize_array(numMPFR_t* dst, const void* src, size_t size);
size_t numMPFR_serialized_size_array(numMPFR_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* See num_conv.h.tmpl */

#ifdef __cplusplus
}
#endif

#endif
