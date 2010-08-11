/* ********************************************************************** */
/* bound.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUNDTYPES_H_
#define _BOUNDTYPES_H_

#include "numTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Types */
/* ********************************************************************** */

typedef numIl_t boundIl_t;
#define boundIl_numref(a) a
#define _boundIl_inf(a)

typedef numIll_t boundIll_t;
#define boundIll_numref(a) a
#define _boundIll_inf(a)

typedef struct _boundMPZ_struct {
  numMPZ_t num; /* always allocated, even if inf=1 */
  char inf;  /* 1 => +/-oo; the sign of num decides the sign of the oo
		0 => >-oo, <+oo */
} boundMPZ_t[1];
#define boundMPZ_numref(a) a->num
#define _boundMPZ_inf(a) a->inf = 0

typedef numRl_t boundRl_t;
#define boundRl_numref(a) a
#define _boundRl_inf(a)

typedef numRll_t boundRll_t;
#define boundRll_numref(a) a
#define _boundRll_inf(a)

typedef numMPQ_t boundMPQ_t;
#define boundMPQ_numref(a) a
#define _boundMPQ_inf(a)

typedef numD_t boundD_t;
#define boundD_numref(a) a
#define _boundD_inf(a)

typedef numDl_t boundDl_t;
#define boundDl_numref(a) a
#define _boundDl_inf(a)

typedef numMPFR_t boundMPFR_t;
#define boundMPFR_numref(a) a
#define _boundMPFR_inf(a)

/*

**********************************************************************
Functions
**********************************************************************

static inline bool boundXXX_infty(boundXXX_t a);
Macro:
static inline numXXX_t boundXXX_numref(boundXXX_t a);

======================================================================
Assignement
======================================================================
static inline void boundXXX_set(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_set_array(boundXXX_t* a, boundXXX_t* b, size_t size);
static inline void boundXXX_set_int(boundXXX_t a, long int i);
static inline void boundXXX_set_infty(boundXXX_t a, int sgn);
static inline void boundXXX_swap(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_set_num(boundXXX_t a, numXXX_t b);

======================================================================
Constructors and Destructors
======================================================================

static inline void boundXXX_init(boundXXX_t a);
static inline void boundXXX_init_array(boundXXX_t* a, size_t size);

static inline void boundXXX_init_set(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_init_set_int(boundXXX_t a, long int i);
static inline void boundXXX_init_set_infty(boundXXX_t a, int sgn);

static inline void boundXXX_clear(boundXXX_t a);
static inline void boundXXX_clear_array(boundXXX_t* a, size_t size);

======================================================================
Arithmetic Operations
======================================================================

   +oo + -oo  \
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

static inline void boundXXX_neg(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_abs(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_add(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_add_uint(boundXXX_t a, boundXXX_t b, unsigned long int c);
static inline void boundXXX_add_num(boundXXX_t a, boundXXX_t b, numXXX_t c);
static inline void boundXXX_sub(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_sub_uint(boundXXX_t a, boundXXX_t b, unsigned long int c);
static inline void boundXXX_sub_num(boundXXX_t a, boundXXX_t b, numXXX_t c);
static inline void boundXXX_mul(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_mul_num(boundXXX_t a, boundXXX_t b, numXXX_t c);
static inline void boundXXX_mul_2(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_div(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_div_num(boundXXX_t a, boundXXX_t b, numXXX_t c);
static inline void boundXXX_div_2(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_min(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_max(boundXXX_t a, boundXXX_t b, boundXXX_t c);
static inline void boundXXX_mul_2exp(boundXXX_t a, boundXXX_t b, int c);
static inline void boundXXX_floor(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_ceil(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_trunc(boundXXX_t a, boundXXX_t b);
static inline void boundXXX_sqrt(boundXXX_t up, boundXXX_t down, boundXXX_t b);
static inline void boundXXX_widening(boundXXX_t a, boundXXX_t b, boundXXX_t c);

======================================================================
Floating-point casts
======================================================================

always rounds toward +oo
static inline void boundXXX_to_float(boundXXX_t a, boundXXX_t b, numinternal_t intern);
static inline void boundXXX_to_double(boundXXX_t a, boundXXX_t b, numinternal_t intern);

======================================================================
Arithmetic Tests
======================================================================

static inline int boundXXX_sgn(boundXXX_t a);
static inline int boundXXX_cmp(boundXXX_t a, boundXXX_t b);
static inline int boundXXX_cmp_int(boundXXX_t a, long int b);
static inline int boundXXX_cmp_num(boundXXX_t a, numXXX_t b);
static inline bool boundXXX_equal(boundXXX_t a, boundXXX_t b);
static inline int boundXXX_hash(boundXXX_t a);

======================================================================
Conversions
======================================================================

static inline bool boundXXX_set_lint(boundXXX_t a, long int b, numinternal_t intern);
static inline bool boundXXX_set_llint(boundXXX_t a, long long int b, numinternal_t intern);
static inline bool boundXXX_set_mpz(boundXXX_t a, mpz_t b, numinternal_t intern);
static inline bool boundXXX_set_lfrac(boundXXX_t a, long int i, long int j, numinternal_t intern);
static inline bool boundXXX_set_llfrac(boundXXX_t a, long long int i, long long int j, numinternal_t intern);
static inline bool boundXXX_set_mpq(boundXXX_t a, mpq_t b, numinternal_t intern);
static inline bool boundXXX_set_double(boundXXX_t a, double b, numinternal_t intern);
static inline bool boundXXX_set_ldouble(boundXXX_t a, long double b, numinternal_t intern);
static inline bool boundXXX_set_mpfr(boundXXX_t a, mpfr_t b, numinternal_t intern);

static inline bool boundXXX_set_numIl(boundXXX_t a, numIl_t b, numinternal_t internal);
static inline bool boundXXX_set_numIll(boundXXX_t a, numIll_t b, numinternal_t internal);
static inline bool boundXXX_set_numMPZ(boundXXX_t a, numMPZ_t b, numinternal_t internal);
static inline bool boundXXX_set_numRl(boundXXX_t a, numRl_t b, numinternal_t internal);
static inline bool boundXXX_set_numRll(boundXXX_t a, numRll_t b, numinternal_t internal);
static inline bool boundXXX_set_numD(boundXXX_t a, numD_t b, numinternal_t internal);
static inline bool boundXXX_set_numDl(boundXXX_t a, numDl_t b, numinternal_t internal);
static inline bool boundXXX_set_numMPFR(boundXXX_t a, numMPFR_t b, numinternal_t internal);

======================================================================
Printing
======================================================================

static inline void boundXXX_print(boundXXX_t a);
static inline void boundXXX_fprint(FILE* stream, boundXXX_t a);
static inline int boundXXX_snprint(char* s, size_t size, boundXXX_t a);

======================================================================
Serialization
======================================================================

Note: call _init before _deserialize

static inline size_t boundXXX_serialize(void* dst, boundXXX_t src);
static inline size_t boundXXX_deserialize(boundXXX_t dst, const void* src);
static inline size_t boundXXX_serialized_size(boundXXX_t a);

static inline size_t boundXXX_serialize_array(void* dst, boundXXX_t* src, size_t size);
static inline size_t boundXXX_deserialize_array(boundXXX_t* dst, const void* src, size_t size);
static inline size_t boundXXX_serialized_size_array(boundXXX_t* src, size_t size);

*/

#ifdef __cplusplus
}
#endif

#endif
