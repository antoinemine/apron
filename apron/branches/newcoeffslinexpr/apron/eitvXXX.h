/* ********************************************************************** */
/* eitvXXX.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _EITVXXX_H_
#define _EITVXXX_H_

#include <stdarg.h>
#include <stdio.h>
#include "num_types.h"
#include "itvXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

#define eitvXXX_supref(a) (a)->itv->sup
#define eitvXXX_neginfref(a) (a)->itv->neginf

/* ********************************************************************** */
/* eitv */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */
static inline void eitvXXX_init(eitvXXX_t a);
void eitvXXX_init_array(eitvXXX_t* a, size_t size);
static inline void eitvXXX_init_set(eitvXXX_t a, const eitvXXX_t b);
static inline void eitvXXX_init_set_int(eitvXXX_t a, long b);
static inline void eitvXXX_init_set_int2(eitvXXX_t a, long b, long c);
static inline void eitvXXX_init_set_num(eitvXXX_t a, const numXXX_t b);
static inline void eitvXXX_init_set_num2(eitvXXX_t a, const numXXX_t b, const numXXX_t c);
static inline void eitvXXX_clear(eitvXXX_t a);
static inline void eitvXXX_clear_array(eitvXXX_t* a, size_t size);
static inline eitvXXX_ptr eitvXXX_alloc(void);
static inline eitvXXX_ptr eitvXXX_alloc_set(const eitvXXX_t a);
static inline void eitvXXX_free(eitvXXX_ptr a);
eitvXXX_t* eitvXXX_array_alloc(size_t size);
void eitvXXX_array_free(eitvXXX_t* a, size_t size);

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void eitvXXX_set(eitvXXX_t a, const eitvXXX_t b);
static inline void eitvXXX_set_num(eitvXXX_t a, const numXXX_t b);
static inline void eitvXXX_set_num2(eitvXXX_t a, const numXXX_t b, const numXXX_t c);
static inline void eitvXXX_set_int(eitvXXX_t a, long int b);
static inline void eitvXXX_set_int2(eitvXXX_t a, long int b, long int c);
static inline void eitvXXX_set_bottom(eitvXXX_t a);
static inline void eitvXXX_set_top(eitvXXX_t a);
static inline void eitvXXX_swap(eitvXXX_t a, eitvXXX_t b);

/* a = [-b,b] */
static inline void eitvXXX_set_unit_num(eitvXXX_t a, const numXXX_t b);
static inline void eitvXXX_set_unit_bound(eitvXXX_t a, const boundXXX_t b);

/* a = b + [-c,c] */
static inline void eitvXXX_enlarge_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c);

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */
bool eitvXXX_canonicalize(eitvXXX_t a, bool integer);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
bool eitvXXX_is_int(const eitvXXX_t a, num_internal_t intern);
  /* has integer bounds */
static inline bool eitvXXX_is_point(const eitvXXX_t a);
  /* Return true iff the interval is a single point */
static inline bool eitvXXX_is_zero(const eitvXXX_t a);
  /* Return true iff the interval is a single zero point */
static inline bool eitvXXX_is_pos(const eitvXXX_t a);
static inline bool eitvXXX_is_neg(const eitvXXX_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
static inline bool eitvXXX_is_top(const eitvXXX_t a);
static inline bool eitvXXX_is_bottom(const eitvXXX_t a);
  /* Return true iff the interval is resp. [-oo,+oo] or empty */
static inline bool eitvXXX_is_leq(const eitvXXX_t a, const eitvXXX_t b);
  /* Inclusion test */
static inline bool eitvXXX_is_eq(const eitvXXX_t a, const eitvXXX_t b);
  /* Equality test */

static inline int eitvXXX_hash(const eitvXXX_t a);
  /* Hash code */
static inline int eitvXXX_cmp(const eitvXXX_t a, const eitvXXX_t b);
static inline int eitvXXX_cmp_zero(const eitvXXX_t a);
  /* Comparison:
     0: equality
     -1: i1 included in i2
     +1: i2 included in i1
     -2: i1->sup less than i2->sup
     +2: i1->sup greater than i2->sup
  */
static inline void eitvXXX_range_abs(boundXXX_t a, const eitvXXX_t b);
  /* a=(max b - min b) */
static inline void eitvXXX_range_rel(boundXXX_t a, const eitvXXX_t b, num_internal_t intern);
  /* a=(max b - min b) / (|a+b|/2) */

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */
bool eitvXXX_meet(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c);
  /* Assign a with the intersection of b and c */
static inline void eitvXXX_join(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c);
  /* Assign a with the union of b and c */
static inline void eitvXXX_widening(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c);
  /* Assign a with the standard interval widening of b by c */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */
static inline void eitvXXX_add(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c);
void eitvXXX_sub(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c);
static inline void eitvXXX_neg(eitvXXX_t a, const eitvXXX_t b);
void eitvXXX_mul(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c, num_internal_t intern);
void eitvXXX_div(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c, num_internal_t intern);
static inline void eitvXXX_add_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c);
static inline void eitvXXX_sub_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c);
void eitvXXX_mul_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c);
void eitvXXX_div_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c);
  /*
static inline void eitvXXX_add_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c);
static inline void eitvXXX_sub_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c);
  */
