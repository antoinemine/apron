/* ************************************************************************* */
/* abstract1.c: level 1 of interface */
/* ************************************************************************* */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "coeff.h"
#include "manager.h"
#include "abstract0.h"
#include "expr0.h"
#include "expr1.h"
#include "permutation.h"
#include "abstract1.h"

static inline abstract1_t abstract1_cons(abstract0_t* a0, environment_t* env)
{
  abstract1_t res;
  res.abstract0 = a0;
  res.env = environment_copy(env);
  return res;
}

void abstract1_manager_raise_invalid_var(manager_t* man,
					 funid_t funid,
					 var_t var)
{
  char msg[80];
  sprintf(msg,"unknown variable %s in the environment",var);
  manager_raise_exception(man,EXC_INVALID_ARGUMENT,funid,msg);
}

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

/* Return a copy of an abstract value, on
   which destructive update does not affect the initial value. */
abstract1_t abstract1_copy(manager_t* man, const abstract1_t* a){
  return abstract1_cons(abstract0_copy(man,a->abstract0),a->env);
}

/* Free all the memory used by the abstract value */
void abstract1_clear(manager_t* man, abstract1_t* a)
{
  abstract0_free(man,a->abstract0);
  environment_deref(a->env);
  a->abstract0 = NULL;
  a->env = NULL;
}

