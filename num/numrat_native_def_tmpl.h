/* ********************************************************************** */
/* numrat_native_def_tmpl.h */
/* ********************************************************************** */

#ifndef _NUMRAT_DEF_H_
#define _NUMRAT_DEF_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"

#include "numConfig.h"
#include "numint_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct numrat_native {
  numint_t n; /* numerator */
  numint_t d; /* denominator, >=0 */
} numrat_t[1];

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numrat_canonicalize(numrat_t r)
{
  if (r->d){
    numint_t pgcd;
    numint_gcd(pgcd,r->n,r->d);
    if (*pgcd==0 || (*pgcd==-1 && (*r->d==NUMINT_MIN || *r->n==NUMINT_MIN))) {
      fprintf(stderr,"overflow in numrat_canonicalize\n");
      return;
    }
    *r->n /= *pgcd;
    *r->d /= *pgcd;
  }
  else {
    numint_set_int(r->n,1);
  }
}
#define numrat_numref(a) (a)->n
#define numrat_denref(a) (a)->d

static inline bool numrat_set_numfrac(numrat_t a, numint_t b, numint_t c)
{
  numint_set(numrat_numref(a),b);
  numint_set(numrat_denref(a),c);
  numrat_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numrat_set(numrat_t a, numrat_t b)
{ *a = *b; }
static inline void numrat_set_array(numrat_t* a, numrat_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numrat_t)); }
static inline void numrat_set_int(numrat_t a, long int i)
{
  numint_set_int(a->n,i);
  numint_set_int(a->d,1);
}

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numrat_init(numrat_t a)
{
  numint_init(a->n);
  numint_init_set_int(a->d,1);
}
static inline void numrat_init_array(numrat_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numrat_init(a[i]);
}
static inline void numrat_init_set(numrat_t a, numrat_t b)
{ *a = *b; }
static inline void numrat_init_set_int(numrat_t a, long int i)
{ numint_set_int(a->n,i); numint_set_int(a->d,1); }

