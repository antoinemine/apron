/* ********************************************************************** */
/* numintMPZ_def.h */
/* ********************************************************************** */

#ifndef _NUMINTMPZ_DEF_H_
#define _NUMINTMPZ_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "numConfig.h"
#include "numint.h"
#include "numintIll_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Require C99 compliant compiler */

#undef NUMINTMPZ_ZERO
#undef NUMINTMPZ_ONE
#undef NUMINTMPZ_MAX
#undef NUMMPZ_MAX
#undef NUMINTMPZ_MIN

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numintMPZ_set(numintMPZ_t a, numintMPZ_t b)
{ mpz_set(a,b); }
static inline void numintMPZ_set_array(numintMPZ_t* a, numintMPZ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_set(a[i],b[i]);
}
static inline void numintMPZ_set_int(numintMPZ_t a, long int i)
{ mpz_set_si(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintMPZ_init(numintMPZ_t a)
{ mpz_init(a); }
static inline void numintMPZ_init_array(numintMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_init(a[i]);
}
static inline void numintMPZ_init_set(numintMPZ_t a, numintMPZ_t b)
{ mpz_init_set(a,b); }
static inline void numintMPZ_init_set_int(numintMPZ_t a, long int i)
{ mpz_init_set_si(a,i); }

static inline void numintMPZ_clear(numintMPZ_t a)
{ mpz_clear(a); }
static inline void numintMPZ_clear_array(numintMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_clear(a[i]);
}
static inline void numintMPZ_swap(numintMPZ_t a, numintMPZ_t b)
{ mpz_swap(a,b); }
/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintMPZ_neg(numintMPZ_t a, numintMPZ_t b)
{ mpz_neg(a,b); }
static inline void numintMPZ_abs(numintMPZ_t a, numintMPZ_t b)
{ mpz_abs(a,b); }
static inline void numintMPZ_add(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_add(a,b,c); }
static inline void numintMPZ_add_uint(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_add_ui(a,b,c); }
static inline void numintMPZ_sub(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_sub(a,b,c); }
static inline void numintMPZ_sub_uint(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_sub_ui(a,b,c); }
static inline void numintMPZ_mul(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_mul(a,b,c); }
static inline void numintMPZ_mul_2(numintMPZ_t a, numintMPZ_t b)
{ mpz_mul_2exp(a,b,1); }
static inline void numintMPZ_fdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_fdiv_q(a,b,c); }
static inline void numintMPZ_cdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_cdiv_q(a,b,c); }
static inline void numintMPZ_tdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_tdiv_q(a,b,c); }
static inline void numintMPZ_cdiv_qr(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c, numintMPZ_t d)
{ mpz_cdiv_qr(a,b,c,d); }
static inline void numintMPZ_cdiv_2(numintMPZ_t a, numintMPZ_t b)
{ mpz_cdiv_q_2exp(a,b,1); }
static inline void numintMPZ_cdiv_q_2exp(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_cdiv_q_2exp(a,b,c); }
static inline void numintMPZ_fdiv_q_2exp(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_fdiv_q_2exp(a,b,c); }
static inline void numintMPZ_min(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)<=0 ? b : c); }
static inline void numintMPZ_max(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)>=0 ? b : c); }

static inline void numintMPZ_sqrt(numintMPZ_t up, numintMPZ_t down, numintMPZ_t b)
{
  int perfect;
  assert(mpz_sgn(b)>=0);
  perfect = mpz_perfect_square_p(b);
  mpz_sqrt(down,b);
  if (perfect) mpz_set(up,down);
  else mpz_add_ui(up,down,1);
}

