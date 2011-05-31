/* ********************************************************************** */
/* numDyyy.h */
/* ********************************************************************** */

#ifndef _NUMDyyy_H_
#define _NUMDyyy_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "num_types.h"
#include "num_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MARK_numDyyy

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numDyyy_set(numDyyy_t a, numDyyy_t b)
{ *a = *b; }
static inline void numDyyy_set_array(numDyyy_t* a, numDyyy_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numDyyy_t)); }
static inline void numDyyy_set_int(numDyyy_t a, long int i)
{ *a = (numDyyy_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numDyyy_init(numDyyy_t a)
{ *a = NUMDyyy_ZERO; }
static inline void numDyyy_init_array(numDyyy_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMDyyy_ZERO;
}
static inline void numDyyy_init_set(numDyyy_t a, numDyyy_t b)
{ numDyyy_set(a,b); }
static inline void numDyyy_init_set_int(numDyyy_t a, long int i)
{ numDyyy_set_int(a,i); }

static inline void numDyyy_clear(numDyyy_t a)
{}
static inline void numDyyy_clear_array(numDyyy_t* a, size_t size)
{}

static inline void numDyyy_swap(numDyyy_t a, numDyyy_t b)
{ numDyyy_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numDyyy_neg(numDyyy_t a, numDyyy_t b)
{ *a = -(*b); }
static inline void numDyyy_add(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b + *c; }
static inline void numDyyy_add_uint(numDyyy_t a, numDyyy_t b, unsigned long int c)
{ *a = *b + (numDyyy_native)c; }
static inline void numDyyy_sub(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b - *c; }
static inline void numDyyy_sub_uint(numDyyy_t a, numDyyy_t b, unsigned long int c)
{ *a = *b - (numDyyy_native)c; }
static inline void numDyyy_mul(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b * *c; }
static inline void numDyyy_div(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b / *c; }

static inline void numDyyy_abs(numDyyy_t a, numDyyy_t b)
{ *a = fabsyyy(*b); }
static inline void numDyyy_mul_2(numDyyy_t a, numDyyy_t b)
{ *a = ldexpyyy(*b,1); }
static inline void numDyyy_div_2(numDyyy_t a, numDyyy_t b)
{ *a = ldexpyyy(*b,-1); }
static inline void numDyyy_min(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = fminyyy(*b,*c); }
static inline void numDyyy_max(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = fmaxyyy(*b,*c); }
static inline void numDyyy_floor(numDyyy_t a, numDyyy_t b)
{ *a = flooryyy(*b); }
static inline void numDyyy_ceil(numDyyy_t a, numDyyy_t b)
{ *a = ceilyyy(*b); }
static inline void numDyyy_trunc(numDyyy_t a, numDyyy_t b)
{ *a = truncyyy(*b); }
void numDyyy_sqrt(numDyyy_t up, numDyyy_t down, numDyyy_t b);
static inline void numDyyy_mul_2exp(numDyyy_t a, numDyyy_t b, int c)
{ *a = ldexpyyy(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numDyyy_sgn(numDyyy_t a)
{ return (*a==NUMDyyy_ZERO ? 0 : (*a>NUMDyyy_ZERO ? 1 : -1)); }
static inline int numDyyy_cmp(numDyyy_t a, numDyyy_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numDyyy_cmp_int(numDyyy_t a, long int b)
{
  numDyyy_native bb = (numDyyy_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numDyyy_equal(numDyyy_t a, numDyyy_t b)
{ return *a==*b; }

static inline bool numDyyy_integer(numDyyy_t a)
{ return ceilyyy(*a) == *a; }
int numDyyy_hash(numDyyy_t a);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

#if defined(MARK_numD)
static inline void numD_print(numD_t a)
{ printf("%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline void numD_fprint(FILE* stream, numD_t a)
{ fprintf(stream,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline int numD_snprint(char* s, size_t size, numD_t a)
{ return snprintf(s,size,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
#elif defined(MARK_numDl)
static inline void numDl_print(numDl_t a)
{ printf("%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline void numDl_fprint(FILE* stream, numDl_t a)
{ fprintf(stream,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline int numDl_snprint(char* s, size_t size, numDl_t a)
{ return snprintf(s,size,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
#else
#error "HERE"
#endif

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numDyyy_infty(numDyyy_t a)
{ return !isfinite(*a); }
static inline void numDyyy_set_infty(numDyyy_t a, int sgn)
{ *a = sgn>0 ? NUMDyyy_MAX : -NUMDyyy_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numDyyy_serialize(void* dst, numDyyy_t src)
{
  num_store_words8(dst,src,sizeof(numDyyy_t));
  return sizeof(numDyyy_t);
}

static inline size_t numDyyy_deserialize(numDyyy_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numDyyy_t));
  return sizeof(numDyyy_t);
}

static inline size_t numDyyy_serialized_size(numDyyy_t a)
{ return sizeof(numDyyy_t); }

size_t numDyyy_serialize_array(void* dst, numDyyy_t* src, size_t size);
size_t numDyyy_deserialize_array(numDyyy_t* dst, const void* src, size_t size);
size_t numDyyy_serialized_size_array(numDyyy_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* See num_conv.h.tmpl */

#undef MARK_numDyyy

#ifdef __cplusplus
}
#endif

#endif
