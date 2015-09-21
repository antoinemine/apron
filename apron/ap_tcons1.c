/* ************************************************************************* */
/* ap_lincons1.c: tree constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_tcons1.h"

/* ********************************************************************** */
/* I. ap_tcons0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */
ap_tcons1_t ap_tcons1_make_unsat(ap_environment_t* env)
{
  return ap_tcons1_of_tcons0(env,ap_tcons0_make_unsat());
}

void ap_tcons1_clear(ap_tcons1_t* cons)
{
  ap_tcons0_clear(&cons->tcons0);
  if (cons->env){
    ap_environment_free(cons->env); cons->env = NULL;
  }
}

void ap_tcons1_fprint(FILE* stream, ap_tcons1_t* cons)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(cons->env);
  ap_tcons0_fprint(stream, &cons->tcons0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_tcons1_print(ap_tcons1_t* cons)
{ ap_tcons1_fprint(stdout,cons); }

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

/* ====================================================================== */
/* I.4 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_tcons1_extend_environment(ap_tcons1_t* ncons,
				  ap_tcons1_t* cons,
				  ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(cons->env,nenv);
  if (dimchange==NULL) return true;
  ncons->tcons0 = ap_tcons0_add_dimensions(&cons->tcons0,dimchange);
  ncons->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
bool ap_tcons1_extend_environment_with(ap_tcons1_t* cons,
					 ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(cons->env,nenv);
  if (dimchange==NULL) return true;
  ap_tcons0_add_dimensions_with(&cons->tcons0,dimchange);
  env = cons->env;
  cons->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  ap_environment_free(env);
  return false;
}

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

ap_tcons1_array_t ap_tcons1_array_make(ap_environment_t* env, size_t size)
{
  ap_tcons1_array_t array;
  array.tcons0_array = ap_tcons0_array_make(size);
  array.env = ap_environment_copy(env);
  return array;
}
void ap_tcons1_array_clear(ap_tcons1_array_t* array)
{
  ap_tcons0_array_clear(&array->tcons0_array);
  ap_environment_free(array->env);
  array->env = NULL;
}
void ap_tcons1_array_fprint(FILE* stream,
			      ap_tcons1_array_t* array)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(array->env);
  ap_tcons0_array_fprint(stream,&array->tcons0_array,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_tcons1_array_print(ap_tcons1_array_t* array)
{ ap_tcons1_array_fprint(stdout,array); }

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

ap_tcons1_t ap_tcons1_array_get(ap_tcons1_array_t* array,
				size_t index)
{
  ap_tcons1_t cons;
  cons.tcons0 = array->tcons0_array.p[index];
  cons.env = array->env;
  return cons;
}

bool ap_tcons1_array_set(ap_tcons1_array_t* array,
			 size_t index, ap_tcons1_t* cons)
{
  if (index>=array->tcons0_array.size || !ap_environment_is_eq(cons->env,array->env))
    return true;
  ap_tcons1_array_clear_index(array,index);
  array->tcons0_array.p[index] = cons->tcons0;
  ap_environment_free(cons->env);
  return false;
}

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

bool
ap_tcons1_array_extend_environment_with(ap_tcons1_array_t* array,
					ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  ap_tcons0_array_add_dimensions_with(&array->tcons0_array,dimchange);
  env = array->env;
  array->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  ap_environment_free(env);
  return false;
}
bool
ap_tcons1_array_extend_environment(ap_tcons1_array_t* narray,
				   ap_tcons1_array_t* array,
				   ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  narray->tcons0_array = ap_tcons0_array_add_dimensions(&array->tcons0_array,dimchange);
  narray->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
