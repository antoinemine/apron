/* ********************************************************************** */
/* numIll.h */
/* ********************************************************************** */

#ifndef _NUMIll_H_
#define _NUMIll_H_

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

static inline void numIll_set(numIll_t a, numIll_t b)
{ *a = *b; }
static inline void numIll_set_array(numIll_t* a, numIll_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numIll_t)); }
static inline void numIll_set_int(numIll_t a, long int i)
{ *a = (numIll_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numIll_init(numIll_t a)
{ *a = NUMIll_ZERO; }
static inline void numIll_init_array(numIll_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMIll_ZERO;
}
static inline void numIll_init_set(numIll_t a, numIll_t b)
{ numIll_set(a,b); }
static inline void numIll_init_set_int(numIll_t a, long int i)
{ numIll_set_int(a,i); }

static inline void numIll_clear(numIll_t a)
{}
static inline void numIll_clear_array(numIll_t* a, size_t size)
{}
static inline void numIll_swap(numIll_t a, numIll_t b)
{ numIll_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numIll_neg(numIll_t a, numIll_t b)
{ *a = -(*b); }
static inline void numIll_abs(numIll_t a, numIll_t b)
{ *a = llabs(*b); }
static inline void numIll_add(numIll_t a, numIll_t b, numIll_t c)
{ *a = *b + *c; }
static inline void numIll_add_uint(numIll_t a, numIll_t b, unsigned long int c)
{ *a = *b + c; }
static inline void numIll_sub(numIll_t a, numIll_t b, numIll_t c)
{ *a = *b - *c; }
static inline void numIll_sub_uint(numIll_t a, numIll_t b, unsigned long int c)
{ *a = *b - c; }
static inline void numIll_mul(numIll_t a, numIll_t b, numIll_t c)
{ *a = *b * *c; }
static inline void numIll_mul_2(numIll_t a, numIll_t b)
{ *a = *b << 1; }

static inline void numIll_tdiv_q(numIll_t q, numIll_t a, numIll_t b)
{ *q = *a / *b; }

static inline int numIll_sgn(numIll_t a)
{ return (*a==NUMIll_ZERO ? 0 : (*a>NUMIll_ZERO ? 1 : -1)); }

static inline void numIll_fdiv_q(numIll_t q, numIll_t a, numIll_t b)
{
  if (numIll_sgn(a)*numIll_sgn(b)<0 && *a % *b) *q = *a / *b - 1;
  else *q = *a / *b;
}
static inline void numIll_cdiv_q(numIll_t q, numIll_t a, numIll_t b)
{
  if (numIll_sgn(a)*numIll_sgn(b)>0 && *a % *b) *q = *a / *b + 1;
  else *q = *a / *b;
}
static inline void numIll_cdiv_qr(numIll_t q, numIll_t r, numIll_t a, numIll_t b)
{
  numIll_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numIll_sgn(a)*numIll_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

static inline void numIll_cdiv_2(numIll_t a, numIll_t b)
{ *a = (*b>=NUMIll_ZERO) ? (*b+1)/2 : *b/2; }
static inline void numIll_cdiv_q_2exp(numIll_t a, numIll_t b, unsigned long int c)
{ *a = (*b >> c)+(*b & ((NUMIll_ONE<<c)-NUMIll_ONE) ? 1 : 0); }
static inline void numIll_fdiv_q_2exp(numIll_t a, numIll_t b, unsigned long int c)
{ *a = (*b >> c); }
static inline void numIll_min(numIll_t a, numIll_t b, numIll_t c)
{ *a = (*b<=*c) ? *b : *c; }
static inline void numIll_max(numIll_t a, numIll_t b, numIll_t c)
{ *a = (*b>=*c) ? *b : *c; }

static const long long numIll_max_exact_double = 1LL << 52;

static inline void numIll_sqrt(numIll_t up, numIll_t down, numIll_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numIll_max_exact_double) {
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

static inline void numIll_mul_2exp(numIll_t a, numIll_t b, int c)
{
  if (c>=0) *a = *b << c;
  else numIll_cdiv_q_2exp(a,b,-c);
}

static inline void numIll_trunc(numIll_t a, numIll_t b)
{ numIll_set(a,b); }
static inline void numIll_floor(numIll_t a, numIll_t b)
{ numIll_set(a,b); }
static inline void numIll_ceil(numIll_t a, numIll_t b)
{ numIll_set(a,b); }
static inline void numIll_div(numIll_t a, numIll_t b, numIll_t c)
{ numIll_cdiv_q(a,b,c); }
static inline void numIll_div_2(numIll_t a, numIll_t b)
{ numIll_cdiv_2(a,b); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numIll_divexact(numIll_t a, numIll_t b, numIll_t c)
{ *a = *b / *c; }
static inline void numIll_mod(numIll_t a, numIll_t b, numIll_t c)
{ *a = *b % *c; }
static inline numIll_native _gcd_auxIll(numIll_native a, numIll_native b)
{
  numIll_native t;
  numIll_abs(&a,&a);
  numIll_abs(&b,&b);
  if (a<b){
    numIll_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMIll_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline void numIll_gcd(numIll_t a, numIll_t b,  numIll_t c)
{ *a = _gcd_auxIll(*b,*c); }

static inline numIll_native _lcm_auxIll(numIll_native a, numIll_native b)
{
  numIll_abs(&a,&a);
  numIll_abs(&b,&b);
  return (a / _gcd_auxIll(a,b)) * b;
}
static inline void numIll_lcm(numIll_t a, numIll_t b,  numIll_t c)
{ *a = _lcm_auxIll(*b,*c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numIll_cmp(numIll_t a, numIll_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numIll_cmp_int(numIll_t a, long int b)
{ return (*a==b ? 0 : (*a>b ? 1 : -1)); }
static inline bool numIll_equal(numIll_t a, numIll_t b)
{ return *a==*b; }
static inline bool numIll_integer(numIll_t a)
{ return true; }
static inline int numIll_hash(numIll_t a)
{ return (int)(*a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numIll_print(numIll_t a)
{ printf("%lli",*a); }
static inline void numIll_fprint(FILE* stream, numIll_t a)
{ fprintf(stream,"%lli",*a); }
static inline int numIll_snprint(char* s, size_t size, numIll_t a)
{ return snprintf(s,size,"%lli",*a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numIll_serialize_id(void)
{ return sizeof(numIll_t)/4; }

static inline size_t numIll_serialize(void* dst, numIll_t src)
{
  num_store_words8(dst,src,sizeof(numIll_t));
  return sizeof(numIll_t);
}

static inline size_t numIll_deserialize(numIll_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numIll_t));
  return sizeof(numIll_t);
}

static inline size_t numIll_serialized_size(numIll_t a)
{ return sizeof(numIll_t); }

static inline size_t numIll_serialize_array(void* dst, numIll_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIll_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numIll_deserialize_array(numIll_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIll_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numIll_serialized_size_array(numIll_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIll_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numIll(long int a)
{ return true; }
static inline bool llint_fits_numIll(long long int a)
{ return true; }
static inline bool mpz_fits_numIll(mpz_t a)
{
  size_t size = mpz_sizeinbase(a,2);
  return (size <= sizeof(numIll_t)*8-1);
}
static inline bool lfrac_fits_numIll(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numIll(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numIll(mpq_t a)
{
  size_t n = mpz_sizeinbase(mpq_numref(a),2);
  size_t d = mpz_sizeinbase(mpq_denref(a),2);
  return ((int)n - (int)d) <= (int)(sizeof(numIll_t)*8-3);
}
static inline bool double_fits_numIll(double a)
{
  return isfinite(a) && a>=(double)(-NUMIll_MAX) && a<=(double)NUMIll_MAX;
}
static inline bool ldouble_fits_numIll(long double a)
{
  return isfinite(a) && a>=(long double)(-NUMIll_MAX) && a<=(long double)NUMIll_MAX;
}
static inline bool mpfr_fits_numIll(mpfr_t a, numinternal_t intern)
{
  return mpfr_number_p(a) && mpfr_fits_intmax_p(a,GMP_RNDU);
}

static inline bool numIll_fits_lint(numIll_t a)
{ return (*a>=-LONG_MAX && *a<=LONG_MAX); }
static inline bool numIll_fits_llint(numIll_t a)
{ return true; }
static inline bool numIll_fits_mpz(numIll_t a)
{ return true; }
static inline bool numIll_fits_lfrac(numIll_t a)
{ return (*a>=-LONG_MAX && *a<=LONG_MAX); }
static inline bool numIll_fits_llfrac(numIll_t a)
{ return true; }
static inline bool numIll_fits_mpq(numIll_t a)
{ return true; }
static inline bool numIll_fits_float(numIll_t a)
{ return true; }
static inline bool numIll_fits_double(numIll_t a)
{ return true; }
static inline bool numIll_fits_ldouble(numIll_t a)
{ return true; }
static inline bool numIll_fits_mpfr(numIll_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numIll_set_lint(numIll_t a, long int b, numinternal_t intern)
{
  *a = (long long int)b;
  return true;
}
static inline bool numIll_set_llint(numIll_t a, long long int b, numinternal_t intern)
{
  *a = b;
  return true;
}
static inline bool numIll_set_mpz(numIll_t a, mpz_t b, numinternal_t intern)
{
  int sgn;
  size_t count;
  unsigned long int tab[2];
  bool res;

  sgn = mpz_sgn(b);
  mpz_export(&tab,&count,1,sizeof(long int),0,0,b);
  if (count==0){
    *a = 0;
    res = true;
  }
  else {
    const int dec = 
      (sizeof(long int) == sizeof(long long int)) ? 
      0 : 
      sizeof(long int)*8;

    *a = tab[0];
    if (count==2){
      assert(dec!=0);
      *a = *a << dec;
      *a = *a + (long long int)(tab[1]);
      assert(*a>=0LL);
      count = mpz_sizeinbase(b,2);
      res = count <= sizeof(numIll_t)*8-1;
    }
    else {
      res = true;
    }
    if (sgn<0) *a = -(*a);
  }
  return res;
}
static inline bool numIll_set_lfrac(numIll_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool numIll_set_llfrac(numIll_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool numIll_set_mpq(numIll_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(intern->q,intern->r, mpq_numref(b),mpq_denref(b));
  return numIll_set_mpz(a,intern->q,intern) && (mpz_sgn(intern->r)==0);
}
static inline bool numIll_set_double(numIll_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool numIll_set_ldouble(numIll_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool numIll_set_mpfr(numIll_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numIll_set_int(a,0); return false; }
  *a = mpfr_get_sj(b,GMP_RNDU);
  return mpfr_integer_p(b);
}
static inline bool lint_set_numIll(long int* a, numIll_t b, numinternal_t intern)
{ *a = (long int)(*b); return true; }
static inline bool llint_set_numIll(long long int* a, numIll_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool mpz_set_numIll(mpz_t a, numIll_t b, numinternal_t intern)
{
  unsigned long long int n;
  unsigned long int rep[2];
  const int dec = 
    (sizeof(long int) == sizeof(long long int)) ? 
    0 : 
    sizeof(long int)*8;
  
  n = llabs(*b);
  if (dec==0){
    rep[0] = n & ULONG_MAX;
    mpz_import(a,1,1,sizeof(unsigned long int),0,0,rep);
  }
  else {
    rep[1] = n & ULONG_MAX;
    rep[0] = n >> dec;
    mpz_import(a,2,1,sizeof(unsigned long int),0,0,rep);
  }
  if (*b<0)
    mpz_neg(a,a);
  return true;
}
static inline bool lfrac_set_numIll(long int* a, long int* b, numIll_t c, numinternal_t intern)
{
  *a = (long int)(*c);
  *b = 1L;
  return true;
}
static inline bool llfrac_set_numIll(long long int* a, long long int* b, numIll_t c, numinternal_t intern)
{
  *a = *c;
  *b = 1LL;
  return true;
}
static inline bool mpq_set_numIll(mpq_t a, numIll_t b, numinternal_t intern)
{
  mpz_set_ui(mpq_denref(a),1);
  return mpz_set_numIll(mpq_numref(a),b,intern);
}
static inline bool double_set_numIll(double* a, numIll_t b, numinternal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_numIll(long double* a, numIll_t b, numinternal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_numIll(mpfr_t a, numIll_t b, numinternal_t intern)
{
  return !mpfr_set_sj(a,*b,GMP_RNDU);
}

#ifdef __cplusplus
}
#endif

#endif
