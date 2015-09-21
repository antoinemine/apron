/* ************************************************************************* */
/* ap_lincons1.c: linear constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_lincons1.h"

/* ********************************************************************** */
/* I. ap_lincons0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */
ap_lincons1_t ap_lincons1_make_unsat(ap_environment_t* env)
{
  ap_lincons1_t lincons1;
  lincons1.lincons0 = ap_lincons0_make_unsat();
  lincons1.env = ap_environment_copy(env);
  return lincons1;
}

void ap_lincons1_clear(ap_lincons1_t* cons)
{
  ap_lincons0_clear(&cons->lincons0);
  if (cons->env){
    ap_environment_free(cons->env); cons->env = NULL;
  }
}

void ap_lincons1_fprint(FILE* stream, ap_lincons1_t* cons)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(cons->env);
  ap_lincons0_fprint(stream, &cons->lincons0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

ap_coeff_t* ap_lincons1_coeffref(ap_lincons1_t* cons, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(cons->env,var);
  return ap_linexpr0_coeffref(cons->lincons0.linexpr0,dim);
}
bool ap_lincons1_get_coeff(ap_coeff_t* coeff, ap_lincons1_t* cons, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(cons->env,var);
  return ap_linexpr0_get_coeff(coeff,cons->lincons0.linexpr0,dim);
}

ap_coeff_t* ap_lincons1_set_list_get_pcoeff(void* cons, bool cst, va_list* va)
{
  ap_coeff_t* pcoeff;
  if (cst){
    pcoeff = ap_lincons1_cstref(cons);
  } else {
    ap_var_t var = va_arg(*va,ap_var_t);
    pcoeff = ap_lincons1_coeffref(cons,var);
  }
  return pcoeff;
}

bool ap_lincons1_set_list(ap_lincons1_t* cons, ...)
{
  va_list va;
  bool res;
  va_start(va,cons);
  res = ap_linexpr0_set_list_generic(ap_lincons1_set_list_get_pcoeff,
				     cons,&va);
  va_end(va);
  return res;
}

/* ====================================================================== */
/* I.4 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_lincons1_extend_environment(ap_lincons1_t* ncons,
				    ap_lincons1_t* cons,
				    ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(cons->env,nenv);
  if (dimchange==NULL) return true;
  ncons->lincons0 = ap_lincons0_add_dimensions(&cons->lincons0,dimchange);
  ncons->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
bool ap_lincons1_extend_environment_with(ap_lincons1_t* cons,
					 ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(cons->env,nenv);
  if (dimchange==NULL) return true;
  ap_lincons0_add_dimensions_with(&cons->lincons0,dimchange);
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

ap_lincons1_array_t ap_lincons1_array_make(ap_environment_t* env, size_t size)
{
  ap_lincons1_array_t array;
  array.lincons0_array = ap_lincons0_array_make(size);
  array.env = ap_environment_copy(env);
  return array;
}
void ap_lincons1_array_clear(ap_lincons1_array_t* array)
{
  ap_lincons0_array_clear(&array->lincons0_array);
  ap_environment_free(array->env);
  array->env = NULL;
}
void ap_lincons1_array_fprint(FILE* stream,
			      ap_lincons1_array_t* array)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(array->env);
  ap_lincons0_array_fprint(stream,&array->lincons0_array,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

ap_lincons1_t ap_lincons1_array_get(ap_lincons1_array_t* array,
				    size_t index)
{
  ap_lincons1_t cons;
  cons.lincons0 = array->lincons0_array.p[index];
  cons.env = array->env;
  return cons;
}

bool ap_lincons1_array_set(ap_lincons1_array_t* array,
			   size_t index, ap_lincons1_t* cons)
{
  if (index>=array->lincons0_array.size || !ap_environment_is_eq(cons->env,array->env))
    return true;
  ap_lincons1_array_clear_index(array,index);
  array->lincons0_array.p[index] = cons->lincons0;
  ap_environment_free(cons->env);
  return false;
}

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

bool
ap_lincons1_array_extend_environment_with(ap_lincons1_array_t* array,
					  ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  ap_lincons0_array_add_dimensions_with(&array->lincons0_array,dimchange);
  env = array->env;
  array->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  ap_environment_free(env);
  return false;
}
bool
ap_lincons1_array_extend_environment(ap_lincons1_array_t* narray,
				     ap_lincons1_array_t* array,
				     ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  narray->lincons0_array = ap_lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  narray->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
