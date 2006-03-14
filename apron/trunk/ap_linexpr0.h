/* ************************************************************************* */
/* ap_linexpr0.h: linear expressions */
/* ************************************************************************* */

/* normally included from expr0.h */

#ifndef _AP_LINEXPR0_H_
#define _AP_LINEXPR0_H_

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_dimension.h"

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Discriminant for dense or sparse representation */
typedef enum ap_linexpr_discr_t {
  AP_LINEXPR_DENSE,
  AP_LINEXPR_SPARSE
} ap_linexpr_discr_t;

/* A term, for use in sparse representation */
/* Meant to be an abstract datatype ! */
typedef struct ap_linterm_t {
  ap_dim_t dim;
  ap_coeff_t coeff;
} ap_linterm_t;

/* A linear expression. */
/* Meant to be an abstract datatype ! */
typedef struct ap_linexpr0_t {
  ap_coeff_t cst;             /* constant */
  ap_linexpr_discr_t discr;   /* discriminant for array */
  size_t size;             /* size of the array */
  union {
    ap_coeff_t* coeff;     /* array of coefficients */
    ap_linterm_t* linterm; /* array of linear terms */
  } p;
} ap_linexpr0_t;
/* Important invariant:
   If sparse representation,

   - linear terms are sorted in increasing order wrt their dimension.

   - AP_DIM_MAX dimensions are meaningless: they serve as free linterm when a new dimension
     is needed (this avoids to permanently reallocating the array.
     They should be ignored.

*/

/* Comment: we do not inline the array in the structure, because this allows to
   redimension (with realloc) the array in a transparent way for the user. */

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr0_t* ap_linexpr0_alloc(ap_linexpr_discr_t lin_discr, size_t size);
  /* Allocates a linear expressions with coefficients by default of type SCALAR
     and DOUBLE. If sparse representation, corresponding new dimensions are
     initialized with AP_DIM_MAX. */

void ap_linexpr0_realloc(ap_linexpr0_t* e, size_t size);
  /* Change the dimensions of the array in linexpr0.
     If new coefficients are added, their type is of type SCALAR and DOUBLE.
     If sparse representation, corresponding new dimensions are initialized
     with AP_DIM_MAX. */

void ap_linexpr0_minimize(ap_linexpr0_t* e);
  /* Reduce the coefficients (transform intervals into scalars when possible).
     In case of sparse representation, also remove zero coefficients */

void ap_linexpr0_free(ap_linexpr0_t* linexpr);
  /* Free the linear expression */

ap_linexpr0_t* ap_linexpr0_copy(const ap_linexpr0_t* a);
  /* Duplication */

void ap_linexpr0_fprint(FILE* stream, const ap_linexpr0_t* a, char** name_of_dim);
  /* Printing a linear expression */

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

