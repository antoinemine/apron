/* ********************************************************************** */
/* numint_native.h */
/* ********************************************************************** */

#ifndef _NUMINT_NATIVE_H_
#define _NUMINT_NATIVE_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"
#include "ap_scalar.h"

#include "num_config.h"
/* Require C99 compliant compiler */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NUMINT_LONGINT)
typedef long int numint_native;
#define NUMINT_ZERO 0L
#define NUMINT_ONE 1L
#define NUMINT_MAX LONG_MAX
#define NUMINT_MIN LONG_MIN

#elif defined(NUMINT_LONGLONGINT)
typedef long long int numint_native;
#define NUMINT_ZERO 0LL
#define NUMINT_ONE 1LL
#define NUMINT_MAX LLONG_MAX
#define NUMINT_MIN LLONG_MIN

#else
#error "HERE"
#endif

typedef numint_native numint_t[1];

#define NUMINT_NATIVE
#
/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numint_set(numint_t a, numint_t b)
{ *a = *b; }
static inline void numint_set_array(numint_t* a, numint_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numint_t)); }
static inline void numint_set_int(numint_t a, long int i)
{ *a = (numint_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numint_init(numint_t a)
{ *a = NUMINT_ZERO; }
static inline void numint_init_array(numint_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMINT_ZERO;
}
static inline void numint_init_set(numint_t a, numint_t b)
{ numint_set(a,b); }
static inline void numint_init_set_int(numint_t a, long int i)
{ numint_set_int(a,i); }

static inline void numint_clear(numint_t a)
{ (void)a; }
static inline void numint_clear_array(numint_t* a, size_t size)
{ (void)a; (void)size; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numint_neg(numint_t a, numint_t b)
{ *a = -(*b); }
static inline void numint_abs(numint_t a, numint_t b)
#if defined(NUMINT_LONGINT)
{ *a = labs(*b); }
#else
{ *a = llabs(*b); }
#endif
static inline void numint_add(numint_t a, numint_t b, numint_t c)
{ *a = *b + *c; }
static inline void numint_add_uint(numint_t a, numint_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numint_sub(numint_t a, numint_t b, numint_t c)
{ *a = *b - *c; }
static inline void numint_sub_uint(numint_t a, numint_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numint_mul(numint_t a, numint_t b, numint_t c)
{ *a = *b * *c; }
static inline void numint_mul_2(numint_t a, numint_t b)
{ *a = *b << 1; }

static inline void numint_tdiv_q(numint_t q, numint_t a, numint_t b)
{ *q = *a / *b; }

static inline int numint_sgn(numint_t a)
{ return (*a==NUMINT_ZERO ? 0 : (*a>NUMINT_ZERO ? 1 : -1)); }

static inline void numint_fdiv_q(numint_t q, numint_t a, numint_t b)
{
  if (numint_sgn(a)*numint_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numint_cdiv_q(numint_t q, numint_t a, numint_t b)
{
  if (numint_sgn(a)*numint_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}
static inline void numint_cdiv_qr(numint_t q, numint_t r, numint_t a, numint_t b)
{
  numint_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numint_sgn(a)*numint_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

static inline void numint_cdiv_2(numint_t a, numint_t b)
{ *a = (*b>=NUMINT_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numint_cdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMINT_ONE<<c)-NUMINT_ONE) ? 1 : 0); }
static inline void numint_fdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numint_tdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ if (*b>=0) numint_fdiv_q_2exp(a,b,c); else numint_cdiv_q_2exp(a,b,c); }
static inline void numint_min(numint_t a, numint_t b, numint_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numint_max(numint_t a, numint_t b, numint_t c)
{ *a = (*b>=*c) ? *b : *c; }
static inline void numint_floor(numint_t a, numint_t b)
{ *a = *b; }
static inline void numint_ceil(numint_t a, numint_t b)
{ *a = *b; }

static const long long numint_max_exact_double = 1LL << 52;

static inline void numint_sqrt(numint_t up, numint_t down, numint_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numint_max_exact_double) {
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

static inline int numint_pow(numint_t up, numint_t down, numint_t b, unsigned long int n)
{
  numint_native r = 1;
  numint_native f = *b;
  while (n) {
    if (n & 1) r *= f;
    f *= f;
    if (f <= 0) return 1;
    n >>= 1;
  }
  *up = r;
  *down = *up;
  return 0;
}

static inline void numint_root(numint_t up, numint_t down, numint_t b, unsigned long int n)
{
  /* we rely on GMP to do the heavy lifting for us */
  mpz_t arg,res;
  int exact;
  long r;
  assert(n > 0);
  assert((n & 1) || (*b >= 0));
  mpz_init_set_si(arg, (*b >= 0) ? *b : -*b);
  mpz_init(res);
  exact = mpz_root(res, arg, n);
  r = mpz_get_ui(res);
  if (*b >= 0) {
    if (exact) *up = *down = r;
    else { *down = r; *up = r+1; }
  }
  else {
    if (exact) *up = *down = -r;
    else { *down = -r-1; *up = -r; }
  }
  mpz_clear(arg);
  mpz_clear(res);
}

static inline void numint_mul_2exp(numint_t a, numint_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numint_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numint_divexact(numint_t a, numint_t b, numint_t c)
{ *a = *b / *c; }
static inline void numint_mod(numint_t a, numint_t b, numint_t c)
{ *a = *b % *c; }
static inline numint_native _gcd_aux2(numint_native a, numint_native b)
{ /* a is supposed to be greater than b */
  numint_native t;
  while (b!=NUMINT_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline numint_native _gcd_aux(numint_native a, numint_native b)
{
  numint_abs(&a,&a);
  numint_abs(&b,&b);
  return (a>=b) ? _gcd_aux2(a,b) : _gcd_aux2(b,a);
}
static inline void numint_gcd(numint_t a, numint_t b,  numint_t c)
{ *a = _gcd_aux(*b,*c); }

static inline numint_native _lcm_aux(numint_native a, numint_native b)
{
  numint_abs(&a,&a);
  numint_abs(&b,&b);
  return a / _gcd_aux(a,b) * b;
}
static inline void numint_lcm(numint_t a, numint_t b,  numint_t c)
{ *a = _lcm_aux(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numint_cmp(numint_t a, numint_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numint_cmp_int(numint_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numint_equal(numint_t a, numint_t b)
{ return *a==*b; }
static inline bool numint_integer(numint_t a)
{ (void)a; return true; }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

#if defined(NUMINT_LONGINT)
static inline void numint_print(numint_t a)
{ printf("%li",*a); }
static inline void numint_fprint(FILE* stream, numint_t a)
{ fprintf(stream,"%li",*a); }
static inline int numint_snprint(char* s, size_t size, numint_t a)
{ return snprintf(s,size,"%li",*a); }
#else
static inline void numint_print(numint_t a)
{ printf("%lli",*a); }
static inline void numint_fprint(FILE* stream, numint_t a)
{ fprintf(stream,"%lli",*a); }
static inline int numint_snprint(char* s, size_t size, numint_t a)
{ return snprintf(s,size,"%lli",*a); }
#endif

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* int2 -> numint */
static inline bool numint_set_int2(numint_t a, long int i, long int j)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j) ? false : true;
}

/* mpz -> numint */
static inline bool numint_set_mpz(numint_t a, mpz_t b)
{
#if NUMINT_MAX == LONG_MAX
  *a = mpz_get_si(b);
#else
  int sgn;
  size_t count;
  unsigned long int tab[2];

  assert(sizeof(numint_t)==2*sizeof(long int));
    
  sgn = mpz_sgn(b);
  mpz_export(&tab,&count,1,sizeof(long int),0,0,b);
  if (count==0){
    *a = 0;
  }
  else {
    *a = tab[0];
    if (count==2){
      *a = *a << (sizeof(long int)*8);
      *a = *a + (long long int)(tab[1]);
      if (*a<0){
        assert(0);
      }
    }
    if (sgn<0) *a = -(*a);
  }
#endif
  return true;
}

/* mpq -> numint */
static inline bool numint_set_mpq_tmp(numint_t a, mpq_t b,
				      mpz_t q, mpz_t r)
{
  mpz_cdiv_qr(q,r, mpq_numref(b),mpq_denref(b));
  numint_set_mpz(a,q);
  bool res = (mpz_sgn(r)==0);
  return res;
}
static inline bool numint_set_mpq(numint_t a, mpq_t b)
{
  mpz_t q,r;
  mpz_init(q);mpz_init(r);
  bool res = numint_set_mpq_tmp(a,b,q,r);
  mpz_clear(q); mpz_clear(r);
  return res;
}
/* double -> numint */
static inline bool numint_set_double(numint_t a, double b)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
/* mpfr -> numint */
static inline bool numint_set_mpfr(numint_t a, mpfr_t b)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numint_set_int(a,0); return false; }
#if defined(NUMINT_LONGINT)
  *a = mpfr_get_si(b,GMP_RNDU);
#else
  *a = mpfr_get_sj(b,GMP_RNDU);
#endif
  return mpfr_integer_p(b);
}
/* numint -> int */
static inline bool int_set_numint(long int* a, numint_t b)
{ *a = (long int)(*b); return true; }

/* numint -> mpz */
static inline bool mpz_set_numint(mpz_t a, numint_t b)
{
#if NUMINT_MAX == LONG_MAX
  mpz_set_si(a,*b);
#else
  unsigned long long int n;
  unsigned long int rep[2];

  assert(sizeof(numint_t)==2*sizeof(long int));

  n = llabs(*b);
  rep[1] = n & ULONG_MAX;
  rep[0] = n >> (sizeof(long int)*8);
  mpz_import(a,2,1,sizeof(unsigned long int),0,0,rep);
  if (*b<0) {
    mpz_neg(a,a);
  }
#endif
  return true;
}

/* numint -> mpq */
static inline bool mpq_set_numint(mpq_t a, numint_t b)
{
#if NUMINT_MAX == LONG_MAX
  mpq_set_si(a,*b,1);
  return true;
#else
  mpz_set_ui(mpq_denref(a),1);
  return mpz_set_numint(mpq_numref(a),b);
#endif
}

/* numint -> double */
static inline bool double_set_numint(double* a, numint_t b)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}

/* numint -> mpfr */
static inline bool mpfr_set_numint(mpfr_t a, numint_t b)
{
#if defined(NUMINT_LONGINT)
  return !mpfr_set_si(a,*b,GMP_RNDU);
#else
  return !mpfr_set_sj(a,*b,GMP_RNDU);
#endif
}

static inline bool mpz_fits_numint(mpz_t a)
{
#if NUMINT_MAX == LONG_MAX
  return mpz_fits_slong_p(a);
#else
  size_t size = mpz_sizeinbase(a,2);
  return (size <= sizeof(numint_t)*8-1);
#endif
}

static inline bool mpq_fits_numint_tmp(mpq_t a, mpz_t mpz)
{
  mpz_cdiv_q(mpz,mpq_numref(a),mpq_denref(a));
  bool res = mpz_fits_numint(mpz);
  return res;
}
static inline bool mpq_fits_numint(mpq_t a)
{
  mpz_t mpz;
  mpz_init(mpz);
  bool res = mpq_fits_numint_tmp(a,mpz);
  mpz_clear(mpz);
  return res;
}
static inline bool double_fits_numint(double a)
{
  return isfinite(a) && a>=(double)(-NUMINT_MAX) && a<=(double)NUMINT_MAX;
}
static inline bool mpfr_fits_numint(mpfr_t a)
{
#if defined(NUMINT_LONGINT)
  return mpfr_number_p(a) && mpfr_fits_slong_p(a,GMP_RNDU);
#else
  return mpfr_number_p(a) && mpfr_fits_intmax_p(a,GMP_RNDU);
#endif
}
static inline bool numint_fits_int(numint_t a)
#if defined(NUMINT_LONGINT)
{ (void)a; return true; }
#else
{ return (*a>=-LONG_MAX && *a<=LONG_MAX); }
#endif
static inline bool numint_fits_float(numint_t a)
{ (void)a; return true; }
static inline bool numint_fits_double(numint_t a)
{ (void)a; return true; }
static inline bool numint_fits_mpfr(numint_t a)
{ (void)a; return true; }


/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numint_serialize_id(void)
{ return sizeof(numint_t)/4; }

static inline size_t numint_serialize(void* dst, numint_t src)
{
  num_store_words8(dst,src,sizeof(numint_t));
  return sizeof(numint_t);
}

static inline size_t numint_deserialize(numint_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numint_t));
  return sizeof(numint_t);
}

static inline size_t numint_serialized_size(numint_t a)
{ (void)a; return sizeof(numint_t); }

#ifdef __cplusplus
}
#endif

#endif
