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

#ifdef __cplusplus
extern "C" {
#endif

typedef long double numfltDl_native;
typedef numfltDl_native numfltDl_t[1];
#define NUMFLTDL_NATIVE
#define NUMFLTDL_ZERO 0.0L
#define NUMFLTDL_ONE 1.0L
#define NUMFLTDL_MANT_DIG LDBL_MANT_DIG
#define NUMFLTDL_MAX NUMFLTDL_ONE/NUMFLTDL_ZERO

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

/* int2 -> numflt */
static inline bool numfltDl_set_int2(numfltDl_t a, long int i, long int j)
{
  assert(j>0);
  *a = (numfltDl_native)i/(numfltDl_native)j;
  return (-*a==(numfltDl_native)(-i)/(numfltDl_native)j);
}

/* mpz -> numflt */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool numfltDl_set_mpz_tmp(numfltDl_t a, mpz_t b, mpfr_t mpfr)
{
  int res = mpfr_set_z(mpfr,b,GMP_RNDU);
  *a = mpfr_get_ld(mpfr,GMP_RNDU);/* Normally, exact conversion here (unless overfloww) */
  return (res==0);
}
static inline bool numfltDl_set_mpz(numfltDl_t a, mpz_t b)
{
  mpfr_t mpfr;
  mpfr_init2(mpfr,NUMFLTDL_MANT_DIG);
  bool res = numfltDl_set_mpz_tmp(a,b,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* mpq -> numflt */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool numfltDl_set_mpq_tmp(numfltDl_t a, mpq_t b, mpfr_t mpfr)
{
  int res = mpfr_set_q(mpfr,b,GMP_RNDU);
  *a = mpfr_get_ld(mpfr,GMP_RNDU);/* Normally, exact conversion here (unless overfloww) */
  return (res==0);
}
static inline bool numfltDl_set_mpq(numfltDl_t a, mpq_t b)
{
  mpfr_t mpfr;
  mpfr_init2(mpfr,NUMFLTDL_MANT_DIG);
  bool res = numfltDl_set_mpq_tmp(a,b,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* double -> numflt */
static inline bool numfltDl_set_double(numfltDl_t a, double k)
{ *a = (numfltDl_native)k; return true; }
/* mpfr -> numflt */
static inline bool numfltDl_set_mpfr(numfltDl_t a, mpfr_t b)
{
  *a = mpfr_get_ld(b,GMP_RNDU);
  return !mpfr_cmp_ld(b,*a);
}


/* numflt -> int */
static inline bool int_set_numfltDl(long int* a, numfltDl_t b)
{
  numfltDl_native c;
  numfltDl_ceil(&c,b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = (long int)c;
  return (*b==c);
}
/* numflt -> mpz */
static inline bool mpz_set_numfltDl(mpz_t a, numfltDl_t b)
{
  double c = ceil(*b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_ui(a,0); return false; }
  mpz_set_d(a,c);
  return (*b==(numfltDl_native)c);
}
/* numflt -> mpq */
static inline bool mpq_set_numfltDl(mpq_t a, numfltDl_t b)
{
  double c = (double)(*b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpq_set_ui(a,0,1); return false; }
  mpq_set_d(a,c);
  return (*b==(numfltDl_native)c);
}
/* numflt -> double */
static inline bool double_set_numfltDl(double* a, numfltDl_t b)
{ *a = *b; return ((numfltDl_native)(*a)==*b); }
/* numflt -> mpfr */
static inline bool mpfr_set_numfltDl(mpfr_t a, numfltDl_t b)
{ return !mpfr_set_ld(a,*b,GMP_RNDU); }

static inline bool mpz_fits_numfltDl(mpz_t a)
{
  double k = mpz_get_d(a);
  return (fabs(k)+1.0) != (double)1.0/(double)0.0;
}
static inline bool mpq_fits_numfltDl(mpq_t a)
{
  double k = mpq_get_d(a);
  return (fabs(k)+1.0) != (double)1.0/(double)0.0;
}
static inline bool double_fits_numfltDl(double a)
{ return true; }
static inline bool mpfr_fits_numfltDl(mpfr_t a)
{ return mpfr_get_exp(a)<1022; /* XXX >1022 for long double */ }
static inline bool numfltDl_fits_int(numfltDl_t a)
{
  numfltDl_native d;
  numfltDl_ceil(&d,a);
  return isfinite(d) && d >= (numfltDl_native)(-LONG_MAX) && d<= (numfltDl_native)LONG_MAX;
}
static inline bool numfltDl_fits_float(numfltDl_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<127);
}
static inline bool numfltDl_fits_double(numfltDl_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<1023);
}
static inline bool numfltDl_fits_mpfr(numfltDl_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<mpfr_get_emax());
}

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
