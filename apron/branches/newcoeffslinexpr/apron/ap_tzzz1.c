/* ************************************************************************* */
/* ap_tzzz1.c: tree expression/constraint */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_tzzz1.h"

#include <stdarg.h>

#define _AP_Tzzz1_MARK_


/* ********************************************************************** */
/* I. ap_tzzz1_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Constructors and Destructors */
/* ====================================================================== */

static inline
ap_tzzz1_t* ap_tzzz1_of_tzzz0(ap_environment_t* env, ap_tzzz0_t* tzzz0)
{
  ap_tzzz1_t* res = malloc(sizeof(ap_tzzz1_t));
  res->tzzz0 = tzzz0;
  res->env = ap_environment_copy(env);
  return res;
}

#if defined(_AP_Texpr1_MARK_)
ap_texpr1_t* ap_texpr1_cst(ap_environment_t* env, ap_coeff_t coeff)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst(coeff));
}
ap_texpr1_t* ap_texpr1_var(ap_environment_t* env, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(env, var);
  if (dim==AP_DIM_MAX)
    return NULL;
  else
    return ap_texpr1_of_texpr0(env, ap_texpr0_dim(dim));
}

ap_texpr1_t* ap_texpr1_node(ap_texpr_op_t op, ap_texpr_rtype_t type, ap_texpr_rdir_t dir, ap_texpr1_t* opA, ap_texpr1_t* opB)
{
  if (opB!=NULL && !ap_environment_is_eq(opA->env, opB->env))
    return NULL;

  ap_texpr0_t* texpr0 = ap_texpr0_node(op,type,dir,opA->texpr0,opB ? opB->texpr0 : NULL);
  ap_texpr1_t* texpr1 = ap_texpr1_of_texpr0(opA->env,texpr0);
  ap_environment_free(opA->env);
  free(opA);
  if (opB){
    ap_environment_free(opB->env);
    free(opB);
  }
  return texpr1;
}
ap_texpr1_t* ap_texpr1_unop(ap_texpr_op_t op, ap_texpr1_t* opA, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  return ap_texpr1_node(op,type,dir,opA,NULL);
}
ap_texpr1_t* ap_texpr1_binop(ap_texpr_op_t op, ap_texpr1_t* opA, ap_texpr1_t* opB, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  return ap_texpr1_node(op,type,dir,opA,opB);
}
#else
ap_tcons1_t* ap_tcons1_make(ap_texpr1_t* expr,
			    ap_constyp_t constyp,
			    mpq_t mpq)
{
  ap_tcons1_t* res = (ap_tcons1_t*)malloc(sizeof(ap_tcons1_t));
  res->env = expr->env;
  res->tcons0 = ap_tcons0_make(expr->texpr0,constyp,mpq);
  return res;
}
ap_tcons1_t* ap_tcons1_make_unsat(ap_environment_t* env)
{
  return ap_tcons1_of_tcons0(env,ap_tcons0_make_unsat());
}
#endif

ap_tzzz1_t* ap_tzzz1_copy(ap_tzzz1_t* a)
{
  if (!a) return NULL;
  return ap_tzzz1_of_tzzz0(a->env,ap_tzzz0_copy(a->tzzz0));
}
void ap_tzzz1_free(ap_tzzz1_t* a)
{
  if (!a) return;
  ap_environment_free(a->env);
  ap_tzzz0_free(a->tzzz0);
  free(a);
}
ap_tzzz1_t* ap_tzzz1_from_linzzz1(ap_linzzz1_t e)
{
  return ap_tzzz1_of_tzzz0(e->env, ap_tzzz0_from_linzzz0(e->linzzz0));
}

/* ====================================================================== */
/* I.2 Printing */
/* ====================================================================== */

