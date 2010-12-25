/* ********************************************************************** */
/* boxXXX_otherops.c */
/* ********************************************************************** */

#include "boxXXX.h"
#include "boxXXX_internal.h"

boxXXX_t* boxXXX_forget_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dim_t* tdim,
			      size_t size,
			      bool project)
{
  boxXXX_t* res;
  size_t i;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  
  res = destructive ? a : boxXXX_copy(man,a);
  if (a->p==NULL){
    return res;
  }
  if (project){
    for (i=0;i<size;i++){
      eitvXXX_ptr eitv = res->p[tdim[i]];
      eitvXXX_set_int(eitv,0);
    }
  }
  else {
    for (i=0;i<size;i++){
      eitvXXX_ptr eitv = res->p[tdim[i]];
      eitvXXX_set_top(eitv);
    }
  }
  return res;
}

boxXXX_t* boxXXX_expand(
    ap_manager_t* man, bool destructive,
    boxXXX_t* a, ap_dim_t dim, size_t nbdimsup
)
{
  size_t offset;
  ap_dimension_t dimsup;
  ap_dimchange_t dimchange;
  size_t i;
  boxXXX_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (dim<a->dim.intd){
    dimsup = ap_dimension_make(nbdimsup,0);
    offset = a->dim.intd;
  } else {
    dimsup = ap_dimension_make(0,nbdimsup);
    offset = a->dim.intd+a->dim.reald;
  }
  if (a->p==NULL || nbdimsup==0){
    res = destructive ? a : boxXXX_copy(man,a);
    res->dim = ap_dimension_add(a->dim,dimsup);
    return res;
  }
  ap_dimchange_init(&dimchange,dimsup);
  for (i=0;i<nbdimsup;i++){
    dimchange.p[i]=offset;
  }
  res = boxXXX_add_dimensions(man,destructive,a,&dimchange,false);
  for (i=offset;i<offset+nbdimsup;i++){
    eitvXXX_set(res->p[i],res->p[dim]);
  }
  ap_dimchange_clear(&dimchange);
  return res;
}

boxXXX_t* boxXXX_fold(ap_manager_t* man,
		      bool destructive,
		      boxXXX_t* a,
		      ap_dim_t* tdim,
		      size_t size)
{
  ap_dim_t dim;
  size_t nbdimsup;
  ap_dimension_t dimsup;
  ap_dimchange_t dimchange;
  size_t i;
  boxXXX_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;

  dim = tdim[0];
  nbdimsup = size-1;
  res = destructive ? a : boxXXX_copy(man,a);
  if (dim<a->dim.intd){
    dimsup = ap_dimension_make(nbdimsup,0);
  } else {
    dimsup = ap_dimension_make(0,nbdimsup);
  }
  if (a->p==NULL || nbdimsup==0){
    res->dim = ap_dimension_sub(a->dim,dimsup);
    return res;
  }
  for (i=1; i<size; i++){
    eitvXXX_join(res->p[dim],res->p[dim],res->p[tdim[i]]);
  }
  ap_dimchange_init(&dimchange,dimsup);
  for (i=0;i<nbdimsup;i++){
    dimchange.p[i]=tdim[i+1];
  }
  res = boxXXX_remove_dimensions(man,true,res,&dimchange);
  ap_dimchange_clear(&dimchange);
  return res;
}


boxXXX_t* boxXXX_widening(ap_manager_t* man,
		    boxXXX_t* a1, boxXXX_t* a2)
{
  size_t i;
  size_t nbdims;
  boxXXX_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a1->dim.intd+a1->dim.reald;
  if (a1->p==NULL){
    return boxXXX_copy(man,a2);
  }
  assert(a2->p!=NULL);
  res = boxXXX_copy(man,a1);
  for (i=0; i<nbdims; i++){
    eitvXXX_widening(res->p[i],a1->p[i],a2->p[i]);
  }
  return res;
}

boxXXX_t* boxXXX_closure(ap_manager_t* man, bool destructive, boxXXX_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return destructive ? a : boxXXX_copy(man,a);
}
