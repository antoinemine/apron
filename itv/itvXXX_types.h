/* ********************************************************************** */
/* itvXXX_types.h: (unidimensional) intervals */
/* ********************************************************************** */

#ifndef _ITVXXX_TYPES_H_
#define _ITVXXX_TYPES_H_

#include "boundTypes.h"
#include "itvConfig.h"
#include "ap_dimension.h"

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

/* Intervals with equality flag (defined here because of itvXXX_internal_t
   datatype below: functions available in eitvXXX.h)
*/
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
  numinternal_t num;
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

/* ********************************************************************** */
/* Interval linear expressions */
/* ********************************************************************** */

/* Interval linear term */
typedef struct itvXXX_linterm_struct {
  eitvXXX_t eitv;
  ap_dim_t dim;
} itvXXX_linterm_struct;
typedef itvXXX_linterm_struct* itvXXX_linterm_ptr;
typedef itvXXX_linterm_struct itvXXX_linterm_t[1];

/* Interval linear expression */
typedef struct itvXXX_linexpr_struct {
  itvXXX_linterm_t* linterm;
  size_t size;
  eitvXXX_t cst;
} itvXXX_linexpr_struct;
typedef itvXXX_linexpr_struct itvXXX_linexpr_t[1];
typedef itvXXX_linexpr_struct* itvXXX_linexpr_ptr;

/* Array of interval linear expressions */
typedef struct itvXXX_linexpr_array_struct {
  itvXXX_linexpr_t* p;
  size_t size;
} itvXXX_linexpr_array_struct;
typedef itvXXX_linexpr_array_struct itvXXX_linexpr_array_t[1];
typedef itvXXX_linexpr_array_struct* itvXXX_linexpr_array_ptr;

/*
  Defined in itvConfig.h for avoiding multiple definitions

 - An interval linear expression is the more general form.
   - A quasilinear expression is such that the only non-scalar
     coefficient is the constant coefficient.
   - A linear expression contains no non-scalar coefficients

   Be cautious, order of labels matters in some functions
*/
/*
typedef enum itvlinexpr_type_t {
  ITV_LINEXPR_INTLINEAR,
  ITV_LINEXPR_QUASILINEAR,
  ITV_LINEXPR_LINEAR
} itvlinexpr_type_t;
*/

/* ********************************************************************** */
/* Interval linear constraints */
/* ********************************************************************** */

/* Interval linear constraint */
typedef struct itvXXX_lincons_struct {
  itvXXX_linexpr_t linexpr;
  itvconstyp_t constyp;
  mpq_t mpq;
} itvXXX_lincons_struct;
typedef itvXXX_lincons_struct itvXXX_lincons_t[1];
typedef itvXXX_lincons_struct* itvXXX_lincons_ptr;

/* Array of interval linear constraint */
typedef struct itvXXX_lincons_array_struct {
  itvXXX_lincons_t* p;
  size_t size;
} itvXXX_lincons_array_struct;
typedef itvXXX_lincons_array_struct itvXXX_lincons_array_t[1];
typedef itvXXX_lincons_array_struct* itvXXX_lincons_array_ptr;

#ifdef __cplusplus
}
#endif

#endif
