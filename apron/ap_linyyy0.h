/* ************************************************************************* */
/* ap_linyyy0.h: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_linyyy0_H_
#define _AP_linyyy0_H_

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_linyyyD.h"
#include "ap_linyyyMPQ.h"
#include "ap_linyyyMPFR.h"
#ifndef _AP_linexpr0_H_
#include "ap_linexpr0.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define _AP_linyyy0_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* A linear expression. */
typedef struct ap_linyyy0_struct {
  ap_scalar_discr_t discr;
  union {
    ap_linyyyD_ptr    D;
    ap_linyyyMPQ_ptr  MPQ;
    ap_linyyyMPFR_ptr MPFR;
  } linyyy;
} ap_linyyy0_struct;
typedef ap_linyyy0_struct ap_linyyy0_t[1];
typedef ap_linyyy0_struct* ap_linyyy0_ptr;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_init(ap_linyyy0_t res, ap_scalar_discr_t discr, size_t size);
void ap_linyyy0_init_set(ap_linyyy0_t res, ap_linyyy0_t e);
MACRO_MAINZ
void ap_linyyy0_init_set_linyyyZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e);
ENDMACRO
void ap_linyyy0_clear(ap_linyyy0_t e);
void ap_linyyy0_fprint(FILE* stream, ap_linyyy0_t e, char** name_of_dim);
void ap_linyyy0_minimize(ap_linyyy0_t a);

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_linyyy0_set(ap_linyyy0_t res, ap_linyyy0_t e, num_internal_t intern);
MACRO_MAINZ
bool ap_linyyy0_set_linyyyZZZ(ap_linyyy0_t a, ap_linyyyZZZ_t b, num_internal_t intern);
bool ap_linyyyZZZ_set_linyyy0(ap_linyyyZZZ_t a, ap_linyyy0_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline 
ap_scalar_discr_t ap_linyyy0_discr(ap_linyyy0_t expr);

#if defined(_AP_linexpr0_MARK_)
bool ap_linexpr0_get_cst(ap_coeff_t coeff, ap_linexpr0_t expr, num_internal_t intern);
  /* Get the constant and assign it to coeff with possible conversion */
bool ap_linexpr0_get_coeff(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim, num_internal_t intern);
  /* Get coefficient of dimension dim in the expression and assign it to
     coeff with possible conversion. */

bool ap_linexpr0_set_cst(ap_linexpr0_t expr, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the constant with possible conversion. */
bool ap_linexpr0_set_coeff(ap_linexpr0_t expr, ap_dim_t dim, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the coefficient of dimension dim in the expression, with possible
     conversion. */

void ap_linexpr0_cstref(ap_coeff_t coeff, ap_linexpr0_t expr);
void ap_linexpr0_coeffref(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim);

bool ap_linexpr0_set_list(num_internal_t intern, ap_linexpr0_t expr, bool* perror, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coeff_tag_t_t, and ended by the tag AP_END;

     - The dimension AP_COEFF_DIM_MAX/AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexpr0_set_list(intern,
			  expr,
			  AP_COEFF_LFRAC,7,9,0,
			  AP_COEFF_DOUBLE2,-3.0,4.5,1,
			  AP_COEFF_LLINT,3LL,AP_COEFF_DIM_MAX,
			  AP_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */

#elif defined (_AP_lincons0_MARK_) || defined (_AP_lingen0_MARK_)

bool ap_linyyy0_get_linexpr0(ap_linexpr0_t e, ap_linyyy0_t c, num_internal_t intern);
  /* Get the underlying expression and assign it to e with possible
     conversion */
ap_yyytyp_t ap_linyyy0_get_yyytyp(ap_linyyy0_t c);

void ap_linyyy0_linexpr0ref(ap_linexpr0_t e, ap_linyyy0_t c);
ap_yyytyp_t* ap_linyyy0_yyytypref(ap_linyyy0_t c);

bool ap_linyyy0_set_linexpr0(ap_linyyy0_t c, ap_linexpr0_t e, num_internal_t intern);
  /* Assign the underlying expression of c to e with possible conversion */
void ap_linyyy0_set_yyytyp(ap_linyyy0_t c, ap_yyytyp_t yyytyp);

#if defined (_AP_lincons0_MARK_)
void ap_lincons0_get_mpq(mpq_t mpq, ap_lincons0_t c);
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c);
void ap_lincons0_set_mpq(ap_lincons0_t c, mpq_t mpq);
#endif

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linyyy0_add_dimensions(ap_linyyy0_t a,
			    ap_linyyy0_t b,
			    ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linyyy0_permute_dimensions(ap_linyyy0_t a,
				ap_linyyy0_t b,
				ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linyyy0_hash(ap_linyyy0_t expr);
bool ap_linyyy0_equal(ap_linyyy0_t expr1,
		       ap_linyyy0_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linyyy0_compare(ap_linyyy0_t expr1,
			ap_linyyy0_t expr2);

static inline 
ap_scalar_discr_t ap_linyyy0_discr(ap_linyyy0_t expr)
{ return expr->discr; }
static inline void ap_linyyy0_cons(ap_linyyy0_t res, ap_linyyy0_t e)
{
  res->discr = e->discr;
  res->linyyy = e->linyyy;
}
MACRO_MAINZ
static inline void ap_linyyy0_cons_ZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e)
{ res->discr = AP_SCALAR_ZZZ; res->linyyy.ZZZ = e; };
ENDMACRO

#undef _AP_linyyy0_MARK_

#ifdef __cplusplus
}
#endif

#endif
