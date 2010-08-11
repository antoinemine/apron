/* ********************************************************************** */
/* itvXXX_YYY_array.c: */
/* ********************************************************************** */

#include "itvXXX_YYY.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void itvXXX_YYY_array_init(itvXXX_YYY_array_t array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = malloc(size*sizeof(itvXXX_YYY_t));
  for (i=0; i<size; i++) itvXXX_YYY_init(array->p[i],0);
}
void itvXXX_YYY_array_init_set(itvXXX_YYY_array_t res, itvXXX_YYY_array_t array)
{
  size_t i;
  res->size = array->size;
  res->p = malloc(array->size*sizeof(itvXXX_YYY_t));
  for (i=0; i<res->size; i++) itvXXX_YYY_init_set(res->p[i],array->p[i]);
}
void itvXXX_YYY_array_set(itvXXX_YYY_array_t res, itvXXX_YYY_array_t array)
{
  size_t i;
  itvXXX_YYY_array_resize(res,array->size);  
  for (i=0; i<res->size; i++) itvXXX_YYY_set(res->p[i],array->p[i]);
}
void itvXXX_YYY_array_resize(itvXXX_YYY_array_t array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      itvXXX_YYY_clear(array->p[i]);
    }
    array->p = realloc(array->p,size*sizeof(itvXXX_YYY_t));
  }
  else { /* size > array->size */
    array->p = realloc(array->p,size*sizeof(itvXXX_YYY_t));
    for (i=array->size; i<size; i++){
      itvXXX_YYY_init(array->p[i],0);
    }
  }
  array->size = size;
  return;
}
void itvXXX_YYY_array_clear(itvXXX_YYY_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) itvXXX_YYY_clear(array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}
void itvXXX_YYY_array_minimize(itvXXX_YYY_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) itvXXX_YYY_minimize(array->p[i]);
}

itvXXX_YYY_array_ptr itvXXX_YYY_array_alloc(size_t size)
{
  itvXXX_YYY_array_ptr res = (itvXXX_YYY_array_ptr)malloc(sizeof(itvXXX_YYY_array_struct));
  itvXXX_YYY_array_init(res,size);
  return res;
}
itvXXX_YYY_array_ptr itvXXX_YYY_array_alloc_set(itvXXX_YYY_array_t a)
{
  itvXXX_YYY_array_ptr res = (itvXXX_YYY_array_ptr)malloc(sizeof(itvXXX_YYY_array_struct));
  itvXXX_YYY_array_init_set(res,a);
  return res;
}
void itvXXX_YYY_array_free(itvXXX_YYY_array_t a)
{
  itvXXX_YYY_array_clear(a);
  free(a);
}

void itvXXX_YYY_array_fprint(FILE* stream, itvXXX_YYY_array_t array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    itvXXX_YYY_fprint(stream,array->p[i],name);
    fprintf(stream,"\n");
  }
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool itvXXX_YYY_array_is_linear(itvXXX_YYY_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itvXXX_YYY_is_linear(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool itvXXX_YYY_array_is_quasilinear(itvXXX_YYY_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itvXXX_YYY_is_quasilinear(array->p[i]);
    if (!res) break;
  }
  return res;
}
itvlinexpr_type_t itvXXX_YYY_array_type(itvXXX_YYY_array_t array)
{
  size_t i;
  itvlinexpr_type_t type;

  type = ITV_LINEXPR_LINEAR;
  for (i=0; i<array->size; i++){
    itvlinexpr_type_t t = itvXXX_YYY_type(array->p[i]);
    if (t<type){
      type = t;
      if (type==ITV_LINEXPR_INTLINEAR)
	break;
    }
  }
  return type;
}

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

size_t itvXXX_YYY_array_supportinterval(itvXXX_YYY_array_t array,
					    ap_dim_t* tdim, size_t maxdim1)
{
  if (array->size==0){
    return 0;
  }
  else if (array->size==1){
    return itvXXX_YYY_supportinterval(array->p[0],tdim);
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
      tnb[k1] = itvXXX_YYY_supportinterval(array->p[i],ttdim[k1]);
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

void itvXXX_YYY_array_add_dimensions(itvXXX_YYY_array_t res,
				     itvXXX_YYY_array_t array,
				     ap_dimchange_t* dimchange)
{
  size_t i;
  itvXXX_YYY_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    itvXXX_YYY_add_dimensions(res->p[i],array->p[i],dimchange);
  }
}

void itvXXX_YYY_array_permute_dimensions(itvXXX_YYY_array_t res,
					 itvXXX_YYY_array_t array,
					 ap_dimperm_t* dimperm)
{
  size_t i;
  itvXXX_YYY_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    itvXXX_YYY_permute_dimensions(res->p[i],array->p[i],dimperm);
  }
}

