/* ********************************************************************** */
/* boxXXX_otherops.c */
/* ********************************************************************** */

#include "boxXXX_internal.h"
#include "boxXXX_otherops.h"

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
      eitvXXX_ptr itv = res->p[tdim[i]];
      boundXXX_set_int(itv->inf,0);
      boundXXX_set_int(itv->sup,0);
    }
  }
  else {
    for (i=0;i<size;i++){
      eitvXXX_ptr itv = res->p[tdim[i]];
      eitvXXX_set_top(itv);
    }
  }
  return res;
}

boxXXX_t* boxXXX_expand(ap_manager_t* man,
		  bool destructive,
		  boxXXX_t* a,
		  ap_dim_t dim,
		  size_t dimsup)
{
  size_t intdimsup,realdimsup,offset;
  ap_dimchange_t dimchange;
  size_t i;
  boxXXX_t* res;

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
    res = destructive ? a : boxXXX_copy(man,a);
    res->intdim = a->intdim+intdimsup;
    res->realdim = a->realdim+realdimsup;
    return res;
  }
  ap_dimchange_init(&dimchange,intdimsup,realdimsup);
  for (i=0;i<dimsup;i++){
    dimchange.dim[i]=offset;
  }
  res = boxXXX_add_dimensions(man,destructive,a,&dimchange,false);
  for (i=offset;i<offset+dimsup;i++){
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
  size_t dimsup,intdimsup,realdimsup;
  ap_dimchange_t dimchange;
  size_t i;
  boxXXX_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = true;

  dim = tdim[0];
  dimsup = size-1;
  res = destructive ? a : boxXXX_copy(man,a);
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
    eitvXXX_join(res->p[dim],res->p[dim],res->p[tdim[i]]);
  }
  ap_dimchange_init(&dimchange,intdimsup,realdimsup);
  for (i=0;i<intdimsup+realdimsup;i++){
    dimchange.dim[i]=tdim[i+1];
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
  nbdims = a1->intdim+a1->realdim;
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
