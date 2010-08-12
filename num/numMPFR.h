/* ********************************************************************** */
/* numMPFR.h */
/* ********************************************************************** */

#ifndef _NUMMPFR_H_
#define _NUMMPFR_H_

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
#include "numD.h"
#include "numDl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTES:
   - use mpfr_set_default_prec to change the precision (global)
   - rounding is always towards +oo
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numMPFR_set(numMPFR_t a, numMPFR_t b)
{ mpfr_set(a,b,GMP_RNDU); }
static inline void numMPFR_set_array(numMPFR_t* a, numMPFR_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_set(a[i],b[i],GMP_RNDU);
}
static inline void numMPFR_set_int(numMPFR_t a, long int i)
{ mpfr_set_si(a,i,GMP_RNDU); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numMPFR_init(numMPFR_t a)
{ mpfr_init(a); }
static inline void numMPFR_init_array(numMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_init(a[i]);
}
static inline void numMPFR_init_set(numMPFR_t a, numMPFR_t b)
{ mpfr_init_set(a,b,GMP_RNDU); }
static inline void numMPFR_init_set_int(numMPFR_t a, long int i)
{ mpfr_init_set_si(a,i,GMP_RNDU); }

static inline void numMPFR_clear(numMPFR_t a)
{ mpfr_clear(a); }
static inline void numMPFR_clear_array(numMPFR_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_clear(a[i]);
}

static inline void numMPFR_swap(numMPFR_t a, numMPFR_t b)
{ mpfr_swap(a,b); }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numMPFR_neg(numMPFR_t a, numMPFR_t b)
{ mpfr_neg(a,b,GMP_RNDU); }
static inline void numMPFR_add(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_add(a,b,c,GMP_RNDU); }
static inline void numMPFR_add_uint(numMPFR_t a, numMPFR_t b, unsigned long int c)
{ mpfr_add_ui(a,b,c,GMP_RNDU); }
static inline void numMPFR_sub(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_sub(a,b,c,GMP_RNDU); }
static inline void numMPFR_sub_uint(numMPFR_t a, numMPFR_t b, unsigned long int c)
{ mpfr_sub_ui(a,b,c,GMP_RNDU); }
static inline void numMPFR_mul(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_mul(a,b,c,GMP_RNDU); }
static inline void numMPFR_div(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_div(a,b,c,GMP_RNDU); }

static inline void numMPFR_abs(numMPFR_t a, numMPFR_t b)
{ mpfr_abs(a,b,GMP_RNDU); }
static inline void numMPFR_mul_2(numMPFR_t a, numMPFR_t b)
{ mpfr_mul_2ui(a,b,1,GMP_RNDU); }
static inline void numMPFR_div_2(numMPFR_t a, numMPFR_t b)
{ mpfr_div_2ui(a,b,1,GMP_RNDU); }
static inline void numMPFR_min(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_min(a,b,c,GMP_RNDU); }
static inline void numMPFR_max(numMPFR_t a, numMPFR_t b, numMPFR_t c)
{ mpfr_max(a,b,c,GMP_RNDU); }
static inline void numMPFR_floor(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_floor(a,b,GMP_RNDU); }
static inline void numMPFR_ceil(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_ceil(a,b,GMP_RNDU); }
static inline void numMPFR_trunc(numMPFR_t a, numMPFR_t b)
{ mpfr_rint_trunc(a,b,GMP_RNDU); }
static inline void numMPFR_sqrt(numMPFR_t up, numMPFR_t down, numMPFR_t b)
{
  mpfr_sqrt(up,b,GMP_RNDU);
  mpfr_sqrt(down,b,GMP_RNDD);
}
static inline void numMPFR_mul_2exp(numMPFR_t a, numMPFR_t b, int c)
{ mpfr_mul_2si(a,b,c,GMP_RNDU); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numMPFR_sgn(numMPFR_t a)
{ return mpfr_sgn(a); }
static inline int numMPFR_cmp(numMPFR_t a, numMPFR_t b)
{ return mpfr_cmp(a,b); }
static inline int numMPFR_cmp_int(numMPFR_t a, long int b)
{ return mpfr_cmp_si(a,b); }
static inline bool numMPFR_equal(numMPFR_t a, numMPFR_t b)
{ return mpfr_equal_p(a,b); }
static inline bool numMPFR_integer(numMPFR_t a)
{ return mpfr_integer_p(a); }
static inline int numMPFR_hash(numMPFR_t a)
{ return mpfr_get_si(a,GMP_RNDU); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline int numMPFR_snprint(char* s, size_t size, numMPFR_t a)
{
  double d;
  /* special cases */
  if (mpfr_nan_p(a)) return snprintf(s,size,"NaN");
  if (mpfr_inf_p(a)) return snprintf(s,size,"%coo",mpfr_sgn(a)>0?'+':'-');
  if (mpfr_zero_p(a)) return snprintf(s,size,"0");
  d = mpfr_get_d(a,GMP_RNDU);
  if (!mpfr_cmp_d(a,d)) return snprintf(s,size,"%.*g",NUMFLT_PRINT_PREC,d);
  else {
    /* general case */
    char* tmp;
    mp_exp_t e;
    int x,i;
    tmp = mpfr_get_str(NULL,&e,10,NUMFLT_PRINT_PREC,a,GMP_RNDU);
    if (!tmp) { *s = 0; return 0; }
    if (tmp[0]=='-' || tmp[0]=='+')
      x=snprintf(s,size,"%c.%se+%ld",tmp[0],tmp+1,(long int)e);
    else
      x=snprintf(s,size,".%se%+ld",tmp,(long int)e);
    mpfr_free_str(tmp);
    return x;
  }
}
static inline void numMPFR_fprint(FILE* stream, numMPFR_t a)
{
  char buf[256];
  numMPFR_snprint(buf,sizeof(buf)-10,a);
  fputs(buf,stream);
}
static inline void numMPFR_print(numMPFR_t a)
{ numMPFR_fprint(stdout,a); }

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numMPFR_infty(numMPFR_t a)
{ return mpfr_inf_p(a); }
static inline void numMPFR_set_infty(numMPFR_t a, int sgn)
{ mpfr_set_inf(a,sgn); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numMPFR_serialize(void* dst, numMPFR_t src)
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

static inline size_t numMPFR_deserialize(numMPFR_t dst, const void* src)
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
static inline size_t numMPFR_serialized_size(numMPFR_t a)
{ return mpfr_get_prec(a)/8+9+sizeof(mp_limb_t); }

static inline size_t numMPFR_serialize_array(void* dst, numMPFR_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numMPFR_deserialize_array(numMPFR_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numMPFR_serialized_size_array(numMPFR_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPFR_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numMPFR(long int a)
{ return numIl_fits_mpfr(&a); }
static inline bool llint_fits_numMPFR(long long int a)
{ return numIll_fits_mpfr(&a);  }
static inline bool mpz_fits_numMPFR(mpz_t a)
{ return numMPZ_fits_mpfr(a); }
static inline bool lfrac_fits_numMPFR(long int i, long int j)
{
  numRl_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numRl_fits_mpfr(&s);
}
static inline bool llfrac_fits_numMPFR(long long int i, long long int j)
{
  numRll_native s;
  *s.n = i;
  *s.n = j;
  return j>0 && numRll_fits_mpfr(&s);
}
static inline bool mpq_fits_numMPFR(mpq_t a)
{ return numMPQ_fits_mpfr(a); }
static inline bool double_fits_numMPFR(double k)
{ return numD_fits_mpfr(&k); }
static inline bool ldouble_fits_numMPFR(long double k)
{ return numDl_fits_mpfr(&k); }
static inline bool mpfr_fits_numMPFR(mpfr_t a, numinternal_t intern)
{ return true; }

static inline bool numMPFR_fits_lint(numMPFR_t a)
{ return mpfr_fits_slong_p(a,GMP_RNDU); }
static inline bool numMPFR_fits_llint(numMPFR_t a)
{ return mpfr_fits_intmax_p(a,GMP_RNDU); }
static inline bool numMPFR_fits_mpz(numMPFR_t a)
{ return true; }
static inline bool numMPFR_fits_lfrac(numMPFR_t a)
{ return mpfr_fits_numRl(a,NULL); }
static inline bool numMPFR_fits_llfrac(numMPFR_t a)
{ return mpfr_fits_numRll(a,NULL); }
static inline bool numMPFR_fits_mpq(numMPFR_t a)
{ return true; }
static inline bool numMPFR_fits_float(numMPFR_t a)
{
  int e;
  e = mpfr_get_exp(a);
  return (e<FLT_MAX_EXP-1);
}
static inline bool numMPFR_fits_double(numMPFR_t a)
{ return mpfr_fits_numD(a,NULL); }
static inline bool numMPFR_fits_ldouble(numMPFR_t a)
{ return mpfr_fits_numDl(a,NULL); }
static inline bool numMPFR_fits_mpfr(numMPFR_t a)
{ return true; }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numMPFR_set_lint(numMPFR_t a, long int i, numinternal_t intern)
{ return mpfr_set_numIl(a,&i,intern); }
static inline bool numMPFR_set_llint(numMPFR_t a, long long int i, numinternal_t intern)
{ return mpfr_set_numIll(a,&i,intern); }
static inline bool numMPFR_set_mpz(numMPFR_t a, mpz_t b, numinternal_t intern)
{ return mpfr_set_numMPZ(a,b,intern); }
static inline bool numMPFR_set_lfrac(numMPFR_t a, long int i, long int j, numinternal_t intern)
{
  assert(j>0);
  numRl_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numRl(a,&s,intern);
}
static inline bool numMPFR_set_llfrac(numMPFR_t a, long long int i, long long int j, numinternal_t intern)
{
  assert(j>0);
  numRll_native s;
  *s.n = i;
  *s.d = j;
  return mpfr_set_numRll(a,&s,intern);
}
static inline bool numMPFR_set_mpq(numMPFR_t a, mpq_t b, numinternal_t intern)
{ return mpfr_set_numMPQ(a,b,intern); }
static inline bool numMPFR_set_double(numMPFR_t a, double k, numinternal_t intern)
{ return mpfr_set_numD(a,&k,intern); }
static inline bool numMPFR_set_ldouble(numMPFR_t a, long double k, numinternal_t intern)
{ return mpfr_set_numDl(a,&k,intern); }
static inline bool numMPFR_set_mpfr(numMPFR_t a, mpfr_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }
static inline bool lint_set_numMPFR(long int* a, numMPFR_t b, numinternal_t intern)
{ return numIl_set_mpfr(a,b,intern); }
static inline bool llint_set_numMPFR(long long int* a, numMPFR_t b, numinternal_t intern)
{ return numIll_set_mpfr(a,b,intern); }
static inline bool mpz_set_numMPFR(mpz_t a, numMPFR_t b, numinternal_t intern)
{ return numMPZ_set_mpfr(a,b,intern); }
static inline bool lfrac_set_numMPFR(long int* i, long int* j, numMPFR_t b, numinternal_t intern)
{
  numRl_native s;
  bool res = numRl_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool llfrac_set_numMPFR(long long int* i, long long int* j, numMPFR_t b, numinternal_t intern)
{
  numRll_native s;
  bool res = numRll_set_mpfr(&s,b,intern);
  *i = *s.n;
  *j = *s.d;
  return res;
}
static inline bool mpq_set_numMPFR(mpq_t a, numMPFR_t b, numinternal_t intern)
{ return numMPQ_set_mpfr(a,b,intern); }
static inline bool double_set_numMPFR(double* a, numMPFR_t b, numinternal_t intern)
{ return numD_set_mpfr(a,b,intern); }
static inline bool ldouble_set_numMPFR(long double* a, numMPFR_t b, numinternal_t intern)
{ return numDl_set_mpfr(a,b,intern); }
static inline bool mpfr_set_numMPFR(mpfr_t a, numMPFR_t b, numinternal_t intern)
{ return !mpfr_set(a,b,GMP_RNDU); }

#ifdef __cplusplus
}
#endif

#endif
