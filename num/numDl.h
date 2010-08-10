/* ********************************************************************** */
/* numDl.h */
/* ********************************************************************** */

#ifndef _NUMDl_H_
#define _NUMDl_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "numTypes.h"
#include "numIl.h"
#include "numIll.h"
#include "numMPZ.h"
#include "numRl.h"
#include "numRll.h"
#include "numMPQ.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numDl_set(numDl_t a, numDl_t b)
{ *a = *b; }
static inline void numDl_set_array(numDl_t* a, numDl_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numDl_t)); }
static inline void numDl_set_int(numDl_t a, long int i)
{ *a = (numDl_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numDl_init(numDl_t a)
{ *a = NUMDl_ZERO; }
static inline void numDl_init_array(numDl_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMDl_ZERO;
}
static inline void numDl_init_set(numDl_t a, numDl_t b)
{ numDl_set(a,b); }
static inline void numDl_init_set_int(numDl_t a, long int i)
{ numDl_set_int(a,i); }

static inline void numDl_clear(numDl_t a)
{}
static inline void numDl_clear_array(numDl_t* a, size_t size)
{}

static inline void numDl_swap(numDl_t a, numDl_t b)
{ numDl_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numDl_neg(numDl_t a, numDl_t b)
{ *a = -(*b); }
static inline void numDl_add(numDl_t a, numDl_t b, numDl_t c)
{ *a = *b + *c; }
static inline void numDl_add_uint(numDl_t a, numDl_t b, unsigned long int c)
{ *a = *b + (numDl_native)c; }
static inline void numDl_sub(numDl_t a, numDl_t b, numDl_t c)
{ *a = *b - *c; }
static inline void numDl_sub_uint(numDl_t a, numDl_t b, unsigned long int c)
{ *a = *b - (numDl_native)c; }
static inline void numDl_mul(numDl_t a, numDl_t b, numDl_t c)
{ *a = *b * *c; }
static inline void numDl_div(numDl_t a, numDl_t b, numDl_t c)
{ *a = *b / *c; }

static inline void numDl_abs(numDl_t a, numDl_t b)
{ *a = fabsl(*b); }
static inline void numDl_mul_2(numDl_t a, numDl_t b)
{ *a = ldexpl(*b,1); }
static inline void numDl_div_2(numDl_t a, numDl_t b)
{ *a = ldexpl(*b,-1); }
static inline void numDl_min(numDl_t a, numDl_t b, numDl_t c)
{ *a = fminl(*b,*c); }
static inline void numDl_max(numDl_t a, numDl_t b, numDl_t c)
{ *a = fmaxl(*b,*c); }
static inline void numDl_floor(numDl_t a, numDl_t b)
{ *a = floorl(*b); }
static inline void numDl_ceil(numDl_t a, numDl_t b)
{ *a = ceill(*b); }
static inline void numDl_trunc(numDl_t a, numDl_t b)
{ *a = truncl(*b); }
static inline void numDl_sqrt(numDl_t up, numDl_t down, numDl_t b)
{
  assert(*b>=0);
  *up = sqrtl(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafterl(*up,0);
}
static inline void numDl_mul_2exp(numDl_t a, numDl_t b, int c)
{ *a = ldexpl(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numDl_sgn(numDl_t a)
{ return (*a==NUMDl_ZERO ? 0 : (*a>NUMDl_ZERO ? 1 : -1)); }
static inline int numDl_cmp(numDl_t a, numDl_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numDl_cmp_int(numDl_t a, long int b)
{
  numDl_native bb = (numDl_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numDl_equal(numDl_t a, numDl_t b)
{ return *a==*b; }

static inline bool numDl_integer(numDl_t a)
{ return ceill(*a) == *a; }
static inline int numDl_hash(numDl_t a)
{
 if (*a <= -(long double)(INT_MAX))
    return -INT_MAX;
  else if (*a < (long double)INT_MAX)
    return lrintl(*a);
  else
    return INT_MAX;
}

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numDl_print(numDl_t a)
{ printf("%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline void numDl_fprint(FILE* stream, numDl_t a)
{ fprintf(stream,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline int numDl_snprint(char* s, size_t size, numDl_t a)
{ return snprintf(s,size,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numDl_infty(numDl_t a)
{ return !isfinite(*a); }
static inline void numDl_set_infty(numDl_t a, int sgn)
{ *a = sgn>0 ? NUMDl_MAX : -NUMDl_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numDl_serialize(void* dst, numDl_t src)
{
  num_store_words8(dst,src,sizeof(numDl_t));
  return sizeof(numDl_t);
}

static inline size_t numDl_deserialize(numDl_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numDl_t));
  return sizeof(numDl_t);
}

static inline size_t numDl_serialized_size(numDl_t a)
{ return sizeof(numDl_t); }

static inline size_t numDl_serialize_array(void* dst, numDl_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDl_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numDl_deserialize_array(numDl_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDl_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numDl_serialized_size_array(numDl_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDl_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numDl(long int a)
{ return true; }
static inline bool llint_fits_numDl(long long int a)
{ return true; }
static inline bool mpz_fits_numDl(mpz_t a)
{ return numMPZ_fits_ldouble(a); }
static inline bool lfrac_fits_numDl(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numDl(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numDl(mpq_t a)
{ return numMPQ_fits_ldouble(a); }
static inline bool double_fits_numDl(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numDl(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numDl(mpfr_t a, numinternal_t intern)
{
  if (mpfr_number_p(a)){
    if (mpfr_sgn(a)==0) return true;
    else {
      mp_exp_t e = mpfr_get_exp(a);
      return e<LDBL_MAX_EXP-1;
    }
  }
  else
    return false;
}

static inline bool numDl_fits_lint(numDl_t a)
{ return ldouble_fits_numIl(*a); }
static inline bool numDl_fits_llint(numDl_t a)
{ return ldouble_fits_numIll(*a); }
static inline bool numDl_fits_lfrac(numDl_t a)
{ return ldouble_fits_numRl(*a); }
static inline bool numDl_fits_llfrac(numDl_t a)
{ return ldouble_fits_numRll(*a); }
static inline bool numDl_fits_float(numDl_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numDl_fits_double(numDl_t a)
{
  int e;
  frexp(*a,&e);
  return (e<DBL_MAX_EXP-1);
}
static inline bool numDl_fits_ldouble(numDl_t a)
{ return true; }
static inline bool numDl_fits_mpfr(numDl_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numDl_set_lint(numDl_t a, long int i, numinternal_t intern)
{ return ldouble_set_numIl(a,&i,intern); }
static inline bool numDl_set_llint(numDl_t a, long long int i, numinternal_t intern)
{ return ldouble_set_numIll(a,&i,intern); }
static inline bool numDl_set_mpz(numDl_t a, mpz_t b, numinternal_t intern)
{ return ldouble_set_numMPZ(a,b,intern); }
static inline bool numDl_set_lfrac(numDl_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numDl_native)i/(numDl_native)j;
  return (-*a==(numDl_native)(-i)/(numDl_native)j);
}
static inline bool numDl_set_llfrac(numDl_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numDl_native)i/(numDl_native)j;
  return (-*a==(numDl_native)(-i)/(numDl_native)j);
}
static inline bool numDl_set_mpq(numDl_t a, mpq_t b, numinternal_t intern)
{ return ldouble_set_numMPQ(a,b,intern); }
static inline bool numDl_set_double(numDl_t a, double k, numinternal_t intern)
{ *a = (long double)k; return true; }
static inline bool numDl_set_ldouble(numDl_t a, long double k, numinternal_t intern)
{ *a = k; return true; }
static inline bool numDl_set_mpfr(numDl_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_ld(b,GMP_RNDU);
  return !mpfr_cmp_ld(b,*a);
}
static inline bool lint_set_numDl(long int* a, numDl_t b, numinternal_t intern)
{ return numIl_set_ldouble(a,*b,intern); }
static inline bool llint_set_numDl(long long int* a, numDl_t b, numinternal_t intern)
{ return numIll_set_ldouble(a,*b,intern); }
static inline bool mpz_set_numDl(mpz_t a, numDl_t b, numinternal_t intern)
{ return numMPZ_set_ldouble(a,*b,intern); }
static inline bool lfrac_set_numDl(long int* i, long int* j, numDl_t b, numinternal_t intern)
{
  numRl_native s;
  bool res = numRl_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_numDl(long long int* i, long long int* j, numDl_t b, numinternal_t intern)
{
  numRll_native s;
  bool res = numRll_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_numDl(mpq_t a, numDl_t b, numinternal_t intern)
{ return numMPQ_set_ldouble(a,*b,intern); }
static inline bool double_set_numDl(double* a, numDl_t b, numinternal_t intern)
{ *a = (double)*b; return true; }
static inline bool ldouble_set_numDl(long double* a, numDl_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool mpfr_set_numDl(mpfr_t a, numDl_t b, numinternal_t intern)
{ return !mpfr_set_ld(a,*b,GMP_RNDU); }

#ifdef __cplusplus
}
#endif

#endif
