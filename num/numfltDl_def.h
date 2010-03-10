/* ********************************************************************** */
/* numfltDl_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTDL_DEF_H_
#define _NUMFLTDL_DEF_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"

#include "numConfig.h"
#include "numintIl_def.h"
#include "numintIll_def.h"
#include "numintMPZ_def.h"
#include "numratRl_def.h"
#include "numratRll_def.h"
#include "numratMPQ_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef long double numfltDl_native;
typedef numfltDl_native numfltDl_t[1];
#define NUMFLTDL_ZERO 0.0L
#define NUMFLTDL_ONE 1.0L
#define NUMFLTDL_MANT_DIG LDBL_MANT_DIG
#define NUMFLTDL_MAX NUMFLTDL_ONE/NUMFLTDL_ZERO
#define NUMDL_MAX NUMFLTDL_ONE/NUMFLTDL_ZERO

#ifndef NUMFLTDL_PRINT_PREC
#define NUMFLTDL_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTDL_PRINT_PREC to be any other
   expression (including variable and function call).
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numfltDl_set(numfltDl_t a, numfltDl_t b)
{ *a = *b; }
static inline void numfltDl_set_array(numfltDl_t* a, numfltDl_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numfltDl_t)); }
static inline void numfltDl_set_int(numfltDl_t a, long int i)
{ *a = (numfltDl_native)i; }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numfltDl_init(numfltDl_t a)
{ *a = NUMFLTDL_ZERO; }
static inline void numfltDl_init_array(numfltDl_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) *(a[i]) = NUMFLTDL_ZERO;
}
static inline void numfltDl_init_set(numfltDl_t a, numfltDl_t b)
{ numfltDl_set(a,b); }
static inline void numfltDl_init_set_int(numfltDl_t a, long int i)
{ numfltDl_set_int(a,i); }

static inline void numfltDl_clear(numfltDl_t a)
{}
static inline void numfltDl_clear_array(numfltDl_t* a, size_t size)
{}

static inline void numfltDl_swap(numfltDl_t a, numfltDl_t b)
{ numfltDl_t t; *t=*a;*a=*b;*b=*t; }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numfltDl_neg(numfltDl_t a, numfltDl_t b)
{ *a = -(*b); }
static inline void numfltDl_add(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b + *c; }
static inline void numfltDl_add_uint(numfltDl_t a, numfltDl_t b, unsigned long int c)
{ *a = *b + (numfltDl_native)c; }
static inline void numfltDl_sub(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b - *c; }
static inline void numfltDl_sub_uint(numfltDl_t a, numfltDl_t b, unsigned long int c)
{ *a = *b - (numfltDl_native)c; }
static inline void numfltDl_mul(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b * *c; }
static inline void numfltDl_div(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = *b / *c; }

static inline void numfltDl_abs(numfltDl_t a, numfltDl_t b)
{ *a = fabsl(*b); }
static inline void numfltDl_mul_2(numfltDl_t a, numfltDl_t b)
{ *a = ldexpl(*b,1); }
static inline void numfltDl_div_2(numfltDl_t a, numfltDl_t b)
{ *a = ldexpl(*b,-1); }
static inline void numfltDl_min(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = fminl(*b,*c); }
static inline void numfltDl_max(numfltDl_t a, numfltDl_t b, numfltDl_t c)
{ *a = fmaxl(*b,*c); }
static inline void numfltDl_floor(numfltDl_t a, numfltDl_t b)
{ *a = floorl(*b); }
static inline void numfltDl_ceil(numfltDl_t a, numfltDl_t b)
{ *a = ceill(*b); }
static inline void numfltDl_trunc(numfltDl_t a, numfltDl_t b)
{ *a = truncl(*b); }
static inline void numfltDl_sqrt(numfltDl_t up, numfltDl_t down, numfltDl_t b)
{
  assert(*b>=0);
  *up = sqrtl(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafterl(*up,0);
}
static inline void numfltDl_mul_2exp(numfltDl_t a, numfltDl_t b, int c)
{ *a = ldexpl(*b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numfltDl_sgn(numfltDl_t a)
{ return (*a==NUMFLTDL_ZERO ? 0 : (*a>NUMFLTDL_ZERO ? 1 : -1)); }
static inline int numfltDl_cmp(numfltDl_t a, numfltDl_t b)
{ return (*a==*b ? 0 : (*a>*b ? 1 : -1)); }
static inline int numfltDl_cmp_int(numfltDl_t a, long int b)
{
  numfltDl_native bb = (numfltDl_native)b;
  return (*a==bb ? 0 : (*a>bb ? 1 : -1));
}
static inline bool numfltDl_equal(numfltDl_t a, numfltDl_t b)
{ return *a==*b; }

static inline bool numfltDl_integer(numfltDl_t a)
{ return ceill(*a) == *a; }
static inline int numfltDl_hash(numfltDl_t a)
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

static inline void numfltDl_print(numfltDl_t a)
{ printf("%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }
static inline void numfltDl_fprint(FILE* stream, numfltDl_t a)
{ fprintf(stream,"%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }
static inline int numfltDl_snprint(char* s, size_t size, numfltDl_t a)
{ return snprintf(s,size,"%.*Lg",NUMFLTDL_PRINT_PREC,*a+NUMFLTDL_ZERO); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numfltDl(long int a)
{ return true; }
static inline bool llint_fits_numfltDl(long long int a)
{ return true; }
static inline bool mpz_fits_numfltDl(mpz_t a)
{ return numintMPZ_fits_ldouble(a); }
static inline bool lfrac_fits_numfltDl(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numfltDl(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numfltDl(mpq_t a)
{ return numratMPQ_fits_ldouble(a); }
static inline bool double_fits_numfltDl(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numfltDl(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numfltDl(mpfr_t a, numinternal_t intern)
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

static inline bool numfltDl_fits_lint(numfltDl_t a)
{ return ldouble_fits_numintIl(*a); }
static inline bool numfltDl_fits_llint(numfltDl_t a)
{ return ldouble_fits_numintIll(*a); }
static inline bool numfltDl_fits_lfrac(numfltDl_t a)
{ return ldouble_fits_numratRl(*a); }
static inline bool numfltDl_fits_llfrac(numfltDl_t a)
{ return ldouble_fits_numratRll(*a); }
static inline bool numfltDl_fits_float(numfltDl_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numfltDl_fits_double(numfltDl_t a)
{
  int e;
  frexp(*a,&e);
  return (e<DBL_MAX_EXP-1);
}
static inline bool numfltDl_fits_ldouble(numfltDl_t a)
{ return true; }
static inline bool numfltDl_fits_mpfr(numfltDl_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<mpfr_get_emax());
}

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

/* lint -> numflt */
static inline bool numfltDl_set_lint(numfltDl_t a, long int i, numinternal_t intern)
{ return ldouble_set_numintIl(a,&i,intern); }
/* llint -> numflt */
static inline bool numfltDl_set_llint(numfltDl_t a, long long int i, numinternal_t intern)
{ return ldouble_set_numintIll(a,&i,intern); }
/* mpz -> numflt */
static inline bool numfltDl_set_mpz(numfltDl_t a, mpz_t b, numinternal_t intern)
{ return ldouble_set_numintMPZ(a,b,intern); }
/* lfrac -> numflt */
static inline bool numfltDl_set_lfrac(numfltDl_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numfltDl_native)i/(numfltDl_native)j;
  return (-*a==(numfltDl_native)(-i)/(numfltDl_native)j);
}
/* llfrac -> numflt */
static inline bool numfltDl_set_llfrac(numfltDl_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numfltDl_native)i/(numfltDl_native)j;
  return (-*a==(numfltDl_native)(-i)/(numfltDl_native)j);
}
/* mpq -> numflt */
static inline bool numfltDl_set_mpq(numfltDl_t a, mpq_t b, numinternal_t intern)
{ return ldouble_set_numratMPQ(a,b,intern); }

