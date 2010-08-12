/* ********************************************************************** */
/* ap_YYYXXX_array.c: */
/* ********************************************************************** */

#include "ap_YYYXXX.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void ap_YYYXXX_array_init(ap_YYYXXX_array_t array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = malloc(size*sizeof(ap_YYYXXX_t));
  for (i=0; i<size; i++) ap_YYYXXX_init(array->p[i],0);
}
void ap_YYYXXX_array_init_set(ap_YYYXXX_array_t res, ap_YYYXXX_array_t array)
{
  size_t i;
  res->size = array->size;
  res->p = malloc(array->size*sizeof(ap_YYYXXX_t));
  for (i=0; i<res->size; i++) ap_YYYXXX_init_set(res->p[i],array->p[i]);
}
void ap_YYYXXX_array_set(ap_YYYXXX_array_t res, ap_YYYXXX_array_t array)
{
  size_t i;
  ap_YYYXXX_array_resize(res,array->size);  
  for (i=0; i<res->size; i++) ap_YYYXXX_set(res->p[i],array->p[i]);
}
void ap_YYYXXX_array_resize(ap_YYYXXX_array_t array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      ap_YYYXXX_clear(array->p[i]);
    }
    array->p = realloc(array->p,size*sizeof(ap_YYYXXX_t));
  }
  else { /* size > array->size */
    array->p = realloc(array->p,size*sizeof(ap_YYYXXX_t));
    for (i=array->size; i<size; i++){
      ap_YYYXXX_init(array->p[i],0);
    }
  }
  array->size = size;
  return;
}
void ap_YYYXXX_array_clear(ap_YYYXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_YYYXXX_clear(array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}
void ap_YYYXXX_array_minimize(ap_YYYXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_YYYXXX_minimize(array->p[i]);
}

ap_YYYXXX_array_ptr ap_YYYXXX_array_alloc(size_t size)
{
  ap_YYYXXX_array_ptr res = (ap_YYYXXX_array_ptr)malloc(sizeof(ap_YYYXXX_array_struct));
  ap_YYYXXX_array_init(res,size);
  return res;
}
ap_YYYXXX_array_ptr ap_YYYXXX_array_alloc_set(ap_YYYXXX_array_t a)
{
  ap_YYYXXX_array_ptr res = (ap_YYYXXX_array_ptr)malloc(sizeof(ap_YYYXXX_array_struct));
  ap_YYYXXX_array_init_set(res,a);
  return res;
}
void ap_YYYXXX_array_free(ap_YYYXXX_array_t a)
{
  ap_YYYXXX_array_clear(a);
  free(a);
}

void ap_YYYXXX_array_fprint(FILE* stream, ap_YYYXXX_array_t array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    ap_YYYXXX_fprint(stream,array->p[i],name);
    fprintf(stream,"\n");
  }
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool ap_YYYXXX_array_is_linear(ap_YYYXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_YYYXXX_is_linear(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool ap_YYYXXX_array_is_quasilinear(ap_YYYXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_YYYXXX_is_quasilinear(array->p[i]);
    if (!res) break;
  }
  return res;
}
ap_linexpr_type_t ap_YYYXXX_array_type(ap_YYYXXX_array_t array)
{
  size_t i;
  ap_linexpr_type_t type;

  type = AP_LINEXPR_LINEAR;
  for (i=0; i<array->size; i++){
    ap_linexpr_type_t t = ap_YYYXXX_type(array->p[i]);
    if (t<type){
      type = t;
      if (type==AP_LINEXPR_INTLINEAR)
	break;
    }
  }
  return type;
}

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

size_t ap_YYYXXX_array_supportinterval(ap_YYYXXX_array_t array,
					    ap_dim_t* tdim, size_t maxdim1)
{
  if (array->size==0){
    return 0;
  }
  else if (array->size==1){
    return ap_YYYXXX_supportinterval(array->p[0],tdim);
  }
  else {
    size_t i,k,nb;
    ap_dim_t* buffer;
    ap_dim_t* ttdim[3];
    size_t tnb[3];

    buffer = (ap_dim_t*)malloc(3*maxdim1*sizeof(ap_dim_t));
    for (i=0; i<3; i++){
      ttdim[i] = &buffer[i*maxdim1];
      tnb[i] = 0;
    }
    k = 0;
    for (i=0; i<array->size; i++){
      size_t k1 = (k+1)%3 ;
      tnb[k1] = ap_YYYXXX_supportinterval(array->p[i],ttdim[k1]);
      itvXXX_support_merge(ttdim,tnb,&k);
    }
    nb = tnb[k];
    memcpy(tdim,&ttdim[k],nb*sizeof(ap_dim_t));
    free(buffer);
    return nb;
  }
}

/* ====================================================================== */
/* VI. Change of dimensions and permutations */
/* ====================================================================== */

void ap_YYYXXX_array_add_dimensions(ap_YYYXXX_array_t res,
				     ap_YYYXXX_array_t array,
				     ap_dimchange_t* dimchange)
{
  size_t i;
  ap_YYYXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_YYYXXX_add_dimensions(res->p[i],array->p[i],dimchange);
  }
}

void ap_YYYXXX_array_permute_dimensions(ap_YYYXXX_array_t res,
					 ap_YYYXXX_array_t array,
					 ap_dimperm_t* dimperm)
{
  size_t i;
  ap_YYYXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_YYYXXX_permute_dimensions(res->p[i],array->p[i],dimperm);
  }
}