void eitvXXX_mul_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c);
void eitvXXX_div_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c);
static inline bool eitvXXX_sqrt(eitvXXX_t a, const eitvXXX_t b, num_internal_t intern);
void eitvXXX_abs(eitvXXX_t a, const eitvXXX_t b);
void eitvXXX_mul_2exp(eitvXXX_t a, const eitvXXX_t b, int c);

static inline void eitvXXX_magnitude(boundXXX_t a, const eitvXXX_t b);
  /* get the absolute value of maximal bound */

static inline void eitvXXX_mod(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c, bool is_int, num_internal_t intern);
  /* x mod y = x - y*trunc(x/y) */

/* ====================================================================== */
/* Casts */
/* ====================================================================== */
/* Integer casts (rounding towards +oo, -oo, 0, or worst-case) */
void eitvXXX_ceil(eitvXXX_t a, const eitvXXX_t b);
void eitvXXX_floor(eitvXXX_t a, const eitvXXX_t b);
void eitvXXX_trunc(eitvXXX_t a, const eitvXXX_t b);
void eitvXXX_to_int(eitvXXX_t a, const eitvXXX_t b);

/* Floating-point casts (worst cases) */
void eitvXXX_to_float(eitvXXX_t a, const eitvXXX_t b, num_internal_t intern);
void eitvXXX_to_double(eitvXXX_t a, const eitvXXX_t b, num_internal_t intern);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* See num_conv.h.tmpl for all the functions */

bool eitvXXX_set_val(num_internal_t intern,
		     eitvXXX_t a, eitv_tag_t tag, ...);
  /* This function assigns the interval from a value defined by a number of
     arguments as specified in the definition of the type eitv_tag_t

     Returns true iff all conversions were exact.

     Example:
     eitvXXX_set_val(intern,eitv,EITV_LFRAC,7L,9L);
     eitvXXX_set_val(intern,EITV_DOUBLE2,-3.0,4.5);
     eitvXXX_set_val(intern,EITV_LLINT,3LL);
     sets resp. eitv to 7/9, [-3,4.5], 3.
     assuming that all the number conversions were exact.
  */

/* internal */
bool eitvXXX_set_generic(num_internal_t intern,
			 eitvXXX_t a, eitv_tag_t tag, va_list* va);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */
int eitvXXX_snprint(char* s, int size, const eitvXXX_t a);
void eitvXXX_fprint(FILE* stream, const eitvXXX_t a);
void eitvXXX_print(const eitvXXX_t a);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t eitvXXX_serialize(void* dst, const eitvXXX_t src);
size_t eitvXXX_deserialize(eitvXXX_t dst, const void* src);
size_t eitvXXX_serialized_size(const eitvXXX_t a);
size_t eitvXXX_serialize_array(void* dst, eitvXXX_t* src, size_t size);
size_t eitvXXX_deserialize_array(eitvXXX_t* dst, const void* src, size_t size);
size_t eitvXXX_serialized_size_array(eitvXXX_t* src, size_t size);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */

