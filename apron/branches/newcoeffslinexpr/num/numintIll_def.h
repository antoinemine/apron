/* ********************************************************************** */
/* numintIll_def.h */
/* ********************************************************************** */

#ifndef _NUMINTILL_DEF_H_
#define _NUMINTILL_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "numConfig.h"
/* Require C99 compliant compiler */
#include "numintIll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numintIll_set(numintIll_t a, numintIll_t b)
{ *a = *b; }
static inline void numintIll_set_array(numintIll_t* a, numintIll_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numintIll_t)); }
static inline void numintIll_set_int(numintIll_t a, long int i)
{ *a = (numintIll_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintIll_init(numintIll_t a)
{ *a = NUMINTILL_ZERO; }
static inline void numintIll_init_array(numintIll_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMINTILL_ZERO;
}
static inline void numintIll_init_set(numintIll_t a, numintIll_t b)
{ numintIll_set(a,b); }
static inline void numintIll_init_set_int(numintIll_t a, long int i)
{ numintIll_set_int(a,i); }

static inline void numintIll_clear(numintIll_t a)
{}
static inline void numintIll_clear_array(numintIll_t* a, size_t size)
{}
static inline void numintIll_swap(numintIll_t a, numintIll_t b)
{ numintIll_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintIll_neg(numintIll_t a, numintIll_t b)
{ *a = -(*b); }
static inline void numintIll_abs(numintIll_t a, numintIll_t b)
{ *a = llabs(*b); }
static inline void numintIll_add(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = *b + *c; }
static inline void numintIll_add_uint(numintIll_t a, numintIll_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numintIll_sub(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = *b - *c; }
static inline void numintIll_sub_uint(numintIll_t a, numintIll_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numintIll_mul(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = *b * *c; }
static inline void numintIll_mul_2(numintIll_t a, numintIll_t b)
{ *a = *b << 1; }

static inline void numintIll_tdiv_q(numintIll_t q, numintIll_t a, numintIll_t b)
{ *q = *a / *b; }

static inline int numintIll_sgn(numintIll_t a)
{ return (*a==NUMINTILL_ZERO ? 0 : (*a>NUMINTILL_ZERO ? 1 : -1)); }

static inline void numintIll_fdiv_q(numintIll_t q, numintIll_t a, numintIll_t b)
{
  if (numintIll_sgn(a)*numintIll_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numintIll_cdiv_q(numintIll_t q, numintIll_t a, numintIll_t b)
{
  if (numintIll_sgn(a)*numintIll_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}
static inline void numintIll_cdiv_qr(numintIll_t q, numintIll_t r, numintIll_t a, numintIll_t b)
{
  numintIll_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numintIll_sgn(a)*numintIll_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

static inline void numintIll_cdiv_2(numintIll_t a, numintIll_t b)
{ *a = (*b>=NUMINTILL_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numintIll_cdiv_q_2exp(numintIll_t a, numintIll_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMINTILL_ONE<<c)-NUMINTILL_ONE) ? 1 : 0); }
static inline void numintIll_fdiv_q_2exp(numintIll_t a, numintIll_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numintIll_min(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numintIll_max(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numintIll_max_exact_double = 1LL << 52;

static inline void numintIll_sqrt(numintIll_t up, numintIll_t down, numintIll_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numintIll_max_exact_double) {
    /* ceil(sqrt(b)) can be exactly represented as a double */
    *up = ceil(f);
    *down = floor(f);
  }
  else {
    /* (unlikely) case where ulp(sqrt(b)) might  be greater than 1 */
    *up = ceil(nextafter(f,+1/0.));
    *down = floor(nextafter(f,0.));
  }
}

static inline void numintIll_mul_2exp(numintIll_t a, numintIll_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numintIll_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numintIll_divexact(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = *b / *c; }
static inline void numintIll_mod(numintIll_t a, numintIll_t b, numintIll_t c)
{ *a = *b % *c; }
static inline numintIll_native _gcd_auxIll(numintIll_native a, numintIll_native b)
{
  numintIll_native t;
  numintIll_abs(&a,&a);
  numintIll_abs(&b,&b);
  if (a<b){
    numintIll_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMINTILL_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline void numintIll_gcd(numintIll_t a, numintIll_t b,  numintIll_t c)
{ *a = _gcd_auxIll(*b,*c); }

static inline numintIll_native _lcm_auxIll(numintIll_native a, numintIll_native b)
{
  numintIll_abs(&a,&a);
  numintIll_abs(&b,&b);
  return (a / _gcd_auxIll(a,b)) * b;
}
static inline void numintIll_lcm(numintIll_t a, numintIll_t b,  numintIll_t c)
{ *a = _lcm_auxIll(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintIll_cmp(numintIll_t a, numintIll_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numintIll_cmp_int(numintIll_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numintIll_equal(numintIll_t a, numintIll_t b)
{ return *a==*b; }
static inline bool numintIll_integer(numintIll_t a)
{ return true; }
static inline int numintIll_hash(numintIll_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintIll_print(numintIll_t a)
{ printf("%lli",*a); }
static inline void numintIll_fprint(FILE* stream, numintIll_t a)
{ fprintf(stream,"%lli",*a); }
static inline int numintIll_snprint(char* s, size_t size, numintIll_t a)
{ return snprintf(s,size,"%lli",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintIll_serialize_id(void)
{ return sizeof(numintIll_t)/4; }

static inline size_t numintIll_serialize(void* dst, numintIll_t src)
{
  num_store_words8(dst,src,sizeof(numintIll_t));
  return sizeof(numintIll_t);
}

static inline size_t numintIll_deserialize(numintIll_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numintIll_t));
  return sizeof(numintIll_t);
}

static inline size_t numintIll_serialized_size(numintIll_t a)
{ return sizeof(numintIll_t); }

#ifdef __cplusplus
}
#endif

#endif
