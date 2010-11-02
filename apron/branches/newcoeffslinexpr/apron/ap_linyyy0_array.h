/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_linyyy0_array.h: array of ap_linyyy0_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_linyyy0_ARRAY_H_
#define _AP_linyyy0_ARRAY_H_

#include "ap_linyyy0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Array of linear expressions */
typedef struct ap_linyyy0_array_struct {
  ap_scalar_discr_t discr;
  union {
    ap_linyyyD_array_ptr    D;
    ap_linyyyMPQ_array_ptr  MPQ;
    ap_linyyyMPFR_array_ptr MPFR;
  } linyyy_array;
} ap_linyyy0_array_struct;
typedef ap_linyyy0_array_struct* ap_linyyy0_array_ptr;
typedef ap_linyyy0_array_struct ap_linyyy0_array_t[1];

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_array_init(ap_linyyy0_array_t e, ap_scalar_discr_t discr, size_t size);
void ap_linyyy0_array_init_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e);
MACRO_MAINZ
void ap_linyyy0_array_init_set_linyyyZZZ_array(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e);
ENDMACRO
void ap_linyyy0_array_resize(ap_linyyy0_array_t a, size_t size);
void ap_linyyy0_array_minimize(ap_linyyy0_array_t a);
void ap_linyyy0_array_clear(ap_linyyy0_array_t e);
void ap_linyyy0_array_fprint(FILE* stream, ap_linyyy0_array_t e, char** name_of_dim);

/* ====================================================================== */
/* II. Access, possibly with conversions */
/* ====================================================================== */

static inline ap_scalar_discr_t ap_linyyy0_array_discr(ap_linyyy0_array_t p);
size_t ap_linyyy0_array_size(ap_linyyy0_array_t p);
#if !defined(_AP_lingen0_ARRAY_H_)
ap_linexpr_type_t ap_linyyy0_array_type(ap_linyyy0_array_t array);
bool ap_linyyy0_array_is_quasilinear(ap_linyyy0_array_t array);
#endif
bool ap_linyyy0_array_is_linear(ap_linyyy0_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

bool ap_linyyy0_array_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e, num_internal_t intern);
bool ap_linyyy0_array_set_index(ap_linyyy0_array_t p, size_t i, ap_linyyy0_t a, num_internal_t intern);
bool ap_linyyy0_array_get_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern);
void ap_linyyy0_array_ref_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i);

MACRO_MAINZ
bool ap_linyyy0_array_set_linyyyZZZ_array(ap_linyyy0_array_t a, ap_linyyyZZZ_array_t b, num_internal_t intern);
bool ap_linyyyZZZ_array_set_linyyy0_array(ap_linyyyZZZ_array_t a, ap_linyyy0_array_t b, num_internal_t intern);

bool ap_linyyy0_array_set_index_linyyyZZZ(ap_linyyy0_array_t p, size_t i, ap_linyyyZZZ_t a, num_internal_t intern);
bool ap_linyyy0_array_get_index_linyyyZZZ(ap_linyyyZZZ_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linyyy0_array_add_dimensions(ap_linyyy0_array_t a,
				  ap_linyyy0_array_t b,
				  ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linyyy0_array_permute_dimensions(ap_linyyy0_array_t a,
				      ap_linyyy0_array_t b,
				      ap_dimperm_t* perm);

static inline ap_scalar_discr_t ap_linyyy0_array_discr(ap_linyyy0_array_t p)
{ return p->discr; }
MACRO_MAINZ
static inline void ap_linyyy0_array_cons_ZZZ(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e)
{ res->discr = AP_SCALAR_ZZZ; res->linyyy_array.ZZZ = e; };
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