static inline void eitvXXX_init(eitvXXX_t a)
{
  itvXXX_init(a->itv);
  a->eq=true;
}
static inline void eitvXXX_init_set(eitvXXX_t a, const eitvXXX_t b)
{
  itvXXX_init_set(a->itv,b->itv);
  a->eq = b->eq;
}
static inline void eitvXXX_init_set_int(eitvXXX_t a, long b)
{
  itvXXX_init_set_int(a->itv,b);
  a->eq = true;
}
  static inline void eitvXXX_init_set_int2(eitvXXX_t a, long b, long c)
{
  itvXXX_init_set_int2(a->itv,b,c);
  a->eq = b==c;
}
static inline void eitvXXX_init_set_num(eitvXXX_t a, const numXXX_t b)
{
  itvXXX_init_set_num(a->itv,b);
  a->eq = true;
}
static inline void eitvXXX_init_set_num2(eitvXXX_t a, const numXXX_t b, const numXXX_t c)
{
  itvXXX_init_set_num2(a->itv,b,c);
  a->eq = numXXX_equal(b,c);
}
static inline void eitvXXX_clear(eitvXXX_t a)
{ itvXXX_clear(a->itv); }
static inline void eitvXXX_clear_array(eitvXXX_t* a, size_t size)
{
#if !NUMXXX_NATIVE
  size_t i;
  for (i=0; i<size; i++) eitvXXX_clear(a[i]);
#endif
}
static inline eitvXXX_ptr eitvXXX_alloc()
{
  eitvXXX_ptr res = (eitvXXX_ptr)malloc(sizeof(eitvXXX_t));
  eitvXXX_init(res);
  return res;
}
static inline eitvXXX_ptr eitvXXX_alloc_set(const eitvXXX_t a)
{
  eitvXXX_ptr res = (eitvXXX_ptr)malloc(sizeof(eitvXXX_t));
  eitvXXX_init_set(res,a);
  return res;
}
static inline void eitvXXX_free(eitvXXX_ptr a)
{ eitvXXX_clear(a); free(a); }

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void eitvXXX_set(eitvXXX_t a, const eitvXXX_t b)
{
  itvXXX_set(a->itv,b->itv);
  a->eq = b->eq;
}
static inline void eitvXXX_set_num(eitvXXX_t a, const numXXX_t b)
{
  itvXXX_set_num(a->itv,b);
  a->eq = true;
}
static inline void eitvXXX_set_num2(eitvXXX_t a, const numXXX_t b, const numXXX_t c)
{
  itvXXX_set_num2(a->itv,b,c);
  a->eq = numXXX_equal(b,c);
}
static inline void eitvXXX_set_int(eitvXXX_t a, long int b)
{
  itvXXX_set_int(a->itv,b);
  a->eq = itvXXX_is_point(a->itv);
}
static inline void eitvXXX_set_int2(eitvXXX_t a, long int b, long int c)
{
  itvXXX_set_int2(a->itv,b,c);
  a->eq = itvXXX_is_point(a->itv);
}
static inline void eitvXXX_set_bottom(eitvXXX_t a)
{
  itvXXX_set_bottom(a->itv);
  a->eq = false;
}
static inline void eitvXXX_set_top(eitvXXX_t a)
{
  itvXXX_set_top(a->itv);
  a->eq = false;
}
static inline void eitvXXX_swap(eitvXXX_t a, eitvXXX_t b)
{ eitvXXX_t t; *t=*a;*a=*b;*b=*t; }

static inline void eitvXXX_set_unit_num(eitvXXX_t a, const numXXX_t b)
{
  itvXXX_set_unit_num(a->itv,b);
  a->eq = (numXXX_sgn(b)==0);
}
static inline void eitvXXX_set_unit_bound(eitvXXX_t a, const boundXXX_t b)
{
  itvXXX_set_unit_bound(a->itv,b);
  a->eq = (boundXXX_sgn(b)==0);
}
static inline void eitvXXX_enlarge_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c)
{
  itvXXX_enlarge_bound(a->itv,b->itv,c);
  a->eq = b->eq && itvXXX_is_point(a->itv);
}

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */

