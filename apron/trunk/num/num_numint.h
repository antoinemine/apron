/* ********************************************************************** */
/* num_numint.h */
/* ********************************************************************** */

#ifndef _NUM_NUMINT_H_
#define _NUM_NUMINT_H_

#ifndef _NUM_H_
#error "File num_numint.h should not be included directly, only via num.h"
#endif

#if defined(NUM_LONGINT)
#define NUMINT_LONGINT

#elif defined(NUM_LONGLONGINT)
#define NUMINT_LONGLONGINT

#elif defined(NUM_MPZ)
#define NUMINT_MPZ

#else
#error "HERE"
#endif

#define NUM_AP_SCALAR AP_SCALAR_MPQ

#include "numint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef numint_t num_t;

#define NUM_NUMINT

#ifdef NUMINT_MAX
#define NUM_MAX NUMINT_MAX
#endif

#ifdef NUMINT_NATIVE
#define NUM_NATIVE
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
	      { numint_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
	      { numint_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
	      { numint_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
	      { numint_init(a); }
static inline void num_init_array(num_t* a, size_t size)
	      { numint_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
	      { numint_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
	      { numint_init_set_int(a,i); }

static inline void num_clear(num_t a)
	      { numint_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
	      { numint_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
	      { numint_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
	      { numint_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
	      { numint_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
	      { numint_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
	      { numint_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
	      { numint_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
	      { numint_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
	      { numint_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
	      { numint_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
	      { numint_cdiv_q(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
	      { numint_cdiv_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
	      { numint_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
	      { numint_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
	      { numint_set(a,b); }
static inline void num_ceil(num_t a, num_t b)
	      { numint_set(a,b); }
static inline void num_trunc(num_t a, num_t b)
	      { numint_set(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
	      { numint_sqrt(up,down,b); }
static inline int  num_pow(num_t up, num_t down, num_t b, unsigned long int n)
              { return numint_pow(up,down,b,n); }
static inline void num_root(num_t up, num_t down, num_t b, unsigned long int n)
              { numint_root(up,down,b,n); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
	      { numint_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
      { return numint_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
      { return numint_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
      { return numint_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
      { return numint_equal(a,b); }
static inline bool num_integer(num_t a)
      { (void)a; return true; }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
	      { numint_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
	      { numint_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
      { return numint_snprint(s,size,a); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool num_set_int2(num_t a, long int i, long int j)
	      { return numint_set_int2(a,i,j); }
  /* int2 -> num */

static inline bool num_set_mpz(num_t a, mpz_t b)
	  { return numint_set_mpz(a,b); }
  /* mpz -> num */

static inline bool num_set_mpq(num_t a, mpq_t b)
	  { return numint_set_mpq(a,b); }
  /* mpq -> num */

static inline bool num_set_double(num_t a, double k)
	  { return numint_set_double(a,k); }
  /* double -> num */
static inline bool num_set_mpfr(num_t a, mpfr_t k)
	  { return numint_set_mpfr(a,k); }
  /* mpfr -> num */
static inline bool num_set_ap_scalar(num_t a, ap_scalar_t* b)
	  { return numint_set_ap_scalar(a,b); }
  /* ap_scalar -> num */

static inline bool int_set_num(long int*a, num_t b)
	  { return int_set_numint(a,b); }
  /* num -> int */
static inline bool mpz_set_num(mpz_t a, num_t b)
	  { return mpz_set_numint(a,b); }
  /* num -> mpz */
static inline bool mpq_set_num(mpq_t a, num_t b)
	  { return mpq_set_numint(a,b); }
  /* num -> mpq */
static inline bool double_set_num(double* a, num_t b)
	  { return double_set_numint(a,b); }
  /* num -> double */
static inline bool mpfr_set_num(mpfr_t a, num_t b)
	  { return mpfr_set_numint(a,b); }
  /* num -> mpfr */
static inline bool ap_scalar_set_num(ap_scalar_t* a, num_t b)
	  { return ap_scalar_set_numint(a,b); }
  /* num -> ap_scalar */

static inline bool mpz_fits_num(mpz_t a)
	  { return mpz_fits_numint(a); }
static inline bool mpq_fits_num(mpq_t a)
	  { return mpq_fits_numint(a); }
static inline bool double_fits_num(double a)
	  { return double_fits_numint(a); }
static inline bool mpfr_fits_num(mpfr_t a)
	  { return mpfr_fits_numint(a); }
static inline bool num_fits_int(num_t a)
	  { return numint_fits_int(a); }
static inline bool num_fits_float(num_t a)
	  { return numint_fits_float(a); }
static inline bool num_fits_double(num_t a)
	  { return numint_fits_double(a); }
static inline bool num_fits_mpfr(num_t a)
	  { return numint_fits_mpfr(a); }

/* Optimized versions */
static inline bool num_set_mpq_tmp(numint_t a, mpq_t b,
				   mpz_t q, mpz_t r, mpfr_t mpfr)
{ (void)mpfr; return numint_set_mpq_tmp(a,b,q,r); }
static inline bool mpq_fits_num_tmp(mpq_t a, mpz_t mpz)
{ return mpq_fits_numint_tmp(a,mpz); }
  
static inline bool num_set_double_tmp(num_t a, double k, mpq_t mpq)
{ (void)mpq; return numint_set_double(a,k); }
static inline bool num_set_mpz_tmp(num_t a, mpz_t b, mpfr_t mpfr)
{ (void)mpfr; return numint_set_mpz(a,b); }
static inline bool int_set_num_tmp(long int* a, num_t b,
				   mpz_t q, mpz_t r)
{ (void)q; (void)r; return int_set_numint(a,b); }
static inline bool mpz_set_num_tmp(mpz_t a, num_t b, mpz_t mpz)
{ (void)mpz; return mpz_set_numint(a,b); }
static inline bool double_set_num_tmp(double* a, num_t b,
				      mpq_t mpq, mpfr_t mpfr)
{ (void)mpq; (void)mpfr; return double_set_numint(a,b); }
static inline bool double_fits_num_tmp(double k, mpq_t mpq)
{ (void)mpq; return double_fits_numint(k); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numint_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numint_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numint_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numint_serialized_size(a); }

#ifdef __cplusplus
}
#endif

#endif
