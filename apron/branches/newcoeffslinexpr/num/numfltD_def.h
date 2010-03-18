/* ********************************************************************** */
/* numfltD_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTD_DEF_H_
#define _NUMFLTD_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "numConfig.h"
#include "numflt.h"
#include "numintIl_def.h"
#include "numintIll_def.h"
#include "numintMPZ_def.h"
#include "numratRl_def.h"
#include "numratRll_def.h"
#include "numratMPQ_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUMFLTD_ZERO 0.0
#define NUMFLTD_ONE 1.0
#define NUMFLTD_MANT_DIG DBL_MANT_DIG
#define NUMFLTD_MAX NUMFLTD_ONE/NUMFLTD_ZERO
#define NUMD_MAX NUMFLTD_ONE/NUMFLTD_ZERO

#ifndef NUMFLTD_PRINT_PREC
#define NUMFLTD_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTD_PRINT_PREC to be any other
   expression (including variable and function call).
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numfltD_set(numfltD_t a, numfltD_t b)
{ *a = *b; }
static inline void numfltD_set_array(numfltD_t* a, numfltD_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numfltD_t)); }
static inline void numfltD_set_int(numfltD_t a, long int i)
{ *a = (numfltD_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numfltD_init(numfltD_t a)
{ *a = NUMFLTD_ZERO; }
static inline void numfltD_init_array(numfltD_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMFLTD_ZERO;
}
static inline void numfltD_init_set(numfltD_t a, numfltD_t b)
{ numfltD_set(a,b); }
static inline void numfltD_init_set_int(numfltD_t a, long int i)
{ numfltD_set_int(a,i); }

static inline void numfltD_clear(numfltD_t a)
{}
static inline void numfltD_clear_array(numfltD_t* a, size_t size)
{}

static inline void numfltD_swap(numfltD_t a, numfltD_t b)
{ numfltD_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numfltD_neg(numfltD_t a, numfltD_t b)
{ *a = -(*b); }
static inline void numfltD_add(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b + *c; }
static inline void numfltD_add_uint(numfltD_t a, numfltD_t b, unsigned long int c)
{ *a = *b + (numfltD_native)c; }
static inline void numfltD_sub(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b - *c; }
static inline void numfltD_sub_uint(numfltD_t a, numfltD_t b, unsigned long int c)
{ *a = *b - (numfltD_native)c; }
static inline void numfltD_mul(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b * *c; }
static inline void numfltD_div(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = *b / *c; }

static inline void numfltD_abs(numfltD_t a, numfltD_t b)
{ *a = fabs(*b); }
static inline void numfltD_mul_2(numfltD_t a, numfltD_t b)
{ *a = ldexp(*b,1); }
static inline void numfltD_div_2(numfltD_t a, numfltD_t b)
{ *a = ldexp(*b,-1); }
static inline void numfltD_min(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = fmin(*b,*c); }
static inline void numfltD_max(numfltD_t a, numfltD_t b, numfltD_t c)
{ *a = fmax(*b,*c); }
static inline void numfltD_floor(numfltD_t a, numfltD_t b)
{ *a = floor(*b); }
static inline void numfltD_ceil(numfltD_t a, numfltD_t b)
{ *a = ceil(*b); }
static inline void numfltD_trunc(numfltD_t a, numfltD_t b)
{ *a = trunc(*b); }
static inline void numfltD_sqrt(numfltD_t up, numfltD_t down, numfltD_t b)
{
  numfltD_t x;
  assert(*b>=0);
  *x = sqrt(*b);
  assert(*x**x>=*b); /* assumes round towards +oo! */
  if (*x**x==*b) *down = *x;
  else *down = nextafter(*x,0);
  *up = *x;
}
static inline void numfltD_mul_2exp(numfltD_t a, numfltD_t b, int c)
{ *a = ldexp(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numfltD_sgn(numfltD_t a)
{ return (*a==NUMFLTD_ZERO ? 0 : (*a>NUMFLTD_ZERO ? 1 : -1)); }
static inline int numfltD_cmp(numfltD_t a, numfltD_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numfltD_cmp_int(numfltD_t a, long int b)
{
  numfltD_native bb = (numfltD_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numfltD_equal(numfltD_t a, numfltD_t b)
{ return *a==*b; }

static inline bool numfltD_integer(numfltD_t a)
{ return ceil(*a) == *a; }
static inline int numfltD_hash(numfltD_t a)
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

static inline void numfltD_print(numfltD_t a)
{ printf("%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }
static inline void numfltD_fprint(FILE* stream, numfltD_t a)
{ fprintf(stream,"%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }
static inline int numfltD_snprint(char* s, size_t size, numfltD_t a)
{ return snprintf(s,size,"%.*g",NUMFLTD_PRINT_PREC,*a+NUMFLTD_ZERO); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numfltD(long int a)
{ return true; }
static inline bool llint_fits_numfltD(long long int a)
{ return true; }
static inline bool mpz_fits_numfltD(mpz_t a)
{ return numintMPZ_fits_double(a); }
static inline bool lfrac_fits_numfltD(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numfltD(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numfltD(mpq_t a)
{ return numratMPQ_fits_double(a); }
static inline bool double_fits_numfltD(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numfltD(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numfltD(mpfr_t a, numinternal_t intern)
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

static inline bool numfltD_fits_lint(numfltD_t a)
{ return double_fits_numintIl(*a); }
static inline bool numfltD_fits_llint(numfltD_t a)
{ return double_fits_numintIll(*a); }
static inline bool numfltD_fits_lfrac(numfltD_t a)
{ return double_fits_numratRl(*a); }
static inline bool numfltD_fits_llfrac(numfltD_t a)
{ return double_fits_numratRll(*a); }
static inline bool numfltD_fits_float(numfltD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numfltD_fits_double(numfltD_t a)
{ return true; }
static inline bool numfltD_fits_ldouble(numfltD_t a)
{ return true; }
static inline bool numfltD_fits_mpfr(numfltD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<mpfr_get_emax());
}

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

/* lint -> numflt */
static inline bool numfltD_set_lint(numfltD_t a, long int i, numinternal_t intern)
{ return double_set_numintIl(a,&i,intern); }
/* llint -> numflt */
static inline bool numfltD_set_llint(numfltD_t a, long long int i, numinternal_t intern)
{ return double_set_numintIll(a,&i,intern); }
/* mpz -> numflt */
static inline bool numfltD_set_mpz(numfltD_t a, mpz_t b, numinternal_t intern)
{ return double_set_numintMPZ(a,b,intern); }
/* lfrac -> numflt */
static inline bool numfltD_set_lfrac(numfltD_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numfltD_native)i/(numfltD_native)j;
  return (-*a==(numfltD_native)(-i)/(numfltD_native)j);
}
/* llfrac -> numflt */
static inline bool numfltD_set_llfrac(numfltD_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numfltD_native)i/(numfltD_native)j;
  return (-*a==(numfltD_native)(-i)/(numfltD_native)j);
}
/* mpq -> numflt */
static inline bool numfltD_set_mpq(numfltD_t a, mpq_t b, numinternal_t intern)
{ return double_set_numratMPQ(a,b,intern); }

/* double -> numflt */
static inline bool numfltD_set_double(numfltD_t a, double k, numinternal_t intern)
{ *a = k; return true; }

/* long double -> numflt */
static inline bool numfltD_set_ldouble(numfltD_t a, long double k, numinternal_t intern)
{ *a = (double)k; return true; }

/* mpfr -> numflt */
static inline bool numfltD_set_mpfr(numfltD_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_d(b,GMP_RNDU);
  return !mpfr_cmp_d(b,*a);
}

/* numflt -> lint */
static inline bool lint_set_numfltD(long int* a, numfltD_t b, numinternal_t intern)
{ return numintIl_set_double(a,*b,intern); }
/* numflt -> llint */
static inline bool llint_set_numfltD(long long int* a, numfltD_t b, numinternal_t intern)
{ return numintIll_set_double(a,*b,intern); }
/* numflt -> mpz */
static inline bool mpz_set_numfltD(mpz_t a, numfltD_t b, numinternal_t intern)
{ return numintMPZ_set_double(a,*b,intern); }
/* numflt -> lfrac */
static inline bool lfrac_set_numfltD(long int* i, long int* j, numfltD_t b, numinternal_t intern)
{
  struct numratRl_native s;
  bool res = numratRl_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> llfrac */
static inline bool llfrac_set_numfltD(long long int* i, long long int* j, numfltD_t b, numinternal_t intern)
{
  struct numratRll_native s;
  bool res = numratRll_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> mpq */
static inline bool mpq_set_numfltD(mpq_t a, numfltD_t b, numinternal_t intern)
{ return numratMPQ_set_double(a,*b,intern); }
/* numflt -> double */
static inline bool double_set_numfltD(double* a, numfltD_t b, numinternal_t intern)
{ *a = *b; return true; }
/* numflt -> long double */
static inline bool ldouble_set_numfltD(long double* a, numfltD_t b, numinternal_t intern)
{ *a = (long double)*b; return true; }
/* numflt -> mpfr */
static inline bool mpfr_set_numfltD(mpfr_t a, numfltD_t b, numinternal_t intern)
{ return !mpfr_set_d(a,*b,GMP_RNDU); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numfltD_infty(numfltD_t a)
{ return !isfinite(*a); }
static inline void numfltD_set_infty(numfltD_t a, int sgn)
{ *a = sgn>0 ? NUMFLTD_MAX : -NUMFLTD_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numfltD_serialize(void* dst, numfltD_t src)
{
  num_store_words8(dst,src,sizeof(numfltD_t));
  return sizeof(numfltD_t);
}

static inline size_t numfltD_deserialize(numfltD_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numfltD_t));
  return sizeof(numfltD_t);
}

static inline size_t numfltD_serialized_size(numfltD_t a)
{ return sizeof(numfltD_t); }

#ifdef __cplusplus
}
#endif

#endif
