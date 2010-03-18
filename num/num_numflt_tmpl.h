/* ********************************************************************** */
/* num_numflt.h */
/* ********************************************************************** */

#ifndef _NUM__NUMFLT_H_
#define _NUM__NUMFLT_H_

#include "numConfig.h"
#include "numflt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef numflt_ptr num_ptr;
typedef numflt_t num_t;

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
{ numflt_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
{ numflt_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
{ numflt_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
{ numflt_init(a); }
static inline void num_init_array(num_t* a, size_t size)
{ numflt_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
{ numflt_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
{ numflt_init_set_int(a,i); }

static inline void num_clear(num_t a)
{ numflt_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
{ numflt_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
{ numflt_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
{ numflt_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
{ numflt_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
{ numflt_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
{ numflt_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
{ numflt_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
{ numflt_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
{ numflt_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
{ numflt_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
{ numflt_div(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
{ numflt_div_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
{ numflt_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
{ numflt_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
{ numflt_floor(a,b); }
static inline void num_ceil(num_t a, num_t b)
{ numflt_ceil(a,b); }
static inline void num_trunc(num_t a, num_t b)
{ numflt_trunc(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
{ numflt_sqrt(up,down,b); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
{ numflt_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
{ return numflt_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
{ return numflt_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
{ return numflt_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
{ return numflt_equal(a,b); }
static inline bool num_integer(num_t a)
{ return numflt_integer(a); }
static inline int num_hash(num_t a)
{ return numflt_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
{ numflt_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
{ numflt_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
{ return numflt_snprint(s,size,a); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Fits */
/* ---------------------------------------------------------------------- */

static inline bool lint_fits_num(long int a)
{ return lint_fits_numflt(a); }
static inline bool llint_fits_num(long long int a)
{ return llint_fits_numflt(a); }
static inline bool mpz_fits_num(mpz_t a)
{ return mpz_fits_numflt(a); }
static inline bool lfrac_fits_num(long int i, long int j)
{ return lfrac_fits_numflt(i,j); }
static inline bool llfrac_fits_num(long long int i, long long int j)
{ return llfrac_fits_numflt(i,j); }
static inline bool mpq_fits_num(mpq_t a)
{ return mpq_fits_numflt(a); }
static inline bool double_fits_num(double a)
{ return double_fits_numflt(a); }
static inline bool ldouble_fits_num(long double a)
{ return ldouble_fits_numflt(a); }
static inline bool mpfr_fits_num(mpfr_t a, numinternal_t intern)
{ return mpfr_fits_numflt(a,intern); }

static inline bool num_fits_lint(num_t a)
{ return numflt_fits_lint(a); }
static inline bool num_fits_llint(num_t a)
{ return numflt_fits_llint(a); }
static inline bool num_fits_lfrac(num_t a)
{ return numflt_fits_lfrac(a); }
static inline bool num_fits_llfrac(num_t a)
{ return numflt_fits_llfrac(a); }
static inline bool num_fits_float(num_t a)
{ return numflt_fits_float(a); }
static inline bool num_fits_double(num_t a)
{ return numflt_fits_double(a); }
static inline bool num_fits_ldouble(num_t a)
{ return numflt_fits_ldouble(a); }
static inline bool num_fits_mpfr(num_t a)
{ return numflt_fits_mpfr(a); }

/* ---------------------------------------------------------------------- */
/* Conversions */
/* ---------------------------------------------------------------------- */

static inline bool num_set_lint(num_t a, long int b, numinternal_t intern)
{ return numflt_set_lint(a,b,intern); }
static inline bool num_set_llint(num_t a, long long int b, numinternal_t intern)
{ return numflt_set_llint(a,b,intern); }
static inline bool num_set_mpz(num_t a, mpz_t b, numinternal_t intern)
{ return numflt_set_mpz(a,b,intern); }

static inline bool num_set_lfrac(num_t a, long int i, long int j, numinternal_t intern)
{ return numflt_set_lfrac(a,i,j,intern); }
static inline bool num_set_llfrac(num_t a, long long int i, long long int j, numinternal_t intern)
{ return numflt_set_llfrac(a,i,j,intern); }
static inline bool num_set_mpq(num_t a, mpq_t b, numinternal_t intern)
{ return numflt_set_mpq(a,b,intern); }

static inline bool num_set_double(num_t a, double k, numinternal_t intern)
{ return numflt_set_double(a,k,intern); }
static inline bool num_set_ldouble(num_t a, long double k, numinternal_t intern)
{ return numflt_set_ldouble(a,k,intern); }
static inline bool num_set_mpfr(num_t a, mpfr_t k, numinternal_t intern)
{ return numflt_set_mpfr(a,k,intern); }

static inline bool lint_set_num(long int* a, num_t b, numinternal_t intern)
{ return lint_set_numflt(a,b,intern); }
static inline bool llint_set_num(long long int* a, num_t b, numinternal_t intern)
{ return llint_set_numflt(a,b,intern); }
static inline bool mpz_set_num(mpz_t a, num_t b, numinternal_t intern)
{ return mpz_set_numflt(a,b,intern); }
static inline bool lfrac_set_num(long int* i, long int* j, num_t b, numinternal_t intern)
{ return lfrac_set_numflt(i,j,b,intern); }
static inline bool llfrac_set_num(long long int* i, long long int* j, num_t b, numinternal_t intern)
{ return llfrac_set_numflt(i,j,b,intern); }
static inline bool mpq_set_num(mpq_t a, num_t b, numinternal_t intern)
{ return mpq_set_numflt(a,b,intern); }
static inline bool double_set_num(double* a, num_t b, numinternal_t intern)
{ return double_set_numflt(a,b,intern); }
static inline bool ldouble_set_num(long double* a, num_t b, numinternal_t intern)
{ return ldouble_set_numflt(a,b,intern); }
static inline bool mpfr_set_num(mpfr_t a, num_t b, numinternal_t intern)
{ return mpfr_set_numflt(a,b,intern); }


/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numflt_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numflt_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numflt_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numflt_serialized_size(a); }

#ifdef __cplusplus
}
#endif

#endif
