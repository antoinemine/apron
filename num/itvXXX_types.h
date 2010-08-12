/* ********************************************************************** */
/* itvXXX_types.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITVXXX_TYPES_H_
#define _ITVXXX_TYPES_H_

#include "bound_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Intervals */
/* ********************************************************************** */

/* Intervals.

   Be cautious: interval [a,b] is represented by [-a,b].  This is because
   bound quantities are always rounded toward +infty */
typedef struct itvXXX_struct {
  boundXXX_t neginf; /* negation of the inf bound */
  boundXXX_t sup; /* sup bound */
} itvXXX_struct;
typedef itvXXX_struct itvXXX_t[1];
typedef itvXXX_struct* itvXXX_ptr;

/* Intervals with equality flag */
typedef struct eitvXXX_struct {
  itvXXX_t itv;
  bool eq;
} eitvXXX_struct;
typedef eitvXXX_struct eitvXXX_t[1];
typedef eitvXXX_struct* eitvXXX_ptr;

/* ********************************************************************** */
/* Workspace */
/* ********************************************************************** */

/* Workspace to avoid temporary allocation and deallocation when numXXX_t and
   boundXXX_t are multiprecision numbers */

/* some useful local constant for linearization */
typedef struct {
  itvXXX_t ulp;        /* [-1;1] * unit in the least place */
  itvXXX_t min;        /* [-1;1] * minimum positive denormal */
  itvXXX_t min_normal; /* [-1;1] * minimum positive normal */
  itvXXX_t max;        /* [-1;1] * maximum non +oo  */
  itvXXX_t max_exact;  /* [-1;1] * maximum exactly representable integer */
} itvXXX_float_const;

typedef struct __itvXXX_internal_struct {
  numXXX_t canonicalize_num;
  boundXXX_t muldiv_bound;
  boundXXX_t mul_bound;
  boundXXX_t sqrt_bound;
  boundXXX_t linear_bound;
  boundXXX_t linear_bound2;
  boundXXX_t linear_bound3;
  itvXXX_t mul_itv;
  itvXXX_t mul_itv2;
  itvXXX_t eval_itv;
  itvXXX_t eval_itv2;
  itvXXX_t eval_itv3;
  numXXX_t quasi_num;
  eitvXXX_t boxize_lincons_eitv;
  itvXXX_t boxize_lincons_eval;
  boundXXX_t boxize_lincons_bound;
  itvXXX_float_const cst_half, cst_single, cst_double, cst_extended, cst_quad;
  itvXXX_t itvXXX_half; /* [-0.5,0.5] */
  mpz_t reduce_lincons_gcd;
  mpz_t reduce_lincons_mpz;
} __itvXXX_internal_struct;
typedef __itvXXX_internal_struct* itvXXX_internal_ptr;
typedef __itvXXX_internal_struct itvXXX_internal_t[1];

itvXXX_internal_ptr itvXXX_internal_alloc(void);
  /* Allocate and initialize internal workspace */
void itvXXX_internal_free(itvXXX_internal_ptr intern);
  /* Clear and free internal workspace */

void itvXXX_internal_init(itvXXX_internal_t intern);
void itvXXX_internal_clear(itvXXX_internal_t intern);
  /* GMP Semantics */

#ifdef __cplusplus
}
#endif

#endif
