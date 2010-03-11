/* ********************************************************************** */
/* numfltMPFR_def.h */
/* ********************************************************************** */

#ifndef _NUMFLTMPFR_DEF_H_
#define _NUMFLTMPFR_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "numConfig.h"
#include "numintIl_def.h"
#include "numintIll_def.h"
#include "numintMPZ_def.h"
#include "numratRl_def.h"
#include "numratRll_def.h"
#include "numratMPQ_def.h"
#include "numfltD_def.h"
#include "numfltDl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTES:
   - use mpfr_set_default_prec to change the precision (global)
   - rounding is always towards +oo
*/

typedef mpfr_t numfltMPFR_t;

#undef NUMFLTMPFR_MAX
#undef NUMMPFR_MAX
#undef NUMFLTMPFR_ZERO
#undef NUMFLTMPFR_ONE
#undef NUMFLTMPFR_MANT_DIG

#ifndef NUMFLTMPFR_PRINT_PREC
#define NUMFLTMPFR_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTMPFR_PRINT_PREC to be any other
   expression (including variable and function call).
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numfltMPFR_set(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_set(a,b,GMP_RNDU); }
static inline void numfltMPFR_set_array(numfltMPFR_t* a, numfltMPFR_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_set(a[i],b[i],GMP_RNDU);
}
static inline void numfltMPFR_set_int(numfltMPFR_t a, long int i)
{ mpfr_set_si(a,i,GMP_RNDU); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numfltMPFR_init(numfltMPFR_t a)
{ mpfr_init(a); }
static inline void numfltMPFR_init_array(numfltMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_init(a[i]);
}
static inline void numfltMPFR_init_set(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_init_set(a,b,GMP_RNDU); }
static inline void numfltMPFR_init_set_int(numfltMPFR_t a, long int i)
{ mpfr_init_set_si(a,i,GMP_RNDU); }

static inline void numfltMPFR_clear(numfltMPFR_t a)
{ mpfr_clear(a); }
static inline void numfltMPFR_clear_array(numfltMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_clear(a[i]);
}

static inline void numfltMPFR_swap(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_swap(a,b); }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numfltMPFR_neg(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_neg(a,b,GMP_RNDU); }
static inline void numfltMPFR_add(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_add(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_add_uint(numfltMPFR_t a, numfltMPFR_t b, unsigned long int c)
{ mpfr_add_ui(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_sub(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_sub(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_sub_uint(numfltMPFR_t a, numfltMPFR_t b, unsigned long int c)
{ mpfr_sub_ui(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_mul(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_mul(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_div(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_div(a,b,c,GMP_RNDU); }

static inline void numfltMPFR_abs(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_abs(a,b,GMP_RNDU); }
static inline void numfltMPFR_mul_2(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_mul_2ui(a,b,1,GMP_RNDU); }
static inline void numfltMPFR_div_2(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_div_2ui(a,b,1,GMP_RNDU); }
static inline void numfltMPFR_min(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_min(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_max(numfltMPFR_t a, numfltMPFR_t b, numfltMPFR_t c)
{ mpfr_max(a,b,c,GMP_RNDU); }
static inline void numfltMPFR_floor(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_rint_floor(a,b,GMP_RNDU); }
static inline void numfltMPFR_ceil(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_rint_ceil(a,b,GMP_RNDU); }
static inline void numfltMPFR_trunc(numfltMPFR_t a, numfltMPFR_t b)
{ mpfr_rint_trunc(a,b,GMP_RNDU); }
static inline void numfltMPFR_sqrt(numfltMPFR_t up, numfltMPFR_t down, numfltMPFR_t b)
{
  mpfr_sqrt(up,b,GMP_RNDU);
  mpfr_sqrt(down,b,GMP_RNDD);
}
static inline void numfltMPFR_mul_2exp(numfltMPFR_t a, numfltMPFR_t b, int c)
{ mpfr_mul_2si(a,b,c,GMP_RNDU); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numfltMPFR_sgn(numfltMPFR_t a)
{ return mpfr_sgn(a); }
static inline int numfltMPFR_cmp(numfltMPFR_t a, numfltMPFR_t b)
{ return mpfr_cmp(a,b); }
static inline int numfltMPFR_cmp_int(numfltMPFR_t a, long int b)
{ return mpfr_cmp_si(a,b); }
static inline bool numfltMPFR_equal(numfltMPFR_t a, numfltMPFR_t b)
{ return mpfr_equal_p(a,b); }
static inline bool numfltMPFR_integer(numfltMPFR_t a)
{ return mpfr_integer_p(a); }
static inline int numfltMPFR_hash(numfltMPFR_t a)
{ return mpfr_get_si(a,GMP_RNDU); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline int numfltMPFR_snprint(char* s, size_t size, numfltMPFR_t a)
{
  double d;
  /* special cases */
  if (mpfr_nan_p(a)) return snprintf(s,size,"NaN");
  if (mpfr_inf_p(a)) return snprintf(s,size,"%coo",mpfr_sgn(a)>0?'+':'-');
  if (mpfr_zero_p(a)) return snprintf(s,size,"0");
  d = mpfr_get_d(a,GMP_RNDU);
  if (!mpfr_cmp_d(a,d)) return snprintf(s,size,"%.*g",NUMFLTMPFR_PRINT_PREC,d);
  else {
    /* general case */
    char* tmp;
    mp_exp_t e;
    int x,i;
    tmp = mpfr_get_str(NULL,&e,10,NUMFLTMPFR_PRINT_PREC,a,GMP_RNDU);
    if (!tmp) { *s = 0; return 0; }
    if (tmp[0]=='-' || tmp[0]=='+')
      x=snprintf(s,size,"%c.%se+%ld",tmp[0],tmp+1,(long int)e);
    else
      x=snprintf(s,size,".%se%+ld",tmp,(long int)e);
    mpfr_free_str(tmp);
    return x;
  }
}
static inline void numfltMPFR_fprint(FILE* stream, numfltMPFR_t a)
{
  char buf[256];
  numfltMPFR_snprint(buf,sizeof(buf)-10,a);
  fputs(buf,stream);
}
static inline void numfltMPFR_print(numfltMPFR_t a)
{ numfltMPFR_fprint(stdout,a); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numfltMPFR(long int a)
{ return numintIl_fits_mpfr(&a); }
static inline bool llint_fits_numfltMPFR(long long int a)
{ return numintIll_fits_mpfr(&a);  }
static inline bool mpz_fits_numfltMPFR(mpz_t a)
{ return numintMPZ_fits_mpfr(a); }
static inline bool lfrac_fits_numfltMPFR(long int i, long int j)
{
  struct numratRl_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numratRl_fits_mpfr(&s);
}
static inline bool llfrac_fits_numfltMPFR(long long int i, long long int j)
{
  struct numratRll_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numratRll_fits_mpfr(&s);
}
static inline bool mpq_fits_numfltMPFR(mpq_t a)
{ return numratMPQ_fits_mpfr(a); }
static inline bool double_fits_numfltMPFR(double k)
{ return numfltD_fits_mpfr(&k); }
static inline bool ldouble_fits_numfltMPFR(long double k)
{ return numfltDl_fits_mpfr(&k); }
static inline bool mpfr_fits_numfltMPFR(mpfr_t a, numinternal_t intern)
{ return true; }

static inline bool numfltMPFR_fits_lint(numfltMPFR_t a)
{ return mpfr_fits_slong_p(a,GMP_RNDU); }
static inline bool numfltMPFR_fits_llint(numfltMPFR_t a)
{ return mpfr_fits_intmax_p(a,GMP_RNDU); }
static inline bool numfltMPFR_fits_lfrac(numfltMPFR_t a)
{ return mpfr_fits_numratRl(a,NULL); }
static inline bool numfltMPFR_fits_llfrac(numfltMPFR_t a)
{ return mpfr_fits_numratRll(a,NULL); }
static inline bool numfltMPFR_fits_float(numfltMPFR_t a)
{
  int e;
  e = mpfr_get_exp(a);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numfltMPFR_fits_double(numfltMPFR_t a)
{ return mpfr_fits_numfltD(a,NULL); }
static inline bool numfltMPFR_fits_ldouble(numfltMPFR_t a)
{ return mpfr_fits_numfltDl(a,NULL); }
static inline bool numfltMPFR_fits_mpfr(numfltMPFR_t a)
{ return true; }

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

/* lint -> numflt */
static inline bool numfltMPFR_set_lint(numfltMPFR_t a, long int i, numinternal_t intern)
{ return mpfr_set_numintIl(a,&i,intern); }
/* llint -> numflt */
static inline bool numfltMPFR_set_llint(numfltMPFR_t a, long long int i, numinternal_t intern)
{ return mpfr_set_numintIll(a,&i,intern); }
/* mpz -> numflt */
static inline bool numfltMPFR_set_mpz(numfltMPFR_t a, mpz_t b, numinternal_t intern)
{ return mpfr_set_numintMPZ(a,b,intern); }
/* lfrac -> numflt */
static inline bool numfltMPFR_set_lfrac(numfltMPFR_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  struct numratRl_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numratRl(a,&s,intern);
}
/* llfrac -> numflt */
static inline bool numfltMPFR_set_llfrac(numfltMPFR_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  struct numratRll_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numratRll(a,&s,intern);
}
/* mpq -> numflt */
static inline bool numfltMPFR_set_mpq(numfltMPFR_t a, mpq_t b, numinternal_t intern)
{ return mpfr_set_numratMPQ(a,b,intern); }
/* double -> numflt */
static inline bool numfltMPFR_set_double(numfltMPFR_t a, double k, numinternal_t intern)
{ return mpfr_set_numfltD(a,&k,intern); }
/* ldouble -> numflt */
static inline bool numfltMPFR_set_ldouble(numfltMPFR_t a, long double k, numinternal_t intern)
{ return mpfr_set_numfltDl(a,&k,intern); }
/* mpfr -> numflt */
static inline bool numfltMPFR_set_mpfr(numfltMPFR_t a, mpfr_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }

/* numflt -> lint */
static inline bool lint_set_numfltMPFR(long int* a, numfltMPFR_t b, numinternal_t intern)
{ return numintIl_set_mpfr(a,b,intern); }
/* numflt -> llint */
static inline bool llint_set_numfltMPFR(long long int* a, numfltMPFR_t b, numinternal_t intern)
{ return numintIll_set_mpfr(a,b,intern); }
/* numflt -> mpz */
static inline bool mpz_set_numfltMPFR(mpz_t a, numfltMPFR_t b, numinternal_t intern)
{ return numintMPZ_set_mpfr(a,b,intern); }
/* numflt -> lfrac */
static inline bool lfrac_set_numfltMPFR(long int* i, long int* j, numfltMPFR_t b, numinternal_t intern)
{
  struct numratRl_native s;
  bool res = numratRl_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> llfrac */
static inline bool llfrac_set_numfltMPFR(long long int* i, long long int* j, numfltMPFR_t b, numinternal_t intern)
{
  struct numratRll_native s;
  bool res = numratRll_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
/* numflt -> mpq */
static inline bool mpq_set_numfltMPFR(mpq_t a, numfltMPFR_t b, numinternal_t intern)
{ return numratMPQ_set_mpfr(a,b,intern); }
/* numflt -> double */
static inline bool double_set_numfltMPFR(double* a, numfltMPFR_t b, numinternal_t intern)
{ return numfltD_set_mpfr(a,b,intern); }
/* numflt -> long double */
static inline bool ldouble_set_numfltMPFR(long double* a, numfltMPFR_t b, numinternal_t intern)
{ return numfltDl_set_mpfr(a,b,intern); }
/* numflt -> mpfr */
static inline bool mpfr_set_numfltMPFR(mpfr_t a, numfltMPFR_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numfltMPFR_infty(numfltMPFR_t a)
{ return mpfr_inf_p(a); }
static inline void numfltMPFR_set_infty(numfltMPFR_t a, int sgn)
{ mpfr_set_inf(a,sgn); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numfltMPFR_serialize(void* dst, numfltMPFR_t src)
{
  size_t count = 0;
  mp_exp_t e;
  mpz_t z;
  /* special cases */
  if (mpfr_nan_p(src)) { *((char*)dst) = 0; return 1; }
  if (mpfr_inf_p(src)) { *((char*)dst) = mpfr_sgn(src)>0?1:2 ; return 1; }
  /* normal case */
  mpz_init(z);
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(z,src);
  *((char*)dst) = mpz_sgn(z)>=0?3:4;
  mpz_export((char*)dst+9,&count,1,1,1,0,z);
  mpz_clear(z);
  /* XXX fails if count or exponent is more than 4-byte wide */
  assert(((unsigned)count)==count);
  assert(((int)e)==e);
  num_dump_word32((char*)dst+1,(unsigned)count);
  num_dump_word32((char*)dst+5,(unsigned)count);
  return count+9;

}

static inline size_t numfltMPFR_deserialize(numfltMPFR_t dst, const void* src)
{
  size_t count;
  mp_exp_t e;
  mpz_t z;
  switch (*((const char*)src)) {
  case 0: mpfr_set_nan(dst); return 1;
  case 1: mpfr_set_inf(dst,1); return 1;
  case 2: mpfr_set_inf(dst,-1); return 1;
  case 3:
  case 4:
    count = num_undump_word32((const char*)src+1);
    e = (int)num_undump_word32((const char*)src+5);
    mpz_init(z);
    mpz_import(z,count,1,1,1,0,(const char*)src+9);
    mpfr_set_z(dst,z,GMP_RNDU);
    mpfr_mul_2si(dst,dst,e,GMP_RNDU);
    if (*((const char*)src)==4) mpfr_neg(dst,dst,GMP_RNDU);
    mpz_clear(z);
    return count+9;
  default:
    assert(0);
    return 1;
  }
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numfltMPFR_serialized_size(numfltMPFR_t a)
{ return mpfr_get_prec(a)/8+9+sizeof(mp_limb_t); }

#ifdef __cplusplus
}
#endif

#endif
