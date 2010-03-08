/* ********************************************************************** */
/* itv.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITV_H_
#define _ITV_H_

#include <stdio.h>
#include "bound.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Be cautious: interval [a,b] is represented by [-a,b].  This is because
   bound quantities are always rounded toward +infty */

typedef struct __itv_struct {
  bound_t neginf; /* negation of the inf bound */
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

/* ********************************************************************** */
/* itv_internal */
/* ********************************************************************** */

itv_internal_t* itv_internal_alloc(void);
  /* Allocate and initialize internal workspace */
void itv_internal_free(itv_internal_t* intern);
  /* Clear and free internal workspace */

void itv_internal_init(itv_internal_t* intern);
void itv_internal_clear(itv_internal_t* intern);
  /* GMP Semantics */

/* ********************************************************************** */
/* itv */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */
static inline void itv_init(itv_t a);
static inline void itv_init_array(itv_t* a, size_t size);
static inline void itv_init_set(itv_t a, itv_t b);
static inline void itv_clear(itv_t a);
static inline void itv_clear_array(itv_t* a, size_t size);
static inline itv_t* itv_array_alloc(size_t size);
static inline void itv_array_free(itv_t* a, size_t size);

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
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

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */
bool itv_canonicalize(itv_internal_t* intern, itv_t a, bool integer);
  /* Canonicalize an interval:
     - if integer is true, narrows bound to integers
     - return true if the interval is bottom
     - return false otherwise
  */
static inline bool itv_is_int(itv_internal_t* intern, itv_t a);
  /* has integer bounds */
static inline bool itv_is_point(itv_internal_t* intern, itv_t a);
  /* Return true iff the interval is a single point */
static inline bool itv_is_zero(itv_t a);
  /* Return true iff the interval is a single zero point */
static inline bool itv_is_pos(itv_t a);
static inline bool itv_is_neg(itv_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
static inline bool itv_is_top(itv_t a);
static inline bool itv_is_bottom(itv_internal_t* intern, itv_t a);
  /* Return true iff the interval is resp. [-oo,+oo] or empty */
static inline bool itv_is_leq(itv_t a, itv_t b);
  /* Inclusion test */
static inline bool itv_is_eq(itv_t a, itv_t b);
  /* Equality test */

static inline int itv_hash(itv_t a);
  /* Hash code */
static inline void itv_range_abs(bound_t a, itv_t b);
  /* a=(max b - min b) */
static inline void itv_range_rel(itv_internal_t* intern, bound_t a, itv_t b);
  /* a=(max b - min b) / (|a+b|/2) */ 

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */
static inline bool itv_meet(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
  /* Assign a with the intersection of b and c */
static inline void itv_join(itv_t a, itv_t b, itv_t c);
  /* Assign a with the union of b and c */
static inline void itv_widening(itv_t a, itv_t b, itv_t c);
  /* Assign a with the standard interval widening of b by c */

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */
static inline void itv_add(itv_t a, itv_t b, itv_t c);
void itv_sub(itv_t a, itv_t b, itv_t c);
void itv_neg(itv_t a, itv_t b);
void itv_mul(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
void itv_div(itv_internal_t* intern, itv_t a, itv_t b, itv_t c);
static inline void itv_add_num(itv_t a, itv_t b, num_t c);
static inline void itv_sub_num(itv_t a, itv_t b, num_t c);
void itv_mul_num(itv_t a, itv_t b, num_t c);
void itv_div_num(itv_t a, itv_t b, num_t c);
static inline void itv_add_bound(itv_t a, itv_t b, bound_t c);
static inline void itv_sub_bound(itv_t a, itv_t b, bound_t c);
void itv_mul_bound(itv_t a, itv_t b, bound_t c);
void itv_div_bound(itv_t a, itv_t b, bound_t c);
bool itv_sqrt(itv_internal_t* intern, itv_t a, itv_t b);
void itv_abs(itv_t a, itv_t b);
static inline void itv_mul_2exp(itv_t a, itv_t b, int c);

static inline void itv_magnitude(bound_t a, itv_t b);
  /* get the absolute value of maximal bound */

void itv_mod(itv_internal_t* intern, itv_t a, itv_t b, itv_t c, bool is_int);
  /* x mod y = x - y*trunc(x/y) */

/* ====================================================================== */
/* Casts */
/* ====================================================================== */
/* Integer casts (rounding towards +oo, -oo, 0, or worst-case) */
static inline void itv_ceil(itv_t a, itv_t b);
static inline void itv_floor(itv_t a, itv_t b);
static inline void itv_trunc(itv_t a, itv_t b);
static inline void itv_to_int(itv_t a, itv_t b);

/* Floating-point casts (worst cases) */
void itv_to_float(itv_t a, itv_t b);
void itv_to_double(itv_t a, itv_t b);


/* ====================================================================== */
/* Printing */
/* ====================================================================== */
int itv_snprint(char* s, size_t size, itv_t a);
void itv_fprint(FILE* stream, itv_t a);
void itv_print(itv_t a);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */
/* These 2 functions return true if the conversion is exact */

bool itv_set_ap_coeff(itv_internal_t* intern, itv_t a, ap_coeff_t* b);
  /* Convert a ap_coeff_t into a itv_t. */

bool ap_coeff_set_itv(itv_internal_t* intern, ap_coeff_t* a, itv_t b);
  /* Convert a itv_t into a ap_coeff_t */

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

/* ====================================================================== */
/* Initialization and clearing */
/* ====================================================================== */

static inline void itv_init(itv_t a)
{
  bound_init(a->neginf);
  bound_init(a->sup);
}
static inline void itv_init_array(itv_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) itv_init(a[i]);
}
static inline void itv_init_set(itv_t a, itv_t b)
{
  bound_init_set(a->neginf,b->neginf);
  bound_init_set(a->sup,b->sup);
}
static inline void itv_clear(itv_t a)
{
  bound_clear(a->neginf);
  bound_clear(a->sup);
}
static inline void itv_clear_array(itv_t* a, size_t size)
{
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

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void itv_set(itv_t a, itv_t b)
{
  bound_set(a->neginf,b->neginf);
  bound_set(a->sup,b->sup);
}
static inline void itv_set_num(itv_t a, num_t b)
{
  bound_set_num(a->sup,b);
  bound_neg(a->neginf,a->sup);
}
static inline void itv_set_num2(itv_t a, num_t b, num_t c)
{
  bound_set_num(a->sup,b);
  num_neg(c,c);
  bound_set_num(a->neginf,c);
  num_neg(c,c);
}
static inline void itv_set_int(itv_t a, long int b)
{
  bound_set_int(a->neginf,-b);
  bound_set_int(a->sup,b);
}
static inline void itv_set_int2(itv_t a, long int b, long int c)
{
  bound_set_int(a->neginf,-b);
  bound_set_int(a->sup,c);
}
static inline void itv_set_bottom(itv_t a)
{
  bound_set_int(a->neginf,-1);
  bound_set_int(a->sup,-1);
}
static inline void itv_set_top(itv_t a)
{
  bound_set_neginfty(a->neginf,1);
  bound_set_neginfty(a->sup,1);
}
static inline void itv_swap(itv_t a, itv_t b)
{ itv_t t; *t=*a;*a=*b;*b=*t; }

static inline void itv_set_unit_num(itv_t a, num_t b)
{
  bound_set_num(a->neginf,b);
  bound_set_num(a->sup,b);
}
static inline void itv_set_unit_bound(itv_t a, bound_t b)
{
  bound_set(a->neginf,b);
  bound_set(a->sup,b);
}
static inline void itv_enlarge_bound(itv_t a, itv_t b, bound_t c)
{
  bound_add(a->neginf,b->neginf,c);
  bound_add(a->sup,b->sup,c);
}

/* ====================================================================== */
/* Normalization and tests */
/* ====================================================================== */

static inline bool itv_is_int(itv_internal_t* intern, itv_t a)
{
  bound_trunc(intern->muldiv_bound,a->sup);
  if (bound_cmp(intern->muldiv_bound,a->sup)) return false;
  bound_trunc(intern->muldiv_bound,a->neginf);
  return !bound_cmp(intern->muldiv_bound,a->neginf);
}
static inline bool itv_is_point(itv_internal_t* intern, itv_t a)
{
  if (!bound_infty(a->neginf) && !bound_infty(a->sup)){
    num_neg(intern->canonicalize_num,bound_numref(a->neginf));
    return num_equal(intern->canonicalize_num,bound_numref(a->sup));
  }
  else
    return false;
}
static inline bool itv_is_zero(itv_t a)
{
  return bound_sgn(a->neginf)==0 && bound_sgn(a->sup)==0;
}
static inline bool itv_is_pos(itv_t a)
{ return (bound_sgn(a->neginf)<=0); }

static inline bool itv_is_neg(itv_t a)
{ return (bound_sgn(a->sup)<=0); }
static inline bool itv_is_top(itv_t a)
{
  return bound_infty(a->neginf) && bound_infty(a->sup);
}
static inline bool itv_is_bottom(itv_internal_t* intern, itv_t a)
{
  return itv_canonicalize(intern, a, false);
}
static inline bool itv_is_leq(itv_t a, itv_t b)
{
  return bound_cmp(a->sup,b->sup)<=0 && bound_cmp(a->neginf,b->neginf)<=0;
}
static inline bool itv_is_eq(itv_t a, itv_t b)
{
  return bound_equal(a->sup,b->sup) && bound_equal(a->neginf,b->neginf);
}

static inline int itv_hash(itv_t a)
{
  return (5*bound_hash(a->neginf) + 7*bound_hash(a->sup));
}
static inline void itv_range_abs(bound_t a, itv_t b)
{ bound_add(a,b->sup,b->neginf); }

static inline void itv_range_rel(itv_internal_t* intern, bound_t a, itv_t b)
{ 
  bound_add(a,b->sup,b->neginf);
  if (!bound_infty(a)) {
    itv_magnitude(intern->muldiv_bound,b);
    bound_div_2(intern->muldiv_bound,intern->muldiv_bound);
    bound_div(a,a,intern->muldiv_bound);
  }
}

/* ====================================================================== */
/* Lattice operations */
/* ====================================================================== */

static inline bool itv_meet(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  bound_min(a->sup,b->sup,c->sup);
  bound_min(a->neginf,b->neginf,c->neginf);
  return itv_canonicalize(intern,a,false);
}
static inline void itv_join(itv_t a, itv_t b, itv_t c)
{
  bound_max(a->sup,b->sup,c->sup);
  bound_max(a->neginf,b->neginf,c->neginf);
}
static inline void itv_widening(itv_t a, itv_t b, itv_t c)
{
  bound_widening(a->sup,b->sup,c->sup);
  bound_widening(a->neginf,b->neginf,c->neginf);
}

/* ====================================================================== */
/* Arithmetic operations */
/* ====================================================================== */

static inline void itv_add(itv_t a, itv_t b, itv_t c)
{
  bound_add(a->sup,b->sup,c->sup);
  bound_add(a->neginf,b->neginf,c->neginf);
}
static inline void itv_add_num(itv_t a, itv_t b, num_t c)
{
  bound_add_num(a->sup,b->sup,c);
  bound_sub_num(a->neginf,b->neginf,c);
}
static inline void itv_sub_num(itv_t a, itv_t b, num_t c)
{
  bound_sub_num(a->sup,b->sup,c);
  bound_add_num(a->neginf,b->neginf,c);
}
static inline void itv_add_bound(itv_t a, itv_t b, bound_t c)
{
  bound_add(a->sup,b->sup,c);
  bound_sub(a->neginf,b->neginf,c);
}
static inline void itv_sub_bound(itv_t a, itv_t b, bound_t c)
{
  bound_sub(a->sup,b->sup,c);
  bound_add(a->neginf,b->neginf,c);
}
static inline void itv_mul_2exp(itv_t a, itv_t b, int c)
{ bound_mul_2exp(a->sup,b->sup,c); bound_mul_2exp(a->neginf,b->neginf,c); }

static inline void itv_magnitude(bound_t a, itv_t b)
{
  if (bound_sgn(b->neginf)<=0) bound_set(a,b->sup);
  else if (bound_sgn(b->sup)<=0) bound_set(a,b->neginf);
  else bound_max(a,b->neginf,b->sup);
}

/* ====================================================================== */
/* Casts */
/* ====================================================================== */

static inline void itv_ceil(itv_t a, itv_t b)
{ bound_ceil(a->sup,b->sup); bound_floor(a->neginf,b->neginf); }

static inline void itv_floor(itv_t a, itv_t b)
{ bound_floor(a->sup,b->sup);  bound_ceil(a->neginf,b->neginf); }

static inline void itv_trunc(itv_t a, itv_t b)
{ bound_trunc(a->sup,b->sup); bound_trunc(a->neginf,b->neginf); }

static inline void itv_to_int(itv_t a, itv_t b)
{ bound_ceil(a->sup,b->sup); bound_ceil(a->neginf,b->neginf); }


static inline void itv_to_float(itv_t a, itv_t b)
{ bound_to_float(a->sup,b->sup); bound_to_float(a->neginf,b->neginf); }

static inline void itv_to_double(itv_t a, itv_t b)
{ bound_to_double(a->sup,b->sup); bound_to_double(a->neginf,b->neginf); }

#ifdef __cplusplus
}
#endif

#endif
