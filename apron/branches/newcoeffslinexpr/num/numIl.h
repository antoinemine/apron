/* ********************************************************************** */
/* numIl.h */
/* ********************************************************************** */

#ifndef _NUMIl_H_
#define _NUMIl_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "numTypes.h"
/* Require C99 compliant compiler */

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numIl_set(numIl_t a, numIl_t b)
{ *a = *b; }
static inline void numIl_set_array(numIl_t* a, numIl_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numIl_t)); }
static inline void numIl_set_int(numIl_t a, long int i)
{ *a = (numIl_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numIl_init(numIl_t a)
{ *a = NUMIl_ZERO; }
static inline void numIl_init_array(numIl_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMIl_ZERO;
}
static inline void numIl_init_set(numIl_t a, numIl_t b)
{ numIl_set(a,b); }
static inline void numIl_init_set_int(numIl_t a, long int i)
{ numIl_set_int(a,i); }

static inline void numIl_clear(numIl_t a)
{}
static inline void numIl_clear_array(numIl_t* a, size_t size)
{}
static inline void numIl_swap(numIl_t a, numIl_t b)
{ numIl_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numIl_neg(numIl_t a, numIl_t b)
{ *a = -(*b); }
static inline void numIl_abs(numIl_t a, numIl_t b)
{ *a = labs(*b); }
static inline void numIl_add(numIl_t a, numIl_t b, numIl_t c)
{ *a = *b + *c; }
static inline void numIl_add_uint(numIl_t a, numIl_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numIl_sub(numIl_t a, numIl_t b, numIl_t c)
{ *a = *b - *c; }
static inline void numIl_sub_uint(numIl_t a, numIl_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numIl_mul(numIl_t a, numIl_t b, numIl_t c)
{ *a = *b * *c; }
static inline void numIl_mul_2(numIl_t a, numIl_t b)
{ *a = *b << 1; }

static inline void numIl_tdiv_q(numIl_t q, numIl_t a, numIl_t b)
{ *q = *a / *b; }

static inline int numIl_sgn(numIl_t a)
{ return (*a==NUMIl_ZERO ? 0 : (*a>NUMIl_ZERO ? 1 : -1)); }

static inline void numIl_fdiv_q(numIl_t q, numIl_t a, numIl_t b)
{
  if (numIl_sgn(a)*numIl_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numIl_cdiv_q(numIl_t q, numIl_t a, numIl_t b)
{
  if (numIl_sgn(a)*numIl_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}
static inline void numIl_cdiv_qr(numIl_t q, numIl_t r, numIl_t a, numIl_t b)
{
  numIl_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numIl_sgn(a)*numIl_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

static inline void numIl_cdiv_2(numIl_t a, numIl_t b)
{ *a = (*b>=NUMIl_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numIl_cdiv_q_2exp(numIl_t a, numIl_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMIl_ONE<<c)-NUMIl_ONE) ? 1 : 0); }
static inline void numIl_fdiv_q_2exp(numIl_t a, numIl_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numIl_min(numIl_t a, numIl_t b, numIl_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numIl_max(numIl_t a, numIl_t b, numIl_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numIl_max_exact_double = 1LL << 52;

static inline void numIl_sqrt(numIl_t up, numIl_t down, numIl_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numIl_max_exact_double) {
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

static inline void numIl_mul_2exp(numIl_t a, numIl_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numIl_cdiv_q_2exp(a,b,-c);
}

static inline void numIl_trunc(numIl_t a, numIl_t b)
{ numIl_set(a,b); }
static inline void numIl_floor(numIl_t a, numIl_t b)
{ numIl_set(a,b); }
static inline void numIl_ceil(numIl_t a, numIl_t b)
{ numIl_set(a,b); }
static inline void numIl_div(numIl_t a, numIl_t b, numIl_t c)
{ numIl_cdiv_q(a,b,c); }
static inline void numIl_div_2(numIl_t a, numIl_t b)
{ numIl_cdiv_2(a,b); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numIl_divexact(numIl_t a, numIl_t b, numIl_t c)
{ *a = *b / *c; }
static inline void numIl_mod(numIl_t a, numIl_t b, numIl_t c)
{ *a = *b % *c; }
static inline numIl_native _gcd_auxIl(numIl_native a, numIl_native b)
{
  numIl_native t;
  numIl_abs(&a,&a);
  numIl_abs(&b,&b);
  if (a<b){
    numIl_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMIl_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline void numIl_gcd(numIl_t a, numIl_t b,  numIl_t c)
{ *a = _gcd_auxIl(*b,*c); }

static inline numIl_native _lcm_auxIl(numIl_native a, numIl_native b)
{
  numIl_abs(&a,&a);
  numIl_abs(&b,&b);
  return (a / _gcd_auxIl(a,b)) * b;
}
static inline void numIl_lcm(numIl_t a, numIl_t b,  numIl_t c)
{ *a = _lcm_auxIl(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numIl_cmp(numIl_t a, numIl_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numIl_cmp_int(numIl_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numIl_equal(numIl_t a, numIl_t b)
{ return *a==*b; }
static inline bool numIl_integer(numIl_t a)
{ return true; }
static inline int numIl_hash(numIl_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numIl_print(numIl_t a)
{ printf("%li",*a); }
static inline void numIl_fprint(FILE* stream, numIl_t a)
{ fprintf(stream,"%li",*a); }
static inline int numIl_snprint(char* s, size_t size, numIl_t a)
{ return snprintf(s,size,"%li",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numIl_serialize_id(void)
{ return sizeof(numIl_t)/4; }

static inline size_t numIl_serialize(void* dst, numIl_t src)
{
  num_store_words8(dst,src,sizeof(numIl_t));
  return sizeof(numIl_t);
}

static inline size_t numIl_deserialize(numIl_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numIl_t));
  return sizeof(numIl_t);
}

static inline size_t numIl_serialized_size(numIl_t a)
{ return sizeof(numIl_t); }

static inline size_t numIl_serialize_array(void* dst, numIl_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIl_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numIl_deserialize_array(numIl_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIl_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numIl_serialized_size_array(numIl_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIl_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numIl(long int a)
{ return true; }
static inline bool llint_fits_numIl(long long int a)
{ return a>=-NUMIl_MAX && a<=NUMIl_MAX; }
static inline bool mpz_fits_numIl(mpz_t a)
{ return mpz_fits_slong_p(a); }
static inline bool lfrac_fits_numIl(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numIl(long long int i, long long int j)
{
  return llint_fits_numIl(i>=0 ? (i+j-1)/j : i/j);
}
static inline bool mpq_fits_numIl(mpq_t a)
{
  size_t n = mpz_sizeinbase(mpq_numref(a),2);
  size_t d = mpz_sizeinbase(mpq_denref(a),2);
  return ((int)n - (int)d) <= (int)(sizeof(numIl_t)*8-3);
}
static inline bool double_fits_numIl(double a)
{
  return isfinite(a) && a>=(double)(-NUMIl_MAX) && a<=(double)NUMIl_MAX;
}
static inline bool ldouble_fits_numIl(long double a)
{
  return isfinite(a) && a>=(long double)(-NUMIl_MAX) && a<=(long double)NUMIl_MAX;
}
static inline bool mpfr_fits_numIl(mpfr_t a, numinternal_t intern)
{
  return mpfr_number_p(a) && mpfr_fits_slong_p(a,GMP_RNDU);
}

static inline bool numIl_fits_lint(numIl_t a)
{ return true; }
static inline bool numIl_fits_llint(numIl_t a)
{ return true; }
static inline bool numIl_fits_mpz(numIl_t a)
{ return true; }
static inline bool numIl_fits_lfrac(numIl_t a)
{ return true; }
static inline bool numIl_fits_llfrac(numIl_t a)
{ return true; }
static inline bool numIl_fits_mpq(numIl_t a)
{ return true; }
static inline bool numIl_fits_float(numIl_t a)
{ return true; }
static inline bool numIl_fits_double(numIl_t a)
{ return true; }
static inline bool numIl_fits_ldouble(numIl_t a)
{ return true; }
static inline bool numIl_fits_mpfr(numIl_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numIl_set_lint(numIl_t a, long int b, numinternal_t intern)
{ *a = b; return true; }
static inline bool numIl_set_llint(numIl_t a, long long int b, numinternal_t intern)
{
  *a = (long int)b;
  return true;
}
static inline bool numIl_set_mpz(numIl_t a, mpz_t b, numinternal_t intern)
{
  *a = mpz_get_si(b);
  return true;
}
static inline bool numIl_set_lfrac(numIl_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j) ? false : true;
}
static inline bool numIl_set_llfrac(numIl_t a, long long int i, long long int j, numinternal_t intern)
{
  long long int t;
  assert(j>0);
  t = i>=0 ? (i+j-1)/j : i/j;
  return numIl_set_llint(a,t,intern) && i%j==0;
}
static inline bool numIl_set_mpq(numIl_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(intern->q, intern->r, mpq_numref(b),mpq_denref(b));
  numIl_set_mpz(a,intern->q,intern);
  return mpz_sgn(intern->r)==0;
}
static inline bool numIl_set_double(numIl_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool numIl_set_ldouble(numIl_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool numIl_set_mpfr(numIl_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numIl_set_int(a,0); return false; }
  *a = mpfr_get_si(b,GMP_RNDU);
  return mpfr_integer_p(b);
}

static inline bool lint_set_numIl(long int* a, numIl_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool llint_set_numIl(long long int* a, numIl_t b, numinternal_t intern)
{ *a = (long long int)(*b); return true; }
static inline bool mpz_set_numIl(mpz_t a, numIl_t b, numinternal_t intern)
{ mpz_set_si(a,*b); return true; }
static inline bool lfrac_set_numIl(long int *a, long int* b, numIl_t c, numinternal_t intern)
{
  *a = *c;
  *b = 1L;
  return true;
}
static inline bool llfrac_set_numIl(long long int *a, long long int* b, numIl_t c, numinternal_t intern)
{
  *a = (long long int)(*c);
  *b = 1LL;
  return true;
}
static inline bool mpq_set_numIl(mpq_t a, numIl_t b, numinternal_t intern)
{
  mpq_set_si(a,*b,1);
  return true;
}
static inline bool double_set_numIl(double* a, numIl_t b, numinternal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_numIl(long double* a, numIl_t b, numinternal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_numIl(mpfr_t a, numIl_t b, numinternal_t intern)
{
  return !mpfr_set_si(a,*b,GMP_RNDU);
}

#ifdef __cplusplus
}
#endif

#endif
