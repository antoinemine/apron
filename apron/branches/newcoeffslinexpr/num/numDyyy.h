/* ********************************************************************** */
/* numDyyy.h */
/* ********************************************************************** */

#ifndef _NUMDyyy_H_
#define _NUMDyyy_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "num_types.h"
#include "numIl.h"
#include "numIll.h"
#include "numMPZ.h"
#include "numRl.h"
#include "numRll.h"
#include "numMPQ.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _NUMDyyy_MARK_

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numDyyy_set(numDyyy_t a, numDyyy_t b)
{ *a = *b; }
static inline void numDyyy_set_array(numDyyy_t* a, numDyyy_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numDyyy_t)); }
static inline void numDyyy_set_int(numDyyy_t a, long int i)
{ *a = (numDyyy_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numDyyy_init(numDyyy_t a)
{ *a = NUMDyyy_ZERO; }
static inline void numDyyy_init_array(numDyyy_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMDyyy_ZERO;
}
static inline void numDyyy_init_set(numDyyy_t a, numDyyy_t b)
{ numDyyy_set(a,b); }
static inline void numDyyy_init_set_int(numDyyy_t a, long int i)
{ numDyyy_set_int(a,i); }

static inline void numDyyy_clear(numDyyy_t a)
{}
static inline void numDyyy_clear_array(numDyyy_t* a, size_t size)
{}

static inline void numDyyy_swap(numDyyy_t a, numDyyy_t b)
{ numDyyy_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numDyyy_neg(numDyyy_t a, numDyyy_t b)
{ *a = -(*b); }
static inline void numDyyy_add(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b + *c; }
static inline void numDyyy_add_uint(numDyyy_t a, numDyyy_t b, unsigned long int c)
{ *a = *b + (numDyyy_native)c; }
static inline void numDyyy_sub(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b - *c; }
static inline void numDyyy_sub_uint(numDyyy_t a, numDyyy_t b, unsigned long int c)
{ *a = *b - (numDyyy_native)c; }
static inline void numDyyy_mul(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b * *c; }
static inline void numDyyy_div(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = *b / *c; }

static inline void numDyyy_abs(numDyyy_t a, numDyyy_t b)
{ *a = fabsyyy(*b); }
static inline void numDyyy_mul_2(numDyyy_t a, numDyyy_t b)
{ *a = ldexpyyy(*b,1); }
static inline void numDyyy_div_2(numDyyy_t a, numDyyy_t b)
{ *a = ldexpyyy(*b,-1); }
static inline void numDyyy_min(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = fminyyy(*b,*c); }
static inline void numDyyy_max(numDyyy_t a, numDyyy_t b, numDyyy_t c)
{ *a = fmaxyyy(*b,*c); }
static inline void numDyyy_floor(numDyyy_t a, numDyyy_t b)
{ *a = flooryyy(*b); }
static inline void numDyyy_ceil(numDyyy_t a, numDyyy_t b)
{ *a = ceilyyy(*b); }
static inline void numDyyy_trunc(numDyyy_t a, numDyyy_t b)
{ *a = truncyyy(*b); }
static inline void numDyyy_sqrt(numDyyy_t up, numDyyy_t down, numDyyy_t b)
{
  assert(*b>=0);
  *up = sqrtyyy(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafteryyy(*up,0);
}
static inline void numDyyy_mul_2exp(numDyyy_t a, numDyyy_t b, int c)
{ *a = ldexpyyy(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numDyyy_sgn(numDyyy_t a)
{ return (*a==NUMDyyy_ZERO ? 0 : (*a>NUMDyyy_ZERO ? 1 : -1)); }
static inline int numDyyy_cmp(numDyyy_t a, numDyyy_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numDyyy_cmp_int(numDyyy_t a, long int b)
{
  numDyyy_native bb = (numDyyy_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numDyyy_equal(numDyyy_t a, numDyyy_t b)
{ return *a==*b; }

static inline bool numDyyy_integer(numDyyy_t a)
{ return ceilyyy(*a) == *a; }
static inline int numDyyy_hash(numDyyy_t a)
{
 if (*a <= -(numDyyy_native)(INT_MAX))
    return -INT_MAX;
  else if (*a < (numDyyy_native)INT_MAX)
    return lrintyyy(*a);
  else
    return INT_MAX;
}


/* ====================================================================== */
/* Printing */
/* ====================================================================== */

#if defined(_NUMD_MARK_)
static inline void numD_print(numD_t a)
{ printf("%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline void numD_fprint(FILE* stream, numD_t a)
{ fprintf(stream,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
static inline int numD_snprint(char* s, size_t size, numD_t a)
{ return snprintf(s,size,"%.*g",NUMFLT_PRINT_PREC,*a+NUMD_ZERO); }
#elif defined(_NUMDl_MARK_)
static inline void numDl_print(numDl_t a)
{ printf("%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline void numDl_fprint(FILE* stream, numDl_t a)
{ fprintf(stream,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
static inline int numDl_snprint(char* s, size_t size, numDl_t a)
{ return snprintf(s,size,"%.*Lg",NUMFLT_PRINT_PREC,*a+NUMDl_ZERO); }
#else
#error "HERE"
#endif

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numDyyy_infty(numDyyy_t a)
{ return !isfinite(*a); }
static inline void numDyyy_set_infty(numDyyy_t a, int sgn)
{ *a = sgn>0 ? NUMDyyy_MAX : -NUMDyyy_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numDyyy_serialize(void* dst, numDyyy_t src)
{
  num_store_words8(dst,src,sizeof(numDyyy_t));
  return sizeof(numDyyy_t);
}

static inline size_t numDyyy_deserialize(numDyyy_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numDyyy_t));
  return sizeof(numDyyy_t);
}

static inline size_t numDyyy_serialized_size(numDyyy_t a)
{ return sizeof(numDyyy_t); }

static inline size_t numDyyy_serialize_array(void* dst, numDyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numDyyy_deserialize_array(numDyyy_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numDyyy_serialized_size_array(numDyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numDyyy(long int a)
{ return true; }
static inline bool llint_fits_numDyyy(long long int a)
{ return true; }
static inline bool mpz_fits_numDyyy(mpz_t a)
{ return numMPZ_fits_yyydouble(a); }
static inline bool lfrac_fits_numDyyy(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numDyyy(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numDyyy(mpq_t a)
{ return numMPQ_fits_yyydouble(a); }
static inline bool double_fits_numDyyy(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numDyyy(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numDyyy(mpfr_t a, num_internal_t intern)
{
  if (mpfr_number_p(a)){
    if (mpfr_sgn(a)==0) return true;
    else {
      mp_exp_t e = mpfr_get_exp(a);
      return e<NUMDyyy_MAX_EXP-1;
    }
  }
  else
    return false;
}
static inline bool numDyyy_fits_lint(numDyyy_t a)
{ return yyydouble_fits_numIl(*a); }
static inline bool numDyyy_fits_llint(numDyyy_t a)
{ return yyydouble_fits_numIll(*a); }
static inline bool numDyyy_fits_mpz(numDyyy_t a)
{ return true; }
static inline bool numDyyy_fits_lfrac(numDyyy_t a)
{ return yyydouble_fits_numRl(*a); }
static inline bool numDyyy_fits_llfrac(numDyyy_t a)
{ return yyydouble_fits_numRll(*a); }
static inline bool numDyyy_fits_mpq(numDyyy_t a)
{ return true; }
static inline bool numDyyy_fits_float(numDyyy_t a)
{
  int e;
  frexpyyy(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numDyyy_fits_double(numDyyy_t a)
#if defined(_NUMD_MARK_)
{ return true; }
#elif defined(_NUMDl_MARK_)
{
  int e;
  frexpyyy(*a,&e);
  return (e<DBL_MAX_EXP-1);
}
#else
#error "HERE"
#endif
static inline bool numDyyy_fits_ldouble(numDyyy_t a)
{ return true; }
static inline bool numDyyy_fits_mpfr(numDyyy_t a)
{
  int e;
  frexpyyy(*a,&e);
  return (e<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numDyyy_set_lint(numDyyy_t a, long int i, num_internal_t intern)
{ return yyydouble_set_numIl(a,&i,intern); }
static inline bool numDyyy_set_llint(numDyyy_t a, long long int i, num_internal_t intern)
{ return yyydouble_set_numIll(a,&i,intern); }
static inline bool numDyyy_set_mpz(numDyyy_t a, mpz_t b, num_internal_t intern)
{ return yyydouble_set_numMPZ(a,b,intern); }
static inline bool numDyyy_set_lfrac(numDyyy_t a, long int i, long int j, num_internal_t intern)
{
  assert(j>0);
  *a = (numDyyy_native)i/(numDyyy_native)j;
  return (-*a==(numDyyy_native)(-i)/(numDyyy_native)j);
}
static inline bool numDyyy_set_llfrac(numDyyy_t a, long long int i, long long int j, num_internal_t intern)
{
  assert(j>0);
  *a = (numDyyy_native)i/(numDyyy_native)j;
  return (-*a==(numDyyy_native)(-i)/(numDyyy_native)j);
}
static inline bool numDyyy_set_mpq(numDyyy_t a, mpq_t b, num_internal_t intern)
{ return yyydouble_set_numMPQ(a,b,intern); }

#if defined(_NUMD_MARK_)
static inline bool numD_set_double(numD_t a, double k, num_internal_t intern)
{ *a = k; return true; }
static inline bool numD_set_ldouble(numD_t a, long double k, num_internal_t intern)
{ *a = (double)k; return (long double)(*a)==k; }
#elif defined(_NUMDl_MARK_)
static inline bool numDl_set_double(numDl_t a, double k, num_internal_t intern)
{ *a = (long double)k; return true; }
static inline bool numDl_set_ldouble(numDl_t a, long double k, num_internal_t intern)
{ *a = k; return true; }
#else
#error "HERE"
#endif

static inline bool numDyyy_set_mpfr(numDyyy_t a, mpfr_t b, num_internal_t intern)
{
  *a = mpfr_get_yyyd(b,GMP_RNDU);
  return !mpfr_cmp_yyyd(b,*a);
}
static inline bool lint_set_numDyyy(long int* a, numDyyy_t b, num_internal_t intern)
{ return numIl_set_yyydouble(a,*b,intern); }
static inline bool llint_set_numDyyy(long long int* a, numDyyy_t b, num_internal_t intern)
{ return numIll_set_yyydouble(a,*b,intern); }
static inline bool mpz_set_numDyyy(mpz_t a, numDyyy_t b, num_internal_t intern)
{ return numMPZ_set_yyydouble(a,*b,intern); }
static inline bool lfrac_set_numDyyy(long int* i, long int* j, numDyyy_t b, num_internal_t intern)
{
  numRl_native s;
  bool res = numRl_set_yyydouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_numDyyy(long long int* i, long long int* j, numDyyy_t b, num_internal_t intern)
{
  numRll_native s;
  bool res = numRll_set_yyydouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_numDyyy(mpq_t a, numDyyy_t b, num_internal_t intern)
{ return numMPQ_set_yyydouble(a,*b,intern); }

#if defined(_NUMD_MARK_)
static inline bool double_set_numD(double* a, numD_t b, num_internal_t intern)
{ *a = *b; return true; }
static inline bool ldouble_set_numD(long double* a, numD_t b, num_internal_t intern)
{ *a = (long double)*b; return true; }
#elif defined(_NUMDl_MARK_)
static inline bool double_set_numDl(double* a, numDl_t b, num_internal_t intern)
{ *a = (double)*b; return (long double)(*a)==*b; }
static inline bool ldouble_set_numDl(long double* a, numDl_t b, num_internal_t intern)
{ *a = *b; return true; }
#endif

static inline bool mpfr_set_numDyyy(mpfr_t a, numDyyy_t b, num_internal_t intern)
{ return !mpfr_set_yyyd(a,*b,GMP_RNDU); }

#undef _NUMDyyy_MARK_

#ifdef __cplusplus
}
#endif

#endif
