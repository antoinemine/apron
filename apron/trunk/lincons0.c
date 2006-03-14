/* ************************************************************************* */
/* lincons0.c: linear constraints and arrays */
/* ************************************************************************* */

#include "lincons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void lincons0_fprint(FILE* stream, const lincons0_t* cons, char** name_of_dim)
{
  linexpr0_fprint(stream,cons->linexpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == CONS_EQ ?
	  " = 0" :
	  ( cons->constyp == CONS_SUPEQ ?
	    " >= 0" :
	    " > 0" ));
}

lincons0_t lincons0_make_unsat()
{
  linexpr0_t* expr;

  expr = linexpr0_alloc(LINEXPR_DENSE,0);
  coeff_set_scalar_double(&expr->cst,-1.0);
  return lincons0_make(CONS_SUPEQ,expr);
}

bool lincons0_is_unsat(const lincons0_t* cons)
{
  linexpr0_t* expr = cons->linexpr0;
  if (expr->size==0){
    switch (expr->cst.discr){
    case COEFF_SCALAR:
      return scalar_sgn(expr->cst.val.scalar)<0;
    case COEFF_INTERVAL:
      return scalar_sgn(expr->cst.val.interval->inf)<0 ||
	interval_is_bottom(expr->cst.val.interval);
    default:
      return false;
    }
  }
  else
    return false;
}

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

lincons0_array_t lincons0_array_make(size_t size)
{
  lincons0_array_t array;
  int i;
  array.size = size;
  array.p = (size==0) ? NULL : (lincons0_t*)malloc(size*sizeof(lincons0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void lincons0_array_clear(lincons0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      lincons0_clear(&array->p[i]);
    free(array->p);
    array->p=NULL;
  }
}

void lincons0_array_fprint(FILE* stream,
			 const lincons0_array_t* array,
			 char** name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of constraints\n");
  } else {
    fprintf(stream,"array of constraints of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      lincons0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void lincons0_array_add_dimensions_with(lincons0_array_t* array,
					const dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) linexpr0_add_dimensions_with(expr,dimchange);
  }
}
lincons0_array_t lincons0_array_add_dimensions(const lincons0_array_t* array,
					       const dimchange_t* dimchange)
{
  size_t i;
  lincons0_array_t narray;

  narray = lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const linexpr0_t* expr = array->p[i].linexpr0;
    linexpr0_t* nexpr = expr ? linexpr0_add_dimensions(expr,dimchange) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].constyp = array->p[i].constyp;
  }
  return narray;
}

void lincons0_array_permute_dimensions_with(lincons0_array_t* array,
					    const dim_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) linexpr0_permute_dimensions_with(expr,perm);
  }
}
lincons0_array_t lincons0_array_permute_dimensions(const lincons0_array_t* array,
						   const dim_t* perm)
{
  size_t i;
  lincons0_array_t narray;
  
  narray = lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const linexpr0_t* expr = array->p[i].linexpr0;
    linexpr0_t* nexpr = expr ? linexpr0_permute_dimensions(expr,perm) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].constyp = array->p[i].constyp;
  }
  return narray;
}
