/* ********************************************************************** */
/* numflt_tmpl.h */
/* ********************************************************************** */

#ifndef _NUMFLT_H_
#define _NUMFLT_H_

#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"

#include "numConfig.h"
#include "numflt_def.h"

#ifdef __cplusplus
extern "C" {
#endif

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
/* Only for floating point */
/* ====================================================================== */

static inline bool numflt_infty(numflt_t a);
static inline void numflt_set_infty(numflt_t a, int sgn);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numflt(long int a);
static inline bool llint_fits_numflt(long long int a);
static inline bool mpz_fits_numflt(mpz_t a);
static inline bool lfrac_fits_numflt(long int i, long int j);
static inline bool llfrac_fits_numflt(long long int i, long long int j);
static inline bool mpq_fits_numflt(mpq_t a);
static inline bool double_fits_numflt(double a);
static inline bool ldouble_fits_numflt(long double a);
static inline bool mpfr_fits_numflt(mpfr_t a, numinternal_t intern);

static inline bool numflt_fits_lint(numflt_t a);
static inline bool numflt_fits_llint(numflt_t a);
static inline bool numflt_fits_lfrac(numflt_t a);
static inline bool numflt_fits_llfrac(numflt_t a);
static inline bool numflt_fits_double(numflt_t a);
static inline bool numflt_fits_ldouble(numflt_t a);
static inline bool numflt_fits_mpfr(numflt_t a);

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

static inline bool numflt_set_lint(numflt_t a, long int b, numinternal_t intern);;
  /* lint -> numflt */
static inline bool numflt_set_llint(numflt_t a, long long int b, numinternal_t intern);;
  /* llint -> numflt */
static inline bool numflt_set_mpz(numflt_t a, mpz_t b, numinternal_t intern);;
  /* mpz -> numflt */
static inline bool numflt_set_lfrac(numflt_t a, long int i, long int j, numinternal_t intern);;
  /* lfrac -> numflt */
static inline bool numflt_set_llfrac(numflt_t a, long long int i, long long int j, numinternal_t intern);;
  /* llfrac -> numflt */
static inline bool numflt_set_mpq(numflt_t a, mpq_t b, numinternal_t intern);
  /* mpq -> numflt */
static inline bool numflt_set_double(numflt_t a, double b, numinternal_t intern);;
  /* double -> numflt */
static inline bool numflt_set_ldouble(numflt_t a, long double b, numinternal_t intern);
  /* ldouble -> numflt */
static inline bool numflt_set_mpfr(numflt_t a, mpfr_t b, numinternal_t intern);
  /* mpfr -> numflt */

static inline bool lint_set_numflt(long int* a, numflt_t b, numinternal_t intern);;
  /* numflt -> lint */
static inline bool llint_set_numflt(long long int* a, numflt_t b, numinternal_t intern);;
  /* numflt -> llint */
static inline bool mpz_set_numflt(mpz_t a, numflt_t b, numinternal_t intern);;
  /* numflt -> mpz */
static inline bool lfrac_set_numflt(long int* i, long int* j, numflt_t b, numinternal_t intern);;
  /* numflt -> lfrac */
static inline bool llfrac_set_numflt(long long int* i, long long int* j, numflt_t b, numinternal_t intern);;
  /* numflt -> llfrac */
static inline bool mpq_set_numflt(mpq_t a, numflt_t b, numinternal_t intern);;
  /* numflt -> mpq */
static inline bool double_set_numflt(double* a, numflt_t b, numinternal_t intern);
  /* numflt -> double */
static inline bool ldouble_set_numflt(long double* a, numflt_t b, numinternal_t intern);
  /* numflt -> ldouble */
static inline bool mpfr_set_numflt(mpfr_t a, numflt_t b, numinternal_t intern);;
  /* numflt -> mpfr */

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numflt_serialize_id(void)
{ return 0x20 + sizeof(numflt_t)/4; }

static inline size_t numflt_serialize(void* dst, numflt_t src);
static inline size_t numflt_deserialize(numflt_t dst, const void* src);
static inline size_t numflt_serialized_size(numflt_t a);

#ifdef __cplusplus
}
#endif

#endif
