/* ********************************************************************** */
/* numflt.h */
/* ********************************************************************** */

#ifndef _NUMFLT_H_
#define _NUMFLT_H_

#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"
#include "ap_scalar.h"
#include "num_config.h"

#ifndef NUMFLT_PRINT_PREC
#define NUMFLT_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLT_PRINT_PREC to be any other
   expression (including variable and function call).
*/

#if defined(NUMFLT_DOUBLE) || defined(NUMFLT_LONGDOUBLE)
#include "numflt_native.h"
#elif defined(NUMFLT_MPFR)
#include "numflt_mpfr.h"
#else
#error "HERE"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
   NUMINT_DOUBLE and NUMFLT_LONGDOUBLE also define

   - NUMFLT_MAX: positive infinity value
     It is assumed that (-NUMINT_MAX) is also representable

   - NUMFLT_NATIVE: no heap allocated memory
*/


/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numflt_set(numflt_t a, numflt_t b);
static inline void numflt_set_array(numflt_t* a, numflt_t* b, size_t size);
static inline void numflt_set_int(numflt_t a, long int i);

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numflt_init(numflt_t a);
static inline void numflt_init_array(numflt_t* a, size_t size);
static inline void numflt_init_set(numflt_t a, numflt_t b);
static inline void numflt_init_set_int(numflt_t a, long int i);

static inline void numflt_clear(numflt_t a);
static inline void numflt_clear_array(numflt_t* a, size_t size);

  static inline void numflt_swap(numflt_t a, numflt_t b);

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numflt_neg(numflt_t a, numflt_t b);
static inline void numflt_abs(numflt_t a, numflt_t b);
static inline void numflt_add(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_add_uint(numflt_t a, numflt_t b, unsigned long int c);
static inline void numflt_sub(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_sub_uint(numflt_t a, numflt_t b, unsigned long int c);
static inline void numflt_mul(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_mul_2(numflt_t a, numflt_t b);
static inline void numflt_div(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_div_2(numflt_t a, numflt_t b);
static inline void numflt_min(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_max(numflt_t a, numflt_t b, numflt_t c);
static inline void numflt_floor(numflt_t a, numflt_t b);
static inline void numflt_ceil(numflt_t a, numflt_t b);
static inline void numflt_trunc(numflt_t a, numflt_t b);
static inline void numflt_sqrt(numflt_t up, numflt_t down, numflt_t b);
static inline int  numflt_pow(numflt_t up, numflt_t down, numflt_t b, unsigned long int n);
static inline void numflt_root(numflt_t up, numflt_t down, numflt_t b, unsigned long int n);
static inline void numflt_mul_2exp(numflt_t a, numflt_t b, int c);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numflt_sgn(numflt_t a);
static inline int numflt_cmp(numflt_t a, numflt_t b);
static inline int numflt_cmp_int(numflt_t a, long int b);
static inline bool numflt_equal(numflt_t a, numflt_t b);
static inline bool numflt_integer(numflt_t a);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numflt_print(numflt_t a);
static inline void numflt_fprint(FILE* stream, numflt_t a);
static inline int  numflt_snprint(char* s, size_t size, numflt_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool numflt_set_int2(numflt_t a, long int i, long int j);
  /* int2 -> numflt */
static inline bool numflt_set_mpz(numflt_t a, mpz_t b);
  /* mpz -> numflt */
static inline bool numflt_set_mpq(numflt_t a, mpq_t b);
  /* mpq -> numflt */
static inline bool numflt_set_double(numflt_t a, double b);
  /* double -> numflt */
static inline bool numflt_set_mpfr(numflt_t a, mpfr_t b);
  /* mpfr -> numflt */
static inline bool numflt_set_ap_scalar(numflt_t a, ap_scalar_t* b);
  /* (finite) ap_scalar -> numflt */

static inline bool int_set_numflt(long int* a, numflt_t b);
  /* numflt -> int */
static inline bool mpz_set_numflt(mpz_t a, numflt_t b);
  /* numflt -> mpz */
static inline bool mpq_set_numflt(mpq_t a, numflt_t b);
  /* numflt -> mpq */
static inline bool double_set_numflt(double* a, numflt_t b);
  /* numflt -> double */
static inline bool mpfr_set_numflt(mpfr_t a, numflt_t b);
  /* numflt -> mpfr */
static inline bool ap_scalar_set_numflt(ap_scalar_t* a, numflt_t b);
  /* numflt -> ap_scalar */

static inline bool mpz_fits_numflt(mpz_t a);
static inline bool mpq_fits_numflt(mpq_t a);
static inline bool double_fits_numflt(double a);
static inline bool mpfr_fits_numflt(mpfr_t a);
static inline bool numflt_fits_int(numflt_t a);
static inline bool numflt_fits_float(numflt_t a);
static inline bool numflt_fits_double(numflt_t a);
static inline bool numflt_fits_mpfr(numflt_t a);

/* Optimized versions */
/* mpfr should have exactly the precision NUMFLT_MANT_DIG */
static inline bool numflt_set_mpz_tmp(numflt_t a, mpz_t b, mpfr_t mpfr);
static inline bool numflt_set_mpq_tmp(numflt_t a, mpq_t b, mpfr_t mpfr);

/* ====================================================================== */
/* Only for floating point */
/* ====================================================================== */

static inline bool numflt_infty(numflt_t a);
static inline void numflt_set_infty(numflt_t a, int sgn);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numflt_serialize_id(void)
{ return 0x20 + sizeof(numflt_t)/4; }

static inline size_t numflt_serialize(void* dst, numflt_t src);
static inline size_t numflt_deserialize(numflt_t dst, const void* src);
static inline size_t numflt_serialized_size(numflt_t a);


/* */
static inline bool numflt_set_ap_scalar(numflt_t a, ap_scalar_t* b)
{
  assert (ap_scalar_infty(b)==0);
  switch (b->discr){
  case AP_SCALAR_MPQ:
    return numflt_set_mpq(a,b->val.mpq);
  case AP_SCALAR_DOUBLE:
    return numflt_set_double(a,b->val.dbl);
  case AP_SCALAR_MPFR:
    return numflt_set_mpfr(a,b->val.mpfr);
  default: abort();
  }
}
static inline bool ap_scalar_set_numflt(ap_scalar_t* a, numflt_t b);

#ifdef __cplusplus
}
#endif

#endif
