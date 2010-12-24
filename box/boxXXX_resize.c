/* ********************************************************************** */
/* boxXXX_resize.c */
/* ********************************************************************** */

#include "boxXXX.h"
#include "boxXXX_internal.h"

boxXXX_t* boxXXX_add_dimensions(ap_manager_t* man,
				bool destructive, boxXXX_t* a,
				ap_dimchange_t* dimchange,
				bool project)
{
  boxXXX_t* res;
  size_t size;
  size_t dimsup;
  int i,k;

  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  res = destructive ? a : boxXXX_copy(man,a);
  if (a->p==NULL){
    goto boxXXX_add_dimensions_exit;
  }
  size = res->intdim+res->realdim;
  dimsup = dimchange->intdim+dimchange->realdim;
  res->p = realloc(res->p,(size+dimsup+1)*sizeof(eitvXXX_t));
  for (i=(int)size+1;i<(int)(size+dimsup+1);i++){
    eitvXXX_init(res->p[i]);
  }
  k = dimsup;
  for (i=(int)size; i>=0; i--){
    if (i<(int)size){
      eitvXXX_set(res->p[i+k],a->p[i]);
    }
    while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
      k--;
      if (project){
	eitvXXX_set_int(res->p[i+k],0);
      }
      else {
	eitvXXX_set_top(res->p[i+k]);
      }
    }
  }  
 boxXXX_add_dimensions_exit:
  res->intdim = a->intdim+dimchange->intdim;
  res->realdim = a->realdim+dimchange->realdim;
  return res;
}

boxXXX_t* boxXXX_remove_dimensions(ap_manager_t* man,
			     bool destructive, boxXXX_t* a,
			     ap_dimchange_t* dimchange)
{
  boxXXX_t* res;
  size_t size;
  size_t dimsup;
  size_t i,k;
  
  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  res = destructive ? a : boxXXX_copy(man,a);
  if (a->p==NULL){
    goto boxXXX_remove_dimensions_exit;
  }
  size = res->intdim+res->realdim;
  dimsup = dimchange->intdim+dimchange->realdim;
  k=0;
  for (i=0; i<size-dimsup; i++){
    while (k<dimsup && dimchange->dim[k]==i+k){
      k++;
    }
    eitvXXX_set(res->p[i],a->p[i+k]);
  }
  eitvXXX_set_int(res->p[size-dimsup],0);
  for (i=size-dimsup+1;i<size+1;i++){
    eitvXXX_clear(res->p[i]);
  }
  res->p = realloc(res->p,(size-dimsup+1)*sizeof(eitvXXX_t));
 boxXXX_remove_dimensions_exit:
  res->intdim = a->intdim-dimchange->intdim;
  res->realdim = a->realdim-dimchange->realdim;
  return res;
}

boxXXX_t* boxXXX_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dimperm_t* perm)
{
  boxXXX_t* res;
  size_t size;
  size_t i;
  
  man->result.flag_best = true;  
  man->result.flag_exact = true;  
  if (a->p==NULL){
    return destructive ? a : boxXXX_copy(man,a);
  }
  res = boxXXX_copy(man,a);
  size = res->intdim+res->realdim;
  for (i=0;i<size;i++){
    eitvXXX_set(res->p[perm->dim[i]],a->p[i]);
  }
  if (destructive) boxXXX_free(man,a);
  return res;
}
			   
