/* ************************************************************************* */
/* ap_yyy1_array.c: array of ap_yyy1_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy1_array.h"
#include "ap_linexprconsD_conv.h"
#include "ap_linexprconsMPQ_conv.h"
#include "ap_linexprconsMPFR_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy1_array_init(ap_yyy1_array_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_yyy0_array_init(res->yyy0_array,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_yyy1_array_init_set(ap_yyy1_array_t res, ap_yyy1_array_t e)
{
  ap_yyy0_array_init_set(res->yyy0_array,e->yyy0_array);
  res->env = ap_environment_copy(e->env);
}
void ap_yyy1_array_init_set_yyy0_array(ap_yyy1_array_t res, ap_yyy0_array_t e, ap_environment_t* env)
{
  ap_yyy0_array_init_set(res->yyy0_array,e);
  res->env = ap_environment_copy(env);
}
MACRO_MAINZ
void ap_yyy1_array_init_set_yyyZZZ_array(ap_yyy1_array_t res, ap_yyyZZZ_array_t e, ap_environment_t* env)
{
  ap_yyy0_array_init_set_yyyZZZ_array(res->yyy0_array,e);
  res->env = ap_environment_copy(env);
}
ENDMACRO
void ap_yyy1_array_minimize(ap_yyy1_array_t e)
{ ap_yyy0_array_minimize(e->yyy0_array); }
void ap_yyy1_array_clear(ap_yyy1_array_t e)
{
  ap_yyy0_array_clear(e->yyy0_array);
  ap_environment_free(e->env);
}
void ap_yyy1_array_fprint(FILE* stream, ap_yyy1_array_t e)
{
  ap_environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_yyy0_array_fprint(stream, e->yyy0_array, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II. Access with possible conversions */
/* ====================================================================== */

size_t ap_yyy1_array_size(ap_yyy1_array_t p)
{ return ap_yyy0_array_size(p->yyy0_array); }
ap_linexpr_type_t ap_yyy1_array_type(ap_yyy1_array_t array)
{ return ap_yyy0_array_type(array->yyy0_array); }
bool ap_yyy1_array_is_linear(ap_yyy1_array_t array)
{ return ap_yyy0_array_is_linear(array->yyy0_array); }
bool ap_yyy1_array_is_quasilinear(ap_yyy1_array_t array)
{ return ap_yyy0_array_is_quasilinear(array->yyy0_array); }

bool ap_yyy1_array_set(ap_yyy1_array_t res, ap_yyy1_array_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_yyy0_array_set(res->yyy0_array,e->yyy0_array,intern);
  }
  else
    return true;
}
bool ap_yyy1_array_set_index(ap_yyy1_array_t p, bool* perror, size_t i, ap_yyy1_t a, num_internal_t intern)
{
  *perror = !ap_environment_is_eq(p->env,a->env);
  if (*perror) return false;
  else 
    return ap_yyy0_array_set_index(p->yyy0_array,i,a->yyy0,intern);
}
bool ap_yyy1_array_get_index(ap_yyy1_t a, ap_yyy1_array_t p, size_t i, num_internal_t intern)
{
  ap_environment_set(&a->env,p->env);
  return ap_yyy0_array_get_index(a->yyy0, p->yyy0_array, i, intern);
}
void ap_yyy1_array_ref_index(ap_yyy1_t a, ap_yyy1_array_t p, size_t i)
{
  a->env = p->env;
  ap_yyy0_array_ref_index(a->yyy0,p->yyy0_array,i);
}
bool ap_yyy1_array_set_yyy0_array(ap_yyy1_array_t res, ap_yyy0_array_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_yyy0_array_set(res->yyy0_array,e,intern);
}
MACRO_MAINZ
bool ap_yyy1_array_set_yyyZZZ_array(ap_yyy1_array_t a, ap_yyyZZZ_array_t b, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&a->env,env);
  return ap_yyy0_array_set_yyyZZZ_array(a->yyy0_array,b,intern);
}
bool ap_yyyZZZ_array_set_yyy1_array(ap_yyyZZZ_array_t a, ap_yyy1_array_t b, num_internal_t intern)
{
  return ap_yyyZZZ_array_set_yyy0_array(a,b->yyy0_array,intern);
}
bool ap_yyy1_array_set_index_yyyZZZ(ap_yyy1_array_t p, size_t i, ap_yyyZZZ_t a, num_internal_t intern)
{
  return ap_yyy0_array_set_index_yyyZZZ(p->yyy0_array,i,a,intern);
}
bool ap_yyy1_array_get_index_yyyZZZ(ap_yyyZZZ_t a, ap_yyy1_array_t p, size_t i, num_internal_t intern)
{
  return ap_yyy0_array_get_index_yyyZZZ(a,p->yyy0_array,i,intern);
}
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy1_array_extend_environment(ap_yyy1_array_t nexpr,
				      bool* perror,
				      ap_yyy1_array_t expr,
				      ap_environment_t* nenv)
{
  if (nexpr->yyy0_array->discr != expr->yyy0_array->discr)
    abort();
  SWITCH(nexpr->yyy0_array->discr)
    ap_yyyXXX_array_extend_environment(nexpr->yyy0_array->yyy_array.XXX,perror,
				       nenv,
				       expr->yyy0_array->yyy_array.XXX,
				       expr->env);
  ENDSWITCH
  ap_environment_set(&nexpr->env,nenv);
}
