/* ************************************************************************* */
/* ap_lincons1.h: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_LINCONS1_H_
#define _AP_LINCONS1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_environment.h"
#include "ap_lincons0.h"
#include "ap_linexpr1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Constraints */
typedef struct ap_lincons1_t {
  ap_lincons0_t lincons0;
  ap_environment_t* env;
} ap_lincons1_t;

/* Array of constraints */
typedef struct ap_lincons1_array_t {
  ap_lincons0_array_t lincons0_array;
  ap_environment_t* env;
} ap_lincons1_array_t;


/* ********************************************************************** */
/* I. ap_lincons1_t */
/* ********************************************************************** */

/* For internal use */
static inline
ap_lincons1_t ap_lincons1_of_lincons0(ap_environment_t* env, ap_lincons0_t lincons0)
{
  ap_lincons1_t res;
  res.lincons0 = lincons0;
  res.env = ap_environment_copy(env);
  return res;
}


/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
ap_lincons1_t ap_lincons1_make(ap_constyp_t constyp,
			       ap_linexpr1_t* expr,
			       ap_scalar_t* scalar);
  /* Create a constraint of given type with the given expression.
     The expression and the optional coefficient are not duplicated,
     just pointed to. */

ap_lincons1_t ap_lincons1_make_unsat(ap_environment_t* env);
  /* Create the constraint -1>=0 */

static inline
ap_lincons1_t ap_lincons1_copy(ap_lincons1_t* cons);
  /* Duplication */

void ap_lincons1_clear(ap_lincons1_t* cons);
  /* Clear the constraint and set pointers to NULL */

void ap_lincons1_fprint(FILE* stream, ap_lincons1_t* cons);
  /* Printing */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

static inline
bool ap_lincons1_is_unsat(ap_lincons1_t* cons);
  /* True if the constraint is b>=0 or [a,b]>=0 with b negative */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

static inline
ap_environment_t* ap_lincons1_envref(ap_lincons1_t* cons);
  /* Get a reference to the environment. Do not free it. */

static inline
ap_constyp_t* ap_lincons1_constypref(ap_lincons1_t* cons);
  /* Get a reference to the type of constraint */

static inline
ap_scalar_t* ap_lincons1_scalarref(ap_lincons1_t* cons);
  /* Get a reference to the auxiliary coefficient of the constraint */

static inline
ap_linexpr1_t ap_lincons1_linexpr1ref(ap_lincons1_t* cons);
  /* Get a reference to the underlying expression of the constraint.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */

/* Get the constant and assign it to coeff */
static inline void ap_lincons1_get_cst(ap_coeff_t* coeff, ap_lincons1_t* cons);

/* Get coefficient of variable var in the constraint and assign it to coeff.
   Return true if var is unknown in the environment */
bool ap_lincons1_get_coeff(ap_coeff_t* coeff, ap_lincons1_t* cons, ap_var_t var);

/* Set the constant of the linear constraint */
static inline void ap_lincons1_set_cst(ap_lincons1_t* cons, ap_coeff_t* cst);

/* Set the coefficient of variable var in the constraint.
   Return true if var is unknown in the environment */
static inline bool ap_lincons1_set_coeff(ap_lincons1_t* cons, ap_var_t var, ap_coeff_t* coeff);

/*
bool ap_lincons1_set_format(ap_lincons1_t* cons, char* fmt, ...);
*/

bool ap_lincons1_set_list(ap_lincons1_t* cons, ...);

static inline
ap_coeff_t* ap_lincons1_cstref(ap_lincons1_t* cons);
  /* Get a reference to the constant. Do not free it. */

ap_coeff_t* ap_lincons1_coeffref(ap_lincons1_t* cons, ap_var_t var);
  /* Get a reference to the coefficient associated to the variable.
     Do not free it.
     In case of sparse representation,
     possibly induce the addition of a new linear term.
     Return NULL if var is unknown in the environment.
  */

static inline
ap_lincons0_t* ap_lincons1_lincons0ref(ap_lincons1_t* cons);
  /* Return underlying constraint of level 0.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */

/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_lincons1_extend_environment(ap_lincons1_t* ncons,
				 ap_lincons1_t* cons,
				 ap_environment_t* nenv);
bool ap_lincons1_extend_environment_with(ap_lincons1_t* cons,
				      ap_environment_t* nenv);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

