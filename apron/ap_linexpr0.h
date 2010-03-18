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
typedef struct ap_linexpr0_t {
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_t    D;
    itvMPQ_linexpr_t  MPQ;
    itvMPFR_linexpr_t MPFR;
  } linexpr;
} ap_linexpr0_t;

/* Important invariant:
   - linear terms are sorted in increasing order wrt their dimension.

   - AP_DIM_MAX dimensions are meaningless: they serve as free linterm when a new dimension
     is needed (this avoids to permanently reallocating the array.
     They should be ignored.
*/

/* Array of linear expressions */
typedef struct ap_linexpr0_array_t {
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_t*    D;
    itvMPQ_linexpr_t*  MPQ;
    itvMPFR_linexpr_t* MPFR;
  } p;
  size_t size;
} ap_linexpr0_array_t;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr0_t* ap_linexpr0_alloc(ap_scalar_discr_t discr, size_t size);
  /* Allocates a linear expressions with the given number of linterms */

void ap_linexpr0_minimize(ap_linexpr0_t* e);
  /* Reduce the coefficients (transform intervals into scalars when possible).
     In case of sparse representation, also remove zero coefficients */

void ap_linexpr0_free(ap_linexpr0_t* linexpr);
  /* Free the linear expression */

ap_linexpr0_t* ap_linexpr0_copy(ap_linexpr0_t* a);
  /* Duplication */

void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t* a, char** name_of_dim);
  /* Printing a linear expression */
  /* Idem for arrays */

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

ap_coeff_t ap_linexpr0_cstref(ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get a reference to the constant coefficient.
     Do not clear it.
     possibly induce the addition of a new linear term.
 */
ap_coeff_t ap_linexpr0_coeffref(ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get a reference to the coefficient associated to the dimension.
     Do not clear it.
     possibly induce the addition of a new linear term.
 */

void ap_linexpr0_get_cst(ap_coeff_t* coeff, ap_linexpr0_t* expr);
  /* Get the constant and assign it to coeff */

bool ap_linexpr0_get_coeff(ap_coeff_t* coeff, ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get coefficient of dimension dim in the expression and assign it to coeff
     Return true in case ap_linexpr0_coeffref returns NULL */

bool ap_linexpr0_set_list(ap_linexpr0_t* expr, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coefftag_t, and ended by the tag ITV_END;

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

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexpr0_add_dimensions_with(ap_linexpr0_t* expr,
				  ap_dimchange_t* dimchange);
ap_linexpr0_t* ap_linexpr0_add_dimensions(ap_linexpr0_t* expr,
					  ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexpr0_permute_dimensions_with(ap_linexpr0_t* expr,
					 ap_dimperm_t* perm);
ap_linexpr0_t* ap_linexpr0_permute_dimensions(ap_linexpr0_t* expr,
					      ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

long ap_linexpr0_hash(ap_linexpr0_t* expr);
bool ap_linexpr0_equal(ap_linexpr0_t* expr1,
		    ap_linexpr0_t* expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexpr0_compare(ap_linexpr0_t* expr1,
			ap_linexpr0_t* expr2);

#ifdef __cplusplus
}
#endif

#endif
