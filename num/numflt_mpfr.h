/* ********************************************************************** */
/* numflt_mpfr.h */
/* ********************************************************************** */

#ifndef _NUMFLT_MPFR_H_
#define _NUMFLT_MPFR_H_

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
#include "ap_scalar.h"
#include "num_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTES:
   - use mpfr_set_default_prec to change the precision (global)
   - rounding is always towards +oo
*/

typedef mpfr_t numflt_t;

#undef NUMFLT_MAX
#undef NUMFLT_NATIVE

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numflt_set(numflt_t a, numflt_t b)
{ mpfr_set(a,b,GMP_RNDU); }
static inline void numflt_set_array(numflt_t* a, numflt_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_set(a[i],b[i],GMP_RNDU);
}
static inline void numflt_set_int(numflt_t a, long int i)
{ mpfr_set_si(a,i,GMP_RNDU); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numflt_init(numflt_t a)
{ mpfr_init_set_ui(a,0,GMP_RNDN); }
static inline void numflt_init_array(numflt_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_init_set_ui(a[i],0,GMP_RNDN);
}
static inline void numflt_init_set(numflt_t a, numflt_t b)
{ mpfr_init_set(a,b,GMP_RNDU); }
static inline void numflt_init_set_int(numflt_t a, long int i)
{ mpfr_init_set_si(a,i,GMP_RNDU); }

static inline void numflt_clear(numflt_t a)
{ mpfr_clear(a); }
static inline void numflt_clear_array(numflt_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpfr_clear(a[i]);
}

static inline void numflt_swap(numflt_t a, numflt_t b)
{ mpfr_swap(a,b); }


/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numflt_neg(numflt_t a, numflt_t b)
{ mpfr_neg(a,b,GMP_RNDU); }
static inline void numflt_add(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_add(a,b,c,GMP_RNDU); }
static inline void numflt_add_uint(numflt_t a, numflt_t b, unsigned long int c)
{ mpfr_add_ui(a,b,c,GMP_RNDU); }
static inline void numflt_sub(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_sub(a,b,c,GMP_RNDU); }
static inline void numflt_sub_uint(numflt_t a, numflt_t b, unsigned long int c)
{ mpfr_sub_ui(a,b,c,GMP_RNDU); }
static inline void numflt_mul(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_mul(a,b,c,GMP_RNDU); }
static inline void numflt_div(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_div(a,b,c,GMP_RNDU); }

static inline void numflt_abs(numflt_t a, numflt_t b)
{ mpfr_abs(a,b,GMP_RNDU); }
static inline void numflt_mul_2(numflt_t a, numflt_t b)
{ mpfr_mul_2ui(a,b,1,GMP_RNDU); }
static inline void numflt_div_2(numflt_t a, numflt_t b)
{ mpfr_div_2ui(a,b,1,GMP_RNDU); }
static inline void numflt_min(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_min(a,b,c,GMP_RNDU); }
static inline void numflt_max(numflt_t a, numflt_t b, numflt_t c)
{ mpfr_max(a,b,c,GMP_RNDU); }
static inline void numflt_floor(numflt_t a, numflt_t b)
{ mpfr_rint_floor(a,b,GMP_RNDU); }
static inline void numflt_ceil(numflt_t a, numflt_t b)
{ mpfr_rint_ceil(a,b,GMP_RNDU); }
static inline void numflt_trunc(numflt_t a, numflt_t b)
{ mpfr_rint_trunc(a,b,GMP_RNDU); }
static inline void numflt_sqrt(numflt_t up, numflt_t down, numflt_t b)
{
  mpfr_sqrt(up,b,GMP_RNDU);
  mpfr_sqrt(down,b,GMP_RNDD);
}
static inline int  numflt_pow(numflt_t up, numflt_t down, numflt_t b, unsigned long int n)
{
  mpfr_pow_ui(up, b, n, GMP_RNDU);
  mpfr_pow_ui(down, b, n, GMP_RNDD);
  return 0;
}
static inline void numflt_root(numflt_t up, numflt_t down, numflt_t b, unsigned long int n)
{
  assert(n > 0);
  assert((n & 1) || (mpfr_sgn(b) >= 0));
#if (MPFR_VERSION_MAJOR < 4)
  mpfr_root(up, b, n, GMP_RNDU);
  mpfr_root(down, b, n, GMP_RNDD);
#else
  mpfr_rootn_ui(up, b, n, GMP_RNDU);
  mpfr_rootn_ui(down, b, n, GMP_RNDD);
#endif
}
static inline void numflt_mul_2exp(numflt_t a, numflt_t b, int c)
{ mpfr_mul_2si(a,b,c,GMP_RNDU); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numflt_sgn(numflt_t a)
{ return mpfr_sgn(a); }
static inline int numflt_cmp(numflt_t a, numflt_t b)
{ return mpfr_cmp(a,b); }
static inline int numflt_cmp_int(numflt_t a, long int b)
{ return mpfr_cmp_si(a,b); }
static inline bool numflt_equal(numflt_t a, numflt_t b)
{ return mpfr_equal_p(a,b); }
static inline bool numflt_integer(numflt_t a)
{ return mpfr_integer_p(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline int numflt_snprint(char* s, size_t size, numflt_t a)
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
    int x;
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
static inline void numflt_fprint(FILE* stream, numflt_t a)
{
  char buf[256];
  numflt_snprint(buf,sizeof(buf)-10,a);
  fputs(buf,stream);
}
static inline void numflt_print(numflt_t a)
{ numflt_fprint(stdout,a); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* int2 -> numflt */
static inline bool numflt_set_int2(numflt_t a, long int i, long int j)
{
  int r1,r2;
  assert(j>0);
  r1=mpfr_set_si(a,i,GMP_RNDU);
  r2=mpfr_div_si(a,a,j,GMP_RNDU);
  return !r1 && !r2;
}

/* mpz -> numflt */
static inline bool numflt_set_mpz_tmp(numflt_t a, mpz_t b, mpfr_t mpfr)
{ (void)mpfr; return !mpfr_set_z(a,b,GMP_RNDU); }
static inline bool numflt_set_mpz(numflt_t a, mpz_t b)
{ return !mpfr_set_z(a,b,GMP_RNDU); }

/* mpq -> numflt */
static inline bool numflt_set_mpq_tmp(numflt_t a, mpq_t b, mpfr_t mpfr)
{ (void)mpfr; return !mpfr_set_q(a,b,GMP_RNDU); }
static inline bool numflt_set_mpq(numflt_t a, mpq_t b)
{ return !mpfr_set_q(a,b,GMP_RNDU); }

/* double -> numflt */
static inline bool numflt_set_double(numflt_t a, double k)
{ return !mpfr_set_d(a,k,GMP_RNDU); }

/* mpfr -> numflt */
static inline bool numflt_set_mpfr(numflt_t a, mpfr_t b)
{ return !mpfr_set(a,b,GMP_RNDU); }

/* numflt -> int */
static inline bool int_set_numflt(long int* a, numflt_t b)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; *a = 0; return false; }
  *a = mpfr_get_si(b,GMP_RNDU);
  return mpfr_integer_p(b);
}
/* numflt -> mpz */
static inline bool mpz_set_numflt(mpz_t a, numflt_t b)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  mpfr_get_z(a,b,GMP_RNDU);
  return mpfr_integer_p(b);
}
/* numflt -> mpq */
static inline bool mpq_set_numflt(mpq_t a, numflt_t b)
{
  mp_exp_t e;
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; mpq_set_si(a,0,1); return false; }
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(mpq_numref(a),b);
  mpz_set_si(mpq_denref(a),1);
  if (e>0) mpq_mul_2exp(a,a,e);
  if (e<0) mpq_div_2exp(a,a,-e);
  return true;
}

/* numflt -> double */
static inline bool double_set_numflt(double* a, numflt_t b)
{
  *a = mpfr_get_d(b,GMP_RNDU);
  return !mpfr_cmp_d(b,*a);
}

/* numflt -> mpfr */
static inline bool mpfr_set_numflt(mpfr_t a, numflt_t b)
{ return !mpfr_set(a,b,GMP_RNDU); }

static inline bool mpz_fits_numflt(mpz_t a)
{ return mpz_sizeinbase(a,2)+1<(size_t)mpfr_get_emax(); }
static inline bool mpq_fits_numflt(mpq_t a)
{
  return ((int)mpz_sizeinbase(mpq_numref(a),2)-
	  (int)mpz_sizeinbase(mpq_denref(a),2)+1<mpfr_get_emax());
}
static inline bool double_fits_numflt(double a)
{ (void)a; return true; }
static inline bool mpfr_fits_numflt(mpfr_t a)
{ (void)a; return true; }

static inline bool numflt_fits_int(numflt_t a)
{ return mpfr_number_p(a) && mpfr_fits_slong_p(a,GMP_RNDU); }
static inline bool numflt_fits_float(numflt_t a)
{ return mpfr_number_p(a) && mpfr_get_exp(a)<126; }
static inline bool numflt_fits_double(numflt_t a)
{ return mpfr_number_p(a) && mpfr_get_exp(a)<1022; }
static inline bool numflt_fits_mpfr(numflt_t a)
{ (void)a; return true; }


/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numflt_infty(numflt_t a)
{ return mpfr_inf_p(a); }
static inline void numflt_set_infty(numflt_t a, int sgn)
{ mpfr_set_inf(a,sgn); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t numflt_serialize(void* dst, numflt_t src)
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

static inline size_t numflt_deserialize(numflt_t dst, const void* src)
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
static inline size_t numflt_serialized_size(numflt_t a)
{ return mpfr_get_prec(a)/8+9+sizeof(mp_limb_t); }


/* */
static inline bool ap_scalar_set_numflt(ap_scalar_t* a, numflt_t b)
{
  ap_scalar_reinit(a,AP_SCALAR_MPFR);
  return mpfr_set_numflt(a->val.mpfr,b);
}

#ifdef __cplusplus
}
#endif

#endif
