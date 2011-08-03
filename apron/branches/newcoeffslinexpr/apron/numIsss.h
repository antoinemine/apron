/* ********************************************************************** */
/* numIsss.h */
/* ********************************************************************** */

#ifndef _NUMIsss_H_
#define _NUMIsss_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "num_types.h"
#include "num_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numIsss_set(numIsss_t a, numIsss_t b)
{ *a = *b; }
static inline void numIsss_set_array(numIsss_t* a, numIsss_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numIsss_t)); }
static inline void numIsss_set_int(numIsss_t a, long int i)
{ *a = (numIsss_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numIsss_init(numIsss_t a)
{ *a = NUMIsss_ZERO; }
static inline void numIsss_init_array(numIsss_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMIsss_ZERO;
}
static inline void numIsss_init_set(numIsss_t a, numIsss_t b)
{ numIsss_set(a,b); }
static inline void numIsss_init_set_int(numIsss_t a, long int i)
{ numIsss_set_int(a,i); }

static inline void numIsss_clear(numIsss_t a)
{}
static inline void numIsss_clear_array(numIsss_t* a, size_t size)
{}
static inline void numIsss_swap(numIsss_t a, numIsss_t b)
{ numIsss_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numIsss_neg(numIsss_t a, numIsss_t b)
{ *a = -(*b); }
static inline void numIsss_abs(numIsss_t a, numIsss_t b)
{ *a = sssabs(*b); }
static inline void numIsss_add(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = *b + *c; }
static inline void numIsss_add_uint(numIsss_t a, numIsss_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numIsss_sub(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = *b - *c; }
static inline void numIsss_sub_uint(numIsss_t a, numIsss_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numIsss_mul(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = *b * *c; }

static inline void numIsss_tdiv_q(numIsss_t q, numIsss_t a, numIsss_t b)
{ *q = *a / *b; }

static inline int numIsss_sgn(numIsss_t a)
{ return (*a==NUMIsss_ZERO ? 0 : (*a>NUMIsss_ZERO ? 1 : -1)); }

static inline void numIsss_fdiv_q(numIsss_t q, numIsss_t a, numIsss_t b)
{
  if (numIsss_sgn(a)*numIsss_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numIsss_cdiv_q(numIsss_t q, numIsss_t a, numIsss_t b)
{
  if (numIsss_sgn(a)*numIsss_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}

void numIsss_cdiv_qr(numIsss_t q, numIsss_t r, numIsss_t a, numIsss_t b);

static inline void numIsss_cdiv_q_2exp(numIsss_t a, numIsss_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMIsss_ONE<<c)-NUMIsss_ONE) ? 1 : 0); }
static inline void numIsss_fdiv_q_2exp(numIsss_t a, numIsss_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numIsss_min(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numIsss_max(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numIsss_max_exact_double = 1LL << 52;

void numIsss_sqrt(numIsss_t up, numIsss_t down, numIsss_t b);

static inline void numIsss_mul_2exp(numIsss_t a, numIsss_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numIsss_cdiv_q_2exp(a,b,-c);
}

static inline void numIsss_trunc(numIsss_t a, numIsss_t b)
{ numIsss_set(a,b); }
static inline void numIsss_floor(numIsss_t a, numIsss_t b)
{ numIsss_set(a,b); }
static inline void numIsss_ceil(numIsss_t a, numIsss_t b)
{ numIsss_set(a,b); }
static inline void numIsss_div(numIsss_t a, numIsss_t b, numIsss_t c)
{ numIsss_cdiv_q(a,b,c); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numIsss_divexact(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = *b / *c; }
static inline void numIsss_mod(numIsss_t a, numIsss_t b, numIsss_t c)
{ *a = *b % *c; }

numIsss_native _numIsss_gcd_aux(numIsss_native a, numIsss_native b);

static inline void numIsss_gcd(numIsss_t a, numIsss_t b,  numIsss_t c)
{ *a = _numIsss_gcd_aux(*b,*c); }

static inline numIsss_native _lcm_auxIsss(numIsss_native a, numIsss_native b)
{
  numIsss_abs(&a,&a);
  numIsss_abs(&b,&b);
  return (a / _numIsss_gcd_aux(a,b)) * b;
}
static inline void numIsss_lcm(numIsss_t a, numIsss_t b,  numIsss_t c)
{ *a = _lcm_auxIsss(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numIsss_cmp(numIsss_t a, numIsss_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numIsss_cmp_int(numIsss_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numIsss_equal(numIsss_t a, numIsss_t b)
{ return *a==*b; }
static inline bool numIsss_integer(numIsss_t a)
{ return true; }
static inline int numIsss_hash(numIsss_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numIsss_print(numIsss_t a)
{ printf("%sssi",*a); }
static inline void numIsss_fprint(FILE* stream, numIsss_t a)
{ fprintf(stream,"%sssi",*a); }
static inline int numIsss_snprint(char* s, int size, numIsss_t a)
{ return ap_snprintf(s,size,"%sssi",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numIsss_serialize_id(void)
{ return sizeof(numIsss_t)/4; }

static inline size_t numIsss_serialize(void* dst, numIsss_t src)
{
  num_store_words8(dst,src,sizeof(numIsss_t));
  return sizeof(numIsss_t);
}

static inline size_t numIsss_deserialize(numIsss_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numIsss_t));
  return sizeof(numIsss_t);
}

static inline size_t numIsss_serialized_size(numIsss_t a)
{ return sizeof(numIsss_t); }

size_t numIsss_serialize_array(void* dst, numIsss_t* src, size_t size);
size_t numIsss_deserialize_array(numIsss_t* dst, const void* src, size_t size);
size_t numIsss_serialized_size_array(numIsss_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* see num_conv.h.tmpl */

#undef MARK_numIsss

#ifdef __cplusplus
}
#endif

#endif
