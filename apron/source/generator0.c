/* ************************************************************************* */
/* generator0.c: generators and arrays */
/* ************************************************************************* */

#include "generator0.h"

/* ********************************************************************** */
/* I. Generators */
/* ********************************************************************** */

void generator0_fprint(FILE* stream, const generator0_t* gen, char** name_of_dim)
{
  fprintf(stream,
	  gen->gentyp == GEN_LINE ?
	  "LINE:   " :
	  ( gen->gentyp == GEN_RAY ?
	    "RAY:    " :
	    "VERTEX: " ));
  linexpr0_fprint(stream,gen->linexpr0,name_of_dim);
}

/* ********************************************************************** */
/* II. Array of Generators */
/* ********************************************************************** */

generator0_array_t generator0_array_make(size_t size)
{
  int i;
  generator0_array_t array;

  array.size = size;
  array.p = (size==0) ? NULL : (generator0_t*)malloc(size*sizeof(generator0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void generator0_array_clear(generator0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      generator0_clear(&array->p[i]);
    free(array->p);
    array->p = NULL;
  }
  array->size=0;
}
void generator0_array_fprint(FILE* stream,
			 const generator0_array_t* array,
			 char** name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of generators\n");
  } else {
    fprintf(stream,"array of generator of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      generator0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */

void generator0_array_add_dimensions_with(generator0_array_t* array,
					  const dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) linexpr0_add_dimensions_with(expr,dimchange);
  }
}
generator0_array_t generator0_array_add_dimensions(const generator0_array_t* array,
						   const dimchange_t* dimchange)
{
  size_t i;
  generator0_array_t narray;

  narray = generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const linexpr0_t* expr = array->p[i].linexpr0;
    linexpr0_t* nexpr = expr ? linexpr0_add_dimensions(expr,dimchange) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].gentyp = array->p[i].gentyp;
  }
  return narray;
}

void generator0_array_permute_dimensions_with(generator0_array_t* array,
					      const dim_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    linexpr0_t* expr = array->p[i].linexpr0;
    if (expr) linexpr0_permute_dimensions_with(expr,perm);
  }
}
generator0_array_t generator0_array_permute_dimensions(const generator0_array_t* array,
						       const dim_t* perm)
{
  size_t i;
  generator0_array_t narray;
  
  narray = generator0_array_make(array->size);
  for(i=0; i<array->size; i++){
    const linexpr0_t* expr = array->p[i].linexpr0;
    linexpr0_t* nexpr = expr ? linexpr0_permute_dimensions(expr,perm) : NULL;
    narray.p[i].linexpr0 = nexpr;
    narray.p[i].gentyp = array->p[i].gentyp;
  }
  return narray;
}
