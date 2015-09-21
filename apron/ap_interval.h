/* ************************************************************************* */
/* ap_interval.h: intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_INTERVAL_H_
#define _AP_INTERVAL_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "ap_config.h"
#include "ap_scalar.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ap_interval_t {
  ap_scalar_t* inf;
  ap_scalar_t* sup;
} ap_interval_t;

/* ====================================================================== */
/* Basics */
/* ====================================================================== */
ap_interval_t* ap_interval_alloc(void);
  /* Initialization, using DOUBLE as default type for scalars */
void ap_interval_reinit(ap_interval_t* interval, ap_scalar_discr_t ap_scalar_discr);
  /* Change the type of scalars */
void ap_interval_free(ap_interval_t* interval);
  /* Free an interval */
void ap_interval_fprint(FILE* stream, ap_interval_t* a);
static inline 
void ap_interval_print(ap_interval_t* a)
{ ap_interval_fprint(stdout,a); }
  /* Printing */
static inline
void ap_interval_swap(ap_interval_t* a, ap_interval_t* b)
{ ap_interval_t t = *a; *a = *b; *b = t; }
  /* Exchange */

/* ====================================================================== */
/* Assignments */
/* ====================================================================== */

void ap_interval_set(ap_interval_t* interval, ap_interval_t* interval2);
  /* Assignment */
void ap_interval_set_scalar(ap_interval_t* interval, ap_scalar_t* inf, ap_scalar_t* sup);
void ap_interval_set_mpq(ap_interval_t* interval, mpq_t inf, mpq_t sup);
void ap_interval_set_int(ap_interval_t* interval, long int inf, long int sup);
void ap_interval_set_frac(ap_interval_t* interval, long int numinf, unsigned long int deninf, long int numsup, unsigned long int densup);
void ap_interval_set_double(ap_interval_t* interval, double inf, double sup);
void ap_interval_set_mpfr(ap_interval_t* interval, mpfr_t inf, mpfr_t sup);
  /* Assignment from resp.
     - two scalars
     - two rationals of type MPQ
     - two integers, giving [inf,dup]
     - two rationals, giving [numinf/deninf,numsup/densup]
     - two double values
     - two MPFR floating-point numbers
  */
void ap_interval_set_top(ap_interval_t* interval);
  /* Assignment to universe interval [-oo,oo],
     does not change the type of scalars */
void ap_interval_set_bottom(ap_interval_t* interval);
  /* Assignment to empty interval [1,-1],
     does not change the type of scalars */

/* ====================================================================== */
/* Combined allocation and assignments */
/* ====================================================================== */

ap_interval_t* ap_interval_alloc_set(ap_interval_t* interval);
  /* Assignment */

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

bool ap_interval_is_top(ap_interval_t* interval);
  /* Is it the universe interval ? */
bool ap_interval_is_bottom(ap_interval_t* interval);
  /* Is it an empty interval ? */
bool ap_interval_is_leq(ap_interval_t* i1, ap_interval_t* i2);
  /* Inclusion test */
int ap_interval_cmp(ap_interval_t* i1, ap_interval_t* i2);
  /* Comparison:
     0: equality
     -1: i1 included in i2
     +1: i2 included in i1
     -2: i1.inf less than i2.inf
     +2: i1.inf greater than i2.inf
  */
bool ap_interval_equal(ap_interval_t* i1, ap_interval_t* i2);
bool ap_interval_equal_int(ap_interval_t* i, int b);
  /* Equality */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_interval_neg(ap_interval_t* a, ap_interval_t* b);
  /* Negation */
long ap_interval_hash(ap_interval_t* itv);
  /* Hash code */

/* ====================================================================== */
/* Array of intervals */
/* ====================================================================== */

ap_interval_t** ap_interval_array_alloc(size_t size);
  /* Allocating an array of intervals, initialized with [0,0] values */
void ap_interval_array_free(ap_interval_t** array, size_t size);
  /* Clearing and freeing an array of intervals */

#ifdef __cplusplus
}
#endif

#endif
