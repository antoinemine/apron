/* ********************************************************************** */
/* itv.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITV_H_
#define _ITV_H_

#include <stdio.h>
#include "num.h"
#include "bound.h"
#include "itv_config.h"
#include "ap_coeff.h"

#include "itv_fun.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Be cautious: interval [a,b] is represented by [-a,b].  This is because
   bound quantities are always rounded toward +infty */

typedef struct __itv_struct {
  bound_t inf; /* negation of the inf bound */
  bound_t sup; /* sup bound */
} __itv_struct;
typedef __itv_struct itv_t[1];
typedef __itv_struct* itv_ptr;


/* Workspace to avoid temporary allocation and deallocation when num_t and
   bound_t are multiprecision numbers */

/* some useful local constant for linearization */
typedef struct {
  itv_t ulp;        /* [-1;1] * unit in the least place */
  itv_t min;        /* [-1;1] * minimum positive denormal */
  itv_t min_normal; /* [-1;1] * minimum positive normal */
  itv_t max;        /* [-1;1] * maximum non +oo  */
  itv_t max_exact;  /* [-1;1] * maximum exactly representable integer */
} float_const;

typedef struct itv_internal_t {
  num_t canonicalize_num;
  bound_t muldiv_bound;
  bound_t mul_bound;
  bound_t sqrt_bound;
  bound_t linear_bound;
  bound_t linear_bound2;
  bound_t linear_bound3;
  itv_t mul_itv;
  itv_t mul_itv2;
  ap_scalar_t* ap_conversion_scalar;
  bound_t ap_conversion_bound;
  itv_t eval_itv;
  itv_t eval_itv2;
  itv_t eval_itv3;
  num_t quasi_num;
  itv_t boxize_lincons_itv;
  itv_t boxize_lincons_eval;
  bound_t boxize_lincons_bound;
  float_const cst_half, cst_single, cst_double, cst_extended, cst_quad;
  itv_t itv_half; /* [-0.5,0.5] */
  mpz_t reduce_lincons_gcd;
  mpz_t reduce_lincons_mpz;
} itv_internal_t;


static inline itv_internal_t* itv_internal_alloc(void);
  /* Allocate and initialize internal workspace */
static inline void itv_internal_free(itv_internal_t* intern);
  /* Clear and free internal workspace */

static inline void itv_internal_init(itv_internal_t* intern);
static inline void itv_internal_clear(itv_internal_t* intern);
  /* GMP Semantics */

/* ********************************************************************** */
/* itv */
/* ********************************************************************** */

/* Initialization and clearing */
static inline void itv_init(itv_t a);
static inline void itv_init_array(itv_t* a, size_t size);
static inline void itv_init_set(itv_t a, itv_t b);
static inline void itv_clear(itv_t a);
static inline void itv_clear_array(itv_t* a, size_t size);
static inline itv_t* itv_array_alloc(size_t size);
static inline void itv_array_free(itv_t* a, size_t size);

/* Assignement */
static inline void itv_set(itv_t a, itv_t b);
static inline void itv_set_num(itv_t a, num_t b);
static inline void itv_set_num2(itv_t a, num_t b, num_t c);
static inline void itv_set_int(itv_t a, long int b);
static inline void itv_set_int2(itv_t a, long int b, long int c);
static inline void itv_set_bottom(itv_t a);
static inline void itv_set_top(itv_t a);
static inline void itv_swap(itv_t a, itv_t b);

/* a = [-b,b] */
static inline void itv_set_unit_num(itv_t a, num_t b);
static inline void itv_set_unit_bound(itv_t a, bound_t b);

/* a = b + [-c,c] */
static inline void itv_enlarge_bound(itv_t a, itv_t b, bound_t c);

/* Normalization and tests */
static inline bool itv_canonicalize(itv_internal_t* intern, itv_t a, bool integer);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
static inline bool itv_is_top(itv_t a);
static inline bool itv_is_bottom(itv_internal_t* intern, itv_t a);
  /* Return true iff the interval is resp. [-oo,+oo] or empty */
static inline bool itv_is_canonical_bottom(itv_t a);
  /* Return true iff the interval is the canonical bottom element [1,-1] */
static inline bool itv_is_point(itv_internal_t* intern, itv_t a);
  /* Return true iff the interval is a single point */
static inline bool itv_is_zero(itv_t a);
  /* Return true iff the interval is a single zero point */
static inline bool itv_is_leq(itv_t a, itv_t b);
  /* Inclusion test */
