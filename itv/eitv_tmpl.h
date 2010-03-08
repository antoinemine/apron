/* ********************************************************************** */
/* itv.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITV_H_
#define _ITV_H_

#include <stdio.h>
#include "itv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __eitv_struct {
  itv_t itv;
  bool eq;
} __eitv_struct;
typedef __eitv_struct eitv_t[1];
typedef __eitv_struct* eitv_ptr;

/* ********************************************************************** */
/* eitv */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */
static inline void eitv_init(eitv_t a);
static inline void eitv_init_array(eitv_t* a, size_t size);
static inline void eitv_init_set(eitv_t a, eitv_t b);
static inline void eitv_clear(eitv_t a);
static inline void eitv_clear_array(eitv_t* a, size_t size);
static inline eitv_t* eitv_array_alloc(size_t size);
static inline void eitv_array_free(eitv_t* a, size_t size);

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void eitv_set(eitv_t a, eitv_t b);
static inline void eitv_set_num(eitv_t a, num_t b);
static inline void eitv_set_num2(eitv_t a, num_t b, num_t c);
static inline void eitv_set_int(eitv_t a, long int b);
static inline void eitv_set_int2(eitv_t a, long int b, long int c);
static inline void eitv_set_bottom(eitv_t a);
static inline void eitv_set_top(eitv_t a);
static inline void eitv_swap(eitv_t a, eitv_t b);

/* a = [-b,b] */
static inline void eitv_set_unit_num(eitv_t a, num_t b);
static inline void eitv_set_unit_bound(eitv_t a, bound_t b);

/* a = b + [-c,c] */
static inline void eitv_enlarge_bound(eitv_t a, eitv_t b, bound_t c);

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */
bool eitv_canonicalize(eitv_internal_t* intern, eitv_t a, bool integer);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
static inline bool eitv_is_int(eitv_internal_t* intern, eitv_t a);
  /* has integer bounds */
static inline bool eitv_is_point(eitv_internal_t* intern, eitv_t a);
  /* Return true iff the interval is a single point */
static inline bool eitv_is_zero(eitv_t a);
  /* Return true iff the interval is a single zero point */
