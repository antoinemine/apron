/* ********************************************************************** */
/* num_conv.h: conversion between nums */
/* ********************************************************************** */

#ifndef _NUM_CONV_H_
#define _NUM_CONV_H_

#include "numConfig.h"
#include "num_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a);
static inline bool llint_fits_num(long long int a);
static inline bool mpz_fits_num(mpz_t a);
static inline bool lfrac_fits_num(long int i, long int j);
static inline bool llfrac_fits_num(long long int i, long long int j);
static inline bool mpq_fits_num(mpq_t a);
static inline bool double_fits_num(double a);
static inline bool ldouble_fits_num(long double a);
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern);

static inline bool num_fits_lint(num_t a);
static inline bool num_fits_llint(num_t a);
static inline bool num_fits_lfrac(num_t a);
static inline bool num_fits_llfrac(num_t a);
static inline bool num_fits_float(num_t a);
static inline bool num_fits_double(num_t a);
static inline bool num_fits_ldouble(num_t a);
static inline bool num_fits_mpfr(num_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern);
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern);
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern);
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern);
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern);
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern);
static inline bool num_set_double(num_t a, double b, numinternal_t intern);
static inline bool num_set_ldouble(num_t a, long double b, numinternal_t intern);
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern);

static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern);
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern);
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern);
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern);
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern);
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern);
static inline bool double_set_num(double* a, num_t b, numinternal_t intern);
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern);
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern);

static inline bool num_set_numIl(num_t a, numIl_t b, numinternal_t internal);
static inline bool num_set_numIll(num_t a, numIll_t b, numinternal_t internal);
static inline bool num_set_numMPZ(num_t a, numMPZ_t b, numinternal_t internal);
static inline bool num_set_numRl(num_t a, numRl_t b, numinternal_t internal);
static inline bool num_set_numRll(num_t a, numRll_t b, numinternal_t internal);
static inline bool num_set_numD(num_t a, numD_t b, numinternal_t internal);
static inline bool num_set_numDl(num_t a, numDl_t b, numinternal_t internal);
static inline bool num_set_numMPFR(num_t a, numMPFR_t b, numinternal_t internal);

/* ********************************************************************** */
/* NUMIL inline definitions */
/* ********************************************************************** */

