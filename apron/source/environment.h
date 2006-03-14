#line 24 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ********************************************************************** */
/* environment.h: binding of addresses (strings) to dimensions */
/* ********************************************************************** */

/* GENERATED FROM environment.nw: DOT NOT MODIFY ! */

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "manager.h"
#include "expr0.h"

#line 43 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ====================================================================== */
/* Datatype */
/* ====================================================================== */

/* An environement is parametrized by the abstract type var_t, 
   equipped with a total ordering function, a copy function, 
   and a free function.

   The parametrization is done via a global variable pointing to an 
   environment_var_t structure.
 */

typedef void* var_t;

typedef struct environment_var_t {
  int (*compare)(var_t v1, var_t v2);  /* Total ordering function */
  var_t (*copy)(var_t var);      /* Duplication function */
  void (*free)(var_t var);            /* Deallocation function */
  char* (*to_string)(var_t var); /* Conversion to a dynamically allocated string */
  void (*fprint)(FILE* stream, var_t var); /* Printing to a stream */
} environment_var_t;

extern environment_var_t environment_var_default;
  /* default manager: var_t are char* */

extern environment_var_t* environment_var;

#line 73 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* The environment_t type is the type used by the level 1 of the interface.
   It should be considered as an abstract type.

   We use reference counting to manage memory. Conventions are:

   - a newly allocated environement has a reference count of one;
   - environement_copy increments the counter and return its argument
   - environement_free decrements it and free the environement
     in case of zero or negative number.
*/

typedef struct environment_t {
  var_t* var_of_dim;
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
} environment_t;

typedef struct environment_name_of_dim_t {
  size_t size;
  char* p[0]; /* Assumed to be of size size */
} environment_name_of_dim_t;

#line 113 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ====================================================================== */
/* Memory management, Constructor, Destructors */
/* ====================================================================== */

void environment_free2(environment_t* e);
  /* Free the environement
     (the structure itself and the memory pointed to by fields) */

static inline
void environment_free(environment_t* e);
  /* Decrement the reference counter and possibly deallocate */

static inline
environment_t* environment_copy(environment_t* e);
  /* Copy */

void environment_fdump(FILE* stream, environment_t* env);
  /* Print an environement under the form:
      environment: dim = (..,..), count = ..
      0: name0
      1: name1
      ...
  */

environment_name_of_dim_t* environment_name_of_dim_alloc(environment_t* e);
void environment_name_of_dim_free(environment_name_of_dim_t*);

environment_t* environment_make_empty();
  /* Build an empty environement */

environment_t* environment_make(var_t* name_of_intdim, size_t intdim,
                                var_t* name_of_realdim, size_t realdim);
  /* Build an environement from an array of integer and
     an array of real variables.
     - Names are duplicated in the result, so it is the responsability
       of the user to free the names he provides.
     - If the result does not satisfy the invariant, return NULL.
  */

environment_t* environment_add(const environment_t* env,
                               var_t* name_of_intdim, size_t intdim,
                               var_t* name_of_realdim, size_t realdim);
  /* Add variables to an environement.
     Same comments as for environement_make */
environment_t* environment_remove(const environment_t* env,
                                  var_t* name_of_intdim, size_t intdim,
                                  var_t* name_of_realdim, size_t realdim);
  /* Remove variables from an environement.
     Same comments as for environement_make */

dim_t environment_dim_of_var(const environment_t* env, var_t name);
  /* - If the variable is present in the environemnt,
       return its associated dimension.
     - Otherwise, return DIM_MAX
  */

static inline
var_t environment_var_of_dim(const environment_t* env, dim_t dim);
  /* - Return the variable asosciated to the dimension.
     - There is no bound check here */

#line 181 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ========================================================================= */
/* Tests */
/* ========================================================================= */

bool environment_is_eq(const environment_t* env1,
                       const environment_t* env2);
  /* Equality test */
bool environment_is_leq(const environment_t* env1,
                       const environment_t* env2);
  /* Inclusion test */
int environment_compare(const environment_t* env1,
                        const environment_t* env2);
  /* Return
    - -2 if the environement are not compatible
      (a variable has different types in the 2 environements)
    - -1 if env1 is a subset of env2
    - 0 if equality
    - +1 if env1 is a superset of env2
    - +2 otherwise (the lce exists and is a strict superset of both)
  */

#line 209 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ========================================================================= */
/* Least common environments and conversion permutations */
/* ========================================================================= */

dimchange_t* environment_dimchange(const environment_t* env1,
                                   const environment_t* env);
  /* Compute the transformation for converting from an environment to a
     superenvironment.
     Return NULL if env is not a superenvironment.
  */

environment_t* environment_lce(const environment_t* env1,
                               const environment_t* env2,
                               dimchange_t** dimchange1,
                               dimchange_t** dimchange2);
  /*
  Least common environment to two environment.

   - Assume environment_is_eq(env1,env2)==false.

   - If environments are not compatible
     (a variable has different types in the 2 environements),
     return NULL

   - Compute also in dimachange1 and dimchange2 the conversion transofrmations.

   - If no dimensions to add to env1, this implies that env is
     actually env1. In this case, *dimchange1==NULL. Otherwise, the
     function allocates the dimchange1 with malloc.
 */
environment_t* environment_lce_array(const environment_t** tenv,
                                     size_t size,
                                     dimchange_t*** ptdimchange);
/*
  Least common environment to an array environments.

  - Assume the size of the array is at least one.

  - If all input environments are the same, *ptdimchange==NULL.
    Otherwise, compute in *ptdimchange the conversion permutations.

  - If no dimensions to add to env[i], this implies that env is actually
    env[i]. In this case, (*ptdimchange)[i]==NULL. Otherwise, the function allocates
    the array (*ptdimchange)[0] with malloc.

 */
#line 262 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ========================================================================= */
/* Variable renaming */
/* ========================================================================= */

environment_t* environment_rename(environment_t* env,
                                  var_t* tvar1, var_t* tvar2, size_t size,
                                  dim_t** permutation);
  /* Rename the variables in the environment.
     size is the common size of tvar1 and tvar2,
     and permutation is a result-parameter

     The function applies the variable substitution tvar1[i]->tvar2[i] to
     the environment, and returns the resulting environment and 
     the allocated transformation permutation in *permutation.
     
     If the parameters are not valid, returns NULL with *permutation==NULL
  */

#line 287 "/udd/bjeannet/dev/commoninterface/environment.nw"
/* ====================================================================== */
/* Inline definitions */
/* ====================================================================== */

static inline
var_t environment_var_of_dim(const environment_t* env, dim_t dim){
  return dim < env->intdim+env->realdim ? env->var_of_dim[dim] : NULL;
}
static inline
void environment_free(environment_t* env){
  if (env->count<=1)
    environment_free2(env);
  else
    env->count--;
}
static inline
environment_t* environment_copy(environment_t* env){
  env->count++;
  return env;
}
#line 310 "/udd/bjeannet/dev/commoninterface/environment.nw"
#endif
