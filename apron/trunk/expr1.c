/* ************************************************************************* */
/* expr1.c: datatypes for dimensions, expressions and constraints */
/* ************************************************************************* */

#include <stdlib.h>
#include <assert.h>
#include "coeff.h"
#include "expr0.h"
#include "expr1.h"
#include "permutation.h"

/* ********************************************************************** */
/* I. Linear and affine expressions  */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

linexpr1_t linexpr1_make(environment_t* env,
			 linexpr_discr_t lin_discr, 
			 size_t size)
{
  linexpr1_t e;
  e.linexpr0 = linexpr0_alloc(lin_discr,
			      lin_discr==LINEXPR_DENSE ? 
			      env->intdim+env->realdim :
			      size);
  e.env = environment_copy(env);
  return e;
}
linexpr1_t linexpr1_copy(const linexpr1_t* e)
{
  linexpr1_t res;
  res.linexpr0 = linexpr0_copy(e->linexpr0);
  res.env = environment_copy(e->env);
  return res;
}
void linexpr1_clear(linexpr1_t* e)
{
  if (e->linexpr0){
    linexpr0_free(e->linexpr0); e->linexpr0 = NULL;
  }
  if (e->env){
    environment_deref(e->env);
    e->env = NULL;
  }
}
void linexpr1_fprint(FILE* stream, const linexpr1_t* e)
{
  environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = environment_name_of_dim_alloc(e->env);
  linexpr0_fprint(stream, e->linexpr0, name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

/* Return NULL if var undeclared */
coeff_t* linexpr1_coeffref(linexpr1_t* expr, var_t var)
{
  dim_t dim;
  
  dim = environment_dim_of_var(expr->env,var);
  return linexpr0_coeffref(expr->linexpr0,dim);
}
/* Return true if var undeclared */
bool linexpr1_get_coeff(coeff_t* coeff, const linexpr1_t* expr, var_t var)
{
  dim_t dim;
  
  dim = environment_dim_of_var(expr->env,var);
  return linexpr0_get_coeff(coeff,expr->linexpr0,dim);
}


/* ====================================================================== */
/* I.4 Changing of environment */
/* ====================================================================== */

linexpr1_t linexpr1_extend_environment_dimchange(const linexpr1_t* expr,
						 environment_t* nenv,
						 dimchange_t* dimchange)
{
  linexpr1_t nexpr;
  nexpr.linexpr0 = linexpr0_add_dimensions(expr->linexpr0,dimchange);
  nexpr.env = environment_copy(nenv);
  return nexpr;
}
void linexpr1_extend_environment_dimchange_with(linexpr1_t* expr,
						environment_t* nenv,
						dimchange_t* dimchange)
{
  environment_t* env;
  linexpr0_add_dimensions_with(expr->linexpr0,dimchange);
  env = expr->env;
  expr->env = environment_copy(nenv);
  environment_deref(env);
}
bool linexpr1_extend_environment(linexpr1_t* nexpr,
				 const linexpr1_t* expr,
				 environment_t* nenv)
{
  dimchange_t* dimchange = environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return true;
  *nexpr = linexpr1_extend_environment_dimchange(expr,nenv,dimchange);
  free(dimchange);
  return false;
}
bool linexpr1_extend_environment_with(linexpr1_t* expr,
				      environment_t* nenv)
{
  dimchange_t* dimchange = environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return true;
  linexpr1_extend_environment_dimchange_with(expr,nenv,dimchange);
  free(dimchange);
  return false;
}

/* ********************************************************************** */
/* II. Linear constraints */
/* ********************************************************************** */

void lincons1_clear(lincons1_t* cons)
{
  lincons0_clear(&cons->lincons0);
  if (cons->env){
    environment_deref(cons->env); cons->env = NULL;
  }
}

void lincons1_fprint(FILE* stream, const lincons1_t* cons)
{  
  environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = environment_name_of_dim_alloc(cons->env);
  lincons0_fprint(stream, &cons->lincons0, name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}

lincons1_t lincons1_make_unsat(environment_t* env)
{
  lincons1_t lincons1;
  lincons1.lincons0 = lincons0_make_unsat();
  lincons1.env = environment_copy(env);
  return lincons1;
}

bool lincons1_get_coeff(coeff_t* coeff, const lincons1_t* cons, var_t var)
{
  dim_t dim = environment_dim_of_var(cons->env,var);
  return linexpr0_get_coeff(coeff,cons->lincons0.linexpr0,dim);
}
bool lincons1_set_coeff(lincons1_t* cons, var_t var, const coeff_t* coeff)
{
  dim_t dim = environment_dim_of_var(cons->env,var);
  return linexpr0_set_coeff(cons->lincons0.linexpr0,dim,coeff);
}

void lincons1_extend_environment_dimchange_with(lincons1_t* cons,
						environment_t* env, dimchange_t* dimchange)
{
  linexpr1_t linexpr1 = lincons1_get_linexpr1(cons);
  linexpr1_extend_environment_dimchange_with(&linexpr1,env,dimchange);
  cons->env = linexpr1.env;
}
lincons1_t lincons1_extend_environment_dimchange(const lincons1_t* cons,
					    environment_t* env, dimchange_t* dimchange)
{
  const linexpr1_t linexpr1 = lincons1_get_linexpr1(cons);
  linexpr1_t nlinexpr1 = linexpr1_extend_environment_dimchange(&linexpr1,env,dimchange);
  return lincons1_make(cons->lincons0.constyp,&nlinexpr1);
}
bool lincons1_extend_environment(lincons1_t* ncons,
				 const lincons1_t* cons,
				 environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(cons->env,env);
  if (dimchange==NULL) return true;
  *ncons = lincons1_extend_environment_dimchange(cons,env,dimchange);
  free(dimchange);
  return false;
}
bool lincons1_extend_environment_with(lincons1_t* cons,
				      environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(cons->env,env);
  if (dimchange==NULL) return true;
  lincons1_extend_environment_dimchange_with(cons,env,dimchange);
  free(dimchange);
  return false;
}

/* ********************************************************************** */
/* III. Array of constraints  */
/* ********************************************************************** */

lincons1_array_t lincons1_array_make(environment_t* env, size_t size)
{
  lincons1_array_t array;
  array.env = environment_copy(env);
  array.lincons0_array = lincons0_array_make(size);
  return array;
}
void lincons1_array_clear(lincons1_array_t* array)
{
  lincons0_array_clear(&array->lincons0_array);
  environment_deref(array->env);
  array->env = NULL;
}
void lincons1_array_fprint(FILE* stream, 
                         const lincons1_array_t* array)
{
  environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = environment_name_of_dim_alloc(array->env);
  lincons0_array_fprint(stream,&array->lincons0_array,name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}
lincons1_t lincons1_array_get(const lincons1_array_t* array, 
                                    size_t index){
  lincons1_t cons;
  cons.lincons0 = array->lincons0_array.p[index];
  cons.env = array->env;
  return cons;
}
bool lincons1_array_set(lincons1_array_t* array, 
                              size_t index, const lincons1_t* cons)
{
  if (index>=array->lincons0_array.size || cons->env != array->env)
    return true;
  lincons1_array_clear_index(array,index);
  array->lincons0_array.p[index] = cons->lincons0;
  environment_deref(array->env);
  return false;
}

void lincons1_array_extend_environment_dimchange_with(lincons1_array_t* array,
						      environment_t* nenv, dimchange_t* dimchange)
{
  size_t i;
  environment_t* env;
  for(i=0; i<array->lincons0_array.size; i++){
    linexpr0_t* expr = array->lincons0_array.p[i].linexpr0;
    linexpr0_add_dimensions_with(expr,dimchange);
  }
  env = array->env;
  array->env = environment_copy(nenv);
  environment_deref(env);
}
lincons1_array_t
lincons1_array_extend_environment_dimchange(const lincons1_array_t* array,
				       environment_t* env, dimchange_t* dimchange)
{
  lincons1_array_t narray;
  size_t i;

  narray = lincons1_array_make(env,array->lincons0_array.size);
  for(i=0; i<array->lincons0_array.size; i++){
    const linexpr0_t* expr = array->lincons0_array.p[i].linexpr0;
    linexpr0_t* nexpr = linexpr0_add_dimensions(expr,dimchange);
    narray.lincons0_array.p[i] = lincons0_make(array->lincons0_array.p[i].constyp,nexpr);
  }
  return narray;
}
bool lincons1_array_extend_environment(lincons1_array_t* narray,
				       const lincons1_array_t* array,
				       environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(array->env,env);
  if (dimchange==NULL) return true;
  *narray = lincons1_array_extend_environment_dimchange(array,env,dimchange);
  free(dimchange);
  return false;
}
bool lincons1_array_extend_environment_with(lincons1_array_t* array,
					    environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(array->env,env);
  if (dimchange==NULL) return true;
  lincons1_array_extend_environment_dimchange_with(array,env,dimchange);
  free(dimchange);
  return false;
}

/* ********************************************************************** */
/* III. Generator */
/* ********************************************************************** */

void generator1_clear(generator1_t* gen)
{
  generator0_clear(&gen->generator0);
  if (gen->env){
    environment_deref(gen->env); gen->env = NULL;
  }
}
void generator1_fprint(FILE* stream, const generator1_t* gen)
{
  environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = environment_name_of_dim_alloc(gen->env);
  generator0_fprint(stream, &gen->generator0, name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}
bool generator1_get_coeff(coeff_t* coeff, const generator1_t* gen, var_t var)
{
  dim_t dim = environment_dim_of_var(gen->env,var);
  return linexpr0_get_coeff(coeff,gen->generator0.linexpr0,dim);
}
bool generator1_set_coeff(generator1_t* gen, var_t var, const coeff_t* coeff)
{
  dim_t dim = environment_dim_of_var(gen->env,var);
  return linexpr0_set_coeff(gen->generator0.linexpr0,dim,coeff);
}
void generator1_extend_environment_dimchange_with(generator1_t* gen,
					   environment_t* env, dimchange_t* dimchange)
{
  linexpr1_t linexpr1 = generator1_get_linexpr1(gen);
  linexpr1_extend_environment_dimchange_with(&linexpr1,env,dimchange);
  gen->env = linexpr1.env;
}
generator1_t generator1_extend_environment_dimchange(const generator1_t* gen,
						environment_t* env, dimchange_t* dimchange)
{
  const linexpr1_t linexpr1 = generator1_get_linexpr1(gen);
  linexpr1_t nlinexpr1 = linexpr1_extend_environment_dimchange(&linexpr1,env,dimchange);
  return generator1_make(gen->generator0.gentyp,&nlinexpr1);
}
bool generator1_extend_environment(generator1_t* ngen,
				   const generator1_t* gen,
				   environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(gen->env,env);
  if (dimchange==NULL) return true;
  *ngen = generator1_extend_environment_dimchange(gen,env,dimchange);
  free(dimchange);
  return false;
}
bool generator1_extend_environment_with(generator1_t* gen,
					environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(gen->env,env);
  if (dimchange==NULL) return true;
  generator1_extend_environment_dimchange_with(gen,env,dimchange);
  free(dimchange);
  return false;
}

/* ********************************************************************** */
/* IV. Array of generators */
/* ********************************************************************** */

generator1_array_t generator1_array_make(environment_t* env, size_t size)
{
  generator1_array_t array;
  array.env = environment_copy(env);
  array.generator0_array = generator0_array_make(size);
  return array;
}
void generator1_array_clear(generator1_array_t* array)
{
  generator0_array_clear(&array->generator0_array);
  environment_deref(array->env);
  array->env = NULL;
}
void generator1_array_fprint(FILE* stream, 
			     const generator1_array_t* array)
{
  environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = environment_name_of_dim_alloc(array->env);
  generator0_array_fprint(stream,&array->generator0_array,name_of_dim->p);
  environment_name_of_dim_free(name_of_dim);
}
generator1_t generator1_array_get(const generator1_array_t* array, 
					size_t index)
{
  generator1_t gen;
  gen.generator0 = array->generator0_array.p[index];
  gen.env = array->env;
  return gen;
}
bool generator1_array_set(generator1_array_t* array, 
				size_t index, const generator1_t* gen)
{
  if (index>=array->generator0_array.size || gen->env != array->env)
    return true;
  generator1_array_clear_index(array,index);
  array->generator0_array.p[index] = gen->generator0;
  environment_deref(array->env);
  return false;
}

void generator1_array_extend_environment_dimchange_with(generator1_array_t* array,
						      environment_t* nenv, dimchange_t* dimchange)
{
  size_t i;
  environment_t* env;
  for(i=0; i<array->generator0_array.size; i++){
    linexpr0_t* expr = array->generator0_array.p[i].linexpr0;
    linexpr0_add_dimensions_with(expr,dimchange);
  }
  env = array->env;
  array->env = environment_copy(nenv);
  environment_deref(env);
}
generator1_array_t
generator1_array_extend_environment_dimchange(const generator1_array_t* array,
				       environment_t* env, dimchange_t* dimchange)
{
  generator1_array_t narray;
  size_t i;

  narray = generator1_array_make(env,array->generator0_array.size);
  for(i=0; i<array->generator0_array.size; i++){
    const linexpr0_t* expr = array->generator0_array.p[i].linexpr0;
    linexpr0_t* nexpr = linexpr0_add_dimensions(expr,dimchange);
    narray.generator0_array.p[i] = generator0_make(array->generator0_array.p[i].gentyp,nexpr);
  }
  return narray;
}
bool generator1_array_extend_environment(generator1_array_t* narray,
				       const generator1_array_t* array,
				       environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(array->env,env);
  if (dimchange==NULL) return true;
  *narray = generator1_array_extend_environment_dimchange(array,env,dimchange);
  free(dimchange);
  return false;
}
bool generator1_array_extend_environment_with(generator1_array_t* array,
					    environment_t* env)
{
  dimchange_t* dimchange = environment_dimchange(array->env,env);
  if (dimchange==NULL) return true;
  generator1_array_extend_environment_dimchange_with(array,env,dimchange);
  free(dimchange);
  return false;
}

/* ====================================================================== */
/* VI. Boxes */
/* ====================================================================== */
void box1_fprint(FILE* stream,
                 const box1_t* box)
{
  size_t i;
  fprintf(stream,"Box of dimensions (%2d,%2d):\n",
	  box->env->intdim,box->env->realdim);
  for (i=0; i<box->env->intdim+box->env->realdim; i++){
    environment_manager->fprint(stream,environment_var_of_dim(box->env,i));
    fprintf(stream," in ");
    interval_fprint(stream,box->p[i]);
    fprintf(stream,"\n");
  }
}
void box1_clear(box1_t* box)
{
  size_t i;
  for (i=0; i<box->env->intdim+box->env->realdim; i++){
    interval_free(box->p[i]);
  }
  free(box->p);
  environment_deref(box->env);
  box->p = NULL;
  box->env = NULL;
}

