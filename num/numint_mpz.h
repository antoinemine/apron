/* ********************************************************************** */
/* numint_mpz.h */
/* ********************************************************************** */

#ifndef _NUMINT_MPZ_H_
#define _NUMINT_MPZ_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"
#include "ap_scalar.h"

#include "num_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Require C99 compliant compiler */

typedef mpz_t numint_t;

#undef NUMINT_MAX
#undef NUMINT_NATIVE

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numint_set(numint_t a, numint_t b)
{ mpz_set(a,b); }
static inline void numint_set_array(numint_t* a, numint_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_set(a[i],b[i]);
}
static inline void numint_set_int(numint_t a, long int i)
{ mpz_set_si(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numint_init(numint_t a)
{ mpz_init(a); }
static inline void numint_init_array(numint_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_init(a[i]);
}
static inline void numint_init_set(numint_t a, numint_t b)
{ mpz_init_set(a,b); }
static inline void numint_init_set_int(numint_t a, long int i)
{ mpz_init_set_si(a,i); }

static inline void numint_clear(numint_t a)
{ mpz_clear(a); }
static inline void numint_clear_array(numint_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_clear(a[i]);
}

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numint_neg(numint_t a, numint_t b)
{ mpz_neg(a,b); }
static inline void numint_abs(numint_t a, numint_t b)
{ mpz_abs(a,b); }
static inline void numint_add(numint_t a, numint_t b, numint_t c)
{ mpz_add(a,b,c); }
static inline void numint_add_uint(numint_t a, numint_t b, unsigned long int c)
{ mpz_add_ui(a,b,c); }
static inline void numint_sub(numint_t a, numint_t b, numint_t c)
{ mpz_sub(a,b,c); }
static inline void numint_sub_uint(numint_t a, numint_t b, unsigned long int c)
{ mpz_sub_ui(a,b,c); }
static inline void numint_mul(numint_t a, numint_t b, numint_t c)
{ mpz_mul(a,b,c); }
static inline void numint_mul_2(numint_t a, numint_t b)
{ mpz_mul_2exp(a,b,1); }
static inline void numint_fdiv_q(numint_t a, numint_t b, numint_t c)
{ mpz_fdiv_q(a,b,c); }
static inline void numint_cdiv_q(numint_t a, numint_t b, numint_t c)
{ mpz_cdiv_q(a,b,c); }
static inline void numint_tdiv_q(numint_t a, numint_t b, numint_t c)
{ mpz_tdiv_q(a,b,c); }
static inline void numint_cdiv_qr(numint_t a, numint_t b, numint_t c, numint_t d)
{ mpz_cdiv_qr(a,b,c,d); }
static inline void numint_cdiv_2(numint_t a, numint_t b)
{ mpz_cdiv_q_2exp(a,b,1); }
static inline void numint_cdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ mpz_cdiv_q_2exp(a,b,c); }
static inline void numint_fdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ mpz_fdiv_q_2exp(a,b,c); }
static inline void numint_tdiv_q_2exp(numint_t a, numint_t b, unsigned long int c)
{ mpz_tdiv_q_2exp(a,b,c); }
static inline void numint_min(numint_t a, numint_t b, numint_t c)
{ mpz_set(a, mpz_cmp(b,c)<=0 ? b : c); }
static inline void numint_max(numint_t a, numint_t b, numint_t c)
{ mpz_set(a, mpz_cmp(b,c)>=0 ? b : c); }
static inline void numint_floor(numint_t a, numint_t b)
{ mpz_set(a, b); }
static inline void numint_ceil(numint_t a, numint_t b)
{ mpz_set(a, b); }

static inline void numint_sqrt(numint_t up, numint_t down, numint_t b)
{
  int perfect;
  assert(mpz_sgn(b)>=0);
  perfect = mpz_perfect_square_p(b);
  mpz_sqrt(down,b);
  if (perfect) mpz_set(up,down);
  else mpz_add_ui(up,down,1);
}

static inline int numint_pow(numint_t up, numint_t down, numint_t b, unsigned long int n)
{
  mpz_pow_ui(up, b, n);
  mpz_set(down, up);
  return 0;
}

static inline void numint_root(numint_t up, numint_t down, numint_t b, unsigned long int n)
{
  assert(n > 0);
  assert((n & 1) || (mpz_sgn(b) >= 0));
  if (mpz_sgn(b) < 0) {
    mpz_neg(b,b);
    if (mpz_root(up, b, n)) {
      mpz_neg(up, up);
      mpz_set(down, up);
    }
    else {
      mpz_neg(up, up);
      mpz_sub_ui(down, up, 1);
    }  
    mpz_neg(b,b);
  }
  else {
    if (mpz_root(down, b, n)) {
      mpz_set(up, down);
    }
    else {
      mpz_add_ui(up, down, 1);
    }  
  }
}

