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
}

/* Important invariant:
   - linear terms are sorted in increasing order wrt their dimension.

   - AP_DIM_MAX dimensions are meaningless: they serve as free linterm when a new dimension
     is needed (this avoids to permanently reallocating the array.
     They should be ignored.
*/

/* Array of linear expressions */
typedef struct ap_linexpr_array_t {
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_t*    D;
    itvMPQ_linexpr_t*  MPQ;
    itvMPFR_linexpr_t* MPFR;
  } p;
  size_t size;
}

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr0_t* ap_linexpr0_alloc(ap_scalar_discr_t discr, size_t size);
  /* Allocates a linear expressions with the given number of linterms */

void ap_linexpr0_realloc(ap_linexpr0_t* e, size_t size);
  /* Change the dimensions of the array in linexpr0. */

void ap_linexpr0_minimize(ap_linexpr0_t* e);
  /* Reduce the coefficients (transform intervals into scalars when possible).
     In case of sparse representation, also remove zero coefficients */

void ap_linexpr0_free(ap_linexpr0_t* linexpr);
  /* Free the linear expression */

ap_linexpr0_t* ap_linexpr0_copy(ap_linexpr0_t* a);
  /* Duplication */

void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t* a, char** name_of_dim);
  /* Printing a linear expression */

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

bool ap_linexpr0_is_integer(ap_linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool ap_linexpr0_is_real(ap_linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integers */

  /* Expression classification */

itv_linexpr_type_t ap_linexpr0_type(ap_linexpr0_t* a);
  /* Return the type of the linear expression */
bool ap_linexpr0_is_linear(ap_linexpr0_t* a);
  /* Return true iff all involved coefficients are scalars */
bool ap_linexpr0_is_quasilinear(ap_linexpr0_t* a);
  /* Return true iff all involved coefficients but the constant are scalars */

itv_linexpr_type_t ap_linexpr0_array_type(ap_linexpr0_array* array);
bool ap_linexpr0_array_is_linear(ap_linexpr0_array* array);
bool ap_linexpr0_array_is_quasilinear(ap_linexpr0_array* array);
  /* Idem for arrays */

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline
size_t ap_linexpr0_size(ap_linexpr0_t* expr);
  /* Get the size of the linear expression */

static inline
ap_coeff_t ap_linexpr0_cstref(ap_linexpr0_t* expr);
  /* Get a reference to the constant. Do not clear it. */

ap_coeff_t ap_linexpr0_coeffref(ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get a reference to the coefficient associated to the dimension.
     Do not clear it.
     possibly induce the addition of a new linear term.
 */

static inline
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
     itv_linexpr_set_list(intern,
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

/* ====================================================================== */
/* Vb. Array of expressions */
/* ====================================================================== */

/* Free the array of expressions of size size */
void ap_linexpr0_array_free(ap_linexpr0_t** texpr, size_t size);

/* ====================================================================== */
/* VI. Inline function definitions */
/* ====================================================================== */

static inline
size_t ap_linexpr0_size(ap_linexpr0_t* expr)
  { return expr->size; }

static inline
ap_coeff_t* ap_linexpr0_cstref(ap_linexpr0_t* expr)
  { return &expr->cst; }

static inline
void ap_linexpr0_get_cst(ap_coeff_t* coeff, ap_linexpr0_t* expr)
  { ap_coeff_set(coeff,&expr->cst); }

static inline
void ap_linexpr0_set_cst(ap_linexpr0_t* expr, ap_coeff_t* cst)
  { ap_coeff_set(&expr->cst,cst); }

static inline
void ap_linexpr0_set_cst_scalar(ap_linexpr0_t* expr, ap_scalar_t* scalar)
  { ap_coeff_set_scalar(&expr->cst, scalar); }

static inline
void ap_linexpr0_set_cst_scalar_int(ap_linexpr0_t* expr, int num)
  { ap_coeff_set_scalar_int(&expr->cst, num); }

static inline
void ap_linexpr0_set_cst_scalar_frac(ap_linexpr0_t* expr, int num, unsigned int den)
  { ap_coeff_set_scalar_frac(&expr->cst, num, den); }

static inline
void ap_linexpr0_set_cst_scalar_double(ap_linexpr0_t* expr, double num)
  { ap_coeff_set_scalar_double(&expr->cst, num); }

static inline
void ap_linexpr0_set_cst_interval(ap_linexpr0_t* expr, ap_interval_t* itv)
  { ap_coeff_set_interval(&expr->cst, itv); }

static inline
void ap_linexpr0_set_cst_interval_int(ap_linexpr0_t* expr, int inf, int sup)
  { ap_coeff_set_interval_int(&expr->cst, inf,sup); }

static inline
void ap_linexpr0_set_cst_interval_scalar(ap_linexpr0_t* expr, ap_scalar_t* inf, ap_scalar_t* sup)
  { ap_coeff_set_interval_scalar(&expr->cst, inf,sup); }

static inline
void ap_linexpr0_set_cst_interval_frac(ap_linexpr0_t* expr,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { ap_coeff_set_interval_frac(&expr->cst, numinf,deninf, numsup,densup); }

static inline
void ap_linexpr0_set_cst_interval_double(ap_linexpr0_t* expr, double inf, double sup)
  { ap_coeff_set_interval_double(&expr->cst, inf,sup); }

static inline
bool ap_linexpr0_set_coeff(ap_linexpr0_t* expr, ap_dim_t dim, ap_coeff_t* coeff)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ap_coeff_set(ecoeff,coeff); return false;} else return true; }

static inline
bool ap_linexpr0_set_coeff_scalar(ap_linexpr0_t* expr, ap_dim_t dim, ap_scalar_t* scalar)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_scalar(ecoeff,scalar); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_scalar_int(ap_linexpr0_t* expr, ap_dim_t dim, int num)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_scalar_int(ecoeff,num); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_scalar_frac(ap_linexpr0_t* expr, ap_dim_t dim, int num, unsigned int den)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_scalar_frac(ecoeff,num, den); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_scalar_double(ap_linexpr0_t* expr, ap_dim_t dim, double num)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_scalar_double(ecoeff,num); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeffinterval(ap_linexpr0_t* expr, ap_dim_t dim, ap_interval_t* itv)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval(ecoeff,itv); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_int(ap_linexpr0_t* expr, ap_dim_t dim, int inf, int sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_int(ecoeff,inf,sup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_scalar(ap_linexpr0_t* expr, ap_dim_t dim, ap_scalar_t* inf, ap_scalar_t* sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_scalar(ecoeff,inf,sup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_frac(ap_linexpr0_t* expr, ap_dim_t dim,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_frac(ecoeff,numinf,deninf, numsup,densup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_double(ap_linexpr0_t* expr, ap_dim_t dim, double inf, double sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_double(ecoeff,inf,sup); return false; } else return true; }



#ifdef __cplusplus
}
#endif

#endif
