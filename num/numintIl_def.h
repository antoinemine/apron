/* ********************************************************************** */
/* numintIl_def.h */
/* ********************************************************************** */

#ifndef _NUMINTIL_DEF_H_
#define _NUMINTIL_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "numConfig.h"
/* Require C99 compliant compiler */
#include "numintIl.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numintIl_set(numintIl_t a, numintIl_t b)
{ *a = *b; }
static inline void numintIl_set_array(numintIl_t* a, numintIl_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numintIl_t)); }
static inline void numintIl_set_int(numintIl_t a, long int i)
{ *a = (numintIl_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintIl_init(numintIl_t a)
{ *a = NUMINTIL_ZERO; }
static inline void numintIl_init_array(numintIl_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMINTIL_ZERO;
}
static inline void numintIl_init_set(numintIl_t a, numintIl_t b)
{ numintIl_set(a,b); }
static inline void numintIl_init_set_int(numintIl_t a, long int i)
{ numintIl_set_int(a,i); }

static inline void numintIl_clear(numintIl_t a)
{}
static inline void numintIl_clear_array(numintIl_t* a, size_t size)
{}
static inline void numintIl_swap(numintIl_t a, numintIl_t b)
{ numintIl_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintIl_neg(numintIl_t a, numintIl_t b)
{ *a = -(*b); }
static inline void numintIl_abs(numintIl_t a, numintIl_t b)
{ *a = labs(*b); }
static inline void numintIl_add(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = *b + *c; }
static inline void numintIl_add_uint(numintIl_t a, numintIl_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numintIl_sub(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = *b - *c; }
static inline void numintIl_sub_uint(numintIl_t a, numintIl_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numintIl_mul(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = *b * *c; }
static inline void numintIl_mul_2(numintIl_t a, numintIl_t b)
{ *a = *b << 1; }

static inline void numintIl_tdiv_q(numintIl_t q, numintIl_t a, numintIl_t b)
{ *q = *a / *b; }

static inline int numintIl_sgn(numintIl_t a)
{ return (*a==NUMINTIL_ZERO ? 0 : (*a>NUMINTIL_ZERO ? 1 : -1)); }

static inline void numintIl_fdiv_q(numintIl_t q, numintIl_t a, numintIl_t b)
{
  if (numintIl_sgn(a)*numintIl_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numintIl_cdiv_q(numintIl_t q, numintIl_t a, numintIl_t b)
{
  if (numintIl_sgn(a)*numintIl_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}
static inline void numintIl_cdiv_qr(numintIl_t q, numintIl_t r, numintIl_t a, numintIl_t b)
{
  numintIl_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numintIl_sgn(a)*numintIl_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

static inline void numintIl_cdiv_2(numintIl_t a, numintIl_t b)
{ *a = (*b>=NUMINTIL_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numintIl_cdiv_q_2exp(numintIl_t a, numintIl_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMINTIL_ONE<<c)-NUMINTIL_ONE) ? 1 : 0); }
static inline void numintIl_fdiv_q_2exp(numintIl_t a, numintIl_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numintIl_min(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numintIl_max(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numintIl_max_exact_double = 1LL << 52;

static inline void numintIl_sqrt(numintIl_t up, numintIl_t down, numintIl_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numintIl_max_exact_double) {
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

static inline void numintIl_mul_2exp(numintIl_t a, numintIl_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numintIl_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numintIl_divexact(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = *b / *c; }
static inline void numintIl_mod(numintIl_t a, numintIl_t b, numintIl_t c)
{ *a = *b % *c; }
static inline numintIl_native _gcd_auxIl(numintIl_native a, numintIl_native b)
{
  numintIl_native t;
  numintIl_abs(&a,&a);
  numintIl_abs(&b,&b);
  if (a<b){
    numintIl_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMINTIL_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline void numintIl_gcd(numintIl_t a, numintIl_t b,  numintIl_t c)
{ *a = _gcd_auxIl(*b,*c); }

static inline numintIl_native _lcm_auxIl(numintIl_native a, numintIl_native b)
{
  numintIl_abs(&a,&a);
  numintIl_abs(&b,&b);
  return (a / _gcd_auxIl(a,b)) * b;
}
static inline void numintIl_lcm(numintIl_t a, numintIl_t b,  numintIl_t c)
{ *a = _lcm_auxIl(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintIl_cmp(numintIl_t a, numintIl_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numintIl_cmp_int(numintIl_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numintIl_equal(numintIl_t a, numintIl_t b)
{ return *a==*b; }
static inline bool numintIl_integer(numintIl_t a)
{ return true; }
static inline int numintIl_hash(numintIl_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintIl_print(numintIl_t a)
{ printf("%li",*a); }
static inline void numintIl_fprint(FILE* stream, numintIl_t a)
{ fprintf(stream,"%li",*a); }
static inline int numintIl_snprint(char* s, size_t size, numintIl_t a)
{ return snprintf(s,size,"%li",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintIl_serialize_id(void)
{ return sizeof(numintIl_t)/4; }

static inline size_t numintIl_serialize(void* dst, numintIl_t src)
{
  num_store_words8(dst,src,sizeof(numintIl_t));
  return sizeof(numintIl_t);
}

static inline size_t numintIl_deserialize(numintIl_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numintIl_t));
  return sizeof(numintIl_t);
}

static inline size_t numintIl_serialized_size(numintIl_t a)
{ return sizeof(numintIl_t); }

#ifdef __cplusplus
}
#endif

#endif
