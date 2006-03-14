/* ************************************************************************* */
/* linexpr0.h: linear expressions */
/* ************************************************************************* */

/* normally included from expr0.h */

#ifndef _LINEXPR0_H_
#define _LINEXPR0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coeff.h"

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* datatype for dimensions */
typedef unsigned int dim_t;
#define DIM_MAX UINT_MAX
/* Used for sparse representations (mean: to be ignored) and also as
   a result when an error occured */

/* Discriminant for dense or sparse representation */
typedef enum linexpr_discr_t {
  LINEXPR_DENSE,
  LINEXPR_SPARSE
} linexpr_discr_t;

/* A term, for use in sparse representation */
/* Meant to be an abstract datatype ! */
typedef struct linterm_t {
  dim_t dim;
  coeff_t coeff;
} linterm_t;

/* A linear expression. */
/* Meant to be an abstract datatype ! */
typedef struct linexpr0_t {
  coeff_t cst;             /* constant */
  linexpr_discr_t discr;   /* discriminant for array */
  size_t size;             /* size of the array */
  union {
    coeff_t* coeff;     /* array of coefficients */
    linterm_t* linterm; /* array of linear terms */
  } p;
} linexpr0_t;
/* Important invariant:
   If sparse representation, 

   - linear terms are sorted in increasing order wrt their dimension.

   - DIM_MAX dimensions are meaningless: they serve as free linterm when a new dimension
     is needed (this avoids to permanently reallocating the array.
     They should be ignored.

*/

/* Comment: we do not inline the array in the structure, because this allows to
   redimension (with realloc) the array in a transparent way for the user. */

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

linexpr0_t* linexpr0_alloc(linexpr_discr_t lin_discr, size_t size);
  /* Allocates a linear expressions with coefficients by default of type SCALAR
     and DOUBLE. If sparse representation, corresponding new dimensions are
     initialized with DIM_MAX. */

void linexpr0_realloc(linexpr0_t* e, size_t size);
  /* Change the dimensions of the array in linexpr0.
     If new coefficients are added, their type is of type SCALAR and DOUBLE.
     If sparse representation, corresponding new dimensions are initialized
     with DIM_MAX. */

void linexpr0_minimize(linexpr0_t* e);
  /* Reduce the coefficients (transform intervals into scalars when possible).
     In case of sparse representation, also remove zero coefficients */

void linexpr0_free(linexpr0_t* linexpr);
  /* Free the linear expression */

linexpr0_t* linexpr0_copy(const linexpr0_t* a);
  /* Duplication */

void linexpr0_fprint(FILE* stream, const linexpr0_t* a, char** name_of_dim);
  /* Printing a linear expression */

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

