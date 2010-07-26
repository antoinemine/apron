/* ************************************************************************* */
/* ap_linexpr0.h: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_LINEXPR0_H_
#define _AP_LINEXPR0_H_

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_dimension.h"
#include "itvD_linexpr.h"
#include "itvMPQ_linexpr.h"
#include "itvMPFR_linexpr.h"
#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* A linear expression. */
typedef struct ap_linexpr0_struct {
  bool ref;
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_ptr    D;
    itvMPQ_linexpr_ptr  MPQ;
    itvMPFR_linexpr_ptr MPFR;
  } linexpr;
} ap_linexpr0_struct;
typedef ap_linexpr0_struct ap_linexpr0_t[1];
typedef ap_linexpr0_struct* ap_linexpr0_ptr;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linexpr0_init(ap_linexpr0_t res, ap_scalar_discr_t discr, size_t size);
void ap_linexpr0_init_set(ap_linexpr0_t res, ap_linexpr0_t e);
void ap_linexpr0_init_set_D(ap_linexpr0_t res, itvD_linexpr_t e);
void ap_linexpr0_init_set_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e);
void ap_linexpr0_init_set_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e);
static inline void ap_linexpr0_init_cons_D(ap_linexpr0_t res, itvD_linexpr_t e);
static inline void ap_linexpr0_init_cons_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e);
static inline void ap_linexpr0_init_cons_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e);

void ap_linexpr0_clear(ap_linexpr0_t e);
void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t e, char** name_of_dim);
  /* Printing a linear expression */
void ap_linexpr0_minimize(ap_linexpr0_t a);

static inline void ap_linexpr0_ref_D(ap_linexpr0_t res, itvD_linexpr_t e);
static inline void ap_linexpr0_ref_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e);
static inline void ap_linexpr0_ref_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e);
  /* Returns a reference on the argument under the form of ap_linexpr0_ref.
     INTERNAL USE.
     BE CAUTIOUS: memory is shared between the result and the argument, and
     memory should be managed through the argument. */

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_linexpr0_set(ap_linexpr0_t res, ap_linexpr0_t e, numinternal_t intern);

bool ap_linexpr0_set_itvD_linexpr(ap_linexpr0_t a, itvD_linexpr_t b, numinternal_t intern);
bool ap_linexpr0_set_itvMPQ_linexpr(ap_linexpr0_t a, itvMPQ_linexpr_t b, numinternal_t intern);
bool ap_linexpr0_set_itvMPFR_linexpr(ap_linexpr0_t a, itvMPFR_linexpr_t b, numinternal_t intern);

bool itvD_linexpr_set_ap_linexpr0(itvD_linexpr_t a, ap_linexpr0_t b, numinternal_t intern);
bool itvDMPQ_linexpr_set_ap_linexpr0(itvMPQ_linexpr_t a, ap_linexpr0_t b, numinternal_t intern);
bool itvMPFR_linexpr_set_ap_linexpr0(itvMPFR_linexpr_t a, ap_linexpr0_t b, numinternal_t intern);

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

bool ap_linexpr0_get_cst(ap_coeff_t coeff, ap_linexpr0_t expr, numinternal_t intern);
  /* Get the constant and assign it to coeff with possible conversion */

bool ap_linexpr0_get_coeff(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim, numinternal_t intern);
  /* Get coefficient of dimension dim in the expression and assign it to coeff
     Return true in case ap_linexpr0_coeffref returns NULL */

void ap_linexpr0_cstref(ap_coeff_t coeff, ap_linexpr0_t expr);
void ap_linexpr0_coeffref(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim);

bool ap_linexpr0_set_list(numinternal_t intern, ap_linexpr0_t expr, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type itvcoefftag_t, and ended by the tag ITV_END;

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

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexpr0_add_dimensions(ap_linexpr0_t a,
				ap_linexpr0_t b,
				ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexpr0_permute_dimensions(ap_linexpr0_t a,
				    ap_linexpr0_t b,
				    ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linexpr0_hash(ap_linexpr0_t expr);
bool ap_linexpr0_equal(ap_linexpr0_t expr1,
		       ap_linexpr0_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexpr0_compare(ap_linexpr0_t expr1,
			ap_linexpr0_t expr2);

static inline void ap_linexpr0_init_cons_D(ap_linexpr0_t res, itvD_linexpr_t e)
{ res->ref = false; res->discr = AP_SCALAR_D; res->linexpr.D = e; };
static inline void ap_linexpr0_init_cons_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPQ; res->linexpr.MPQ = e; };
static inline void ap_linexpr0_init_cons_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPFR; res->linexpr.MPFR = e; };

static inline void ap_linexpr0_ref_D(ap_linexpr0_t res, itvD_linexpr_t e)
{ res->ref = true; res->discr = AP_SCALAR_D; res->linexpr.D = e; };
static inline void ap_linexpr0_ref_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPQ; res->linexpr.MPQ = e; };
static inline void ap_linexpr0_ref_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPFR; res->linexpr.MPFR = e; };

#ifdef __cplusplus
}
#endif

#endif
