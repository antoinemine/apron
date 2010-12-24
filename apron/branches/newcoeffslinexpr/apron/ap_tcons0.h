/* ************************************************************************* */
/* ap_tcons0.h: tree expressions constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_TCONS0_H_
#define _AP_TCONS0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_texpr0.h"
#include "ap_lincons0.h"
#include "ap_lincons0_array.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Datatypes */
/* ====================================================================== */

/* Represents the constraint "expr constyp 0" */
typedef struct ap_tcons0_t {
  ap_texpr0_t* texpr0;  /* expression */
  ap_constyp_t constyp; /* type of constraint */
  mpq_t mpq;  /* For EQMOD constraint, indicates the
		 modulo */
} ap_tcons0_t;

/* Array of constraints */
typedef struct ap_tcons0_array_t {
  ap_tcons0_t** p;
  size_t size;
} ap_tcons0_array_t;

/* ********************************************************************** */
/* I. ap_tcons0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
ap_tcons0_t* ap_tcons0_make(ap_texpr0_t* texpr,
			    ap_constyp_t constyp,
			    mpq_t mpq);
  /* Create a constraint of given type with the given expression.
     The expression is not duplicated, just pointed to */

ap_tcons0_t* ap_tcons0_make_unsat(void);
  /* Create the constraint -1>=0 */

ap_tcons0_t* ap_tcons0_from_lincons0(ap_lincons0_t cons);
  /* From linear constraint to comb-like tree expression constraint  */

static inline
ap_tcons0_t* ap_tcons0_copy(ap_tcons0_t* cons);
  /* Duplication */

static inline
void ap_tcons0_free(ap_tcons0_t* cons);
  /* Free the linear expression of the constraint and set pointer to NULL */

void ap_tcons0_fprint(FILE* stream,
		      ap_tcons0_t* cons, char** name_of_dim);
  /* Printing a linear constraint */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

static inline
bool ap_tcons0_is_interval_cst(ap_tcons0_t* a);
  /* no-variable, only constant leaves */

static inline
bool ap_tcons0_is_interval_linear(ap_tcons0_t* a);
  /* linear with possibly interval coefficients, no rounding */

static inline
bool ap_tcons0_is_interval_polynomial(ap_tcons0_t* a);
  /* polynomial with possibly interval coefficients, no rounding  */

static inline
bool ap_tcons0_is_interval_polyfrac(ap_tcons0_t* a);
  /* polynomial fraction with possibly interval coefficients, no rounding */

static inline
bool ap_tcons0_is_linear(ap_tcons0_t* a);
  /* all coefficients are scalar (non-interval) */

/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void ap_tcons0_add_dimensions_with(ap_tcons0_t* cons,
				   ap_dimchange_t* dimchange);
static inline
ap_tcons0_t* ap_tcons0_add_dimensions(ap_tcons0_t* cons,
				     ap_dimchange_t* dimchange);

static inline
void ap_tcons0_permute_dimensions_with(ap_tcons0_t* cons,
				       ap_dimperm_t* perm);
