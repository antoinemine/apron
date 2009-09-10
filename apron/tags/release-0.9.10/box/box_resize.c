/* ********************************************************************** */
/* box_resize.c */
/* ********************************************************************** */

#include "box_internal.h"
#include "box_resize.h"

box_t* box_add_dimensions(ap_manager_t* man,
			  bool destructive, box_t* a,
			  ap_dimchange_t* dimchange,
			  bool project)
{
  box_t* res;
  size_t size;
  size_t dimsup;
  int i,k;

  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    goto box_add_dimensions_exit;
  }
  size = res->intdim+res->realdim;
  dimsup = dimchange->intdim+dimchange->realdim;
  res->p = realloc(res->p,(size+dimsup+1)*sizeof(itv_t));
  for (i=(int)size+1;i<(int)(size+dimsup+1);i++){
    itv_init(res->p[i]);
  }
  k = dimsup;
  for (i=(int)size; i>=0; i--){
    if (i<(int)size){
      itv_set(res->p[i+k],a->p[i]);
    }
    while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
      k--;
      if (project){
	bound_set_int(res->p[i+k]->inf,0);
	bound_set_int(res->p[i+k]->sup,0);
      }
      else {
	itv_set_top(res->p[i+k]);
      }
    }
  }  
 box_add_dimensions_exit:
  res->intdim = a->intdim+dimchange->intdim;
  res->realdim = a->realdim+dimchange->realdim;
  return res;
}

box_t* box_remove_dimensions(ap_manager_t* man,
			     bool destructive, box_t* a,
			     ap_dimchange_t* dimchange)
{
  box_t* res;
  size_t size;
  size_t dimsup;
  size_t i,k;
  
  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    goto box_remove_dimensions_exit;
  }
  size = res->intdim+res->realdim;
  dimsup = dimchange->intdim+dimchange->realdim;
  k=0;
  for (i=0; i<size-dimsup; i++){
    while (k<dimsup && dimchange->dim[k]==i+k){
      k++;
    }
    itv_set(res->p[i],a->p[i+k]);
  }
  itv_set_int(res->p[size-dimsup],0);
  for (i=size-dimsup+1;i<size+1;i++){
    itv_clear(res->p[i]);
  }
  res->p = realloc(res->p,(size-dimsup+1)*sizeof(itv_t));
 box_remove_dimensions_exit:
  res->intdim = a->intdim-dimchange->intdim;
  res->realdim = a->realdim-dimchange->realdim;
  return res;
}

box_t* box_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_dimperm_t* perm)
{
  box_t* res;
  size_t size;
  size_t i;
  
  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  if (a->p==NULL){
    return destructive ? a : box_copy(man,a);
  }
  res = box_copy(man,a);
  size = res->intdim+res->realdim;
  for (i=0;i<size;i++){
    itv_set(res->p[perm->dim[i]],a->p[i]);
  }
  if (destructive) box_free(man,a);
  return res;
}
			   
