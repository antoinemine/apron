/* ************************************************************************* */
/* ap_generator1.h: linear generators and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_GENERATOR1_H_
#define _AP_GENERATOR1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_environment.h"
#include "ap_generator0.h"
#include "ap_linexpr1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Generators */
typedef struct ap_generator1_t {
  ap_generator0_t generator0;
  ap_environment_t* env;
} ap_generator1_t;

/* Array of generators */
typedef struct ap_generator1_array_t {
  ap_generator0_array_t generator0_array;
  ap_environment_t* env;
} ap_generator1_array_t;


/* ********************************************************************** */
/* I. ap_generator0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

static inline
ap_generator1_t ap_generator1_make(ap_gentyp_t gentyp, ap_linexpr1_t* expr);
  /* Create a generator of given type with the given expression.
     The expression is not duplicated. */

static inline
ap_generator1_t ap_generator1_copy(ap_generator1_t* gen);
  /* Duplication */

void ap_generator1_clear(ap_generator1_t* gen);
  /* Clear the generator and set pointers to NULL */

void ap_generator1_fprint(FILE* stream, ap_generator1_t* gen);
  /* Printing */

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

static inline
ap_environment_t* ap_generator1_envref(ap_generator1_t* gen);
  /* Get a reference to the environment. Do not free it. */

static inline
ap_gentyp_t* ap_generator1_gentypref(ap_generator1_t* gen);
  /* Get a reference to the type of generator */

static inline
ap_linexpr1_t ap_generator1_linexpr1ref(ap_generator1_t* gen);
  /* Get a reference to the underlying expression of the generator.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/envionment. */

/* Get the constant and assign it to coeff */
static inline void ap_generator1_get_cst(ap_coeff_t* coeff, ap_generator1_t* gen);

/* Get coefficient of variable var in the generator and assign it to coeff.
   Return true if var is unknown in the environment */
bool ap_generator1_get_coeff(ap_coeff_t* coeff, ap_generator1_t* gen, ap_var_t var);

/* Set the constant of the linear generator */
static inline void ap_generator1_set_cst(ap_generator1_t* gen, ap_coeff_t* cst);

/* Set the coefficient of variable var in the generator.
   Return true if var is unknown in the environment */
static inline bool ap_generator1_set_coeff(ap_generator1_t* gen, ap_var_t var, ap_coeff_t* coeff);

/*
bool ap_generator1_set_format(ap_generator1_t* cons, char* fmt, ...);
*/

bool ap_generator1_set_list(ap_generator1_t* gen, ...);

static inline
ap_coeff_t* ap_generator1_cstref(ap_generator1_t* gen);
  /* Get a reference to the constant. Do not free it. */

ap_coeff_t* ap_generator1_coeffref(ap_generator1_t* gen, ap_var_t var);
  /* Get a reference to the coefficient associated to the variable.
     Do not free it.
     In case of sparse representation,
     possibly induce the addition of a new linear term.
     Return NULL if var is unknown in the environment.
  */

static inline
ap_generator0_t* ap_generator1_generator0ref(ap_generator1_t* gen);
  /* Return underlying generator of level 0.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */

/* ====================================================================== */
/* I.3 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_generator1_extend_environment(ap_generator1_t* ngen,
				      ap_generator1_t* gen,
				      ap_environment_t* nenv);
bool ap_generator1_extend_environment_with(ap_generator1_t* gen,
					   ap_environment_t* nenv);

/* ********************************************************************** */
/* II. Array of linear generators */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

ap_generator1_array_t ap_generator1_array_make(ap_environment_t* env, size_t size);
  /* Allocate an array of size generators.
     The generators are initialized with NULL pointers,
     so that ap_generator1_array_free may be safely called */
void ap_generator1_array_clear(ap_generator1_array_t* array);
  /* Clear the generators of the array, and then the array itself */
void ap_generator1_array_fprint(FILE* stream,
			   ap_generator1_array_t* ap_generator1_array);
  /* Printing */

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

static inline
size_t ap_generator1_array_size(ap_generator1_array_t* array);

static inline
ap_environment_t* ap_generator1_array_envref(ap_generator1_array_t* array);

static inline
void ap_generator1_array_clear_index(ap_generator1_array_t* array, size_t index);
  /* Clear the generator at index index. */

ap_generator1_t ap_generator1_array_get(ap_generator1_array_t* array,
			      size_t index);
  /* Return the linear generator of the given index
     Nothing is duplicated, and the result should never be cleared.
     Modifying the argument or the result is equivalent, except for
     change of environments */

bool ap_generator1_array_set(ap_generator1_array_t* array,
			      size_t index, ap_generator1_t* gen);
  /* Fill the index of the array with the generator.
     Assumes array->env==gen->env.
     Nothing is duplicated.
     The argument should never be cleared. (its environment is dereferenced).
     If a generator was already stored, it is first cleared.
     Return true iff problem (index or array->env!=gen->env)
  */

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

bool
ap_generator1_array_extend_environment_with(ap_generator1_array_t* array,
					    ap_environment_t* env);
bool
ap_generator1_array_extend_environment(ap_generator1_array_t* narray,
				       ap_generator1_array_t* array,
				       ap_environment_t* env);

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline
ap_generator1_t ap_generator1_make(ap_gentyp_t gentyp, ap_linexpr1_t* expr)
{
  ap_generator1_t gen;
  gen.generator0 = ap_generator0_make(gentyp,expr->linexpr0);
  gen.env = expr->env;
  return gen;
}
static inline
ap_generator1_t ap_generator1_copy(ap_generator1_t* gen){
  ap_generator1_t ngen;
  ngen.generator0 = ap_generator0_copy(&gen->generator0);
  ngen.env = ap_environment_copy(gen->env);
  return ngen;
}

static inline
ap_environment_t* ap_generator1_envref(ap_generator1_t* gen){
  return gen->env;
}

static inline
ap_gentyp_t* ap_generator1_gentypref(ap_generator1_t* gen){
  return &gen->generator0.gentyp;
}
static inline
ap_linexpr1_t ap_generator1_linexpr1ref(ap_generator1_t* gen){
  ap_linexpr1_t expr;
  expr.linexpr0 = gen->generator0.linexpr0;
  expr.env = gen->env;
  return expr;
}
static inline
ap_generator0_t* ap_generator1_generator0ref(ap_generator1_t* gen){
  return &gen->generator0;
}
static inline
ap_coeff_t* ap_generator1_cstref(ap_generator1_t* gen){
  return &gen->generator0.linexpr0->cst;
}
static inline void ap_generator1_get_cst(ap_coeff_t* coeff, ap_generator1_t* gen){
  ap_linexpr0_get_cst(coeff,gen->generator0.linexpr0);
}
static inline void ap_generator1_set_cst(ap_generator1_t* gen, ap_coeff_t* cst){
  ap_linexpr0_set_cst(gen->generator0.linexpr0,cst);
}
static inline bool ap_generator1_set_coeff(ap_generator1_t* gen, ap_var_t var, ap_coeff_t* coeff)
  { ap_coeff_t* ecoeff = ap_generator1_coeffref(gen,var); if (ecoeff){ap_coeff_set(ecoeff,coeff); return false;} else return true; }

static inline
size_t ap_generator1_array_size(ap_generator1_array_t* array){
  return array->generator0_array.size;
}

static inline
ap_environment_t* ap_generator1_array_envref(ap_generator1_array_t* array){
  return array->env;
}

static inline
void ap_generator1_array_clear_index(ap_generator1_array_t* array, size_t index){
  ap_generator0_clear(&array->generator0_array.p[index]);
}

#ifdef __cplusplus
}
#endif

#endif
