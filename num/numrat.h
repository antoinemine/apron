/* ********************************************************************** */
/* numrat.h */
/* ********************************************************************** */

#ifndef _NUMRAT_H_
#define _NUMRAT_H_

#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"
#include "ap_scalar.h"
#include "num_config.h"

/* We assume that the basic NUMINT on which rational is built is properly
   defined */
#include "numint.h"

/* Two main cases: for NUMINT_MPZ, the scaling to rational is already done */
#if defined(NUMINT_MPZ)
#include "numrat_mpq.h"
#elif defined(NUMINT_NATIVE)
#include "numrat_native.h"
#else
#error "HERE"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numrat_set(numrat_t a, numrat_t b);
static inline void numrat_set_array(numrat_t* a, numrat_t* b, size_t size);
static inline void numrat_set_int(numrat_t a, long int i);

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numrat_init(numrat_t a);
static inline void numrat_init_array(numrat_t* a, size_t size);
static inline void numrat_init_set(numrat_t a, numrat_t b);
static inline void numrat_init_set_int(numrat_t a, long int i);

static inline void numrat_clear(numrat_t a);
static inline void numrat_clear_array(numrat_t* a, size_t size);

static inline void numrat_swap(numrat_t a, numrat_t b)
{ numrat_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numrat_neg(numrat_t a, numrat_t b);
static inline void numrat_inv(numrat_t a, numrat_t b);
static inline void numrat_abs(numrat_t a, numrat_t b);
static inline void numrat_add(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_add_uint(numrat_t a, numrat_t b, unsigned long int c);
static inline void numrat_sub(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_sub_uint(numrat_t a, numrat_t b, unsigned long int c);
static inline void numrat_mul(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_mul_2(numrat_t a, numrat_t b);
static inline void numrat_div(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_div_2(numrat_t a, numrat_t b);
static inline void numrat_min(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_max(numrat_t a, numrat_t b, numrat_t c);
static inline void numrat_floor(numrat_t a, numrat_t b);
static inline void numrat_ceil(numrat_t a, numrat_t b);
static inline void numrat_trunc(numrat_t a, numrat_t b);
static inline void numrat_sqrt(numrat_t up, numrat_t down, numrat_t b);
static inline int  numrat_pow(numrat_t up, numrat_t down, numrat_t b, unsigned long int n);
static inline void numrat_root(numrat_t up, numrat_t down, numrat_t b, unsigned long int n);
static inline void numrat_mul_2exp(numrat_t a, numrat_t b, int c);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numrat_sgn(numrat_t a);
static inline int numrat_cmp(numrat_t a, numrat_t b);
static inline int numrat_cmp_int(numrat_t a, long int b);
static inline bool numrat_equal(numrat_t a, numrat_t b);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numrat_print(numrat_t a);
static inline void numrat_fprint(FILE* stream, numrat_t a);
static inline int numrat_snprint(char* s, size_t size, numrat_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numrat_set_int2(numrat_t a, long int i, long int j);
  /* int2 -> numrat */
static inline bool numrat_set_mpz(numrat_t a, mpz_t b);
  /* mpz -> numrat */
static inline bool numrat_set_mpq(numrat_t a, mpq_t b);
  /* mpq -> numrat */
static inline bool numrat_set_double(numrat_t a, double b);
  /* double -> numrat */
static inline bool numrat_set_mpfr(numrat_t a, mpfr_t b);
  /* mpfr -> numrat */
static inline bool numrat_set_ap_scalar(numrat_t a, ap_scalar_t* b);
  /* (finite) ap_scalar -> numrat */
static inline bool int_set_numrat(long int* a, numrat_t b);
  /* numrat -> int */
static inline bool mpz_set_numrat(mpz_t a, numrat_t b);
  /* numrat -> mpz */
static inline bool mpq_set_numrat(mpq_t a, numrat_t b);
  /* numrat -> mpq */
static inline bool double_set_numrat(double* a, numrat_t b);
  /* numrat -> double */
static inline bool mpfr_set_numrat(mpfr_t a, numrat_t b);
  /* numrat -> mpfr */
static inline bool ap_scalar_set_numrat(ap_scalar_t* a, numrat_t b);
  /* numrat -> ap_scalar */

static inline bool mpz_fits_numrat(mpz_t a);
static inline bool mpq_fits_numrat(mpq_t a);
static inline bool double_fits_numrat(double a);
static inline bool mpfr_fits_numrat(mpfr_t a);
static inline bool numrat_fits_int(numrat_t a);
static inline bool numrat_fits_float(numrat_t a);
static inline bool numrat_fits_double(numrat_t a);
static inline bool numrat_fits_mpfr(numrat_t a);

/* Optimized versions */
static inline bool int_set_numrat_tmp(long int* a, numrat_t b,
				      mpz_t q, mpz_t r);
static inline bool mpz_set_numrat_tmp(mpz_t a, numrat_t b, mpz_t mpz);
/* mpfr should have exactly the precision 53 bits (DOUBLE_MANT_DIG) */
static inline bool double_set_numrat_tmp(double* a, numrat_t b,
					 mpq_t mpq, mpfr_t mpfr);
static inline bool numrat_set_double_tmp(numrat_t a, double k, mpq_t mpq);
static inline bool double_fits_numrat_tmp(double k, mpq_t mpq);


/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numrat_canonicalize(numrat_t a);
static inline bool numrat_set_numint2(numrat_t a, numint_t b, numint_t c);

/*
static inline numint_t numrat_numref(numrat_t a);
static inline numint_t numrat_denref(numrat_t a);
*/


/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numrat_serialize_id(void);
static inline size_t numrat_serialize(void* dst, numrat_t src);
static inline size_t numrat_deserialize(numrat_t dst, const void* src);
static inline size_t numrat_serialized_size(numrat_t a);


/* */
static inline bool numrat_set_ap_scalar(numrat_t a, ap_scalar_t* b)
{
  assert (ap_scalar_infty(b)==0);
  switch (b->discr){
  case AP_SCALAR_MPQ:
    return numrat_set_mpq(a,b->val.mpq);
  case AP_SCALAR_DOUBLE:
    return numrat_set_double(a,b->val.dbl);
  case AP_SCALAR_MPFR:
    return numrat_set_mpfr(a,b->val.mpfr);
  default: abort();
  }
}
static inline bool ap_scalar_set_numrat(ap_scalar_t* a, numrat_t b)
{
  ap_scalar_reinit(a,AP_SCALAR_MPQ);
  return mpq_set_numrat(a->val.mpq,b);
}

#ifdef __cplusplus
}
#endif

#endif
