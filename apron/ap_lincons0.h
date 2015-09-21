/* ************************************************************************* */
/* ap_lincons0.h: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_LINCONS0_H_
#define _AP_LINCONS0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_linexpr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for type of constraints */
typedef enum ap_constyp_t {
  AP_CONS_EQ,    /* equality constraint */
  AP_CONS_SUPEQ, /* >= constraint */
  AP_CONS_SUP,   /* > constraint */
  AP_CONS_EQMOD, /* congruence equality constraint */
  AP_CONS_DISEQ  /* disequality constraint */
} ap_constyp_t;

/* Represents the constraint "expr constyp 0" */
typedef struct ap_lincons0_t {
  ap_linexpr0_t* linexpr0;  /* expression */
  ap_constyp_t constyp;     /* type of constraint */
  ap_scalar_t* scalar;      /* maybe NULL.  

			       For EQMOD constraint, indicates the
			       modulo */
} ap_lincons0_t;

/* Array of constraints */
typedef struct ap_lincons0_array_t {
  ap_lincons0_t* p;
  size_t size;
} ap_lincons0_array_t;

/* ********************************************************************** */
/* I. ap_lincons0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
ap_lincons0_t ap_lincons0_make(ap_constyp_t constyp, 
			       ap_linexpr0_t* linexpr,
			       ap_scalar_t* scalar);
  /* Create a constraint of given type with the given expression.
     The expression and the coefficient are not duplicated, just pointed to */

ap_lincons0_t ap_lincons0_make_unsat(void);
  /* Create the constraint -1>=0 */

static inline
ap_lincons0_t ap_lincons0_copy(ap_lincons0_t* cons);
  /* Duplication */

static inline
void ap_lincons0_clear(ap_lincons0_t* cons);
  /* Free the linear expression of the constraint and set pointer to NULL */

void ap_lincons0_print(ap_lincons0_t* cons, char** name_of_dim);
void ap_lincons0_fprint(FILE* stream,
			ap_lincons0_t* cons, char** name_of_dim);
  /* Printing a linear constraint */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

bool ap_lincons0_is_unsat(ap_lincons0_t* cons);
  /* True if the constraint is b>=0 or [a,b]>=0 with b negative */

bool ap_lincons0_is_sat(ap_lincons0_t* cons);
  /* True if the constraint is trivially satisfiable, e.g. [a,b]>=0 with a positive */

/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void ap_lincons0_add_dimensions_with(ap_lincons0_t* cons,
				     ap_dimchange_t* dimchange);
static inline
ap_lincons0_t ap_lincons0_add_dimensions(ap_lincons0_t* cons,
					 ap_dimchange_t* dimchange);

static inline
void ap_lincons0_permute_dimensions_with(ap_lincons0_t* cons,
					 ap_dimperm_t* perm);
static inline
ap_lincons0_t ap_lincons0_permute_dimensions(ap_lincons0_t* cons,
					     ap_dimperm_t* perm);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_lincons0_array_t ap_lincons0_array_make(size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers, */

void ap_lincons0_array_resize(ap_lincons0_array_t* array, size_t size);
  /* Resize an array of size constraints.
     New constraints are initialized with NULL pointers,
     Removed constraints with non-NULL pointers are deallocated */

void ap_lincons0_array_clear(ap_lincons0_array_t* array);
  /* Clear the constraints of the array, and then the array itself */

void ap_lincons0_array_print(ap_lincons0_array_t* ap_lincons0_array,
			     char** name_of_dim);
void ap_lincons0_array_fprint(FILE* stream,
			      ap_lincons0_array_t* ap_lincons0_array,
			      char** name_of_dim);
  /* Printing */

ap_linexpr_type_t ap_lincons0_array_type(ap_lincons0_array_t* array);
bool ap_lincons0_array_is_linear(ap_lincons0_array_t* array);
bool ap_lincons0_array_is_quasilinear(ap_lincons0_array_t* array);
  /* Are all the expressions involved linear (resp. quasilinear) */

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_lincons0_array_add_dimensions_with(ap_lincons0_array_t* array,
					   ap_dimchange_t* dimchange);
ap_lincons0_array_t ap_lincons0_array_add_dimensions(ap_lincons0_array_t* array,
						     ap_dimchange_t* dimchange);

void ap_lincons0_array_permute_dimensions_with(ap_lincons0_array_t* array,
					       ap_dimperm_t* perm);
ap_lincons0_array_t ap_lincons0_array_permute_dimensions(ap_lincons0_array_t* array,
							 ap_dimperm_t* perm);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline ap_lincons0_t ap_lincons0_make(ap_constyp_t constyp, ap_linexpr0_t* linexpr, ap_scalar_t* scalar)
{
  ap_lincons0_t cons;
  cons.constyp = constyp;
  cons.linexpr0 = linexpr;
  cons.scalar = scalar;
  return cons;
}
static inline ap_lincons0_t ap_lincons0_copy(ap_lincons0_t* cons)
{
  return ap_lincons0_make(cons->constyp, 
			  cons->linexpr0 ? ap_linexpr0_copy(cons->linexpr0) : NULL,
			  cons->scalar ? ap_scalar_alloc_set(cons->scalar) : NULL);
}
static inline void ap_lincons0_clear(ap_lincons0_t* lincons)
{
  if (lincons->linexpr0){
    ap_linexpr0_free(lincons->linexpr0);
  }
  lincons->linexpr0 = NULL;
  if (lincons->scalar){
    ap_scalar_free(lincons->scalar);
  }
  lincons->scalar = NULL;
}

static inline
void ap_lincons0_add_dimensions_with(ap_lincons0_t* cons,
				     ap_dimchange_t* dimchange)
{ ap_linexpr0_add_dimensions_with(cons->linexpr0,dimchange); }
static inline
ap_lincons0_t ap_lincons0_add_dimensions(ap_lincons0_t* cons,
					 ap_dimchange_t* dimchange)
{
  return ap_lincons0_make(cons->constyp,
			  ap_linexpr0_add_dimensions(cons->linexpr0,dimchange),
			  cons->scalar ? ap_scalar_alloc_set(cons->scalar) : NULL);
}
static inline
void ap_lincons0_permute_dimensions_with(ap_lincons0_t* cons,
					 ap_dimperm_t* perm)
{ ap_linexpr0_permute_dimensions_with(cons->linexpr0,perm); }
static inline
ap_lincons0_t ap_lincons0_permute_dimensions(ap_lincons0_t* cons,
					     ap_dimperm_t* perm)
{
  return ap_lincons0_make(cons->constyp,
			  ap_linexpr0_permute_dimensions(cons->linexpr0,perm),
			  cons->scalar ? ap_scalar_alloc_set(cons->scalar) : NULL);
}
  
#ifdef __cplusplus
}
#endif

#endif
