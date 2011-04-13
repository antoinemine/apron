/* ************************************************************************* */
/* ap_linyyy1_array.c: array of ap_linyyy1_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linyyy1_array.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_array_init(ap_linyyy1_array_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_linyyy0_array_init(res->linyyy0_array,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_linyyy1_array_init_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e)
{
  ap_linyyy0_array_init_set(res->linyyy0_array,e->linyyy0_array);
  res->env = ap_environment_copy(e->env);
}
void ap_linyyy1_array_init_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env)
{
  ap_linyyy0_array_init_set(res->linyyy0_array,e);
  res->env = ap_environment_copy(env);
}
MACRO_MAINZ
void ap_linyyy1_array_init_set_linyyyZZZ_array(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env)
{
  ap_linyyy0_array_init_set_linyyyZZZ_array(res->linyyy0_array,e);
  res->env = ap_environment_copy(env);
}
ENDMACRO
void ap_linyyy1_array_minimize(ap_linyyy1_array_t e)
{ ap_linyyy0_array_minimize(e->linyyy0_array); }
void ap_linyyy1_array_clear(ap_linyyy1_array_t e)
{
  ap_linyyy0_array_clear(e->linyyy0_array);
  ap_environment_free(e->env);
}
void ap_linyyy1_array_fprint(FILE* stream, ap_linyyy1_array_t e)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_linyyy0_array_fprint(stream, e->linyyy0_array, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II. Access with possible conversions */
/* ====================================================================== */

size_t ap_linyyy1_array_size(ap_linyyy1_array_t p)
{ return ap_linyyy0_array_size(p->linyyy0_array); }
#if !defined(_AP_lingen1_ARRAY_H_)
ap_linexpr_type_t ap_linyyy1_array_type(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_type(array->linyyy0_array); }
bool ap_linyyy1_array_is_quasilinear(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_is_quasilinear(array->linyyy0_array); }
#endif
bool ap_linyyy1_array_is_linear(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_is_linear(array->linyyy0_array); }

bool ap_linyyy1_array_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_linyyy0_array_set(res->linyyy0_array,e->linyyy0_array,intern);
  }
  else
    return true;
}
bool ap_linyyy1_array_set_index(ap_linyyy1_array_t p, bool* perror, size_t i, ap_linyyy1_t a, num_internal_t intern)
{
  *perror = !ap_environment_is_eq(p->env,a->env);
  if (*perror) return false;
  else
    return ap_linyyy0_array_set_index(p->linyyy0_array,i,a->linyyy0,intern);
}
bool ap_linyyy1_array_get_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern)
{
  ap_environment_set(&a->env,p->env);
  return ap_linyyy0_array_get_index(a->linyyy0, p->linyyy0_array, i, intern);
}
void ap_linyyy1_array_ref_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i)
{
  a->env = p->env;
  ap_linyyy0_array_ref_index(a->linyyy0,p->linyyy0_array,i);
}
bool ap_linyyy1_array_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_linyyy0_array_set(res->linyyy0_array,e,intern);
}
MACRO_ALLX
bool ap_linyyy1_array_set_linyyyXXX_array(ap_linyyy1_array_t a, ap_linyyyXXX_array_t b, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&a->env,env);
  return ap_linyyy0_array_set_linyyyXXX_array(a->linyyy0_array,b,intern);
}
bool ap_linyyyXXX_array_set_linyyy1_array(ap_linyyyXXX_array_t a, ap_linyyy1_array_t b, num_internal_t intern)
{
  return ap_linyyyXXX_array_set_linyyy0_array(a,b->linyyy0_array,intern);
}
bool ap_linyyy1_array_set_index_linyyyXXX(ap_linyyy1_array_t p, size_t i, ap_linyyyXXX_t a, num_internal_t intern)
{
  return ap_linyyy0_array_set_index_linyyyXXX(p->linyyy0_array,i,a,intern);
}
bool ap_linyyy1_array_get_index_linyyyXXX(ap_linyyyXXX_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern)
{
  return ap_linyyy0_array_get_index_linyyyXXX(a,p->linyyy0_array,i,intern);
}
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_array_extend_environment(ap_linyyy1_array_t nexpr,
					 bool* perror,
					 ap_linyyy1_array_t expr,
					 ap_environment_t* nenv)
{
  if (nexpr->linyyy0_array->discr != expr->linyyy0_array->discr)
    abort();
  SWITCH(nexpr->linyyy0_array->discr)
    ap_linyyyXXX_array_extend_environment(nexpr->linyyy0_array->linyyy_array.XXX,perror,
					  nenv,
					  expr->linyyy0_array->linyyy_array.XXX,
					  expr->env);
  ENDSWITCH
  ap_environment_set(&nexpr->env,nenv);
}
