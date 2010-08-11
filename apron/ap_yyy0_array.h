/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_yyy_array.h: array of ap_yyy_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_yyy_ARRAY_H_
#define _AP_yyy_ARRAY_H_

#include "ap_yyy.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _AP_yyy_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Array of linear expressions */
typedef struct ap_yyy_array_struct {
  bool ref;
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_array_ptr    D;
    itvMPQ_linexpr_array_ptr  MPQ;
    itvMPFR_linexpr_array_ptr MPFR;
  } linexpr_array;
} ap_yyy_array_struct;
typedef ap_yyy_array_struct* ap_yyy_array_ptr;
typedef ap_yyy_array_struct ap_yyy_array_t[1];

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy_array_init(ap_yyy_array_t e, ap_scalar_discr_t discr, size_t size);
void ap_yyy_array_init_set(ap_yyy_array_t res, ap_yyy_array_t e);
MACROX
static inline void ap_yyy_array_init_cons_XXX(ap_yyy_array_t res, itvXXX_linexpr_array_t e);
ENDMACRO
void ap_yyy_array_minimize(ap_yyy_array_t a);
void ap_yyy_array_clear(ap_yyy_array_t e);
void ap_yyy_array_fprint(FILE* stream, ap_yyy_array_t e, char** name_of_dim);

MACROX
static inline void ap_yyy_array_ref_XXX(ap_yyy_array_t res, itvXXX_linexpr_array_t e);
ENDMACRO
  /* Returns a reference on the argument under the form of ap_yyy_ref.
     INTERNAL USE.
     BE CAUTIOUS: memory is shared between the result and the argument, and
     memory should be managed through the argument. */

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_yyy_array_set(ap_yyy_array_t res, ap_yyy_array_t e, numinternal_t intern);

MACROX
bool ap_yyy_array_set_itvXXX_linexpr(ap_yyy_array_t a, itvXXX_linexpr_array_t b, numinternal_t intern);
bool itvXXX_linexpr_array_set_ap_yyy_array(itvXXX_linexpr_array_t a, ap_yyy_array_t b, numinternal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

size_t ap_yyy_array_size(ap_yyy_array_t p);
itvlinexpr_type_t ap_yyy_array_type(ap_yyy_array_t array);
bool ap_yyy_array_is_linear(ap_yyy_array_t array);
bool ap_yyy_array_is_quasilinear(ap_yyy_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

void ap_yyy_array_ref_index(ap_yyy_t a, ap_yyy_array_t p, size_t i);

bool ap_yyy_array_get_index(ap_yyy_t a, ap_yyy_array_t p, size_t i, numinternal_t intern);
bool ap_yyy_array_set_index(ap_yyy_array_t p, size_t i, ap_yyy_t a, numinternal_t intern);

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_yyy_array_add_dimensions(ap_yyy_array_t a,
				      ap_yyy_array_t b,
				      ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_yyy_array_permute_dimensions(ap_yyy_array_t a,
					  ap_yyy_array_t b,
					  ap_dimperm_t* perm);

MACROX
static inline void ap_yyy_array_init_cons_XXX(ap_yyy_array_t res, itvXXX_linexpr_array_t e)
{ res->ref = false; res->discr = AP_SCALAR_XXX; res->linexpr_array.XXX = e; };

static inline void ap_yyy_array_ref_XXX(ap_yyy_array_t res, itvXXX_linexpr_array_t e)
{ res->ref = true; res->discr = AP_SCALAR_XXX; res->linexpr_array.XXX = e; };
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
