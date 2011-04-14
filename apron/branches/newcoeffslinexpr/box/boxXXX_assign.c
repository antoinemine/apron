/* ********************************************************************** */
/* boxXXX_assign.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boxXXX_internal.h"
#include "ap_linexprXXX.h"
#include "ap_generic.h"

#define _BOXXX_MARK_BOXXX_
  /* Undefined at the end */

boxXXX_t* boxXXX_assign_linexpr_array(ap_manager_t* man,
				      bool destructive,
				      boxXXX_t* a,
				      ap_dim_t* tdim,
				      ap_linexpr0_array_t array,
				      boxXXX_t* dest)
{
  bool exact;
  size_t i,size;
  boxXXX_t* res;
  ap_linexpr0_t linexpr0ref;
  boxXXX_internal_t* intern = man->internal;

  exact = true;
  if (a->e->linterm==NULL || (dest && dest->e->linterm==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    if (destructive){
      if (a->e->linterm) boxXXX_set_bottom(a);
      return a;
    }
    else {
      return boxXXX_bottom(man,a->dim);
    }
  }
  size = ap_linexpr0_array_size(array);
  if (size==1){
      res = destructive ? a : boxXXX_copy(man,a);
      ap_linexpr0_array_ref_index(linexpr0ref,array,0);
      exact = eitvXXX_eval_ap_linexpr0(
	  res->e->linterm[tdim[0]]->eitv,
	  linexpr0ref, a->e, intern->num
      );
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<size;i++){
      ap_linexpr0_array_ref_index(linexpr0ref,array,i);
      exact = eitvXXX_eval_ap_linexpr0(
	  res->e->linterm[tdim[i]]->eitv,
	  linexpr0ref, a->e, intern->num
      ) && exact;
    }
    if (destructive) boxXXX_free(man,a);
  }
  if (dest)
    res = boxXXX_meet(man,true,res,dest);
  man->result.flag_best = size==1 && exact;
  man->result.flag_exact = false;
  return res;
}

boxXXX_t* boxXXX_substitute_linexpr_array(ap_manager_t* man,
					  bool destructive,
					  boxXXX_t* a,
					  ap_dim_t* tdim,
					  ap_linexpr0_array_t array,
					  boxXXX_t* dest)
{
#if defined(_BOD_MARK_BOD_)
  return (boxXXX_t*)ap_generic_asssub_linexpr_array(false,AP_SCALAR_D,
						    man,destructive,a,
						    tdim,array,
						    dest);
#elif defined(_BOMPFR_MARK_BOMPFR_) || defined(_BODl_MARK_BODl_)
  return (boxXXX_t*)ap_generic_asssub_linexpr_array(false,AP_SCALAR_MPFR,
						    man,destructive,a,
						    tdim,array,
						    dest);
#else
  return (boxXXX_t*)ap_generic_asssub_linexpr_array(false,AP_SCALAR_MPQ,
						    man,destructive,a,
						    tdim,array,
						    dest);
#endif

}

boxXXX_t* boxXXX_assign_texpr_array(ap_manager_t* man,
				    bool destructive,
				    boxXXX_t* a,
				    ap_dim_t* tdim,
				    ap_texpr0_array_t* array,
				    boxXXX_t* dest)
{
  size_t i;
  boxXXX_t* res;
  boxXXX_internal_t* intern = man->internal;

  if (a->e->linterm==NULL || (dest && dest->e->linterm==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    if (destructive){
      if (a->e->linterm) boxXXX_set_bottom(a);
      return a;
    }
    else {
      return boxXXX_bottom(man,a->dim);
    }
  }
  if (array->size==1){
    res = destructive ? a : boxXXX_copy(man,a);
    eitvXXX_eval_ap_texpr0(
	res->e->linterm[tdim[0]]->eitv,
	array->p[0],a->e,intern->num);
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<array->size;i++){
      eitvXXX_eval_ap_texpr0(
	  res->e->linterm[tdim[i]]->eitv,
	  array->p[i],a->e,intern->num);
    }
    if (destructive) boxXXX_free(man,a);
  }
  if (dest)
    res = boxXXX_meet(man,true,res,dest);
  man->result.flag_best = false;
  man->result.flag_exact = false;
  return res;
}

boxXXX_t* boxXXX_substitute_texpr_array(ap_manager_t* man,
					bool destructive,
					boxXXX_t* a,
					ap_dim_t* tdim,
					ap_texpr0_array_t* array,
					boxXXX_t* dest)
{
  return (boxXXX_t*)ap_generic_asssub_texpr_array(false,
						  man,destructive,a,
						  tdim,array,
						  dest);
}

#undef _BOXXX_MARK_BOXXX_