/* Return the abstract size of an abstract value (see manager_t) */
size_t abstract1_size(manager_t* man, const abstract1_t* a){
  return abstract0_size(man,a->abstract0);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */


/* Minimize the size of the representation of a.
   This may result in a later recomputation of internal information.
*/
void abstract1_minimize(manager_t* man, const abstract1_t* a){
  abstract0_minimize(man,a->abstract0);
}

/* Put the abstract value in canonical form. (not yet clear definition) */
void abstract1_canonicalize(manager_t* man, const abstract1_t* a){
  abstract0_canonicalize(man,a->abstract0);
}

/* Perform some transformation on the abstract value, guided by the
   field algorithm.

   The transformation may lose information.  The argument "algorithm"
   overrides the field algorithm of the structure of type foption_t
   associated to abstract1_approximate (commodity feature). */
void abstract1_approximate(manager_t* man, abstract1_t* a, int algorithm){
  abstract0_approximate(man,a->abstract0, algorithm);
}

tbool_t abstract1_is_minimal(manager_t* man, const abstract1_t* a){
  return abstract0_is_minimal(man,a->abstract0);
}
tbool_t abstract1_is_canonical(manager_t* man, const abstract1_t* a){
  return abstract0_is_canonical(man,a->abstract0);
}


/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

/* Print the abstract value */
void abstract1_fprint(FILE* stream,
		      manager_t* man,
		      const abstract1_t* a)
{
  environment_name_of_dim_t* name_of_dim;

  name_of_dim = environment_name_of_dim_alloc(e->env);
  abstract0_fprint(stream,man,a->abstract0, name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}

/* Print the difference between a1 (old value) and a2 (new value), using
   function name_of_dim to name dimensions.  The meaning of difference is
   library dependent. */
void abstract1_fprintdiff(FILE* stream,
			  manager_t* man,
			  const abstract1_t* a1, const abstract1_t* a2)
{
  environment_name_of_dim_t* name_of_dim;
  if (!environment_is_eq(a1->env,a2->env)){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,fun_fprintdiff,
			    "the 2 abstract values are not defined on the same environement");
    fprintf(stream,"unknown diff\n");
  }
  name_of_dim = environment_name_of_dim_alloc(e->env);
  abstract0_fprintdiff(stream,man,a1->abstract0, a2->abstract0, name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}

/* Dump the internal representation of an abstract value,
   for debugging purposes */
void abstract1_fdump(FILE* stream, manager_t* man, const abstract1_t* a)
{
  fprintf(stream, "abstract value of level 1:\n");
  environment_fdump(stream,a->env);
  abstract0_fdump(stream,man,a->abstract0);
}


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

/* DOIT-ON SÉRIALISER L'ENVIRONNEMENT ? */

/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

membuf_t abstract1_serialize_raw(manager_t* man, const abstract1_t* a){
  membuf_t membuf;
  manager_raise_exception(man,EXC_NOT_IMPLEMENTED,fun_serialize_raw,"");
  membuf.ptr = NULL;
  membuf.ptr = 0;
  return membuf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
abstract1_t abstract1_deserialize_raw(manager_t* man, void* ptr){
  manager_raise_exception(man,EXC_NOT_IMPLEMENTED,fun_deserialize_raw,"");
  return abstract1_top(man,environment_make_empty());
}


/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* Create a bottom (empty) value */
abstract1_t abstract1_bottom(manager_t* man, environment_t* env)
{
  return abstract1_cons(abstract0_bottom(man,env->intdim,env->realdim),env);
}

/* Create a top (universe) value */
abstract1_t abstract1_top(manager_t* man, environment_t* env)
{
  return abstract1_cons(abstract0_top(man,env->intdim,env->realdim),env);
}

/* Abstract an hypercube defined by the arrays tvar and tinterval,
   satisfying: forall i, tvar[i] in tinterval[i].

   If no inclusion is specified for a variable in the environement, its value
   is no constrained in the resulting abstract value.
*/
abstract1_t abstract1_of_box(manager_t* man,
			     environment_t* env,
			     var_t* tvar,
			     interval_t** tinterval,
			     size_t size)
{
  struct interval_t** itv;
  abstract1_t a;
  size_t i;

  itv = interval_array_alloc(env->intdim+env->realdim);
  for (i=0; i<env->intdim+env->realdim; i++){
    interval_set_top(itv[i]);
  }
  for (i=0; i<size; i++){
    dim_t dim = environment_dim_of_var(env,tvar[i]);
    if (dim==DIM_MAX){
      abstract1_manager_raise_invalid_var(man,FUN_OF_BOX,tvar[i]);
      interval_array_free(itv,size);
      return abstract1_top(man,env);
    }
    interval_set(itv[dim],tinterval[i]);
  }
  a = abstract1_cons(abstract0_of_box(man,env->intdim,env->realdim,itv),env);
  interval_array_free(itv,env->intdim+env->realdim);
  return a;
}

/* Abstract a convex polyhedra defined by the array of linear constraints
   of size size */
abstract1_t abstract1_of_lincons_array(manager_t* man,
				       environment_t* env,
				       const lincons1_array_t* array)
{
  abstract1_t a;
  lincons0_array_t* array0;
  dimchange_t* dimchange;

  if (env==NULL)
    env = array->env;

  if (environment_is_eq(env,array->env)){
    array0 = array->lincons0_array;
  }
  else {
    dimchange = environment_dimchange(array->env,env);
    if (dimchange==NULL){
      manager_raise_exception(man,
			      EXC_INVALID_ARGUMENT,
			      FUN_OF_LINCONS_ARRAY,
			      "environment of input constraints is not a subset of the requested environment");
      return abstract1_top(man,env);
    }
    array0 = lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  a = abstract1_cons(abstract0_of_lincons_array(man,
						 env->intdim,
						 env->realdim,
						 &array0);
		      env);
  if (dimchange){
    free(dimchange);
    lincons0_array_clear(&array0);
  }
  return a;
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

const environment_t* abstract1_environment(manager_t* man, const abstract1_t* a){
  return a->env;
}

const abstract0_t* abstract1_abstract0(manager_t* man, const abstract1_t* a){
  return a->abstract0;
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

/* If any of the following functions returns tbool_top, this means that
   an exception has occured, or that the exact computation was
   considered too expensive to be performed (according to the options).
   The flag exact and best should be cleared in such a case. */

tbool_t abstract1_is_bottom(manager_t* man, const abstract1_t* a){
  return abstract0_is_bottom(man,a->abstract0);
}
tbool_t abstract1_is_top(manager_t* man, const abstract1_t* a){
  return abstract0_is_top(man,a->abstract0);
}

/* inclusion check */
tbool_t abstract1_is_leq(manager_t* man, const abstract1_t* a1, const abstract1_t* a2)
{
  if (abstract1_check_env2(FUN_IS_LEQ,man,a1,a2))
    return abstract0_is_leq(man,a1->abstract0,a2->abstract0);
  else
    return tbool_top;
}
/* equality check */
tbool_t abstract1_is_eq(manager_t* man, const abstract1_t* a1, const abstract1_t* a2)
{
  if (abstract1_check_env2(FUN_IS_EQ,man,a1,a2))
    return abstract0_is_eq(man,a1->abstract0,a2->abstract0);
  else
    return tbool_top;
}
/* does the abstract value satisfy the linear constraint ? */
tbool_t abstract1_sat_lincons(manager_t* man, const abstract1_t* a, const lincons1_t* cons)
{
  lincons1_t conss;
  environment_t* env;
  dim_t* perm1;
  dim_t* perm2;
  tbool_t res;

  if (environment_is_eq(a->env,cons->env)){
    res = abstract0_sat_lincons(man,a->abstract0,&cons->lincons0);
  }
  else {
    dimchange_t* dimchange;
    lincons0_t cons0;

    dimchange == environment_dimchange(cons->env,a->env);
    if (dimchange==NULL){
      manager_raise_exception(man,EXC_INVALID_ARGUMENT,FUN_SAT_LINCONS,
			      "the environment of the constraint is not a subset of the environment of the abstract value");
      res = tbool_top;
      return res;
    }
    cons0 = lincons0_add_dimensions(&cons.lincons0,dimchange);
    res = abstract0_sat_lincons(man,a->abstract0,&cons0);
    free(dimchange);
    lincons0_clear(&cons0);
  }
  return res;
}

/* Is the dimension included in the interval in the abstract value ? */
tbool_t abstract1_sat_interval(manager_t* man, const abstract1_t* a,
			       var_t var, const interval_t* interval)
{
  dim_t dim;
  tbool_t res;

  dim = environment_dim_of_var(a->env, var);
  if (dim==DIM_MAX){
    abstract1_manager_raise_invalid_var(man,FUN_SAT_INTERVAL,var);
    res = tbool_top;
    return res;
  }
  return abstract0_sat_interval(man, a->abstract0, dim, interval);
}

/* Is the dimension included in the interval in the abstract value ? */
tbool_t abstract1_is_dimension_unconstrained(manager_t* man, const abstract1_t* a,
					     var_t var)
{
  dim_t dim;
  tbool_t res;

  dim = environment_dim_of_var(a->env, var);
  if (dim==DIM_MAX){
    abstract1_manager_raise_invalid_var(man,FUN_IS_DIMENSION_UNCONSTRAINED,var);
    res = tbool_top;
    return res;
  }
  return abstract0_is_dimension_unconstrained(man, a->abstract0, dim);
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

/* Returns the interval taken by a linear expression
   over the abstract value */
interval_t* abstract1_bound_linexpr(manager_t* man,
				    const abstract1_t* a, const linexpr1_t* expr)
{
  environment_t* env;
  interval_t* res;

  if (environment_is_eq(a->env,expr->env)){
    res = abstract0_bound_linexpr(man,a->abstract0,expr->linexpr0);
  }
  else {
    dimchange_t* dimchange;
    linexpr0_t* expr0;

    dimchange == environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      manager_raise_exception(man,EXC_INVALID_ARGUMENT,FUN_BOUND_LINEXPR,
			      "the environment of the linear expression is not a subset of the environment of the abstract value");
      res = interval_alloc();
      interval_reinit(man->option.scalar_discr);
      interval_set_top(res);
      return res;
    }
    expr0 = linexpr0_add_dimensions(expr->linexpr0,dimchange);
    res = abstract0_bound_linexpr(man,a->abstract0,expr0);
    free(dimchange);
    linexpr0_free(expr0);
  }
  return res;
}

/* Returns the interval taken by the dimension over the abstract
   value */
interval_t* abstract1_bound_dimension(manager_t* man,
				      const abstract1_t* a, var_t var)
{
  dim_t dim;
  interval_t* res;

  dim = environment_dim_of_var(a->env, var);
  if (dim==DIM_MAX){
    abstract1_manager_raise_invalid_var(man,FUN_BOUND_DIMENSION,var);
    res = interval_alloc();
    interval_reinit(man->option.scalar_discr);
    interval_set_top(res);
    return res;
  }
  return abstract0_bound_dimension(man, a->abstract0, dim);
}

/* Converts an abstract value to a polyhedra (conjunction of
   linear constraints). */
lincons1_array_t abstract1_to_lincons_array(manager_t* man, const abstract1_t* a)
{
  lincons1_array_t array;

  array.lincons0_array = abstract0_to_lincons_array(man,a->abstract0);
  array.env = environment_copy(a->env);
  return array;
}

/* Converts an abstract value to an interval/hypercube.  The size
   of the resulting array is abstract1_dimension(man,a).  This
   function can be reimplemented by using abstract1_bound_linexpr
   */
box1_t abstract1_to_box(manager_t* man, const abstract1_t* a)
{
  box1_t box;

  box.p = abstract0_to_box(man,a->abstract0);
  box.env = environment_copy(a->env);
  return box;
}

/* Converts an abstract value to a system of generators. */
generator1_array_t abstract1_to_generator_array(manager_t* man, const abstract1_t* a)
{
  generator1_array_t array;

  array.generator0_array = abstract0_to_generator_array(man,a->abstract0);
  array.env = environment_copy(a->env);
  return array;
}

/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

static inline 
abstract1_t abstract1_consres(bool destructive, abstract1_t* a, abstract0_t* value)
{
  abstract1_t res;
  if (destructive){
    if (value==a->abstract0)
      res = *a;
    else {
      res.abstract0 = value;
      res.env = a->env;
    }
  }
  else {
    res = abstract1_cons(value,a->env);
  }
}


abstract1_t abstract1_meetjoin(funid_t funid, manager_t* man, bool destructive, abstract1_t* a1, const abstract1_t* a2)
{
  abstract1_t res;
  if (abstract1_check_env2(funid,man,a1,a2)){
    abstract0_t* value = abstract0_meetjoin(funid,man,destructive,a1->abstract0,a2->abstract0);
    res = abstract1_consres(destructive, a1, value);
  }
  else {
    res = abstract1_top(man,a1->env);
    if (destructive) abstract1_clear(a1);
  }
  return res:
}
abstract1_t abstract1_meet(manager_t* man, bool destructive, abstract1_t* a1, const abstract1_t* a2){
  return abstract1_meetjoin(FUN_MEET,man,destructive,a1,a2);
}
abstract1_t abstract1_join(manager_t* man, bool destructive, abstract1_t* a1, const abstract1_t* a2){
  return abstract1_meetjoin(FUN_JOIN,man,destructive,a1,a2);
}
abstract1_t abstract1_meetjoin_array(funid_t funid, manager_t* man, const abstract1_t* tab, size_t size)
{
  abstract1_t res;
  if (abstract1_check_env_array(funid,man,tab,size)){
    abstract0_t** tab0 = malloc(size*sizeof(abstract0_t*));
    for (i=0;i<size;i++) tab0[i]=tab[i].abstract0;
    res.abstract0 = abstract0_meetjoin_array(funid,man,tab0,size);
    res.env = environment_copy(tab[0].env);
    free(tab0);
  }
  else {
    res = abstract1_top(man, size==0 ? env_make_empty() : tab[0].env);
  }
  return res;
}
abstract1_t abstract1_meet_array(manager_t* man, const abstract1_t* tab, size_t size){
  return abstract1_meetjoin_array(FUN_MEET_ARRAY,man,tab,size);
}
abstract1_t abstract1_join_array(manager_t* man, const abstract1_t* tab, size_t size){
  return abstract1_meetjoin_array(FUN_JOIN_ARRAY,man,tab,size);
}

abstract1_t abstract1_meet_lincons_array(manager_t* man,
					 bool destructive,
					 abstract1_t* a, const lincons1_array_t* array)
{
  abstract1_t res;
  lincons0_array_t* array0;
  dimchange_t* dimchange;

  if (environment_is_eq(a->env,array->env)){
    array0 = array->lincons0_array;
  }
  else {
    dimchange = environment_dimchange(array->env,env);
    if (dimchange==NULL){
      manager_raise_exception(man,
			      EXC_INVALID_ARGUMENT,
			      FUN_MEET_LINCONS_ARRAY,
			      "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? *a : abstract1_copy(man,a);
      return res;
    }
    array0 = lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  abstract0_t* value = abstract0_meet_lincons_array(man,destructive,a->abstract0,&array0);
  if (dimchange){
    free(dimchange);
    lincons0_array_clear(&array0);
  }
  res = abstract1_consres(destructive, a, value);
  return res;
}

abstract1_t abstract1_add_ray_array(manager_t* man,
				    bool destructive,
				    abstract1_t* a, const generator1_array_t* array)
{
  abstract1_t res;
  generator0_array_t* array0;
  dimchange_t* dimchange;

  if (environment_is_eq(a->env,array->env)){
    array0 = array->generator0_array;
  }
  else {
    dimchange = environment_dimchange(array->env,env);
    if (dimchange==NULL){
      manager_raise_exception(man,
			      EXC_INVALID_ARGUMENT,
			      FUN_ADD_RAY_ARRAY,
			      "environment of array of generators is not a subset of the environment of the abstract value");
      res = abstract1_top(man,a->env);
      if (destructive) abstract1_clear(a);
      return res;
    }
    array0 = generator0_array_add_dimensions(&array->generator0_array,dimchange);
  }
  abstract0_t* value = abstract0_add_ray_array(man,destructive,a->abstract0,&array0);
  if (dimchange){
    free(dimchange);
    generator0_array_clear(&array0);
  }
  res = abstract1_consres(destructive, a, value);
  return res;
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

abstract1_t abstract1_asssub_linexpr(funid_t funid, manager_t* man, bool destructive, abstract1_t* a, var_t var, linexpr1_t* expr, const abstract1_t* dest)
{
  dim_t dim;
  abstract1_t res;
  linexpr0_t* linexpr0;
  dimchange_t* dimchange;
  
  if ( dest==NULL || abstract1_check_env2(funid,man,a,dest)){
    dim = environment_dim_of_var(a->env,var);
    if (dim==DIM_MAX){
      abstract1_raise_var(man,funid,var);
      goto abstract1_asssub_linexpr_exit;
    }
    if (environment_is_eq(a->env,expr->env)){
      linexpr0 = expr->linexpr0;
    }
    else {
      dimchange = environment_dimchange(expr->env,a->env);
      if (dimchange==NULL){
	abstract1_raise_invalid_linexpr(man,funid);
	goto abstract1_asssub_linexpr_exit;
      }
      linexpr0 = linexpr0_add_dimensions(expr->linexpr0,dimchange);
    }
    abstract0_t* value1 = a->abstract0;
    abstract0_t* value = abstract0_asssub_linexpr(funid,man,destructive,a->abstract0,dim,linexpr0,dest ? dest->abstract0 : NULL);
    if (dimchange){
      free(dimchange);
      linexpr0_free(linexpr0);
    }
    res = abstract1_consres(destructive, a, value);
  }
  else { 
  abstract1_asssub_linexpr_exit:
    res = abstract1_top(man,a->env);
    if (destructive) abstract1_clear(a);
  }
  return res;
}

abstract1_t abstract1_assign_linexpr(manager_t* man, bool destructive, abstract1_t* a, var_t var, linexpr1_t* expr, const abstract1_t* dest){
  return abstract1_asssub_linexpr(FUN_ASSIGN_LINEXPR,man,destructive,a,var,expr,dest);
}
abstract1_t abstract1_substitute_linexpr(manager_t* man, bool destructive, abstract1_t* a, var_t var, linexpr1_t* expr, const abstract1_t* dest){
  return abstract1_asssub_linexpr(FUN_SUBSTITUE_LINEXPR,man,destructive,a,var,expr,dest);
}

abstract1_t abstract1_asssub_linexpr_array(funid_t funid, manager_t* man, bool destructive, 
					   abstract1_t* a, var_t* var, linexpr1_t* texpr, size_t size, 
					   const abstract1_t* dest)
{
  dim_t* tdim;
  linexpr0_t** tlinexpr0;
  abstract1_t res;
  linexpr0_t* linexpr0;
  dimchange_t* dimchange;
  size_t i;

  i = 0;
  tdim = NULL;
  tlinexpr0 = NULL;
  if ( dest==NULL || abstract1_check_env2(funid,man,a,dest)){
    tdim = malloc(size*sizeof(dim_t));
    tlinexpr0 = malloc(size*sizeof(linexpr0_t*));
    for (i=0; i<size; i++){
      tdim[i] = environment_dim_of_var(a->env,var[i]);
      if (dim==DIM_MAX){
	abstract1_raise_var(man,funid,tvar[i]);
	goto abstract1_asssub_linexpr_array_exit;
      }
      if (environment_is_eq(a->env,texpr[i].env)){
	tlinexpr0[i] = texpr[i].linexpr0;
      }
      else {
	dimchange = environment_dimchange(texpr[i].env,a->env);
	if (dimchange==NULL){
	  abstract1_raise_invalid_linexpr(man,funid);
	  goto abstract1_asssub_linexpr_array_exit;
	}
	tlinexpr0[i] = linexpr0_add_dimensions(texpr[i].linexpr0,dimchange);
	free(dimchange);
      }
    }
    abstract0_t* value = abstract0_asssub_linexpr_array(funid,man,destructive,a->abstract0,tdim,tlinexpr0,dest ? dest->abstract0 : NULL);
    res = abstract1_consres(destructive, a, value);
  }
  else { 
  abstract1_asssub_linexpr_exit:
    res = abstract1_top(man,a->env);
    if (destructive) abstract1_clear(a);
  }
  if (tlinexpr0!=NULL){
    size_t j;
    for (j=0; j<i; j++){
      if (tlinexpr0[j]!=texpr[j].linexpr0)
	linexpr0_free(tlinexpr0[j]);
    }
    free(tlinexpr0);
    free(tdim);
  }
  return res;
}

abstract1_t abstract1_assign_linexpr_array(manager_t* man, bool destructive, 
					   abstract1_t* a, var_t* tvar, linexpr1_t* texpr, size_t size,
					   const abstract1_t* dest){
  return abstract1_asssub_linexpr_array(FUN_ASSIGN_LINEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
}
abstract1_t abstract1_substitute_linexpr_array(manager_t* man, bool destructive, 
					   abstract1_t* a, var_t* tvar, linexpr1_t* texpr, size_t size,
					   const abstract1_t* dest){
  return abstract1_asssub_linexpr_array(FUN_SUBSTITUTE_LINEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

abstract1_t abstract1_forget_array(manager_t* man, bool destructive,
				   abstract1_t* a,
				   var_t* tvar, size_t size,
				   bool project)
{
  dim_t* tdim;
  size_t i;
  abstract1_t res;

  tdim = malloc(size*sizeof(dim_t));
  for (i=0; i<size; i++){
    tdim[i] = environment_dim_of_var(a->env,var[i]);
    if (dim==DIM_MAX){
      abstract1_raise_var(man,FUN_FORGET_ARRAY,tvar[i]);
      res = abstract1_top(man,a->env);
      if (destructive) abstract1_clear(a);
      goto abstract1_forget_array_exit;
    }
  }
  abstract0_t* value = abstract0_forget_array(man,destructive,a->abstract0,tdim,size,project);
  res = abstract1_consres(destructive, a, value);
 abstract1_forget_array_exit:
  free(tdim);
  return res;
}

/* ============================================================ */
/* III.4 Change of environnement */
/* ============================================================ */

void abstract1_change_environment(manager_t* man, bool destructive,
				  abstract1_t* a,
				  environment_t* nenv,
				  bool project)
{
  environment_t* env;
  abstract1_t res;
  dimchange_t* dimchange1;
  dimchange_t* dimchange2;

  if (environment_is_eq(a->env,nenv)){
    res = destructive ? *a : abstract1_copy(man,a);
    return res;
  }
  dimchange1 = dimchange2 = dimchangei = NULL;
  env = environment_lce(a->env,nenv,&dimchange1,&dimchange2);
  if (env==NULL){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,
			    FUN_CHANGE_ENVIRONMENT,
			    "the abstract value and the new environment are incompatible");
    res = abstract1_top(man,nenv);
    if (destructive) abstract1_clear(man,a);
    return res;
  }
  assert(dimchange1 || dimchange2);
  value = 
    dimchange1 ? 
    abstract0_add_dimensions(man,destructive,a->abstract0,dimchange1)
    a->abstract0;
  ;
  if (dimchange2){
    dimchange_add_invert(dimchange2);
    value = abstract0_remove_dimensions(man,destructive || dimchange1,value,dimchangei);
    free(dimchange2);
  }
  if (dimchange1)
    free(dimchange1);
  res = abstract1_consres(destructive, a, value);
  return res;
}

abstract1_t abstract1_rename(manager_t* man,
			     bool destructive,
			     abstract1_t* a,
			     var_t* tvar1, var_t* tvar2, size_t size)
{
  abstract1_t res;
  dim_t* perm;
  environment* env;
  abstract0_t* value;

  env = environment_rename(a->env,tvar1,tvar2,&perm);
  if (env==NULL){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,FUN_RENAME,
			    "renaming specification invalid");
    res = abstract1_top(man,a->env);
    if (destructive) abstract1_clear(ma,a);
  }
  else {
    value = abstract0_permute_dimensions(man,destructive,a->abstract0,perm);
    res = abstract1_consres(destructive, a, value);
    free(perm);
  }
  return res;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

static int dim_cmp(const void* a, const void* b)
{
  dim_t aa = *((dim_t*)a);
  dim_t bb = *((dim_t*)b);
  return aa>bb ? 1 : -1;
}

abstract1_t abstract1_expand(manager_t* man,
			     bool destructive,
			     abstract1_t* a,
			     var_t var,
			     var_t* tvar,
			     size_t size)
{
  abstract0_t* a0;
  dim_t dim;
  dim_t* perm;
  environment_t* nenv;
  size_t i,j;
  abstract0_t* value;

  /* particular case */
  if (size==0){
    res = destructive ? *a : abstract1_copy(man,a);
    return res;
  }
  /* Getting the dimension to expand */
  dim = environment_dim_of_var(a->env, var);
  if (dim==DIM_MAX){
    abstract1_raise_invalid_var(man,FUN_EXPAND,var);
  abstract1_expand_exit:
    res = abstract1_top(man,env);
    if (destructive) abstract1_free(man,a);
    return res;
  }
  /* Doing the expansion at level 0 */
  value = abstract0_expand(man,destructive,a->abstract0,dim,size);

  /* Building the resulting environment and permutation to apply */
  nenv =
    (dim<a->env->intdim) ?
    environment_add_perm(a->env, tvar, size, NULL,0, &perm) :
    environment_add_perm(a->env, NULL,0, tvar, size, &perm);
  if (nenv==NULL){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,
			    FUN_EXPAND,
			    "some new variables resulting from the expansion already exist in current environment");
    goto abstract1_expand_exit;
  }

  /* Apply permutation and return */
  value = abstract0_permute_dimensions(man,true,value,perm);
  free(perm);
  res = abstract1_consres(destructive, a, value);
  return res;
}

void abstract1_fold(manager_t* man,
		    bool destructive,
		    abstract1_t* a, var_t* tvar, size_t size)
{
  abstract0_t* a0;
  dim_t* tdim;
  environment_t* nenv;
  size_t i;
  dim_t dim = 
  if (size==0){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,FUN_FOLD,
			    "array of variables to fold of size 0");
  abstract1_fold_exit:
    res = abstract1_top(man,env);
    if (destructive) abstract1_free(man,a);
    return res;
  }
  else if (size==1){
    res = destructive ? *a : abstract1_copy(man,a);
    return res;
  }

  /* Compute resulting environment */
  dim = environment_dim_of_var(a->env,tvar[0]);
  if (dim==DIM_MAX){
    abstract1_raise_invalid_var(man,FUN_FOLD,tvar[0]);
    goto abstract1_fold_exit;
  }
  nenv =
    (dim<a->env->intdim) ?
    environment_remove(a->env, &tvar[1], n-1, NULL,0) :
    environment_remove(a->env, NULL,0, &tvar[1], n-1);
  if (nenv==NULL){
    manager_raise_exception(man,EXC_INVALID_ARGUMENT,FUN_FOLD,
			    "some variables to fold are unkown in the environment");
     goto abstract1_fold_exit;
  }
  /* Translate variables to dimensions */
  tdim = malloc(size*sizeof(dim_t));
  tdim[0] = dim;
  for (i=1; i<size; i++){
    tdim[i] = environment_dim_of_var(nenv, tvar[i]);
    if (tdim[i]==DIM_MAX){
      abstract1_raise_invalid_var(man,FUN_FOLD,tvar[i]);
      free(tdim);
      goto _abstract1_fold_exit;
    }
  }
  /* Perform the operation */
  value = abstract0_fold(man,destructive,a,tdim,size);
  free(tdim);
  res = abstract1_consres(destructive, a, value);
  return res;
}  

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */
abstract1_t abstract1_widening(manager_t* man,
			       const abstract1_t* a1, const abstract1_t* a2){
  return abstract1_meetjoin(FUN_WIDENING,man,false,a1,a2);
}
/* Widening with threshold */
abstract1_t abstract1_widening_threshold(manager_t* man,
					 const abstract1_t* a1, const abstract1_t* a2
					 const lincons0_array_t* array)
{
  abstract1_t res;
  lincons0_array_t* array0;
  dimchange_t* dimchange;

  if (environment_is_eq(a1->env,array->env)){
    array0 = array->lincons0_array;
  }
  else {
    dimchange = environment_dimchange(array->env,env);
    if (dimchange==NULL){
      manager_raise_exception(man,
			      EXC_INVALID_ARGUMENT,
			      FUN_MEET_LINCONS_ARRAY,
			      "environment of array of constraints is not a subset of the environment of the abstract value");
res = abstract1_top(man,a1->env);
      return res;
    }
    array0 = lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  if (abstract1_check_env2(FUN_WIDENING,man,a1,a2)){
    abstract0_t* value = abstract0_widening_threshold(man,a1->abstract0,a2->abstract0,&array0);
    res = abstract1_cons(value,a1->env);
  }
  else {
    res = abstract1_top(man,a1->env);
  }
  if (dimchange){
    free(dimchange);
    lincons0_array_clear(&array0);
  }
  return res;
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

abstract1_t abstract1_closure(manager_t* man, bool destructive, 
			      abstract1_t* a)
{
  abstract0_t* value;
  value = abstract0_closure(man,destructive,a->abstract0);
  return abstract1_consres(destructive, a, value);
}
