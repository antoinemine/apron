/* ********************************************************************** */
/* ap_environment.h: binding of addresses (strings) to dimensions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* GENERATED FROM ap_environment.nw: DO NOT MODIFY ! */

#ifndef _AP_ENVIRONMENT_H_
#define _AP_ENVIRONMENT_H_

#include "ap_config.h"
#include "ap_dimension.h"
#include "ap_var.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ====================================================================== */
/* Datatype */
/* ====================================================================== */

/* The ap_environment_t type is the type used by the level 1 of the interface.
   It should be considered as an abstract type.

   We use reference counting to manage memory. Conventions are:

   - a newly allocated environment has a reference count of one;
   - environment_copy increments the counter and return its argument
   - environment_free decrements it and free the environment
     in case of zero or negative number.
*/

typedef struct ap_environment_t {
  ap_var_t* var_of_dim;
  /*
    Array of size intdim+realdim, indexed by dimensions.
    - It should not contain identical strings..
    - Slice [0..intdim-1] is lexicographically sorted,
      and denotes integer variables.
    - Slice [intdim..intdim+realdim-1] is lexicographically sorted,
      and denotes real variables.
    - The memory allocated for the variables are attached to the structure
      (they are freed when the structure is no longer in use)
  */
  size_t intdim; /* Number of integer variables */
  size_t realdim;/* Number of real variables */
  size_t count; /* For reference counting */
} ap_environment_t;

typedef struct ap_environment_name_of_dim_t {
  size_t size;
  char* p[]; /* Assumed to be of size size */
} ap_environment_name_of_dim_t;

/* ====================================================================== */
/* Memory management, Constructor, Destructors */
/* ====================================================================== */

void ap_environment_free2(ap_environment_t* e);
  /* Free the environment
     (the structure itself and the memory pointed to by fields) */

static inline
void ap_environment_free(ap_environment_t* e);
  /* Decrement the reference counter and possibly deallocate */

static inline
ap_environment_t* ap_environment_copy(ap_environment_t* e);
  /* Copy */

void ap_environment_fdump(FILE* stream, ap_environment_t* env);
  /* Print an environment under the form:
      environment: dim = (..,..), count = ..
      0: name0
      1: name1
      ...
  */

ap_environment_name_of_dim_t* ap_environment_name_of_dim_alloc(ap_environment_t* e);
void ap_environment_name_of_dim_free(ap_environment_name_of_dim_t*);

ap_environment_t* ap_environment_alloc_empty(void);
  /* Build an empty environment */

ap_environment_t* ap_environment_alloc(ap_var_t* name_of_intdim, size_t intdim,
				       ap_var_t* name_of_realdim, size_t realdim);
  /* Build an environment from an array of integer and
     an array of real variables.
     - Variables are duplicated in the result, so it is the responsibility
       of the user to free the variables he provides.
     - If the result does not satisfy the invariant, return NULL.
  */

ap_environment_t* ap_environment_add(ap_environment_t* env,
				     ap_var_t* name_of_intdim, size_t intdim,
				     ap_var_t* name_of_realdim, size_t realdim);
  /* Add variables to an environment.
     Same comments as for ap_environment_alloc */
ap_environment_t* ap_environment_add_perm(ap_environment_t* env,
					  ap_var_t* name_of_intdim, size_t intdim,
					  ap_var_t* name_of_realdim, size_t realdim,
					  ap_dimperm_t* dimpermu);
  /* Same as previous functions, but in addition return in *dimperm
     the permutation to apply after having added new variables at the end of their
     respective slice, in order to get them sorted. 
     If the result is NULL, so is dimperm->dim */

ap_environment_t* ap_environment_remove(ap_environment_t* env,
					ap_var_t* tvar, size_t size);
  /* Remove variables from an environment.
     Return NULL in case of attempt to remove an unknown variable.
     Same comments as for environment_alloc.
  */

static inline
bool ap_environment_mem_var(ap_environment_t* env, ap_var_t name);
  /* Return true if the variable is present in the environemnt */

ap_dim_t ap_environment_dim_of_var(ap_environment_t* env, ap_var_t name);
  /* - If the variable is present in the environemnt,
       return its associated dimension.
     - Otherwise, return AP_DIM_MAX
  */