bool linexpr0_is_integer(const linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool linexpr0_is_real(const linexpr0_t* a, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

bool linexpr0_is_linear(const linexpr0_t* a);
  /* Return true iff all involved coefficients are scalars */
bool linexpr0_is_quasilinear(const linexpr0_t* a);
  /* Return true iff all involved coefficients but the constant are scalars */


/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline
size_t linexpr0_size(const linexpr0_t* expr);
  /* Get the size of the linear expression */

static inline
coeff_t* linexpr0_cstref(linexpr0_t* expr);
  /* Get a reference to the constant. Do not free it. */

coeff_t* linexpr0_coeffref(linexpr0_t* expr, dim_t dim);
  /* Get a reference to the coefficient associated to the dimension.
     Do not free it.
     In case of sparse representation,
     possibly induce the addition of a new linear term.
     Return NULL if:
     In case of dense representation, dim>=expr->size.
     In case of sparse representation, dim==DIM_MAX.
 */

static inline
void linexpr0_get_cst(coeff_t* coeff, const linexpr0_t* expr);
  /* Get the constant and assign it to coeff */

bool linexpr0_get_coeff(coeff_t* coeff, const linexpr0_t* expr, dim_t dim);
  /* Get coefficient of dimension dim in the expression and assign it to coeff
     Return true in case linexpr0_coeffref returns NULL */

/* Set the constant of the linear expression */
static inline void linexpr0_set_cst(linexpr0_t* expr, const coeff_t* cst);
static inline void linexpr0_set_cst_scalar(linexpr0_t* expr, const scalar_t* scalar);
static inline void linexpr0_set_cst_scalar_int(linexpr0_t* expr, int num);
static inline void linexpr0_set_cst_scalar_frac(linexpr0_t* expr, int num, unsigned int den);
static inline void linexpr0_set_cst_scalar_double(linexpr0_t* expr, double num);
static inline void linexpr0_set_cst_interval(linexpr0_t* expr, const interval_t* itv);
static inline void linexpr0_set_cst_interval_infsup(linexpr0_t* expr, const scalar_t* inf, const scalar_t* sup);
static inline void linexpr0_set_cst_interval_infsup_int(linexpr0_t* expr, int inf, int sup);
static inline void linexpr0_set_cst_interval_infsup_frac(linexpr0_t* expr,
							 int numinf, unsigned int deninf,
							 int numsup, unsigned int densup);
static inline void linexpr0_set_cst_interval_infsup_double(linexpr0_t* expr, double inf, double sup);

/* Set the coefficient of dimension dim in the expression.
   Return true in case linexpr0_coeffref returns NULL */
static inline bool linexpr0_set_coeff(linexpr0_t* expr, dim_t dim, const coeff_t* coeff);
static inline bool linexpr0_set_coeff_scalar(linexpr0_t* expr, dim_t dim, const scalar_t* scalar);
static inline bool linexpr0_set_coeff_scalar_int(linexpr0_t* expr, dim_t dim, int num);
static inline bool linexpr0_set_coeff_scalar_frac(linexpr0_t* expr, dim_t dim, int num, unsigned int den);
static inline bool linexpr0_set_coeff_scalar_double(linexpr0_t* expr, dim_t dim, double num);
static inline bool linexpr0_set_coeff_interval(linexpr0_t* expr, dim_t dim, const interval_t* itv);
static inline bool linexpr0_set_coeff_interval_infsup(linexpr0_t* expr, dim_t dim, const scalar_t* inf, const scalar_t* sup);
static inline bool linexpr0_set_coeff_interval_infsup_int(linexpr0_t* expr, dim_t dim, int inf, int sup);
static inline bool linexpr0_set_coeff_interval_infsup_frac(linexpr0_t* expr, dim_t dim,
							   int numinf, unsigned int deninf,
							   int numsup, unsigned int densup);
static inline bool linexpr0_set_coeff_interval_infsup_double(linexpr0_t* expr, dim_t dim, double inf, double sup);

/* Iterator (Macro): use:
   linexpr0_ForeachLinterm(linexpr0_t* e, size_t i, dim_t d, coeff_t* coeff){
     ..
   }
   where
   - e is the inspected expression,
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term
   - coeff is a pointer to the corresponding coefficient

   DIM_MAX dimensions are filtered out.

*/
#define linexpr0_ForeachLinterm(e, i, d, coeff) \
  for ((i)=0; \
       (i)<(e)->size ? \
	 ((e)->discr==LINEXPR_DENSE ? \
	  ((d) = (i), (coeff) = &(e)->p.coeff[i], true) : \
	  ((d) = (e)->p.linterm[i].dim, (coeff) = &(e)->p.linterm[i].coeff, (d)!=DIM_MAX)) : \
	 false; \
       i++)

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* Datatype for specifying change of dimension */
typedef struct dimchange_t {
  size_t intdim ; /* Number of integer dimensions to add/remove */
  size_t realdim; /* Number of real dimensions to add/remove */
  dim_t dim[0];   /* Assumed to be an array of size intdim+realdim */
} dimchange_t;

/* The semantics is the following:

- Addition of dimensions:

  dimchange.dim[k] means: add one dimension at dimension k and shift the
  already existing dimensions greater than or equal to k one step on the right
  (or increment them).

  if k is equal to the size of the vector, then it means: add a dimension at
  the end.

  Repetion are allowed, and means that one inserts more than one dimensions.

  Example:
  linexpr0_add_dimensions([i0 i1 r0 r1], { [0 1 2 2 4],3,1 }) returns
  [0 i0 0 i1 0 0 r0 r1 0], considered as a vector with 5 integer dimensions
  and 4 real dimensions.

- Removal of dimensions

  dimchange.dim[k] means: remove the dimension k and shift the dimensions
  greater than k one step on the left (or decrement them).

  Repetitions are meaningless (and are not correct specification)

  Example:
  linexpr0_remove_dimensions([i0 i1 i2 r0 r1 r2], { [0 2 4],2,1 }) returns
  [i1 r0 r2], considered as a vector with 1 integer dimensions
  and 2 real dimensions.

*/

dimchange_t* dimchange_alloc(size_t intdim, size_t realdim);
  /* Allocate a dimchange structure */

static inline
void dimchange_free(dimchange_t* dimchange)
  { free(dimchange); }

void dimchange_add_invert(dimchange_t* dimchange);
  /* Assuming that dimchange is a transformation for add_dimensions, 
     return the inverse transformation using remove_dimensions */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void linexpr0_add_dimensions_with(linexpr0_t* expr,
				  const dimchange_t* dimchange);
linexpr0_t* linexpr0_add_dimensions(const linexpr0_t* expr,
				    const dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void linexpr0_permute_dimensions_with(linexpr0_t* expr,
				      const dim_t* perm);
linexpr0_t* linexpr0_permute_dimensions(const linexpr0_t* expr,
					const dim_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

int linexpr0_hash(const linexpr0_t* expr);
bool linexpr0_equal(linexpr0_t* expr1,
		    linexpr0_t* expr2);
  /* Induces reduction of the coefficients */

/* ====================================================================== */
/* VI. Inline function definitions */
/* ====================================================================== */

static inline
size_t linexpr0_size(const linexpr0_t* expr)
  { return expr->size; }

static inline
coeff_t* linexpr0_cstref(linexpr0_t* expr)
  { return &expr->cst; }

static inline
void linexpr0_get_cst(coeff_t* coeff, const linexpr0_t* expr)
  { coeff_set(coeff,&expr->cst); }

static inline
void linexpr0_set_cst(linexpr0_t* expr, const coeff_t* cst)
  { coeff_set(&expr->cst,cst); }

static inline
void linexpr0_set_cst_scalar(linexpr0_t* expr, const scalar_t* scalar)
  { coeff_set_scalar(&expr->cst, scalar); }

static inline
void linexpr0_set_cst_scalar_int(linexpr0_t* expr, int num)
  { coeff_set_scalar_int(&expr->cst, num); }

static inline
void linexpr0_set_cst_scalar_frac(linexpr0_t* expr, int num, unsigned int den)
  { coeff_set_scalar_frac(&expr->cst, num, den); }

static inline
void linexpr0_set_cst_scalar_double(linexpr0_t* expr, double num)
  { coeff_set_scalar_double(&expr->cst, num); }

static inline
void linexpr0_set_cst_interval(linexpr0_t* expr, const interval_t* itv)
  { coeff_set_interval(&expr->cst, itv); }

static inline
void linexpr0_set_cst_interval_infsup_int(linexpr0_t* expr, int inf, int sup)
  { coeff_set_interval_infsup_int(&expr->cst, inf,sup); }

static inline
void linexpr0_set_cst_interval_infsup(linexpr0_t* expr, const scalar_t* inf, const scalar_t* sup)
  { coeff_set_interval_infsup(&expr->cst, inf,sup); }

static inline
void linexpr0_set_cst_interval_infsup_frac(linexpr0_t* expr,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { coeff_set_interval_infsup_frac(&expr->cst, numinf,deninf, numsup,densup); }

static inline
void linexpr0_set_cst_interval_infsup_double(linexpr0_t* expr, double inf, double sup)
  { coeff_set_interval_infsup_double(&expr->cst, inf,sup); }

static inline
bool linexpr0_set_coeff(linexpr0_t* expr, dim_t dim, const coeff_t* coeff)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){coeff_set(ecoeff,coeff); return false;} else return true; }

static inline
bool linexpr0_set_coeff_scalar(linexpr0_t* expr, dim_t dim, const scalar_t* scalar)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_scalar(ecoeff,scalar); return false; } else return true; }

static inline
bool linexpr0_set_coeff_scalar_int(linexpr0_t* expr, dim_t dim, int num)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_scalar_int(ecoeff,num); return false; } else return true; }