static inline bool eitvXXX_is_point(const eitvXXX_t a)
{
  return a->eq || itvXXX_is_point(a->itv);
}
static inline bool eitvXXX_is_zero(const eitvXXX_t a)
{
  return (boundXXX_sgn(a->itv->sup)==0 &&
	  (a->eq ? true : boundXXX_sgn(a->itv->neginf)==0));
}
static inline bool eitvXXX_is_pos(const eitvXXX_t a)
{ return itvXXX_is_pos(a->itv); }
static inline bool eitvXXX_is_neg(const eitvXXX_t a)
{ return itvXXX_is_neg(a->itv); }
static inline bool eitvXXX_is_top(const eitvXXX_t a)
{
  return !a->eq && itvXXX_is_top(a->itv);
}
static inline bool eitvXXX_is_bottom(const eitvXXX_t a)
{
  return !a->eq && eitvXXX_canonicalize((eitvXXX_ptr)a, false);
}
static inline bool eitvXXX_is_leq(const eitvXXX_t a, const eitvXXX_t b)
{
  return
    (a->eq && b->eq ?
     boundXXX_equal(a->itv->sup,b->itv->sup) :
     itvXXX_is_leq(a->itv,b->itv));
}
static inline bool eitvXXX_is_eq(const eitvXXX_t a, const eitvXXX_t b)
{
  return boundXXX_equal(a->itv->sup,b->itv->sup) &&
    (a->eq && b->eq ?
     true :
     boundXXX_equal(a->itv->neginf,b->itv->neginf));
}

static inline int eitvXXX_hash(const eitvXXX_t a)
{
  return a->eq ? 12*boundXXX_hash(a->itv->sup) : itvXXX_hash(a->itv);
}
static inline int eitvXXX_cmp(const eitvXXX_t a, const eitvXXX_t b)
{
  return a->eq && b->eq ? 2*boundXXX_cmp(a->itv->sup,b->itv->sup) : itvXXX_cmp(a->itv,b->itv);
}
static inline int eitvXXX_cmp_zero(const eitvXXX_t a)
{
  return a->eq ? 2*boundXXX_sgn(a->itv->sup) : eitvXXX_cmp_zero(a);
}
static inline void eitvXXX_range_abs(boundXXX_t a, const eitvXXX_t b)
{ itvXXX_range_abs(a,b->itv); }

static inline void eitvXXX_range_rel(boundXXX_t a, const eitvXXX_t b, num_internal_t intern)
{ itvXXX_range_rel(a,b->itv, intern); }

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */

static inline void eitvXXX_join(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c)
{
  itvXXX_join(a->itv,b->itv,c->itv);
  a->eq = false;
}
static inline void eitvXXX_widening(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c)
{
  itvXXX_widening(a->itv,b->itv,c->itv);
  a->eq = false;
}

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */

static inline void eitvXXX_add(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c)
{
  if (NUMXXX_EXACT && b->eq && c->eq){
    numXXX_add(boundXXX_numref(a->itv->sup),
	       boundXXX_numref(b->itv->sup),
	       boundXXX_numref(c->itv->sup));
    _boundXXX_set_finite(a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_add(a->itv->sup,b->itv->sup,c->itv->sup);
    boundXXX_add(a->itv->neginf,b->itv->neginf,c->itv->neginf);
    a->eq = false;
  }
}
static inline void eitvXXX_add_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c)
{
  boundXXX_add_num(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_sub_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitvXXX_sub_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c)
{
  boundXXX_sub_num(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_add_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
  /*
static inline void eitvXXX_add_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c)
{
  boundXXX_add(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_sub(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitvXXX_sub_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c)
{
  boundXXX_sub(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_add(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
  */
static inline void eitvXXX_magnitude(boundXXX_t a, const eitvXXX_t b)
{
  itvXXX_magnitude(a,b->itv);
}

static inline void eitvXXX_neg(eitvXXX_t a, const eitvXXX_t b)
{
  if (a!=b){
    boundXXX_set(a->itv->neginf,b->itv->sup);
    boundXXX_set(a->itv->sup,b->itv->neginf);
    a->eq = b->eq;
  } else {
    boundXXX_swap(a->itv->neginf,a->itv->sup);
  }
}

static inline bool eitvXXX_sqrt(eitvXXX_t a, const eitvXXX_t b, num_internal_t intern)
{
  bool exact = itvXXX_sqrt(a->itv,b->itv, intern);
  a->eq = exact && itvXXX_is_point(a->itv);
  return exact;
}

static inline void eitvXXX_mod(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c,
			       bool is_int,
			       num_internal_t intern)
{
  itvXXX_mod(a->itv,b->itv,c->itv,is_int, intern);
  a->eq = false;
}

#ifdef __cplusplus
}
#endif

#endif