static inline
ap_var_t ap_environment_var_of_dim(ap_environment_t* env, ap_dim_t dim);
  /* - Return the variable associated to the dimension.
     - There is no bound check here */

/* ========================================================================= */
/* Tests */
/* ========================================================================= */

bool ap_environment_is_eq(ap_environment_t* env1,
			  ap_environment_t* env2);
  /* Equality test */
bool ap_environment_is_leq(ap_environment_t* env1,
			   ap_environment_t* env2);
  /* Inclusion test */
int ap_environment_compare(ap_environment_t* env1,
                           ap_environment_t* env2);
  /* Return
    - -2 if the environments are not compatible
      (a variable has a different type in the 2 environments)
    - -1 if env1 is a subset of env2
    - 0 if equality
    - +1 if env1 is a superset of env2
    - +2 otherwise (the lce exists and is a strict superset of both)
  */
int ap_environment_hash(ap_environment_t* env);
  /* Return an hash code */

/* ========================================================================= */
/* Least common environments and conversion permutations */
/* ========================================================================= */

ap_dimchange_t* ap_environment_dimchange(ap_environment_t* env1,
					 ap_environment_t* env);
  /* Compute the transformation for converting from an environment to a
     superenvironment.
     Return NULL if env is not a superenvironment.
  */

ap_dimchange2_t* ap_environment_dimchange2(ap_environment_t* env1,
					   ap_environment_t* env2);
  /* Compute the transformation for converting from a first environment to a
     second environment, by first adding (some) variables of the second
     environment, and then removing (some) variables of the first
     environment. 

     If the two environments are incompatible, returns NULL
  */

ap_environment_t* ap_environment_lce(ap_environment_t* env1,
				     ap_environment_t* env2,
				     ap_dimchange_t** dimchange1,
				     ap_dimchange_t** dimchange2);
  /*
  Least common environment to two environments.

   - If environments are not compatible
     (a variable has different types in the 2 environments),
     return NULL

   - Compute also in dimchange1 and dimchange2 the conversion transformations.

   - If no dimensions to add to env1, this implies that env is
     actually env1. In this case, *dimchange1==NULL. Otherwise, the
     function allocates *dimchange1 with ap_dimchange_alloc.
 */
ap_environment_t* ap_environment_lce_array(ap_environment_t** tenv,
					   size_t size,
					   ap_dimchange_t*** ptdimchange);
/*
  Least common environment to an array environments.

  - Assume the size of the array is at least one.

  - If all input environments are the same, *ptdimchange==NULL.
    Otherwise, compute in *ptdimchange the conversion permutations.

  - If no dimensions to add to tenv[i], this implies that env is actually
    tenv[i]. In this case, (*ptdimchange)[i]==NULL. 
    Otherwise, the function allocates
    (*ptdimchange)[0] with ap_dimchange_alloc.

 */

/* ========================================================================= */
/* Variable renaming */
/* ========================================================================= */

ap_environment_t* ap_environment_rename(ap_environment_t* env,
					ap_var_t* tvar1, ap_var_t* tvar2, size_t size,
					ap_dimperm_t* perm);
  /* Rename the variables in the environment.
     size is the common size of tvar1 and tvar2,
     and perm is a result-parameter

     The function applies the variable substitution tvar1[i]->tvar2[i] to
     the environment, and returns the resulting environment and 
     the allocated transformation permutation in *permutation.
     
     If the parameters are not valid, returns NULL with perm->dim==NULL
  */

/* ====================================================================== */
/* Inline definitions */
/* ====================================================================== */

static inline
bool ap_environment_mem_var(ap_environment_t* env, ap_var_t name)
{ return (ap_environment_dim_of_var(env,name) != AP_DIM_MAX); }

static inline
ap_var_t ap_environment_var_of_dim(ap_environment_t* env, ap_dim_t dim){
  return dim < env->intdim+env->realdim ? env->var_of_dim[dim] : NULL;
}
static inline
void ap_environment_free(ap_environment_t* env){
  if (env->count<=1)
    ap_environment_free2(env);
  else
    env->count--;
}
static inline
ap_environment_t* ap_environment_copy(ap_environment_t* env){
  env->count++;
  return env;
}
#ifdef __cplusplus
}
#endif

#endif
