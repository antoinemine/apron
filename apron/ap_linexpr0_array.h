/* ************************************************************************* */
/* ap_linexpr0_array.h: array of linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_LINEXPR0_ARRAY_H_
#define _AP_LINEXPR0_ARRAY_H_

#include "ap_linexpr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Array of linear expressions */
typedef struct ap_linexpr0_array_struct {
  bool ref;
  ap_scalar_discr_t discr;
  union {
    itvD_linexpr_t*    D;
    itvMPQ_linexpr_t*  MPQ;
    itvMPFR_linexpr_t* MPFR;
  } p;
  size_t size;
} ap_linexpr0_array_struct;
typedef ap_linexpr0_array_struct ap_linexpr0_array_t[1];
typedef ap_linexpr0_array_struct* ap_linexpr0_array_ptr;

static inline void ap_linexpr0_array_init_ref_D(ap_linexpr0_array_t res, itvD_linexpr_t* p, size_t size);
static inline void ap_linexpr0_array_init_ref_MPQ(ap_linexpr0_array_t res, itvMPQ_linexpr_t* p, size_t size);
static inline void ap_linexpr0_array_init_ref_MPFR(ap_linexpr0_array_t res, itvMPFR_linexpr_t* p, size_t size);

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexpr0_array_add_dimensions(ap_linexpr0_array_t a,
				      ap_linexpr0_array_t b,
				      ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexpr0_array_permute_dimensions(ap_linexpr0_array_t a,
					  ap_linexpr0_array_t b,
					  ap_dimperm_t* perm);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif
