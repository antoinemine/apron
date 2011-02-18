/* ********************************************************************** */
/* itvXXX.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITVXXX_H_
#define _ITVXXX_H_

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#include "num_types.h"
#include "boundXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* itv */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */
static inline void itvXXX_init(itvXXX_t a);
void itvXXX_init_array(itvXXX_t* a, size_t size);
static inline void itvXXX_init_set(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_clear(itvXXX_t a);
static inline void itvXXX_clear_array(itvXXX_t* a, size_t size);
itvXXX_t* itvXXX_array_alloc(size_t size);
void itvXXX_array_free(itvXXX_t* a, size_t size);

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void itvXXX_set(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_set_num(itvXXX_t a, numXXX_t b);
static inline void itvXXX_set_num2(itvXXX_t a, numXXX_t b, numXXX_t c);
static inline void itvXXX_set_int(itvXXX_t a, long int b);
static inline void itvXXX_set_int2(itvXXX_t a, long int b, long int c);
static inline void itvXXX_set_bottom(itvXXX_t a);
static inline void itvXXX_set_top(itvXXX_t a);
static inline void itvXXX_swap(itvXXX_t a, itvXXX_t b);

/* a = [-b,b] */
static inline void itvXXX_set_unit_num(itvXXX_t a, numXXX_t b);
static inline void itvXXX_set_unit_bound(itvXXX_t a, boundXXX_t b);

/* a = b + [-c,c] */
static inline void itvXXX_enlarge_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */
bool itvXXX_canonicalize(itvXXX_t a, bool integer, num_internal_t intern);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
bool itvXXX_is_int(itvXXX_t a, num_internal_t intern);
  /* has integer bounds */
bool itvXXX_is_point(itvXXX_t a);
  /* Return true iff the interval is a single point */
static inline bool itvXXX_is_zero(itvXXX_t a);
  /* Return true iff the interval is a single zero point */
static inline bool itvXXX_is_pos(itvXXX_t a);
static inline bool itvXXX_is_neg(itvXXX_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
static inline bool itvXXX_is_top(itvXXX_t a);
static inline bool itvXXX_is_bottom(itvXXX_t a, num_internal_t intern);
  /* Return true iff the interval is resp. [-oo,+oo] or empty */
static inline bool itvXXX_is_leq(itvXXX_t a, itvXXX_t b);
  /* Inclusion test */
static inline bool itvXXX_is_eq(itvXXX_t a, itvXXX_t b);
  /* Equality test */

static inline int itvXXX_hash(itvXXX_t a);
  /* Hash code */
int itvXXX_cmp(itvXXX_t a, itvXXX_t b);
int itvXXX_cmp_zero(itvXXX_t a);
  /* Comparison:
     0: equality
     -1: i1 included in i2
     +1: i2 included in i1
     -2: i1->sup less than i2->sup
     +2: i1->sup greater than i2->sup
  */
static inline void itvXXX_range_abs(boundXXX_t a, itvXXX_t b);
  /* a=(max b - min b) */
void itvXXX_range_rel(boundXXX_t a, itvXXX_t b, num_internal_t intern);
  /* a=(max b - min b) / (|a+b|/2) */

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */
bool itvXXX_meet(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern);
  /* Assign a with the intersection of b and c */
void itvXXX_join(itvXXX_t a, itvXXX_t b, itvXXX_t c);
  /* Assign a with the union of b and c */
void itvXXX_widening(itvXXX_t a, itvXXX_t b, itvXXX_t c);
  /* Assign a with the standard interval widening of b by c */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */
static inline void itvXXX_add(itvXXX_t a, itvXXX_t b, itvXXX_t c);
void itvXXX_sub(itvXXX_t a, itvXXX_t b, itvXXX_t c);
void itvXXX_neg(itvXXX_t a, itvXXX_t b);
void itvXXX_mul(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern);
void itvXXX_div(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern);
static inline void itvXXX_add_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
static inline void itvXXX_sub_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
void itvXXX_mul_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
void itvXXX_div_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
static inline void itvXXX_add_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
static inline void itvXXX_sub_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
void itvXXX_mul_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
void itvXXX_div_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
bool itvXXX_sqrt(itvXXX_t a, itvXXX_t b, num_internal_t intern);
void itvXXX_abs(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_mul_2exp(itvXXX_t a, itvXXX_t b, int c);

static inline void itvXXX_magnitude(boundXXX_t a, itvXXX_t b);
  /* get the absolute value of maximal bound */

void itvXXX_mod(itvXXX_t a, itvXXX_t b, itvXXX_t c, bool is_int, num_internal_t intern);
  /* x mod y = x - y*trunc(x/y) */

/* ====================================================================== */
/* Casts */
/* ====================================================================== */
/* Integer casts (rounding towards +oo, -oo, 0, or worst-case) */
static inline void itvXXX_ceil(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_floor(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_trunc(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_to_int(itvXXX_t a, itvXXX_t b);

/* Floating-point casts (worst cases) */
void itvXXX_to_float(itvXXX_t a, itvXXX_t b, num_internal_t intern);
void itvXXX_to_double(itvXXX_t a, itvXXX_t b, num_internal_t intern);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool itvXXX_set_generic(num_internal_t intern,
			itvXXX_t a, itv_tag_t tag, va_list* va);
bool itvXXX_set_val(num_internal_t intern,
		    itvXXX_t a, itv_tag_t tag, ...);
  /* This function assigns the interval from a value defined by a number of
     arguments as specified in the definition of the type itv_tag_t

     Returns true iff all conversions were exact.

     Example:
     itvXXX_set_val(intern,itv,ITV_LFRAC,7,9);
     itvXXX_set_val(intern,ITV_DOUBLE2,-3.0,4.5);
     itvXXX_set_val(intern,ITV_LLINT,3LL);
     sets resp. itv to 7/9, [-3,4.5], 3.
     assuming that all the number conversions were exact.
  */
bool itvXXX_set_lint(itvXXX_t a, long int b, num_internal_t intern);
bool itvXXX_set_lint2(itvXXX_t a, long int b, long int c, num_internal_t intern);
bool itvXXX_set_llint(itvXXX_t a, long long int b, num_internal_t intern);
bool itvXXX_set_llint2(itvXXX_t a, long long int b, long long int c, num_internal_t intern);
bool itvXXX_set_mpz(itvXXX_t a, mpz_t b, num_internal_t intern);
bool itvXXX_set_mpz2(itvXXX_t a, mpz_t b, mpz_t c, num_internal_t intern);
bool itvXXX_set_lfrac(itvXXX_t a, long int i, long int j, num_internal_t intern);
bool itvXXX_set_lfrac2(itvXXX_t a, long int i, long int j, long int k, long int l, num_internal_t intern);
bool itvXXX_set_llfrac(itvXXX_t a, long long int i, long long int j, num_internal_t intern);
bool itvXXX_set_llfrac2(itvXXX_t a, long long int i, long long int j, long long int k, long long int l, num_internal_t intern);
bool itvXXX_set_mpq(itvXXX_t a, mpq_t b, num_internal_t intern);
bool itvXXX_set_mpq2(itvXXX_t a, mpq_t b, mpq_t c, num_internal_t intern);
bool itvXXX_set_double(itvXXX_t a, double b, num_internal_t intern);
bool itvXXX_set_double2(itvXXX_t a, double b, double c, num_internal_t intern);
bool itvXXX_set_ldouble(itvXXX_t a, long double b, num_internal_t intern);
bool itvXXX_set_ldouble2(itvXXX_t a, long double b, long double c, num_internal_t intern);
bool itvXXX_set_mpfr(itvXXX_t a, mpfr_t b, num_internal_t intern);
bool itvXXX_set_mpfr2(itvXXX_t a, mpfr_t b, mpfr_t c, num_internal_t intern);

MACRO_ALLZ
bool itvXXX_set_numZZZ(itvXXX_t a, numZZZ_t b, num_internal_t intern);
bool itvXXX_set_numZZZ2(itvXXX_t a, numZZZ_t b, numZZZ_t c, num_internal_t intern);
ENDMACRO

MACRO_MAINZ
bool itvXXX_set_itvZZZ(itvXXX_t a, itvZZZ_t b, num_internal_t intern);
bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* Printing */
/* ====================================================================== */
int itvXXX_snprint(char* s, size_t size, itvXXX_t a);
void itvXXX_fprint(FILE* stream, itvXXX_t a);
void itvXXX_print(itvXXX_t a);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */

static inline void itvXXX_init(itvXXX_t a)
{
  boundXXX_init(a->neginf);
  boundXXX_init(a->sup);
}
static inline void itvXXX_init_set(itvXXX_t a, itvXXX_t b)
{
  boundXXX_init_set(a->neginf,b->neginf);
  boundXXX_init_set(a->sup,b->sup);
}
static inline void itvXXX_clear(itvXXX_t a)
{
  boundXXX_clear(a->neginf);
  boundXXX_clear(a->sup);
}
static inline void itvXXX_clear_array(itvXXX_t* a, size_t size)
{
#if !NUMXXX_NATIVE
  size_t i;
  for (i=0; i<size; i++) itvXXX_clear(a[i]);
#endif
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void itvXXX_set(itvXXX_t a, itvXXX_t b)
{
  boundXXX_set(a->neginf,b->neginf);
  boundXXX_set(a->sup,b->sup);
}
static inline void itvXXX_set_num(itvXXX_t a, numXXX_t b)
{
  boundXXX_set_num(a->sup,b);
  boundXXX_neg(a->neginf,a->sup);
}
static inline void itvXXX_set_num2(itvXXX_t a, numXXX_t b, numXXX_t c)
{
  numXXX_neg(b,b);
  boundXXX_set_num(a->neginf,b);
  numXXX_neg(b,b);
  boundXXX_set_num(a->sup,c);
}
static inline void itvXXX_set_int(itvXXX_t a, long int b)
{
  boundXXX_set_int(a->neginf,-b);
  boundXXX_set_int(a->sup,b);
}
static inline void itvXXX_set_int2(itvXXX_t a, long int b, long int c)
{
  boundXXX_set_int(a->neginf,-b);
  boundXXX_set_int(a->sup,c);
}
static inline void itvXXX_set_bottom(itvXXX_t a)
{
  boundXXX_set_int(a->neginf,-1);
  boundXXX_set_int(a->sup,-1);
}
static inline void itvXXX_set_top(itvXXX_t a)
{
  boundXXX_set_infty(a->neginf,1);
  boundXXX_set_infty(a->sup,1);
}
static inline void itvXXX_swap(itvXXX_t a, itvXXX_t b)
{ itvXXX_t t; *t=*a;*a=*b;*b=*t; }

static inline void itvXXX_set_unit_num(itvXXX_t a, numXXX_t b)
{
  boundXXX_set_num(a->neginf,b);
  boundXXX_set_num(a->sup,b);
}
static inline void itvXXX_set_unit_bound(itvXXX_t a, boundXXX_t b)
{
  boundXXX_set(a->neginf,b);
  boundXXX_set(a->sup,b);
}
static inline void itvXXX_enlarge_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  boundXXX_add(a->neginf,b->neginf,c);
  boundXXX_add(a->sup,b->sup,c);
}

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */

static inline bool itvXXX_is_zero(itvXXX_t a)
{
  return boundXXX_sgn(a->neginf)==0 && boundXXX_sgn(a->sup)==0;
}
static inline bool itvXXX_is_pos(itvXXX_t a)
{ return (boundXXX_sgn(a->neginf)<=0); }

static inline bool itvXXX_is_neg(itvXXX_t a)
{ return (boundXXX_sgn(a->sup)<=0); }
static inline bool itvXXX_is_top(itvXXX_t a)
{
  return boundXXX_infty(a->neginf) && boundXXX_infty(a->sup);
}
static inline bool itvXXX_is_bottom(itvXXX_t a, num_internal_t intern)
{
  return itvXXX_canonicalize(a, false, intern);
}
static inline bool itvXXX_is_leq(itvXXX_t a, itvXXX_t b)
{
  return boundXXX_cmp(a->sup,b->sup)<=0 && boundXXX_cmp(a->neginf,b->neginf)<=0;
}
static inline bool itvXXX_is_eq(itvXXX_t a, itvXXX_t b)
{
  return boundXXX_equal(a->sup,b->sup) && boundXXX_equal(a->neginf,b->neginf);
}

static inline int itvXXX_hash(itvXXX_t a)
{
  return (5*boundXXX_hash(a->neginf) + 7*boundXXX_hash(a->sup));
}
static inline void itvXXX_range_abs(boundXXX_t a, itvXXX_t b)
{ boundXXX_add(a,b->sup,b->neginf); }

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */

static inline void itvXXX_add(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  boundXXX_add(a->sup,b->sup,c->sup);
  boundXXX_add(a->neginf,b->neginf,c->neginf);
}
static inline void itvXXX_add_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  boundXXX_add_num(a->sup,b->sup,c);
  boundXXX_sub_num(a->neginf,b->neginf,c);
}
static inline void itvXXX_sub_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  boundXXX_sub_num(a->sup,b->sup,c);
  boundXXX_add_num(a->neginf,b->neginf,c);
}
static inline void itvXXX_add_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  boundXXX_add(a->sup,b->sup,c);
  boundXXX_sub(a->neginf,b->neginf,c);
}
static inline void itvXXX_sub_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  boundXXX_sub(a->sup,b->sup,c);
  boundXXX_add(a->neginf,b->neginf,c);
}
static inline void itvXXX_mul_2exp(itvXXX_t a, itvXXX_t b, int c)
{ boundXXX_mul_2exp(a->sup,b->sup,c); boundXXX_mul_2exp(a->neginf,b->neginf,c); }

static inline void itvXXX_magnitude(boundXXX_t a, itvXXX_t b)
{
  if (boundXXX_sgn(b->neginf)<=0) boundXXX_set(a,b->sup);
  else if (boundXXX_sgn(b->sup)<=0) boundXXX_set(a,b->neginf);
  else boundXXX_max(a,b->neginf,b->sup);
}

/* ====================================================================== */
/* Casts */
/* ====================================================================== */

static inline void itvXXX_ceil(itvXXX_t a, itvXXX_t b)
{ boundXXX_ceil(a->sup,b->sup); boundXXX_floor(a->neginf,b->neginf); }

static inline void itvXXX_floor(itvXXX_t a, itvXXX_t b)
{ boundXXX_floor(a->sup,b->sup);  boundXXX_ceil(a->neginf,b->neginf); }

static inline void itvXXX_trunc(itvXXX_t a, itvXXX_t b)
{ boundXXX_trunc(a->sup,b->sup); boundXXX_trunc(a->neginf,b->neginf); }

static inline void itvXXX_to_int(itvXXX_t a, itvXXX_t b)
{ boundXXX_ceil(a->sup,b->sup); boundXXX_ceil(a->neginf,b->neginf); }


#ifdef __cplusplus
}
#endif

#endif
