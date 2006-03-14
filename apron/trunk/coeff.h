#line 9 "/udd/bjeannet/dev/commoninterface/coeff.nw"
/* ************************************************************************* */
/* coeff.h: coefficients and intervals */
/* ************************************************************************* */

/* GENERATED FROM coeff.nw: DOT NOT MODIFY ! */

#ifndef _COEFF_H_
#define _COEFF_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>

#ifndef HAS_BOOL
#define HAS_BOOL
typedef enum bool {
  false=0,
  true=1
} bool;
#endif


#line 114 "/udd/bjeannet/dev/commoninterface/coeff.nw"
/* ********************************************************************** */
/* I. Scalars  */
/* ********************************************************************** */

typedef enum scalar_discr_t {
  SCALAR_DOUBLE, /* flottant avec double */
  SCALAR_MPQ,   /* rationnel avec multiprécision de GMP */
} scalar_discr_t;

typedef struct scalar_t {
  scalar_discr_t discr;
  union {
    mpq_ptr mpq; /* +infty coded by 1/0, -infty coded by -1/0 */
    double dbl;
  } val;
} scalar_t;

/* follows all the necessary operations on numbers of type scalar_t */

scalar_t* scalar_alloc();
  /* Allocates a scalar, of default type DOUBLE (the most economical) */ 
void scalar_free(scalar_t* scalar);
  /* Free a scalar */
void scalar_reinit(scalar_t* scalar, scalar_discr_t d);
  /* Change the type of an already allocated scalar 
     (mainly for internal use */

/* Assignements */

void scalar_set(scalar_t* scalar, const scalar_t* scalar2);
  /* Assignement */
void scalar_set_mpq(scalar_t* scalar, const mpq_t mpq);
void scalar_set_int(scalar_t* scalar, long int i);
void scalar_set_frac(scalar_t* scalar, long int i, unsigned long int j);
  /* Change the type of scalar to MPQ and initialize it resp. with
     - mpq
     - integer i
     - rational i/j, assuming j!=0 
  */ 
void scalar_set_double(scalar_t* scalar, double k);
  /* Change the type of scalar to DOUBLE and initialize it with k. */
void scalar_set_infty(scalar_t* scalar, int sgn);
  /* Assignement to sgn*infty. Keep the type of the scalar.
     If sgn == 0, set to zero. */

/* Combined allocation and assignement */
scalar_t* scalar_alloc_set(const scalar_t* scalar2);
scalar_t* scalar_alloc_set_mpq(const mpq_t mpq);
  /* Change the type of scalar to MPQ and initialize it withmpq */ 
scalar_t* scalar_alloc_double(double k);
  /* Change the type of scalar to DOUBLE and initialize it with k. */

/* Conversions */

void mpq_set_scalar(mpq_t mpq, const scalar_t* scalar, int round);
  /* Assign mpq with the value of scalar, 
     possibly converting from DOUBLE type. 
     round currently unused. */ 
double scalar_get_double(const scalar_t* scalar, int round);
  /* Return the value of scalar in DOUBLE type, 
     possibly converting from MPQ type.
     Conversion may be not exact.
     round currently unused. */ 

/* Tests */

int scalar_infty(const scalar_t* scalar);
  /* -1:-infty, 0:finite; 1:+infty */
int scalar_cmp(const scalar_t* a, const scalar_t* b);
  /* Exact comparison between two scalars.
     -1: a is less than b
     0: a is equal to b
     1: a is greater than b
  */
bool scalar_equal(const scalar_t* a, const scalar_t* b);
  /* Exact Equality test */
int scalar_sgn(const scalar_t* a);
  /* -1: negative, 0: null, +1: positive  */

/* Other operations */

void scalar_neg(scalar_t* a, const scalar_t* b);
  /* Negation */
void scalar_inv(scalar_t* a, const scalar_t* b);
  /* Inversion. Not exact for floating-point type */
static inline
void scalar_swap(scalar_t* a, scalar_t* b){ scalar_t t = *a; *a = *b; *b = t; }
  /* Exchange */

int scalar_hash(const scalar_t* a);
  /* Return an hash code (for instance for OCaml interface) */
void scalar_fprint(FILE* stream, const scalar_t* a);
  /* Printing */

#line 241 "/udd/bjeannet/dev/commoninterface/coeff.nw"
/* ********************************************************************** */
/* II. Intervals  */
/* ********************************************************************** */

typedef struct interval_t {
  scalar_t* inf;
  scalar_t* sup;
} interval_t;
/* QUESTION: should we consider also open intervals ? */

interval_t* interval_alloc();
  /* Initialization, using DOUBLE as default type for scalars */ 
void interval_reinit(interval_t* interval, scalar_discr_t scalar_discr);
  /* Change the type of scalars */
void interval_free(interval_t* interval);
  /* Free an interval */

/* Assignements */

void interval_set(interval_t* interval, const interval_t* interval2);
  /* Assignement */ 
void interval_set_infsup(interval_t* interval, const scalar_t* inf, const scalar_t* sup);
void interval_set_infsup_int(interval_t* interval, int inf, int sup);
void interval_set_infsup_frac(interval_t* interval, int numinf, unsigned int deninf, int numsup, unsigned int densup);
void interval_set_infsup_double(interval_t* interval, double inf, double sup);
  /* Assignement from resp.
     - two scalars
     - two integers, giving [inf,dup]
     - two rationals, giving [numinf/deninf,numsup/densup]
     - two double values
  */
void interval_set_top(interval_t* interval);
  /* Assignement to universe interval [-oo,oo], 
     does not change the type of scalars */