static inline
ap_tcons0_t* ap_tcons0_permute_dimensions(ap_tcons0_t* cons,
					 ap_dimperm_t* perm);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_tcons0_array_t ap_tcons0_array_make(size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers, */

void ap_tcons0_array_resize(ap_tcons0_array_t* array, size_t size);
  /* Resize an array of size constraints.
     New constraints are initialized with NULL pointers,
     Removed constraints with non-NULL pointers are deallocated */

void ap_tcons0_array_clear(ap_tcons0_array_t* array);
  /* Clear the constraints of the array, and then the array itself */

ap_tcons0_array_t ap_tcons0_array_from_lincons0_array(ap_lincons0_array_t e);
  /* From linear constraints to comb-like constraint trees */

void ap_tcons0_array_fprint(FILE* stream,
			      ap_tcons0_array_t* ap_tcons0_array,
			      char** name_of_dim);
void ap_tcons0_array_print(ap_tcons0_array_t* ap_tcons0_array,
			   char** name_of_dim);
  /* Printing */

bool ap_tcons0_array_is_interval_linear(ap_tcons0_array_t* array);
bool ap_tcons0_array_is_interval_polynomial(ap_tcons0_array_t* array);
bool ap_tcons0_array_is_interval_polyfrac(ap_tcons0_array_t* array);
bool ap_tcons0_array_is_scalar(ap_tcons0_array_t* array);

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */

void ap_tcons0_array_add_dimensions_with(ap_tcons0_array_t* array,
					 ap_dimchange_t* dimchange);
ap_tcons0_array_t ap_tcons0_array_add_dimensions(ap_tcons0_array_t* array,
						 ap_dimchange_t* dimchange);

void ap_tcons0_array_permute_dimensions_with(ap_tcons0_array_t* array,
					     ap_dimperm_t* perm);
ap_tcons0_array_t ap_tcons0_array_permute_dimensions(ap_tcons0_array_t* array,
						     ap_dimperm_t* perm);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline ap_tcons0_t* ap_tcons0_make(ap_texpr0_t* texpr, ap_constyp_t constyp, mpq_t mpq)
{
  ap_tcons0_t* cons = (ap_tcons0_t*)malloc(sizeof(ap_tcons0_t));
  cons->texpr0 = texpr;
  cons->constyp = constyp;
  mpq_init(cons->mpq);
  if (mpq) mpq_set(cons->mpq,mpq);
  return cons;
}

static inline ap_tcons0_t* ap_tcons0_copy(ap_tcons0_t* cons)
{
  return ap_tcons0_make(ap_texpr0_copy(cons->texpr0),
			cons->constyp,
			cons->mpq);
}
static inline void ap_tcons0_free(ap_tcons0_t* tcons)
{
  if (tcons){
    if (tcons->texpr0){
      ap_texpr0_free(tcons->texpr0);
      tcons->texpr0 = NULL;
    }
    mpq_clear(tcons->mpq);
    free(tcons);
  }
}

static inline
bool ap_tcons0_is_interval_cst(ap_tcons0_t* a)
{ return ap_texpr0_is_interval_cst(a->texpr0); }
static inline
bool ap_tcons0_is_interval_linear(ap_tcons0_t* a)
{ return ap_texpr0_is_interval_linear(a->texpr0); }
static inline
bool ap_tcons0_is_interval_polynomial(ap_tcons0_t* a)
{ return ap_texpr0_is_interval_polynomial(a->texpr0); }
static inline
bool ap_tcons0_is_interval_polyfrac(ap_tcons0_t* a)
{ return ap_texpr0_is_interval_polyfrac(a->texpr0); }
static inline
bool ap_tcons0_is_scalar(ap_tcons0_t* a)
{ return ap_texpr0_is_scalar(a->texpr0); }

static inline
void ap_tcons0_add_dimensions_with(ap_tcons0_t* cons,
				   ap_dimchange_t* dimchange)
{ ap_texpr0_add_dimensions_with(cons->texpr0,dimchange); }
static inline
ap_tcons0_t* ap_tcons0_add_dimensions(ap_tcons0_t* cons,
				     ap_dimchange_t* dimchange)
{
  return ap_tcons0_make(ap_texpr0_add_dimensions(cons->texpr0,dimchange),
			cons->constyp,
			cons->mpq);
}
static inline
void ap_tcons0_permute_dimensions_with(ap_tcons0_t* cons,
				       ap_dimperm_t* perm)
{ ap_texpr0_permute_dimensions_with(cons->texpr0,perm); }
static inline
ap_tcons0_t* ap_tcons0_permute_dimensions(ap_tcons0_t* cons,
					 ap_dimperm_t* perm)
{
  return ap_tcons0_make(ap_texpr0_permute_dimensions(cons->texpr0,perm),
			cons->constyp,
			cons->mpq);
}

#ifdef __cplusplus
}
#endif

#endif
