/* ********************************************************************** */
/* numfltD_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTD_DEF_H_
#define _NUMFLTD_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "numConfig.h"
#include "numfltD.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numfltD_set(numfltD_t a, numfltD_t b)
{ *a = *b; }
static inline void numfltD_set_array(numfltD_t* a, numfltD_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numfltD_t)); }
static inline void numfltD_set_int(numfltD_t a, long int i)
{ *a = (numfltD_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numfltD_init(numfltD_t a)
{ *a = NUMFLTD_ZERO; }
static inline void numfltD_init_array(numfltD_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMFLTD_ZERO;
}
static inline void numfltD_init_set(numfltD_t a, numfltD_t b)
{ numfltD_set(a,b); }
static inline void numfltD_init_set_int(numfltD_t a, long int i)
{ numfltD_set_int(a,i); }

static inline void numfltD_clear(numfltD_t a)
{}
static inline void numfltD_clear_array(numfltD_t* a, size_t size)
{}

static inline void numfltD_swap(numfltD_t a, numfltD_t b)
{ numfltD_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numfltD_neg(numfltD_t a, numfltD_t b)
{ *a = -(*b); }
static inline void numfltD_add(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b + *c; }
static inline void numfltD_add_uint(numfltD_t a, numfltD_t b, unsigned long int c)
{ *a = *b + (numfltD_native)c; }
static inline void numfltD_sub(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b - *c; }
static inline void numfltD_sub_uint(numfltD_t a, numfltD_t b, unsigned long int c)
{ *a = *b - (numfltD_native)c; }
static inline void numfltD_mul(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b * *c; }
static inline void numfltD_div(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b / *c; }

static inline void numfltD_abs(numfltD_t a, numfltD_t b)
{ *a = fabs(*b); }
static inline void numfltD_mul_2(numfltD_t a, numfltD_t b)
{ *a = ldexp(*b,1); }
static inline void numfltD_div_2(numfltD_t a, numfltD_t b)
{ *a = ldexp(*b,-1); }
static inline void numfltD_min(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = fmin(*b,*c); }
static inline void numfltD_max(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = fmax(*b,*c); }
static inline void numfltD_floor(numfltD_t a, numfltD_t b)
{ *a = floor(*b); }
static inline void numfltD_ceil(numfltD_t a, numfltD_t b)
{ *a = ceil(*b); }
static inline void numfltD_trunc(numfltD_t a, numfltD_t b)
{ *a = trunc(*b); }
static inline void numfltD_sqrt(numfltD_t up, numfltD_t down, numfltD_t b)
{
  numfltD_t x;
  assert(*b>=0);
  *x = sqrt(*b);
  assert(*x**x>=*b); /* assumes round towards +oo! */
  if (*x**x==*b) *down = *x;
  else *down = nextafter(*x,0);
  *up = *x;
}
static inline void numfltD_mul_2exp(numfltD_t a, numfltD_t b, int c)
{ *a = ldexp(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numfltD_sgn(numfltD_t a)
{ return (*a==NUMFLTD_ZERO ? 0 : (*a>NUMFLTD_ZERO ? 1 : -1)); }
static inline int numfltD_cmp(numfltD_t a, numfltD_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numfltD_cmp_int(numfltD_t a, long int b)
{
  numfltD_native bb = (numfltD_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numfltD_equal(numfltD_t a, numfltD_t b)
{ return *a==*b; }

static inline bool numfltD_integer(numfltD_t a)
{ return ceil(*a) == *a; }
static inline int numfltD_hash(numfltD_t a)
{
 if (*a <= -(double)(INT_MAX))
    return -INT_MAX;
  else if (*a < (double)INT_MAX)
    return lrint(*a);
  else
    return INT_MAX;
}


/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numfltD_print(numfltD_t a)
{ printf("%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }
static inline void numfltD_fprint(FILE* stream, numfltD_t a)
{ fprintf(stream,"%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }
static inline int numfltD_snprint(char* s, size_t size, numfltD_t a)
{ return snprintf(s,size,"%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numfltD_infty(numfltD_t a)
{ return !isfinite(*a); }
static inline void numfltD_set_infty(numfltD_t a, int sgn)
{ *a = sgn>0 ? NUMFLTD_MAX : -NUMFLTD_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numfltD_serialize(void* dst, numfltD_t src)
{
  num_store_words8(dst,src,sizeof(numfltD_t));
  return sizeof(numfltD_t);
}

static inline size_t numfltD_deserialize(numfltD_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numfltD_t));
  return sizeof(numfltD_t);
}

static inline size_t numfltD_serialized_size(numfltD_t a)
{ return sizeof(numfltD_t); }

#ifdef __cplusplus
}
#endif

#endif
