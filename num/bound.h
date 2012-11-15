/* ********************************************************************** */
/* bound.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUND_H_
#define _BOUND_H_

#ifndef _NUM_H_
#error "File bound.h requires num.h to be first included"
#endif

#include <stdio.h>
#include "ap_scalar.h"
#include "bound_def.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool bound_infty(bound_t a);
/*
Macro:
static inline num_t bound_numref(bound_t a); 
*/

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void bound_set(bound_t a, bound_t b);
static inline void bound_set_array(bound_t* a, bound_t* b, size_t size);
static inline void bound_set_int(bound_t a, long int i);
static inline void bound_set_num(bound_t a, num_t b);
static inline void bound_set_infty(bound_t a, int sgn);
static inline void bound_swap(bound_t a, bound_t b);

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void bound_init(bound_t a);
static inline void bound_init_array(bound_t* a, size_t size);

static inline void bound_init_set(bound_t a, bound_t b);
static inline void bound_init_set_int(bound_t a, long int i);
static inline void bound_init_set_infty(bound_t a, int sgn);

static inline void bound_clear(bound_t a);
static inline void bound_clear_array(bound_t* a, size_t size);

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

/* +oo + -oo  \ 
   -oo + +oo  | undefined
   +oo - +oo  |
   -oo - -oo  /

   +oo + x = +oo - x = x - -oo = +oo
   -oo + x = -oo - x = x - +oo = -oo

   0 * +oo = +oo * 0 = 0 * -oo = -oo * 0 = 0
   x * +oo = +oo * x =  sign(x) * oo  if x!=0
   x * -oo = -oo * x = -sign(x) * oo  if x!=0

   0 / x = x / +oo = x / -oo = 0
   x / 0 = sign(x) * oo     if x!=0
   +oo / x =  sign(x) * oo  if x!=0,+oo,-oo
   -oo / x = -sign(x) * oo  if x!=0,+oo,-oo
   
*/

static inline void bound_neg(bound_t a, bound_t b);
static inline void bound_abs(bound_t a, bound_t b);
static inline void bound_add(bound_t a, bound_t b, bound_t c);
static inline void bound_add_uint(bound_t a, bound_t b, unsigned long int c);
static inline void bound_add_num(bound_t a, bound_t b, num_t c);
static inline void bound_sub(bound_t a, bound_t b, bound_t c);
static inline void bound_sub_uint(bound_t a, bound_t b, unsigned long int c);
static inline void bound_sub_num(bound_t a, bound_t b, num_t c);
static inline void bound_mul(bound_t a, bound_t b, bound_t c);
static inline void bound_mul_num(bound_t a, bound_t b, num_t c);
static inline void bound_mul_2(bound_t a, bound_t b);
static inline void bound_div(bound_t a, bound_t b, bound_t c);
static inline void bound_div_num(bound_t a, bound_t b, num_t c);
static inline void bound_div_2(bound_t a, bound_t b);
static inline void bound_min(bound_t a, bound_t b, bound_t c);
static inline void bound_max(bound_t a, bound_t b, bound_t c);
static inline void bound_mul_2exp(bound_t a, bound_t b, int c);
static inline void bound_floor(bound_t a, bound_t b);
static inline void bound_ceil(bound_t a, bound_t b);
static inline void bound_trunc(bound_t a, bound_t b);
static inline void bound_sqrt(bound_t up, bound_t down, bound_t b);
static inline void bound_pow(bound_t up, bound_t down, bound_t b, unsigned long n);
static inline void bound_root(bound_t up, bound_t down, bound_t b, unsigned long n);


/* ====================================================================== */
/* Floating-point casts */
/* ====================================================================== */

/* always rounds toward +oo */
static inline void bound_to_float(bound_t a, bound_t b);
static inline void bound_to_double(bound_t a, bound_t b);

  /* next float / double towards +oo, rounded towards +oo */
static inline void bound_next_float(bound_t a, bound_t b);
static inline void bound_next_double(bound_t a, bound_t b);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int bound_sgn(bound_t a);
static inline int bound_cmp(bound_t a, bound_t b);
static inline int bound_cmp_int(bound_t a, long int b);
static inline int bound_cmp_num(bound_t a, num_t b);
static inline bool bound_equal(bound_t a, bound_t b);
static inline int bound_hash(bound_t a);
static inline bool bound_integer(bound_t a);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void bound_print(bound_t a);
static inline void bound_fprint(FILE* stream, bound_t a);
static inline int bound_snprint(char* s, size_t size, bound_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool bound_set_ap_scalar(bound_t a, ap_scalar_t* b);
  /* Convert a ap_scalar_t into a bound_t. */
  /* Return true iff the conversion is exact */

static inline bool ap_scalar_set_bound(ap_scalar_t* a, bound_t b);
  /* Convert a bound_t into a ap_scalar_t */
  /* Reinitialize a with the best type, depending on bound_t and num_t */
  /* Return true iff the conversion is exact.

     Normally always return true, with the exception of long double type for
     num. */

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

/* Note: call _init before _deserialize */

static inline size_t bound_serialize(void* dst, bound_t src);
static inline size_t bound_deserialize(bound_t dst, const void* src);
static inline size_t bound_serialized_size(bound_t a);

static inline size_t bound_serialize_array(void* dst, bound_t* src, size_t size);
static inline size_t bound_deserialize_array(bound_t* dst, const void* src, size_t size);
static inline size_t bound_serialized_size_array(bound_t* src, size_t size);

#ifdef __cplusplus
}
#endif

#endif
