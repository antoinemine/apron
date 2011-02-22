/* ********************************************************************** */
/* ap_linyyyXXX_array.c: */
/* ********************************************************************** */

#include "ap_linyyyXXX.h"

#define _AP_linyyy_array_MARK_

#if defined(_AP_linexpr_array_MARK_) || defined(_AP_lincons_array_MARK_)
#include "ap_tyyy0.h"
#endif

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void ap_linyyyXXX_array_init(ap_linyyyXXX_array_t array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = malloc(size*sizeof(ap_linyyyXXX_t));
  for (i=0; i<size; i++) ap_linyyyXXX_init(array->p[i],0);
}
void ap_linyyyXXX_array_init_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array)
{
  size_t i;
  res->size = array->size;
  res->p = malloc(array->size*sizeof(ap_linyyyXXX_t));
  for (i=0; i<res->size; i++) ap_linyyyXXX_init_set(res->p[i],array->p[i]);
}
void ap_linyyyXXX_array_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<res->size; i++) ap_linyyyXXX_set(res->p[i],array->p[i]);
}
void ap_linyyyXXX_array_resize(ap_linyyyXXX_array_t array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      ap_linyyyXXX_clear(array->p[i]);
    }
    array->p = realloc(array->p,size*sizeof(ap_linyyyXXX_t));
  }
  else { /* size > array->size */
    array->p = realloc(array->p,size*sizeof(ap_linyyyXXX_t));
    for (i=array->size; i<size; i++){
      ap_linyyyXXX_init(array->p[i],0);
    }
  }
  array->size = size;
  return;
}
void ap_linyyyXXX_array_clear(ap_linyyyXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_linyyyXXX_clear(array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}
void ap_linyyyXXX_array_minimize(ap_linyyyXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_linyyyXXX_minimize(array->p[i]);
}

ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc(size_t size)
{
  ap_linyyyXXX_array_ptr res = (ap_linyyyXXX_array_ptr)malloc(sizeof(ap_linyyyXXX_array_struct));
  ap_linyyyXXX_array_init(res,size);
  return res;
}
ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc_set(ap_linyyyXXX_array_t a)
{
  ap_linyyyXXX_array_ptr res = (ap_linyyyXXX_array_ptr)malloc(sizeof(ap_linyyyXXX_array_struct));
  ap_linyyyXXX_array_init_set(res,a);
  return res;
}
void ap_linyyyXXX_array_free(ap_linyyyXXX_array_t a)
{
  ap_linyyyXXX_array_clear(a);
  free(a);
}

void ap_linyyyXXX_array_fprint(FILE* stream, ap_linyyyXXX_array_t array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_fprint(stream,array->p[i],name);
    fprintf(stream,"\n");
  }
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool ap_linyyyXXX_array_is_linear(ap_linyyyXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_linyyyXXX_is_linear(array->p[i]);
    if (!res) break;
  }
  return res;
}

#if defined(_AP_linexpr_array_MARK_) || defined(_AP_lincons_array_MARK_)
bool ap_linyyyXXX_array_is_quasilinear(ap_linyyyXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_linyyyXXX_is_quasilinear(array->p[i]);
    if (!res) break;
  }
  return res;
}
ap_linexpr_type_t ap_linyyyXXX_array_type(ap_linyyyXXX_array_t array)
{
  size_t i;
  ap_linexpr_type_t type;

  type = AP_LINEXPR_LINEAR;
  for (i=0; i<array->size; i++){
    ap_linexpr_type_t t = ap_linyyyXXX_type(array->p[i]);
    if (t<type){
      type = t;
      if (type==AP_LINEXPR_INTLINEAR)
	break;
    }
  }
  return type;
}
#endif

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */
size_t ap_linyyyXXX_array_supportinterval(ap_linyyyXXX_array_t array,
					  ap_dim_t* tdim, size_t maxdim1)
{
  if (array->size==0){
    return 0;
  }
  else if (array->size==1){
    return ap_linyyyXXX_supportinterval(array->p[0],tdim);
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
      tnb[k1] = ap_linyyyXXX_supportinterval(array->p[i],ttdim[k1]);
      ap_dimsupport_merge(ttdim,tnb,&k);
    }
    nb = tnb[k];
    memcpy(tdim,&ttdim[k],nb*sizeof(ap_dim_t));
    free(buffer);
    return nb;
  }
}

#if defined(_AP_linexpr_array_MARK_) || defined(_AP_lincons_array_MARK_)
bool ap_linyyyXXX_array_set_tyyy0_array(ap_linyyyXXX_array_t res,
					bool* perror,
					struct ap_tyyy0_array_t* texpr,
					num_internal_t intern)
{
  size_t i;
  bool exact = true;
  *perror = false;
  ap_linyyyXXX_array_resize(res,texpr->size);
  for (i=0;i<texpr->size;i++){
    exact = ap_linyyyXXX_set_tyyy0(res->p[i],perror,texpr->p[i],intern) && exact;
    if (*perror) break;
  }
  return exact;
}
void ap_linyyyXXX_array_intlinearize_tyyy0_array(ap_linyyyXXX_array_t res,
						 ap_tyyy0_array_t* texpr,
						 eitvXXX_t* env, size_t intdim, num_internal_t intern)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,texpr->size);
  for (i=0;i<texpr->size;i++){
    ap_linyyyXXX_intlinearize_tyyy0(res->p[i],texpr->p[i],env,intdim,intern);
  }
}
#endif

/* ====================================================================== */
/* VI. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyyXXX_array_add_dimensions(ap_linyyyXXX_array_t res,
				     ap_linyyyXXX_array_t array,
				     ap_dimchange_t* dimchange)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_add_dimensions(res->p[i],array->p[i],dimchange);
  }
}

void ap_linyyyXXX_array_permute_dimensions(ap_linyyyXXX_array_t res,
					 ap_linyyyXXX_array_t array,
					 ap_dimperm_t* dimperm)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_permute_dimensions(res->p[i],array->p[i],dimperm);
  }
}
void ap_linyyyXXX_array_extend_environment(ap_linyyyXXX_array_t res,
					   bool* perror,
					   ap_environment_t* nenv,
					   ap_linyyyXXX_array_t array,
					   ap_environment_t* env)
{
  if (env==nenv){
    if (res!=array) ap_linyyyXXX_array_set(res,array);
    *perror = false;
  }
  else {
    size_t i;
    ap_dimchange_t* dimchange = ap_environment_dimchange(env,nenv);
    ap_linyyyXXX_array_resize(res,array->size);
    *perror = (dimchange==NULL);
    if (!(*perror)){
      for (i=0; i<array->size; i++){
	ap_linyyyXXX_add_dimensions(res->p[i],array->p[i],dimchange);
	ap_dimchange_free(dimchange);
      }
    }
  }
}
