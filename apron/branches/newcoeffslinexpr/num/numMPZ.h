/* ********************************************************************** */
/* numMPZ.h */
/* ********************************************************************** */

#ifndef _NUMMPZ_DEF_H_
#define _NUMMPZ_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "numTypes.h"
#include "numIll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Require C99 compliant compiler */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numMPZ_set(numMPZ_t a, numMPZ_t b)
{ mpz_set(a,b); }

static inline void numMPZ_set_array(numMPZ_t* a, numMPZ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_set(a[i],b[i]);
}
static inline void numMPZ_set_int(numMPZ_t a, long int i)
{ mpz_set_si(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numMPZ_init(numMPZ_t a)
{ mpz_init(a); }
static inline void numMPZ_init_array(numMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_init(a[i]);
}
static inline void numMPZ_init_set(numMPZ_t a, numMPZ_t b)
{ mpz_init_set(a,b); }
static inline void numMPZ_init_set_int(numMPZ_t a, long int i)
{ mpz_init_set_si(a,i); }

static inline void numMPZ_clear(numMPZ_t a)
{ mpz_clear(a); }
static inline void numMPZ_clear_array(numMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_clear(a[i]);
}
static inline void numMPZ_swap(numMPZ_t a, numMPZ_t b)
{ mpz_swap(a,b); }
/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numMPZ_neg(numMPZ_t a, numMPZ_t b)
{ mpz_neg(a,b); }
static inline void numMPZ_abs(numMPZ_t a, numMPZ_t b)
{ mpz_abs(a,b); }
static inline void numMPZ_add(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_add(a,b,c); }
static inline void numMPZ_add_uint(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_add_ui(a,b,c); }
static inline void numMPZ_sub(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_sub(a,b,c); }
static inline void numMPZ_sub_uint(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_sub_ui(a,b,c); }
static inline void numMPZ_mul(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_mul(a,b,c); }
static inline void numMPZ_mul_2(numMPZ_t a, numMPZ_t b)
{ mpz_mul_2exp(a,b,1); }
static inline void numMPZ_fdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_fdiv_q(a,b,c); }
static inline void numMPZ_cdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_cdiv_q(a,b,c); }
static inline void numMPZ_tdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_tdiv_q(a,b,c); }
static inline void numMPZ_cdiv_qr(numMPZ_t a, numMPZ_t b, numMPZ_t c, numMPZ_t d)
{ mpz_cdiv_qr(a,b,c,d); }
static inline void numMPZ_cdiv_2(numMPZ_t a, numMPZ_t b)
{ mpz_cdiv_q_2exp(a,b,1); }
static inline void numMPZ_cdiv_q_2exp(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_cdiv_q_2exp(a,b,c); }
static inline void numMPZ_fdiv_q_2exp(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_fdiv_q_2exp(a,b,c); }
static inline void numMPZ_min(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)<=0 ? b : c); }
static inline void numMPZ_max(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)>=0 ? b : c); }

static inline void numMPZ_sqrt(numMPZ_t up, numMPZ_t down, numMPZ_t b)
{
  int perfect;
  assert(mpz_sgn(b)>=0);
  perfect = mpz_perfect_square_p(b);
  mpz_sqrt(down,b);
  if (perfect) mpz_set(up,down);
  else mpz_add_ui(up,down,1);
}

static inline void numMPZ_mul_2exp(numMPZ_t a, numMPZ_t b, int c)
{
  if (c>=0) mpz_mul_2exp(a,b,c);
  else mpz_cdiv_q_2exp(a,b,-c);
}

