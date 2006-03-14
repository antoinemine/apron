/* ************************************************************************* */
/* lincons0.h: linear constraints and arrays */
/* ************************************************************************* */

/* normally included from expr0.h */

#ifndef _LINCONS0_H_
#define _LINCONS0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "coeff.h"
#include "linexpr0.h"

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for type of constraints */
typedef enum constyp_t {
  CONS_EQ,    /* equality constraint */
  CONS_SUPEQ, /* >= constraint */
  CONS_SUP   /* > constraint */
} constyp_t;

/* Represents the constraint "expr constyp 0" */
typedef struct lincons0_t {
  linexpr0_t* linexpr0;   /* expression */
  constyp_t constyp; /* type of constraint */
} lincons0_t;

/* Array of constraints */
typedef struct lincons0_array_t {
  lincons0_t* p;
  size_t size;
} lincons0_array_t;

/* ********************************************************************** */
/* I. lincons0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
lincons0_t lincons0_make(constyp_t constyp, linexpr0_t* linexpr);
  /* Create a constraint of given type with the given expression.
     The expression is not duplicated, just pointed to */

lincons0_t lincons0_make_unsat();
  /* Create the constraint -1>=0 */

static inline
lincons0_t lincons0_copy(const lincons0_t* cons);
  /* Duplication */

static inline
void lincons0_clear(lincons0_t* cons);
  /* Free the linear expression of the constraint and set pointer to NULL */

void lincons0_fprint(FILE* stream,
                     const lincons0_t* cons, char** name_of_dim);
  /* Printing a linear constraint */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

bool lincons0_is_unsat(const lincons0_t* cons);
  /* True if the constraint is b>=0 or [a,b]>=0 with b negative */


/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void lincons0_add_dimensions_with(lincons0_t* cons,
				  const dimchange_t* dimchange);
static inline
lincons0_t lincons0_add_dimensions(const lincons0_t* cons,
				   const dimchange_t* dimchange);

static inline
void lincons0_permute_dimensions_with(lincons0_t* cons,
				      const dim_t* perm);
static inline
lincons0_t lincons0_permute_dimensions(const lincons0_t* cons,
				       const dim_t* perm);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

lincons0_array_t lincons0_array_make(size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers, */

void lincons0_array_clear(lincons0_array_t* array);
  /* Clear the constraints of the array, and then the array itself */

void lincons0_array_fprint(FILE* stream,
                         const lincons0_array_t* lincons0_array,
                         char** name_of_dim);
  /* Printing */

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void lincons0_array_add_dimensions_with(lincons0_array_t* array,
					const dimchange_t* dimchange);
lincons0_array_t lincons0_array_add_dimensions(const lincons0_array_t* array,
					       const dimchange_t* dimchange);

void lincons0_array_permute_dimensions_with(lincons0_array_t* array,
					    const dim_t* perm);
lincons0_array_t lincons0_array_permute_dimensions(const lincons0_array_t* array,
						   const dim_t* perm);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline lincons0_t lincons0_make(constyp_t constyp, linexpr0_t* linexpr)
{
  lincons0_t cons;
  cons.constyp = constyp;
  cons.linexpr0 = linexpr;
  return cons;
}
static inline lincons0_t lincons0_copy(const lincons0_t* cons)
{
  return lincons0_make(cons->constyp, linexpr0_copy(cons->linexpr0));
}
static inline void lincons0_clear(lincons0_t* lincons)
{
  if (lincons->linexpr0){
    linexpr0_free(lincons->linexpr0);
  }
  lincons->linexpr0 = NULL;
}

static inline
void lincons0_add_dimensions_with(lincons0_t* cons,
				  const dimchange_t* dimchange)
{ linexpr0_add_dimensions_with(cons->linexpr0,dimchange); }
static inline
lincons0_t lincons0_add_dimensions(const lincons0_t* cons,
				   const dimchange_t* dimchange)
{
  return lincons0_make(cons->constyp,
		       linexpr0_add_dimensions(cons->linexpr0,dimchange));
}
static inline
void lincons0_permute_dimensions_with(lincons0_t* cons,
				      const dim_t* perm)
{ linexpr0_permute_dimensions_with(cons->linexpr0,perm); }
static inline
lincons0_t lincons0_permute_dimensions(const lincons0_t* cons,
				       const dim_t* perm)
{
  return lincons0_make(cons->constyp,
		       linexpr0_permute_dimensions(cons->linexpr0,perm));
}

#endif
