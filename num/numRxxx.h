/* ********************************************************************** */
/* numRxxx.h */
/* ********************************************************************** */

#ifndef _NUMRXXX_H_
#define _NUMRXXX_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "numTypes.h"
#include "numIxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numRxxx_canonicalize(numRxxx_t r)
{
  if (r->d){
    numIxxx_t pgcd;
    numIxxx_gcd(pgcd,r->n,r->d);
    if (*pgcd==0 || (*pgcd==-1 && (*r->d==NUMIXXX_MIN || *r->n==NUMIXXX_MIN))) {
      fprintf(stderr,"overflow in numRxxx_canonicalize\n");
      return;
    }
    *r->n /= *pgcd;
    *r->d /= *pgcd;
  }
  else {
    numIxxx_set_int(r->n,1);
  }
}
static inline bool numRxxx_set_numIxxx2(numRxxx_t a, numIxxx_t b, numIxxx_t c)
{
  numIxxx_set(numRxxx_numref(a),b);
  numIxxx_set(numRxxx_denref(a),c);
  numRxxx_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numRxxx_set(numRxxx_t a, numRxxx_t b)
{ *a = *b; }
static inline void numRxxx_set_array(numRxxx_t* a, numRxxx_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numRxxx_t)); }
static inline void numRxxx_set_int(numRxxx_t a, long int i)
{
  numIxxx_set_int(a->n,i);
  numIxxx_set_int(a->d,1);
}

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numRxxx_init(numRxxx_t a)
{
  numIxxx_init(a->n);
  numIxxx_init_set_int(a->d,1);
}
static inline void numRxxx_init_array(numRxxx_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numRxxx_init(a[i]);
}
static inline void numRxxx_init_set(numRxxx_t a, numRxxx_t b)
{ *a = *b; }
static inline void numRxxx_init_set_int(numRxxx_t a, long int i)
{ numIxxx_set_int(a->n,i); numIxxx_set_int(a->d,1); }

