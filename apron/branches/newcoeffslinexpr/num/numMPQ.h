/* ********************************************************************** */
/* numMPQ.h */
/* ********************************************************************** */

#ifndef _NUMMPQ_H_
#define _NUMMPQ_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "num_types.h"
#include "numIl.h"
#include "numIll.h"
#include "numMPZ.h"
#include "numRl.h"
#include "numRll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numMPQ_canonicalize(numMPQ_t a)
{ mpq_canonicalize(a); }

static inline bool numMPQ_set_numMPZ2(numMPQ_t a, numMPZ_t b, numMPZ_t c)
{
  numMPZ_set(numMPQ_numref(a),b);
  numMPZ_set(numMPQ_denref(a),c);
  numMPQ_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numMPQ_set(numMPQ_t a, numMPQ_t b)
{ mpq_set(a,b); }
static inline void numMPQ_set_array(numMPQ_t* a, numMPQ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_set(a[i],b[i]);
}
static inline void numMPQ_set_int(numMPQ_t a, long int i)
{ mpq_set_si(a,i,1); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numMPQ_init(numMPQ_t a)
{ mpq_init(a); }
static inline void numMPQ_init_array(numMPQ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_init(a[i]);
}
static inline void numMPQ_init_set(numMPQ_t a, numMPQ_t b)
{ mpq_init(a); mpq_set(a,b); }
static inline void numMPQ_init_set_int(numMPQ_t a, long int i)
{ mpq_init(a); mpq_set_si(a,i,1); }

static inline void numMPQ_clear(numMPQ_t a)
{ mpq_clear(a); }
static inline void numMPQ_clear_array(numMPQ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_clear(a[i]);
}
static inline void numMPQ_swap(numMPQ_t a, numMPQ_t b)
{ mpq_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numMPQ_neg(numMPQ_t a, numMPQ_t b)
{ mpq_neg(a,b); }
static inline void numMPQ_inv(numMPQ_t a, numMPQ_t b)
{ mpq_inv(a,b); }
static inline void numMPQ_abs(numMPQ_t a, numMPQ_t b)
{ mpq_abs(a,b); }
static inline void numMPQ_add(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_add(a,b,c); }
static inline void numMPQ_add_uint(numMPQ_t a, numMPQ_t b, unsigned long int c)
{
  if (a==b) {
    mpq_t tmp;
    mpq_init(tmp);
    mpq_set_ui(tmp,c,1);
    mpq_add(a,b,tmp);
    mpq_clear(tmp);
  }
  else {
    mpq_set_ui(a,c,1);
    mpq_add(a,a,b);
  }
}
static inline void numMPQ_sub(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_sub(a,b,c); }
static inline void numMPQ_sub_uint(numMPQ_t a, numMPQ_t b, unsigned long int c)
{
  if (a==b) {
    mpq_t tmp;
    mpq_init(tmp);
    mpq_set_ui(tmp,c,1);
    mpq_sub(a,b,tmp);
    mpq_clear(tmp);
  }
  else {
    mpq_set_ui(a,c,1);
    mpq_sub(a,b,a);
  }
}
static inline void numMPQ_mul(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_mul(a,b,c); }
static inline void numMPQ_mul_2(numMPQ_t a, numMPQ_t b)
{ mpq_mul_2exp(a,b,1); }
static inline void numMPQ_div(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_div(a,b,c); }
static inline void numMPQ_div_2(numMPQ_t a, numMPQ_t b)
{ mpq_div_2exp(a,b,1); }
static inline void numMPQ_min(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_set(a, mpq_cmp(b,c)<=0 ? b : c); }
static inline void numMPQ_max(numMPQ_t a, numMPQ_t b, numMPQ_t c)
{ mpq_set(a, mpq_cmp(b,c)>=0 ? b : c); }
static inline void numMPQ_floor(numMPQ_t a, numMPQ_t b)
{
  numMPZ_fdiv_q(numMPQ_numref(a),numMPQ_numref(b),numMPQ_denref(b));
  numMPZ_set_int(numMPQ_denref(a),1);
}
static inline void numMPQ_ceil(numMPQ_t a, numMPQ_t b)
{
  numMPZ_cdiv_q(numMPQ_numref(a),numMPQ_numref(b),numMPQ_denref(b));
  numMPZ_set_int(numMPQ_denref(a),1);
}
static inline void numMPQ_trunc(numMPQ_t a, numMPQ_t b)
{
  numMPZ_tdiv_q(numMPQ_numref(a),numMPQ_numref(b),numMPQ_denref(b));
  numMPZ_set_int(numMPQ_denref(a),1);
}
static inline void numMPQ_sqrt(numMPQ_t up, numMPQ_t down, numMPQ_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  mpz_t tmp;
  int perfect;
  assert(mpq_sgn(b)>=0);
  mpz_init(tmp);
  mpz_mul(tmp,mpq_numref(b),mpq_denref(b));
  perfect = mpz_perfect_square_p(tmp);
  mpz_sqrt(mpq_numref(down),tmp);
  if (perfect) mpz_set(mpq_numref(up),mpq_numref(down));
  else mpz_add_ui(mpq_numref(up),mpq_numref(down),1);
  mpz_set(mpq_denref(up),mpq_denref(b));
  mpz_set(mpq_denref(down),mpq_denref(b));
  numMPQ_canonicalize(up);
  numMPQ_canonicalize(down);
  mpz_clear(tmp);
}

static inline void numMPQ_mul_2exp(numMPQ_t a, numMPQ_t b, int c)
{
  if (c>=0) mpq_mul_2exp(a,b,c);
  else mpq_div_2exp(a,b,-c);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numMPQ_sgn(numMPQ_t a)
{ return mpq_sgn(a); }
static inline int numMPQ_cmp(numMPQ_t a, numMPQ_t b)
{ return mpq_cmp(a,b); }
static inline int numMPQ_cmp_int(numMPQ_t a, long int b)
{ return mpq_cmp_si(a,b,1); }
static inline bool numMPQ_equal(numMPQ_t a, numMPQ_t b)
{ return mpq_equal(a,b); }
static inline bool numMPQ_integer(numMPQ_t a)
{ return (mpz_cmp_ui(mpq_denref(a),1)==0); }
static inline int numMPQ_hash(numMPQ_t a)
{
  double d = mpq_get_d(a);
  if (d <= -(double)(INT_MAX))
    return -INT_MAX;
  else if (d < (double)INT_MAX)
    return lrint(d);
  else
    return INT_MAX;
}
/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numMPQ_print(numMPQ_t a)
{ mpq_out_str(stdout,10,a); }
static inline void numMPQ_fprint(FILE* stream, numMPQ_t a)
{ mpq_out_str(stream,10,a); }
static inline int numMPQ_snprint(char* s, size_t size, numMPQ_t a)
{
  int res;
  if (mpz_sizeinbase(mpq_numref(a),10) +
      mpz_sizeinbase(mpq_denref(a),10) +
      3 > size )
    res = snprintf(s,size, mpq_sgn(a)>0 ? "+BIG" : "-BIG");
  else {
    mpq_get_str(s,10,a);
    res = strlen(s);
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numMPQ_serialize_id(void)
{ return 0x1f; }

static inline size_t numMPQ_serialize(void* dst, numMPQ_t src)
{
  size_t count1 = 0;
  size_t count2 = 0;
  *((char*)dst) = mpq_sgn(src);
  mpz_export((char*)dst+9,&count1,1,1,1,0,mpq_numref(src));
  mpz_export((char*)dst+count1+9,&count2,1,1,1,0,mpq_denref(src));
  assert(((unsigned)count1)==count1);
  assert(((unsigned)count2)==count2);
  num_dump_word32((char*)dst+1,(unsigned)count1);
  num_dump_word32((char*)dst+5,(unsigned)count2);
  return count1+count2+9;
}

static inline size_t numMPQ_deserialize(numMPQ_t dst, const void* src)
{
  size_t count1 = num_undump_word32((const char*)src+1);
  size_t count2 = num_undump_word32((const char*)src+5);
  mpz_import(mpq_numref(dst),count1,1,1,1,0,(const char*)src+9);
  mpz_import(mpq_denref(dst),count2,1,1,1,0,(const char*)src+count1+9);
  if (*(const char*)src<0)
    mpq_neg(dst,dst);
  return count1+count2+9;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numMPQ_serialized_size(numMPQ_t a)
{
  return
    (mpz_sizeinbase(mpq_numref(a),2)+mpz_sizeinbase(mpq_denref(a),2))/8+
    9+2*sizeof(mp_limb_t);
}

static inline size_t numMPQ_serialize_array(void* dst, numMPQ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numMPQ_deserialize_array(numMPQ_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numMPQ_serialized_size_array(numMPQ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numMPQ(long int a)
{ return true; }
static inline bool llint_fits_numMPQ(long long int a)
{ return true; }
static inline bool mpz_fits_numMPQ(mpz_t a)
{ return true; }
static inline bool lfrac_fits_numMPQ(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numMPQ(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numMPQ(mpq_t a)
{ return true; }
static inline bool double_fits_numMPQ(double k)
{ return isfinite(k); }
static inline bool ldouble_fits_numMPQ(long double k)
{ return isfinite(k); }
static inline bool mpfr_fits_numMPQ(mpfr_t a, num_internal_t intern)
{ return mpfr_number_p(a); }

static inline bool numMPQ_fits_lint(numMPQ_t a)
{ return mpq_fits_numIl(a); }
static inline bool numMPQ_fits_llint(numMPQ_t a)
{ return mpq_fits_numIll(a); }
static inline bool numMPQ_fits_mpz(numMPQ_t a)
{ return true; }
static inline bool numMPQ_fits_lfrac(numMPQ_t a)
{ return mpq_fits_numRl(a); }
static inline bool numMPQ_fits_llfrac(numMPQ_t a)
{ return mpq_fits_numRll(a); }
static inline bool numMPQ_fits_mpq(numMPQ_t a)
{ return true; }
static inline bool numMPQ_fits_float(numMPQ_t a)
{
  return ((int)mpz_sizeinbase(numMPQ_numref(a),2)-
	  (int)mpz_sizeinbase(numMPQ_denref(a),2)<FLT_MAX_EXP-2);
}
static inline bool numMPQ_fits_double(numMPQ_t a)
{
  return ((int)mpz_sizeinbase(numMPQ_numref(a),2)-
	  (int)mpz_sizeinbase(numMPQ_denref(a),2)<DBL_MAX_EXP-2);
}
static inline bool numMPQ_fits_ldouble(numMPQ_t a)
{
  return ((int)mpz_sizeinbase(numMPQ_numref(a),2)-
	  (int)mpz_sizeinbase(numMPQ_denref(a),2)<LDBL_MAX_EXP-2);
}
static inline bool numMPQ_fits_mpfr(numMPQ_t a)
{
  return ((int)mpz_sizeinbase(numMPQ_numref(a),2)-
	  (int)mpz_sizeinbase(numMPQ_denref(a),2)+1<mpfr_get_emax());
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numMPQ_set_lint(numMPQ_t a, long int b, num_internal_t intern)
{ return mpq_set_numIl(a,&b,intern); }
static inline bool numMPQ_set_llint(numMPQ_t a, long long int b, num_internal_t intern)
{ return mpq_set_numIll(a,&b,intern); }
static inline bool numMPQ_set_mpz(numMPQ_t a, mpz_t b, num_internal_t intern)
{ return mpq_set_numMPZ(a,b,intern); }
static inline bool numMPQ_set_lfrac(numMPQ_t a, long int i, long int j, num_internal_t intern)
{ mpq_set_si(a,i,j); return true; }
static inline bool numMPQ_set_llfrac(numMPQ_t a, long long int i, long long int j, num_internal_t intern)
{
  numMPZ_set_llint(mpq_numref(a),i,intern);
  numMPZ_set_llint(mpq_denref(a),j,intern);
  mpq_canonicalize(a);
  return true;
}
static inline bool numMPQ_set_mpq(numMPQ_t a, mpq_t b, num_internal_t intern)
{ mpq_set(a,b); return true; }
static inline bool numMPQ_set_double(numMPQ_t a, double k, num_internal_t intern)
{
  if (!isfinite(k)) { DEBUG_SPECIAL; numMPQ_set_int(a,0); return false; }
  mpq_set_d(a,k);
  return true;
}
static inline bool numMPQ_set_mpfr(numMPQ_t a, mpfr_t b, num_internal_t intern)
{
  mp_exp_t e;
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numMPQ_set_int(a,0); return false; }
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(mpq_numref(a),b);
  mpz_set_si(mpq_denref(a),1);
  if (e>0) mpq_mul_2exp(a,a,e);
  if (e<0) mpq_div_2exp(a,a,-e);
  return true;
}
static inline bool numMPQ_set_ldouble(numMPQ_t a, long double b, num_internal_t intern)
{
  mp_exp_t e;
  if (!isfinite(b)) { DEBUG_SPECIAL; mpq_set_si(a,0,1); return false; }
  int res = mpfr_set_ld(intern->ldbl,b,GMP_RNDU);
  numMPQ_set_mpfr(a,intern->ldbl,intern);
  return (res==0);
}
static inline bool lint_set_numMPQ(long int* a, numMPQ_t b, num_internal_t intern)
{ return numIl_set_mpq(a,b,intern); }
static inline bool llint_set_numMPQ(long long int* a, numMPQ_t b, num_internal_t intern)
{ return numIll_set_mpq(a,b,intern); }
static inline bool mpz_set_numMPQ(mpz_t a, numMPQ_t b, num_internal_t intern)
{ return numMPZ_set_mpq(a,b,intern); }
static inline bool lfrac_set_numMPQ(long int* i, long int* j, numMPQ_t b, num_internal_t intern)
{
  numIl_set_mpz(i,mpq_numref(b),intern);
  numIl_set_mpz(j,mpq_denref(b),intern);
  return true;
}
static inline bool llfrac_set_numMPQ(long long int* i, long long int* j, numMPQ_t b, num_internal_t intern)
{
  numIll_set_mpz(i,mpq_numref(b),intern);
  numIll_set_mpz(j,mpq_denref(b),intern);
  return true;
}
static inline bool mpq_set_numMPQ(mpq_t a, numMPQ_t b, num_internal_t intern)
{ mpq_set(a,b); return true; }
/* mpfr should have exactly a precision of 53 bits */
static inline bool double_set_numMPQ(double* a, numMPQ_t b, num_internal_t intern)
{
  int res = mpfr_set_q(intern->dbl,b,GMP_RNDU);
  *a = mpfr_get_d(intern->dbl,GMP_RNDU); /* should be exact */
  return (res==0);
}
static inline bool ldouble_set_numMPQ(long double* a, numMPQ_t b, num_internal_t intern)
{
  int res = mpfr_set_q(intern->ldbl,b,GMP_RNDU);
  *a = mpfr_get_ld(intern->ldbl,GMP_RNDU); /* should be exact */
  return (res==0);
}
static inline bool mpfr_set_numMPQ(mpfr_t a, numMPQ_t b, num_internal_t intern)
{ return !mpfr_set_q(a,b,GMP_RNDU); }

/* ********************************************************************** */
/* Underlying integer */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numintMPQ_set(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_set(a,b); }
static inline void numintMPQ_set_array(numintMPQ_t* a, numintMPQ_t* b, size_t size)
	      { numMPZ_set_array(a,b,size); }
static inline void numintMPQ_set_int(numintMPQ_t a, long int i)
	      { numMPZ_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintMPQ_init(numintMPQ_t a)
	      { numMPZ_init(a); }
static inline void numintMPQ_init_array(numintMPQ_t* a, size_t size)
	      { numMPZ_init_array(a,size); }
static inline void numintMPQ_init_set(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_init_set(a,b); }
static inline void numintMPQ_init_set_int(numintMPQ_t a, long int i)
	      { numMPZ_init_set_int(a,i); }

static inline void numintMPQ_clear(numintMPQ_t a)
	      { numMPZ_clear(a); }
static inline void numintMPQ_clear_array(numintMPQ_t* a, size_t size)
	      { numMPZ_clear_array(a,size); }

static inline void numintMPQ_swap(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintMPQ_neg(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_neg(a,b); }
static inline void numintMPQ_abs(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_abs(a,b); }
static inline void numintMPQ_add(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_add(a,b,c); }
static inline void numintMPQ_add_uint(numintMPQ_t a, numintMPQ_t b, unsigned long int c)
	      { numMPZ_add_uint(a,b,c); }
static inline void numintMPQ_sub(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_sub(a,b,c); }
static inline void numintMPQ_sub_uint(numintMPQ_t a, numintMPQ_t b, unsigned long int c)
	      { numMPZ_sub_uint(a,b,c); }
static inline void numintMPQ_mul(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_mul(a,b,c); }
static inline void numintMPQ_mul_2(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_mul_2(a,b); }
static inline void numintMPQ_div(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_cdiv_q(a,b,c); }
static inline void numintMPQ_div_2(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_cdiv_2(a,b); }
static inline void numintMPQ_min(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_min(a,b,c); }
static inline void numintMPQ_max(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
	      { numMPZ_max(a,b,c); }
static inline void numintMPQ_mul_2exp(numintMPQ_t a, numintMPQ_t b, int c)
	      { numMPZ_mul_2exp(a,b,c); }

static inline void numintMPQ_floor(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_set(a,b); }
static inline void numintMPQ_ceil(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_set(a,b); }
static inline void numintMPQ_trunc(numintMPQ_t a, numintMPQ_t b)
	      { numMPZ_set(a,b); }
static inline void numintMPQ_sqrt(numintMPQ_t up, numintMPQ_t down, numintMPQ_t b)
	      { numMPZ_sqrt(up,down,b); }

static inline void numintMPQ_divexact(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
              { numMPZ_divexact(a,b,c); }
static inline void numintMPQ_gcd(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
              { numMPZ_gcd(a,b,c); }
static inline void numintMPQ_lcm(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
              { numMPZ_lcm(a,b,c); }
static inline void numintMPQ_fdiv_q(numintMPQ_t a, numintMPQ_t b, numintMPQ_t c)
              { numMPZ_fdiv_q(a,b,c); }
static inline void numintMPQ_cdiv_q(numintMPQ_t q, numintMPQ_t a, numintMPQ_t b)
              { numMPZ_cdiv_q(q,a,b); }
static inline void numintMPQ_tdiv_q(numintMPQ_t q, numintMPQ_t a, numintMPQ_t b)
              { numMPZ_tdiv_q(q,a,b); }
static inline void numintMPQ_cdiv_qr(numintMPQ_t q, numintMPQ_t r, numintMPQ_t a, numintMPQ_t b)
              { numMPZ_cdiv_qr(q,r,a,b); }
static inline void numintMPQ_cdiv_2(numintMPQ_t a, numintMPQ_t b)
              { numMPZ_cdiv_2(a,b); }
static inline void numintMPQ_cdiv_q_2exp(numintMPQ_t a, numintMPQ_t b, unsigned long int c)
              { numMPZ_cdiv_q_2exp(a,b,c); }
static inline void numintMPQ_fdiv_q_2exp(numintMPQ_t a, numintMPQ_t b, unsigned long int c)
              { numMPZ_fdiv_q_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintMPQ_sgn(numintMPQ_t a)
	 { return numMPZ_sgn(a); }
static inline int numintMPQ_cmp(numintMPQ_t a, numintMPQ_t b)
      { return numMPZ_cmp(a,b); }
static inline int numintMPQ_cmp_int(numintMPQ_t a, long int b)
      { return numMPZ_cmp_int(a,b); }
static inline bool numintMPQ_equal(numintMPQ_t a, numintMPQ_t b)
      { return numMPZ_equal(a,b); }
static inline bool numintMPQ_integer(numintMPQ_t a)
      { return true; }
static inline int numintMPQ_hash(numintMPQ_t a)
      { return numMPZ_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintMPQ_print(numintMPQ_t a)
	      { numMPZ_print(a); }
static inline void numintMPQ_fprint(FILE* stream, numintMPQ_t a)
	      { numMPZ_fprint(stream, a); }
static inline int numintMPQ_snprint(char* s, size_t size, numintMPQ_t a)
      { return numMPZ_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintMPQ_serialize_id(void)
{ return numMPZ_serialize_id(); }

static inline size_t numintMPQ_serialize(void* dst, numintMPQ_t src)
{ return numMPZ_serialize(dst,src); }

static inline size_t numintMPQ_deserialize(numintMPQ_t dst, const void* src)
{ return numMPZ_deserialize(dst,src); }

static inline size_t numintMPQ_serialized_size(numintMPQ_t src)
{ return numMPZ_serialized_size(src); }

static inline size_t numintMPQ_serialize_array(void* dst, numintMPQ_t* src, size_t size)
{ return numMPZ_serialize_array(dst,src,size); }

static inline size_t numintMPQ_deserialize_array(numintMPQ_t* dst, const void* src, size_t size)
{ return numMPZ_deserialize_array(dst,src,size); }

static inline size_t numintMPQ_serialized_size_array(numintMPQ_t* src, size_t size)
{ return numMPZ_serialized_size_array(src,size); }

#ifdef __cplusplus
}
#endif

#endif
