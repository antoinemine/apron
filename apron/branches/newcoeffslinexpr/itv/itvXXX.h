/* ********************************************************************** */
/* itvXXX.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITVXXX_H_
#define _ITVXXX_H_

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "itvXXX_types.h"
#include "boundXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* itvXXX_internal */
/* ********************************************************************** */

itvXXX_internal_ptr itvXXX_internal_alloc(void);
  /* Allocate and initialize internal workspace */
void itvXXX_internal_free(itvXXX_internal_ptr intern);
  /* Clear and free internal workspace */

void itvXXX_internal_init(itvXXX_internal_t intern);
void itvXXX_internal_clear(itvXXX_internal_t intern);
  /* GMP Semantics */

/* ********************************************************************** */
/* itv */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */
static inline void itvXXX_init(itvXXX_t a);
static inline void itvXXX_init_array(itvXXX_t* a, size_t size);
static inline void itvXXX_init_set(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_clear(itvXXX_t a);
static inline void itvXXX_clear_array(itvXXX_t* a, size_t size);
static inline itvXXX_t* itvXXX_array_alloc(size_t size);
static inline void itvXXX_array_free(itvXXX_t* a, size_t size);

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
bool itvXXX_canonicalize(itvXXX_internal_t intern, itvXXX_t a, bool integer);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
static inline bool itvXXX_is_int(itvXXX_internal_t intern, itvXXX_t a);
  /* has integer bounds */
static inline bool itvXXX_is_point(itvXXX_t a);
  /* Return true iff the interval is a single point */
static inline bool itvXXX_is_zero(itvXXX_t a);
  /* Return true iff the interval is a single zero point */
static inline bool itvXXX_is_pos(itvXXX_t a);
static inline bool itvXXX_is_neg(itvXXX_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
static inline bool itvXXX_is_top(itvXXX_t a);
static inline bool itvXXX_is_bottom(itvXXX_internal_t intern, itvXXX_t a);
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
static inline void itvXXX_range_rel(itvXXX_internal_t intern, boundXXX_t a, itvXXX_t b);
  /* a=(max b - min b) / (|a+b|/2) */

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */
static inline bool itvXXX_meet(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c);
  /* Assign a with the intersection of b and c */
static inline void itvXXX_join(itvXXX_t a, itvXXX_t b, itvXXX_t c);
  /* Assign a with the union of b and c */
static inline void itvXXX_widening(itvXXX_t a, itvXXX_t b, itvXXX_t c);
  /* Assign a with the standard interval widening of b by c */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */
static inline void itvXXX_add(itvXXX_t a, itvXXX_t b, itvXXX_t c);
void itvXXX_sub(itvXXX_t a, itvXXX_t b, itvXXX_t c);
void itvXXX_neg(itvXXX_t a, itvXXX_t b);
void itvXXX_mul(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c);
void itvXXX_div(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c);
static inline void itvXXX_add_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
static inline void itvXXX_sub_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
void itvXXX_mul_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
void itvXXX_div_num(itvXXX_t a, itvXXX_t b, numXXX_t c);
static inline void itvXXX_add_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
static inline void itvXXX_sub_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
void itvXXX_mul_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
void itvXXX_div_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c);
bool itvXXX_sqrt(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b);
void itvXXX_abs(itvXXX_t a, itvXXX_t b);
static inline void itvXXX_mul_2exp(itvXXX_t a, itvXXX_t b, int c);

static inline void itvXXX_magnitude(boundXXX_t a, itvXXX_t b);
  /* get the absolute value of maximal bound */

void itvXXX_mod(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c, bool is_int);
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
static inline void itvXXX_to_float(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b);
static inline void itvXXX_to_double(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool itvXXX_set_lint(itvXXX_t a, long int b, numinternal_t intern);
static inline bool itvXXX_set_lint2(itvXXX_t a, long int b, long int c, numinternal_t intern);
static inline bool itvXXX_set_llint(itvXXX_t a, long long int b, numinternal_t intern);
static inline bool itvXXX_set_llint2(itvXXX_t a, long long int b, long long int c, numinternal_t intern);
static inline bool itvXXX_set_mpz(itvXXX_t a, mpz_t b, numinternal_t intern);
static inline bool itvXXX_set_mpz2(itvXXX_t a, mpz_t b, mpz_t c, numinternal_t intern);
static inline bool itvXXX_set_lfrac(itvXXX_t a, long int i, long int j, numinternal_t intern);
static inline bool itvXXX_set_lfrac2(itvXXX_t a, long int i, long int j, long int k, long int l, numinternal_t intern);
static inline bool itvXXX_set_llfrac(itvXXX_t a, long long int i, long long int j, numinternal_t intern);
static inline bool itvXXX_set_llfrac2(itvXXX_t a, long long int i, long long int j, long long int k, long long int l, numinternal_t intern);
static inline bool itvXXX_set_mpq(itvXXX_t a, mpq_t b, numinternal_t intern);
static inline bool itvXXX_set_mpq2(itvXXX_t a, mpq_t b, mpq_t c, numinternal_t intern);
static inline bool itvXXX_set_double(itvXXX_t a, double b, numinternal_t intern);
static inline bool itvXXX_set_double2(itvXXX_t a, double b, double c, numinternal_t intern);
static inline bool itvXXX_set_ldouble(itvXXX_t a, long double b, numinternal_t intern);
static inline bool itvXXX_set_ldouble2(itvXXX_t a, long double b, long double c, numinternal_t intern);
static inline bool itvXXX_set_mpfr(itvXXX_t a, mpfr_t b, numinternal_t intern);
static inline bool itvXXX_set_mpfr2(itvXXX_t a, mpfr_t b, mpfr_t c, numinternal_t intern);

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
static inline void itvXXX_init_array(itvXXX_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) itvXXX_init(a[i]);
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
static inline itvXXX_t* itvXXX_array_alloc(size_t size)
{
  itvXXX_t* res = (itvXXX_t*)malloc(size*sizeof(itvXXX_t));
  itvXXX_init_array(res,size);
  return res;
}
static inline void itvXXX_array_free(itvXXX_t* a, size_t size)
{
  itvXXX_clear_array(a,size);
  free(a);
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
  boundXXX_set_num(a->sup,b);
  numXXX_neg(c,c);
  boundXXX_set_num(a->neginf,c);
  numXXX_neg(c,c);
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

static inline bool itvXXX_is_int(itvXXX_internal_t intern, itvXXX_t a)
{
  boundXXX_trunc(intern->muldiv_bound,a->sup);
  if (boundXXX_cmp(intern->muldiv_bound,a->sup)) return false;
  boundXXX_trunc(intern->muldiv_bound,a->neginf);
  return !boundXXX_cmp(intern->muldiv_bound,a->neginf);
}
static inline bool itvXXX_is_point(itvXXX_t a)
{
  if (!boundXXX_infty(a->neginf) && !boundXXX_infty(a->sup)){
    numXXX_ptr b = boundXXX_numref(a->neginf);
    numXXX_neg(b,b);
    bool res = numXXX_equal(b,boundXXX_numref(a->sup));
    numXXX_neg(b,b);
    return res;
  }
  else
    return false;
}
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
static inline bool itvXXX_is_bottom(itvXXX_internal_t intern, itvXXX_t a)
{
  return itvXXX_canonicalize(intern, a, false);
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

static inline void itvXXX_range_rel(itvXXX_internal_t intern, boundXXX_t a, itvXXX_t b)
{
  boundXXX_add(a,b->sup,b->neginf);
  if (!boundXXX_infty(a)) {
    itvXXX_magnitude(intern->muldiv_bound,b);
    boundXXX_div_2(intern->muldiv_bound,intern->muldiv_bound);
    boundXXX_div(a,a,intern->muldiv_bound);
  }
}

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */

static inline bool itvXXX_meet(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  boundXXX_min(a->sup,b->sup,c->sup);
  boundXXX_min(a->neginf,b->neginf,c->neginf);
  return itvXXX_canonicalize(intern,a,false);
}
static inline void itvXXX_join(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  boundXXX_max(a->sup,b->sup,c->sup);
  boundXXX_max(a->neginf,b->neginf,c->neginf);
}
static inline void itvXXX_widening(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  boundXXX_widening(a->sup,b->sup,c->sup);
  boundXXX_widening(a->neginf,b->neginf,c->neginf);
}

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


static inline void itvXXX_to_float(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b)
{ boundXXX_to_float(a->sup,b->sup,intern->num);
  boundXXX_to_float(a->neginf,b->neginf,intern->num); }

static inline void itvXXX_to_double(itvXXX_internal_t intern, itvXXX_t a, itvXXX_t b)
{ boundXXX_to_double(a->sup,b->sup,intern->num);
  boundXXX_to_double(a->neginf,b->neginf,intern->num); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool itvXXX_set_lint(itvXXX_t a, long int b, numinternal_t intern)
{ return itvXXX_set_lint2(a,b,b,intern); }
static inline bool itvXXX_set_lint2(itvXXX_t a, long int b, long int c, numinternal_t intern)
{
  bool res = boundXXX_set_lint(a->neginf,-b,intern);
  return boundXXX_set_lint(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_llint(itvXXX_t a, long long int b, numinternal_t intern)
{ return itvXXX_set_llint2(a,b,b,intern); }
static inline bool itvXXX_set_llint2(itvXXX_t a, long long int b, long long int c, numinternal_t intern)
{
  bool res = boundXXX_set_llint(a->neginf,-b,intern);
  return boundXXX_set_llint(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_mpz(itvXXX_t a, mpz_t b, numinternal_t intern)
{ return itvXXX_set_mpz2(a,b,b,intern); }
static inline bool itvXXX_set_mpz2(itvXXX_t a, mpz_t b, mpz_t c, numinternal_t intern)
{
  mpz_neg(b,b);
  bool res = boundXXX_set_mpz(a->neginf,b,intern);
  mpz_neg(b,b);
  return boundXXX_set_mpz(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_lfrac(itvXXX_t a, long int i, long int j, numinternal_t intern)
{ return itvXXX_set_lfrac2(a,i,j,i,j,intern); }
static inline bool itvXXX_set_lfrac2(itvXXX_t a, long int i, long int j, long int k, long int l, numinternal_t intern)
{
  bool res = boundXXX_set_lfrac(a->neginf,-i,j,intern);
  return boundXXX_set_lfrac(a->sup,k,l,intern) && res;
}
static inline bool itvXXX_set_llfrac(itvXXX_t a, long long int i, long long int j, numinternal_t intern)
{ return itvXXX_set_llfrac2(a,i,j,i,j,intern); }
static inline bool itvXXX_set_llfrac2(itvXXX_t a, long long int i, long long int j, long long int k, long long int l, numinternal_t intern)
{
  bool res = boundXXX_set_llfrac(a->neginf,-i,j,intern);
  return boundXXX_set_llfrac(a->sup,k,l,intern) && res;
}
static inline bool itvXXX_set_mpq(itvXXX_t a, mpq_t b, numinternal_t intern)
{ return itvXXX_set_mpq2(a,b,b,intern); }
static inline bool itvXXX_set_mpq2(itvXXX_t a, mpq_t b, mpq_t c, numinternal_t intern)
{
  mpq_neg(b,b);
  bool res = boundXXX_set_mpq(a->neginf,b,intern);
  mpq_neg(b,b);
  return boundXXX_set_mpq(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_double(itvXXX_t a, double b, numinternal_t intern)
{ return itvXXX_set_double2(a,b,b,intern); }
static inline bool itvXXX_set_double2(itvXXX_t a, double b, double c, numinternal_t intern)
{
  bool res = boundXXX_set_double(a->neginf,-b,intern);
  return boundXXX_set_double(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_ldouble(itvXXX_t a, long double b, numinternal_t intern)
{ return itvXXX_set_ldouble2(a,b,b,intern); }
static inline bool itvXXX_set_ldouble2(itvXXX_t a, long double b, long double c, numinternal_t intern)
{
  bool res = boundXXX_set_ldouble(a->neginf,-b,intern);
  return boundXXX_set_ldouble(a->sup,c,intern) && res;
}
static inline bool itvXXX_set_mpfr(itvXXX_t a, mpfr_t b, numinternal_t intern)
{ return itvXXX_set_mpfr2(a,b,b,intern); }
static inline bool itvXXX_set_mpfr2(itvXXX_t a, mpfr_t b, mpfr_t c, numinternal_t intern)
{
  mpfr_neg(b,b,GMP_RNDU);
  bool res = boundXXX_set_mpfr(a->neginf,b,intern);
  mpfr_neg(b,b,GMP_RNDU);
  return boundXXX_set_mpfr(a->sup,c,intern) && res;
}

#ifdef __cplusplus
}
#endif

#endif
