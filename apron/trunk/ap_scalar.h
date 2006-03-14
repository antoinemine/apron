/* ************************************************************************* */
/* ap_scalar.h: scalars */
/* ************************************************************************* */

#ifndef _AP_SCALAR_H_
#define _AP_SCALAR_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>

#include "ap_config.h"


/* ********************************************************************** */
/* I. Datatypes  */
/* ********************************************************************** */

typedef enum ap_scalar_discr_t {
  AP_SCALAR_DOUBLE, /* flottant avec double */
  AP_SCALAR_MPQ,   /* rationnel avec multiprécision de GMP */
} ap_scalar_discr_t;

typedef struct ap_scalar_t {
  ap_scalar_discr_t discr;
  union {
    double dbl;
    mpq_ptr mpq; /* +infty coded by 1/0, -infty coded by -1/0 */
  } val;
} ap_scalar_t;

/* ********************************************************************** */
/* II. Operations  */
/* ********************************************************************** */

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc();
  /* Allocates a scalar, of default type DOUBLE (the most economical) */
void ap_scalar_free(ap_scalar_t* scalar);
  /* Free a scalar */
void ap_scalar_reinit(ap_scalar_t* scalar, ap_scalar_discr_t d);
  /* Change the type of an already allocated scalar
     (mainly for internal use */
void ap_scalar_fprint(FILE* stream, const ap_scalar_t* a);
  /* Printing */

static inline
void ap_scalar_swap(ap_scalar_t* a, ap_scalar_t* b);
  /* Exchange */

/* ====================================================================== */
/* Assignements */
/* ====================================================================== */

void ap_scalar_set(ap_scalar_t* scalar, const ap_scalar_t* scalar2);
  /* Assignement */
void ap_scalar_set_mpq(ap_scalar_t* scalar, const mpq_t mpq);
void ap_scalar_set_int(ap_scalar_t* scalar, long int i);
void ap_scalar_set_frac(ap_scalar_t* scalar, long int i, unsigned long int j);
  /* Change the type of scalar to MPQ and initialize it resp. with
     - mpq
     - integer i
     - rational i/j, assuming j!=0
  */
void ap_scalar_set_double(ap_scalar_t* scalar, double k);
  /* Change the type of scalar to DOUBLE and initialize it with k. */
void ap_scalar_set_infty(ap_scalar_t* scalar, int sgn);
  /* Assignement to sgn*infty. Keep the type of the scalar.
     If sgn == 0, set to zero. */

/* ====================================================================== */
/* Combined allocation and assignement */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc_set(const ap_scalar_t* scalar2);
ap_scalar_t* ap_scalar_alloc_set_mpq(const mpq_t mpq);
  /* Change the type of scalar to MPQ and initialize it withmpq */
ap_scalar_t* ap_scalar_alloc_double(double k);
  /* Change the type of scalar to DOUBLE and initialize it with k. */

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

void ap_mpq_set_scalar(mpq_t mpq, const ap_scalar_t* scalar, int round);
  /* Assign mpq with the value of scalar,
     possibly converting from DOUBLE type.
     round currently unused. */
double ap_scalar_get_double(const ap_scalar_t* scalar, int round);
  /* Return the value of scalar in DOUBLE type,
     possibly converting from MPQ type.
     Conversion may be not exact.
     round currently unused. */

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_scalar_infty(const ap_scalar_t* scalar);
  /* -1:-infty, 0:finite; 1:+infty */
int ap_scalar_cmp(const ap_scalar_t* a, const ap_scalar_t* b);
int ap_scalar_cmp_int(const ap_scalar_t* a, int b);
  /* Exact comparison between two scalars (resp. a scalar and an integer)
     -1: a is less than b
     0: a is equal to b
     1: a is greater than b
  */
bool ap_scalar_equal(const ap_scalar_t* a, const ap_scalar_t* b);
bool ap_scalar_equal_int(const ap_scalar_t* a, int b);
  /* Exact Equality test */
int ap_scalar_sgn(const ap_scalar_t* a);
  /* -1: negative, 0: null, +1: positive  */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_scalar_neg(ap_scalar_t* a, const ap_scalar_t* b);
  /* Negation */
void ap_scalar_inv(ap_scalar_t* a, const ap_scalar_t* b);
  /* Inversion. Not exact for floating-point type */

int ap_scalar_hash(const ap_scalar_t* a);
  /* Return an hash code (for instance for OCaml interface) */

/* ********************************************************************** */
/* III. FOR INTERNAL USE ONLY */
/* ********************************************************************** */

static inline
void ap_scalar_init(ap_scalar_t* scalar, ap_scalar_discr_t d)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    scalar->val.mpq = malloc(sizeof(mpq_t));
    mpq_init(scalar->val.mpq);
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
  case AP_SCALAR_DOUBLE:
    break;
  }
}
static inline
void ap_scalar_swap(ap_scalar_t* a, ap_scalar_t* b){ ap_scalar_t t = *a; *a = *b; *b = t; }

#endif
