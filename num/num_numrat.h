/* ********************************************************************** */
/* num_numrat.h */
/* ********************************************************************** */

#ifndef _NUM_NUMRAT_H_
#define _NUM_NUMRAT_H_

#ifndef _NUM_H_
#error "File num_numrat.h should not be included directly, only via num.h"
#endif

#if defined(NUM_LONGRAT)
#define NUMINT_LONGINT

#elif defined(NUM_LONGLONGRAT)
#define NUMINT_LONGLONGINT

#elif defined(NUM_MPQ)
#define NUMINT_MPZ

#else
#error "HERE"
#endif

#define NUM_AP_SCALAR AP_SCALAR_MPQ

#include "numrat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef numrat_t num_t;

#define NUM_NUMRAT

#ifdef NUMINT_NATIVE
#define NUM_NATIVE
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
              { numrat_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
              { numrat_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
              { numrat_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
              { numrat_init(a); }
static inline void num_init_array(num_t* a, size_t size)
              { numrat_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
              { numrat_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
              { numrat_init_set_int(a,i); }

static inline void num_clear(num_t a)
              { numrat_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
              { numrat_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
              { numrat_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
              { numrat_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
              { numrat_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
              { numrat_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
              { numrat_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
              { numrat_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
              { numrat_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
              { numrat_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
              { numrat_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
              { numrat_div(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
              { numrat_div_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
              { numrat_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
              { numrat_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
              { numrat_floor(a,b); }
static inline void num_ceil(num_t a, num_t b)
              { numrat_ceil(a,b); }
static inline void num_trunc(num_t a, num_t b)
              { numrat_trunc(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
              { numrat_sqrt(up,down,b); }
static inline int  num_pow(num_t up, num_t down, num_t b, unsigned long int n)
              { return numrat_pow(up,down,b,n); }
static inline void num_root(num_t up, num_t down, num_t b, unsigned long int n)
              { numrat_root(up,down,b,n); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
              { numrat_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
         { return numrat_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
      { return numrat_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
      { return numrat_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
      { return numrat_equal(a,b); }
static inline bool num_integer(num_t a)
      { return numrat_integer(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
              { numrat_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
              { numrat_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
      { return numrat_snprint(s,size,a); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_int2(num_t a, long int i, long int j)
              { return numrat_set_int2(a,i,j); }
  /* int2 -> num */

static inline bool num_set_mpz(num_t a, mpz_t b)
          { return numrat_set_mpz(a,b); }
  /* mpz -> num */

static inline bool num_set_mpq(num_t a, mpq_t b)
          { return numrat_set_mpq(a,b); }
  /* mpq -> num */

static inline bool num_set_double(num_t a, double k)
          { return numrat_set_double(a,k); }
  /* double -> num */
static inline bool num_set_mpfr(num_t a, mpfr_t k)
          { return numrat_set_mpfr(a,k); }
  /* mpfr -> num */
static inline bool num_set_ap_scalar(num_t a, ap_scalar_t* b)
          { return numrat_set_ap_scalar(a,b); }
  /* ap_scalar -> num */

static inline bool int_set_num(long int*a, num_t b)
          { return int_set_numrat(a,b); }
  /* num -> int */
static inline bool mpz_set_num(mpz_t a, num_t b)
          { return mpz_set_numrat(a,b); }
  /* num -> mpz */
static inline bool mpq_set_num(mpq_t a, num_t b)
          { return mpq_set_numrat(a,b); }
  /* num -> mpq */
static inline bool double_set_num(double* a, num_t b)
          { return double_set_numrat(a,b); }
  /* num -> double */
static inline bool mpfr_set_num(mpfr_t a, num_t b)
          { return mpfr_set_numrat(a,b); }
  /* num -> mpfr */
static inline bool ap_scalar_set_num(ap_scalar_t* a, num_t b)
          { return ap_scalar_set_numrat(a,b); }
  /* num -> ap_scalar */

static inline bool mpz_fits_num(mpz_t a)
          { return mpz_fits_numrat(a); }
static inline bool mpq_fits_num(mpq_t a)
          { return mpq_fits_numrat(a); }
static inline bool double_fits_num(double a)
          { return double_fits_numrat(a); }
static inline bool mpfr_fits_num(mpfr_t a)
          { return mpfr_fits_numrat(a); }
static inline bool num_fits_int(num_t a)
          { return numrat_fits_int(a); }
static inline bool num_fits_float(num_t a)
          { return numrat_fits_float(a); }
static inline bool num_fits_double(num_t a)
          { return numrat_fits_double(a); }
static inline bool num_fits_mpfr(num_t a)
          { return numrat_fits_mpfr(a); }

/* Optimized versions */
static inline bool num_set_double_tmp(num_t a, double k, mpq_t mpq)
        { return numrat_set_double_tmp(a,k,mpq); }
static inline bool int_set_num_tmp(long int* a, num_t b, 
				   mpz_t q, mpz_t r)
        { return int_set_numrat_tmp(a,b,q,r); }
static inline bool mpz_set_num_tmp(mpz_t a, num_t b, mpz_t mpz)
        { return mpz_set_numrat_tmp(a,b,mpz); }
static inline bool double_set_num_tmp(double* a, num_t b, 
				      mpq_t mpq, mpfr_t mpfr)
        { return double_set_numrat_tmp(a,b,mpq,mpfr); }
static inline bool double_fits_num_tmp(double k, mpq_t mpq)
        { return double_fits_numrat_tmp(k,mpq); }

static inline bool num_set_mpq_tmp(num_t a, mpq_t b, 
				   mpz_t q, mpz_t r, mpfr_t mpfr)
{ (void)q; (void)r; (void)mpfr; return numrat_set_mpq(a,b); }
static inline bool mpq_fits_num_tmp(mpq_t a, mpz_t mpz)
{ (void)mpz; return mpq_fits_numrat(a); }
static inline bool num_set_mpz_tmp(num_t a, mpz_t b, mpfr_t mpfr)
{ (void)mpfr; return numrat_set_mpz(a,b); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numrat_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numrat_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numrat_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numrat_serialized_size(a); }

#ifdef __cplusplus
}
#endif

#endif
