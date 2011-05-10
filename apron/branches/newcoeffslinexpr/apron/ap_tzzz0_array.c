/* ************************************************************************* */
/* ap_tzzz0.c: tree expressions/constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_tzzz0.h"
#include "ap_linzzz0.h"

#include <stdarg.h>

#define _AP_tzzz0_array_MARK_

ap_tzzz0_array_t ap_tzzz0_array_make(size_t size)
{
  ap_tzzz0_array_t array;
  size_t i;
  array.p = (ap_tzzz0_t**)malloc(size*sizeof(ap_tzzz0_t*));
  array.size = size;
  for (i=0;i<size;i++) array.p[i] = NULL;
  return array;
}

void ap_tzzz0_array_resize(ap_tzzz0_array_t* array, size_t size)
{
  size_t i;

  for (i=size; i<array->size; i++){
    ap_tzzz0_free(array->p[i]);
    array->p[i] = NULL;
  }
  array->p = (ap_tzzz0_t**)realloc(array->p,size*sizeof(ap_tzzz0_t*));
  for (i=array->size; i<size; i++){
    array->p[i] = NULL;
  }
  return;
}

ap_tzzz0_array_t ap_tzzz0_array_copy(ap_tzzz0_array_t* array)
{
  ap_tzzz0_array_t res;
  size_t i;
  res.p = (ap_tzzz0_t**)malloc(array->size*sizeof(ap_tzzz0_t*));
  res.size = array->size;
  for (i=0;i<res.size;i++) res.p[i] = ap_tzzz0_copy(array->p[i]);
  return res;
}
void ap_tzzz0_array_clear(ap_tzzz0_array_t* array)
{
  if (array->p){
    size_t i;
    for (i=0; i<array->size; i++){
      if (array->p[i]) ap_tzzz0_free(array->p[i]);
      array->p[i] = NULL;
    }
    free(array->p);
    array->p= NULL;
  }
}

ap_tzzz0_array_t ap_tzzz0_array_from_linzzz0_array(ap_linzzz0_array_t array)
{
  ap_tzzz0_array_t res;
  size_t i;
  ap_linzzz0_t ref;
  res.size = ap_linzzz0_array_size(array);
  res.p = (ap_tzzz0_t**)malloc(res.size*sizeof(ap_tzzz0_t*));
  for (i=0;i<res.size;i++){
    ap_linzzz0_array_ref_index(ref,array,i);
    res.p[i] = ap_tzzz0_from_linzzz0(ref);
  }
  return res;
}

void ap_tzzz0_array_fprint(FILE* stream,
			    ap_tzzz0_array_t* array,
			    char** name_of_dim)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    ap_tzzz0_fprint(stream,array->p[i],name_of_dim);
    fprintf(stream,"\n");
  }
}
void ap_tzzz0_array_print(ap_tzzz0_array_t* array,
			   char** name_of_dim)
{ ap_tzzz0_array_fprint(stdout,array,name_of_dim); }

size_t ap_tzzz0_array_support(ap_tzzz0_array_t* array, ap_dim_t* tdim)
{
  ap_dim_t max,i,r,w;
  size_t size;

  /* compute occurence vector */
  max = 0;
  for (i=0; i<array->size; i++){
    size_t max1 = ap_tzzz0_max_dim(array->p[i]);
    if (max1>max) max = max1;
  }
  if (max==0){
    return 0;
  }
  else {
    for (w=0;w<max;i++){
      tdim[w] = AP_DIM_MAX;
    }
    for (i=0; i<array->size; i++){
      ap_tzzz0_support_internal(array->p[i], tdim);
    }
    w = 0;
    for (r=0; r<max; r++){
      if (tdim[r]!=AP_DIM_MAX){
	tdim[w] = tdim[r];
	w++;
      }
    }
    return (size_t)w;
  }
}

static bool ap_tzzz0_array_is_template(ap_tzzz0_array_t* array, bool (*is_template)(ap_tzzz0_t* tzzz))
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = is_template(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool ap_tzzz0_array_is_interval_linear(ap_tzzz0_array_t* array)
{
  return ap_tzzz0_array_is_template(array,ap_tzzz0_is_interval_linear);
}
bool ap_tzzz0_array_is_interval_polynomial(ap_tzzz0_array_t* array)
{
return ap_tzzz0_array_is_template(array,ap_tzzz0_is_interval_polynomial);
}
bool ap_tzzz0_array_is_interval_polyfrac(ap_tzzz0_array_t* array)
{
  return ap_tzzz0_array_is_template(array,ap_tzzz0_is_interval_polyfrac);
}
bool ap_tzzz0_array_is_scalar(ap_tzzz0_array_t* array)
{
  return ap_tzzz0_array_is_template(array,ap_tzzz0_is_scalar);
}

ap_tzzz0_array_t ap_tzzz0_array_add_dimensions(ap_tzzz0_array_t* array,
						 ap_dimchange_t* change)
{
  size_t i;
  ap_tzzz0_array_t res;

  res = ap_tzzz0_array_make(array->size);
  for (i=0; i<array->size; i++){
    res.p[i] = ap_tzzz0_add_dimensions(array->p[i],change);
  }
  return res;
}
ap_tzzz0_array_t ap_tzzz0_array_permute_dimensions(ap_tzzz0_array_t* array,
						   ap_dimperm_t* perm)
{
  size_t i;
  ap_tzzz0_array_t res;

  res = ap_tzzz0_array_make(array->size);
  for (i=0; i<array->size; i++){
    res.p[i] = ap_tzzz0_permute_dimensions(array->p[i],perm);
  }
  return res;
}
void ap_tzzz0_array_add_dimensions_with(ap_tzzz0_array_t* array,
					ap_dimchange_t* change)
{
  size_t i;
  for (i=0; i<array->size; i++){
    ap_tzzz0_add_dimensions_with(array->p[i],change);
  }
}
void ap_tzzz0_array_permute_dimensions_with(ap_tzzz0_array_t* array,
					    ap_dimperm_t* perm)
{
  size_t i;
  for (i=0; i<array->size; i++){
    ap_tzzz0_permute_dimensions_with(array->p[i],perm);
  }
}