ap_lincons1_array_t ap_lincons1_array_make(ap_environment_t* env, size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers,
     so that ap_lincons1_array_free may be safely called */
void ap_lincons1_array_clear(ap_lincons1_array_t* array);
  /* Clear the constraints of the array, and then the array itself */
void ap_lincons1_array_fprint(FILE* stream,
			   ap_lincons1_array_t* ap_lincons1_array);
  /* Printing */

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

static inline
size_t ap_lincons1_array_size(ap_lincons1_array_t* array);

static inline
ap_environment_t* ap_lincons1_array_envref(ap_lincons1_array_t* array);

static inline
void ap_lincons1_array_clear_index(ap_lincons1_array_t* array, size_t index);
  /* Clear the constraint at index index. */

ap_lincons1_t ap_lincons1_array_get(ap_lincons1_array_t* array,
			      size_t index);
  /* Return the linear constraint of the given index
     Nothing is duplicated, and the result should never be cleared.
     Modifying the argument or the result is equivalent, except for
     change of environments */

bool ap_lincons1_array_set(ap_lincons1_array_t* array,
			      size_t index, ap_lincons1_t* cons);
  /* Fill the index of the array with the constraint.
     Assumes ap_environment_is_eq(array->env,cons->env).
     Nothing is duplicated.
     The argument should never be cleared. (its environment is dereferenced).
     If a constraint was already stored, it is first cleared.
     Return true iff problem (index or array->env!=cons->env)
  */

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

bool
ap_lincons1_array_extend_environment_with(ap_lincons1_array_t* array,
					  ap_environment_t* env);
bool
ap_lincons1_array_extend_environment(ap_lincons1_array_t* narray,
				     ap_lincons1_array_t* array,
				     ap_environment_t* env);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline
ap_lincons1_t ap_lincons1_make(ap_constyp_t constyp,
			       ap_linexpr1_t* expr,
			       ap_scalar_t* scalar)
{
  ap_lincons1_t cons;
  cons.lincons0 = ap_lincons0_make(constyp,expr->linexpr0,scalar);
  cons.env = expr->env;
  return cons;
}
static inline
ap_lincons1_t ap_lincons1_copy(ap_lincons1_t* cons){
  ap_lincons1_t ncons;
  ncons.lincons0 = ap_lincons0_copy(&cons->lincons0);
  ncons.env = ap_environment_copy(cons->env);
  return ncons;
}

bool ap_lincons1_is_unsat(ap_lincons1_t* cons){
  return ap_lincons0_is_unsat(&cons->lincons0);
}

static inline
ap_environment_t* ap_lincons1_envref(ap_lincons1_t* cons){
  return cons->env;
}

static inline
ap_constyp_t* ap_lincons1_constypref(ap_lincons1_t* cons){
  return &cons->lincons0.constyp;
}
static inline
ap_scalar_t* ap_lincons1_scalarref(ap_lincons1_t* cons){
  return cons->lincons0.scalar;
}
static inline
ap_linexpr1_t ap_lincons1_linexpr1ref(ap_lincons1_t* cons){
  ap_linexpr1_t expr;
  expr.linexpr0 = cons->lincons0.linexpr0;
  expr.env = cons->env;
  return expr;
}
static inline
ap_lincons0_t* ap_lincons1_lincons0ref(ap_lincons1_t* cons){
  return &cons->lincons0;
}
static inline
ap_coeff_t* ap_lincons1_cstref(ap_lincons1_t* cons){
  return &cons->lincons0.linexpr0->cst;
}
static inline void ap_lincons1_get_cst(ap_coeff_t* coeff, ap_lincons1_t* cons){
  ap_linexpr0_get_cst(coeff,cons->lincons0.linexpr0);
}
static inline void ap_lincons1_set_cst(ap_lincons1_t* cons, ap_coeff_t* cst){
  ap_linexpr0_set_cst(cons->lincons0.linexpr0,cst);
}
static inline bool ap_lincons1_set_coeff(ap_lincons1_t* cons, ap_var_t var, ap_coeff_t* coeff)
  { ap_coeff_t* ecoeff = ap_lincons1_coeffref(cons,var); if (ecoeff){ap_coeff_set(ecoeff,coeff); return false;} else return true; }

static inline
size_t ap_lincons1_array_size(ap_lincons1_array_t* array){
  return array->lincons0_array.size;
}

static inline
ap_environment_t* ap_lincons1_array_envref(ap_lincons1_array_t* array){
  return array->env;
}

static inline
void ap_lincons1_array_clear_index(ap_lincons1_array_t* array, size_t index){
  ap_lincons0_clear(&array->lincons0_array.p[index]);
}

#ifdef __cplusplus
}
#endif

#endif