#if NUM_NUMIL

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return a>=-NUMINTIL_MAX && a<=NUMINTIL_MAX; }
static inline bool mpz_fits_num(mpz_t a)
{ return mpz_fits_slong_p(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{
  return llint_fits_num(i>=0 ? (i+j-1)/j : i/j);
}
static inline bool lfrac_fits_numint(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numint(long long int i, long long int j)
{
  return llint_fits_num(i>=0 ? (i+j-1)/j : i/j);
}
static inline bool mpq_fits_num(mpq_t a)
{
  size_t n = mpz_sizeinbase(mpq_numref(a),2);
  size_t d = mpz_sizeinbase(mpq_denref(a),2);
  return ((int)n - (int)d) <= (int)(sizeof(num_t)*8-3);
}
static inline bool double_fits_num(double a)
{
  return isfinite(a) && a>=(double)(-NUMINTIL_MAX) && a<=(double)NUMINTIL_MAX;
}
static inline bool ldouble_fits_num(long double a)
{
  return isfinite(a) && a>=(long double)(-NUMINTIL_MAX) && a<=(long double)NUMINTIL_MAX;
}
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{
  return mpfr_number_p(a) && mpfr_fits_slong_p(a,GMP_RNDU);
}

static inline bool num_fits_lint(num_t a)
{ return true; }
static inline bool num_fits_llint(num_t a)
{ return true; }
static inline bool num_fits_lfrac(num_t a)
{ return true; }
static inline bool num_fits_llfrac(num_t a)
{ return true; }
static inline bool num_fits_float(num_t a)
{ return true; }
static inline bool num_fits_double(num_t a)
{ return true; }
static inline bool num_fits_ldouble(num_t a)
{ return true; }
static inline bool num_fits_mpfr(num_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{ *a = b; return true; }
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{
  *a = (long int)b;
  return true;
}
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{
  *a = mpz_get_si(b);
  return true;
}
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j) ? false : true;
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  long long int t;
  assert(j>0);
  t = i>=0 ? (i+j-1)/j : i/j;
  return num_set_llint(a,t,intern) && i%j==0;
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(intern->q, intern->r, mpq_numref(b),mpq_denref(b));
  num_set_mpz(a,intern->q,intern);
  return mpz_sgn(intern->r)==0;
}
static inline bool num_set_double(num_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool num_set_ldouble(num_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return b==c;
}
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  *a = mpfr_get_si(b,GMP_RNDU);
  return mpfr_integer_p(b);
}

static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ *a = (long long int)(*b); return true; }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ mpz_set_si(a,*b); return true; }
static inline bool lfrac_set_num(long int *a, long int* b, num_t c, numinternal_t intern)
{
  *a = *c;
  *b = 1L;
  return true;
}
static inline bool llfrac_set_num(long long int *a, long long int* b, num_t c, numinternal_t intern)
{
  *a = (long long int)(*c);
  *b = 1LL;
  return true;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{
  mpq_set_si(a,*b,1);
  return true;
}
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{
  return !mpfr_set_si(a,*b,GMP_RNDU);
}

/* ********************************************************************** */
/* NUMILL inline definitions */
/* ********************************************************************** */

#elif NUM_NUMILL

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return true; }
static inline bool mpz_fits_num(mpz_t a)
{
  size_t size = mpz_sizeinbase(a,2);
  return (size <= sizeof(num_t)*8-1);
}
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_num(mpq_t a)
{
  size_t n = mpz_sizeinbase(mpq_numref(a),2);
  size_t d = mpz_sizeinbase(mpq_denref(a),2);
  return ((int)n - (int)d) <= (int)(sizeof(num_t)*8-3);
}
static inline bool double_fits_num(double a)
{
  return isfinite(a) && a>=(double)(-NUMINTILL_MAX) && a<=(double)NUMINTILL_MAX;
}
static inline bool ldouble_fits_num(long double a)
{
  return isfinite(a) && a>=(long double)(-NUMINTILL_MAX) && a<=(long double)NUMINTILL_MAX;
}
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{
  return mpfr_number_p(a) && mpfr_fits_intmax_p(a,GMP_RNDU);
}

