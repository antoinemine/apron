/* ********************************************************************** */
/* ap_dimension.h: dimensions and related operations */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_DIMENSION_H_
#define _AP_DIMENSION_H_

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for dimensions */
typedef unsigned int ap_dim_t;
#define AP_DIM_MAX UINT_MAX
/* Used for sparse representations (mean: to be ignored) and also as
   a result when an error occurred */

/* Datatype for specifying the dimensionality of an abstract value */
typedef struct ap_dimension_t {
  size_t intdim;
  size_t realdim;
} ap_dimension_t;

/* Datatype for specifying change of dimension (addition or removal) */
typedef struct ap_dimchange_t {
  ap_dim_t* dim;  /* Assumed to be an array of size intdim+realdim */
  size_t intdim ; /* Number of integer dimensions to add/remove */
  size_t realdim; /* Number of real dimensions to add/remove */
} ap_dimchange_t;

/* The semantics is the following:

- Addition of dimensions:

  dimchange.dim[i]=k means: add one dimension at dimension k and shift the
  already existing dimensions greater than or equal to k one step on the
  right (or increment them).

  if k is equal to the size of the vector, then it means: add a dimension at
  the end.

  Repetition are allowed, and means that one inserts more than one dimensions.

  Example:
  linexpr0_add_dimensions([i0 i1 r0 r1], { [0 1 2 2 4],3,1 }) returns
  [0 i0 0 i1 0 0 r0 r1 0], considered as a vector with 6 integer dimensions
  and 3 real dimensions.

- Removal of dimensions

  dimchange.dim[i]=k means: remove the dimension k and shift the dimensions
  greater than k one step on the left (or decrement them).

  Repetitions are meaningless (and are not correct specification)

  Example:
  linexpr0_remove_dimensions([i0 i1 i2 r0 r1 r2], { [0 2 4],2,1 }) returns
  [i1 r0 r2], considered as a vector with 1 integer dimensions
  and 2 real dimensions.

*/

/* Datatype for specifying double changes of dimensions (combination of
   addition and then removal). Used by level 1 function
   change_environment. */
typedef struct ap_dimchange2_t {
  ap_dimchange_t* add;    /* If not NULL, specifies the adding new dimensions */
  ap_dimchange_t* remove; /* If not NULL, specifies the removal of dimensions */
} ap_dimchange2_t;

/* Datatype for permutations */
typedef struct ap_dimperm_t {
  ap_dim_t* dim;    /* Array assumed to be of size size */
  size_t size;
} ap_dimperm_t;
/* Such an object represent the permutation
   i -> dimperm.p[i] for 0<=i<dimperm.size */

/* ====================================================================== */
/* Functions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* ap_dimchange_t */
/* ---------------------------------------------------------------------- */

void ap_dimchange_init(ap_dimchange_t* dimchange, size_t intdim, size_t realdim);
  /* Initialize a dimchange structure (allocate internal array) */
ap_dimchange_t* ap_dimchange_alloc(size_t intdim, size_t realdim);
  /* Allocate and initialize a dimchange structure */

static inline void ap_dimchange_clear(ap_dimchange_t* dimchange);
  /* Clear a dimchange structure (deallocate internal array) */
static inline void ap_dimchange_free(ap_dimchange_t* dimchange);
  /* Deallocate and clear a dimchange structure */

void ap_dimchange_fprint(FILE* stream, ap_dimchange_t* dimchange);
  /* Printing */
void ap_dimchange_add_invert(ap_dimchange_t* dimchange);
  /* Assuming that dimchange is a transformation for add_dimensions,
     invert it to obtain the inverse transformation using remove_dimensions */

/* ---------------------------------------------------------------------- */
/* ap_dimchange2_t */
/* ---------------------------------------------------------------------- */

static inline void ap_dimchange2_init(ap_dimchange2_t* dimchange2,
				      ap_dimchange_t* add, 
				      ap_dimchange_t* remove);
  /* Initialize a dimchange2 structure by filling its fields with
     arguments */
static inline ap_dimchange2_t* ap_dimchange2_alloc(ap_dimchange_t* add, 
						   ap_dimchange_t* remove);
  /* Allocate and initialize a dimchange2 structure */

void ap_dimchange2_clear(ap_dimchange2_t* dimchange2);
  /* Clear a dimchange structure (deallocate its fields) */
void ap_dimchange2_free(ap_dimchange2_t* dimchange2);
  /* Deallocate and clear a dimchange2 structure */

void ap_dimchange2_fprint(FILE* stream, ap_dimchange2_t* dimchange2);
  /* Printing */

/* ---------------------------------------------------------------------- */
/* ap_dimperm_t */
/* ---------------------------------------------------------------------- */

void ap_dimperm_init(ap_dimperm_t* dimperm, size_t size);
  /* Initialize a dimperm structure (allocate internal array) */
ap_dimperm_t* ap_dimperm_alloc(size_t size);
  /* Allocate and initialize a dimperm structure */

static inline void ap_dimperm_clear(ap_dimperm_t* dimperm);
  /* Clear a dimperm structure (deallocate internal arrau) */
static inline void ap_dimperm_free(ap_dimperm_t* dimperm);
  /* Deallocate and clear a dimchange structure */

void ap_dimperm_fprint(FILE* stream, ap_dimperm_t* perm);
  /* Print a permutation under the form:
     dimperm: size=...
     0 -> perm->dim[0]
     1 -> perm->dim[1]
     ...
 */

void ap_dimperm_set_id(ap_dimperm_t* perm);
  /* Generate the identity permutation */

void ap_dimperm_compose(ap_dimperm_t* perm,
			ap_dimperm_t* perm1, ap_dimperm_t* perm2);
  /* Compose the 2 permutations perm1 and perm2 (in this order)
     and store the result the already allocated perm.
     The sizes of permutations are supposed to be equal.
     At exit, we have perm.dim[i] = perm2.dim[perm1.dim[i]]
  */
void ap_dimperm_invert(ap_dimperm_t* nperm, ap_dimperm_t* perm);
  /* Invert the permutation perm and store it in the already allocated nperm.
     The sizes of permutations are supposed to be equal.
  */

/* ====================================================================== */
/* Inline Functions Definitions */
/* ====================================================================== */
static inline void ap_dimchange_clear(ap_dimchange_t* dimchange)
{
  if (dimchange->dim) free(dimchange->dim);
  dimchange->intdim = dimchange->realdim = 0;
  dimchange->dim = NULL;
}
static inline void ap_dimchange_free(ap_dimchange_t* dimchange)
{
  ap_dimchange_clear(dimchange);
  free(dimchange);
}

static inline
void ap_dimperm_clear(ap_dimperm_t* dimperm)
{
  if (dimperm->dim) free(dimperm->dim);
  dimperm->size = 0;
  dimperm->dim = NULL;
}
static inline
void ap_dimperm_free(ap_dimperm_t* dimperm)
{
  ap_dimperm_clear(dimperm);
  free(dimperm);
}

static inline 
void ap_dimchange2_init(ap_dimchange2_t* dimchange2,
			ap_dimchange_t* add, 
			ap_dimchange_t* remove)
{ 
  dimchange2->add = add;
  dimchange2->remove = remove;
}
static inline 
ap_dimchange2_t* ap_dimchange2_alloc(ap_dimchange_t* add, 
				     ap_dimchange_t* remove)
{
  ap_dimchange2_t* res = (ap_dimchange2_t*)malloc(sizeof(ap_dimchange2_t));
  ap_dimchange2_init(res,add,remove);
  return res;
}

#ifdef __cplusplus
}
#endif

#endif
