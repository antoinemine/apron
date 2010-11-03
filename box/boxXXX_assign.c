/* ********************************************************************** */
/* boxXXX_assign.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boxXXX_internal.h"
#include "boxXXX_constructor.h"
#include "boxXXX_assign.h"

#include "ap_generic.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

boxXXX_t* boxXXX_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				boxXXX_t* a,
				ap_dim_t* tdim, 
				ap_linexpr0_t** texpr,
				size_t size,
				boxXXX_t* dest)
{
  bool exact;
  size_t i;
  boxXXX_t* res;
  boxXXX_internal_t* intern = man->internal;
  
  exact = true;
  if (a->p==NULL || (dest && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return destructive ? a : boxXXX_copy(man,a);
  }
  if (size==1){
      res = destructive ? a : boxXXX_copy(man,a);
      exact = itv_eval_ap_linexpr0(intern->itv,
				   res->p[tdim[0]],texpr[0],a->p);
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<size;i++){
      exact = itv_eval_ap_linexpr0(intern->itv,
				   res->p[tdim[i]],texpr[i],a->p) && exact;
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
				    ap_linexpr0_t** texpr,
				    size_t size,
				    boxXXX_t* dest)
{
  return (boxXXX_t*)ap_generic_substitute_linexpr_array(man,destructive,a,
						     tdim,texpr,size,
						     dest);
}

boxXXX_t* boxXXX_assign_texpr(ap_manager_t* man,
			bool destructive,
			boxXXX_t* a,
			ap_dim_t dim, ap_texpr0_t* texpr,
			boxXXX_t* dest)
{
  bool exact;
  boxXXX_t* res;
  boxXXX_internal_t* intern = man->internal;

  res = destructive ? a : boxXXX_copy(man,a);
  if (a->p==NULL || (dest!=NULL && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return res;
  }
  itv_eval_ap_texpr0(intern->itv,res->p[dim],texpr,a->p);
  if (dest)
    res = boxXXX_meet(man,true,res,dest);
  man->result.flag_best = false;
  man->result.flag_exact = false;
  return res;
}

boxXXX_t* boxXXX_assign_texpr_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dim_t* tdim, 
			      ap_texpr0_t** texpr,
			      size_t size,
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
  if (size==1){
    res = destructive ? a : boxXXX_copy(man,a);
    itv_eval_ap_texpr0(intern->itv,res->p[tdim[0]],texpr[0],a->p);
  }
  else {
    res = boxXXX_copy(man,a);
    for (i=0;i<size;i++){
      itv_eval_ap_texpr0(intern->itv,res->p[tdim[i]],texpr[i],a->p);
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
				  ap_texpr0_t** texpr,
				  size_t size,
				  boxXXX_t* dest)
{
  return (boxXXX_t*)ap_generic_substitute_texpr_array(man,destructive,a,
						   tdim,texpr,size,
						   dest);
}

boxXXX_t* boxXXX_substitute_texpr(ap_manager_t* man,
			    bool destructive,
			    boxXXX_t* a,
			    ap_dim_t dim, ap_texpr0_t* texpr,
			    boxXXX_t* dest)
{
  return (boxXXX_t*)ap_generic_substitute_texpr_array(man,destructive,a,
						   &dim,&texpr,1,
						   dest);
}