/* double -> numflt */
static inline bool numfltDl_set_double(numfltDl_t a, double k, numinternal_t intern)
{ *a = (long double)k; return true; }

/* long double -> numflt */
static inline bool numfltDl_set_ldouble(numfltDl_t a, long double k, numinternal_t intern)
{ *a = k; return true; }

/* mpfr -> numflt */
static inline bool numfltDl_set_mpfr(numfltDl_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_ld(b,GMP_RNDU);
  return !mpfr_cmp_ld(b,*a);
}

/* numflt -> lint */
static inline bool lint_set_numfltDl(long int* a, numfltDl_t b, numinternal_t intern)
{ return numintIl_set_ldouble(a,*b,intern); }
/* numflt -> llint */
static inline bool llint_set_numfltDl(long long int* a, numfltDl_t b, numinternal_t intern)
{ return numintIll_set_ldouble(a,*b,intern); }
/* numflt -> mpz */
static inline bool mpz_set_numfltDl(mpz_t a, numfltDl_t b, numinternal_t intern)
{ return numintMPZ_set_ldouble(a,*b,intern); }
/* numflt -> lfrac */
static inline bool lfrac_set_numfltDl(long int* i, long int* j, numfltDl_t b, numinternal_t intern)
{
  struct numratRl_native s;
  bool res = numratRl_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> llfrac */
static inline bool llfrac_set_numfltDl(long long int* i, long long int* j, numfltDl_t b, numinternal_t intern)
{
  struct numratRll_native s;
  bool res = numratRll_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> mpq */
static inline bool mpq_set_numfltDl(mpq_t a, numfltDl_t b, numinternal_t intern)
{ return numratMPQ_set_ldouble(a,*b,intern); }
/* numflt -> double */
static inline bool double_set_numfltDl(double* a, numfltDl_t b, numinternal_t intern)
{ *a = (double)*b; return true; }
/* numflt -> long double */
static inline bool ldouble_set_numfltDl(long double* a, numfltDl_t b, numinternal_t intern)
{ *a = *b; return true; }
/* numflt -> mpfr */
static inline bool mpfr_set_numfltDl(mpfr_t a, numfltDl_t b, numinternal_t intern)
{ return !mpfr_set_ld(a,*b,GMP_RNDU); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numfltDl_infty(numfltDl_t a)
{ return !isfinite(*a); }
static inline void numfltDl_set_infty(numfltDl_t a, int sgn)
{ *a = sgn>0 ? NUMFLTDL_MAX : -NUMFLTDL_MAX; }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numfltDl_serialize(void* dst, numfltDl_t src)
{
  num_store_words8(dst,src,sizeof(numfltDl_t));
  return sizeof(numfltDl_t);
}

static inline size_t numfltDl_deserialize(numfltDl_t dst, const void* src)
{
  num_store_words8(dst,src,sizeof(numfltDl_t));
  return sizeof(numfltDl_t);
}

static inline size_t numfltDl_serialized_size(numfltDl_t a)
{ return sizeof(numfltDl_t); }

#ifdef __cplusplus
}
#endif

#endif
