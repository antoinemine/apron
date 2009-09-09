/* ********************************************************************** */
/* box_otherops.c */
/* ********************************************************************** */

#include "box_internal.h"
#include "box_otherops.h"

box_t* box_forget_array(ap_manager_t* man,
			bool destructive,
			box_t* a,
			ap_dim_t* tdim,
			size_t size,
			bool project)
{
  box_t* res;
  size_t i;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  
  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    return res;
  }
  if (project){
    for (i=0;i<size;i++){
      itv_ptr itv = res->p[tdim[i]];
      bound_set_int(itv->inf,0);
      bound_set_int(itv->sup,0);
    }
  }
  else {
    for (i=0;i<size;i++){
      itv_ptr itv = res->p[tdim[i]];
      itv_set_top(itv);
    }
  }
  return res;
}

box_t* box_expand(ap_manager_t* man,
		  bool destructive,
		  box_t* a,
		  ap_dim_t dim,
		  size_t dimsup)
{
  size_t intdimsup,realdimsup,offset;
  ap_dimchange_t dimchange;
  size_t i;
  box_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (dim<a->intdim){
    intdimsup = dimsup;
    realdimsup = 0;
    offset = a->intdim;
  } else {
    intdimsup = 0;
    realdimsup = dimsup;
    offset = a->intdim+a->realdim;
  }
  if (a->p==NULL || dimsup==0){
    res = destructive ? a : box_copy(man,a);
    res->intdim = a->intdim+intdimsup;
    res->realdim = a->realdim+realdimsup;
    return res;
  }
  ap_dimchange_init(&dimchange,intdimsup,realdimsup);
  for (i=0;i<dimsup;i++){
    dimchange.dim[i]=offset;
  }
  res = box_add_dimensions(man,destructive,a,&dimchange,false);
  for (i=offset;i<offset+dimsup;i++){
    itv_set(res->p[i],res->p[dim]);
  }
  ap_dimchange_clear(&dimchange);
  return res;
}

box_t* box_fold(ap_manager_t* man,
		bool destructive,
		box_t* a,
		ap_dim_t* tdim,
		size_t size)
{
  ap_dim_t dim;
  size_t dimsup,intdimsup,realdimsup;
  ap_dimchange_t dimchange;
  size_t i;
  box_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;

  dim = tdim[0];
  dimsup = size-1;
  res = destructive ? a : box_copy(man,a);
  if (dim<a->intdim){
    intdimsup = dimsup;
    realdimsup = 0;
  } else {
    intdimsup = 0;
    realdimsup = dimsup;
  }
  if (a->p==NULL || dimsup==0){
    res->intdim = a->intdim-intdimsup;
    res->realdim = a->realdim-realdimsup;
    return res;
  }
  for (i=1; i<size; i++){
    itv_join(res->p[dim],res->p[dim],res->p[tdim[i]]);
  }
  ap_dimchange_init(&dimchange,intdimsup,realdimsup);
  for (i=0;i<intdimsup+realdimsup;i++){
    dimchange.dim[i]=tdim[i+1];
  }
  res = box_remove_dimensions(man,true,res,&dimchange);
  ap_dimchange_clear(&dimchange);
  return res;
}


box_t* box_widening(ap_manager_t* man,
		    box_t* a1, box_t* a2)
{
  size_t i;
  size_t nbdims;
  box_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a1->intdim+a1->realdim;
  if (a1->p==NULL){
    return box_copy(man,a2);
  }
  assert(a2->p!=NULL);
  res = box_copy(man,a1);
  for (i=0; i<nbdims; i++){
    itv_widening(res->p[i],a1->p[i],a2->p[i]);
  }
  return res;
}

box_t* box_closure(ap_manager_t* man, bool destructive, box_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return destructive ? a : box_copy(man,a);
}
