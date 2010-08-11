/* ********************************************************************** */
/* numTypes.h */
/* ********************************************************************** */

#ifndef _NUMTYPES_H_
#define _NUMTYPES_H_

#include "numConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Integers */
/* ********************************************************************** */

typedef long int numIl_native;
typedef numIl_native* numIl_ptr;
typedef numIl_native numIl_t[1];
#define NUMIl_ZERO 0L
#define NUMIl_ONE 1L
#define NUMIl_MAX LONG_MAX
#define NUMIl_MIN LONG_MIN
#define NUMIl_EXACT 1
#define NUMIl_DIVEXACT 0
#define NUMIl_NATIVE 1

typedef long long int numIll_native;
typedef numIll_native* numIll_ptr;
typedef numIll_native numIll_t[1];
#define NUMIll_ZERO 0LL
#define NUMIll_ONE 1LL
#define NUMIll_MAX LLONG_MAX
#define NUMIll_MIN LLONG_MIN
#define NUMIll_EXACT 1
#define NUMIll_DIVEXACT 0
#define NUMIll_NATIVE 1

typedef mpz_ptr numMPZ_ptr;
typedef mpz_t numMPZ_t;
#undef NUMMPZ_ZERO
#undef NUMMPZ_ONE
#undef NUMMPZ_MAX
#undef NUMMPZ_MAX
#undef NUMMPZ_MIN
#define NUMMPZ_EXACT 1
#define NUMMPZ_DIVEXACT 1
#define NUMMPZ_NATIVE 0

/* ********************************************************************** */
/* Rationals */
/* ********************************************************************** */

typedef struct numRl_native {
  numIl_t n; /* numerator */
  numIl_t d; /* denominator, >=0 */
} numRl_native;
typedef numRl_native* numRl_ptr;
typedef numRl_native numRl_t[1];
#define numRl_numref(a) ((a)->n)
#define numRl_denref(a) ((a)->d)
typedef numIl_t numintRl_t;
#define NUMRl_EXACT 1
#define NUMRl_DIVEXACT 1
#define NUMRl_NATIVE 1

typedef struct numRll_native {
  numIll_t n; /* numerator */
  numIll_t d; /* denominator, >=0 */
} numRll_native;
typedef numRll_native* numRll_ptr;
typedef numRll_native numRll_t[1];
#define numRll_numref(a) ((a)->n)
#define numRll_denref(a) ((a)->d)
typedef numIll_t numintRll_t;
#define NUMRll_EXACT 1
#define NUMRll_DIVEXACT 1
#define NUMRll_NATIVE 1

typedef mpq_ptr numMPQ_ptr;
typedef mpq_t numMPQ_t;
#define numMPQ_numref(a) mpq_numref(a)
#define numMPQ_denref(a) mpq_denref(a)
typedef numMPZ_t numintMPQ_t;
#define NUMMPQ_EXACT 1
#define NUMMPQ_DIVEXACT 1
#define NUMMPQ_NATIVE 0

/* ********************************************************************** */
/* Floating-points */
/* ********************************************************************** */

#ifndef NUMFLT_PRINT_PREC
#define NUMFLT_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTD_PRINT_PREC to be any other
   expression (including variable and function call).
*/

typedef double numD_native;
typedef numD_native* numD_ptr;
typedef numD_native numD_t[1];
#define NUMD_ZERO 0.0
#define NUMD_ONE 1.0
#define NUMD_MANT_DIG DBL_MANT_DIG
#define NUMD_MAX NUMD_ONE/NUMD_ZERO
#define NUMD_EXACT 0
#define NUMD_DIVEXACT 0
#define NUMD_NATIVE 1

typedef long double numDl_native;
typedef numDl_native* numDl_ptr;
typedef numDl_native numDl_t[1];
#define NUMDl_ZERO 0.0L
#define NUMDl_ONE 1.0L
#define NUMDl_MANT_DIG LDBL_MANT_DIG
#define NUMDl_MAX NUMDl_ONE/NUMDl_ZERO
#define NUMDl_EXACT 0
#define NUMDl_DIVEXACT 0
#define NUMDl_NATIVE 1

typedef mpfr_ptr numMPFR_ptr;
typedef mpfr_t numMPFR_t;
#undef NUMMPFR_MAX
#undef NUMMPFR_MAX
#undef NUMMPFR_ZERO
#undef NUMMPFR_ONE
#undef NUMMPFR_MANT_DIG
#define NUMMPFR_EXACT 0
#define NUMMPFR_DIVEXACT 0
#define NUMMPFR_NATIVE 0

