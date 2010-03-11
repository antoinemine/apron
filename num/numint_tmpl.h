/* ********************************************************************** */
/* numint.h */
/* ********************************************************************** */

#ifndef _NUMINT_H_
#define _NUMINT_H_

#include "numConfig.h"
#include "numint_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numint_set(numint_t a, numint_t b);
static inline void numint_set_array(numint_t* a, numint_t* b, size_t size);
static inline void numint_set_int(numint_t a, long int i);

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numint_init(numint_t a);
static inline void numint_init_array(numint_t* a, size_t size);
static inline void numint_init_set(numint_t a, numint_t b);
static inline void numint_init_set_int(numint_t a, long int i);

static inline void numint_clear(numint_t a);
static inline void numint_clear_array(numint_t* a, size_t size);

static inline void numint_swap(numint_t a, numint_t b);

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numint_neg(numint_t a, numint_t b);
static inline void numint_abs(numint_t a, numint_t b);
static inline void numint_add(numint_t a, numint_t b, numint_t c);
static inline void numint_add_uint(numint_t a, numint_t b, unsigned long int c);
static inline void numint_sub(numint_t a, numint_t b, numint_t c);
static inline void numint_sub_uint(numint_t a, numint_t b, unsigned long int c);
static inline void numint_mul(numint_t a, numint_t b, numint_t c);
static inline void numint_mul_2(numint_t a, numint_t b);
static inline void numint_fdiv_q(numint_t a, numint_t b, numint_t c);
static inline void numint_cdiv_q(numint_t q, numint_t a, numint_t b);
static inline void numint_cdiv_t(numint_t q, numint_t a, numint_t b);
static inline void numint_cdiv_qr(numint_t q, numint_t r, numint_t a, numint_t b);
static inline void numint_cdiv_2(numint_t a, numint_t b);
static inline void numint_cdiv_q_2exp(numint_t a, numint_t b, unsigned long int c);
static inline void numint_fdiv_q_2exp(numint_t a, numint_t b, unsigned long int c);
static inline void numint_min(numint_t a, numint_t b, numint_t c);
static inline void numint_max(numint_t a, numint_t b, numint_t c);
static inline void numint_floor(numint_t a, numint_t b);
static inline void numint_ceil(numint_t a, numint_t b);
static inline void numint_sqrt(numint_t up, numint_t down, numint_t b);
static inline void numint_mul_2exp(numint_t a, numint_t b, int c);

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numint_divexact(numint_t a, numint_t b, numint_t c);
static inline void numint_mod(numint_t a, numint_t b, numint_t c);
static inline void numint_gcd(numint_t a, numint_t b,  numint_t c);
static inline void numint_lcm(numint_t a, numint_t b,  numint_t c);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numint_sgn(numint_t a);
static inline int numint_cmp(numint_t a, numint_t b);
static inline int numint_cmp_int(numint_t a, long int b);
static inline bool numint_equal(numint_t a, numint_t b);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numint_print(numint_t a);
static inline void numint_fprint(FILE* stream, numint_t a);
static inline int numint_snprint(char* s, size_t size, numint_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_numint(long int a);
static inline bool llint_fits_numint(long long int a);
static inline bool mpz_fits_numint(mpz_t a);
static inline bool lfrac_fits_numint(long int i, long int j);
static inline bool llfrac_fits_numint(long long int i, long long int j);
static inline bool mpq_fits_numint(mpq_t a);
static inline bool double_fits_numint(double a);
static inline bool ldouble_fits_numint(long double a);
static inline bool mpfr_fits_numint(mpfr_t a, numinternal_t intern);

static inline bool numint_fits_lint(numint_t a);
static inline bool numint_fits_llint(numint_t a);
static inline bool numint_fits_lfrac(numint_t a);
static inline bool numint_fits_llfrac(numint_t a);
static inline bool numint_fits_double(numint_t a);
static inline bool numint_fits_ldouble(numint_t a);
static inline bool numint_fits_mpfr(numint_t a);

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

static inline bool numint_set_lint(numint_t a, long int b, numinternal_t intern);;
  /* lint -> numint */
static inline bool numint_set_llint(numint_t a, long long int b, numinternal_t intern);;
  /* llint -> numint */
static inline bool numint_set_mpz(numint_t a, mpz_t b, numinternal_t intern);;
  /* mpz -> numint */
static inline bool numint_set_lfrac(numint_t a, long int i, long int j, numinternal_t intern);;
  /* lfrac -> numint */
static inline bool numint_set_llfrac(numint_t a, long long int i, long long int j, numinternal_t intern);;
  /* llfrac -> numint */
static inline bool numint_set_mpq(numint_t a, mpq_t b, numinternal_t intern);
  /* mpq -> numint */
static inline bool numint_set_double(numint_t a, double b, numinternal_t intern);;
  /* double -> numint */
static inline bool numint_set_ldouble(numint_t a, long double b, numinternal_t intern);
  /* ldouble -> numint */
static inline bool numint_set_mpfr(numint_t a, mpfr_t b, numinternal_t intern);
  /* mpfr -> numint */

static inline bool lint_set_numint(long int* a, numint_t b, numinternal_t intern);;
  /* numint -> lint */
static inline bool llint_set_numint(long long int* a, numint_t b, numinternal_t intern);;
  /* numint -> llint */
static inline bool mpz_set_numint(mpz_t a, numint_t b, numinternal_t intern);;
  /* numint -> mpz */
static inline bool lfrac_set_numint(long int* i, long int* j, numint_t b, numinternal_t intern);;
  /* numint -> lfrac */
static inline bool llfrac_set_numint(long long int* i, long long int* j, numint_t b, numinternal_t intern);;
  /* numint -> llfrac */
static inline bool mpq_set_numint(mpq_t a, numint_t b, numinternal_t intern);;
  /* numint -> mpq */
static inline bool double_set_numint(double* a, numint_t b, numinternal_t intern);
  /* numint -> double */
static inline bool ldouble_set_numint(long double* a, numint_t b, numinternal_t intern);
  /* numint -> ldouble */
static inline bool mpfr_set_numint(mpfr_t a, numint_t b, numinternal_t intern);;
  /* numint -> mpfr */

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numint_serialize_id(void);
static inline size_t numint_serialize(void* dst, numint_t src);
static inline size_t numint_deserialize(numint_t dst, const void* src);
static inline size_t numint_serialized_size(numint_t a);

#ifdef __cplusplus
}
#endif

#endif
