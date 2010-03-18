/* ********************************************************************** */
/* numint.h */
/* ********************************************************************** */

#ifndef _NUMINT_H_
#define _NUMINT_H_

#include "numConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NUM_NUMIL
typedef long int numint_native;
typedef numint_native* numint_ptr;
typedef numint_native numint_t[1];
#define NUMINTIL_ZERO 0L
#define NUMINTIL_ONE 1L
#define NUMINTIL_MAX LONG_MAX
#define NUMIL_MAX LONG_MAX
#define NUMINTIL_MIN LONG_MIN

#elif NUM_NUMILL
typedef long long int numint_native;
typedef numint_native* numint_ptr;
typedef numint_native numint_t[1];
#define NUMINTILL_ZERO 0LL
#define NUMINTILL_ONE 1LL
#define NUMINTILL_MAX LLONG_MAX
#define NUMILL_MAX LLONG_MAX
#define NUMINTILL_MIN LLONG_MIN

#elif NUM_NUMMPZ
typedef mpz_ptr numint_ptr;
typedef mpz_t numint_t;
#undef NUMINTMPZ_ZERO
#undef NUMINTMPZ_ONE
#undef NUMINTMPZ_MAX
#undef NUMMPZ_MAX
#undef NUMINTMPZ_MIN

#else
#error "HERE"
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
