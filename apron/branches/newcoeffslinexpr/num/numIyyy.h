/* ********************************************************************** */
/* numIyyy.h */
/* ********************************************************************** */

#ifndef _NUMIyyy_H_
#define _NUMIyyy_H_

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

static inline void numIyyy_set(numIyyy_t a, numIyyy_t b)
{ *a = *b; }
static inline void numIyyy_set_array(numIyyy_t* a, numIyyy_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numIyyy_t)); }
static inline void numIyyy_set_int(numIyyy_t a, long int i)
{ *a = (numIyyy_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numIyyy_init(numIyyy_t a)
{ *a = NUMIyyy_ZERO; }
static inline void numIyyy_init_array(numIyyy_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMIyyy_ZERO;
}
static inline void numIyyy_init_set(numIyyy_t a, numIyyy_t b)
{ numIyyy_set(a,b); }
static inline void numIyyy_init_set_int(numIyyy_t a, long int i)
{ numIyyy_set_int(a,i); }

static inline void numIyyy_clear(numIyyy_t a)
{}
static inline void numIyyy_clear_array(numIyyy_t* a, size_t size)
{}
static inline void numIyyy_swap(numIyyy_t a, numIyyy_t b)
{ numIyyy_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numIyyy_neg(numIyyy_t a, numIyyy_t b)
{ *a = -(*b); }
static inline void numIyyy_abs(numIyyy_t a, numIyyy_t b)
{ *a = yyyabs(*b); }
static inline void numIyyy_add(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = *b + *c; }
static inline void numIyyy_add_uint(numIyyy_t a, numIyyy_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numIyyy_sub(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = *b - *c; }
static inline void numIyyy_sub_uint(numIyyy_t a, numIyyy_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numIyyy_mul(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = *b * *c; }
static inline void numIyyy_mul_2(numIyyy_t a, numIyyy_t b)
{ *a = *b << 1; }

static inline void numIyyy_tdiv_q(numIyyy_t q, numIyyy_t a, numIyyy_t b)
{ *q = *a / *b; }

static inline int numIyyy_sgn(numIyyy_t a)
{ return (*a==NUMIyyy_ZERO ? 0 : (*a>NUMIyyy_ZERO ? 1 : -1)); }

static inline void numIyyy_fdiv_q(numIyyy_t q, numIyyy_t a, numIyyy_t b)
{
  if (numIyyy_sgn(a)*numIyyy_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numIyyy_cdiv_q(numIyyy_t q, numIyyy_t a, numIyyy_t b)
{
  if (numIyyy_sgn(a)*numIyyy_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}

void numIyyy_cdiv_qr(numIyyy_t q, numIyyy_t r, numIyyy_t a, numIyyy_t b);

static inline void numIyyy_cdiv_2(numIyyy_t a, numIyyy_t b)
{ *a = (*b>=NUMIyyy_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numIyyy_cdiv_q_2exp(numIyyy_t a, numIyyy_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMIyyy_ONE<<c)-NUMIyyy_ONE) ? 1 : 0); }
static inline void numIyyy_fdiv_q_2exp(numIyyy_t a, numIyyy_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numIyyy_min(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numIyyy_max(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numIyyy_max_exact_double = 1LL << 52;

void numIyyy_sqrt(numIyyy_t up, numIyyy_t down, numIyyy_t b);

static inline void numIyyy_mul_2exp(numIyyy_t a, numIyyy_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numIyyy_cdiv_q_2exp(a,b,-c);
}

static inline void numIyyy_trunc(numIyyy_t a, numIyyy_t b)
{ numIyyy_set(a,b); }
static inline void numIyyy_floor(numIyyy_t a, numIyyy_t b)
{ numIyyy_set(a,b); }
static inline void numIyyy_ceil(numIyyy_t a, numIyyy_t b)
{ numIyyy_set(a,b); }
static inline void numIyyy_div(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ numIyyy_cdiv_q(a,b,c); }
static inline void numIyyy_div_2(numIyyy_t a, numIyyy_t b)
{ numIyyy_cdiv_2(a,b); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numIyyy_divexact(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = *b / *c; }
static inline void numIyyy_mod(numIyyy_t a, numIyyy_t b, numIyyy_t c)
{ *a = *b % *c; }

numIyyy_native _numIyyy_gcd_aux(numIyyy_native a, numIyyy_native b);

static inline void numIyyy_gcd(numIyyy_t a, numIyyy_t b,  numIyyy_t c)
{ *a = _numIyyy_gcd_aux(*b,*c); }

static inline numIyyy_native _lcm_auxIyyy(numIyyy_native a, numIyyy_native b)
{
  numIyyy_abs(&a,&a);
  numIyyy_abs(&b,&b);
  return (a / _numIyyy_gcd_aux(a,b)) * b;
}
static inline void numIyyy_lcm(numIyyy_t a, numIyyy_t b,  numIyyy_t c)
{ *a = _lcm_auxIyyy(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numIyyy_cmp(numIyyy_t a, numIyyy_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numIyyy_cmp_int(numIyyy_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numIyyy_equal(numIyyy_t a, numIyyy_t b)
{ return *a==*b; }
static inline bool numIyyy_integer(numIyyy_t a)
{ return true; }
static inline int numIyyy_hash(numIyyy_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numIyyy_print(numIyyy_t a)
{ printf("%yyyi",*a); }
static inline void numIyyy_fprint(FILE* stream, numIyyy_t a)
{ fprintf(stream,"%yyyi",*a); }
static inline int numIyyy_snprint(char* s, size_t size, numIyyy_t a)
{ return snprintf(s,size,"%yyyi",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numIyyy_serialize_id(void)
{ return sizeof(numIyyy_t)/4; }

static inline size_t numIyyy_serialize(void* dst, numIyyy_t src)
{
  num_store_words8(dst,src,sizeof(numIyyy_t));
  return sizeof(numIyyy_t);
}

static inline size_t numIyyy_deserialize(numIyyy_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numIyyy_t));
  return sizeof(numIyyy_t);
}

static inline size_t numIyyy_serialized_size(numIyyy_t a)
{ return sizeof(numIyyy_t); }

size_t numIyyy_serialize_array(void* dst, numIyyy_t* src, size_t size);
size_t numIyyy_deserialize_array(numIyyy_t* dst, const void* src, size_t size);
size_t numIyyy_serialized_size_array(numIyyy_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* see num_conv.h.tmpl */

#ifdef __cplusplus
}
#endif

#endif
