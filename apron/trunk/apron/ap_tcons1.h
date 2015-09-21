/* ************************************************************************* */
/* ap_tcons1.h: tree constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_TCONS1_H_
#define _AP_TCONS1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_lincons1.h"
#include "ap_tcons0.h"
#include "ap_environment.h"
#include "ap_texpr1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Constraints */
typedef struct ap_tcons1_t {
  ap_tcons0_t tcons0;
  ap_environment_t* env;
} ap_tcons1_t;

/* Array of constraints */
typedef struct ap_tcons1_array_t {
  ap_tcons0_array_t tcons0_array;
  ap_environment_t* env;
} ap_tcons1_array_t;

/* ********************************************************************** */
/* I. ap_tcons1_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
ap_tcons1_t ap_tcons1_make(ap_constyp_t constyp,
			   ap_texpr1_t* expr,
			   ap_scalar_t* scalar);
  /* Create a constraint of given type with the given expression.
     The expression and the optional coefficient are not duplicated,
     just pointed to. */

static inline
ap_tcons1_t ap_tcons1_from_lincons1(ap_lincons1_t* cons);
  /* Create a tree constraint from a linear constraint */

static inline
ap_tcons1_t ap_tcons1_copy(ap_tcons1_t* cons);
  /* Duplication */

void ap_tcons1_clear(ap_tcons1_t* cons);
  /* Clear the constraint and set pointers to NULL */

void ap_tcons1_fprint(FILE* stream, ap_tcons1_t* cons);
void ap_tcons1_print(ap_tcons1_t* cons);
  /* Printing */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

static inline
ap_environment_t* ap_tcons1_envref(ap_tcons1_t* cons);
  /* Get a reference to the environment. Do not free it. */

static inline
ap_constyp_t* ap_tcons1_constypref(ap_tcons1_t* cons);
  /* Get a reference to the type of constraint */

static inline
ap_scalar_t* ap_tcons1_scalarref(ap_tcons1_t* cons);
  /* Get a reference to the auxiliary coefficient of the constraint */

static inline
ap_texpr1_t ap_tcons1_texpr1ref(ap_tcons1_t* cons);
  /* Get a reference to the underlying expression of the constraint.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */

static inline
ap_tcons0_t* ap_tcons1_tcons0ref(ap_tcons1_t* cons);
  /* Return underlying constraint of level 0.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */

/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_tcons1_extend_environment(ap_tcons1_t* ncons,
				  ap_tcons1_t* cons,
				  ap_environment_t* nenv);
bool ap_tcons1_extend_environment_with(ap_tcons1_t* cons,
				       ap_environment_t* nenv);

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

ap_tcons1_array_t ap_tcons1_array_make(ap_environment_t* env, size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers,
     so that ap_tcons1_array_free may be safely called */
void ap_tcons1_array_clear(ap_tcons1_array_t* array);
  /* Clear the constraints of the array, and then the array itself */
void ap_tcons1_array_fprint(FILE* stream,
			    ap_tcons1_array_t* array);

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

static inline
size_t ap_tcons1_array_size(ap_tcons1_array_t* array);
  /* Return the size of the array */
static inline
ap_environment_t* ap_tcons1_array_envref(ap_tcons1_array_t* array);
  /* Return a reference to the environment of the array. Do not free it. */

static inline
void ap_tcons1_array_clear_index(ap_tcons1_array_t* array, size_t index);
  /* Clear the constraint at index index. */

ap_tcons1_t ap_tcons1_array_get(ap_tcons1_array_t* array,
			      size_t index);
  /* Return the linear constraint of the given index
     Nothing is duplicated, and the result should never be cleared.
     Modifying the argument or the result is equivalent, except for
     change of environments */

bool ap_tcons1_array_set(ap_tcons1_array_t* array,
			      size_t index, ap_tcons1_t* cons);
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
ap_tcons1_array_extend_environment_with(ap_tcons1_array_t* array,
					  ap_environment_t* env);
bool
ap_tcons1_array_extend_environment(ap_tcons1_array_t* narray,
				     ap_tcons1_array_t* array,
				     ap_environment_t* env);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline
ap_tcons1_t ap_tcons1_of_tcons0(ap_environment_t* env, ap_tcons0_t tcons0)
{
  ap_tcons1_t res ;
  res.env = ap_environment_copy(env);
  res.tcons0 = tcons0;
  return res;
}

static inline
ap_tcons1_t ap_tcons1_make(ap_constyp_t constyp,
			   ap_texpr1_t* expr,
			   ap_scalar_t* scalar)
{
  ap_tcons1_t cons;
  cons.tcons0 = ap_tcons0_make(constyp,expr->texpr0,scalar);
  cons.env = expr->env;
  return cons;
}
static inline
ap_tcons1_t ap_tcons1_from_lincons1(ap_lincons1_t* cons){
  return ap_tcons1_of_tcons0(cons->env,
			     ap_tcons0_from_lincons0(&cons->lincons0));
}
static inline
ap_tcons1_t ap_tcons1_copy(ap_tcons1_t* cons){
  return ap_tcons1_of_tcons0(cons->env,
			     ap_tcons0_copy(&cons->tcons0));
}

static inline
ap_environment_t* ap_tcons1_envref(ap_tcons1_t* cons){
  return cons->env;
}

static inline
ap_constyp_t* ap_tcons1_constypref(ap_tcons1_t* cons){
  return &cons->tcons0.constyp;
}
static inline
ap_scalar_t* ap_tcons1_scalarref(ap_tcons1_t* cons){
  return cons->tcons0.scalar;
}
static inline
ap_texpr1_t ap_tcons1_texpr1ref(ap_tcons1_t* cons){
  ap_texpr1_t expr;
  expr.texpr0 = cons->tcons0.texpr0;
  expr.env = cons->env;
  return expr;
}
static inline
ap_tcons0_t* ap_tcons1_tcons0ref(ap_tcons1_t* cons){
  return &cons->tcons0;
}
static inline
size_t ap_tcons1_array_size(ap_tcons1_array_t* array){
  return array->tcons0_array.size;
}

static inline
ap_environment_t* ap_tcons1_array_envref(ap_tcons1_array_t* array){
  return array->env;
}

static inline
void ap_tcons1_array_clear_index(ap_tcons1_array_t* array, size_t index){
  ap_tcons0_clear(&array->tcons0_array.p[index]);
}

#ifdef __cplusplus
}
#endif

#endif
