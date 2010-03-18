/* ********************************************************************** */
/* numfltDl_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTDL_DEF_H_
#define _NUMFLTDL_DEF_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "numConfig.h"
#include "numfltDl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numfltDl_set(numfltDl_t a, numfltDl_t b)
{ *a = *b; }
static inline void numfltDl_set_array(numfltDl_t* a, numfltDl_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numfltDl_t)); }
static inline void numfltDl_set_int(numfltDl_t a, long int i)
{ *a = (numfltDl_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numfltDl_init(numfltDl_t a)
{ *a = NUMFLTDL_ZERO; }
static inline void numfltDl_init_array(numfltDl_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMFLTDL_ZERO;
}
static inline void numfltDl_init_set(numfltDl_t a, numfltDl_t b)
{ numfltDl_set(a,b); }
static inline void numfltDl_init_set_int(numfltDl_t a, long int i)
{ numfltDl_set_int(a,i); }

static inline void numfltDl_clear(numfltDl_t a)
{}
static inline void numfltDl_clear_array(numfltDl_t* a, size_t size)
{}

static inline void numfltDl_swap(numfltDl_t a, numfltDl_t b)
{ numfltDl_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numfltDl_neg(numfltDl_t a, numfltDl_t b)
{ *a = -(*b); }
static inline void numfltDl_add(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b + *c; }
static inline void numfltDl_add_uint(numfltDl_t a, numfltDl_t b, unsigned long int c)
{ *a = *b + (numfltDl_native)c; }
static inline void numfltDl_sub(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b - *c; }
static inline void numfltDl_sub_uint(numfltDl_t a, numfltDl_t b, unsigned long int c)
{ *a = *b - (numfltDl_native)c; }
static inline void numfltDl_mul(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b * *c; }
static inline void numfltDl_div(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b / *c; }

static inline void numfltDl_abs(numfltDl_t a, numfltDl_t b)
{ *a = fabsl(*b); }
static inline void numfltDl_mul_2(numfltDl_t a, numfltDl_t b)
{ *a = ldexpl(*b,1); }
static inline void numfltDl_div_2(numfltDl_t a, numfltDl_t b)
{ *a = ldexpl(*b,-1); }
static inline void numfltDl_min(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = fminl(*b,*c); }
static inline void numfltDl_max(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = fmaxl(*b,*c); }
static inline void numfltDl_floor(numfltDl_t a, numfltDl_t b)
{ *a = floorl(*b); }
static inline void numfltDl_ceil(numfltDl_t a, numfltDl_t b)
{ *a = ceill(*b); }
static inline void numfltDl_trunc(numfltDl_t a, numfltDl_t b)
{ *a = truncl(*b); }
static inline void numfltDl_sqrt(numfltDl_t up, numfltDl_t down, numfltDl_t b)
{
  assert(*b>=0);
  *up = sqrtl(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafterl(*up,0);
}
static inline void numfltDl_mul_2exp(numfltDl_t a, numfltDl_t b, int c)
{ *a = ldexpl(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numfltDl_sgn(numfltDl_t a)
{ return (*a==NUMFLTDL_ZERO ? 0 : (*a>NUMFLTDL_ZERO ? 1 : -1)); }
static inline int numfltDl_cmp(numfltDl_t a, numfltDl_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numfltDl_cmp_int(numfltDl_t a, long int b)
{
  numfltDl_native bb = (numfltDl_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numfltDl_equal(numfltDl_t a, numfltDl_t b)
{ return *a==*b; }

static inline bool numfltDl_integer(numfltDl_t a)
{ return ceill(*a) == *a; }
static inline int numfltDl_hash(numfltDl_t a)
{
 if (*a <= -(long double)(INT_MAX))
    return -INT_MAX;
  else if (*a < (long double)INT_MAX)
    return lrintl(*a);
  else
    return INT_MAX;
}

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numfltDl_print(numfltDl_t a)
{ printf("%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }
static inline void numfltDl_fprint(FILE* stream, numfltDl_t a)
{ fprintf(stream,"%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }
static inline int numfltDl_snprint(char* s, size_t size, numfltDl_t a)
{ return snprintf(s,size,"%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numfltDl_infty(numfltDl_t a)
{ return !isfinite(*a); }
static inline void numfltDl_set_infty(numfltDl_t a, int sgn)
{ *a = sgn>0 ? NUMFLTDL_MAX : -NUMFLTDL_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numfltDl_serialize(void* dst, numfltDl_t src)
{
  num_store_words8(dst,src,sizeof(numfltDl_t));
  return sizeof(numfltDl_t);
}

static inline size_t numfltDl_deserialize(numfltDl_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numfltDl_t));
  return sizeof(numfltDl_t);
}

static inline size_t numfltDl_serialized_size(numfltDl_t a)
{ return sizeof(numfltDl_t); }

#ifdef __cplusplus
}
#endif

#endif
