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
  if (a->p==NULL || (dest && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return destructive ? a : boxXXX_copy(man,a);
  }
  size = ap_linexpr0_array_size(array);
  if (size==1){
      res = destructive ? a : boxXXX_copy(man,a);
      ap_linexpr0_array_ref_index(linexpr0ref,array,0);
      exact = eitvXXX_eval_ap_linexpr0(res->p[tdim[0]],linexpr0ref,a->p,
				       intern->num);
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<size;i++){
      ap_linexpr0_array_ref_index(linexpr0ref,array,i);
      exact = eitvXXX_eval_ap_linexpr0(res->p[tdim[i]],linexpr0ref,a->p,
				       intern->num) && exact;
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
  
  if (a->p==NULL || (dest && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return destructive ? a : boxXXX_copy(man,a);
  }
  if (array->size==1){
    res = destructive ? a : boxXXX_copy(man,a);
    eitvXXX_eval_ap_texpr0(res->p[tdim[0]],array->p[0],a->p,intern->num);
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<array->size;i++){
      eitvXXX_eval_ap_texpr0(res->p[tdim[i]],array->p[i],a->p,intern->num);
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
