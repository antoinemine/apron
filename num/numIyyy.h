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
/* Require C99 compliant compiler */

#ifdef __cplusplus
extern "C" {
#endif

#define _NUMIyyy_MARK_

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
static inline void numIyyy_cdiv_qr(numIyyy_t q, numIyyy_t r, numIyyy_t a, numIyyy_t b)
{
  numIyyy_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numIyyy_sgn(a)*numIyyy_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}

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

static inline void numIyyy_sqrt(numIyyy_t up, numIyyy_t down, numIyyy_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numIyyy_max_exact_double) {
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
static inline numIyyy_native _gcd_auxIyyy(numIyyy_native a, numIyyy_native b)
{
  numIyyy_native t;
  numIyyy_abs(&a,&a);
  numIyyy_abs(&b,&b);
  if (a<b){
    numIyyy_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMIyyy_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
static inline void numIyyy_gcd(numIyyy_t a, numIyyy_t b,  numIyyy_t c)
{ *a = _gcd_auxIyyy(*b,*c); }

static inline numIyyy_native _lcm_auxIyyy(numIyyy_native a, numIyyy_native b)
{
  numIyyy_abs(&a,&a);
  numIyyy_abs(&b,&b);
  return (a / _gcd_auxIyyy(a,b)) * b;
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

static inline size_t numIyyy_serialize_array(void* dst, numIyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numIyyy_deserialize_array(numIyyy_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numIyyy_serialized_size_array(numIyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

#if defined(_NUMIl_MARK_)

static inline bool lint_fits_numIyyy(long int a)
{ return true; }
static inline bool llint_fits_numIyyy(long long int a)
{ return a>=-NUMIyyy_MAX && a<=NUMIyyy_MAX; }
static inline bool mpz_fits_numIyyy(mpz_t a)
{ return mpz_fits_slong_p(a); }
static inline bool lfrac_fits_numIyyy(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numIyyy(long long int i, long long int j)
{
  return llint_fits_numIyyy(i>=0 ? (i+j-1)/j : i/j);
}
static inline bool mpq_fits_numIyyy(mpq_t a)
{
  size_t n = mpz_sizeinbase(mpq_numref(a),2);
  size_t d = mpz_sizeinbase(mpq_denref(a),2);
  return ((int)n - (int)d) <= (int)(sizeof(numIyyy_t)*8-3);
}
static inline bool double_fits_numIyyy(double a)
{
  return isfinite(a) && a>=(double)(-NUMIyyy_MAX) && a<=(double)NUMIyyy_MAX;
}
static inline bool ldouble_fits_numIyyy(long double a)
{
  return isfinite(a) && a>=(long double)(-NUMIyyy_MAX) && a<=(long double)NUMIyyy_MAX;
}
static inline bool mpfr_fits_numIyyy(mpfr_t a, num_internal_t intern)
{
  return mpfr_number_p(a) && mpfr_fits_slong_p(a,GMP_RNDU);
}

static inline bool numIyyy_fits_lint(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_llint(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_mpz(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_lfrac(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_llfrac(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_mpq(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_float(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_double(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_ldouble(numIyyy_t a)
{ return true; }
static inline bool numIyyy_fits_mpfr(numIyyy_t a)
{ return true; }

#elif defined (_NUMIll_MARK_)

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
static inline bool mpfr_fits_numIll(mpfr_t a, num_internal_t intern)
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

#else
#error "HERE"
#endif

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

#if defined(_NUMIl_MARK_)

static inline bool numIl_set_lint(numIl_t a, long int b, num_internal_t intern)
{ *a = b; return true; }
static inline bool numIl_set_llint(numIl_t a, long long int b, num_internal_t intern)
{
  *a = (long int)b;
  return true;
}
static inline bool numIl_set_mpz(numIl_t a, mpz_t b, num_internal_t intern)
{
  *a = mpz_get_si(b);
  return true;
}
static inline bool numIl_set_lfrac(numIl_t a, long int i, long int j, num_internal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j) ? false : true;
}
static inline bool numIl_set_llfrac(numIl_t a, long long int i, long long int j, num_internal_t intern)
{
  long long int t;
  assert(j>0);
  t = i>=0 ? (i+j-1)/j : i/j;
  return numIl_set_llint(a,t,intern) && i%j==0;
}
static inline bool numIl_set_mpq(numIl_t a, mpq_t b, num_internal_t intern)
{
  mpz_cdiv_qr(intern->q, intern->r, mpq_numref(b),mpq_denref(b));
  numIl_set_mpz(a,intern->q,intern);
  return mpz_sgn(intern->r)==0;
}
static inline bool numIl_set_double(numIl_t a, double b, num_internal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool numIl_set_ldouble(numIl_t a, long double b, num_internal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool numIl_set_mpfr(numIl_t a, mpfr_t b, num_internal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numIl_set_int(a,0); return false; }
  *a = mpfr_get_si(b,GMP_RNDU);
  return mpfr_integer_p(b);
}

static inline bool lint_set_numIl(long int* a, numIl_t b, num_internal_t intern)
{ *a = *b; return true; }
static inline bool llint_set_numIl(long long int* a, numIl_t b, num_internal_t intern)
{ *a = (long long int)(*b); return true; }
static inline bool mpz_set_numIl(mpz_t a, numIl_t b, num_internal_t intern)
{ mpz_set_si(a,*b); return true; }
static inline bool lfrac_set_numIl(long int *a, long int* b, numIl_t c, num_internal_t intern)
{
  *a = *c;
  *b = 1L;
  return true;
}
static inline bool llfrac_set_numIl(long long int *a, long long int* b, numIl_t c, num_internal_t intern)
{
  *a = (long long int)(*c);
  *b = 1LL;
  return true;
}
static inline bool mpq_set_numIl(mpq_t a, numIl_t b, num_internal_t intern)
{
  mpq_set_si(a,*b,1);
  return true;
}
static inline bool double_set_numIl(double* a, numIl_t b, num_internal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_numIl(long double* a, numIl_t b, num_internal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_numIl(mpfr_t a, numIl_t b, num_internal_t intern)
{
  return !mpfr_set_si(a,*b,GMP_RNDU);
}

#elif defined(_NUMIll_MARK_)

static inline bool numIll_set_lint(numIll_t a, long int b, num_internal_t intern)
{
  *a = (long long int)b;
  return true;
}
static inline bool numIll_set_llint(numIll_t a, long long int b, num_internal_t intern)
{
  *a = b;
  return true;
}
static inline bool numIll_set_mpz(numIll_t a, mpz_t b, num_internal_t intern)
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
static inline bool numIll_set_lfrac(numIll_t a, long int i, long int j, num_internal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool numIll_set_llfrac(numIll_t a, long long int i, long long int j, num_internal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool numIll_set_mpq(numIll_t a, mpq_t b, num_internal_t intern)
{
  mpz_cdiv_qr(intern->q,intern->r, mpq_numref(b),mpq_denref(b));
  return numIll_set_mpz(a,intern->q,intern) && (mpz_sgn(intern->r)==0);
}
static inline bool numIll_set_double(numIll_t a, double b, num_internal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool numIll_set_ldouble(numIll_t a, long double b, num_internal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool numIll_set_mpfr(numIll_t a, mpfr_t b, num_internal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numIll_set_int(a,0); return false; }
  *a = mpfr_get_sj(b,GMP_RNDU);
  return mpfr_integer_p(b);
}
static inline bool lint_set_numIll(long int* a, numIll_t b, num_internal_t intern)
{ *a = (long int)(*b); return true; }
static inline bool llint_set_numIll(long long int* a, numIll_t b, num_internal_t intern)
{ *a = *b; return true; }
static inline bool mpz_set_numIll(mpz_t a, numIll_t b, num_internal_t intern)
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
static inline bool lfrac_set_numIll(long int* a, long int* b, numIll_t c, num_internal_t intern)
{
  *a = (long int)(*c);
  *b = 1L;
  return true;
}
static inline bool llfrac_set_numIll(long long int* a, long long int* b, numIll_t c, num_internal_t intern)
{
  *a = *c;
  *b = 1LL;
  return true;
}
static inline bool mpq_set_numIll(mpq_t a, numIll_t b, num_internal_t intern)
{
  mpz_set_ui(mpq_denref(a),1);
  return mpz_set_numIll(mpq_numref(a),b,intern);
}
static inline bool double_set_numIll(double* a, numIll_t b, num_internal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_numIll(long double* a, numIll_t b, num_internal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_numIll(mpfr_t a, numIll_t b, num_internal_t intern)
{
  return !mpfr_set_sj(a,*b,GMP_RNDU);
}

#else
#error "HERE"
#endif

#undef _NUMIyyy_MARK_

#ifdef __cplusplus
}
#endif

#endif