static inline void numintMPZ_mul_2exp(numintMPZ_t a, numintMPZ_t b, int c)
{
  if (c>=0) mpz_mul_2exp(a,b,c);
  else mpz_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numintMPZ_divexact(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_divexact(a,b,c); }
static inline void numintMPZ_mod(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_mod(a,b,c); }
static inline void numintMPZ_gcd(numintMPZ_t a, numintMPZ_t b,  numintMPZ_t c)
{ mpz_gcd(a,b,c); }
static inline void numintMPZ_lcm(numintMPZ_t a, numintMPZ_t b,  numintMPZ_t c)
{ mpz_lcm(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintMPZ_sgn(numintMPZ_t a)
{ return mpz_sgn(a); }
static inline int numintMPZ_cmp(numintMPZ_t a, numintMPZ_t b)
{ return mpz_cmp(a,b); }
static inline int numintMPZ_cmp_int(numintMPZ_t a, long int b)
{ return mpz_cmp_si(a,b); }
static inline bool numintMPZ_equal(numintMPZ_t a, numintMPZ_t b)
{ return mpz_cmp(a,b)==0; }
static inline bool numintMPZ_integer(numintMPZ_t a)
{ return true; }
static inline int numintMPZ_hash(numintMPZ_t a)
{ return mpz_get_si(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintMPZ_print(numintMPZ_t a)
{ mpz_out_str(stdout,10,a); }
static inline void numintMPZ_fprint(FILE* stream, numintMPZ_t a)
{ mpz_out_str(stream,10,a); }
static inline int numintMPZ_snprint(char* s, size_t size, numintMPZ_t a)
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

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numintMPZ(long int a)
{ return true; }
static inline bool llint_fits_numintMPZ(long long int a)
{ return true; }
static inline bool mpz_fits_numintMPZ(mpz_t a)
{ return true; }
static inline bool lfrac_fits_numintMPZ(long int i, long int j)
{ return true; }
static inline bool llfrac_fits_numintMPZ(long long int i, long long int j)
{ return true; }
static inline bool mpq_fits_numintMPZ(mpq_t a)
{ return true; }
static inline bool double_fits_numintMPZ(double a)
{ return isfinite(a); }
static inline bool ldouble_fits_numintMPZ(long double a)
{ return isfinite(a); }
static inline bool mpfr_fits_numintMPZ(mpfr_t a, numinternal_t intern)
{ return mpfr_number_p(a); }

static inline bool numintMPZ_fits_lint(numintMPZ_t a)
{ return mpz_fits_slong_p(a); }
static inline bool numintMPZ_fits_llint(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool numintMPZ_fits_lfrac(numintMPZ_t a)
{ return mpz_fits_slong_p(a); }
static inline bool numintMPZ_fits_llfrac(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)<sizeof(long long int)*8-1); }
static inline bool numintMPZ_fits_float(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)<FLT_MAX_EXP-1); }
static inline bool numintMPZ_fits_double(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)<DBL_MAX_EXP-1); }
static inline bool numintMPZ_fits_ldouble(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)<LDBL_MAX_EXP-1); }
static inline bool numintMPZ_fits_mpfr(numintMPZ_t a)
{ return (mpz_sizeinbase(a,2)+1<(size_t)mpfr_get_emax()); }

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

/* lint -> numint */
static inline bool numintMPZ_set_lint(numintMPZ_t a, long int b, numinternal_t intern)
{ mpz_set_si(a,b); return true; }

/* llint -> numint */
static inline bool numintMPZ_set_llint(numintMPZ_t a, long long int b, numinternal_t intern)
{ return mpz_set_numintIll(a,&b,intern); }

/* mpz -> numint */
static inline bool numintMPZ_set_mpz(numintMPZ_t a, mpz_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }

/* lfrac -> numint */
static inline bool numintMPZ_set_lfrac(numintMPZ_t a, long int i, long int j, numinternal_t intern)
{ 
  long int q = (i>=0) ? (i+j-1)/j : i/j;
  mpz_set_si(a,q);
  return (i%j==0);
}

/* llfrac -> numint */
static inline bool numintMPZ_set_llfrac(numintMPZ_t a, long long int i, long long int j, numinternal_t intern)
{ 
  long long int q = (i>=0) ? (i+j-1)/j : i/j;
  return numintMPZ_set_llint(a,q,intern) && (i%j==0);
}

/* mpq -> numint */
static inline bool numintMPZ_set_mpq(numintMPZ_t a, mpq_t b, numinternal_t intern)
{
  mpz_cdiv_qr(a, intern->r, mpq_numref(b),mpq_denref(b));
  return mpz_sgn(intern->r)==0;
}

