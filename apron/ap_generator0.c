/* ************************************************************************* */
/* ap_generator0.c: generators and arrays */
/* ************************************************************************* */

#include "ap_generator0.h"

/* ********************************************************************** */
/* I. Generators */
/* ********************************************************************** */

void ap_generator0_fprint(FILE* stream, const ap_generator0_t* gen, char** name_of_dim)
{
  fprintf(stream,
	  gen->gentyp == AP_GEN_LINE ?
	  "LINE:   " :
	  ( gen->gentyp == AP_GEN_RAY ?
	    "RAY:    " :
	    "VERTEX: " ));
  ap_linexpr0_fprint(stream,gen->linexpr0,name_of_dim);
}

/* ********************************************************************** */
/* II. Array of Generators */
/* ********************************************************************** */

ap_generator0_array_t ap_generator0_array_make(size_t size)
{
  int i;
  ap_generator0_array_t array;

  array.size = size;
  array.p = (size==0) ? NULL : (ap_generator0_t*)malloc(size*sizeof(ap_generator0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void ap_generator0_array_clear(ap_generator0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      ap_generator0_clear(&array->p[i]);
    free(array->p);
    array->p = NULL;
  }
  array->size=0;
}
void ap_generator0_array_fprint(FILE* stream,
			 const ap_generator0_array_t* array,
			 char** name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of generators\n");
  } else {
    fprintf(stream,"array of generator of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      ap_generator0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */

void ap_generator0_array_add_dimensions_with(ap_generator0_array_t* array,
					  const ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_generator0_array_t ap_generator0_array_add_dimensions(const ap_generator0_array_t* array,
						   const ap_dimchange_t* dimchange)
{
  size_t i;
  ap_generator0_array_t narray;

  narray = ap_generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const ap_linexpr0_t* expr = array->p[i].linexpr0;
    ap_linexpr0_t* nexpr = expr ? ap_linexpr0_add_dimensions(expr,dimchange) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].gentyp = array->p[i].gentyp;
  }
  return narray;
}

void ap_generator0_array_permute_dimensions_with(ap_generator0_array_t* array,
					      const ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) ap_linexpr0_permute_dimensions_with(expr,perm);
  }
}
ap_generator0_array_t ap_generator0_array_permute_dimensions(const ap_generator0_array_t* array,
						       const ap_dimperm_t* perm)
{
  size_t i;
  ap_generator0_array_t narray;
  
  narray = ap_generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const ap_linexpr0_t* expr = array->p[i].linexpr0;
    ap_linexpr0_t* nexpr = expr ? ap_linexpr0_permute_dimensions(expr,perm) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].gentyp = array->p[i].gentyp;
  }
  return narray;
}
