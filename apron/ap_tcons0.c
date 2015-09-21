/* ************************************************************************* */
/* ap_tcons0.c: tree constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_tcons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void ap_tcons0_fprint(FILE* stream, ap_tcons0_t* cons, char** name_of_dim)
{
  ap_texpr0_fprint(stream,cons->texpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	     (cons->constyp == AP_CONS_DISEQ ?
	      " != 0" :
	      "\"ERROR in ap_tcons0_fprint\""))));
  if (cons->constyp == AP_CONS_EQMOD){
    assert(cons->scalar!=NULL);
    fprintf(stream," mod ");
    ap_scalar_fprint(stream,cons->scalar);
  }
}

ap_tcons0_t ap_tcons0_make_unsat()
{
  ap_texpr0_t* expr;

  expr = ap_texpr0_cst_scalar_int(-1);
  return ap_tcons0_make(AP_CONS_SUPEQ,expr,NULL);
}

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_tcons0_array_t ap_tcons0_array_make(size_t size)
{
  ap_tcons0_array_t array;
  size_t i;
  array.size = size;
  array.p = (size==0) ? NULL : (ap_tcons0_t*)malloc(size*sizeof(ap_tcons0_t));
  for (i=0; i<size; i++){
    array.p[i].texpr0 = NULL;
    array.p[i].scalar = NULL;
  }
  return array;
}
void ap_tcons0_array_resize(ap_tcons0_array_t* array, size_t size)
{
  size_t i;
  for (i=size; i<array->size; i++){
    ap_tcons0_clear(&array->p[i]);
  }
  array->p = (ap_tcons0_t*)realloc(array->p,size*sizeof(ap_tcons0_t));
   for (i=array->size; i<size; i++){
    array->p[i].texpr0 = NULL;
    array->p[i].scalar = NULL;
  }
   array->size = size;
}

void ap_tcons0_array_clear(ap_tcons0_array_t* array)
{
  size_t i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      ap_tcons0_clear(&array->p[i]);
    free(array->p);
    array->p=NULL;
  }
}

void ap_tcons0_array_fprint(FILE* stream,
			      ap_tcons0_array_t* array,
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
      ap_tcons0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

bool ap_tcons0_array_is_interval_linear(ap_tcons0_array_t* array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    if (!ap_texpr0_is_interval_linear(array->p[i].texpr0)){
      res = false;
      break;
    }
  }
  return res;
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_tcons0_array_add_dimensions_with(ap_tcons0_array_t* array,
					 ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_texpr0_t* expr = array->p[i].texpr0;
    if (expr) ap_texpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_tcons0_array_t ap_tcons0_array_add_dimensions(ap_tcons0_array_t* array,
						 ap_dimchange_t* dimchange)
{
  size_t i;
  ap_tcons0_array_t narray;

  narray = ap_tcons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_tcons0_add_dimensions(&array->p[i],dimchange);
  }
  return narray;
}

void ap_tcons0_array_remove_dimensions_with(ap_tcons0_array_t* array,
					    ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_texpr0_t* expr = array->p[i].texpr0;
    if (expr) ap_texpr0_remove_dimensions_with(expr,dimchange);
  }
}
ap_tcons0_array_t ap_tcons0_array_remove_dimensions(ap_tcons0_array_t* array,
						    ap_dimchange_t* dimchange)
{
  size_t i;
  ap_tcons0_array_t narray;

  narray = ap_tcons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_tcons0_remove_dimensions(&array->p[i],dimchange);
  }
  return narray;
}

void ap_tcons0_array_permute_dimensions_with(ap_tcons0_array_t* array,
					     ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_texpr0_t* expr = array->p[i].texpr0;
    if (expr) ap_texpr0_permute_dimensions_with(expr,perm);
  }
}
ap_tcons0_array_t ap_tcons0_array_permute_dimensions(ap_tcons0_array_t* array,
						     ap_dimperm_t* perm)
{
  size_t i;
  ap_tcons0_array_t narray;

  narray = ap_tcons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_tcons0_permute_dimensions(&array->p[i],perm);
  }
  return narray;
}