/*

**********************************************************************
Prototypes of common functions
**********************************************************************

======================================================================
Assignement
======================================================================
static inline void numXXX_set(numXXX_t a, numXXX_t b);
static inline void numXXX_set_array(numXXX_t* a, numXXX_t* b, size_t size);
static inline void numXXX_set_int(numXXX_t a, long int i);

======================================================================
Constructors and Destructors
======================================================================

static inline void numXXX_init(numXXX_t a);
static inline void numXXX_init_array(numXXX_t* a, size_t size);
static inline void numXXX_init_set(numXXX_t a, numXXX_t b);
static inline void numXXX_init_set_int(numXXX_t a, long int i);

static inline void numXXX_clear(numXXX_t a);
static inline void numXXX_clear_array(numXXX_t* a, size_t size);

static inline void numXXX_swap(numXXX_t a, numXXX_t b);

======================================================================
Arithmetic Operations
======================================================================

static inline void numXXX_neg(numXXX_t a, numXXX_t b);
static inline void numXXX_abs(numXXX_t a, numXXX_t b);
static inline void numXXX_add(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_add_uint(numXXX_t a, numXXX_t b, unsigned long int c);
static inline void numXXX_sub(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_sub_uint(numXXX_t a, numXXX_t b, unsigned long int c);
static inline void numXXX_mul(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_mul_2(numXXX_t a, numXXX_t b);
static inline void numXXX_div(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_div_2(numXXX_t a, numXXX_t b);
static inline void numXXX_min(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_max(numXXX_t a, numXXX_t b, numXXX_t c);

static inline void numXXX_mul_2exp(numXXX_t a, numXXX_t b, int c);
  multiplies b by 2^c (c can be positive or negative)

static inline void numXXX_floor(numXXX_t a, numXXX_t b);
static inline void numXXX_ceil(numXXX_t a, numXXX_t b);
static inline void numXXX_trunc(numXXX_t a, numXXX_t b);
  Approximate to the nearest integer toward resp. -infty, +infty

static inline void numXXX_sqrt(numXXX_t up, numXXX_t down, numXXX_t b);
  Compute both an upper bound and a lower bound

======================================================================
Arithmetic Tests
======================================================================

static inline int numXXX_sgn(numXXX_t a);
static inline int numXXX_cmp(numXXX_t a, numXXX_t b);
static inline int numXXX_cmp_int(numXXX_t a, long int b);
static inline bool numXXX_equal(numXXX_t a, numXXX_t b);
static inline bool numXXX_integer(numXXX_t a);
static inline int numXXX_hash(numXXX_t a);

======================================================================
Printing
======================================================================

static inline void numXXX_print(numXXX_t a);
static inline void numXXX_fprint(FILE* stream, numXXX_t a);
static inline int numXXX_snprint(char* s, size_t size, numXXX_t a);

======================================================================
Serialization
======================================================================

  Notes:
   - call _serialized_size to get the minimal size of the buffer to allocate
   - _serialize then returns the actual size of the serialized data (may be 
   less than _serialized_size)
   - deserialze assumes that the num destination has already been initialized
   - when deserializing an array, you must supply the number of elements in the
   array (it is not stored with the array by serialize)
   - serialization is not type-safe (it depends on the chosen numerical type
   and, for types using native integers, on whether the machine is 32 or 64 
   bits). You can only deserialize data if the numXXX_serialize_id function 
   returns the same value as when the data was serialized. (This id is not
   stored systematically with each serialized number to save state, so, the
   checking is up to you.)

static inline unsigned char numXXX_serialize_id(void);
static inline size_t numXXX_serialize(void* dst, numXXX_t src);
static inline size_t numXXX_deserialize(numXXX_t dst, const void* src);
static inline size_t numXXX_serialized_size(numXXX_t a);

static inline size_t numXXX_serialize_array(void* dst, numXXX_t* src, size_t size);
static inline size_t numXXX_deserialize_array(numXXX_t* dst, const void* src, size_t size);
static inline size_t numXXX_serialized_size_array(numXXX_t* src, size_t size);

======================================================================
Fits
======================================================================

static inline bool lint_fits_numXXX(long int a);
static inline bool llint_fits_numXXX(long long int a);
static inline bool mpz_fits_numXXX(mpz_t a);
static inline bool lfrac_fits_numXXX(long int i, long int j);
static inline bool llfrac_fits_numXXX(long long int i, long long int j);
static inline bool mpq_fits_numXXX(mpq_t a);
static inline bool double_fits_numXXX(double a);
static inline bool ldouble_fits_numXXX(long double a);
static inline bool mpfr_fits_numXXX(mpfr_t a, numinternal_t intern);

static inline bool numXXX_fits_lint(numXXX_t a);
static inline bool numXXX_fits_llint(numXXX_t a);
static inline bool numXXX_fits_mpz(numXXX_t a);
static inline bool numXXX_fits_lfrac(numXXX_t a);
static inline bool numXXX_fits_llfrac(numXXX_t a);
static inline bool numXXX_fits_mpq(numXXX_t a);
static inline bool numXXX_fits_float(numXXX_t a);
static inline bool numXXX_fits_double(numXXX_t a);
static inline bool numXXX_fits_ldouble(numXXX_t a);
static inline bool numXXX_fits_mpfr(numXXX_t a);

======================================================================
Conversions 
======================================================================

static inline bool numXXX_set_lint(numXXX_t a, long int b, numinternal_t intern);
static inline bool numXXX_set_llint(numXXX_t a, long long int b, numinternal_t intern);
static inline bool numXXX_set_mpz(numXXX_t a, mpz_t b, numinternal_t intern);
static inline bool numXXX_set_lfrac(numXXX_t a, long int i, long int j, numinternal_t intern);
static inline bool numXXX_set_llfrac(numXXX_t a, long long int i, long long int j, numinternal_t intern);
static inline bool numXXX_set_mpq(numXXX_t a, mpq_t b, numinternal_t intern);
static inline bool numXXX_set_double(numXXX_t a, double b, numinternal_t intern);
static inline bool numXXX_set_ldouble(numXXX_t a, long double b, numinternal_t intern);
static inline bool numXXX_set_mpfr(numXXX_t a, mpfr_t b, numinternal_t intern);

static inline bool lint_set_numXXX(long int* a, numXXX_t b, numinternal_t intern);
static inline bool llint_set_numXXX(long long int* a, numXXX_t b, numinternal_t intern);
static inline bool mpz_set_numXXX(mpz_t a, numXXX_t b, numinternal_t intern);
static inline bool lfrac_set_numXXX(long int* i, long int* j, numXXX_t b, numinternal_t intern);
static inline bool llfrac_set_numXXX(long long int* i, long long int* j, numXXX_t b, numinternal_t intern);
static inline bool mpq_set_numXXX(mpq_t a, numXXX_t b, numinternal_t intern);
static inline bool double_set_numXXX(double* a, numXXX_t b, numinternal_t intern);
static inline bool ldouble_set_numXXX(long double* a, numXXX_t b, numinternal_t intern);
static inline bool mpfr_set_numXXX(mpfr_t a, numXXX_t b, numinternal_t intern);

**********************************************************************
Integer functions
**********************************************************************
static inline void numXXX_gcd(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_lcm(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_fdiv_q(numXXX_t a, numXXX_t b, numXXX_t c);
static inline void numXXX_cdiv_q(numXXX_t q, numXXX_t a, numXXX_t b);
static inline void numXXX_tdiv_q(numXXX_t q, numXXX_t a, numXXX_t b);
static inline void numXXX_cdiv_qr(numXXX_t q, numXXX_t r, numXXX_t a, numXXX_t b);
static inline void numXXX_cdiv_2(numXXX_t a, numXXX_t b);
static inline void numXXX_cdiv_q_2exp(numXXX_t a, numXXX_t b, unsigned long int c);
static inline void numXXX_fdiv_q_2exp(numXXX_t a, numXXX_t b, unsigned long int c);

**********************************************************************
Rational functions
**********************************************************************

static inline void numXXX_canonicalize(numXXX_t a);
static inline bool numRl_set_numIl2(numRl_t a, numIl_t b, numIl_t c);
static inline bool numRll_set_numIll2(numRll_t a, numIll_t b, numIll_t c);
static inline bool numMPQ_set_numMPZ2(numMPQ_t a, numMPZ_t b, numMPZ_t c);

**********************************************************************
Floating-point functions
**********************************************************************
static inline bool numXXX_infty(numXXX_t a);
static inline void numXXX_set_infty(numXXX_t a, int sgn);


*/

#ifdef __cplusplus
}
#endif

#endif
