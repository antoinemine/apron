/* ********************************************************************** */
/* numrat.h */
/* ********************************************************************** */

#ifndef _NUMRAT_H_
#define _NUMRAT_H_

#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"

#include "numConfig.h"

#include "numint.h"
#include "numrat_def.h"

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

static inline void numrat_swap(numrat_t a, numrat_t b);

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
/* Rational operations */
/* ====================================================================== */

static inline void numrat_canonicalize(numrat_t a);
static inline bool numrat_set_numfrac(numrat_t a, numint_t b, numint_t c);

/*
static inline numint_t numrat_numref(numrat_t a);
static inline numint_t numrat_denref(numrat_t a);
*/

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numrat(long int a);
static inline bool llint_fits_numrat(long long int a);
static inline bool mpz_fits_numrat(mpz_t a);
static inline bool lfrac_fits_numrat(long int i, long int j);
static inline bool llfrac_fits_numrat(long long int i, long long int j);
static inline bool mpq_fits_numrat(mpq_t a);
static inline bool double_fits_numrat(double a);
static inline bool ldouble_fits_numrat(long double a);
static inline bool mpfr_fits_numrat(mpfr_t a, numinternal_t intern);

static inline bool numrat_fits_lint(numrat_t a);
static inline bool numrat_fits_llint(numrat_t a);
static inline bool numrat_fits_lfrac(numrat_t a);
static inline bool numrat_fits_llfrac(numrat_t a);
static inline bool numrat_fits_double(numrat_t a);
static inline bool numrat_fits_ldouble(numrat_t a);
static inline bool numrat_fits_mpfr(numrat_t a);

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

static inline bool numrat_set_lint(numrat_t a, long int b, numinternal_t intern);;
  /* lint -> numrat */
static inline bool numrat_set_llint(numrat_t a, long long int b, numinternal_t intern);;
  /* llint -> numrat */
static inline bool numrat_set_mpz(numrat_t a, mpz_t b, numinternal_t intern);;
  /* mpz -> numrat */
static inline bool numrat_set_lfrac(numrat_t a, long int i, long int j, numinternal_t intern);;
  /* lfrac -> numrat */
static inline bool numrat_set_llfrac(numrat_t a, long long int i, long long int j, numinternal_t intern);;
  /* llfrac -> numrat */
static inline bool numrat_set_mpq(numrat_t a, mpq_t b, numinternal_t intern);
  /* mpq -> numrat */
static inline bool numrat_set_double(numrat_t a, double b, numinternal_t intern);;
  /* double -> numrat */
static inline bool numrat_set_ldouble(numrat_t a, long double b, numinternal_t intern);
  /* ldouble -> numrat */
static inline bool numrat_set_mpfr(numrat_t a, mpfr_t b, numinternal_t intern);
  /* mpfr -> numrat */

static inline bool lint_set_numrat(long int* a, numrat_t b, numinternal_t intern);;
  /* numrat -> lint */
static inline bool llint_set_numrat(long long int* a, numrat_t b, numinternal_t intern);;
  /* numrat -> llint */
static inline bool mpz_set_numrat(mpz_t a, numrat_t b, numinternal_t intern);;
  /* numrat -> mpz */
static inline bool lfrac_set_numrat(long int* i, long int* j, numrat_t b, numinternal_t intern);;
  /* numrat -> lfrac */
static inline bool llfrac_set_numrat(long long int* i, long long int* j, numrat_t b, numinternal_t intern);;
  /* numrat -> llfrac */
static inline bool mpq_set_numrat(mpq_t a, numrat_t b, numinternal_t intern);;
  /* numrat -> mpq */
static inline bool double_set_numrat(double* a, numrat_t b, numinternal_t intern);
  /* numrat -> double */
static inline bool ldouble_set_numrat(long double* a, numrat_t b, numinternal_t intern);
  /* numrat -> ldouble */
static inline bool mpfr_set_numrat(mpfr_t a, numrat_t b, numinternal_t intern);;
  /* numrat -> mpfr */

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numrat_serialize_id(void);
static inline size_t numrat_serialize(void* dst, numrat_t src);
static inline size_t numrat_deserialize(numrat_t dst, const void* src);
static inline size_t numrat_serialized_size(numrat_t a);

#ifdef __cplusplus
}
#endif

#endif
