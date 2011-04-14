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
  if (a->e->linterm==NULL){
    goto boxXXX_add_dimensions_exit;
  }
  size = ap_dimension_size(a->dim);
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  ap_linexprXXX_resize(res->e,size+dimsup+1);
  for (i=(int)size+1;i<(int)(size+dimsup+1);i++){
    res->e->linterm[i]->dim = (size_t)i;
    eitvXXX_init(res->e->linterm[i]->eitv);
  }
  k = dimsup;
  for (i=(int)size; i>=0; i--){
    if (i<(int)size){
      eitvXXX_set(res->e->linterm[i+k]->eitv,a->e->linterm[i]->eitv);
    }
    while (k>=1 && dimchange->p[k-1]==(ap_dim_t)i){
      k--;
      if (project){
	eitvXXX_set_int(res->e->linterm[i+k]->eitv,0);
      }
      else {
	eitvXXX_set_top(res->e->linterm[i+k]->eitv);
      }
    }
  }
 boxXXX_add_dimensions_exit:
  res->dim.intd = a->dim.intd+dimchange->dim.intd;
  res->dim.reald = a->dim.reald+dimchange->dim.reald;
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
  if (a->e->linterm==NULL){
    goto boxXXX_remove_dimensions_exit;
  }
  size = ap_dimension_size(res->dim);
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k=0;
  for (i=0; i<size-dimsup; i++){
    while (k<dimsup && dimchange->p[k]==i+k){
      k++;
    }
    eitvXXX_set(res->e->linterm[i]->eitv,a->e->linterm[i+k]->eitv);
  }
  eitvXXX_set_int(res->e->linterm[size-dimsup]->eitv,0);
  ap_linexprXXX_resize(res->e,size+1-dimsup);
 boxXXX_remove_dimensions_exit:
  res->dim.intd = a->dim.intd-dimchange->dim.intd;
  res->dim.reald = a->dim.reald-dimchange->dim.reald;
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
  if (a->e->linterm==NULL){
    return destructive ? a : boxXXX_copy(man,a);
  }
  res = boxXXX_copy(man,a);
  size = ap_dimension_size(res->dim);
  for (i=0;i<size;i++){
    eitvXXX_set(res->e->linterm[perm->p[i]]->eitv,a->e->linterm[i]->eitv);
  }
  if (destructive) boxXXX_free(man,a);
  return res;
}
