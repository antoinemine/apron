/* ********************************************************************** */
/* numD.h */
/* ********************************************************************** */

#ifndef _NUMD_H_
#define _NUMD_H_

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
static inline void numD_set(numD_t a, numD_t b)
{ *a = *b; }
static inline void numD_set_array(numD_t* a, numD_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numD_t)); }
static inline void numD_set_int(numD_t a, long int i)
{ *a = (numD_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numD_init(numD_t a)
{ *a = NUMD_ZERO; }
static inline void numD_init_array(numD_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMD_ZERO;
}
static inline void numD_init_set(numD_t a, numD_t b)
{ numD_set(a,b); }
static inline void numD_init_set_int(numD_t a, long int i)
{ numD_set_int(a,i); }

static inline void numD_clear(numD_t a)
{}
static inline void numD_clear_array(numD_t* a, size_t size)
{}

static inline void numD_swap(numD_t a, numD_t b)
{ numD_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numD_neg(numD_t a, numD_t b)
{ *a = -(*b); }
static inline void numD_add(numD_t a, numD_t b, numD_t c)
{ *a = *b + *c; }
static inline void numD_add_uint(numD_t a, numD_t b, unsigned long int c)
{ *a = *b + (numD_native)c; }
static inline void numD_sub(numD_t a, numD_t b, numD_t c)
{ *a = *b - *c; }
static inline void numD_sub_uint(numD_t a, numD_t b, unsigned long int c)
{ *a = *b - (numD_native)c; }
static inline void numD_mul(numD_t a, numD_t b, numD_t c)
{ *a = *b * *c; }
static inline void numD_div(numD_t a, numD_t b, numD_t c)
{ *a = *b / *c; }

static inline void numD_abs(numD_t a, numD_t b)
{ *a = fabs(*b); }
static inline void numD_mul_2(numD_t a, numD_t b)
{ *a = ldexp(*b,1); }
static inline void numD_div_2(numD_t a, numD_t b)
{ *a = ldexp(*b,-1); }
static inline void numD_min(numD_t a, numD_t b, numD_t c)
{ *a = fmin(*b,*c); }
static inline void numD_max(numD_t a, numD_t b, numD_t c)
{ *a = fmax(*b,*c); }
static inline void numD_floor(numD_t a, numD_t b)
{ *a = floor(*b); }
static inline void numD_ceil(numD_t a, numD_t b)
{ *a = ceil(*b); }
static inline void numD_trunc(numD_t a, numD_t b)
{ *a = trunc(*b); }
static inline void numD_sqrt(numD_t up, numD_t down, numD_t b)
{
  numD_t x;
  assert(*b>=0);
  *x = sqrt(*b);
  assert(*x**x>=*b); /* assumes round towards +oo! */
  if (*x**x==*b) *down = *x;
  else *down = nextafter(*x,0);
  *up = *x;
}
static inline void numD_mul_2exp(numD_t a, numD_t b, int c)
{ *a = ldexp(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numD_sgn(numD_t a)
{ return (*a==NUMD_ZERO ? 0 : (*a>NUMD_ZERO ? 1 : -1)); }
static inline int numD_cmp(numD_t a, numD_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numD_cmp_int(numD_t a, long int b)
{
  numD_native bb = (numD_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numD_equal(numD_t a, numD_t b)
{ return *a==*b; }

static inline bool numD_integer(numD_t a)
{ return ceil(*a) == *a; }
static inline int numD_hash(numD_t a)
{
 if (*a <= -(double)(INT_MAX))
    return -INT_MAX;
  else if (*a < (double)INT_MAX)
    return lrint(*a);
  else
    return INT_MAX;
}


/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numD_print(numD_t a)
{ printf("%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline void numD_fprint(FILE* stream, numD_t a)
{ fprintf(stream,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline int numD_snprint(char* s, size_t size, numD_t a)
{ return snprintf(s,size,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numD_infty(numD_t a)
{ return !isfinite(*a); }
static inline void numD_set_infty(numD_t a, int sgn)
{ *a = sgn>0 ? NUMD_MAX : -NUMD_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numD_serialize(void* dst, numD_t src)
{
  num_store_words8(dst,src,sizeof(numD_t));
  return sizeof(numD_t);
}

static inline size_t numD_deserialize(numD_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numD_t));
  return sizeof(numD_t);
}

static inline size_t numD_serialized_size(numD_t a)
{ return sizeof(numD_t); }

static inline size_t numD_serialize_array(void* dst, numD_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numD_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numD_deserialize_array(numD_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numD_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numD_serialized_size_array(numD_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numD_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numD(long int a)
{ return true; }
static inline bool llint_fits_numD(long long int a)
{ return true; }
static inline bool mpz_fits_numD(mpz_t a)
{ return numMPZ_fits_double(a); }
static inline bool lfrac_fits_numD(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numD(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numD(mpq_t a)
{ return numMPQ_fits_double(a); }
static inline bool double_fits_numD(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numD(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numD(mpfr_t a, numinternal_t intern)
{
  if (mpfr_number_p(a)){
    if (mpfr_sgn(a)==0) return true;
    else {
      mp_exp_t e = mpfr_get_exp(a);
      return e<DBL_MAX_EXP-1;
    }
  }
  else
    return false;
}
static inline bool numD_fits_lint(numD_t a)
{ return double_fits_numIl(*a); }
static inline bool numD_fits_llint(numD_t a)
{ return double_fits_numIll(*a); }
static inline bool numD_fits_lfrac(numD_t a)
{ return double_fits_numRl(*a); }
static inline bool numD_fits_llfrac(numD_t a)
{ return double_fits_numRll(*a); }
static inline bool numD_fits_float(numD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numD_fits_double(numD_t a)
{ return true; }
static inline bool numD_fits_ldouble(numD_t a)
{ return true; }
static inline bool numD_fits_mpfr(numD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numD_set_lint(numD_t a, long int i, numinternal_t intern)
{ return double_set_numIl(a,&i,intern); }
static inline bool numD_set_llint(numD_t a, long long int i, numinternal_t intern)
{ return double_set_numIll(a,&i,intern); }
static inline bool numD_set_mpz(numD_t a, mpz_t b, numinternal_t intern)
{ return double_set_numMPZ(a,b,intern); }
static inline bool numD_set_lfrac(numD_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numD_native)i/(numD_native)j;
  return (-*a==(numD_native)(-i)/(numD_native)j);
}
static inline bool numD_set_llfrac(numD_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numD_native)i/(numD_native)j;
  return (-*a==(numD_native)(-i)/(numD_native)j);
}
static inline bool numD_set_mpq(numD_t a, mpq_t b, numinternal_t intern)
{ return double_set_numMPQ(a,b,intern); }
static inline bool numD_set_double(numD_t a, double k, numinternal_t intern)
{ *a = k; return true; }
static inline bool numD_set_ldouble(numD_t a, long double k, numinternal_t intern)
{ *a = (double)k; return true; }
static inline bool numD_set_mpfr(numD_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_d(b,GMP_RNDU);
  return !mpfr_cmp_d(b,*a);
}
static inline bool lint_set_numD(long int* a, numD_t b, numinternal_t intern)
{ return numIl_set_double(a,*b,intern); }
static inline bool llint_set_numD(long long int* a, numD_t b, numinternal_t intern)
{ return numIll_set_double(a,*b,intern); }
static inline bool mpz_set_numD(mpz_t a, numD_t b, numinternal_t intern)
{ return numMPZ_set_double(a,*b,intern); }
static inline bool lfrac_set_numD(long int* i, long int* j, numD_t b, numinternal_t intern)
{
  numRl_native s;
  bool res = numRl_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_numD(long long int* i, long long int* j, numD_t b, numinternal_t intern)
{
  numRll_native s;
  bool res = numRll_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_numD(mpq_t a, numD_t b, numinternal_t intern)
{ return numMPQ_set_double(a,*b,intern); }
static inline bool double_set_numD(double* a, numD_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool ldouble_set_numD(long double* a, numD_t b, numinternal_t intern)
{ *a = (long double)*b; return true; }
static inline bool mpfr_set_numD(mpfr_t a, numD_t b, numinternal_t intern)
{ return !mpfr_set_d(a,*b,GMP_RNDU); }

#ifdef __cplusplus
}
#endif

#endif
