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
  ap_scalar_discr_t discr;
  union {
    ap_zzzD_array_ptr    D;
    ap_zzzMPQ_array_ptr  MPQ;
    ap_zzzMPFR_array_ptr MPFR;
  } zzz_array;
} ap_yyy_array_struct;
typedef ap_yyy_array_struct* ap_yyy_array_ptr;
typedef ap_yyy_array_struct ap_yyy_array_t[1];

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy_array_init(ap_yyy_array_t e, ap_scalar_discr_t discr, size_t size);
void ap_yyy_array_init_set(ap_yyy_array_t res, ap_yyy_array_t e);
void ap_yyy_array_minimize(ap_yyy_array_t a);
void ap_yyy_array_clear(ap_yyy_array_t e);
void ap_yyy_array_fprint(FILE* stream, ap_yyy_array_t e, char** name_of_dim);

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_yyy_array_set(ap_yyy_array_t res, ap_yyy_array_t e, num_internal_t intern);

MACRO_MAINZ
bool ap_yyy_array_set_zzzZZZ(ap_yyy_array_t a, ap_zzzZZZ_array_t b, num_internal_t intern);
bool ap_zzzZZZ_array_set_yyy_array(ap_zzzZZZ_array_t a, ap_yyy_array_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

size_t ap_yyy_array_size(ap_yyy_array_t p);
ap_linexpr_type_t ap_yyy_array_type(ap_yyy_array_t array);
bool ap_yyy_array_is_linear(ap_yyy_array_t array);
bool ap_yyy_array_is_quasilinear(ap_yyy_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

void ap_yyy_array_ref_index(ap_yyy_t a, ap_yyy_array_t p, size_t i);

bool ap_yyy_array_get_index(ap_yyy_t a, ap_yyy_array_t p, size_t i, num_internal_t intern);
bool ap_yyy_array_set_index(ap_yyy_array_t p, size_t i, ap_yyy_t a, num_internal_t intern);

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

MACRO_MAINZ
static inline void ap_yyy_array_cons_ZZZ(ap_yyy_array_t res, ap_zzzZZZ_array_t e)
{ res->discr = AP_SCALAR_ZZZ; res->zzz_array.ZZZ = e; };
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