static inline void numMPZ_trunc(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_floor(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_ceil(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_div(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ numMPZ_cdiv_q(a,b,c); }
static inline void numMPZ_div_2(numMPZ_t a, numMPZ_t b)
{ numMPZ_cdiv_2(a,b); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numMPZ_divexact(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_divexact(a,b,c); }
static inline void numMPZ_mod(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_mod(a,b,c); }
static inline void numMPZ_gcd(numMPZ_t a, numMPZ_t b,  numMPZ_t c)
{ mpz_gcd(a,b,c); }
static inline void numMPZ_lcm(numMPZ_t a, numMPZ_t b,  numMPZ_t c)
{ mpz_lcm(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numMPZ_sgn(numMPZ_t a)
{ return mpz_sgn(a); }
static inline int numMPZ_cmp(numMPZ_t a, numMPZ_t b)
{ return mpz_cmp(a,b); }
static inline int numMPZ_cmp_int(numMPZ_t a, long int b)
{ return mpz_cmp_si(a,b); }
static inline bool numMPZ_equal(numMPZ_t a, numMPZ_t b)
{ return mpz_cmp(a,b)==0; }
static inline bool numMPZ_integer(numMPZ_t a)
{ return true; }
static inline int numMPZ_hash(numMPZ_t a)
{ return mpz_get_si(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numMPZ_print(numMPZ_t a)
{ mpz_out_str(stdout,10,a); }
static inline void numMPZ_fprint(FILE* stream, numMPZ_t a)
{ mpz_out_str(stream,10,a); }
static inline int numMPZ_snprint(char* s, size_t size, numMPZ_t a)
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
/* Serialization */
/* ====================================================================== */

static inline unsigned char numMPZ_serialize_id(void)
{ return 0xf; }

static inline size_t numMPZ_serialize(void* dst, numMPZ_t src)
{
  size_t count = 0;
  *((char*)dst) = mpz_sgn(src);
  mpz_export((char*)dst+5,&count,1,1,1,0,src);
  assert(((unsigned)count)==count);
  num_dump_word32((char*)dst+1,(unsigned)count);
  return count+5;
}

static inline size_t numMPZ_deserialize(numMPZ_t dst, const void* src)
{
  size_t count = num_undump_word32((const char*)src+1);
  mpz_import(dst,count,1,1,1,0,(const char*)src+5);
  if (*(const char*)src<0)
    mpz_neg(dst,dst);
  return count+5;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numMPZ_serialized_size(numMPZ_t a)
{ return mpz_sizeinbase(a,2)/8+5+sizeof(mp_limb_t); }

static inline size_t numMPZ_serialize_array(void* dst, numMPZ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t numMPZ_deserialize_array(numMPZ_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t numMPZ_serialized_size_array(numMPZ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Fits */
/* ====================================================================== */

static inline bool lint_fits_numMPZ(long int a)
{ return true; }
static inline bool llint_fits_numMPZ(long long int a)
{ return true; }
static inline bool mpz_fits_numMPZ(mpz_t a)
{ return true; }
static inline bool lfrac_fits_numMPZ(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numMPZ(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numMPZ(mpq_t a)
{ return true; }
static inline bool double_fits_numMPZ(double a)
{ return isfinite(a); }
static inline bool ldouble_fits_numMPZ(long double a)
{ return isfinite(a); }
static inline bool mpfr_fits_numMPZ(mpfr_t a, numinternal_t intern)
{ return mpfr_number_p(a); }

static inline bool numMPZ_fits_lint(numMPZ_t a)
{ return mpz_fits_slong_p(a); }
static inline bool numMPZ_fits_llint(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool numMPZ_fits_lfrac(numMPZ_t a)
{ return mpz_fits_slong_p(a); }
static inline bool numMPZ_fits_llfrac(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool numMPZ_fits_float(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)<FLT_MAX_EXP-1); }
static inline bool numMPZ_fits_double(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)<DBL_MAX_EXP-1); }
static inline bool numMPZ_fits_ldouble(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)<LDBL_MAX_EXP-1); }
static inline bool numMPZ_fits_mpfr(numMPZ_t a)
{ return (mpz_sizeinbase(a,2)+1<(size_t)mpfr_get_emax()); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numMPZ_set_lint(numMPZ_t a, long int b, numinternal_t intern)
{ mpz_set_si(a,b); return true; }
static inline bool numMPZ_set_llint(numMPZ_t a, long long int b, numinternal_t intern)
{ return mpz_set_numIll(a,&b,intern); }
static inline bool numMPZ_set_mpz(numMPZ_t a, mpz_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }
static inline bool numMPZ_set_lfrac(numMPZ_t a, long int i, long int j, numinternal_t intern)
{ 
  long int q = (i>=0) ? (i+j-1)/j : i/j;
  mpz_set_si(a,q);
  return (i%j==0);
}
static inline bool numMPZ_set_llfrac(numMPZ_t a, long long int i, long long int j, numinternal_t intern)
{ 
  long long int q = (i>=0) ? (i+j-1)/j : i/j;
  return numMPZ_set_llint(a,q,intern) && (i%j==0);
}
static inline bool numMPZ_set_mpq(numMPZ_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(a, intern->r, mpq_numref(b),mpq_denref(b));
  return mpz_sgn(intern->r)==0;
}
static inline bool numMPZ_set_double(numMPZ_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  mpz_set_d(a,c);
  return (b==c);
}
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool numMPZ_set_ldouble(numMPZ_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  int res = mpfr_set_ld(intern->ldbl,b,GMP_RNDU);
  mpfr_get_z(a,intern->ldbl,GMP_RNDU);
  return (res==0) && (b==c);
}
static inline bool numMPZ_set_mpfr(numMPZ_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numMPZ_set_int(a,0); return false; }
  mpfr_get_z(a,b,GMP_RNDU);
  return mpfr_integer_p(b);
}

static inline bool lint_set_numMPZ(long int* a, numMPZ_t b, numinternal_t intern)
{ *a = mpz_get_si(b); return true; }
static inline bool llint_set_numMPZ(long long int* a, numMPZ_t b, numinternal_t intern)
{ return numIll_set_mpz(a,b,intern); }
static inline bool mpz_set_numMPZ(mpz_t a, numMPZ_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }
static inline bool lfrac_set_numMPZ(long int* i, long int* j, numMPZ_t b, numinternal_t intern)
{ *i = mpz_get_si(b); *j = 1L; return true; }
static inline bool llfrac_set_numMPZ(long long int* i, long long int* j, numMPZ_t b, numinternal_t intern)
{ *j = 1LL; return llint_set_numMPZ(i,b,intern); }
static inline bool mpq_set_numMPZ(mpq_t a, numMPZ_t b, numinternal_t intern)
{
  mpz_set(mpq_numref(a),b);
  mpz_set_ui(mpq_denref(a),1);
  return true;
}
/* mpfr is supposed to have exactly the IEEE754 double precision of 53 bits */
static inline bool double_set_numMPZ(double* a, numMPZ_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->dbl,b,GMP_RNDU);
  *a = mpfr_get_d(intern->dbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool ldouble_set_numMPZ(long double* a, numMPZ_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->ldbl,b,GMP_RNDU);
  *a = mpfr_get_ld(intern->ldbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
static inline bool mpfr_set_numMPZ(mpfr_t a, numMPZ_t b, numinternal_t intern)
{ return !mpfr_set_z(a,b,GMP_RNDU); }


#ifdef __cplusplus
}
#endif

#endif