static inline void numRxxx_clear(numRxxx_t a)
{}
static inline void numRxxx_clear_array(numRxxx_t* a, size_t size)
{}
static inline void numRxxx_swap(numRxxx_t a, numRxxx_t b)
{ numRxxx_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numRxxx_neg(numRxxx_t a, numRxxx_t b)
{ numIxxx_neg(a->n,b->n); numIxxx_set(a->d,b->d); }
static inline void numRxxx_inv(numRxxx_t a, numRxxx_t b)
{
  if (a!=b)
    *a = *b;
  numIxxx_swap(a->n,a->d);
  if (*a->d<0){
    numIxxx_neg(a->n,a->n);
    numIxxx_neg(a->d,a->d);
  }
}
static inline void numRxxx_abs(numRxxx_t a, numRxxx_t b)
{ numIxxx_abs(a->n,b->n); numIxxx_set(a->d,b->d); }
static inline void numRxxx_add(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{
  numIxxx_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d + *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIxxx_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) + (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRxxx_canonicalize(a);
}
static inline void numRxxx_add_uint(numRxxx_t a, numRxxx_t b, unsigned long int c)
{
  *a->n = *b->n + (numIxxx_native)c * (*b->d);
  *a->d = *b->d;
  numRxxx_canonicalize(a);
}
static inline void numRxxx_sub(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{
  numIxxx_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d - *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIxxx_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) - (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRxxx_canonicalize(a);
}
static inline void numRxxx_sub_uint(numRxxx_t a, numRxxx_t b, unsigned long int c)
{
  *a->n = *b->n - (numIxxx_native)c * (*b->d);
  *a->d = *b->d;
  numRxxx_canonicalize(a);
}
static inline void numRxxx_mul(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{
  *a->n = *b->n * *c->n;
  *a->d = *b->d * *c->d;
  numRxxx_canonicalize(a);
}
static inline void numRxxx_mul_2(numRxxx_t a, numRxxx_t b)
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
static inline void numRxxx_div(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{
  numIxxx_t d;
  *d = *b->d * *c->n;
  if (*d<0) {
    *a->n = - *b->n * *c->d;
    *a->d = - *d;
  }
  else {
    *a->n = *b->n * *c->d;
    *a->d = *d;
  }
  numRxxx_canonicalize(a);
}
static inline void numRxxx_div_2(numRxxx_t a, numRxxx_t b)
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
static inline void numRxxx_floor(numRxxx_t a, numRxxx_t b)
{
  numIxxx_fdiv_q(a->n, b->n, b->d);
  numIxxx_set_int(a->d,1);
}
static inline void numRxxx_ceil(numRxxx_t a, numRxxx_t b)
{
  numIxxx_cdiv_q(a->n, b->n, b->d);
  numIxxx_set_int(a->d,1);
}
static inline void numRxxx_trunc(numRxxx_t a, numRxxx_t b)
{
  numIxxx_tdiv_q(a->n, b->n, b->d);
  numIxxx_set_int(a->d,1);
}
static inline void numRxxx_sqrt(numRxxx_t up, numRxxx_t down, numRxxx_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  numIxxx_t pq;
  assert(*b->n>=0);
  numIxxx_mul(pq, numRxxx_numref(b), numRxxx_denref(b));
  numIxxx_sqrt(numRxxx_numref(up), numRxxx_numref(down), pq);
  numIxxx_set(numRxxx_denref(up),numRxxx_denref(b));
  numIxxx_set(numRxxx_denref(down),numRxxx_denref(b));
  numRxxx_canonicalize(up);
  numRxxx_canonicalize(down);
}

static inline void numRxxx_mul_2exp(numRxxx_t a, numRxxx_t b, int c)
{
  if (c>=0) {
    *a->n = *b->n << c;
    *a->d = *b->d;
  }
  else {
    *a->n = *b->n;
    *a->d = *b->d << (-c);
  }
  numRxxx_canonicalize(a);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numRxxx_sgn(numRxxx_t a)
{ return numIxxx_sgn(a->n); }
static inline int numRxxx_cmp(numRxxx_t a, numRxxx_t b)
{
  numIxxx_t aa,bb;
#if 0
  *aa = *a->n * *b->d;
  *bb = *a->d * *b->n;
#else /* more cost but less overflows */
  numIxxx_t d;
  numIxxx_lcm(d,a->d,b->d);
  *aa = *a->n * (*d / *a->d);
  *bb = (*d / *b->d) * *b->n;
#endif
  return numIxxx_cmp(aa,bb);
}
static inline int numRxxx_cmp_int(numRxxx_t a, long int b)
{
  numIxxx_t aa,bb;
  *aa = *a->n;
  *bb = *a->d * b;
  return numIxxx_cmp(aa,bb);
}
static inline bool numRxxx_equal(numRxxx_t a, numRxxx_t b)
{ return numIxxx_equal(a->n,b->n) && numIxxx_equal(a->d,b->d); }

static inline void numRxxx_min(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{ numRxxx_set(a, numRxxx_cmp(b,c)<=0 ? b : c); }
static inline void numRxxx_max(numRxxx_t a, numRxxx_t b, numRxxx_t c)
{ numRxxx_set(a, numRxxx_cmp(b,c)>=0 ? b : c); }

static inline bool numRxxx_integer(numRxxx_t a)
{ return (*a->d==1); }
static inline int numRxxx_hash(numRxxx_t a)
{ return (int)((*a->n)/(*a->d)); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numRxxx_fprint(FILE* stream, numRxxx_t a)
{
  if (*a->n==0)
    fprintf(stream,"0");
  else if (*a->d==1)
    numIxxx_fprint(stream,a->n);
  else {
    numIxxx_fprint(stream,a->n);
    fprintf(stream,"/");
    numIxxx_fprint(stream,a->d);
  }
}
static inline void numRxxx_print(numRxxx_t a)
{
  numRxxx_fprint(stdout,a);
}
static inline int numRxxx_snprint(char* s, size_t size, numRxxx_t a)
{
  int res;
  if (*a->n==0)
    res = snprintf(s,size, "0");
  else if (*a->d==1)
    res = numIxxx_snprint(s,size,a->n);
  else {
    res = numIxxx_snprint(s,size,a->n);
    res += snprintf(s+res,size-res,"/");
    res += numIxxx_snprint(s+res,size-res,a->n);
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numRxxx_serialize_id(void)
{ return 0x10 + sizeof(numIxxx_t)/4; }

static inline size_t numRxxx_serialize(void* dst, numRxxx_t src)
{
  size_t x = numIxxx_serialize(dst,numRxxx_numref(src));
  return x + numIxxx_serialize((char*)dst+x,numRxxx_denref(src));
}

static inline size_t numRxxx_deserialize(numRxxx_t dst, const void* src)
{
  size_t x = numIxxx_deserialize(numRxxx_numref(dst),src);
  return x + numIxxx_deserialize(numRxxx_denref(dst),(const char*)src+x);
}

static inline size_t numRxxx_serialized_size(numRxxx_t a)
{
  return numIxxx_serialized_size(numRxxx_numref(a)) +
	 numIxxx_serialized_size(numRxxx_denref(a));
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numRxxx(long int a)
{ return lint_fits_numIxxx(a); }
static inline bool llint_fits_numRxxx(long long int a)
{ return llint_fits_numIxxx(a); }
static inline bool mpz_fits_numRxxx(mpz_t a)
{ return mpz_fits_numIxxx(a); }
static inline bool lfrac_fits_numRxxx(long int i, long int j)
{ return lint_fits_numIxxx(i) && lint_fits_numIxxx(j); }
static inline bool llfrac_fits_numRxxx(long long int i, long long int j)
{ return llint_fits_numIxxx(i) && llint_fits_numIxxx(j); }
static inline bool mpq_fits_numRxxx(mpq_t a)
{ return mpz_fits_numIxxx(mpq_numref(a)) && mpz_fits_numIxxx(mpq_denref(a)); }
static inline bool double_fits_numRxxx(double k)
{
  double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexp(k,&e);
  return e < (int)sizeof(numIxxx_t)*8-1;
}
static inline bool ldouble_fits_numRxxx(long double k)
{
  long double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexpl(k,&e);
  return e < (int)sizeof(numIxxx_t)*8-1;
}
static inline bool mpfr_fits_numRxxx(mpfr_t a, numinternal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(a)) return false;
  if (mpfr_sgn(a)==0) return true;
  e = mpfr_get_exp(a);
  return e < (mp_exp_t)sizeof(numIxxx_t)*8-1;
}
static inline bool numRxxx_fits_lint(numRxxx_t a)
{
  numIxxx_t b;
  numIxxx_cdiv_q(b,a->n,a->d);
  return *b<=LONG_MAX && *b>=-LONG_MAX;
}
static inline bool numRxxx_fits_llint(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_lfrac(numRxxx_t a)
{ return numIxxx_fits_lint(numRxxx_numref(a)) && numIxxx_fits_lint(numRxxx_denref(a)); }
static inline bool numRxxx_fits_llfrac(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_mpq(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_float(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_double(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_ldouble(numRxxx_t a)
{ return true; }
static inline bool numRxxx_fits_mpfr(numRxxx_t a)
{ return numIxxx_fits_mpfr(numRxxx_numref(a)) && numIxxx_fits_mpfr(numRxxx_denref(a)); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numRxxx_set_lint(numRxxx_t a, long int b, numinternal_t intern)
{
  numIxxx_set_lint(a->n,b,intern);
  numIxxx_set_int(a->d,1L);
  return true;
}
static inline bool numRxxx_set_llint(numRxxx_t a, long long int b, numinternal_t intern)
{
  numIxxx_set_llint(a->n,b,intern);
  numIxxx_set_int(a->d,1);
  return true;
}
static inline bool numRxxx_set_mpz(numRxxx_t a, mpz_t b, numinternal_t intern)
{
  numIxxx_set_mpz(a->n,b,intern);
  numIxxx_set_int(a->d,1);
  return true;
}
static inline bool numRxxx_set_lfrac(numRxxx_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numIxxx_set_lint(a->n,i,intern);
  numIxxx_set_lint(a->d,j,intern);
  numRxxx_canonicalize(a);
  return true;
}
static inline bool numRxxx_set_llfrac(numRxxx_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numIxxx_set_llint(a->n,i,intern);
  numIxxx_set_llint(a->d,j,intern);
  numRxxx_canonicalize(a);
  return true;
}
static inline bool numRxxx_set_mpq(numRxxx_t a, mpq_t b, numinternal_t intern)
{
  numIxxx_set_mpz(a->n,mpq_numref(b),intern);
  numIxxx_set_mpz(a->d,mpq_denref(b),intern);
  numRxxx_canonicalize(a);
  return true;
}
static inline bool numRxxx_set_double(numRxxx_t a, double k, numinternal_t intern)
{
  const int size = sizeof(numIxxx_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numRxxx_set_int(a,0); return false; }
  if (k==0.0){
    numRxxx_set_int(a,0);
    return true;
  }
  k = frexp(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMIXXX_ONE;
      *a->d = NUMIXXX_MAX;
    }
    else {
      numRxxx_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexp(k,l);
    res = numIxxx_set_double(a->n,k,intern);
    *a->d = NUMIXXX_ONE << (l-e);
    numRxxx_canonicalize(a);
    return res;
  }
  else {
    numRxxx_set_int(a,0);
    return false;
  }
}
static inline bool numRxxx_set_ldouble(numRxxx_t a, long double k, numinternal_t intern)
{
  const int size = sizeof(numIxxx_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; numRxxx_set_int(a,0); return false; }
  if (k==0.0){
    numRxxx_set_int(a,0);
    return true;
  }
  k = frexpl(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMIXXX_ONE;
      *a->d = NUMIXXX_MAX;
    }
    else {
      numRxxx_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexpl(k,l);
    res = numIxxx_set_ldouble(a->n,k,intern);
    *a->d = NUMIXXX_ONE << (l-e);
    numRxxx_canonicalize(a);
    return res;
  }
  else {
    numRxxx_set_int(a,0);
    return false;
  }
}
static inline bool numRxxx_set_mpfr(numRxxx_t a, mpfr_t b, numinternal_t intern)
{
  long double k = mpfr_get_ld(b, GMP_RNDU);
  return numRxxx_set_ldouble(a,k,intern) && (mpfr_cmp_ld(b,k)==0);
}
static inline bool lint_set_numRxxx(long int* a, numRxxx_t b, numinternal_t intern)
{
  numIxxx_t q,r;
  numIxxx_cdiv_qr(q,r,numRxxx_numref(b),numRxxx_denref(b));
  return lint_set_numIxxx(a,q,intern) && (numIxxx_sgn(r)==0);
}
static inline bool llint_set_numRxxx(long long int* a, numRxxx_t b, numinternal_t intern)
{
  numIxxx_t q,r;
  numIxxx_cdiv_qr(q,r,numRxxx_numref(b),numRxxx_denref(b));
  return llint_set_numIxxx(a,q,intern) && (numIxxx_sgn(r)==0);
}
static inline bool mpz_set_numRxxx(mpz_t a, numRxxx_t b, numinternal_t intern)
{
  numIxxx_t q,r;
  numIxxx_cdiv_qr(q,r,numRxxx_numref(b),numRxxx_denref(b));
  mpz_set_numIxxx(a,q,intern);
  return (numIxxx_sgn(r)==0);
}
static inline bool lfrac_set_numRxxx(long int* i, long int* j, numRxxx_t b, numinternal_t intern)
{
  lint_set_numIxxx(i,numRxxx_numref(b),intern);
  lint_set_numIxxx(j,numRxxx_denref(b),intern);
  return true;
}
static inline bool llfrac_set_numRxxx(long long int* i, long long int* j, numRxxx_t b, numinternal_t intern)
{
  llint_set_numIxxx(i,numRxxx_numref(b),intern);
  llint_set_numIxxx(j,numRxxx_denref(b),intern);
  return true;
}
static inline bool mpq_set_numRxxx(mpq_t a, numRxxx_t b, numinternal_t intern)
{
  mpz_set_numIxxx(mpq_numref(a), numRxxx_numref(b),intern);
  mpz_set_numIxxx(mpq_denref(a), numRxxx_denref(b),intern);
  return true;
}
static inline bool double_set_numRxxx(double* a, numRxxx_t b, numinternal_t intern)
{
  *a = (double)(*b->n)/(double)(*b->d);
  return (-*a==(double)(-*b->n)/(double)(*b->d));
}
static inline bool ldouble_set_numRxxx(long double* a, numRxxx_t b, numinternal_t intern)
{
  *a = (long double)(*b->n)/(long double)(*b->d);
  return (-*a==(long double)(-*b->n)/(long double)(*b->d));
}
static inline bool mpfr_set_numRxxx(mpfr_t a, numRxxx_t b, numinternal_t intern)
{
  int r = mpfr_set_si(a,*numRxxx_numref(b),GMP_RNDU);
  return !mpfr_div_si(a,a,*numRxxx_denref(b),GMP_RNDU) && !r;
}

#ifdef __cplusplus
}
#endif

#endif