static inline bool itv_is_eq(itv_t a, itv_t b);
  /* Equality test */
static inline int itv_hash(itv_t a);
  /* Hash code */
static inline bool itv_is_pos(itv_t a);
static inline bool itv_is_neg(itv_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */

static inline bool itv_is_int(itv_internal_t* intern, itv_t a);
  /* has integer bounds */

static inline void itv_range_abs(bound_t a, itv_t b);
  /* a=(max b - min b) */
static inline void itv_range_rel(itv_internal_t* intern, bound_t a, itv_t b);
  /* a=(max b - min b) / (|a+b|/2) */

/* Lattice operations */
static inline bool itv_meet(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
  /* Assign a with the intersection of b and c */
static inline void itv_join(itv_t a, itv_t b, itv_t c);
  /* Assign a with the union of b and c */
static inline void itv_widening(itv_t a, itv_t b, itv_t c);
  /* Assign a with the standard interval widening of b by c */

/* Arithmetic operations */
static inline void itv_add(itv_t a, itv_t b, itv_t c);
static inline void itv_sub(itv_t a, itv_t b, itv_t c);
static inline void itv_neg(itv_t a, itv_t b);
static inline void itv_mul(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
static inline void itv_div(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
static inline void itv_add_num(itv_t a, itv_t b, num_t c);
static inline void itv_sub_num(itv_t a, itv_t b, num_t c);
static inline void itv_mul_num(itv_t a, itv_t b, num_t c);
static inline void itv_div_num(itv_t a, itv_t b, num_t c);
static inline void itv_add_bound(itv_t a, itv_t b, bound_t c);
static inline void itv_sub_bound(itv_t a, itv_t b, bound_t c);
static inline void itv_mul_bound(itv_t a, itv_t b, bound_t c);
static inline void itv_div_bound(itv_t a, itv_t b, bound_t c);
static inline bool itv_sqrt(itv_internal_t* intern, itv_t a, itv_t b);
static inline void itv_abs(itv_t a, itv_t b);
static inline void itv_mul_2exp(itv_t a, itv_t b, int c);

static inline void itv_magnitude(bound_t a, itv_t b);
  /* get the absolute value of maximal bound */

static inline void itv_mod(itv_internal_t* intern, itv_t a, itv_t b, itv_t c, bool is_int);
  /* x mod y = x - y*trunc(x/y) */

/* Integer casts (rounding towards +oo, -oo, 0, or worst-case) */
static inline void itv_ceil(itv_t a, itv_t b);
static inline void itv_floor(itv_t a, itv_t b);
static inline void itv_trunc(itv_t a, itv_t b);
static inline void itv_to_int(itv_t a, itv_t b);

/* Floating-point casts (worst cases) */
static inline void itv_to_float(itv_t a, itv_t b);
static inline void itv_to_double(itv_t a, itv_t b);

  /* Inverse of the above functions */
static inline void itv_unceil(itv_t a, itv_t b);
static inline void itv_unfloor(itv_t a, itv_t b);
static inline void itv_untrunc(itv_t a, itv_t b);
static inline void itv_from_int(itv_t a, itv_t b);
static inline void itv_from_float(itv_t a, itv_t b);
static inline void itv_from_double(itv_t a, itv_t b);

/* Printing */
static inline int itv_snprint(char* s, size_t size, itv_t a);
static inline void itv_fprint(FILE* stream, itv_t a);
static inline void itv_print(itv_t a);

/* All these functions return true if the conversion is exact */
static inline bool itv_set_ap_scalar(itv_internal_t* intern, itv_t a, ap_scalar_t* b);
  /* Convert a ap_scalar_t into a itv_t.
     Assumes the scalar is finite.
     If it returns true, the interval is a single point */
static inline bool itv_set_ap_interval(itv_internal_t* intern, itv_t a, ap_interval_t* b);
  /* Convert a ap_interval_t into a itv_t */
static inline bool itv_set_ap_coeff(itv_internal_t* intern, itv_t a, ap_coeff_t* b);
  /* Convert a ap_coeff_t into a itv_t. */

static inline bool ap_interval_set_itv(itv_internal_t* intern, ap_interval_t* a, itv_t b);
  /* Convert a itv_t into a ap_interval_t */

static inline bool ap_coeff_set_itv(itv_internal_t* intern, ap_coeff_t* a, itv_t b);
  /* Convert a itv_t into a ap_coeff_t */

static inline bool itv_array_set_ap_interval_array(itv_internal_t* intern, itv_t** ptitv, ap_interval_t** array, size_t size);
  /* Convert an array of ap_interval_t into an array of itv_t.
     The paramater ptitv is a result parameter.
     The result is to be found in *ptitv */

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

void ITVFUN(itv_internal_init)(itv_internal_t* intern);
void ITVFUN(itv_internal_clear)(itv_internal_t* intern);
itv_internal_t* ITVFUN(itv_internal_alloc)(void);
void ITVFUN(itv_internal_free)(itv_internal_t* intern);
bool ITVFUN(itv_canonicalize)(itv_internal_t* intern, itv_t a, bool integer);
void ITVFUN(itv_mul_num)(itv_t a, itv_t b, num_t c);
void ITVFUN(itv_div_num)(itv_t a, itv_t b, num_t c);
void ITVFUN(itv_div_num)(itv_t a, itv_t b, num_t c);
void ITVFUN(itv_mul_bound)(itv_t a, itv_t b, bound_t c);
void ITVFUN(itv_div_bound)(itv_t a, itv_t b, bound_t c);
void ITVFUN(itv_sub)(itv_t a, itv_t b, itv_t c);
void ITVFUN(itv_neg)(itv_t a, itv_t b);
void ITVFUN(itv_mul)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
void ITVFUN(itv_div)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
bool ITVFUN(itv_sqrt)(itv_internal_t* intern, itv_t a, itv_t b);
void ITVFUN(itv_abs)(itv_t a, itv_t b);
void ITVFUN(itv_mod)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c,  bool is_int);
void ITVFUN(itv_fprint)(FILE* stream, itv_t a);
void ITVFUN(itv_print)(itv_t a);
int  ITVFUN(itv_snprint)(char* s, size_t size, itv_t a);

bool ITVFUN(itv_set_ap_scalar)(itv_internal_t* intern, itv_t a, ap_scalar_t* b);
bool ITVFUN(itv_set_ap_interval)(itv_internal_t* intern, itv_t a, ap_interval_t* b);
bool ITVFUN(itv_set_ap_coeff)(itv_internal_t* intern, itv_t itv, ap_coeff_t* coeff);
bool ITVFUN(ap_interval_set_itv)(itv_internal_t* intern,  ap_interval_t* a, itv_t b);
bool ITVFUN(ap_coeff_set_itv)(itv_internal_t* intern, ap_coeff_t* a, itv_t b);
bool ITVFUN(itv_array_set_ap_interval_array)(itv_internal_t* intern, itv_t** ptitv, ap_interval_t** array, size_t size);
  
/** The power operator */
void ITVFUN(itv_pow)(itv_internal_t* intern, itv_t a, itv_t b, itv_t n);
static inline void itv_pow(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{ ITVFUN(itv_pow)(intern,a,b,c); }
void ITVFUN(itv_inv_pow)(itv_internal_t* intern, itv_t a, itv_t orga, itv_t b, itv_t n);
static inline void itv_inv_pow(itv_internal_t* intern, itv_t a, itv_t orga, itv_t b, itv_t c)
{ ITVFUN(itv_inv_pow)(intern,a,orga,b,c); }


static inline void itv_internal_init(itv_internal_t* intern)
{ ITVFUN(itv_internal_init)(intern); }

static inline void itv_internal_clear(itv_internal_t* intern)
{ ITVFUN(itv_internal_clear)(intern); }

static inline itv_internal_t* itv_internal_alloc(void)
{ return ITVFUN(itv_internal_alloc)(); }

static inline void itv_internal_free(itv_internal_t* intern)
{ ITVFUN(itv_internal_free)(intern); }

static inline bool itv_canonicalize(itv_internal_t* intern,
 itv_t a, bool integer)
{ return ITVFUN(itv_canonicalize)(intern,a,integer); }

static inline void itv_mul_num(itv_t a, itv_t b, num_t c)
{ ITVFUN(itv_mul_num)(a,b,c); }

static inline void itv_div_num(itv_t a, itv_t b, num_t c)
{ ITVFUN(itv_div_num)(a,b,c); }

static inline void itv_mul_bound(itv_t a, itv_t b, bound_t c)
{ ITVFUN(itv_mul_bound)(a,b,c); }

static inline void itv_div_bound(itv_t a, itv_t b, bound_t c)
{ ITVFUN(itv_div_bound)(a,b,c); }

static inline void itv_sub(itv_t a, itv_t b, itv_t c)
{ ITVFUN(itv_sub)(a,b,c); }

static inline void itv_neg(itv_t a, itv_t b)
{ ITVFUN(itv_neg)(a,b); }

static inline void itv_mul(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{ ITVFUN(itv_mul)(intern,a,b,c); }

static inline void itv_div(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{ ITVFUN(itv_div)(intern,a,b,c); }

static inline void itv_fprint(FILE* stream, itv_t a)
{ ITVFUN(itv_fprint)(stream,a); }
static inline void itv_print(itv_t itv)
{ ITVFUN(itv_print)(itv); }

static inline int itv_snprint(char* s, size_t size, itv_t a)
{ return ITVFUN(itv_snprint)(s,size,a); }

static inline bool itv_set_ap_scalar(itv_internal_t* intern, itv_t a, ap_scalar_t* b)
{ return ITVFUN(itv_set_ap_scalar)(intern,a,b); }

static inline bool itv_set_ap_interval(itv_internal_t* intern, itv_t a, ap_interval_t* b)
{ return ITVFUN(itv_set_ap_interval)(intern,a,b); }

static inline bool itv_set_ap_coeff(itv_internal_t* intern, itv_t itv, ap_coeff_t* coeff)
{ return ITVFUN(itv_set_ap_coeff)(intern,itv,coeff); }

static inline bool ap_interval_set_itv(itv_internal_t* intern, ap_interval_t* a, itv_t b)
{ return ITVFUN(ap_interval_set_itv)(intern,a,b); }

static inline bool ap_coeff_set_itv(itv_internal_t* intern, ap_coeff_t* a, itv_t b)
{ return ITVFUN(ap_coeff_set_itv)(intern,a,b); }

static inline bool itv_array_set_ap_interval_array(itv_internal_t* intern, itv_t** ptitv, ap_interval_t** array, size_t size)
{ return ITVFUN(itv_array_set_ap_interval_array)(intern,ptitv,array,size); }

static inline void itv_init(itv_t a)
{
  bound_init(a->inf);
  bound_init(a->sup);
}
static inline void itv_init_array(itv_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) itv_init(a[i]);
}
static inline void itv_init_set(itv_t a, itv_t b)
{
  bound_init_set(a->inf,b->inf);
  bound_init_set(a->sup,b->sup);
}
static inline void itv_clear(itv_t a)
{
  bound_clear(a->inf);
  bound_clear(a->sup);
}
static inline void itv_clear_array(itv_t* a, size_t size)
{
  (void)a;
  (void)size;
#if !defined(NUM_NATIVE)
  size_t i;
  for (i=0; i<size; i++) itv_clear(a[i]);
#endif
}
static inline itv_t* itv_array_alloc(size_t size)
{
  itv_t* res = (itv_t*)malloc(size*sizeof(itv_t));
  itv_init_array(res,size);
  return res;
}
static inline void itv_array_free(itv_t* a, size_t size)
{
  itv_clear_array(a,size);
  free(a);
}

static inline void itv_set(itv_t a, itv_t b)
{
  bound_set(a->inf,b->inf);
  bound_set(a->sup,b->sup);
}
static inline void itv_set_num(itv_t a, num_t b)
{
  bound_set_num(a->sup,b);
  bound_neg(a->inf,a->sup);
}
static inline void itv_set_num2(itv_t a, num_t b, num_t c)
{
  num_neg(b,b);
  bound_set_num(a->inf,b);
  num_neg(b,b);
  bound_set_num(a->sup,c);
}
static inline void itv_set_unit_num(itv_t a, num_t b)
{
  bound_set_num(a->inf,b);
  bound_set_num(a->sup,b);
}
static inline void itv_set_unit_bound(itv_t a, bound_t b)
{
  bound_set(a->inf,b);
  bound_set(a->sup,b);
}
static inline void itv_enlarge_bound(itv_t a, itv_t b, bound_t c)
{
  bound_add(a->inf,b->inf,c);
  bound_add(a->sup,b->sup,c);
}
static inline void itv_set_int(itv_t a, long int b)
{
  bound_set_int(a->inf,-b);
  bound_set_int(a->sup,b);
}
static inline void itv_set_int2(itv_t a, long int b, long int c)
{
  bound_set_int(a->inf,-b);
  bound_set_int(a->sup,c);
}
static inline void itv_set_bottom(itv_t a)
{
  bound_set_int(a->inf,-1);
  bound_set_int(a->sup,-1);
}
static inline void itv_set_top(itv_t a)
{
  bound_set_infty(a->inf,1);
  bound_set_infty(a->sup,1);
}
static inline void itv_swap(itv_t a, itv_t b)
{ itv_t t; *t=*a;*a=*b;*b=*t; }

static inline bool itv_is_top(itv_t a)
{
  return bound_infty(a->inf) && bound_infty(a->sup);
}
static inline bool itv_is_bottom(itv_internal_t* intern, itv_t a)
{
  return itv_canonicalize(intern, a, false);
}
static inline bool itv_is_canonical_bottom(itv_t a)
{
  return !bound_cmp_int(a->inf,-1) && !bound_cmp_int(a->sup,-1);
}
static inline bool itv_is_point(itv_internal_t* intern, itv_t a)
{
  if (!bound_infty(a->inf) && !bound_infty(a->sup)){
    num_neg(intern->canonicalize_num,bound_numref(a->inf));
    return num_equal(intern->canonicalize_num,bound_numref(a->sup));
  }
  else
    return false;
}
static inline bool itv_is_zero(itv_t a)
{
  return bound_sgn(a->inf)==0 && bound_sgn(a->sup)==0;
}
static inline bool itv_is_leq(itv_t a, itv_t b)
{
  if (itv_is_canonical_bottom(a)) {
    return true;
  }
  else if (itv_is_canonical_bottom(b)) {
    return false;
  }
  else {
    return bound_cmp(a->sup,b->sup)<=0 && bound_cmp(a->inf,b->inf)<=0;
  }
}
static inline bool itv_is_eq(itv_t a, itv_t b)
{
  return bound_equal(a->sup,b->sup) && bound_equal(a->inf,b->inf);
}
static inline int itv_hash(itv_t a)
{
  return (5*bound_hash(a->inf) + 7*bound_hash(a->sup));
}
static inline bool itv_meet(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  bound_min(a->sup,b->sup,c->sup);
  bound_min(a->inf,b->inf,c->inf);
  return itv_canonicalize(intern,a,false);
}
static inline void itv_join(itv_t a, itv_t b, itv_t c)
{
  if (itv_is_canonical_bottom(b)) {
    itv_set(a,c);
  }
  else if (itv_is_canonical_bottom(c)) {
    itv_set(a,b);
  }
  else {
    bound_max(a->sup,b->sup,c->sup);
    bound_max(a->inf,b->inf,c->inf);
  }
}
static inline void bound_widening(bound_t a, bound_t b, bound_t c)
{
  if (bound_infty(c) ||
      bound_cmp(b,c)<0){
    bound_set_infty(a,+1);
  } else {
    bound_set(a,b);
  }
}
static inline void itv_widening(itv_t a, itv_t b, itv_t c)
{
  if (itv_is_canonical_bottom(b)) {
    itv_set(a,c);
  }
  else if (itv_is_canonical_bottom(c)) {
    itv_set(a,b);
  }
  else {
    bound_widening(a->sup,b->sup,c->sup);
    bound_widening(a->inf,b->inf,c->inf);
  }
}
static inline void itv_add(itv_t a, itv_t b, itv_t c)
{
  bound_add(a->sup,b->sup,c->sup);
  bound_add(a->inf,b->inf,c->inf);
}
static inline void itv_add_num(itv_t a, itv_t b, num_t c)
{
  bound_add_num(a->sup,b->sup,c);
  bound_sub_num(a->inf,b->inf,c);
}
static inline void itv_sub_num(itv_t a, itv_t b, num_t c)
{
  bound_sub_num(a->sup,b->sup,c);
  bound_add_num(a->inf,b->inf,c);
}
static inline void itv_add_bound(itv_t a, itv_t b, bound_t c)
{
  bound_add(a->sup,b->sup,c);
  bound_sub(a->inf,b->inf,c);
}
static inline void itv_sub_bound(itv_t a, itv_t b, bound_t c)
{
  bound_sub(a->sup,b->sup,c);
  bound_add(a->inf,b->inf,c);
}
static inline bool itv_sqrt(itv_internal_t* intern, itv_t a, itv_t b)
{ return ITVFUN(itv_sqrt)(intern,a,b); }

static inline void itv_abs(itv_t a, itv_t b)
{ ITVFUN(itv_abs)(a,b); }

static inline void itv_mod(itv_internal_t* intern, itv_t a, itv_t b, itv_t c, bool is_int)
{ ITVFUN(itv_mod)(intern,a,b,c,is_int); }

static inline void itv_ceil(itv_t a, itv_t b)
{ bound_ceil(a->sup,b->sup); bound_floor(a->inf,b->inf); }

static inline void itv_floor(itv_t a, itv_t b)
{ bound_floor(a->sup,b->sup);  bound_ceil(a->inf,b->inf); }

static inline void itv_trunc(itv_t a, itv_t b)
{ bound_trunc(a->sup,b->sup); bound_trunc(a->inf,b->inf); }

static inline void itv_to_int(itv_t a, itv_t b)
{ bound_ceil(a->sup,b->sup); bound_ceil(a->inf,b->inf); }

static inline void itv_to_float(itv_t a, itv_t b)
{ bound_to_float(a->sup,b->sup); bound_to_float(a->inf,b->inf); }

static inline void itv_to_double(itv_t a, itv_t b)
{ bound_to_double(a->sup,b->sup); bound_to_double(a->inf,b->inf); }

static inline void itv_mul_2exp(itv_t a, itv_t b, int c)
{ bound_mul_2exp(a->sup,b->sup,c); bound_mul_2exp(a->inf,b->inf,c); }

static inline bool itv_is_pos(itv_t a)
{ return (bound_sgn(a->inf)<=0); }

static inline bool itv_is_neg(itv_t a)
{ return (bound_sgn(a->sup)<=0); }

static inline void itv_magnitude(bound_t a, itv_t b)
{
  if (bound_sgn(b->inf)<=0) bound_set(a,b->sup);
  else if (bound_sgn(b->sup)<=0) bound_set(a,b->inf);
  else bound_max(a,b->inf,b->sup);
}

static inline void itv_range_abs(bound_t a, itv_t b)
{ bound_add(a,b->sup,b->inf); }

static inline void itv_range_rel(itv_internal_t* intern, bound_t a, itv_t b)
{
  bound_add(a,b->sup,b->inf);
  if (!bound_infty(a)) {
    itv_magnitude(intern->muldiv_bound,b);
    bound_div_2(intern->muldiv_bound,intern->muldiv_bound);
    bound_div(a,a,intern->muldiv_bound);
  }
}

static inline bool itv_is_int(itv_internal_t* intern, itv_t a)
{
  bound_trunc(intern->muldiv_bound,a->sup);
  if (bound_cmp(intern->muldiv_bound,a->sup)) return false;
  bound_trunc(intern->muldiv_bound,a->inf);
  return !bound_cmp(intern->muldiv_bound,a->inf);
}



static inline void itv_unceil(itv_t a, itv_t b)
{
  /* [a,b] ->  [ceil(a)-1,floor(b)] */
  bound_floor(a->sup,b->sup);
  bound_floor(a->inf,b->inf);
  bound_add_uint(a->inf,a->inf,1);
}

static inline void itv_unfloor(itv_t a, itv_t b)
{
  /* [a,b] -> [ceil(a),floor(b)+1] */
  bound_floor(a->inf,b->inf);
  bound_floor(a->sup,b->sup);
  bound_add_uint(a->sup,a->sup,1);
}

static inline void itv_untrunc(itv_t a, itv_t b)
{
  /* trunc(x) = ceil(x) if x < 0, floor(x) if x > 0 */
  bound_floor(a->inf,b->inf);
  bound_floor(a->sup,b->sup);
  if (bound_sgn(a->inf) > 0) bound_add_uint(a->inf,a->inf,1);
  if (bound_sgn(a->sup) > 0) bound_add_uint(a->sup,a->sup,1);
}

static inline void itv_from_int(itv_t a, itv_t b)
{
  /* [a,b] -> [floor(a),ceil(b)] */
  bound_ceil(a->sup,b->sup);
  bound_ceil(a->inf,b->inf);
}

static inline void itv_from_float(itv_t a, itv_t b)
{
  /* special case to ensure that signs are respected */
  if (bound_sgn(b->sup)==0) bound_set(a->sup,b->sup);
  else bound_next_float(a->sup,b->sup);
  if (bound_sgn(b->inf)==0) bound_set(a->inf,b->inf);
  else bound_next_float(a->inf,b->inf);
}

static inline void itv_from_double(itv_t a, itv_t b)
{
  if (bound_sgn(b->sup)==0) bound_set(a->sup,b->sup);
  else bound_next_double(a->sup,b->sup);
  if (bound_sgn(b->inf)==0) bound_set(a->inf,b->inf);
  else bound_next_double(a->inf,b->inf);
}


#ifdef __cplusplus
}
#endif

#endif
