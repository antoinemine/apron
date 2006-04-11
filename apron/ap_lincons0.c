/* ************************************************************************* */
/* ap_lincons0.c: linear constraints and arrays */
/* ************************************************************************* */

#include "ap_lincons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void ap_lincons0_fprint(FILE* stream, const ap_lincons0_t* cons, char** name_of_dim)
{
  ap_linexpr0_fprint(stream,cons->linexpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    " > 0" ));
}

ap_lincons0_t ap_lincons0_make_unsat()
{
  ap_linexpr0_t* expr;

  expr = ap_linexpr0_alloc(AP_LINEXPR_DENSE,0);
  ap_coeff_set_scalar_double(&expr->cst,-1.0);
  return ap_lincons0_make(AP_CONS_SUPEQ,expr);
}

bool ap_lincons0_is_unsat(const ap_lincons0_t* cons)
{
  size_t i,nbcoeffs;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  int sgn;
  ap_linexpr0_t* expr = cons->linexpr0;
  
  nbcoeffs = 0;
  ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
    if (!ap_coeff_zero(coeff)){
      nbcoeffs++;
      if (nbcoeffs>0) break;
    }
  }
  if (nbcoeffs==0){
    switch (expr->cst.discr){
    case AP_COEFF_SCALAR:
      sgn = ap_scalar_sgn(expr->cst.val.scalar);
      switch(cons->constyp){
      case AP_CONS_EQ:
	return (sgn!=0);
      case AP_CONS_SUPEQ:
	return (sgn<0);
      case AP_CONS_SUP:
	return (sgn<=0);
      default:
	abort();
      }
    case AP_COEFF_INTERVAL:
      switch(cons->constyp){
	sgn = ap_scalar_sgn(expr->cst.val.interval->sup);
      case AP_CONS_EQ:
	return 
	  sgn < 0 ||
	  ap_scalar_sgn(expr->cst.val.interval->inf)>0;
      case AP_CONS_SUPEQ:
	return sgn<0;
      case AP_CONS_SUP:
	return (sgn<=0);
      default:
	abort();
      }	
    default:
      abort();
    }
  }
  else
    return false;
}

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_lincons0_array_t ap_lincons0_array_make(size_t size)
{
  ap_lincons0_array_t array;
  int i;
  array.size = size;
  array.p = (size==0) ? NULL : (ap_lincons0_t*)malloc(size*sizeof(ap_lincons0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void ap_lincons0_array_clear(ap_lincons0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      ap_lincons0_clear(&array->p[i]);
    free(array->p);
    array->p=NULL;
  }
}

void ap_lincons0_array_fprint(FILE* stream,
			 const ap_lincons0_array_t* array,
			 char** name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of constraints\n");
  } else {
    fprintf(stream,"array of constraints of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      ap_lincons0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_lincons0_array_add_dimensions_with(ap_lincons0_array_t* array,
					const ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_lincons0_array_t ap_lincons0_array_add_dimensions(const ap_lincons0_array_t* array,
					       const ap_dimchange_t* dimchange)
{
  size_t i;
  ap_lincons0_array_t narray;

  narray = ap_lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const ap_linexpr0_t* expr = array->p[i].linexpr0;
    ap_linexpr0_t* nexpr = expr ? ap_linexpr0_add_dimensions(expr,dimchange) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].constyp = array->p[i].constyp;
  }
  return narray;
}

void ap_lincons0_array_permute_dimensions_with(ap_lincons0_array_t* array,
						  const ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_permute_dimensions_with(expr,perm);
  }
}
ap_lincons0_array_t ap_lincons0_array_permute_dimensions(const ap_lincons0_array_t* array,
							       const ap_dimperm_t* perm)
{
  size_t i;
  ap_lincons0_array_t narray;
  
  narray = ap_lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const ap_linexpr0_t* expr = array->p[i].linexpr0;
    ap_linexpr0_t* nexpr = expr ? ap_linexpr0_permute_dimensions(expr,perm) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].constyp = array->p[i].constyp;
  }
  return narray;
}
