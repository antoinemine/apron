/* ************************************************************************* */
/* ap_lincons0.h: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_LINCONS0_H_
#define _AP_LINCONS0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_linexpr0.h"
#include "itvD_lincons.h"
#include "itvMPQ_lincons.h"
#include "itvMPFR_lincons.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

typedef struct ap_lincons0_struct {
  bool ref;
  ap_scalar_discr_t discr;
  union {
    itvD_lincons_ptr    D;
    itvMPQ_lincons_ptr  MPQ;
    itvMPFR_lincons_ptr MPFR;
  } lincons;
} ap_lincons0_struct;
typedef ap_lincons0_struct ap_lincons0_t[1];
typedef ap_lincons0_struct* ap_lincons0_ptr;


/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_lincons0_init(ap_lincons0_t res, ap_scalar_discr_t discr, size_t size);
void ap_lincons0_init_set(ap_lincons0_t res, ap_lincons0_t e);
void ap_lincons0_init_set_D(ap_lincons0_t res, itvD_lincons_t e);
void ap_lincons0_init_set_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e);
void ap_lincons0_init_set_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e);
static inline void ap_lincons0_init_cons_D(ap_lincons0_t res, itvD_lincons_t e);
static inline void ap_lincons0_init_cons_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e);
static inline void ap_lincons0_init_cons_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e);

void ap_lincons0_minimize(ap_lincons0_t a);
void ap_lincons0_clear(ap_lincons0_t e);
void ap_lincons0_fprint(FILE* stream, ap_lincons0_t e, char** name_of_dim);
  /* Printing a linear expression */

static inline void ap_lincons0_ref_D(ap_lincons0_t res, itvD_lincons_t e);
static inline void ap_lincons0_ref_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e);
static inline void ap_lincons0_ref_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e);
  /* Returns a reference on the argument under the form of ap_lincons0_ref.
     INTERNAL USE.
     BE CAUTIOUS: memory is shared between the result and the argument, and
     memory should be managed through the argument. */

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_lincons0_set(ap_lincons0_t res, ap_lincons0_t e, numinternal_t intern);

bool ap_lincons0_set_itvD_lincons(ap_lincons0_t a, itvD_lincons_t b, numinternal_t intern);
bool ap_lincons0_set_itvMPQ_lincons(ap_lincons0_t a, itvMPQ_lincons_t b, numinternal_t intern);
bool ap_lincons0_set_itvMPFR_lincons(ap_lincons0_t a, itvMPFR_lincons_t b, numinternal_t intern);

bool itvD_lincons_set_ap_lincons0(itvD_lincons_t a, ap_lincons0_t b, numinternal_t intern);
bool itvDMPQ_lincons_set_ap_lincons0(itvMPQ_lincons_t a, ap_lincons0_t b, numinternal_t intern);
bool itvMPFR_lincons_set_ap_lincons0(itvMPFR_lincons_t a, ap_lincons0_t b, numinternal_t intern);

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

void ap_lincons0_linexpr0ref(ap_linexpr0_t e, ap_lincons0_t c);
itvconstyp_t* ap_lincons0_constypref(ap_lincons0_t c);
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c);

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_lincons0_add_dimensions(ap_lincons0_t a,
				ap_lincons0_t b,
				ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_lincons0_permute_dimensions(ap_lincons0_t a,
				    ap_lincons0_t b,
				    ap_dimperm_t* perm);

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_lincons0_hash(ap_lincons0_t expr);
bool ap_lincons0_equal(ap_lincons0_t expr1,
		       ap_lincons0_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_lincons0_compare(ap_lincons0_t expr1,
			ap_lincons0_t expr2);

static inline void ap_lincons0_init_cons_D(ap_lincons0_t res, itvD_lincons_t e)
{ res->ref = false; res->discr = AP_SCALAR_D; res->lincons.D = e; };
static inline void ap_lincons0_init_cons_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPQ; res->lincons.MPQ = e; };
static inline void ap_lincons0_init_cons_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e)
{ res->ref = false; res->discr = AP_SCALAR_MPFR; res->lincons.MPFR = e; };

static inline void ap_lincons0_ref_D(ap_lincons0_t res, itvD_lincons_t e)
{ res->ref = true; res->discr = AP_SCALAR_D; res->lincons.D = e; };
static inline void ap_lincons0_ref_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPQ; res->lincons.MPQ = e; };
static inline void ap_lincons0_ref_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e)
{ res->ref = true; res->discr = AP_SCALAR_MPFR; res->lincons.MPFR = e; };

#ifdef __cplusplus
}
#endif

#endif