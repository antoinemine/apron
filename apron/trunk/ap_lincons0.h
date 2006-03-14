/* ************************************************************************* */
/* ap_lincons0.h: linear constraints and arrays */
/* ************************************************************************* */

/* normally included from expr0.h */

#ifndef _AP_LINCONS0_H_
#define _AP_LINCONS0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_linexpr0.h"

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for type of constraints */
typedef enum ap_constyp_t {
  AP_CONS_EQ,    /* equality constraint */
  AP_CONS_SUPEQ, /* >= constraint */
  AP_CONS_SUP   /* > constraint */
} ap_constyp_t;

/* Represents the constraint "expr constyp 0" */
typedef struct ap_lincons0_t {
  ap_linexpr0_t* linexpr0;   /* expression */
  ap_constyp_t constyp; /* type of constraint */
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
ap_lincons0_t ap_lincons0_make(ap_constyp_t constyp, ap_linexpr0_t* linexpr);
  /* Create a constraint of given type with the given expression.
     The expression is not duplicated, just pointed to */

ap_lincons0_t ap_lincons0_make_unsat();
  /* Create the constraint -1>=0 */

static inline
ap_lincons0_t ap_lincons0_copy(const ap_lincons0_t* cons);
  /* Duplication */

static inline
void ap_lincons0_clear(ap_lincons0_t* cons);
  /* Free the linear expression of the constraint and set pointer to NULL */

void ap_lincons0_fprint(FILE* stream,
                     const ap_lincons0_t* cons, char** name_of_dim);
  /* Printing a linear constraint */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

bool ap_lincons0_is_unsat(const ap_lincons0_t* cons);
  /* True if the constraint is b>=0 or [a,b]>=0 with b negative */


/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void ap_lincons0_add_dimensions_with(ap_lincons0_t* cons,
				  const ap_dimchange_t* dimchange);
static inline
ap_lincons0_t ap_lincons0_add_dimensions(const ap_lincons0_t* cons,
				   const ap_dimchange_t* dimchange);

static inline
void ap_lincons0_permute_dimensions_with(ap_lincons0_t* cons,
				      const ap_dimperm_t* perm);
static inline
ap_lincons0_t ap_lincons0_permute_dimensions(const ap_lincons0_t* cons,
				       const ap_dimperm_t* perm);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_lincons0_array_t ap_lincons0_array_make(size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers, */

void ap_lincons0_array_clear(ap_lincons0_array_t* array);
  /* Clear the constraints of the array, and then the array itself */

void ap_lincons0_array_fprint(FILE* stream,
                         const ap_lincons0_array_t* ap_lincons0_array,
                         char** name_of_dim);
  /* Printing */

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_lincons0_array_add_dimensions_with(ap_lincons0_array_t* array,
					const ap_dimchange_t* dimchange);
ap_lincons0_array_t ap_lincons0_array_add_dimensions(const ap_lincons0_array_t* array,
					       const ap_dimchange_t* dimchange);

void ap_lincons0_array_permute_dimensions_with(ap_lincons0_array_t* array,
					    const ap_dimperm_t* perm);
ap_lincons0_array_t ap_lincons0_array_permute_dimensions(const ap_lincons0_array_t* array,
						   const ap_dimperm_t* perm);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline ap_lincons0_t ap_lincons0_make(ap_constyp_t constyp, ap_linexpr0_t* linexpr)
{
  ap_lincons0_t cons;
  cons.constyp = constyp;
  cons.linexpr0 = linexpr;
  return cons;
}
static inline ap_lincons0_t ap_lincons0_copy(const ap_lincons0_t* cons)
{
  return ap_lincons0_make(cons->constyp, ap_linexpr0_copy(cons->linexpr0));
}
static inline void ap_lincons0_clear(ap_lincons0_t* lincons)
{
  if (lincons->linexpr0){
    ap_linexpr0_free(lincons->linexpr0);
  }
  lincons->linexpr0 = NULL;
}

static inline
void ap_lincons0_add_dimensions_with(ap_lincons0_t* cons,
				  const ap_dimchange_t* dimchange)
{ ap_linexpr0_add_dimensions_with(cons->linexpr0,dimchange); }
static inline
ap_lincons0_t ap_lincons0_add_dimensions(const ap_lincons0_t* cons,
				   const ap_dimchange_t* dimchange)
{
  return ap_lincons0_make(cons->constyp,
		       ap_linexpr0_add_dimensions(cons->linexpr0,dimchange));
}
static inline
void ap_lincons0_permute_dimensions_with(ap_lincons0_t* cons,
					 const ap_dimperm_t* perm)
{ ap_linexpr0_permute_dimensions_with(cons->linexpr0,perm); }
static inline
ap_lincons0_t ap_lincons0_permute_dimensions(const ap_lincons0_t* cons,
					     const ap_dimperm_t* perm)
{
  return ap_lincons0_make(cons->constyp,
		       ap_linexpr0_permute_dimensions(cons->linexpr0,perm));
}

#endif
