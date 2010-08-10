/* ********************************************************************** */
/* numRyyy.h */
/* ********************************************************************** */

#ifndef _NUMRYYY_H_
#define _NUMRYYY_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "numTypes.h"
#include "numIyyy.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numRyyy_canonicalize(numRyyy_t r)
{
  if (r->d){
    numIyyy_t pgcd;
    numIyyy_gcd(pgcd,r->n,r->d);
    if (*pgcd==0 || (*pgcd==-1 && (*r->d==NUMIYYY_MIN || *r->n==NUMIYYY_MIN))) {
      fprintf(stderr,"overflow in numRyyy_canonicalize\n");
      return;
    }
    *r->n /= *pgcd;
    *r->d /= *pgcd;
  }
  else {
    numIyyy_set_int(r->n,1);
  }
}
static inline bool numRyyy_set_numIyyy2(numRyyy_t a, numIyyy_t b, numIyyy_t c)
{
  numIyyy_set(numRyyy_numref(a),b);
  numIyyy_set(numRyyy_denref(a),c);
  numRyyy_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numRyyy_set(numRyyy_t a, numRyyy_t b)
{ *a = *b; }
static inline void numRyyy_set_array(numRyyy_t* a, numRyyy_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numRyyy_t)); }
static inline void numRyyy_set_int(numRyyy_t a, long int i)
{
  numIyyy_set_int(a->n,i);
  numIyyy_set_int(a->d,1);
}

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numRyyy_init(numRyyy_t a)
{
  numIyyy_init(a->n);
  numIyyy_init_set_int(a->d,1);
}
static inline void numRyyy_init_array(numRyyy_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numRyyy_init(a[i]);
}
static inline void numRyyy_init_set(numRyyy_t a, numRyyy_t b)
{ *a = *b; }
static inline void numRyyy_init_set_int(numRyyy_t a, long int i)
{ numIyyy_set_int(a->n,i); numIyyy_set_int(a->d,1); }