static inline void numint_mul_2exp(numint_t a, numint_t b, int c)
{
  if (c>=0) mpz_mul_2exp(a,b,c);
  else mpz_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numint_divexact(numint_t a, numint_t b, numint_t c)
{ mpz_divexact(a,b,c); }
static inline void numint_mod(numint_t a, numint_t b, numint_t c)
{ mpz_mod(a,b,c); }
static inline void numint_gcd(numint_t a, numint_t b,  numint_t c)
{ mpz_gcd(a,b,c); }
static inline void numint_lcm(numint_t a, numint_t b,  numint_t c)
{ mpz_lcm(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numint_sgn(numint_t a)
{ return mpz_sgn(a); }
static inline int numint_cmp(numint_t a, numint_t b)
{ return mpz_cmp(a,b); }
static inline int numint_cmp_int(numint_t a, long int b)
{ return mpz_cmp_si(a,b); }
static inline bool numint_equal(numint_t a, numint_t b)
{ return mpz_cmp(a,b)==0; }
static inline bool numint_integer(numint_t a)
{ (void)a; return true; }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numint_print(numint_t a)
{ mpz_out_str(stdout,10,a); }
static inline void numint_fprint(FILE* stream, numint_t a)
{ mpz_out_str(stream,10,a); }
static inline int numint_snprint(char* s, size_t size, numint_t a)
{
  int res;
  if (mpz_sizeinbase(a,10)+2>size)
    res = snprintf(s,size, mpz_sgn(a)>0 ? "+BIG" : "-BIG");
  else {
    mpz_get_str(s,10,a);
    res = strlen(s);
  }
  return res;
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* int2 -> numint */
static inline bool numint_set_int2(numint_t a, long int i, long int j)
{
  unsigned long int r;
  assert(j>0);
  numint_set_int(a,i);
  r = mpz_cdiv_q_ui(a,a,j);
  return r ? false : true;
}

/* mpz -> numint */
static inline bool numint_set_mpz(numint_t a, mpz_t b)
{ mpz_set(a,b); return true; }

/* mpq -> numint */
static inline bool numint_set_mpq_tmp(numint_t a, mpq_t b,
				      mpz_t q, mpz_t r)
{
  (void)q;
  mpz_cdiv_qr(a, r, mpq_numref(b),mpq_denref(b));
  bool res = (mpz_sgn(r)==0);
  return res;
}
static inline bool numint_set_mpq(numint_t a, mpq_t b)
{
  mpz_t r;
  mpz_init(r);
  bool res = numint_set_mpq_tmp(a,b,r,r);
  mpz_clear(r);
  return res;
}

/* double -> numint */
static inline bool numint_set_double(numint_t a, double b)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  mpz_set_d(a,c);
  return (b==c);
}

/* mpfr -> numint */
static inline bool numint_set_mpfr(numint_t a, mpfr_t b)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numint_set_int(a,0); return false; }
  mpfr_get_z(a,b,GMP_RNDU);
  return mpfr_integer_p(b);
}

/* numint -> int */
static inline bool int_set_numint(long int* a, numint_t b)
{ *a = mpz_get_si(b); return true; }

/* numint -> mpz */
static inline bool mpz_set_numint(mpz_t a, numint_t b)
{ mpz_set(a,b); return true; }
/* numint -> mpq */
static inline bool mpq_set_numint(mpq_t a, numint_t b)
{
  mpz_set(mpq_numref(a),b);
  mpz_set_ui(mpq_denref(a),1);
  return true;
}
/* numint -> double */
/* mpfr is supposed to have exactly the IEEE754 double precision of 53 bits */
static inline bool double_set_numint_tmp(double* a, numint_t b,
					 mpfr_t mpfr)
{
  int res = mpfr_set_z(mpfr,b,GMP_RNDU);
  *a = mpfr_get_d(mpfr,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
static inline bool double_set_numint(double* a, numint_t b)
{
  mpfr_t mpfr;

  mpfr_init2(mpfr,53);
  bool res = double_set_numint_tmp(a,b,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* numint -> mpfr */
static inline bool mpfr_set_numint(mpfr_t a, numint_t b)
{ return !mpfr_set_z(a,b,GMP_RNDU); }

static inline bool mpz_fits_numint(mpz_t a)
{ (void)a; return true; }
static inline bool mpq_fits_numint_tmp(mpq_t a, mpz_t mpz)
{ (void)a; (void)mpz; return true; }
static inline bool mpq_fits_numint(mpq_t a)
{ (void)a; return true; }
static inline bool double_fits_numint(double a)
{ return isfinite(a); }
static inline bool mpfr_fits_numint(mpfr_t a)
{ return mpfr_number_p(a); }
static inline bool numint_fits_int(numint_t a)
{ return mpz_fits_slong_p(a); }
static inline bool numint_fits_float(numint_t a)
{ return (mpz_sizeinbase(a,2)<127); }
static inline bool numint_fits_double(numint_t a)
{ return (mpz_sizeinbase(a,2)<1023); }
static inline bool numint_fits_mpfr(numint_t a)
{ return (mpz_sizeinbase(a,2)+1<(size_t)mpfr_get_emax()); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numint_serialize_id(void)
{ return 0xf; }

static inline size_t numint_serialize(void* dst, numint_t src)
{
  size_t count = 0;
  *((char*)dst) = mpz_sgn(src);
  mpz_export((char*)dst+5,&count,1,1,1,0,src);
  assert(((unsigned)count)==count);
  num_dump_word32((char*)dst+1,(unsigned)count);
  return count+5;
}

static inline size_t numint_deserialize(numint_t dst, const void* src)
{
  size_t count = num_undump_word32((const char*)src+1);
  mpz_import(dst,count,1,1,1,0,(const char*)src+5);
  if (*(const char*)src<0)
    mpz_neg(dst,dst);
  return count+5;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numint_serialized_size(numint_t a)
{ return mpz_sizeinbase(a,2)/8+5+sizeof(mp_limb_t); }

#ifdef __cplusplus
}
#endif

#endif