static inline bool num_fits_lint(num_t a)
{ return (*a>=-LONG_MAX && *a<=LONG_MAX); }
static inline bool num_fits_llint(num_t a)
{ return true; }
static inline bool num_fits_lfrac(num_t a)
{ return (*a>=-LONG_MAX && *a<=LONG_MAX); }
static inline bool num_fits_llfrac(num_t a)
{ return true; }
static inline bool num_fits_float(num_t a)
{ return true; }
static inline bool num_fits_double(num_t a)
{ return true; }
static inline bool num_fits_ldouble(num_t a)
{ return true; }
static inline bool num_fits_mpfr(num_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{
  *a = (long long int)b;
  return true;
}
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{
  *a = b;
  return true;
}
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
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
    *a = tab[0];
    if (count==2){
      *a = *a << (sizeof(long int)*8);
      *a = *a + (long long int)(tab[1]);
      assert(*a>=0LL);
      count = mpz_sizeinbase(b,2);
      res = count <= sizeof(num_t)*8-1;
    }
    else {
      res = true;
    }
    if (sgn<0) *a = -(*a);
  }
  return res;
}
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  if (i>=0) *a = (i+j-1)/j;
  else *a = i/j;
  return (i%j==0);
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(intern->q,intern->r, mpq_numref(b),mpq_denref(b));
  return num_set_mpz(a,intern->q,intern) && (mpz_sgn(intern->r)==0);
}
static inline bool num_set_double(num_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool num_set_ldouble(num_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = c;
  return (b==c);
}
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  *a = mpfr_get_sj(b,GMP_RNDU);
  return mpfr_integer_p(b);
}
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ *a = (long int)(*b); return true; }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{
  unsigned long long int n;
  unsigned long int rep[2];

  n = llabs(*b);
  rep[1] = n & ULONG_MAX;
  rep[0] = n >> (sizeof(long int)*8);
  mpz_import(a,2,1,sizeof(unsigned long int),0,0,rep);
  if (*b<0)
    mpz_neg(a,a);
  return true;
}
static inline bool lfrac_set_num(long int* a, long int* b, num_t c, numinternal_t intern)
{
  *a = (long int)(*c);
  *b = 1L;
  return true;
}
static inline bool llfrac_set_num(long long int* a, long long int* b, num_t c, numinternal_t intern)
{
  *a = *c;
  *b = 1LL;
  return true;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{
  mpz_set_ui(mpq_denref(a),1);
  return mpz_set_num(mpq_numref(a),b,intern);
}
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{
  *a = (double)(*b);
  double aa = -((double)(-(*b)));
  return (*a==aa);
}
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{
  *a = (long double)(*b);
  long double aa = -((long double)(-(*b)));
  return (*a==aa);
}
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{
  return !mpfr_set_sj(a,*b,GMP_RNDU);
}

/* ********************************************************************** */
/* NUMMPZ inline definitions */
/* ********************************************************************** */

#elif NUM_NUMMPZ
#include "numIll_conv.h"

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return true; }
static inline bool mpz_fits_num(mpz_t a)
{ return true; }
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_num(mpq_t a)
{ return true; }
static inline bool double_fits_num(double a)
{ return isfinite(a); }
static inline bool ldouble_fits_num(long double a)
{ return isfinite(a); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{ return mpfr_number_p(a); }

static inline bool num_fits_lint(num_t a)
{ return mpz_fits_slong_p(a); }
static inline bool num_fits_llint(num_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool num_fits_lfrac(num_t a)
{ return mpz_fits_slong_p(a); }
static inline bool num_fits_llfrac(num_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool num_fits_float(num_t a)
{ return (mpz_sizeinbase(a,2)<FLT_MAX_EXP-1); }
static inline bool num_fits_double(num_t a)
{ return (mpz_sizeinbase(a,2)<DBL_MAX_EXP-1); }
static inline bool num_fits_ldouble(num_t a)
{ return (mpz_sizeinbase(a,2)<LDBL_MAX_EXP-1); }
static inline bool num_fits_mpfr(num_t a)
{ return (mpz_sizeinbase(a,2)+1<(size_t)mpfr_get_emax()); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{ mpz_set_si(a,b); return true; }
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{ return mpz_set_numIll(a,&b,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{ 
  long int q = (i>=0) ? (i+j-1)/j : i/j;
  mpz_set_si(a,q);
  return (i%j==0);
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{ 
  long long int q = (i>=0) ? (i+j-1)/j : i/j;
  return num_set_llint(a,q,intern) && (i%j==0);
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(a, intern->r, mpq_numref(b),mpq_denref(b));
  return mpz_sgn(intern->r)==0;
}
static inline bool num_set_double(num_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  mpz_set_d(a,c);
  return (b==c);
}
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool num_set_ldouble(num_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  int res = mpfr_set_ld(intern->ldbl,b,GMP_RNDU);
  mpfr_get_z(a,intern->ldbl,GMP_RNDU);
  return (res==0) && (b==c);
}
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  mpfr_get_z(a,b,GMP_RNDU);
  return mpfr_integer_p(b);
}

static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ *a = mpz_get_si(b); return true; }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return numIll_set_mpz(a,b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{ *i = mpz_get_si(b); *j = 1L; return true; }
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{ *j = 1LL; return llint_set_num(i,b,intern); }
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{
  mpz_set(mpq_numref(a),b);
  mpz_set_ui(mpq_denref(a),1);
  return true;
}
/* mpfr is supposed to have exactly the IEEE754 double precision of 53 bits */
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->dbl,b,GMP_RNDU);
  *a = mpfr_get_d(intern->dbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->ldbl,b,GMP_RNDU);
  *a = mpfr_get_ld(intern->ldbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return !mpfr_set_z(a,b,GMP_RNDU); }

#endif

/* ********************************************************************** */
/* Additional inline definitions */
/* ********************************************************************** */

#if NUM_NUMIL | NUM_NUMILL
static inline bool lint_fits_numint(long int a)
{ return lint_fits_num(a); }
static inline bool llint_fits_numint(long long int a)
{ return llint_fits_num(a); }
static inline bool mpz_fits_numint(mpz_t a)
{ return mpz_fits_num(a); }
static inline bool numint_fits_lint(numint_t a)
{ return num_fits_lint(a); }
static inline bool numint_fits_llint(numint_t a)
{ return num_fits_llint(a); }
static inline bool numint_fits_mpfr(numint_t a)
{ return num_fits_mpfr(a); }

static inline bool numint_set_lint(numint_t a, long int b, numinternal_t intern)
{ return num_set_lint(a,b,intern); }
static inline bool numint_set_llint(numint_t a, long long int b, numinternal_t intern)
{ return num_set_llint(a,b,intern); }
static inline bool numint_set_mpz(numint_t a, mpz_t b, numinternal_t intern)
{ return num_set_mpz(a,b,intern); }
static inline bool numint_set_double(numint_t a, double b, numinternal_t intern)
{ return num_set_double(a,b,intern); }
static inline bool numint_set_ldouble(numint_t a, long double b, numinternal_t intern)
{ return num_set_ldouble(a,b,intern); }
static inline bool lint_set_numint(long int* a, numint_t b, numinternal_t intern)
{ return lint_set_num(a,b,intern); }
static inline bool llint_set_numint(long long int* a, numint_t b, numinternal_t intern)
{ return llint_set_num(a,b,intern); }
static inline bool mpz_set_numint(mpz_t a, numint_t b, numinternal_t intern)
{ return mpz_set_num(a,b,intern); }
#endif

/* ********************************************************************** */
/* NUMRL, NUMRLL inline definitions */
/* ********************************************************************** */

#if NUM_NUMRL || NUM_NUMRLL
#if NUM_NUMRL
#include "numIl_conv.h"
#else
#include "numIll_conv.h"
#endif

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_num(long int a)
{ return lint_fits_numint(a); }
static inline bool llint_fits_num(long long int a)
{ return llint_fits_numint(a); }
static inline bool mpz_fits_num(mpz_t a)
{ return mpz_fits_numint(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{ return lint_fits_numint(i) && lint_fits_numint(j); }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return llint_fits_numint(i) && llint_fits_numint(j); }
static inline bool mpq_fits_num(mpq_t a)
{ return mpz_fits_numint(mpq_numref(a)) && mpz_fits_numint(mpq_denref(a)); }
static inline bool double_fits_num(double k)
{
  double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexp(k,&e);
  return e < (int)sizeof(numint_t)*8-1;
}
static inline bool ldouble_fits_num(long double k)
{
  long double mant;
  int e;
  if (!isfinite(k)) return false;
  mant = frexpl(k,&e);
  return e < (int)sizeof(numint_t)*8-1;
}
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(a)) return false;
  if (mpfr_sgn(a)==0) return true;
  e = mpfr_get_exp(a);
  return e < (mp_exp_t)sizeof(numint_t)*8-1;
}
static inline bool num_fits_lint(num_t a)
{
  numint_t b;
  numint_cdiv_q(b,a->n,a->d);
  return *b<=LONG_MAX && *b>=-LONG_MAX;
}
static inline bool num_fits_llint(num_t a)
{ return true; }
static inline bool num_fits_lfrac(num_t a)
{ return numint_fits_lint(numrat_numref(a)) && numint_fits_lint(numrat_denref(a)); }
static inline bool num_fits_llfrac(num_t a)
{ return true; }
static inline bool num_fits_mpq(num_t a)
{ return true; }
static inline bool num_fits_float(num_t a)
{ return true; }
static inline bool num_fits_double(num_t a)
{ return true; }
static inline bool num_fits_ldouble(num_t a)
{ return true; }
static inline bool num_fits_mpfr(num_t a)
{ return numint_fits_mpfr(numrat_numref(a)) && numint_fits_mpfr(numrat_denref(a)); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{
  numint_set_lint(a->n,b,intern);
  numint_set_int(a->d,1L);
  return true;
}
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{
  numint_set_llint(a->n,b,intern);
  numint_set_int(a->d,1);
  return true;
}
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{
  numint_set_mpz(a->n,b,intern);
  numint_set_int(a->d,1);
  return true;
}
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numint_set_lint(a->n,i,intern);
  numint_set_lint(a->d,j,intern);
  numrat_canonicalize(a);
  return true;
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numint_set_llint(a->n,i,intern);
  numint_set_llint(a->d,j,intern);
  numrat_canonicalize(a);
  return true;
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{
  numint_set_mpz(a->n,mpq_numref(b),intern);
  numint_set_mpz(a->d,mpq_denref(b),intern);
  numrat_canonicalize(a);
  return true;
}
static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{
  const int size = sizeof(numint_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  if (k==0.0){
    num_set_int(a,0);
    return true;
  }
  k = frexp(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMINT_ONE;
      *a->d = NUMINT_MAX;
    }
    else {
      num_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexp(k,l);
    res = numint_set_double(a->n,k,intern);
    *a->d = NUMINT_ONE << (l-e);
    numrat_canonicalize(a);
    return res;
  }
  else {
    num_set_int(a,0);
    return false;
  }
}
static inline bool num_set_ldouble(num_t a, long double k, numinternal_t intern)
{
  const int size = sizeof(numint_t)*8-2;
  int e,l;
  bool res;

  if (!isfinite(k)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  if (k==0.0){
    num_set_int(a,0);
    return true;
  }
  k = frexpl(k,&e);
  if (e < -size){
    if (k>0.0){
      *a->n = NUMINT_ONE;
      *a->d = NUMINT_MAX;
    }
    else {
      num_set_int(a,0);
    }
    return false;
  }
  else if (size-e>=0) {
    l = (e>=0) ? size : (size + e);
    k = ldexpl(k,l);
    res = numint_set_ldouble(a->n,k,intern);
    *a->d = NUMINT_ONE << (l-e);
    numrat_canonicalize(a);
    return res;
  }
  else {
    num_set_int(a,0);
    return false;
  }
}
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  long double k = mpfr_get_ld(b, GMP_RNDU);
  return num_set_ldouble(a,k,intern) && (mpfr_cmp_ld(b,k)==0);
}
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  return lint_set_numint(a,q,intern) && (numint_sgn(r)==0);
}
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  return llint_set_numint(a,q,intern) && (numint_sgn(r)==0);
}
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{
  numint_t q,r;
  numint_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  mpz_set_numint(a,q,intern);
  return (numint_sgn(r)==0);
}
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{
  lint_set_numint(i,numrat_numref(b),intern);
  lint_set_numint(j,numrat_denref(b),intern);
  return true;
}
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{
  llint_set_numint(i,numrat_numref(b),intern);
  llint_set_numint(j,numrat_denref(b),intern);
  return true;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{
  mpz_set_numint(mpq_numref(a), numrat_numref(b),intern);
  mpz_set_numint(mpq_denref(a), numrat_denref(b),intern);
  return true;
}
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{
  *a = (double)(*b->n)/(double)(*b->d);
  return (-*a==(double)(-*b->n)/(double)(*b->d));
}
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{
  *a = (long double)(*b->n)/(long double)(*b->d);
  return (-*a==(long double)(-*b->n)/(long double)(*b->d));
}
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{
  int r = mpfr_set_si(a,*numrat_numref(b),GMP_RNDU);
  return !mpfr_div_si(a,a,*numrat_denref(b),GMP_RNDU) && !r;
}

/* ********************************************************************** */
/* NUMMPQ inline definitions */
/* ********************************************************************** */

#elif NUM_NUMMPQ
#include "numIl_conv.h"
#include "numIll_conv.h"
#include "numMPZ_conv.h"
#include "numRl_conv.h"
#include "numRll_conv.h"

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return true; }
static inline bool mpz_fits_num(mpz_t a)
{ return true; }
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_num(mpq_t a)
{ return true; }
static inline bool double_fits_num(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_num(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{ return mpfr_number_p(a); }

static inline bool num_fits_lint(num_t a)
{ return mpq_fits_numIl(a); }
static inline bool num_fits_llint(num_t a)
{ return mpq_fits_numIll(a); }
static inline bool num_fits_lfrac(num_t a)
{ return mpq_fits_numRl(a); }
static inline bool num_fits_llfrac(num_t a)
{ return mpq_fits_numRll(a); }
static inline bool num_fits_float(num_t a)
{
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)<FLT_MAX_EXP-2);
}
static inline bool num_fits_double(num_t a)
{
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)<DBL_MAX_EXP-2);
}
static inline bool num_fits_ldouble(num_t a)
{
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)<LDBL_MAX_EXP-2);
}
static inline bool num_fits_mpfr(num_t a)
{
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)+1<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{ return mpq_set_numIl(a,&b,intern); }
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{ return mpq_set_numIll(a,&b,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ return mpq_set_numMPZ(a,b,intern); }
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{ mpq_set_si(a,i,j); return true; }
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  numMPZ_set_llint(mpq_numref(a),i,intern);
  numMPZ_set_llint(mpq_denref(a),j,intern);
  mpq_canonicalize(a);
  return true;
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{ mpq_set(a,b); return true; }
static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{
  if (!isfinite(k)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  mpq_set_d(a,k);
  return true;
}
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; num_set_int(a,0); return false; }
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(mpq_numref(a),b);
  mpz_set_si(mpq_denref(a),1);
  if (e>0) mpq_mul_2exp(a,a,e);
  if (e<0) mpq_div_2exp(a,a,-e);
  return true;
}
static inline bool num_set_ldouble(num_t a, long double b, numinternal_t intern)
{
  mp_exp_t e;
  if (!isfinite(b)) { DEBUG_SPECIAL; mpq_set_si(a,0,1); return false; }
  int res = mpfr_set_ld(intern->ldbl,b,GMP_RNDU);
  num_set_mpfr(a,intern->ldbl,intern);
  return (res==0);
}
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ return numIl_set_mpq(a,b,intern); }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return numIll_set_mpq(a,b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ return numMPZ_set_mpq(a,b,intern); }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{
  numIl_set_mpz(i,mpq_numref(b),intern);
  numIl_set_mpz(j,mpq_denref(b),intern);
  return true;
}
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{
  numIll_set_mpz(i,mpq_numref(b),intern);
  numIll_set_mpz(j,mpq_denref(b),intern);
  return true;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{ mpq_set(a,b); return true; }
/* mpfr should have exactly a precision of 53 bits */
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{
  int res = mpfr_set_q(intern->dbl,b,GMP_RNDU);
  *a = mpfr_get_d(intern->dbl,GMP_RNDU); /* should be exact */
  return (res==0);
}
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{
  int res = mpfr_set_q(intern->ldbl,b,GMP_RNDU);
  *a = mpfr_get_ld(intern->ldbl,GMP_RNDU); /* should be exact */
  return (res==0);
}
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return !mpfr_set_q(a,b,GMP_RNDU); }