static inline void numRyyy_clear(numRyyy_t a)
{}
static inline void numRyyy_clear_array(numRyyy_t* a, size_t size)
{}
static inline void numRyyy_swap(numRyyy_t a, numRyyy_t b)
{ numRyyy_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numRyyy_neg(numRyyy_t a, numRyyy_t b)
{ numIyyy_neg(a->n,b->n); numIyyy_set(a->d,b->d); }
static inline void numRyyy_inv(numRyyy_t a, numRyyy_t b)
{
  if (a!=b)
    *a = *b;
  numIyyy_swap(a->n,a->d);
  if (*a->d<0){
    numIyyy_neg(a->n,a->n);
    numIyyy_neg(a->d,a->d);
  }
}
static inline void numRyyy_abs(numRyyy_t a, numRyyy_t b)
{ numIyyy_abs(a->n,b->n); numIyyy_set(a->d,b->d); }
static inline void numRyyy_add(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d + *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIyyy_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) + (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRyyy_canonicalize(a);
}
static inline void numRyyy_add_uint(numRyyy_t a, numRyyy_t b, unsigned long int c)
{
  *a->n = *b->n + (numIyyy_native)c * (*b->d);
  *a->d = *b->d;
  numRyyy_canonicalize(a);
}
static inline void numRyyy_sub(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d - *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIyyy_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) - (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRyyy_canonicalize(a);
}
static inline void numRyyy_sub_uint(numRyyy_t a, numRyyy_t b, unsigned long int c)
{
  *a->n = *b->n - (numIyyy_native)c * (*b->d);
  *a->d = *b->d;
  numRyyy_canonicalize(a);
}
static inline void numRyyy_mul(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  *a->n = *b->n * *c->n;
  *a->d = *b->d * *c->d;
  numRyyy_canonicalize(a);
}
static inline void numRyyy_mul_2(numRyyy_t a, numRyyy_t b)
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
static inline void numRyyy_div(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
  *d = *b->d * *c->n;
  if (*d<0) {
    *a->n = - *b->n * *c->d;
    *a->d = - *d;
  }
  else {
    *a->n = *b->n * *c->d;
    *a->d = *d;
  }
  numRyyy_canonicalize(a);
}
static inline void numRyyy_div_2(numRyyy_t a, numRyyy_t b)
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
static inline void numRyyy_floor(numRyyy_t a, numRyyy_t b)
{
  numIyyy_fdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
static inline void numRyyy_ceil(numRyyy_t a, numRyyy_t b)
{
  numIyyy_cdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
static inline void numRyyy_trunc(numRyyy_t a, numRyyy_t b)
{
  numIyyy_tdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
static inline void numRyyy_sqrt(numRyyy_t up, numRyyy_t down, numRyyy_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  numIyyy_t pq;
  assert(*b->n>=0);
  numIyyy_mul(pq, numRyyy_numref(b), numRyyy_denref(b));
  numIyyy_sqrt(numRyyy_numref(up), numRyyy_numref(down), pq);
  numIyyy_set(numRyyy_denref(up),numRyyy_denref(b));
  numIyyy_set(numRyyy_denref(down),numRyyy_denref(b));
  numRyyy_canonicalize(up);
  numRyyy_canonicalize(down);
}

static inline void numRyyy_mul_2exp(numRyyy_t a, numRyyy_t b, int c)
{
  if (c>=0) {
    *a->n = *b->n << c;
    *a->d = *b->d;
  }
  else {
    *a->n = *b->n;
    *a->d = *b->d << (-c);
  }
  numRyyy_canonicalize(a);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numRyyy_sgn(numRyyy_t a)
{ return numIyyy_sgn(a->n); }
static inline int numRyyy_cmp(numRyyy_t a, numRyyy_t b)
{
  numIyyy_t aa,bb;
#if 0
  *aa = *a->n * *b->d;
  *bb = *a->d * *b->n;
#else /* more cost but less overflows */
  numIyyy_t d;
  numIyyy_lcm(d,a->d,b->d);
  *aa = *a->n * (*d / *a->d);
  *bb = (*d / *b->d) * *b->n;
#endif
  return numIyyy_cmp(aa,bb);
}
static inline int numRyyy_cmp_int(numRyyy_t a, long int b)
{
  numIyyy_t aa,bb;
  *aa = *a->n;
  *bb = *a->d * b;
  return numIyyy_cmp(aa,bb);
}
static inline bool numRyyy_equal(numRyyy_t a, numRyyy_t b)
{ return numIyyy_equal(a->n,b->n) && numIyyy_equal(a->d,b->d); }

static inline void numRyyy_min(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{ numRyyy_set(a, numRyyy_cmp(b,c)<=0 ? b : c); }
static inline void numRyyy_max(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{ numRyyy_set(a, numRyyy_cmp(b,c)>=0 ? b : c); }

static inline bool numRyyy_integer(numRyyy_t a)
{ return (*a->d==1); }
static inline int numRyyy_hash(numRyyy_t a)
{ return (int)((*a->n)/(*a->d)); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numRyyy_fprint(FILE* stream, numRyyy_t a)
{
  if (*a->n==0)
    fprintf(stream,"0");
  else if (*a->d==1)
    numIyyy_fprint(stream,a->n);
  else {
    numIyyy_fprint(stream,a->n);
    fprintf(stream,"/");
    numIyyy_fprint(stream,a->d);
  }
}
static inline void numRyyy_print(numRyyy_t a)
{
  numRyyy_fprint(stdout,a);
}
static inline int numRyyy_snprint(char* s, size_t size, numRyyy_t a)
{
  int res;
  if (*a->n==0)
    res = snprintf(s,size, "0");
  else if (*a->d==1)
    res = numIyyy_snprint(s,size,a->n);
  else {
    res = numIyyy_snprint(s,size,a->n);
    res += snprintf(s+res,size-res,"/");
    res += numIyyy_snprint(s+res,size-res,a->n);
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numRyyy_serialize_id(void)
{ return 0x10 + sizeof(numIyyy_t)/4; }

static inline size_t numRyyy_serialize(void* dst, numRyyy_t src)
{
  size_t x = numIyyy_serialize(dst,numRyyy_numref(src));
  return x + numIyyy_serialize((char*)dst+x,numRyyy_denref(src));
}

static inline size_t numRyyy_deserialize(numRyyy_t dst, const void* src)
{
  size_t x = numIyyy_deserialize(numRyyy_numref(dst),src);
  return x + numIyyy_deserialize(numRyyy_denref(dst),(const char*)src+x);
}

static inline size_t numRyyy_serialized_size(numRyyy_t a)
{
  return numIyyy_serialized_size(numRyyy_numref(a)) +
	 numIyyy_serialized_size(numRyyy_denref(a));
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numRyyy(long int a)
{ return lint_fits_numIyyy(a); }
static inline bool llint_fits_numRyyy(long long int a)
{ return llint_fits_numIyyy(a); }
static inline bool mpz_fits_numRyyy(mpz_t a)
{ return mpz_fits_numIyyy(a); }
static inline bool lfrac_fits_numRyyy(long int i, long int j)
{ return lint_fits_numIyyy(i) && lint_fits_numIyyy(j); }
static inline bool llfrac_fits_numRyyy(long long int i, long long int j)
{ return llint_fits_numIyyy(i) && llint_fits_numIyyy(j); }
static inline bool mpq_fits_numRyyy(mpq_t a)
{ return mpz_fits_numIyyy(mpq_numref(a)) && mpz_fits_numIyyy(mpq_denref(a)); }
static inline bool double_fits_numRyyy(double k)
{
  double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexp(k,&e);
  return e < (int)sizeof(numIyyy_t)*8-1;
}
static inline bool ldouble_fits_numRyyy(long double k)
{
  long double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexpl(k,&e);
  return e < (int)sizeof(numIyyy_t)*8-1;
}
static inline bool mpfr_fits_numRyyy(mpfr_t a, numinternal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(a)) return false;
  if (mpfr_sgn(a)==0) return true;
  e = mpfr_get_exp(a);
  return e < (mp_exp_t)sizeof(numIyyy_t)*8-1;
}
static inline bool numRyyy_fits_lint(numRyyy_t a)
{
  numIyyy_t b;
  numIyyy_cdiv_q(b,a->n,a->d);
  return *b<=LONG_MAX && *b>=-LONG_MAX;
}
static inline bool numRyyy_fits_llint(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_lfrac(numRyyy_t a)
{ return numIyyy_fits_lint(numRyyy_numref(a)) && numIyyy_fits_lint(numRyyy_denref(a)); }
static inline bool numRyyy_fits_llfrac(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_mpq(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_float(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_double(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_ldouble(numRyyy_t a)
{ return true; }
static inline bool numRyyy_fits_mpfr(numRyyy_t a)
{ return numIyyy_fits_mpfr(numRyyy_numref(a)) && numIyyy_fits_mpfr(numRyyy_denref(a)); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numRyyy_set_lint(numRyyy_t a, long int b, numinternal_t intern)
{
  numIyyy_set_lint(a->n,b,intern);
  numIyyy_set_int(a->d,1L);
  return true;
}
static inline bool numRyyy_set_llint(numRyyy_t a, long long int b, numinternal_t intern)
{
  numIyyy_set_llint(a->n,b,intern);
  numIyyy_set_int(a->d,1);
  return true;
}
static inline bool numRyyy_set_mpz(numRyyy_t a, mpz_t b, numinternal_t intern)
{
  numIyyy_set_mpz(a->n,b,intern);
  numIyyy_set_int(a->d,1);
  return true;
}
static inline bool numRyyy_set_lfrac(numRyyy_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numIyyy_set_lint(a->n,i,intern);
  numIyyy_set_lint(a->d,j,intern);
  numRyyy_canonicalize(a);
  return true;
}
static inline bool numRyyy_set_llfrac(numRyyy_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numIyyy_set_llint(a->n,i,intern);
  numIyyy_set_llint(a->d,j,intern);
  numRyyy_canonicalize(a);
  return true;
}
static inline bool numRyyy_set_mpq(numRyyy_t a, mpq_t b, numinternal_t intern)
{
  numIyyy_set_mpz(a->n,mpq_numref(b),intern);
  numIyyy_set_mpz(a->d,mpq_denref(b),intern);
  numRyyy_canonicalize(a);
  return true;
}
static inline bool numRyyy_set_double(numRyyy_t a, double k, numinternal_t intern)
{
  const int size = sizeof(numIyyy_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numRyyy_set_int(a,0); return false; }
  if (k==0.0){
    numRyyy_set_int(a,0);
    return true;
  }
  k = frexp(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMIYYY_ONE;
      *a->d = NUMIYYY_MAX;
    }
    else {
      numRyyy_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexp(k,l);
    res = numIyyy_set_double(a->n,k,intern);
    *a->d = NUMIYYY_ONE << (l-e);
    numRyyy_canonicalize(a);
    return res;
  }
  else {
    numRyyy_set_int(a,0);
    return false;
  }
}
static inline bool numRyyy_set_ldouble(numRyyy_t a, long double k, numinternal_t intern)
{
  const int size = sizeof(numIyyy_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numRyyy_set_int(a,0); return false; }
  if (k==0.0){
    numRyyy_set_int(a,0);
    return true;
  }
  k = frexpl(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMIYYY_ONE;
      *a->d = NUMIYYY_MAX;
    }
    else {
      numRyyy_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexpl(k,l);
    res = numIyyy_set_ldouble(a->n,k,intern);
    *a->d = NUMIYYY_ONE << (l-e);
    numRyyy_canonicalize(a);
    return res;
  }
  else {
    numRyyy_set_int(a,0);
    return false;
  }
}
static inline bool numRyyy_set_mpfr(numRyyy_t a, mpfr_t b, numinternal_t intern)
{
  long double k = mpfr_get_ld(b, GMP_RNDU);
  return numRyyy_set_ldouble(a,k,intern) && (mpfr_cmp_ld(b,k)==0);
}
static inline bool lint_set_numRyyy(long int* a, numRyyy_t b, numinternal_t intern)
{
  numIyyy_t q,r;
  numIyyy_cdiv_qr(q,r,numRyyy_numref(b),numRyyy_denref(b));
  return lint_set_numIyyy(a,q,intern) && (numIyyy_sgn(r)==0);
}
static inline bool llint_set_numRyyy(long long int* a, numRyyy_t b, numinternal_t intern)
{
  numIyyy_t q,r;
  numIyyy_cdiv_qr(q,r,numRyyy_numref(b),numRyyy_denref(b));
  return llint_set_numIyyy(a,q,intern) && (numIyyy_sgn(r)==0);
}
static inline bool mpz_set_numRyyy(mpz_t a, numRyyy_t b, numinternal_t intern)
{
  numIyyy_t q,r;
  numIyyy_cdiv_qr(q,r,numRyyy_numref(b),numRyyy_denref(b));
  mpz_set_numIyyy(a,q,intern);
  return (numIyyy_sgn(r)==0);
}
static inline bool lfrac_set_numRyyy(long int* i, long int* j, numRyyy_t b, numinternal_t intern)
{
  lint_set_numIyyy(i,numRyyy_numref(b),intern);
  lint_set_numIyyy(j,numRyyy_denref(b),intern);
  return true;
}
static inline bool llfrac_set_numRyyy(long long int* i, long long int* j, numRyyy_t b, numinternal_t intern)
{
  llint_set_numIyyy(i,numRyyy_numref(b),intern);
  llint_set_numIyyy(j,numRyyy_denref(b),intern);
  return true;
}
static inline bool mpq_set_numRyyy(mpq_t a, numRyyy_t b, numinternal_t intern)
{
  mpz_set_numIyyy(mpq_numref(a), numRyyy_numref(b),intern);
  mpz_set_numIyyy(mpq_denref(a), numRyyy_denref(b),intern);
  return true;
}
static inline bool double_set_numRyyy(double* a, numRyyy_t b, numinternal_t intern)
{
  *a = (double)(*b->n)/(double)(*b->d);
  return (-*a==(double)(-*b->n)/(double)(*b->d));
}
static inline bool ldouble_set_numRyyy(long double* a, numRyyy_t b, numinternal_t intern)
{
  *a = (long double)(*b->n)/(long double)(*b->d);
  return (-*a==(long double)(-*b->n)/(long double)(*b->d));
}
static inline bool mpfr_set_numRyyy(mpfr_t a, numRyyy_t b, numinternal_t intern)
{
  int r = mpfr_set_si(a,*numRyyy_numref(b),GMP_RNDU);
  return !mpfr_div_si(a,a,*numRyyy_denref(b),GMP_RNDU) && !r;
}

#ifdef __cplusplus
}
#endif

#endif