static inline bool eitv_is_pos(eitv_t a);
static inline bool eitv_is_neg(eitv_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
static inline bool eitv_is_top(eitv_t a);
static inline bool eitv_is_bottom(eitv_internal_t* intern, eitv_t a);
  /* Return true iff the interval is resp. [-oo,+oo] or empty */
static inline bool eitv_is_leq(eitv_t a, eitv_t b);
  /* Inclusion test */
static inline bool eitv_is_eq(eitv_t a, eitv_t b);
  /* Equality test */

static inline int eitv_hash(eitv_t a);
  /* Hash code */
static inline void eitv_range_abs(bound_t a, eitv_t b);
  /* a=(max b - min b) */
static inline void eitv_range_rel(eitv_internal_t* intern, bound_t a, eitv_t b);
  /* a=(max b - min b) / (|a+b|/2) */

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */
static inline bool eitv_meet(eitv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c);
  /* Assign a with the intersection of b and c */
static inline void eitv_join(eitv_t a, eitv_t b, eitv_t c);
  /* Assign a with the union of b and c */
static inline void eitv_widening(eitv_t a, eitv_t b, eitv_t c);
  /* Assign a with the standard interval widening of b by c */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */
static inline void eitv_add(eitv_t a, eitv_t b, eitv_t c);
void eitv_sub(eitv_t a, eitv_t b, eitv_t c);
void eitv_neg(eitv_t a, eitv_t b);
void eitv_mul(eitv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c);
void eitv_div(eitv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c);
static inline void eitv_add_num(eitv_t a, eitv_t b, num_t c);
static inline void eitv_sub_num(eitv_t a, eitv_t b, num_t c);
void eitv_mul_num(eitv_t a, eitv_t b, num_t c);
void eitv_div_num(eitv_t a, eitv_t b, num_t c);
static inline void eitv_add_bound(eitv_t a, eitv_t b, bound_t c);
static inline void eitv_sub_bound(eitv_t a, eitv_t b, bound_t c);
void eitv_mul_bound(eitv_t a, eitv_t b, bound_t c);
void eitv_div_bound(eitv_t a, eitv_t b, bound_t c);
static inline bool eitv_sqrt(eitv_internal_t* intern, eitv_t a, eitv_t b);
void eitv_abs(eitv_t a, eitv_t b);
static inline void eitv_mul_2exp(eitv_t a, eitv_t b, int c);

static inline void eitv_magnitude(bound_t a, eitv_t b);
  /* get the absolute value of maximal bound */

static inline void eitv_mod(eitv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c, bool is_int);
  /* x mod y = x - y*trunc(x/y) */

/* ====================================================================== */
/* Casts */
/* ====================================================================== */
/* Integer casts (rounding towards +oo, -oo, 0, or worst-case) */
static inline void eitv_ceil(eitv_t a, eitv_t b);
static inline void eitv_floor(eitv_t a, eitv_t b);
static inline void eitv_trunc(eitv_t a, eitv_t b);
static inline void eitv_to_int(eitv_t a, eitv_t b);

/* Floating-point casts (worst cases) */
static inline void eitv_to_float(eitv_t a, eitv_t b);
static inline void eitv_to_double(eitv_t a, eitv_t b);


/* ====================================================================== */
/* Printing */
/* ====================================================================== */
static inline int eitv_snprint(char* s, size_t size, eitv_t a);
static inline void eitv_fprint(FILE* stream, eitv_t a);
static inline void eitv_print(eitv_t a);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */

static inline void eitv_init(eitv_t a)
{
  itv_init(a->itv);
  a->eq=true;
}
static inline void eitv_init_array(eitv_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) eitv_init(a[i]);
}
static inline void eitv_init_set(eitv_t a, eitv_t b)
{
  itv_init_set(a->itv,b->itv);
  a->eq = b->eq;
}
static inline void eitv_clear(eitv_t a)
{ itv_clear(a->itv); }
static inline void eitv_clear_array(eitv_t* a, size_t size)
{
#if !defined(NUM_NATIVE)
  size_t i;
  for (i=0; i<size; i++) eitv_clear(a[i]);
#endif
}
static inline eitv_t* eitv_array_alloc(size_t size)
{
  eitv_t* res = (eitv_t*)malloc(size*sizeof(eitv_t));
  eitv_init_array(res,size);
  return res;
}
static inline void eitv_array_free(eitv_t* a, size_t size)
{
  eitv_clear_array(a,size);
  free(a);
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void eitv_set(eitv_t a, eitv_t b)
{
  bound_set(a->itv->neginf,b->itv->neginf);
  bound_set(a->itv->sup,b->itv->sup);
  a->eq = b->eq;
}
static inline void eitv_set_num(eitv_t a, num_t b)
{
  itv_set_num(a->itv,b);
  a->eq = true;
}
static inline void eitv_set_num2(eitv_t a, num_t b, num_t c)
{
  itv_set_num2(a->itv,b,c);
  a->eq = false;
}
static inline void eitv_set_int(eitv_t a, long int b)
{
  bound_set_int(a->itv->sup,b);
  a->eq = true;
}
static inline void eitv_set_int2(eitv_t a, long int b, long int c)
{
  itv_set_int2(a->itv,b,c);
  a->eq = (b == -c);
}
static inline void eitv_set_bottom(eitv_t a)
{
  itv_set_bottom(a->itv);
  a->eq = false;
}
static inline void eitv_set_top(eitv_t a)
{
  itv_set_top(a->itv);
  a->eq = false;
}
static inline void eitv_swap(eitv_t a, eitv_t b)
{ eitv_t t; *t=*a;*a=*b;*b=*t; }

static inline void eitv_set_unit_num(eitv_t a, num_t b)
{
  itv_set_unit_num(a->itv,b);
  a->eq = false;
}
static inline void eitv_set_unit_bound(eitv_t a, bound_t b)
{
  itv_set_unit_bound(a->itv,b);
  a->eq = false;
}
static inline void eitv_enlarge_bound(eitv_t a, eitv_t b, bound_t c)
{
  itv_enlarge_bound(a->itv,b->itv,c);
  a->eq = false;
}

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */

static inline bool eitv_is_int(eitv_internal_t* intern, eitv_t a)
{
  bound_trunc(intern->muldiv_bound,a->itv->sup);
  if (bound_cmp(intern->muldiv_bound,a->itv->sup)) return false;
  if (a->eq)
    return true;
  else {
    bound_trunc(intern->muldiv_bound,a->itv->neginf);
    return !bound_cmp(intern->muldiv_bound,a->itv->neginf);
  }
}
static inline bool eitv_is_point(eitv_internal_t* intern, eitv_t a)
{
  return a->eq || itv_is_point(intern,a->itv);
}
static inline bool eitv_is_zero(eitv_t a)
{
  return (bound_sgn(a->itv->sup)==0 &&
	  (a->eq ? true : bound_sgn(a->itv->neginf)==0));
}
static inline bool eitv_is_pos(eitv_t a)
{ return itv_is_pos(a->itv); }
static inline bool eitv_is_neg(eitv_t a)
{ return itv_is_neg(a->itv); }
static inline bool eitv_is_top(eitv_t a)
{
  return !a->eq && itv_is_top(a->itv);
}
static inline bool eitv_is_bottom(eitv_internal_t* intern, eitv_t a)
{
  return !a->eq && eitv_canonicalize(intern, a, false);
}
static inline bool eitv_is_leq(eitv_t a, eitv_t b)
{
  return
    (a->eq && b->eq ?
     bound_equal(a->itv->sup,b->itv->sup) :
     itv_is_leq(a->itv,b->itv));
}
static inline bool eitv_is_eq(eitv_internal_t* intern, eitv_t a, eitv_t b)
{
  return bound_equal(a->itv->sup,b->itv->sup) &&
    (a->eq && b->eq ?
     true :
     bound_equal(a->itv->neginf,b->itv->neginf));
}

static inline int eitv_hash(eitv_t a)
{
  return a->eq ?
    12*bound_hash(a->itv->sup) :
    5*bound_hash(a->itv->neginf) + 7*bound_hash(a->itv->sup);
}
static inline void eitv_range_abs(bound_t a, eitv_t b)
{ itv_range_abs(a,b->itv); }

static inline void eitv_range_rel(eitv_internal_t* intern, bound_t a, eitv_t b)
{ itv_range_rel(intern,a,b->itv); }

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */

static inline bool eitv_meet(eitv_internal_t* intern, eitv_t a, eitv_t b, eitv_t c)
{
  bound_min(a->itv->sup,b->itv->sup,c->itv->sup);
  bound_min(a->itv->neginf,b->itv->neginf,c->itv->neginf);
  return eitv_canonicalize(intern,a,false);
}
static inline void eitv_join(eitv_t a, eitv_t b, eitv_t c)
{
  itv_join(a->itv,b->itv,c->itv);
  a->eq = false;
}
static inline void eitv_widening(eitv_t a, eitv_t b, eitv_t c)
{
  itv_widening(a->itv,b->itv,c->itv);
  a->eq = false;
}

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */

static inline void eitv_add(eitv_t a, eitv_t b, eitv_t c)
{
  bound_add(a->itv->sup,b->itv->sup,c->itv->sup);
  if (NUM_EXACT && b->eq && c->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_add(a->itv->neginf,b->itv->neginf,c->itv->neginf);
    a->eq = false;
  }
}
static inline void eitv_add_num(eitv_t a, eitv_t b, num_t c)
{
  bound_add_num(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_sub_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitv_sub_num(eitv_t a, eitv_t b, num_t c)
{
  bound_sub_num(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_add_num(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitv_add_bound(eitv_t a, eitv_t b, bound_t c)
{
  bound_add(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_sub(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitv_sub_bound(eitv_t a, eitv_t b, bound_t c)
{
  bound_sub(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_add(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}
static inline void eitv_mul_2exp(eitv_t a, eitv_t b, int c)
{
  bound_mul_2exp(a->itv->sup,b->itv->sup,c);
  if (NUM_EXACT && b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_mul_2exp(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}

static inline void eitv_magnitude(bound_t a, eitv_t b)
{
  itv_magnitude(a,b->itv);
}

static inline void eitv_neg(eitv_t a, eitv_t b)
{
  if (a!=b){
    bound_set(a->itv->neginf,b->itv->sup);
    bound_set(a->itv->sup,b->itv->neginf);
  } else {
    bound_swap(a->itv->neginf,a->itv->sup);
  }
  a->eq = b->eq;
}

static inline bool eitv_sqrt(itv_internal_t* intern, eitv_t a, eitv_t b)
{
  bool exact = itv_sqrt(intern,a->itv,b->itv);
  a->eq = NUM_EXACT && b->eq && exact;
  return exact;
}

static inline void eitv_mod(itv_internal_t* intern, 
			    eitv_t a, eitv_t b, eitv_t c,
			    bool is_int)
{
  itv_mod(intern,a->itv,b->itv,c->itv,is_int);
  a->eq = false;
}
/* ====================================================================== */
/* Casts */
/* ====================================================================== */

static inline void eitv_ceil(eitv_t a, eitv_t b)
{
  bound_ceil(a->itv->sup,b->itv->sup); 
  if (b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_floor(a->itv->neginf,b->itv->sup);
    a->eq = false;
  }
}

static inline void eitv_floor(eitv_t a, eitv_t b)
{
  bound_floor(a->itv->sup,b->itv->sup); 
  if (b->eq){
    bound_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    bound_ceil(a->itv->neginf,b->itv->sup);
    a->eq = false;
  }
}

static inline void eitv_trunc(eitv_t a, eitv_t b)
{
  itv_trunc(a->itv,b->itv);
  a->eq = false;
}

static inline void eitv_to_int(eitv_t a, eitv_t b)
{
  itv_to_int(a->itv,b->itv);
  a->eq = false;
}

static inline void eitv_to_float(eitv_t a, eitv_t b)
{
  itv_to_float(a->itv,b->itv);
  a->eq = false;
}
static inline void eitv_to_double(eitv_t a, eitv_t b)
{
  itv_to_double(a->itv,b->itv);
  a->eq = false;
}
/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void eitv_fprint(FILE* stream, eitv_t a)
{ 
  if (a->eq)
    bound_fprint(stream,a->itv->sup);
  else
    itv_fprint(stream,a->itv);
 }
static inline void eitv_print(eitv_t a)
{ eitv_fprint(stream, a); }
static inline int eitv_snprint(char* s, size_t size, eitv_t a)
{ 
  return a->eq ? 
    bound_snprint(s,size,a->itv->sup) :
    itv_snprintf(s,size,a->itv); 
}

#ifdef __cplusplus
}
#endif

#endif