static inline
bool linexpr0_set_coeff_scalar_frac(linexpr0_t* expr, dim_t dim, int num, unsigned int den)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_scalar_frac(ecoeff,num, den); return false; } else return true; }

static inline
bool linexpr0_set_coeff_scalar_double(linexpr0_t* expr, dim_t dim, double num)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_scalar_double(ecoeff,num); return false; } else return true; }

static inline
bool linexpr0_set_coeff_interval(linexpr0_t* expr, dim_t dim, const interval_t* itv)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_interval(ecoeff,itv); return false; } else return true; }

static inline
bool linexpr0_set_coeff_interval_infsup_int(linexpr0_t* expr, dim_t dim, int inf, int sup)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_interval_infsup_int(ecoeff,inf,sup); return false; } else return true; }

static inline
bool linexpr0_set_coeff_interval_infsup(linexpr0_t* expr, dim_t dim, const scalar_t* inf, const scalar_t* sup)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_interval_infsup(ecoeff,inf,sup); return false; } else return true; }

static inline
bool linexpr0_set_coeff_interval_infsup_frac(linexpr0_t* expr, dim_t dim,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_interval_infsup_frac(ecoeff,numinf,deninf, numsup,densup); return false; } else return true; }

static inline
bool linexpr0_set_coeff_interval_infsup_double(linexpr0_t* expr, dim_t dim, double inf, double sup)
  { coeff_t* ecoeff = linexpr0_coeffref(expr,dim); if (ecoeff){ coeff_set_interval_infsup_double(ecoeff,inf,sup); return false; } else return true; }




#endif