/* double -> numint */
static inline bool numintMPZ_set_double(numintMPZ_t a, double b, numinternal_t intern)
{
  double c = ceil(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  mpz_set_d(a,c);
  return (b==c);
}

/* ldouble -> numint */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool numintMPZ_set_ldouble(numintMPZ_t a, long double b, numinternal_t intern)
{
  long double c = ceill(b);
  if (!isfinite(c)) { DEBUG_SPECIAL; mpz_set_si(a,0); return false; }
  int res = mpfr_set_ld(intern->ldbl,b,GMP_RNDU);
  mpfr_get_z(a,intern->ldbl,GMP_RNDU);
  return (res==0) && (b==c);
}

/* mpfr -> numint */
static inline bool numintMPZ_set_mpfr(numintMPZ_t a, mpfr_t b, numinternal_t intern)
{
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numintMPZ_set_int(a,0); return false; }
  mpfr_get_z(a,b,GMP_RNDU);
  return mpfr_integer_p(b);
}

/* numint -> lint */
static inline bool lint_set_numintMPZ(long int* a, numintMPZ_t b, numinternal_t intern)
{ *a = mpz_get_si(b); return true; }

/* numint -> llint */
static inline bool llint_set_numintMPZ(long long int* a, numintMPZ_t b, numinternal_t intern)
{ return numintIll_set_mpz(a,b,intern); }

/* numint -> mpz */
static inline bool mpz_set_numintMPZ(mpz_t a, numintMPZ_t b, numinternal_t intern)
{ mpz_set(a,b); return true; }

/* numint -> lfrac */
static inline bool lfrac_set_numintMPZ(long int* i, long int* j, numintMPZ_t b, numinternal_t intern)
{ *i = mpz_get_si(b); *j = 1L; return true; }

/* numint -> llfrac */
static inline bool llfrac_set_numintMPZ(long long int* i, long long int* j, numintMPZ_t b, numinternal_t intern)
{ *j = 1LL; return llint_set_numintMPZ(i,b,intern); }

/* numint -> mpq */
static inline bool mpq_set_numintMPZ(mpq_t a, numintMPZ_t b, numinternal_t intern)
{
  mpz_set(mpq_numref(a),b);
  mpz_set_ui(mpq_denref(a),1);
  return true;
}

/* numint -> double */
/* mpfr is supposed to have exactly the IEEE754 double precision of 53 bits */
static inline bool double_set_numintMPZ(double* a, numintMPZ_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->dbl,b,GMP_RNDU);
  *a = mpfr_get_d(intern->dbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
/* numint -> ldouble */
/* mpfr is supposed to have exactly the IEEE754 double precision of NUMFLTDL_MANT_DIG bits */
static inline bool ldouble_set_numintMPZ(long double* a, numintMPZ_t b, numinternal_t intern)
{
  int res = mpfr_set_z(intern->ldbl,b,GMP_RNDU);
  *a = mpfr_get_ld(intern->ldbl,GMP_RNDU);/* Normally, exact conversion here (unless overflow) */
  return (res==0);
}
/* numint -> mpfr */
static inline bool mpfr_set_numintMPZ(mpfr_t a, numintMPZ_t b, numinternal_t intern)
{ return !mpfr_set_z(a,b,GMP_RNDU); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintMPZ_serialize_id(void)
{ return 0xf; }

static inline size_t numintMPZ_serialize(void* dst, numintMPZ_t src)
{
  size_t count = 0;
  *((char*)dst) = mpz_sgn(src);
  mpz_export((char*)dst+5,&count,1,1,1,0,src);
  assert(((unsigned)count)==count);
  num_dump_word32((char*)dst+1,(unsigned)count);
  return count+5;
}

static inline size_t numintMPZ_deserialize(numintMPZ_t dst, const void* src)
{
  size_t count = num_undump_word32((const char*)src+1);
  mpz_import(dst,count,1,1,1,0,(const char*)src+5);
  if (*(const char*)src<0)
    mpz_neg(dst,dst);
  return count+5;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numintMPZ_serialized_size(numintMPZ_t a)
{ return mpz_sizeinbase(a,2)/8+5+sizeof(mp_limb_t); }

#ifdef __cplusplus
}
#endif

#endif
