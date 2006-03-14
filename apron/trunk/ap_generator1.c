/* ************************************************************************* */
/* ap_generator1.c: linear generators */
/* ************************************************************************* */

#include "ap_generator1.h"

/* ********************************************************************** */
/* I. ap_generator0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

void ap_generator1_clear(ap_generator1_t* gen)
{
  ap_generator0_clear(&gen->generator0);
  if (gen->env){
    ap_environment_free(gen->env); gen->env = NULL;
  }
}

void ap_generator1_fprint(FILE* stream, const ap_generator1_t* gen)
{  
  ap_environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = ap_environment_name_of_dim_alloc(gen->env);
  ap_generator0_fprint(stream, &gen->generator0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

ap_coeff_t* ap_generator1_coeffref(ap_generator1_t* gen, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(gen->env,var);
  return ap_linexpr0_coeffref(gen->generator0.linexpr0,dim);
}
bool ap_generator1_get_coeff(ap_coeff_t* coeff, const ap_generator1_t* gen, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(gen->env,var);
  return ap_linexpr0_get_coeff(coeff,gen->generator0.linexpr0,dim);
}

/*
ap_coeff_t* ap_generator1_set_format_get_pcoeff(char* str,
					va_list* va,
					void* gen,
					bool* b)
{
  ap_var_t var;
  ap_coeff_t* pcoeff;

  switch (*str){
  case '%':
    var = va_arg(*va,ap_var_t);
    pcoeff = ap_generator1_coeffref((ap_generator1_t*)gen,var);
    *b = (pcoeff==NULL);
    break;
  case '@':
    pcoeff = ap_generator1_cstref((ap_generator1_t*)gen);
    *b = false;
    break;
  default:
    fprintf(stderr,
	    "ap_linexpr0_set_format_generic/ap_generator1_set_format_get_pcoeff: bad format string \"%s\"",str);
    abort();
  }
  return pcoeff;
}

bool ap_generator1_set_format(ap_generator1_t* gen, char* fmt, ...)
{
  va_list ap;
  bool b;

  va_start(ap,fmt);
  b = ap_linexpr0_set_format_generic(ap_generator1_set_format_get_pcoeff,
				  gen, fmt, &ap);
  va_end(ap);
  return b;
}
*/

ap_coeff_t* ap_generator1_set_list_get_pcoeff(void* gen, bool cst, va_list* va)
{
  ap_coeff_t* pcoeff;
  if (cst){
    pcoeff = ap_generator1_cstref((ap_generator1_t*)gen);
  } else {
    ap_var_t var = va_arg(*va,ap_var_t);
    pcoeff = ap_generator1_coeffref((ap_generator1_t*)gen,var);
  }
  return pcoeff;
}

bool ap_generator1_set_list(ap_generator1_t* gen, ...)
{
  va_list va;
  bool res;
  va_start(va,gen);
  res = ap_linexpr0_set_list_generic(ap_generator1_set_list_get_pcoeff,
				     gen,&va);
  va_end(va);
  return res;
}


/* ====================================================================== */
/* I.4 Change of dimensions and permutations */
/* ====================================================================== */

bool ap_generator1_extend_environment(ap_generator1_t* ngen,
				 const ap_generator1_t* gen,
				 ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(gen->env,nenv);
  if (dimchange==NULL) return true;
  ngen->generator0 = ap_generator0_add_dimensions(&gen->generator0,dimchange);
  ngen->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
bool ap_generator1_extend_environment_with(ap_generator1_t* gen,
				      ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(gen->env,nenv);
  if (dimchange==NULL) return true;
  ap_generator0_add_dimensions_with(&gen->generator0,dimchange);
  env = gen->env;
  gen->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  ap_environment_free(env);
  return false;
}

/* ********************************************************************** */
/* II. Array of linear generators */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

ap_generator1_array_t ap_generator1_array_make(ap_environment_t* env, size_t size)
{
  ap_generator1_array_t array;
  array.generator0_array = ap_generator0_array_make(size);
  array.env = ap_environment_copy(env);
  return array;
}
void ap_generator1_array_clear(ap_generator1_array_t* array)
{
  ap_generator0_array_clear(&array->generator0_array);
  ap_environment_free(array->env);
  array->env = NULL;
}
void ap_generator1_array_fprint(FILE* stream,
			   const ap_generator1_array_t* array)
{
  ap_environment_name_of_dim_t* name_of_dim;
  
  name_of_dim = ap_environment_name_of_dim_alloc(array->env);
  ap_generator0_array_fprint(stream,&array->generator0_array,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

ap_generator1_t ap_generator1_array_get(const ap_generator1_array_t* array,
			      size_t index)
{
  ap_generator1_t gen;
  gen.generator0 = array->generator0_array.p[index];
  gen.env = array->env;
  return gen;
}

bool ap_generator1_array_set(ap_generator1_array_t* array,
			size_t index, const ap_generator1_t* gen)
{
  if (index>=array->generator0_array.size || !ap_environment_is_eq(gen->env,array->env))
    return true;
  ap_generator1_array_clear_index(array,index);
  array->generator0_array.p[index] = gen->generator0;
  ap_environment_free(gen->env);
  return false;
}

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

bool
ap_generator1_array_extend_environment_with(ap_generator1_array_t* array,
				       ap_environment_t* nenv)
{
  ap_environment_t* env;
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  ap_generator0_array_add_dimensions_with(&array->generator0_array,dimchange);
  env = array->env;
  array->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  ap_environment_free(env);
  return false;
}
bool
ap_generator1_array_extend_environment(ap_generator1_array_t* narray,
				  const ap_generator1_array_t* array,
				  ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(array->env,nenv);
  if (dimchange==NULL) return true;
  narray->generator0_array = ap_generator0_array_add_dimensions(&array->generator0_array,dimchange);
  narray->env = ap_environment_copy(nenv);
  ap_dimchange_free(dimchange);
  return false;
}
