/* ************************************************************************* */
/* ap_scalar.h: scalars */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_SCALAR_H_
#define _AP_SCALAR_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gmp.h"
#include "mpfr.h"

#include "ap_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************** */
/* I. Datatypes  */
/* ********************************************************************** */

typedef enum ap_scalar_discr_t {
  AP_SCALAR_DOUBLE, /* double-precision floating-point number */
  AP_SCALAR_MPQ,    /* GMP arbitrary precision rational */
  AP_SCALAR_MPFR,   /* MPFR floating-point number */
} ap_scalar_discr_t;

typedef struct ap_scalar_t {
  ap_scalar_discr_t discr;
  union {
    double dbl;
    mpq_ptr mpq; /* +infty coded by 1/0, -infty coded by -1/0 */
    mpfr_ptr mpfr;
  } val;
} ap_scalar_t;

/* ********************************************************************** */
/* II. Operations  */
/* ********************************************************************** */

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc(void);
  /* Allocates a scalar, of default type DOUBLE (the most economical) */
void ap_scalar_free(ap_scalar_t* scalar);
  /* Free a scalar */
void ap_scalar_reinit(ap_scalar_t* scalar, ap_scalar_discr_t d);
  /* Change the type of an already allocated scalar
     (mainly for internal use */
static inline 
void ap_scalar_print(ap_scalar_t* a);
void ap_scalar_fprint(FILE* stream, ap_scalar_t* a);
  /* Printing */

extern int ap_scalar_print_prec;
  /* Number of significant digits to print for floating-point numbers.
     Defaults to 20.
   */

static inline
void ap_scalar_swap(ap_scalar_t* a, ap_scalar_t* b);
  /* Exchange */

/* ====================================================================== */
/* Assignments */
/* ====================================================================== */

void ap_scalar_set(ap_scalar_t* scalar, ap_scalar_t* scalar2);
  /* Assignment */
void ap_scalar_set_mpq(ap_scalar_t* scalar, mpq_t mpq);
void ap_scalar_set_int(ap_scalar_t* scalar, long int i);
void ap_scalar_set_frac(ap_scalar_t* scalar, long int i, unsigned long int j);
  /* Change the type of scalar to MPQ and initialize it resp. with
     - mpq
     - integer i
     - rational i/j, assuming j!=0
  */
void ap_scalar_set_double(ap_scalar_t* scalar, double k);
  /* Change the type of scalar to DOUBLE and initialize it with k. */
void ap_scalar_set_mpfr(ap_scalar_t* scalar, mpfr_t mpfr);
  /* Change the type of scalar to MPFR and initialize it with mpfr. 
     The precision of the scalar is changed to match that of mpfr. */
void ap_scalar_set_infty(ap_scalar_t* scalar, int sgn);
  /* Assignment to sgn*infty. Keep the type of the scalar.
     If sgn == 0, set to zero. */

/* ====================================================================== */
/* Combined allocation and assignment */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc_set(ap_scalar_t* scalar2);
ap_scalar_t* ap_scalar_alloc_set_mpq(mpq_t mpq);
  /* Allocate an MPQ scalar and initialize it with mpq */
ap_scalar_t* ap_scalar_alloc_set_double(double k);
  /* Allocate an DOUBLE scalar and initialize it with k. */
ap_scalar_t* ap_scalar_alloc_set_mpfr(mpfr_t mpfr);
  /* Allocate an MPFR scalar and initialize it with mpfr. 
     The precisio of the scalar matches that of mpfr. */

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* For the two next functions, the returned value is zero if conversion is
   exact, positive if the result is greater, negative if it is lower. */

int ap_mpq_set_scalar(mpq_t mpq, ap_scalar_t* scalar, mp_rnd_t round);
  /* Assign mpq with the value of scalar,
     possibly converting from another type.
     Currently, round is not needed, as the conversion is exact */
int ap_double_set_scalar(double* k, ap_scalar_t* scalar, mp_rnd_t round);
  /* Return the value of scalar in DOUBLE type,
     possibly converting from another type. */
int ap_mpfr_set_scalar(mpfr_t mpfr, ap_scalar_t* scalar, mp_rnd_t round);
  /* Return the value of scalar in MPFR type,
     possibly converting from another type. 
     The precision of mpfr is NOT changed to match that of scalar, so, 
     rounding may occur. */

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_scalar_infty(ap_scalar_t* scalar);
  /* -1:-infty, 0:finite; 1:+infty */
int ap_scalar_cmp(ap_scalar_t* a, ap_scalar_t* b);
int ap_scalar_cmp_int(ap_scalar_t* a, int b);
  /* Exact comparison between two scalars (resp. a scalar and an integer)
     -1: a is less than b
     0: a is equal to b
     1: a is greater than b
  */
bool ap_scalar_equal(ap_scalar_t* a, ap_scalar_t* b);
bool ap_scalar_equal_int(ap_scalar_t* a, int b);
  /* Exact Equality test */
int ap_scalar_sgn(ap_scalar_t* a);
  /* -1: negative, 0: null, +1: positive  */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_scalar_neg(ap_scalar_t* a, ap_scalar_t* b);
  /* Negation */
void ap_scalar_inv(ap_scalar_t* a, ap_scalar_t* b);
  /* Inversion. Not exact for floating-point type */

long ap_scalar_hash(ap_scalar_t* a);
  /* Return an hash code (for instance for OCaml interface) */

/* ********************************************************************** */
/* III. FOR INTERNAL USE ONLY */
/* ********************************************************************** */

static inline
void ap_scalar_init(ap_scalar_t* scalar, ap_scalar_discr_t d)
{
  scalar->discr = d;
  switch(d){
  case AP_SCALAR_MPQ:
    scalar->val.mpq = (mpq_ptr)malloc(sizeof(mpq_t));
    mpq_init(scalar->val.mpq);
    break;
  case AP_SCALAR_MPFR:
    scalar->val.mpfr = (mpfr_ptr)malloc(sizeof(mpfr_t));
    mpfr_init(scalar->val.mpfr);
    break;
  case AP_SCALAR_DOUBLE:
    scalar->val.dbl = 0.0;
    break;
  }
}
static inline
void ap_scalar_clear(ap_scalar_t* scalar)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    mpq_clear(scalar->val.mpq);
    free(scalar->val.mpq);
    break;
  case AP_SCALAR_MPFR:
    mpfr_clear(scalar->val.mpfr);
    free(scalar->val.mpfr);
    break;
  case AP_SCALAR_DOUBLE:
    break;
  }
}
static inline void ap_scalar_print(ap_scalar_t* a)
{ ap_scalar_fprint(stdout,a); }
static inline
void ap_scalar_swap(ap_scalar_t* a, ap_scalar_t* b){ ap_scalar_t t = *a; *a = *b; *b = t; }

#ifdef __cplusplus
}
#endif

#endif
