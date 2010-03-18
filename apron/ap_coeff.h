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

typedef struct ap_coeff_t {
  ap_scalar_discr_t discr; /* discriminant for coefficient */
  union {
    eitvD_t    D;
    eitvMPQ_t  MPQ;
    eitvMPFR_t MPFR;
  } eitv;
} ap_coeff_t;

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

static inline void ap_coeff_init(ap_coeff_t* coeff, ap_scalar_discr_t discr);
void ap_coeff_init_set(ap_coeff_t* coeff, ap_coeff_t* coeff2);
void ap_coeff_clear(ap_coeff_t* coeff);

void ap_coeff_reinit(ap_coeff_t* coeff, ap_scalar_discr_t ap_scalar_discr);
  /* Changing the type of scalar(s) and the type of the coefficient */

ap_coeff_t* ap_coeff_alloc(ap_scalar_discr_t discr);
  /* Initialization, specifying the type of the coefficient */
void ap_coeff_free(ap_coeff_t* a);
  /* Free a coefficient */

void ap_coeff_fprint(FILE* stream, ap_coeff_t* a);
static inline void ap_coeff_print(ap_coeff_t* a);

static inline void ap_coeff_swap(ap_coeff_t* a, ap_coeff_t* b)
{ ap_coeff_t t = *a; *a = *b; *b = t; }
  /* Exchange */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

void ap_coeff_set(ap_coeff_t* a, ap_coeff_t* b);
void ap_coeff_set_numMPQ(ap_coeff_t* coeff, numMPQ_t num);
void ap_coeff_set_numD(ap_coeff_t* coeff, numD_t num);
void ap_coeff_set_numMPFR(ap_coeff_t* coeff, numMPFR_t num);
void ap_coeff_set_lint(ap_coeff_t* coeff, long int num);
void ap_coeff_set_lfrac(ap_coeff_t* coeff, long int num, long int den);
void ap_coeff_set_llfrac(ap_coeff_t* coeff, long long int num, long long int den);
void ap_coeff_set_mpq(ap_coeff_t* coeff, mpq_t mpq);
void ap_coeff_set_double(ap_coeff_t* coeff, double num);
void ap_coeff_set_mpfr(ap_coeff_t* coeff, mpfr_t mpfr);
void ap_coeff_set_int(ap_coeff_t* coeff, int num);
  /* Assign a coefficient with resp.
     - a coeff
     - a num (giving its type)
     - an integer, converted to type MPQ (same as numIl)
     - a rational, converted to type MPQ (same as numRl)
     - a rational of type mpq_t, converted to type MPQ (same as numMPQ)
     - a double, converted to type DOUBLE (same as numD)
     - a MPFR, converted to type MPFR (same as numMPFR)
     - a (small) integer, converted to existing type (small, because it is
       assumed that the conversion is exact; typically, -1,0,1,...).
  */
void ap_coeff_set_eitvMPQ(ap_coeff_t* coeff, eitvMPQ_t eitv);
void ap_coeff_set_eitvD(ap_coeff_t* coeff, eitvD_t eitv);
void ap_coeff_set_eitvMPFR(ap_coeff_t* coeff, eitvMPFR_t eitv);
void ap_coeff_set_lfrac2(ap_coeff_t* coeff, 
			 long int numinf, long int deninf,
			 long int numsup, long int densup);
void ap_coeff_set_mpq2(ap_coeff_t* coeff, mpq_t inf, mpq_t sup);
void ap_coeff_set_double2(ap_coeff_t* coeff, double inf, double sup);
void ap_coeff_set_mpfr2(ap_coeff_t* coeff, mpfr_t inf, mpfr_t sup);
void ap_coeff_set_int2(ap_coeff_t* coeff, int inf, int sup);
void ap_coeff_set_top(ap_coeff_t* coeff);
  /* Assign a coefficient with resp.
     - an interval (giving its type)
     - an interval of integers, converted to type MPQ
     - an interval of rationals, converted to type MPQ
     - an interval of rationals of type MPQ
     - an interval of double, converted to type DOUBLE
     - an interval of MPFR, converted to type MPFR
     - a (small) integer, converted to existing type (small, because it is
       assumed that the conversion is exact; typically, -1,0,1,...).
     - a top interval (the type remains the same).
  */

bool eitvD_set_ap_coeff(eitvD_t eitv, ap_coeff_t* coeff, numinternal_t intern);
bool eitvMPQ_set_ap_coeff(eitvMPQ_t eitv, ap_coeff_t* coeff, numinternal_t intern);
bool eitvMPFR_set_ap_coeff(eitvMPFR_t eitv, ap_coeff_t* coeff, numinternal_t intern);

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

bool ap_coeff_zero(ap_coeff_t* coeff);
  /* Return true iff coeff is zero */
bool ap_coeff_equal(ap_coeff_t* a, ap_coeff_t* b);
  /* Return true iff equality */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

long ap_coeff_hash(ap_coeff_t* coeff);
  /* Hash code */

/* ====================================================================== */
/* FOR INTERNAL USE ONLY */
/* ====================================================================== */

static inline void ap_coeff_print(ap_coeff_t* a)
{ ap_coeff_fprint(stdout,a); }
  /* Printing */

static inline void ap_coeff_init(ap_coeff_t* a, ap_scalar_discr_t discr)
{
  a->discr = discr;
  switch(discr){
  case AP_SCALAR_D:
    eitvD_init(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_init(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_init(a->eitv.MPFR);
    break;
  }
}


#ifdef __cplusplus
}
#endif

#endif