static inline void numrat_clear(numrat_t a)
{}
static inline void numrat_clear_array(numrat_t* a, size_t size)
{}
static inline void numrat_swap(numrat_t a, numrat_t b)
{ numrat_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numrat_neg(numrat_t a, numrat_t b)
{ numint_neg(a->n,b->n); numint_set(a->d,b->d); }
static inline void numrat_inv(numrat_t a, numrat_t b)
{
  if (a!=b)
    *a = *b;
  numint_swap(a->n,a->d);
  if (*a->d<0){
    numint_neg(a->n,a->n);
    numint_neg(a->d,a->d);
  }
}
static inline void numrat_abs(numrat_t a, numrat_t b)
{ numint_abs(a->n,b->n); numint_set(a->d,b->d); }
static inline void numrat_add(numrat_t a, numrat_t b, numrat_t c)
{
  numint_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d + *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numint_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) + (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numrat_canonicalize(a);
}
static inline void numrat_add_uint(numrat_t a, numrat_t b, unsigned long int c)
{
  *a->n = *b->n + (numint_native)c * (*b->d);
  *a->d = *b->d;
  numrat_canonicalize(a);
}
static inline void numrat_sub(numrat_t a, numrat_t b, numrat_t c)
{
  numint_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d - *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numint_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) - (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numrat_canonicalize(a);
}
static inline void numrat_sub_uint(numrat_t a, numrat_t b, unsigned long int c)
{
  *a->n = *b->n - (numint_native)c * (*b->d);
  *a->d = *b->d;
  numrat_canonicalize(a);
}
static inline void numrat_mul(numrat_t a, numrat_t b, numrat_t c)
{
  *a->n = *b->n * *c->n;
  *a->d = *b->d * *c->d;
  numrat_canonicalize(a);
}
static inline void numrat_mul_2(numrat_t a, numrat_t b)
{
  if (*b->d % 2 == 0){
    *a->n = *b->n;
    *a->d = *b->d / 2;
  }
  else {
    *a->n = *b->n * 2;
    *a->d = *b->d;
  }
}
static inline void numrat_div(numrat_t a, numrat_t b, numrat_t c)
{
  numint_t d;
  *d = *b->d * *c->n;
  if (*d<0) {
    *a->n = - *b->n * *c->d;
    *a->d = - *d;
  }
  else {
    *a->n = *b->n * *c->d;
    *a->d = *d;
  }
  numrat_canonicalize(a);
}
static inline void numrat_div_2(numrat_t a, numrat_t b)
{
  if (*b->n % 2 == 0){
    *a->n = *b->n / 2;
    *a->d = *b->d;
  }
  else {
    *a->n = *b->n;
    *a->d = *b->d * 2;
  }
}
static inline void numrat_floor(numrat_t a, numrat_t b)
{
  numint_fdiv_q(a->n, b->n, b->d);
  numint_set_int(a->d,1);
}
static inline void numrat_ceil(numrat_t a, numrat_t b)
{
  numint_cdiv_q(a->n, b->n, b->d);
  numint_set_int(a->d,1);
}
static inline void numrat_trunc(numrat_t a, numrat_t b)
{
  numint_tdiv_q(a->n, b->n, b->d);
  numint_set_int(a->d,1);
}
static inline void numrat_sqrt(numrat_t up, numrat_t down, numrat_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  numint_t pq;
  assert(*b->n>=0);
  numint_mul(pq, numrat_numref(b), numrat_denref(b));
  numint_sqrt(numrat_numref(up), numrat_numref(down), pq);
  numint_set(numrat_denref(up),numrat_denref(b));
  numint_set(numrat_denref(down),numrat_denref(b));
  numrat_canonicalize(up);
  numrat_canonicalize(down);
}

static inline void numrat_mul_2exp(numrat_t a, numrat_t b, int c)
{
  if (c>=0) {
    *a->n = *b->n << c;
    *a->d = *b->d;
  }
  else {
    *a->n = *b->n;
    *a->d = *b->d << (-c);
  }
  numrat_canonicalize(a);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numrat_sgn(numrat_t a)
{ return numint_sgn(a->n); }
static inline int numrat_cmp(numrat_t a, numrat_t b)
{
  numint_t aa,bb;
#if 0
  *aa = *a->n * *b->d;
  *bb = *a->d * *b->n;
#else /* more cost but less overflows */
  numint_t d;
  numint_lcm(d,a->d,b->d);
  *aa = *a->n * (*d / *a->d);
  *bb = (*d / *b->d) * *b->n;
#endif
  return numint_cmp(aa,bb);
}
static inline int numrat_cmp_int(numrat_t a, long int b)
{
  numint_t aa,bb;
  *aa = *a->n;
  *bb = *a->d * b;
  return numint_cmp(aa,bb);
}
static inline bool numrat_equal(numrat_t a, numrat_t b)
{ return numint_equal(a->n,b->n) && numint_equal(a->d,b->d); }

static inline void numrat_min(numrat_t a, numrat_t b, numrat_t c)
{ numrat_set(a, numrat_cmp(b,c)<=0 ? b : c); }
static inline void numrat_max(numrat_t a, numrat_t b, numrat_t c)
{ numrat_set(a, numrat_cmp(b,c)>=0 ? b : c); }

static inline bool numrat_integer(numrat_t a)
{ return (*a->d==1); }
static inline int numrat_hash(numrat_t a)
{ return (int)((*a->n)/(*a->d)); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numrat_fprint(FILE* stream, numrat_t a)
{
  if (*a->n==0)
    fprintf(stream,"0");
  else if (*a->d==1)
    numint_fprint(stream,a->n);
  else {
    numint_fprint(stream,a->n);
    fprintf(stream,"/");
    numint_fprint(stream,a->d);
  }
}
static inline void numrat_print(numrat_t a)
{
  numrat_fprint(stdout,a);
}
static inline int numrat_snprint(char* s, size_t size, numrat_t a)
{
  int res;
  if (*a->n==0)
    res = snprintf(s,size, "0");
  else if (*a->d==1)
    res = numint_snprint(s,size,a->n);
  else {
    res = numint_snprint(s,size,a->n);
    res += snprintf(s+res,size-res,"/");
    res += numint_snprint(s+res,size-res,a->n);
  }
  return res;
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numrat(long int a)
{ return lint_fits_numint(a); }
static inline bool llint_fits_numrat(long long int a)
{ return llint_fits_numint(a); }
static inline bool mpz_fits_numrat(mpz_t a)
{ return mpz_fits_numint(a); }
static inline bool lfrac_fits_numrat(long int i, long int j)
{ return lint_fits_numint(i) && lint_fits_numint(j); }
static inline bool llfrac_fits_numrat(long long int i, long long int j)
{ return llint_fits_numint(i) && llint_fits_numint(j); }
static inline bool mpq_fits_numrat(mpq_t a)
{ return mpz_fits_numint(mpq_numref(a)) && mpz_fits_numint(mpq_denref(a)); }
static inline bool double_fits_numrat(double k)
{
  double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexp(k,&e);
  return e < (int)sizeof(numint_t)*8-1;
}
static inline bool ldouble_fits_numrat(long double k)
{
  long double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexpl(k,&e);
  return e < (int)sizeof(numint_t)*8-1;
}
static inline bool mpfr_fits_numrat(mpfr_t a, numinternal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(a)) return false;
  if (mpfr_sgn(a)==0) return true;
  e = mpfr_get_exp(a);
  return e < (mp_exp_t)sizeof(numint_t)*8-1;
}
static inline bool numrat_fits_lint(numrat_t a)
{
  numint_t b;
  numint_cdiv_q(b,a->n,a->d);
  return *b<=LONG_MAX && *b>=-LONG_MAX;
}
static inline bool numrat_fits_llint(numrat_t a)
{ return true; }
static inline bool numrat_fits_lfrac(numrat_t a)
{ return numint_fits_lint(numrat_numref(a)) && numint_fits_lint(numrat_denref(a)); }
static inline bool numrat_fits_llfrac(numrat_t a)
{ return true; }
static inline bool numrat_fits_mpq(numrat_t a)
{ return true; }
static inline bool numrat_fits_float(numrat_t a)
{ return true; }
static inline bool numrat_fits_double(numrat_t a)
{ return true; }
static inline bool numrat_fits_ldouble(numrat_t a)
{ return true; }
static inline bool numrat_fits_mpfr(numrat_t a)
{ return numint_fits_mpfr(numrat_numref(a)) && numint_fits_mpfr(numrat_denref(a)); }

/* lint -> numrat */
static inline bool numrat_set_lint(numrat_t a, long int b, numinternal_t intern)
{
  assert(j>0);
  numint_set_lint(a->n,b,intern);
  numint_set_int(a->d,1L);
  return true;
}
/* llint -> numrat */
static inline bool numrat_set_llint(numrat_t a, long long int b, numinternal_t intern)
{
  assert(j>0);
  numint_set_llint(a->n,b,intern);
  numint_set_int(a->d,1);
  return true;
}
/* mpz -> numrat */
static inline bool numrat_set_mpz(numrat_t a, mpz_t b, numinternal_t intern)
{
  numint_set_mpz(a->n,b,intern);
  numint_set_int(a->d,1);
  return true;
}
/* lfrac -> numrat */
static inline bool numrat_set_lfrac(numrat_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numint_set_lint(a->n,i,intern);
  numint_set_lint(a->d,j,intern);
  numrat_canonicalize(a);
  return true;
}
/* llfrac -> numrat */
static inline bool numrat_set_llfrac(numrat_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numint_set_llint(a->n,i,intern);
  numint_set_llint(a->d,j,intern);
  numrat_canonicalize(a);
  return true;
}
/* mpq -> numrat */
static inline bool numrat_set_mpq(numrat_t a, mpq_t b, numinternal_t intern)
{
  numint_set_mpz(a->n,mpq_numref(b),intern);
  numint_set_mpz(a->d,mpq_denref(b),intern);
  numrat_canonicalize(a);
  return true;
}
/* double -> numrat */
static inline bool numrat_set_double(numrat_t a, double k, numinternal_t intern)
{
  const int size = sizeof(numint_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numrat_set_int(a,0); return false; }
  if (k==0.0){
    numrat_set_int(a,0);
    return true;
  }
  k = frexp(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMINT_ONE;
      *a->d = NUMINT_MAX;
    }
    else {
      numrat_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexp(k,l);
    res = numint_set_double(a->n,k,intern);
    *a->d = NUMINT_ONE << (l-e);
    numrat_canonicalize(a);
    return res;
  }
  else {
    numrat_set_int(a,0);
    return false;
  }
}
/* ldouble -> numrat */
static inline bool numrat_set_ldouble(numrat_t a, long double k, numinternal_t intern)
{
  const int size = sizeof(numint_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numrat_set_int(a,0); return false; }
  if (k==0.0){
    numrat_set_int(a,0);
    return true;
  }
  k = frexpl(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMINT_ONE;
      *a->d = NUMINT_MAX;
    }
    else {
      numrat_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexpl(k,l);
    res = numint_set_ldouble(a->n,k,intern);
    *a->d = NUMINT_ONE << (l-e);
    numrat_canonicalize(a);
    return res;
  }
  else {
    numrat_set_int(a,0);
    return false;
  }
}
/* mpfr -> numrat */
static inline bool numrat_set_mpfr(numrat_t a, mpfr_t b, numinternal_t intern)
{
  long double k = mpfr_get_ld(b, GMP_RNDU);
  return numrat_set_ldouble(a,k,intern) && (mpfr_cmp_ld(b,k)==0);
}

/* numrat -> lint */
static inline bool lint_set_numrat(long int* a, numrat_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  return lint_set_numint(a,q,intern) && (numint_sgn(r)==0);
}
/* numrat -> llint */
static inline bool llint_set_numrat(long long int* a, numrat_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  return llint_set_numint(a,q,intern) && (numint_sgn(r)==0);
}
/* numrat -> mpz */
static inline bool mpz_set_numrat(mpz_t a, numrat_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  mpz_set_numint(a,q,intern);
  return (numint_sgn(r)==0);
}
/* numrat -> lfrac */
static inline bool lfrac_set_numrat(long int* i, long int* j, numrat_t b, numinternal_t intern)
{
  lint_set_numint(i,numrat_numref(b),intern);
  lint_set_numint(j,numrat_denref(b),intern);
  return true;
}
/* numrat -> llfrac */
static inline bool llfrac_set_numrat(long long int* i, long long int* j, numrat_t b, numinternal_t intern)
{
  llint_set_numint(i,numrat_numref(b),intern);
  llint_set_numint(j,numrat_denref(b),intern);
  return true;
}
/* numrat -> mpq */
static inline bool mpq_set_numrat(mpq_t a, numrat_t b, numinternal_t intern)
{
  mpz_set_numint(mpq_numref(a), numrat_numref(b),intern);
  mpz_set_numint(mpq_denref(a), numrat_denref(b),intern);
  return true;
}
/* numrat -> double */
static inline bool double_set_numrat(double* a, numrat_t b, numinternal_t intern)
{
  *a = (double)(*b->n)/(double)(*b->d);
  return (-*a==(double)(-*b->n)/(double)(*b->d));
}
/* numrat -> ldouble */
static inline bool ldouble_set_numrat(long double* a, numrat_t b, numinternal_t intern)
{
  *a = (long double)(*b->n)/(long double)(*b->d);
  return (-*a==(long double)(-*b->n)/(long double)(*b->d));
}
/* numrat -> mpfr */
static inline bool mpfr_set_numrat(mpfr_t a, numrat_t b, numinternal_t intern)
{
  int r = mpfr_set_si(a,*numrat_numref(b),GMP_RNDU);
  return !mpfr_div_si(a,a,*numrat_denref(b),GMP_RNDU) && !r;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numrat_serialize_id(void)
{ return 0x10 + sizeof(numint_t)/4; }

static inline size_t numrat_serialize(void* dst, numrat_t src)
{
  size_t x = numint_serialize(dst,numrat_numref(src));
  return x + numint_serialize((char*)dst+x,numrat_denref(src));
}

static inline size_t numrat_deserialize(numrat_t dst, const void* src)
{
  size_t x = numint_deserialize(numrat_numref(dst),src);
  return x + numint_deserialize(numrat_denref(dst),(const char*)src+x);
}

static inline size_t numrat_serialized_size(numrat_t a)
{
  return numint_serialized_size(numrat_numref(a)) +
	 numint_serialized_size(numrat_denref(a));
}

#ifdef __cplusplus
}
#endif

#endif
