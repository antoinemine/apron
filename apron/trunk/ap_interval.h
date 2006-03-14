/* ************************************************************************* */
/* ap_interval.h: intervals */
/* ************************************************************************* */

#ifndef _AP_INTERVAL_H_
#define _AP_INTERVAL_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>

#include "ap_config.h"
#include "ap_scalar.h"

typedef struct ap_interval_t {
  ap_scalar_t* inf;
  ap_scalar_t* sup;
} ap_interval_t;

/* ====================================================================== */
/* Basics */
/* ====================================================================== */
ap_interval_t* ap_interval_alloc();
  /* Initialization, using DOUBLE as default type for scalars */
void ap_interval_reinit(ap_interval_t* interval, ap_scalar_discr_t ap_scalar_discr);
  /* Change the type of scalars */
void ap_interval_free(ap_interval_t* interval);
  /* Free an interval */
void ap_interval_fprint(FILE* stream, const ap_interval_t* a);
  /* Printing */
static inline
void ap_interval_swap(ap_interval_t* a, ap_interval_t* b)
{ ap_interval_t t = *a; *a = *b; *b = t; }
  /* Exchange */

/* ====================================================================== */
/* Assignements */
/* ====================================================================== */

void ap_interval_set(ap_interval_t* interval, const ap_interval_t* interval2);
  /* Assignement */
void ap_interval_set_scalar(ap_interval_t* interval, const ap_scalar_t* inf, const ap_scalar_t* sup);
void ap_interval_set_mpq(ap_interval_t* interval, const mpq_t inf, const mpq_t sup);
void ap_interval_set_int(ap_interval_t* interval, int inf, int sup);
void ap_interval_set_frac(ap_interval_t* interval, int numinf, unsigned int deninf, int numsup, unsigned int densup);
void ap_interval_set_double(ap_interval_t* interval, double inf, double sup);
  /* Assignement from resp.
     - two scalars
     - two rationals of type MPQ
     - two integers, giving [inf,dup]
     - two rationals, giving [numinf/deninf,numsup/densup]
     - two double values
  */
void ap_interval_set_top(ap_interval_t* interval);
  /* Assignement to universe interval [-oo,oo],
     does not change the type of scalars */
void ap_interval_set_bottom(ap_interval_t* interval);
  /* Assignement to empty interval [1,-1],
     does not change the type of scalars */

/* ====================================================================== */
/* Combined allocation and assignements */
/* ====================================================================== */

ap_interval_t* ap_interval_alloc_set(const ap_interval_t* interval);
  /* Assignement */

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

bool ap_interval_is_top(const ap_interval_t* interval);
  /* Is it the universe interval ? */
bool ap_interval_is_bottom(const ap_interval_t* interval);
  /* Is it an empty interval ? */
bool ap_interval_is_leq(const ap_interval_t* i1, const ap_interval_t* i2);
  /* Inclusion test */
int ap_interval_cmp(const ap_interval_t* i1, const ap_interval_t* i2);
  /* Comparison:
     0: equality
     -1: i1 included in i2
     +1: i2 included in i1
     -2: i1.inf less than i2.inf
     +2: i1.inf greater than i2.inf
  */
bool ap_interval_equal(const ap_interval_t* i1, const ap_interval_t* i2);
  /* Equality */

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_interval_neg(ap_interval_t* a, const ap_interval_t* b);
  /* Negation */
int ap_interval_hash(const ap_interval_t* itv);
  /* Hash code */

/* ====================================================================== */
/* Array of intervals */
/* ====================================================================== */

ap_interval_t** ap_interval_array_alloc(size_t size);
  /* Allocating an array of intervals, initialized with [0,0] values */
void ap_interval_array_free(ap_interval_t** array, size_t size);
  /* Clearing and freeing an array of intervals */

#endif
