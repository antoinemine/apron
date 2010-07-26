/* ************************************************************************* */
/* ap_coeff.h: coefficients, that are either scalars or intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _AP_COEFF_H_
#define _AP_COEFF_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "ap_config.h"
#include "eitvMPQ.h"
#include "eitvD.h"
#include "eitvMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ap_scalar_discr_t {
  AP_SCALAR_MPQ,    /* GMP arbitrary precision rational */
  AP_SCALAR_D,      /* double-precision floating-point number */
  AP_SCALAR_MPFR,   /* MPFR floating-point number */
} ap_scalar_discr_t;

typedef struct ap_coeff_struct {
  bool ref;         /* If true, is only a reference */
  ap_scalar_discr_t discr; /* discriminant for coefficient */
  union {
    eitvD_ptr    D;
    eitvMPQ_ptr  MPQ;
    eitvMPFR_ptr MPFR;
  } eitv;
} ap_coeff_struct;

typedef ap_coeff_struct ap_coeff_t[1];
typedef ap_coeff_struct* ap_coeff_ptr;

struct eitvIl_struct;
struct eitvIll_struct;
struct eitvMPZ_struct;
struct eitvRl_struct;
struct eitvRll_struct;
struct eitvDl_struct;

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

void ap_coeff_init(ap_coeff_t coeff, ap_scalar_discr_t discr);
void ap_coeff_init_set(ap_coeff_t coeff, ap_coeff_t coeff2);
void ap_coeff_init_set_D(ap_coeff_t coeff, eitvD_t eitv);
void ap_coeff_init_set_MPQ(ap_coeff_t coeff, eitvMPQ_t eitv);
void ap_coeff_init_set_MPFR(ap_coeff_t coeff, eitvMPFR_t eitv);
void ap_coeff_clear(ap_coeff_t coeff);
void ap_coeff_fprint(FILE* stream, ap_coeff_t a);
static inline void ap_coeff_print(ap_coeff_t a);

bool ap_coeff_set(ap_coeff_t a, ap_coeff_t b, numinternal_t intern);
void ap_coeff_set_int(ap_coeff_t a, int b);
void ap_coeff_set_bottom(ap_coeff_t coeff);
void ap_coeff_set_top(ap_coeff_t coeff);


static inline void ap_coeff_swap(ap_coeff_t a, ap_coeff_t b)
{ ap_coeff_struct t = *a; *a = *b; *b = t; }
  /* Exchange */

static inline void ap_coeff_ref_D(ap_coeff_t res, eitvD_t e);
static inline void ap_coeff_ref_MPQ(ap_coeff_t res, eitvMPQ_t e);
static inline void ap_coeff_ref_MPFR(ap_coeff_t res, eitvMPFR_t e);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_coeff_set_eitvIl(ap_coeff_t coeff, struct eitvIl_struct* eitv, numinternal_t intern);
bool ap_coeff_set_eitvIll(ap_coeff_t coeff, struct eitvIll_struct* eitv, numinternal_t intern);
bool ap_coeff_set_eitvMPZ(ap_coeff_t coeff, struct eitvMPZ_struct* eitv, numinternal_t intern);
bool ap_coeff_set_eitvRl(ap_coeff_t coeff, struct eitvRl_struct* eitv, numinternal_t intern);
bool ap_coeff_set_eitvRll(ap_coeff_t coeff, struct eitvRll_struct* eitv, numinternal_t intern);
bool ap_coeff_set_eitvDl(ap_coeff_t coeff, struct eitvDl_struct* eitv, numinternal_t intern);

bool ap_coeff_set_eitvD(ap_coeff_t coeff, eitvD_t eitv, numinternal_t intern);
bool ap_coeff_set_eitvMPQ(ap_coeff_t coeff, eitvMPQ_t eitv, numinternal_t intern);
bool ap_coeff_set_eitvMPFR(ap_coeff_t coeff, eitvMPFR_t, numinternal_t intern);

bool eitvIl_set_ap_coeff(struct eitvIl_struct* eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvIll_set_ap_coeff(struct eitvIll_struct* eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvMPZ_set_ap_coeff(struct eitvMPZ_struct* eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvRl_set_ap_coeff(struct eitvRl_struct* eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvRll_set_ap_coeff(struct eitvRll_struct* eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvDl_set_ap_coeff(struct eitvDl_struct* eitv, ap_coeff_t coeff, numinternal_t intern);

bool eitvD_set_ap_coeff(eitvD_t eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvMPQ_set_ap_coeff(eitvMPQ_t eitv, ap_coeff_t coeff, numinternal_t intern);
bool eitvMPFR_set_ap_coeff(eitvMPFR_t eitv, ap_coeff_t coeff, numinternal_t intern);
  /* Return true if conversion is exact */

/* ====================================================================== */
/* Tests */
/* ====================================================================== */
bool ap_coeff_is_point(ap_coeff_t a);
  /* Return true iff the interval is a single point */
bool ap_coeff_is_zero(ap_coeff_t a);
  /* Return true iff the interval is a single zero point */
bool ap_coeff_is_pos(ap_coeff_t a);
bool ap_coeff_is_neg(ap_coeff_t a);
  /* Included in [0;+oo], [-oo;0], or any of those */
bool ap_coeff_is_top(ap_coeff_t a);
  /* Return true iff the interval is resp. [-oo,+oo] */
bool ap_coeff_is_eq(ap_coeff_t a, ap_coeff_t b);

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

long ap_coeff_hash(ap_coeff_t coeff);
  /* Hash code */

/* ====================================================================== */
/* FOR INTERNAL USE ONLY */
/* ====================================================================== */

static inline void ap_coeff_print(ap_coeff_t a)
{ ap_coeff_fprint(stdout,a); }
  /* Printing */

static inline void ap_coeff_ref_D(ap_coeff_t res, eitvD_t e)
{ res->discr = AP_SCALAR_D; res->ref = true; res->eitv.D = e; };
static inline void ap_coeff_ref_MPQ(ap_coeff_t res, eitvMPQ_t e)
{ res->discr = AP_SCALAR_MPQ; res->ref = true; res->eitv.MPQ = e; };
static inline void ap_coeff_ref_MPFR(ap_coeff_t res, eitvMPFR_t e)
{ res->discr = AP_SCALAR_MPFR; res->ref = true; res->eitv.MPFR = e; };

#ifdef __cplusplus
}
#endif

#endif
