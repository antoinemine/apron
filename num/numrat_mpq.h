/* ********************************************************************** */
/* rational_mpq.h */
/* ********************************************************************** */

#ifndef _NUMRAT_MPQ_H_
#define _NUMRAT_MPQ_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"
#include "ap_scalar.h"

#include "num_config.h"

#ifdef NUMINT_MPZ
#include "numint.h"
#else
#error "HERE"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef mpq_t numrat_t;


/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numrat_canonicalize(numrat_t a)
{ mpq_canonicalize(a); }

#define numrat_numref(a) mpq_numref(a)
#define numrat_denref(a) mpq_denref(a)

static inline bool numrat_set_numint2(numrat_t a, numint_t b, numint_t c)
{
  numint_set(numrat_numref(a),b);
  numint_set(numrat_denref(a),c);
  numrat_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numrat_set(numrat_t a, numrat_t b)
{ mpq_set(a,b); }
static inline void numrat_set_array(numrat_t* a, numrat_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_set(a[i],b[i]);
}
static inline void numrat_set_int(numrat_t a, long int i)
{ mpq_set_si(a,i,1); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numrat_init(numrat_t a)
{ mpq_init(a); }
static inline void numrat_init_array(numrat_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_init(a[i]);
}
static inline void numrat_init_set(numrat_t a, numrat_t b)
{ mpq_init(a); mpq_set(a,b); }
static inline void numrat_init_set_int(numrat_t a, long int i)
{ mpq_init(a); mpq_set_si(a,i,1); }

static inline void numrat_clear(numrat_t a)
{ mpq_clear(a); }
static inline void numrat_clear_array(numrat_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_clear(a[i]);
}

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numrat_neg(numrat_t a, numrat_t b)
{ mpq_neg(a,b); }
static inline void numrat_inv(numrat_t a, numrat_t b)
{ mpq_inv(a,b); }
static inline void numrat_abs(numrat_t a, numrat_t b)
{ mpq_abs(a,b); }
static inline void numrat_add(numrat_t a, numrat_t b, numrat_t c)
{ mpq_add(a,b,c); }
static inline void numrat_add_uint(numrat_t a, numrat_t b, unsigned long int c)
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
static inline void numrat_sub(numrat_t a, numrat_t b, numrat_t c)
{ mpq_sub(a,b,c); }
static inline void numrat_sub_uint(numrat_t a, numrat_t b, unsigned long int c)
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
static inline void numrat_mul(numrat_t a, numrat_t b, numrat_t c)
{ mpq_mul(a,b,c); }
static inline void numrat_mul_2(numrat_t a, numrat_t b)
{ mpq_mul_2exp(a,b,1); }
static inline void numrat_div(numrat_t a, numrat_t b, numrat_t c)
{ mpq_div(a,b,c); }
static inline void numrat_div_2(numrat_t a, numrat_t b)
{ mpq_div_2exp(a,b,1); }
static inline void numrat_min(numrat_t a, numrat_t b, numrat_t c)
{ mpq_set(a, mpq_cmp(b,c)<=0 ? b : c); }
static inline void numrat_max(numrat_t a, numrat_t b, numrat_t c)
{ mpq_set(a, mpq_cmp(b,c)>=0 ? b : c); }
static inline void numrat_floor(numrat_t a, numrat_t b)
{
  numint_fdiv_q(numrat_numref(a),numrat_numref(b),numrat_denref(b));
  numint_set_int(numrat_denref(a),1);
}
static inline void numrat_ceil(numrat_t a, numrat_t b)
{
  numint_cdiv_q(numrat_numref(a),numrat_numref(b),numrat_denref(b));
  numint_set_int(numrat_denref(a),1);
}
static inline void numrat_trunc(numrat_t a, numrat_t b)
{
  numint_tdiv_q(numrat_numref(a),numrat_numref(b),numrat_denref(b));
  numint_set_int(numrat_denref(a),1);
}
static inline void numrat_sqrt(numrat_t up, numrat_t down, numrat_t b)
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
  numrat_canonicalize(up);
  numrat_canonicalize(down);
  mpz_clear(tmp);
}

static inline int numrat_pow(numrat_t up, numrat_t down, numrat_t b, unsigned long int n)
{
  mpz_pow_ui(numrat_numref(up), numrat_numref(b), n);
  mpz_pow_ui(numrat_denref(up), numrat_denref(b), n);
  numrat_canonicalize(up);
  mpz_set(numrat_numref(down), numrat_numref(up));
  mpz_set(numrat_denref(down), numrat_denref(up));
 return 0;
}

static inline void numrat_root(numrat_t up, numrat_t down, numrat_t b, unsigned long int n)
{
  numint_root(numrat_numref(up), numrat_numref(down), numrat_numref(b), n);
  numint_root(numrat_denref(down), numrat_denref(up), numrat_denref(b), n);
  numrat_canonicalize(up);
  numrat_canonicalize(down);
}

static inline void numrat_mul_2exp(numrat_t a, numrat_t b, int c)
{
  if (c>=0) mpq_mul_2exp(a,b,c);
  else mpq_div_2exp(a,b,-c);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numrat_sgn(numrat_t a)
{ return mpq_sgn(a); }
static inline int numrat_cmp(numrat_t a, numrat_t b)
{ return mpq_cmp(a,b); }
static inline int numrat_cmp_int(numrat_t a, long int b)
{ return mpq_cmp_si(a,b,1); }
static inline bool numrat_equal(numrat_t a, numrat_t b)
{ return mpq_equal(a,b); }
static inline bool numrat_integer(numrat_t a)
{ return (mpz_cmp_ui(mpq_denref(a),1)==0); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numrat_print(numrat_t a)
{ mpq_out_str(stdout,10,a); }
static inline void numrat_fprint(FILE* stream, numrat_t a)
{ mpq_out_str(stream,10,a); }
static inline int numrat_snprint(char* s, size_t size, numrat_t a)
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
/* Conversions */
/* ====================================================================== */

/* int2 -> numrat */
static inline bool numrat_set_int2(numrat_t a, long int i, long int j)
{ assert(j>0); mpq_set_si(a,i,j); numrat_canonicalize(a); return true; }

/* mpz -> numrat */
static inline bool numrat_set_mpz(numrat_t a, mpz_t b)
{ 
  mpz_set(mpq_numref(a),b); 
  mpz_set_ui(mpq_denref(a),1);
  return true;
}

/* mpq -> numrat */
static inline bool numrat_set_mpq(numrat_t a, mpq_t b)
{ mpq_set(a,b); return true; }

/* double -> numrat */
static inline bool numrat_set_double(numrat_t a, double k)
{ 
  if (!isfinite(k)) { DEBUG_SPECIAL; numrat_set_int(a,0); return false; }
  mpq_set_d(a,k); 
  return true; 
}
static inline bool numrat_set_double_tmp(numrat_t a, double k, mpq_t mpq)
{ (void)mpq; return numrat_set_double(a,k); }
/* mpfr -> numrat */
static inline bool numrat_set_mpfr(numrat_t a, mpfr_t b)
{
  mp_exp_t e;
  if (!mpfr_number_p(b)) { DEBUG_SPECIAL; numrat_set_int(a,0); return false; }
  /* XXX might fail if scaled exponent not representable in mp_exp_t */
  e = mpfr_get_z_exp(mpq_numref(a),b);
  mpz_set_si(mpq_denref(a),1);
  if (e>0) mpq_mul_2exp(a,a,e);
  if (e<0) mpq_div_2exp(a,a,-e);
  return true;
}

/* numrat -> int */
static inline bool int_set_numrat_tmp(long int* a, numrat_t b, 
				      mpz_t q, mpz_t r)
{ 
  mpz_cdiv_qr(q,r,numrat_numref(b),numrat_denref(b));
  *a = mpz_get_si(q);
  return (mpz_sgn(r)==0);
}
static inline bool int_set_numrat(long int* a, numrat_t b)
{ 
  mpz_t q,r;
  mpz_init(q); mpz_init(r);
  bool res = int_set_numrat_tmp(a,b,q,r);
  mpz_clear(q); mpz_clear(r);
  return res;
}
/* numrat -> mpz */
static inline bool mpz_set_numrat_tmp(mpz_t a, numrat_t b, mpz_t mpz)
{
  mpz_cdiv_qr(a, mpz, mpq_numref(b),mpq_denref(b));
  bool res = (mpz_sgn(mpz)==0);
  return res;
}
static inline bool mpz_set_numrat(mpz_t a, numrat_t b)
{
  numint_t r;
  numint_init(r);
  bool res = mpz_set_numrat_tmp(a,b,r);
  numint_clear(r);
  return res;
}
/* numrat -> mpq */
static inline bool mpq_set_numrat(mpq_t a, numrat_t b)
{ mpq_set(a,b); return true; }
/* numrat -> double */
/* mpfr should have exactly a precision of 53 bits */
static inline bool double_set_numrat_tmp(double* a, numrat_t b, 
					 mpq_t mpq, // not used
					 mpfr_t mpfr)
{
  (void)mpq;
  int res = mpfr_set_q(mpfr,b,GMP_RNDU);
  *a = mpfr_get_d(mpfr,GMP_RNDU); /* should be exact */
  return (res==0);
}
static inline bool double_set_numrat(double* a, numrat_t b)
{
  mpfr_t mpfr;
  mpfr_init2(mpfr,53);
  bool res = double_set_numrat_tmp(a,b,NULL,mpfr);
  mpfr_clear(mpfr);
  return res;
}
/* numrat -> mpfr */
static inline bool mpfr_set_numrat(mpfr_t a, numrat_t b)
{ return !mpfr_set_q(a,b,GMP_RNDU); }

static inline bool mpz_fits_numrat(mpz_t a)
{ (void)a; return true; }
static inline bool mpq_fits_numrat(mpq_t a)
{ (void)a; return true; }
static inline bool double_fits_numrat(double k)
{ return isfinite(k); }
static inline bool double_fits_numrat_tmp(double k, mpq_t mpq)
{ (void)mpq; return double_fits_numrat(k); }
static inline bool mpfr_fits_numrat(mpfr_t a)
{ return mpfr_number_p(a); }
static inline bool numrat_fits_int(numrat_t a)
{
  double d = ceil(mpq_get_d(a));
  return (d+1.0)<=LONG_MAX && (d-1.0)>=-LONG_MAX;
}
static inline bool numrat_fits_float(numrat_t a)
{ 
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)<126); 
}
static inline bool numrat_fits_double(numrat_t a)
{ 
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)<1022); 
}
static inline bool numrat_fits_mpfr(numrat_t a)
{ 
  return ((int)mpz_sizeinbase(numrat_numref(a),2)-
	  (int)mpz_sizeinbase(numrat_denref(a),2)+1<mpfr_get_emax()); 
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numrat_serialize_id(void)
{ return 0x1f; }

static inline size_t numrat_serialize(void* dst, numrat_t src)
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

static inline size_t numrat_deserialize(numrat_t dst, const void* src) 
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
static inline size_t numrat_serialized_size(numrat_t a) 
{ 
  return 
    (mpz_sizeinbase(mpq_numref(a),2)+mpz_sizeinbase(mpq_denref(a),2))/8+
    9+2*sizeof(mp_limb_t);
}

#ifdef __cplusplus
}
#endif

#endif
