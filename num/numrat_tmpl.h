/* ********************************************************************** */
/* numrat.h */
/* ********************************************************************** */

#ifndef _NUMRAT_H_
#define _NUMRAT_H_

#include "numConfig.h"

#include "numint.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NUM_NUMRL
typedef struct numrat_native {
  numintIl_t n; /* numerator */
  numintIl_t d; /* denominator, >=0 */
} numrat_native;
typedef numrat_native* numrat_ptr;
typedef numrat_native numrat_t[1];
#define numrat_numref(a) (a)->n
#define numrat_denref(a) (a)->d
#elif NUM_NUMRLL
typedef struct numrat_native {
  numintIll_t n; /* numerator */
  numintIll_t d; /* denominator, >=0 */
} numrat_native;
typedef numrat_native* numrat_ptr;
typedef numrat_native numrat_t[1];
#define numrat_numref(a) (a)->n
#define numrat_denref(a) (a)->d
#elif NUM_NUMMPQ
typedef mpq_ptr numrat_ptr;
typedef mpq_t numrat_t;
#define numrat_numref(a) mpq_numref(a)
#define numrat_denref(a) mpq_denref(a)
#else
#error "HERE"
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
