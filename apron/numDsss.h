/* ********************************************************************** */
/* numDwww.h */
/* ********************************************************************** */

#ifndef _NUMDwww_H_
#define _NUMDwww_H_

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

#define MARK_numDwww

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numDwww_set(numDwww_t a, numDwww_t b)
{ *a = *b; }
static inline void numDwww_set_array(numDwww_t* a, numDwww_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numDwww_t)); }
static inline void numDwww_set_int(numDwww_t a, long int i)
{ *a = (numDwww_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numDwww_init(numDwww_t a)
{ *a = NUMDwww_ZERO; }
static inline void numDwww_init_array(numDwww_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMDwww_ZERO;
}
static inline void numDwww_init_set(numDwww_t a, numDwww_t b)
{ numDwww_set(a,b); }
static inline void numDwww_init_set_int(numDwww_t a, long int i)
{ numDwww_set_int(a,i); }

static inline void numDwww_clear(numDwww_t a)
{}
static inline void numDwww_clear_array(numDwww_t* a, size_t size)
{}

static inline void numDwww_swap(numDwww_t a, numDwww_t b)
{ numDwww_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numDwww_neg(numDwww_t a, numDwww_t b)
{ *a = -(*b); }
static inline void numDwww_add(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = *b + *c; }
static inline void numDwww_add_uint(numDwww_t a, numDwww_t b, unsigned long int c)
{ *a = *b + (numDwww_native)c; }
static inline void numDwww_sub(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = *b - *c; }
static inline void numDwww_sub_uint(numDwww_t a, numDwww_t b, unsigned long int c)
{ *a = *b - (numDwww_native)c; }
static inline void numDwww_mul(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = *b * *c; }
static inline void numDwww_div(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = *b / *c; }

static inline void numDwww_abs(numDwww_t a, numDwww_t b)
{ *a = fabswww(*b); }
static inline void numDwww_min(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = fminwww(*b,*c); }
static inline void numDwww_max(numDwww_t a, numDwww_t b, numDwww_t c)
{ *a = fmaxwww(*b,*c); }
static inline void numDwww_floor(numDwww_t a, numDwww_t b)
{ *a = floorwww(*b); }
static inline void numDwww_ceil(numDwww_t a, numDwww_t b)
{ *a = ceilwww(*b); }
static inline void numDwww_trunc(numDwww_t a, numDwww_t b)
{ *a = truncwww(*b); }
void numDwww_sqrt(numDwww_t up, numDwww_t down, numDwww_t b);
static inline void numDwww_mul_2exp(numDwww_t a, numDwww_t b, int c)
{ *a = ldexpwww(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numDwww_sgn(numDwww_t a)
{ return (*a==NUMDwww_ZERO ? 0 : (*a>NUMDwww_ZERO ? 1 : -1)); }
static inline int numDwww_cmp(numDwww_t a, numDwww_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numDwww_cmp_int(numDwww_t a, long int b)
{
  numDwww_native bb = (numDwww_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numDwww_equal(numDwww_t a, numDwww_t b)
{ return *a==*b; }

static inline bool numDwww_integer(numDwww_t a)
{ return ceilwww(*a) == *a; }
int numDwww_hash(numDwww_t a);

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

static inline bool numDwww_infty(numDwww_t a)
{ return !isfinite(*a); }
static inline void numDwww_set_infty(numDwww_t a, int sgn)
{ *a = sgn>0 ? NUMDwww_MAX : -NUMDwww_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numDwww_serialize(void* dst, numDwww_t src)
{
  num_store_words8(dst,src,sizeof(numDwww_t));
  return sizeof(numDwww_t);
}

static inline size_t numDwww_deserialize(numDwww_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numDwww_t));
  return sizeof(numDwww_t);
}

static inline size_t numDwww_serialized_size(numDwww_t a)
{ return sizeof(numDwww_t); }

size_t numDwww_serialize_array(void* dst, numDwww_t* src, size_t size);
size_t numDwww_deserialize_array(numDwww_t* dst, const void* src, size_t size);
size_t numDwww_serialized_size_array(numDwww_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* See num_conv.h.tmpl */
#undef MARK_numDwww

#ifdef __cplusplus
}
#endif

#endif