bool ap_linexpr0_is_integer(const ap_linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool ap_linexpr0_is_real(const ap_linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

bool ap_linexpr0_is_linear(const ap_linexpr0_t* a);
  /* Return true iff all involved coefficients are scalars */
bool ap_linexpr0_is_quasilinear(const ap_linexpr0_t* a);
  /* Return true iff all involved coefficients but the constant are scalars */


/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline
size_t ap_linexpr0_size(const ap_linexpr0_t* expr);
  /* Get the size of the linear expression */

static inline
ap_coeff_t* ap_linexpr0_cstref(ap_linexpr0_t* expr);
  /* Get a reference to the constant. Do not free it. */

ap_coeff_t* ap_linexpr0_coeffref(ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get a reference to the coefficient associated to the dimension.
     Do not free it.
     In case of sparse representation,
     possibly induce the addition of a new linear term.
     Return NULL if:
     In case of dense representation, dim>=expr->size.
     In case of sparse representation, dim==AP_DIM_MAX.
 */

static inline
void ap_linexpr0_get_cst(ap_coeff_t* coeff, const ap_linexpr0_t* expr);
  /* Get the constant and assign it to coeff */

bool ap_linexpr0_get_coeff(ap_coeff_t* coeff, const ap_linexpr0_t* expr, ap_dim_t dim);
  /* Get coefficient of dimension dim in the expression and assign it to coeff
     Return true in case ap_linexpr0_coeffref returns NULL */

/* Set the constant of the linear expression */
static inline void ap_linexpr0_set_cst(ap_linexpr0_t* expr, const ap_coeff_t* cst);
static inline void ap_linexpr0_set_cst_scalar(ap_linexpr0_t* expr, const ap_scalar_t* scalar);
static inline void ap_linexpr0_set_cst_scalar_int(ap_linexpr0_t* expr, int num);
static inline void ap_linexpr0_set_cst_scalar_frac(ap_linexpr0_t* expr, int num, unsigned int den);
static inline void ap_linexpr0_set_cst_scalar_double(ap_linexpr0_t* expr, double num);
static inline void ap_linexpr0_set_cst_interval(ap_linexpr0_t* expr, const ap_interval_t* itv);
static inline void ap_linexpr0_set_cst_interval_scalar(ap_linexpr0_t* expr, const ap_scalar_t* inf, const ap_scalar_t* sup);
static inline void ap_linexpr0_set_cst_interval_int(ap_linexpr0_t* expr, int inf, int sup);
static inline void ap_linexpr0_set_cst_interval_frac(ap_linexpr0_t* expr,
							 int numinf, unsigned int deninf,
							 int numsup, unsigned int densup);
static inline void ap_linexpr0_set_cst_interval_double(ap_linexpr0_t* expr, double inf, double sup);

/* Set the coefficient of dimension dim in the expression.
   Return true in case ap_linexpr0_coeffref returns NULL */
static inline bool ap_linexpr0_set_coeff(ap_linexpr0_t* expr, ap_dim_t dim, const ap_coeff_t* coeff);
static inline bool ap_linexpr0_set_coeff_scalar(ap_linexpr0_t* expr, ap_dim_t dim, const ap_scalar_t* scalar);
static inline bool ap_linexpr0_set_coeff_scalar_int(ap_linexpr0_t* expr, ap_dim_t dim, int num);
static inline bool ap_linexpr0_set_coeff_scalar_frac(ap_linexpr0_t* expr, ap_dim_t dim, int num, unsigned int den);
static inline bool ap_linexpr0_set_coeff_scalar_double(ap_linexpr0_t* expr, ap_dim_t dim, double num);
static inline bool ap_linexpr0_set_coeffinterval(ap_linexpr0_t* expr, ap_dim_t dim, const ap_interval_t* itv);
static inline bool ap_linexpr0_set_coeff_interval_scalar(ap_linexpr0_t* expr, ap_dim_t dim, const ap_scalar_t* inf, const ap_scalar_t* sup);
static inline bool ap_linexpr0_set_coeff_interval_int(ap_linexpr0_t* expr, ap_dim_t dim, int inf, int sup);
static inline bool ap_linexpr0_set_coeff_interval_frac(ap_linexpr0_t* expr, ap_dim_t dim,
							   int numinf, unsigned int deninf,
							   int numsup, unsigned int densup);
static inline bool ap_linexpr0_set_coeff_interval_double(ap_linexpr0_t* expr, ap_dim_t dim, double inf, double sup);

/*
bool ap_linexpr0_set_format_generic(ap_coeff_t* (*get_pcoeff)(char*,va_list*,void*,bool*),
				 void* expr, char* fmt, va_list* ap);

bool ap_linexpr0_set_format(ap_linexpr0_t* expr, char* fmt, ...);
*/

typedef enum ap_coefftag_t {
  AP_COEFF,
  AP_COEFF_S,
  AP_COEFF_S_MPQ,
  AP_COEFF_S_INT,
  AP_COEFF_S_FRAC,
  AP_COEFF_S_DOUBLE,
  AP_COEFF_I,
  AP_COEFF_I_SCALAR,
  AP_COEFF_I_MPQ,
  AP_COEFF_I_INT,
  AP_COEFF_I_FRAC,
  AP_COEFF_I_DOUBLE,
  AP_CST,
  AP_CST_S,
  AP_CST_S_MPQ,
  AP_CST_S_INT,
  AP_CST_S_FRAC,
  AP_CST_S_DOUBLE,
  AP_CST_I,
  AP_CST_I_SCALAR,
  AP_CST_I_MPQ,
  AP_CST_I_INT,
  AP_CST_I_FRAC,
  AP_CST_I_DOUBLE,
  AP_END
} ap_coefftag_t;
 
bool ap_linexpr0_set_list_generic(ap_coeff_t* (*get_pcoeff)(void* expr, bool cst, va_list* va),
				  void* expr, va_list* va);

bool ap_linexpr0_set_list(ap_linexpr0_t* expr, ...);

/* Iterator (Macro): use:
   ap_linexpr0_ForeachLinterm(ap_linexpr0_t* e, size_t i, ap_dim_t d, ap_coeff_t* coeff){
     ..
   }
   where
   - e is the inspected expression,
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term
   - coeff is a pointer to the corresponding coefficient

   AP_DIM_MAX dimensions are filtered out.

*/
#define ap_linexpr0_ForeachLinterm(_p_e_, _p_i_, _p_dim_, _p_ap_coeff) \
  for ((_p_i_)=0; \
       (_p_i_)<(_p_e_)->size ? \
	 ((_p_e_)->discr==AP_LINEXPR_DENSE ? \
	  ((_p_dim_) = (_p_i_), \
	   (_p_ap_coeff) = &(_p_e_)->p.coeff[_p_i_], \
	   true) :				\
	  ((_p_dim_) = (_p_e_)->p.linterm[_p_i_].dim, \
	   (_p_ap_coeff) = &(_p_e_)->p.linterm[_p_i_].coeff, \
	   (_p_dim_)!=AP_DIM_MAX)) :			   \
	 false; \
       (_p_i_)++)

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexpr0_add_dimensions_with(ap_linexpr0_t* expr,
				  const ap_dimchange_t* dimchange);
ap_linexpr0_t* ap_linexpr0_add_dimensions(const ap_linexpr0_t* expr,
				    const ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexpr0_permute_dimensions_with(ap_linexpr0_t* expr,
					 const ap_dimperm_t* perm);
ap_linexpr0_t* ap_linexpr0_permute_dimensions(const ap_linexpr0_t* expr,
					      const ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linexpr0_hash(const ap_linexpr0_t* expr);
bool ap_linexpr0_equal(ap_linexpr0_t* expr1,
		    ap_linexpr0_t* expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexpr0_compare(ap_linexpr0_t* expr1,
		     ap_linexpr0_t* expr2);

/* ====================================================================== */
/* VI. Inline function definitions */
/* ====================================================================== */

static inline
size_t ap_linexpr0_size(const ap_linexpr0_t* expr)
  { return expr->size; }

static inline
ap_coeff_t* ap_linexpr0_cstref(ap_linexpr0_t* expr)
  { return &expr->cst; }

static inline
void ap_linexpr0_get_cst(ap_coeff_t* coeff, const ap_linexpr0_t* expr)
  { ap_coeff_set(coeff,&expr->cst); }

static inline
void ap_linexpr0_set_cst(ap_linexpr0_t* expr, const ap_coeff_t* cst)
  { ap_coeff_set(&expr->cst,cst); }

static inline
void ap_linexpr0_set_cst_scalar(ap_linexpr0_t* expr, const ap_scalar_t* scalar)
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
void ap_linexpr0_set_cst_interval(ap_linexpr0_t* expr, const ap_interval_t* itv)
  { ap_coeff_set_interval(&expr->cst, itv); }

static inline
void ap_linexpr0_set_cst_interval_int(ap_linexpr0_t* expr, int inf, int sup)
  { ap_coeff_set_interval_int(&expr->cst, inf,sup); }

static inline
void ap_linexpr0_set_cst_interval_scalar(ap_linexpr0_t* expr, const ap_scalar_t* inf, const ap_scalar_t* sup)
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
bool ap_linexpr0_set_coeff(ap_linexpr0_t* expr, ap_dim_t dim, const ap_coeff_t* coeff)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ap_coeff_set(ecoeff,coeff); return false;} else return true; }

static inline
bool ap_linexpr0_set_coeff_scalar(ap_linexpr0_t* expr, ap_dim_t dim, const ap_scalar_t* scalar)
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
bool ap_linexpr0_set_coeffinterval(ap_linexpr0_t* expr, ap_dim_t dim, const ap_interval_t* itv)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval(ecoeff,itv); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_int(ap_linexpr0_t* expr, ap_dim_t dim, int inf, int sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_int(ecoeff,inf,sup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_scalar(ap_linexpr0_t* expr, ap_dim_t dim, const ap_scalar_t* inf, const ap_scalar_t* sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_scalar(ecoeff,inf,sup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_frac(ap_linexpr0_t* expr, ap_dim_t dim,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_frac(ecoeff,numinf,deninf, numsup,densup); return false; } else return true; }

static inline
bool ap_linexpr0_set_coeff_interval_double(ap_linexpr0_t* expr, ap_dim_t dim, double inf, double sup)
  { ap_coeff_t* ecoeff = ap_linexpr0_coeffref(expr,dim); if (ecoeff){ ap_coeff_set_interval_double(ecoeff,inf,sup); return false; } else return true; }




#endif
