/* ********************************************************************** */
/* numfltD_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTD_DEF_H_
#define _NUMFLTD_DEF_H_

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

#ifdef __cplusplus
extern "C" {
#endif

typedef double numfltD_native;
typedef numfltD_native numfltD_t[1];
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

/* int2 -> numflt */
static inline bool numfltD_set_int2(numfltD_t a, long int i, long int j)
{
  assert(j>0);
  *a = (numfltD_native)i/(numfltD_native)j;
  return (-*a==(numfltD_native)(-i)/(numfltD_native)j);
}

/* mpz -> numflt */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTD_MANT_DIG bits */
static inline bool numfltD_set_mpz_tmp(numfltD_t a, mpz_t b, mpfr_t mpfr)
{
  int res = mpfr_set_z(mpfr,b,GMP_RNDU);
  *a = mpfr_get_d(mpfr,GMP_RNDU);/* Normally, exact conversion here (unless overfloww) */
  return (res==0);
}
static inline bool numfltD_set_mpz(numfltD_t a, mpz_t b)
{
  mpfr_t mpfr;
  mpfr_init2(mpfr,NUMFLTD_MANT_DIG);
  bool res = numfltD_set_mpz_tmp(a,b,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* mpq -> numflt */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTD_MANT_DIG bits */
static inline bool numfltD_set_mpq_tmp(numfltD_t a, mpq_t b, mpfr_t mpfr)
{
  int res = mpfr_set_q(mpfr,b,GMP_RNDU);
  *a = mpfr_get_d(mpfr,GMP_RNDU);/* Normally, exact conversion here (unless overfloww) */
  return (res==0);
}
static inline bool numfltD_set_mpq(numfltD_t a, mpq_t b)
{
  mpfr_t mpfr;
  mpfr_init2(mpfr,NUMFLTD_MANT_DIG);
  bool res = numfltD_set_mpq_tmp(a,b,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* double -> numflt */
static inline bool numfltD_set_double(numfltD_t a, double k)
{ *a = (numfltD_native)k; return true; }
/* mpfr -> numflt */
static inline bool numfltD_set_mpfr(numfltD_t a, mpfr_t b)
{
  *a = mpfr_get_d(b,GMP_RNDU);
  return !mpfr_cmp_d(b,*a);
}

/* numflt -> int */
static inline bool int_set_numfltD(long int* a, numfltD_t b)
{
  numfltD_native c;
  numfltD_ceil(&c,b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = (long int)c;
  return (*b==c);
}
/* numflt -> mpz */
static inline bool mpz_set_numfltD(mpz_t a, numfltD_t b)
{
  double c = ceil(*b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_ui(a,0); return false; }
  mpz_set_d(a,c);
  return (*b==(numfltD_native)c);
}
/* numflt -> mpq */
static inline bool mpq_set_numfltD(mpq_t a, numfltD_t b)
{
  if (!isfinite(*b)) { DEBUG_SPECIAL; mpq_set_ui(a,0,1); return false; }
  mpq_set_d(a,*b);
  return true;
}

/* numflt -> double */
static inline bool double_set_numfltD(double* a, numfltD_t b)
{ *a = *b; return true; }
/* numflt -> mpfr */
static inline bool mpfr_set_numfltD(mpfr_t a, numfltD_t b)
{ return !mpfr_set_d(a,*b,GMP_RNDU); }

static inline bool mpz_fits_numfltD(mpz_t a)
{
  double k = mpz_get_d(a);
  return (fabs(k)+1.0) != (double)1.0/(double)0.0;
}
static inline bool mpq_fits_numfltD(mpq_t a)
{
  double k = mpq_get_d(a);
  return (fabs(k)+1.0) != (double)1.0/(double)0.0;
}
static inline bool double_fits_numfltD(double a)
{ return true; }
static inline bool mpfr_fits_numfltD(mpfr_t a)
{ return mpfr_get_exp(a)<1022; /* XXX >1022 for long double */ }
static inline bool numfltD_fits_int(numfltD_t a)
{
  numfltD_native d;
  numfltD_ceil(&d,a);
  return isfinite(d) && d >= (numfltD_native)(-LONG_MAX) && d<= (numfltD_native)LONG_MAX;
}
static inline bool numfltD_fits_float(numfltD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<127);
}
static inline bool numfltD_fits_double(numfltD_t a)
{
  return true;
}
static inline bool numfltD_fits_mpfr(numfltD_t a)
{
  int e;
  frexp(*a,&e);
  return (e<mpfr_get_emax());
}

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
