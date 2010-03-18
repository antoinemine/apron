/* ********************************************************************** */
/* numflt_tmpl.h */
/* ********************************************************************** */

#ifndef _NUMFLT_H_
#define _NUMFLT_H_

#include "numConfig.h"
#include "float.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NUM_NUMD
typedef double numflt_native;
typedef numflt_native* numflt_ptr;
typedef numflt_native numflt_t[1];
#define NUMFLTD_ZERO 0.0
#define NUMFLTD_ONE 1.0
#define NUMFLTD_MANT_DIG DBL_MANT_DIG
#define NUMFLTD_MAX NUMFLTD_ONE/NUMFLTD_ZERO
#define NUMD_MAX NUMFLTD_ONE/NUMFLTD_ZERO

#ifndef NUMFLTD_PRINT_PREC
#define NUMFLTD_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTD_PRINT_PREC to be any other
   expression (including variable and function call).
*/

#elif NUM_NUMDL
typedef long double numflt_native;
typedef numflt_native* numflt_ptr;
typedef numflt_native numflt_t[1];
#define NUMFLTDL_ZERO 0.0L
#define NUMFLTDL_ONE 1.0L
#define NUMFLTDL_MANT_DIG LDBL_MANT_DIG
#define NUMFLTDL_MAX NUMFLTDL_ONE/NUMFLTDL_ZERO
#define NUMDL_MAX NUMFLTDL_ONE/NUMFLTDL_ZERO

#ifndef NUMFLTDL_PRINT_PREC
#define NUMFLTDL_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTDL_PRINT_PREC to be any other
   expression (including variable and function call).
*/

#elif NUM_NUMMPFR
typedef mpfr_ptr numflt_ptr;
typedef mpfr_t numflt_t;
#undef NUMFLTMPFR_MAX
#undef NUMMPFR_MAX
#undef NUMFLTMPFR_ZERO
#undef NUMFLTMPFR_ONE
#undef NUMFLTMPFR_MANT_DIG

#ifndef NUMFLTMPFR_PRINT_PREC
#define NUMFLTMPFR_PRINT_PREC 20
#endif
/* Number of significant digits used for printing.
   Defaults to 20, but you can override NUMFLTMPFR_PRINT_PREC to be any other
   expression (including variable and function call).
*/

#else
#error "HERE"
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
