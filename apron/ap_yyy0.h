/* ************************************************************************* */
/* ap_yyy0.h: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_yyy0_H_
#define _AP_yyy0_H_

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_dimension.h"
#ifndef _AP_linexpr0_H_
#include "ap_linexpr0.h"
#endif
#include "ap_yyyD.h"
#include "ap_yyyMPQ.h"
#include "ap_yyyMPFR.h"
#ifdef __cplusplus
extern "C" {
#endif

#define _AP_yyy0_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* A linear expression. */
typedef struct ap_yyy0_struct {
  ap_scalar_discr_t discr;
  union {
    ap_yyyD_ptr    D;
    ap_yyyMPQ_ptr  MPQ;
    ap_yyyMPFR_ptr MPFR;
  } yyy;
} ap_yyy0_struct;
typedef ap_yyy0_struct ap_yyy0_t[1];
typedef ap_yyy0_struct* ap_yyy0_ptr;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy0_init(ap_yyy0_t res, ap_scalar_discr_t discr, size_t size);
void ap_yyy0_init_set(ap_yyy0_t res, ap_yyy0_t e);
MACRO_MAINZ
void ap_yyy0_init_set_yyyZZZ(ap_yyy0_t res, ap_yyyZZZ_t e);
ENDMACRO
void ap_yyy0_clear(ap_yyy0_t e);
void ap_yyy0_fprint(FILE* stream, ap_yyy0_t e, char** name_of_dim);
void ap_yyy0_minimize(ap_yyy0_t a);

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_yyy0_set(ap_yyy0_t res, ap_yyy0_t e, num_internal_t intern);
MACRO_MAINZ
bool ap_yyy0_set_yyyZZZ(ap_yyy0_t a, ap_yyyZZZ_t b, num_internal_t intern);
bool ap_yyyZZZ_set_yyy0(ap_yyyZZZ_t a, ap_yyy0_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

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
     the definition of the type ap_coeff_tag_t_t, and ended by the tag ITV_END;

     - The dimension ITV_DIM_MAX/AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexpr0_set_list(intern,
			  expr,
			  ITV_LFRAC,7,9,0,
			  ITV_DOUBLE2,-3.0,4.5,1,
			  ITV_LLINT,3LL,ITV_DIM_MAX,
			  ITV_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */

#elif defined(_AP_lincons0_MARK_)

bool ap_lincons0_get_linexpr0(ap_linexpr0_t e, ap_lincons0_t c, num_internal_t intern);
  /* Get the underlying expression and assign it to e with possible
     conversion */
ap_constyp_t ap_lincons0_get_constyp(ap_lincons0_t c);
void ap_lincons0_get_mpq(mpq_t mpq, ap_lincons0_t c);

void ap_lincons0_linexpr0ref(ap_linexpr0_t e, ap_lincons0_t c);
ap_constyp_t* ap_lincons0_constypref(ap_lincons0_t c);
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c);

bool ap_lincons0_set_linexpr0(ap_lincons0_t c, ap_linexpr0_t e, num_internal_t intern);
  /* Assign the underlying expression of c to e with possible conversion */
void ap_lincons0_set_constyp(ap_lincons0_t c, ap_constyp_t constyp);
void ap_lincons0_set_mpq(ap_lincons0_t c, mpq_t mpq);

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_yyy0_add_dimensions(ap_yyy0_t a,
			    ap_yyy0_t b,
			    ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_yyy0_permute_dimensions(ap_yyy0_t a,
				ap_yyy0_t b,
				ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_yyy0_hash(ap_yyy0_t expr);
bool ap_yyy0_equal(ap_yyy0_t expr1,
		       ap_yyy0_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_yyy0_compare(ap_yyy0_t expr1,
			ap_yyy0_t expr2);

MACRO_MAINZ
static inline void ap_yyy0_cons_ZZZ(ap_yyy0_t res, ap_yyyZZZ_t e)
{ res->discr = AP_SCALAR_ZZZ; res->yyy.ZZZ = e; };
ENDMACRO

#undef _AP_yyy0_MARK_

#ifdef __cplusplus
}
#endif

#endif
