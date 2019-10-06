/* ************************************************************************* */
/* ap_lincons0.c: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_lincons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void ap_lincons0_print(ap_lincons0_t* cons,
		       char** name_of_dim)
{ ap_lincons0_fprint(stdout,cons,name_of_dim); }
void ap_lincons0_fprint(FILE* stream, ap_lincons0_t* cons, char** name_of_dim)
{
  ap_linexpr0_fprint(stream,cons->linexpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	     (cons->constyp == AP_CONS_DISEQ ?
	      " != 0" :
	      "\"ERROR in ap_lincons0_fprint\""))));
  if (cons->constyp == AP_CONS_EQMOD){
    assert(cons->scalar!=NULL);
    fprintf(stream," mod ");
    ap_scalar_fprint(stream,cons->scalar);
  }
}

ap_lincons0_t ap_lincons0_make_unsat()
{
  ap_linexpr0_t* expr;

  expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0);
  ap_coeff_set_scalar_double(&expr->cst,-1.0);
  return ap_lincons0_make(AP_CONS_SUPEQ,expr,NULL);
}

bool ap_lincons0_is_unsat(ap_lincons0_t* cons)
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
      case AP_CONS_EQMOD:
	return (sgn!=0);
      case AP_CONS_DISEQ:
	return (sgn==0);
      case AP_CONS_SUPEQ:
	return (sgn<0);
      case AP_CONS_SUP:
	return (sgn<=0);
      default:
        abort();
      }
    case AP_COEFF_INTERVAL:
      sgn = ap_scalar_sgn(expr->cst.val.interval->sup);
      switch(cons->constyp){
      case AP_CONS_EQ:
      case AP_CONS_EQMOD:
	return
	  sgn < 0 ||
	  ap_scalar_sgn(expr->cst.val.interval->inf)>0;
      case AP_CONS_DISEQ:
	return
	  sgn>=0 &&
	  ap_scalar_sgn(expr->cst.val.interval->inf)<=0;
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

bool ap_lincons0_is_sat(ap_lincons0_t* cons)
{
  size_t i,nbcoeffs;
  ap_dim_t dim;
  ap_coeff_t* coeff;
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
      switch(cons->constyp){
      case AP_CONS_EQ:
      case AP_CONS_EQMOD:
	return ap_scalar_sgn(expr->cst.val.scalar)==0;
      case AP_CONS_DISEQ:
	return ap_scalar_sgn(expr->cst.val.scalar)!=0;
      case AP_CONS_SUPEQ:
	return ap_scalar_sgn(expr->cst.val.scalar)>=0;
      case AP_CONS_SUP:
	return ap_scalar_sgn(expr->cst.val.scalar)>0;
      default:
        abort();
      }
    case AP_COEFF_INTERVAL:
      if (ap_interval_is_bottom(expr->cst.val.interval)) 
        return true;
      switch(cons->constyp){
      case AP_CONS_EQ:
      case AP_CONS_EQMOD:
	return
          ap_scalar_sgn(expr->cst.val.interval->inf)==0 &&
          ap_scalar_sgn(expr->cst.val.interval->sup)==0;
      case AP_CONS_DISEQ:
        return
          ap_scalar_sgn(expr->cst.val.interval->inf)<0 ||
          ap_scalar_sgn(expr->cst.val.interval->sup)>0;
      case AP_CONS_SUPEQ:
	return 
          ap_scalar_sgn(expr->cst.val.interval->inf)>=0;
      case AP_CONS_SUP:
	return 
          ap_scalar_sgn(expr->cst.val.interval->inf)>0;
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
  size_t i;
  array.size = size;
  array.p = (size==0) ? NULL : (ap_lincons0_t*)malloc(size*sizeof(ap_lincons0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
    array.p[i].scalar = NULL;
  }
  return array;
}
void ap_lincons0_array_resize(ap_lincons0_array_t* array, size_t size)
{
  size_t i;
  for (i=size; i<array->size; i++){
    ap_lincons0_clear(&array->p[i]);
  }
  array->p = (ap_lincons0_t*)realloc(array->p,size*sizeof(ap_lincons0_t));
   for (i=array->size; i<size; i++){
    array->p[i].linexpr0 = NULL;
    array->p[i].scalar = NULL;
  }
   array->size = size;
}

void ap_lincons0_array_clear(ap_lincons0_array_t* array)
{
  size_t i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      ap_lincons0_clear(&array->p[i]);
    free(array->p);
    array->p=NULL;
  }
}

void ap_lincons0_array_print(ap_lincons0_array_t* array,
			     char** name_of_dim)
{ ap_lincons0_array_fprint(stdout,array,name_of_dim); }
void ap_lincons0_array_fprint(FILE* stream,
			      ap_lincons0_array_t* array,
			      char** name_of_dim)
{
  size_t i;

  if (array->size==0){
    fprintf(stream,"empty array of constraints\n");
  } else {
    fprintf(stream,"array of constraints of size %lu\n",
	    (unsigned long)array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2lu: ",(unsigned long)i);
      ap_lincons0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

ap_linexpr_type_t ap_lincons0_array_type(ap_lincons0_array_t* array)
{
  size_t i;
  ap_linexpr_type_t res = AP_LINEXPR_LINEAR;
  for (i=0; i<array->size; i++){
    ap_linexpr_type_t type = ap_linexpr0_type(array->p[i].linexpr0);
    if (type < res)
      type = res;
    if (res==AP_LINEXPR_INTLINEAR)
      break;
  }
  return res;
}
bool ap_lincons0_array_is_quasilinear(ap_lincons0_array_t* array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    if (!ap_linexpr0_is_quasilinear(array->p[i].linexpr0)){
      res = false;
      break;
    }
  }
  return res;
}
bool ap_lincons0_array_is_linear(ap_lincons0_array_t* array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    if (!ap_linexpr0_is_linear(array->p[i].linexpr0)){
      res = false;
      break;
    }
  }
  return res;
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_lincons0_array_add_dimensions_with(ap_lincons0_array_t* array,
					   ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_lincons0_array_t ap_lincons0_array_add_dimensions(ap_lincons0_array_t* array,
						     ap_dimchange_t* dimchange)
{
  size_t i;
  ap_lincons0_array_t narray;

  narray = ap_lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_lincons0_add_dimensions(&array->p[i],dimchange);
  }
  return narray;
}

void ap_lincons0_array_permute_dimensions_with(ap_lincons0_array_t* array,
					       ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_permute_dimensions_with(expr,perm);
  }
}
ap_lincons0_array_t ap_lincons0_array_permute_dimensions(ap_lincons0_array_t* array,
							 ap_dimperm_t* perm)
{
  size_t i;
  ap_lincons0_array_t narray;

  narray = ap_lincons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_lincons0_permute_dimensions(&array->p[i],perm);
  }
  return narray;
}
