/* ************************************************************************* */
/* ap_lincons0.h: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_LINCONS0_ARRAY_H_
#define _AP_LINCONS0_ARRAY_H_

#include "ap_lincons0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

typedef struct ap_lincons0_array_struct {
  bool ref;
  ap_scalar_discr_t discr;
  union {
    itvD_lincons_array_ptr    D;
    itvMPQ_lincons_array_ptr  MPQ;
    itvMPFR_lincons_array_ptr MPFR;
  } lincons_array;
} ap_lincons0_array_struct;
typedef ap_lincons0_array_struct ap_lincons0_array_t[1];
typedef ap_lincons0_array_struct* ap_lincons0_array_ptr;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_lincons0_array_init(ap_lincons0_array_t e, ap_scalar_discr_t discr, size_t size);
void ap_lincons0_array_init_set(ap_lincons0_array_t res,ap_lincons0_array_t e);
static inline void ap_lincons0_array_init_cons_D(ap_lincons0_array_t res, itvD_lincons_array_t e);
static inline void ap_lincons0_array_init_cons_MPQ(ap_lincons0_array_t res, itvMPQ_lincons_array_t e);
static inline void ap_lincons0_array_init_cons_MPFR(ap_lincons0_array_t res, itvMPFR_lincons_array_t e);

void ap_lincons0_array_minimize(ap_lincons0_array_t a);
void ap_lincons0_array_clear(ap_lincons0_array_t e);
void ap_lincons0_array_fprint(FILE* stream, ap_lincons0_array_t e, char** name_of_dim);

static inline void ap_lincons0_array_ref_D(ap_lincons0_array_t res, itvD_lincons_array_t e);
static inline void ap_lincons0_array_ref_MPQ(ap_lincons0_array_t res, itvMPQ_lincons_array_t e);
static inline void ap_lincons0_array_ref_MPFR(ap_lincons0_array_t res, itvMPFR_lincons_array_t e);
  /* Returns a reference on the argument under the form of ap_lincons0_ref.
     INTERNAL USE.
     BE CAUTIOUS: memory is shared between the result and the argument, and
     memory should be managed through the argument. */

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_lincons0_array_set(ap_lincons0_array_t res, ap_lincons0_array_t e, numinternal_t intern);

bool ap_lincons0_array_set_itvD_lincons(ap_lincons0_array_t a, itvD_lincons_array_t b, numinternal_t intern);
bool ap_lincons0_array_set_itvMPQ_lincons(ap_lincons0_array_t a, itvMPQ_lincons_array_t b, numinternal_t intern);
bool ap_lincons0_array_set_itvMPFR_lincons(ap_lincons0_array_t a, itvMPFR_lincons_array_t b, numinternal_t intern);

bool itvD_lincons_array_set_ap_lincons0_array(itvD_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern);
bool itvDMPQ_lincons_array_set_ap_lincons0_array(itvMPQ_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern);
bool itvMPFR_lincons_array_set_ap_lincons0_array(itvMPFR_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern);

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

size_t ap_lincons0_array_size(ap_lincons0_array_t p);
itvlinexpr_type_t ap_lincons0_array_type(ap_lincons0_array_t array);
bool ap_lincons0_array_is_linear(ap_lincons0_array_t array);
bool ap_lincons0_array_is_quasilinear(ap_lincons0_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

void ap_lincons0_array_ref_index(ap_lincons0_t a, ap_lincons0_array_t p, size_t i);

bool ap_lincons0_array_get_index(ap_lincons0_t a, ap_lincons0_array_t p, size_t i, numinternal_t intern);
bool ap_lincons0_array_set_index(ap_lincons0_array_t p, size_t i, ap_lincons0_t a, numinternal_t intern);

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_lincons0_array_add_dimensions(ap_lincons0_array_t a,
				      ap_lincons0_array_t b,
				      ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_lincons0_array_permute_dimensions(ap_lincons0_array_t a,
					  ap_lincons0_array_t b,
					  ap_dimperm_t* perm);

static inline void ap_lincons0_array_init_cons_D(ap_lincons0_array_t res, itvD_lincons_array_t e)
{ res->ref = false; res->discr = AP_SCALAR_D; res->lincons_array.D = e; };
static inline void ap_lincons0_array_init_cons_MPQ(ap_lincons0_array_t res, itvMPQ_lincons_array_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPQ; res->lincons_array.MPQ = e; };
static inline void ap_lincons0_array_init_cons_MPFR(ap_lincons0_array_t res, itvMPFR_lincons_array_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPFR; res->lincons_array.MPFR = e; };

static inline void ap_lincons0_array_ref_D(ap_lincons0_array_t res, itvD_lincons_array_t e)
{ res->ref = true; res->discr = AP_SCALAR_D; res->lincons_array.D = e; };
static inline void ap_lincons0_array_ref_MPQ(ap_lincons0_array_t res, itvMPQ_lincons_array_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPQ; res->lincons_array.MPQ = e; };
static inline void ap_lincons0_array_ref_MPFR(ap_lincons0_array_t res, itvMPFR_lincons_array_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPFR; res->lincons_array.MPFR = e; };

#ifdef __cplusplus
}
#endif

#endif