void interval_set_bottom(interval_t* interval);
  /* Assignement to empty interval [1,-1],
     does not change the type of scalars */

/* Combined allocation and assignements */

interval_t* interval_alloc_set(const interval_t* interval);
  /* Assignement */ 
interval_t* interval_alloc_set_infsup(const scalar_t* inf, const scalar_t* sup);
interval_t* interval_alloc_set_infsup_int(int inf, int sup);
interval_t* interval_alloc_set_infsup_frac(int numinf, unsigned int deninf, int numsup, unsigned int densup);
interval_t* interval_alloc_set_infsup_double(double inf, double sup);
  /* Assignement from resp.
     - two scalars
     - two integers, giving [inf,dup]
     - two rationals, giving [numinf/deninf,numsup/densup]
     - two double values
  */

/* Tests */

bool interval_is_top(const interval_t* interval);
  /* Is it the universe interval ? */
bool interval_is_bottom(const interval_t* interval);
  /* Is it an empty interval ? */
bool interval_is_leq(const interval_t* i1, const interval_t* i2);
  /* Inclusion test */
int interval_cmp(const interval_t* i1, const interval_t* i2);
  /* Comparison:
     0: equality
     -1: i1 included in i2
     +1: i2 included in i1
     -2: i1.inf less than i2.inf
     +2: i1.inf greater than i2.inf
  */
bool interval_equal(const interval_t* i1, const interval_t* i2);
  /* Equality */

/* Other operations */

int interval_hash(const interval_t* itv);
  /* Hash code */
void interval_fprint(FILE* stream, const interval_t* a);
  /* Printing */

/* Array of intervals */
interval_t** interval_array_alloc(size_t size);
  /* Allocating an array of intervals, initialized with [0,0] values */ 
void interval_array_free(interval_t** array, size_t size);
  /* Clearing and freeing an array of intervals */ 

#line 335 "/udd/bjeannet/dev/commoninterface/coeff.nw"
/* ********************************************************************** */
/* III. Coefficients  */
/* ********************************************************************** */

typedef enum coeff_discr_t {
  COEFF_SCALAR,
  COEFF_INTERVAL
} coeff_discr_t;
  /* Discriminant for coefficients */

typedef struct coeff_t {
  coeff_discr_t discr; /* discriminant for coefficient */
  union {
    scalar_t* scalar;         /* cst (normal linear expression) */
    interval_t* interval;   /* interval (quasi-linear expression) */
  } val;
} coeff_t;

coeff_t* coeff_alloc(coeff_discr_t coeff_discr);
  /* Initialization, specifying the type of the coefficient */ 
void coeff_reinit(coeff_t* coeff, coeff_discr_t coeff_discr, scalar_discr_t scalar_discr);
  /* Changing the type of scalar(s) and the type of the coefficient */ 
void coeff_free(coeff_t* a);
  /* Free a coefficient */

void coeff_reduce(coeff_t* coeff);
  /* If the coefficient is an interval [a;a], convert it to a scalar */

/* Assignements */

void coeff_set(coeff_t* a, const coeff_t* b);
  /* Assignement */
void coeff_set_scalar(coeff_t* coeff, const scalar_t* scalar);
void coeff_set_scalar_int(coeff_t* coeff, int num);
void coeff_set_scalar_frac(coeff_t* coeff, int num, unsigned int den);
void coeff_set_scalar_double(coeff_t* coeff, double num);
  /* Assign a coefficient of type SCALAR, with resp.
     - a coeff
     - an integer, converted to type MPQ
     - a rational, converted to type MPQ
     - a double, converted to type DOUBLE
  */
void coeff_set_interval(coeff_t* coeff, const interval_t* itv);
void coeff_set_interval_infsup(coeff_t* coeff, const scalar_t* inf, const scalar_t* sup);
void coeff_set_interval_infsup_int(coeff_t* coeff, int inf, int sup);
void coeff_set_interval_infsup_frac(coeff_t* coeff, 
                                  int numinf, unsigned int deninf, 
                                  int numsup, unsigned int densup);
void coeff_set_interval_infsup_double(coeff_t* coeff, double inf, double sup);
  /* Assign a coefficient of type INTERVAL, with resp.
     - an interval of coeff
     - an interval of integers, converted to type MPQ
     - an interval of rationals, converted to type MPQ
     - an interval of double, converted to type DOUBLE
  */

/* Combined allocation and assignement */
coeff_t* coeff_alloc_set(const coeff_t* coeff);
coeff_t* coeff_alloc_set_scalar(const scalar_t* scalar);
coeff_t* coeff_alloc_set_interval(const interval_t* interval);

int coeff_cmp(const coeff_t* coeff1, const coeff_t* coeff2);
  /* Non Total Comparison:
     - If the 2 coefficients are both scalars, corresp. to scalar_cmp
     - If the 2 coefficients are both intervals, corresp. to interval_cmp
     - otherwise, -3 if the first is a scalar, 3 otherwise
  */
bool coeff_equal(const coeff_t* coeff1, const coeff_t* coeff2);
  /* Equality */

bool coeff_zero(const coeff_t* coeff);
  /* Return true iff coeff is a zero scalar or an interval with zero bounds */

int coeff_hash(const coeff_t* coeff);
  /* Hash code */
#line 414 "/udd/bjeannet/dev/commoninterface/coeff.nw"
/* FOR INTERNAL USE ONLY */
void coeff_init(coeff_t* coeff, coeff_discr_t coeff_discr);
void coeff_init_set(coeff_t* coeff, coeff_t* coeff2);
void coeff_clear(coeff_t* coeff);
#line 421 "/udd/bjeannet/dev/commoninterface/coeff.nw"
#endif