void ap_tzzz1_fprint(FILE* stream, ap_tzzz1_t* a)
{
  if (!a) return;
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(a->env);
  ap_tzzz0_fprint(stream,a->tzzz0,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* I.3 Tests, size */
/* ====================================================================== */

bool ap_tzzz1_has_var(ap_tzzz1_t* a, ap_var_t var)
{
  if (!a) return false;
  ap_dim_t dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX)
    return false;
  else
    return ap_tzzz0_has_dim(a->tzzz0, dim);
}


/* ====================================================================== */
/* I.4 Operations */
/* ====================================================================== */

#if defined(_AP_Texpr1_MARK_)
ap_texpr1_t* ap_texpr1_substitute(ap_texpr1_t* a, ap_var_t var, ap_texpr1_t *dst)
{
  if (!a || !dst || !ap_environment_is_eq(a->env, dst->env))
    return NULL;
  ap_dim_t dim = ap_environment_dim_of_var(a->env,var);
  if (dim==AP_DIM_MAX)
    return NULL;
  return ap_texpr1_of_texpr0(a->env,
			     ap_texpr0_substitute(a->texpr0,dim,dst->texpr0));
}

/* Return true if var is unknown, or incompatible environment */
bool ap_texpr1_substitute_with(ap_texpr1_t* a, ap_var_t var, ap_texpr1_t *dst)
{
  if (!a || !dst || !ap_environment_is_eq(a->env, dst->env))
    return true;
  ap_dim_t dim = ap_environment_dim_of_var(a->env,var);
  if (dim==AP_DIM_MAX)
    return true;
  ap_texpr0_substitute_with(a->texpr0,dim,dst->texpr0);
  return false;
}
#endif

/* ====================================================================== */
/* I.5 Change of environment */
/* ====================================================================== */

ap_tzzz1_t* ap_tzzz1_extend_environment_dimchange(ap_tzzz1_t* a,
						   ap_environment_t* nenv,
						   ap_dimchange_t* dimchange)
{
  return ap_tzzz1_of_tzzz0(nenv,ap_tzzz0_add_dimensions(a->tzzz0,dimchange));
}
void ap_tzzz1_extend_environment_dimchange_with(ap_tzzz1_t* a,
						 ap_environment_t* nenv,
						 ap_dimchange_t* dimchange)
{
  ap_environment_t* env;
  ap_tzzz0_add_dimensions_with(a->tzzz0,dimchange);
  env = a->env;
  a->env = ap_environment_copy(nenv);
  ap_environment_free(env);
}
ap_tzzz1_t* ap_tzzz1_extend_environment(ap_tzzz1_t* a,
					  ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(a->env,nenv);
  if (dimchange==NULL)
    return NULL;
  ap_tzzz1_t* res = ap_tzzz1_extend_environment_dimchange(a,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return res;
}
bool ap_tzzz1_extend_environment_with(ap_tzzz1_t* a,
				       ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(a->env,nenv);
  if (dimchange==NULL)
    return true;
  ap_tzzz1_extend_environment_dimchange_with(a,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return false;
}


/* ********************************************************************** */
/* II. ap_tzzz1_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

ap_tzzz1_array_t ap_tzzz1_array_make(ap_environment_t* env, size_t size)
{
  ap_tzzz1_array_t array;
  array.tzzz0_array = ap_tzzz0_array_make(size);
  array.env = ap_environment_copy(env);
  return array;
}
void ap_tzzz1_array_clear(ap_tzzz1_array_t* array)
{
  ap_tzzz0_array_clear(&array->tzzz0_array);
  ap_environment_free(array->env);
  array->env = NULL;
}
void ap_tzzz1_array_fprint(FILE* stream,
			      ap_tzzz1_array_t* array)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(array->env);
  ap_tzzz0_array_fprint(stream,&array->tzzz0_array,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

bool ap_tzzz1_array_set_index(ap_tzzz1_array_t* array, size_t i, ap_tzzz1_t* a)
{
  ap_tzzz1_array_free_index(array,i);
  if (ap_environment_is_eq(array->env,a->env)){
    array->tzzz0_array.p[i] = ap_tzzz0_copy(a->tzzz0);
    return false;
  }
  else {
    return true;
  }
}
ap_tzzz1_t* ap_tzzz1_array_get_index(ap_tzzz1_array_t* array, size_t i)
{
  return ap_tzzz1_of_tzzz0(array->env,array->tzzz0_array.p[i]);
}
ap_tzzz1_t ap_tzzz1_array_ref_index(ap_tzzz1_array_t* array, size_t i)
{
  assert(i<array->tzzz0_array.size);
  ap_tzzz1_t res;
  res.env = array->env;
  res.tzzz0 = array->tzzz0_array.p[i];
  return res;
}

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

void ap_tzzz1_array_extend_environment_dimchange_with(
    ap_tzzz1_array_t* array, ap_environment_t* nenv, ap_dimchange_t* dimchange
)
{
  ap_environment_t* env;
  ap_tzzz0_array_add_dimensions_with(&array->tzzz0_array,dimchange);
  env = array->env;
  array->env = ap_environment_copy(nenv);
  ap_environment_free(env);
}
ap_tzzz1_array_t ap_tzzz1_array_extend_environment_dimchange(
    ap_tzzz1_array_t* array, ap_environment_t* nenv, ap_dimchange_t* dimchange
)
{
  ap_tzzz1_array_t narray;
  narray.tzzz0_array = ap_tzzz0_array_add_dimensions(&array->tzzz0_array,dimchange);
  narray.env = ap_environment_copy(nenv);
  return narray;
}
bool ap_tzzz1_array_extend_environment_with(ap_tzzz1_array_t* array,
                                            ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  ap_tzzz1_array_extend_environment_dimchange_with(array,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return false;
}
ap_tzzz1_array_t
ap_tzzz1_array_extend_environment(bool* perror,
                                  ap_tzzz1_array_t* array,
                                  ap_environment_t* nenv)
{
  ap_tzzz1_array_t narray;
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL){
    *perror = true;
  } else {
    *perror = false;
    narray = ap_tzzz1_array_extend_environment_dimchange(array,nenv,dimchange);
    ap_dimchange_free(dimchange);
  }
  return narray;
}
