/* ************************************************************************* */
/* ap_generator0.c: generators and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_generator0.h"

/* ********************************************************************** */
/* I. Generators */
/* ********************************************************************** */

void ap_generator0_fprint(FILE* stream, ap_generator0_t* gen, char** name_of_dim)
{
  char* str = NULL;

  switch(gen->gentyp){
  case AP_GEN_LINE:
    str = "LINE:    ";
    break;
  case AP_GEN_RAY:
    str = "RAY:     ";
    break;
  case AP_GEN_VERTEX:
    str = "VERTEX:  ";
    break;
  case AP_GEN_LINEMOD:
    str = "LINEMOD: ";
    break;
  case AP_GEN_RAYMOD:
    str = "RAYMOD:  ";
    break;
  }
  fprintf(stream,"%s",str);
  ap_linexpr0_fprint(stream,gen->linexpr0,name_of_dim);
}

/* ********************************************************************** */
/* II. Array of Generators */
/* ********************************************************************** */

ap_generator0_array_t ap_generator0_array_make(size_t size)
{
  size_t i;
  ap_generator0_array_t array;

  array.size = size;
  array.p = (size==0) ? NULL : (ap_generator0_t*)malloc(size*sizeof(ap_generator0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}
void ap_generator0_array_resize(ap_generator0_array_t* array, size_t size)
{
  size_t i;
  for (i=size; i<array->size; i++){
    ap_generator0_clear(&array->p[i]);
  }
  array->p = (ap_generator0_t*)realloc(array->p,size*sizeof(ap_generator0_t));
  for (i=array->size; i<size; i++)
    array->p[i].linexpr0 = NULL;
  array->size = size;
}

void ap_generator0_array_clear(ap_generator0_array_t* array)
{
  size_t i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      ap_generator0_clear(&array->p[i]);
    free(array->p);
    array->p = NULL;
  }
  array->size=0;
}
void ap_generator0_array_fprint(FILE* stream,
				ap_generator0_array_t* array,
				char** name_of_dim)
{
  size_t i;

  if (array->size==0){
    fprintf(stream,"empty array of generators\n");
  } else {
    fprintf(stream,"array of generator of size %lu\n",
	    (unsigned long)array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2lu: ",(unsigned long)i);
      ap_generator0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */

void ap_generator0_array_add_dimensions_with(ap_generator0_array_t* array,
					     ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_generator0_array_t ap_generator0_array_add_dimensions(ap_generator0_array_t* array,
							 ap_dimchange_t* dimchange)
{
  size_t i;
  ap_generator0_array_t narray;

  narray = ap_generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] = ap_generator0_add_dimensions(&array->p[i], dimchange);
  }
  return narray;
}

void ap_generator0_array_permute_dimensions_with(ap_generator0_array_t* array,
						 ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_permute_dimensions_with(expr,perm);
  }
}
ap_generator0_array_t ap_generator0_array_permute_dimensions(ap_generator0_array_t* array,
							     ap_dimperm_t* perm)
{
  size_t i;
  ap_generator0_array_t narray;
  
  narray = ap_generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] = ap_generator0_permute_dimensions(&array->p[i], perm);
  }
  return narray;
}
