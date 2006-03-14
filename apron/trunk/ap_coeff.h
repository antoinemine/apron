/* ************************************************************************* */
/* ap_coeff.h: coefficients, that are either scalars or intervals */
/* ************************************************************************* */

#ifndef _AP_COEFF_H_
#define _AP_COEFF_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>

#include "ap_config.h"
#include "ap_scalar.h"
#include "ap_interval.h"

typedef enum ap_coeff_discr_t {
  AP_COEFF_SCALAR,
  AP_COEFF_INTERVAL
} ap_coeff_discr_t;
  /* Discriminant for coefficients */

typedef struct ap_coeff_t {
  ap_coeff_discr_t discr; /* discriminant for coefficient */
  union {
    ap_scalar_t* scalar;         /* cst (normal linear expression) */
    ap_interval_t* interval;   /* interval (quasi-linear expression) */
  } val;
} ap_coeff_t;

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

ap_coeff_t* ap_coeff_alloc(ap_coeff_discr_t ap_coeff_discr);
  /* Initialization, specifying the type of the coefficient */
void ap_coeff_reinit(ap_coeff_t* coeff, ap_coeff_discr_t ap_coeff_discr, ap_scalar_discr_t ap_scalar_discr);
  /* Changing the type of scalar(s) and the type of the coefficient */
void ap_coeff_free(ap_coeff_t* a);
  /* Free a coefficient */

void ap_coeff_reduce(ap_coeff_t* coeff);
  /* If the coefficient is an interval [a;a], convert it to a scalar */
static inline
void ap_coeff_swap(ap_coeff_t* a, ap_coeff_t* b)
{ ap_coeff_t t = *a; *a = *b; *b = t; }
  /* Exchange */

/* ====================================================================== */
/* Assignements */
/* ====================================================================== */

void ap_coeff_set(ap_coeff_t* a, const ap_coeff_t* b);
  /* Assignement */
void ap_coeff_set_scalar(ap_coeff_t* coeff, const ap_scalar_t* scalar);
void ap_coeff_set_scalar_mpq(ap_coeff_t* coeff, const mpq_t mpq);
void ap_coeff_set_scalar_int(ap_coeff_t* coeff, int num);
void ap_coeff_set_scalar_frac(ap_coeff_t* coeff, int num, unsigned int den);
void ap_coeff_set_scalar_double(ap_coeff_t* coeff, double num);
  /* Assign a coefficient of type SCALAR, with resp.
     - a coeff
     - a rational of type mpq_t, converted to type MPQ
     - an integer, converted to type MPQ
     - a rational, converted to type MPQ
     - a double, converted to type DOUBLE
  */
void ap_coeff_set_interval(ap_coeff_t* coeff, const ap_interval_t* itv);
void ap_coeff_set_interval_scalar(ap_coeff_t* coeff, const ap_scalar_t* inf, const ap_scalar_t* sup);
void ap_coeff_set_interval_mpq(ap_coeff_t* coeff, const mpq_t inf, const mpq_t sup);
void ap_coeff_set_interval_int(ap_coeff_t* coeff, int inf, int sup);
void ap_coeff_set_interval_frac(ap_coeff_t* coeff,
                                  int numinf, unsigned int deninf,
                                  int numsup, unsigned int densup);
void ap_coeff_set_interval_double(ap_coeff_t* coeff, double inf, double sup);
  /* Assign a coefficient of type INTERVAL, with resp.
     - an interval of coeff
     - an interval of rationals of type MPQ
     - an interval of integers, converted to type MPQ
     - an interval of rationals, converted to type MPQ
     - an interval of double, converted to type DOUBLE
  */

/* ====================================================================== */
/* Combined allocation and assignement */
/* ====================================================================== */
ap_coeff_t* ap_coeff_alloc_set(const ap_coeff_t* coeff);
ap_coeff_t* ap_coeff_alloc_set_scalar(const ap_scalar_t* scalar);
ap_coeff_t* ap_coeff_alloc_set_interval(const ap_interval_t* interval);

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_coeff_cmp(const ap_coeff_t* coeff1, const ap_coeff_t* coeff2);
  /* Non Total Comparison:
     - If the 2 coefficients are both scalars, corresp. to ap_scalar_cmp
     - If the 2 coefficients are both intervals, corresp. to ap_interval_cmp
     - otherwise, -3 if the first is a scalar, 3 otherwise
  */
bool ap_coeff_equal(const ap_coeff_t* coeff1, const ap_coeff_t* coeff2);
  /* Equality */

bool ap_coeff_zero(const ap_coeff_t* coeff);
  /* Return true iff coeff is a zero scalar or an interval with zero bounds */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */
void ap_coeff_neg(ap_coeff_t* a, const ap_coeff_t* b);
  /* Negation */

int ap_coeff_hash(const ap_coeff_t* coeff);
  /* Hash code */

/* ====================================================================== */
/* FOR INTERNAL USE ONLY */
/* ====================================================================== */
void ap_coeff_init(ap_coeff_t* coeff, ap_coeff_discr_t ap_coeff_discr);
void ap_coeff_init_set(ap_coeff_t* coeff, const ap_coeff_t* coeff2);
void ap_coeff_clear(ap_coeff_t* coeff);

#endif