/* ********************************************************************** */
/* NUMD inline definitions */
/* ********************************************************************** */

#elif NUM_NUMD
#include "numIl_conv.h"
#include "numIll_conv.h"
#include "numMPZ_conv.h"
#include "numRl_conv.h"
#include "numRll_conv.h"
#include "numMPQ_conv.h"

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return true; }
static inline bool mpz_fits_num(mpz_t a)
{ return numMPZ_fits_double(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_num(mpq_t a)
{ return numMPQ_fits_double(a); }
static inline bool double_fits_num(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_num(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
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
static inline bool num_fits_lint(num_t a)
{ return double_fits_numIl(*a); }
static inline bool num_fits_llint(num_t a)
{ return double_fits_numIll(*a); }
static inline bool num_fits_lfrac(num_t a)
{ return double_fits_numRl(*a); }
static inline bool num_fits_llfrac(num_t a)
{ return double_fits_numRll(*a); }
static inline bool num_fits_float(num_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool num_fits_double(num_t a)
{ return true; }
static inline bool num_fits_ldouble(num_t a)
{ return true; }
static inline bool num_fits_mpfr(num_t a)
{
  int e;
  frexp(*a,&e);
  return (e<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int i, numinternal_t intern)
{ return double_set_numIl(a,&i,intern); }
static inline bool num_set_llint(num_t a, long long int i, numinternal_t intern)
{ return double_set_numIll(a,&i,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ return double_set_numMPZ(a,b,intern); }
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numflt_native)i/(numflt_native)j;
  return (-*a==(numflt_native)(-i)/(numflt_native)j);
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numflt_native)i/(numflt_native)j;
  return (-*a==(numflt_native)(-i)/(numflt_native)j);
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{ return double_set_numMPQ(a,b,intern); }
static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{ *a = k; return true; }
static inline bool num_set_ldouble(num_t a, long double k, numinternal_t intern)
{ *a = (double)k; return true; }
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_d(b,GMP_RNDU);
  return !mpfr_cmp_d(b,*a);
}
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ return numIl_set_double(a,*b,intern); }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return numIll_set_double(a,*b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ return numMPZ_set_double(a,*b,intern); }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{
  numratRl_native s;
  bool res = numRl_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{
  numratRll_native s;
  bool res = numRll_set_double(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{ return numMPQ_set_double(a,*b,intern); }
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{ *a = (long double)*b; return true; }
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return !mpfr_set_d(a,*b,GMP_RNDU); }

/* ********************************************************************** */
/* NUMDL inline definitions */
/* ********************************************************************** */

#elif NUM_NUMDL
#include "numIl_conv.h"
#include "numIll_conv.h"
#include "numMPZ_conv.h"
#include "numRl_conv.h"
#include "numRll_conv.h"
#include "numMPQ_conv.h"

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return true; }
static inline bool llint_fits_num(long long int a)
{ return true; }
static inline bool mpz_fits_num(mpz_t a)
{ return numMPZ_fits_ldouble(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_num(mpq_t a)
{ return numMPQ_fits_ldouble(a); }
static inline bool double_fits_num(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_num(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
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

static inline bool num_fits_lint(num_t a)
{ return ldouble_fits_numIl(*a); }
static inline bool num_fits_llint(num_t a)
{ return ldouble_fits_numIll(*a); }
static inline bool num_fits_lfrac(num_t a)
{ return ldouble_fits_numRl(*a); }
static inline bool num_fits_llfrac(num_t a)
{ return ldouble_fits_numRll(*a); }
static inline bool num_fits_float(num_t a)
{
  int e;
  frexp(*a,&e);
  return (e<FLT_MAX_EXP-1);
}
static inline bool num_fits_double(num_t a)
{
  int e;
  frexp(*a,&e);
  return (e<DBL_MAX_EXP-1);
}
static inline bool num_fits_ldouble(num_t a)
{ return true; }
static inline bool num_fits_mpfr(num_t a)
{
  int e;
  frexpl(*a,&e);
  return (e<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int i, numinternal_t intern)
{ return ldouble_set_numIl(a,&i,intern); }
static inline bool num_set_llint(num_t a, long long int i, numinternal_t intern)
{ return ldouble_set_numIll(a,&i,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ return ldouble_set_numMPZ(a,b,intern); }
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numflt_native)i/(numflt_native)j;
  return (-*a==(numflt_native)(-i)/(numflt_native)j);
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  *a = (numflt_native)i/(numflt_native)j;
  return (-*a==(numflt_native)(-i)/(numflt_native)j);
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{ return ldouble_set_numMPQ(a,b,intern); }
static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{ *a = (long double)k; return true; }
static inline bool num_set_ldouble(num_t a, long double k, numinternal_t intern)
{ *a = k; return true; }
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{
  *a = mpfr_get_ld(b,GMP_RNDU);
  return !mpfr_cmp_ld(b,*a);
}
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ return numIl_set_ldouble(a,*b,intern); }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return numIll_set_ldouble(a,*b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ return numMPZ_set_ldouble(a,*b,intern); }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{
  numratRl_native s;
  bool res = numRl_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{
  numratRll_native s;
  bool res = numRll_set_ldouble(&s,*b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{ return numMPQ_set_ldouble(a,*b,intern); }
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{ *a = (double)*b; return true; }
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{ *a = *b; return true; }
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return !mpfr_set_ld(a,*b,GMP_RNDU); }

/* ********************************************************************** */
/* NUMMPFR inline definitions */
/* ********************************************************************** */

#elif NUM_NUMMPFR
#include "numIl_conv.h"
#include "numIll_conv.h"
#include "numMPZ_conv.h"
#include "numRl_conv.h"
#include "numRll_conv.h"
#include "numMPQ_conv.h"
#include "numD_conv.h"
#include "numDl_conv.h"

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_num(long int a)
{ return numIl_fits_mpfr(&a); }
static inline bool llint_fits_num(long long int a)
{ return numIll_fits_mpfr(&a);  }
static inline bool mpz_fits_num(mpz_t a)
{ return numMPZ_fits_mpfr(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{
  numratRl_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numRl_fits_mpfr(&s);
}
static inline bool llfrac_fits_num(long long int i, long long int j)
{
  numratRll_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numRll_fits_mpfr(&s);
}
static inline bool mpq_fits_num(mpq_t a)
{ return numMPQ_fits_mpfr(a); }
static inline bool double_fits_num(double k)
{ return numD_fits_mpfr(&k); }
static inline bool ldouble_fits_num(long double k)
{ return numDl_fits_mpfr(&k); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{ return true; }

static inline bool num_fits_lint(num_t a)
{ return mpfr_fits_slong_p(a,GMP_RNDU); }
static inline bool num_fits_llint(num_t a)
{ return mpfr_fits_intmax_p(a,GMP_RNDU); }
static inline bool num_fits_lfrac(num_t a)
{ return mpfr_fits_numRl(a,NULL); }
static inline bool num_fits_llfrac(num_t a)
{ return mpfr_fits_numRll(a,NULL); }
static inline bool num_fits_float(num_t a)
{
  int e;
  e = mpfr_get_exp(a);
  return (e<FLT_MAX_EXP-1);
}
static inline bool num_fits_double(num_t a)
{ return mpfr_fits_numD(a,NULL); }
static inline bool num_fits_ldouble(num_t a)
{ return mpfr_fits_numDl(a,NULL); }
static inline bool num_fits_mpfr(num_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_lint(num_t a, long int i, numinternal_t intern)
{ return mpfr_set_numIl(a,&i,intern); }
static inline bool num_set_llint(num_t a, long long int i, numinternal_t intern)
{ return mpfr_set_numIll(a,&i,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ return mpfr_set_numMPZ(a,b,intern); }
static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numratRl_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numRl(a,&s,intern);
}
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numratRll_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numRll(a,&s,intern);
}
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{ return mpfr_set_numMPQ(a,b,intern); }
static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{ return mpfr_set_numD(a,&k,intern); }
static inline bool num_set_ldouble(num_t a, long double k, numinternal_t intern)
{ return mpfr_set_numDl(a,&k,intern); }
static inline bool num_set_mpfr(num_t a, mpfr_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }
static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ return numIl_set_mpfr(a,b,intern); }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return numIll_set_mpfr(a,b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ return numMPZ_set_mpfr(a,b,intern); }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{
  numratRl_native s;
  bool res = numRl_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{
  numratRll_native s;
  bool res = numRll_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{ return numMPQ_set_mpfr(a,b,intern); }
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{ return numD_set_mpfr(a,b,intern); }
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{ return numDl_set_mpfr(a,b,intern); }
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }

#endif

/* ********************************************************************** */
/* Inline definitions */
/* ********************************************************************** */

static inline bool num_set_numIl(num_t a, numIl_t b, numinternal_t internal)
{ return num_set_lint(a,*b,internal); }
static inline bool num_set_numIll(num_t a, numIll_t b, numinternal_t internal)
{ return num_set_llint(a,*b,internal); }
static inline bool num_set_numMPZ(num_t a, numMPZ_t b, numinternal_t internal)
{ return num_set_mpz(a,b,internal); }
static inline bool num_set_numRl(num_t a, numRl_t b, numinternal_t internal)
{ return num_set_lfrac(a,*b->n,*b->d,internal); }
static inline bool num_set_numRll(num_t a, numRll_t b, numinternal_t internal)
{ return num_set_llfrac(a,*b->n,*b->d,internal); }
static inline bool num_set_numD(num_t a, numD_t b, numinternal_t internal)
{ return num_set_double(a,*b,internal); }
static inline bool num_set_numDl(num_t a, numDl_t b, numinternal_t internal)
{ return num_set_ldouble(a,*b,internal); }
static inline bool num_set_numMPFR(num_t a, numMPFR_t b, numinternal_t internal)
{ return num_set_mpfr(a,b,internal); }

#ifdef __cplusplus
}
#endif

#endif
