/* ********************************************************************** */
/* box_assign.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box_constructor.h"
#include "box_assign.h"

#include "ap_generic.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

box_t* box_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				box_t* a,
				ap_dim_t* tdim, 
				ap_linexpr0_t** texpr,
				size_t size,
				box_t* dest)
{
  typedef void* (*boxfunction_meet_ptr)(ap_manager_t* standard_manager, bool destructive, box_t* a1, box_t* a2);

  bool exact;
  size_t i;
  box_t* res;
  box_internal_t* intern = man->internal;
  boxfunction_meet_ptr funmeet_ptr = (boxfunction_meet_ptr) man->funptr[AP_FUNID_MEET];
  
  exact = true;
  if (a->p==NULL || (dest && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return destructive ? a : box_copy(man,a);
  }
  if (size==1){
      res = destructive ? a : box_copy(man,a);
      exact = itv_eval_ap_linexpr0(intern->itv,
				   res->p[tdim[0]],texpr[0],a->p);
  }
  else {
    res = box_copy(man,a);
    for (i=0;i<size;i++){
      exact = itv_eval_ap_linexpr0(intern->itv,
				   res->p[tdim[i]],texpr[i],a->p) && exact;
    }
    if (destructive) box_free(man,a);
  }
  if (dest)
    res = (*funmeet_ptr)(man,true,res,dest); /* box_meet except for policy iteration */
  man->result.flag_best = size==1 && exact;
  man->result.flag_exact = false;
  return res;
}

box_t* box_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive,
				    box_t* a,
				    ap_dim_t* tdim, 
				    ap_linexpr0_t** texpr,
				    size_t size,
				    box_t* dest)
{
  return (box_t*)ap_generic_substitute_linexpr_array(man,destructive,a,
						     tdim,texpr,size,
						     dest);
}

box_t* box_assign_texpr(ap_manager_t* man,
			bool destructive,
			box_t* a,
			ap_dim_t dim, ap_texpr0_t* texpr,
			box_t* dest)
{
  typedef void* (*boxfunction_meet_ptr)(ap_manager_t* standard_manager, bool destructive, box_t* a1, box_t* a2);
  bool exact;
  box_t* res;
  box_internal_t* intern = man->internal;
  boxfunction_meet_ptr funmeet_ptr = (boxfunction_meet_ptr) man->funptr[AP_FUNID_MEET];

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL || (dest!=NULL && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return res;
  }
  itv_eval_ap_texpr0(intern->itv,res->p[dim],texpr,a->p);
  if (dest)
    res = (*funmeet_ptr)(man,true,res,dest); /* box_meet except for policy iteration */
  man->result.flag_best = false;
  man->result.flag_exact = false;
  return res;
}

box_t* box_assign_texpr_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_dim_t* tdim, 
			      ap_texpr0_t** texpr,
			      size_t size,
			      box_t* dest)
{
  typedef void* (*boxfunction_meet_ptr)(ap_manager_t* standard_manager, bool destructive, box_t* a1, box_t* a2);
  size_t i;
  box_t* res;
  box_internal_t* intern = man->internal;
  boxfunction_meet_ptr funmeet_ptr = (boxfunction_meet_ptr) man->funptr[AP_FUNID_MEET];
  
  if (a->p==NULL || (dest && dest->p==NULL)){
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return destructive ? a : box_copy(man,a);
  }
  if (size==1){
    res = destructive ? a : box_copy(man,a);
    itv_eval_ap_texpr0(intern->itv,res->p[tdim[0]],texpr[0],a->p);
  }
  else {
    res = box_copy(man,a);
    for (i=0;i<size;i++){
      itv_eval_ap_texpr0(intern->itv,res->p[tdim[i]],texpr[i],a->p);
    }
    if (destructive) box_free(man,a);
  }
  if (dest)
    res = (*funmeet_ptr)(man,true,res,dest); /* box_meet except for policy iteration */
  man->result.flag_best = false;
  man->result.flag_exact = false;
  return res;
}

box_t* box_substitute_texpr_array(ap_manager_t* man,
				  bool destructive,
				  box_t* a,
				  ap_dim_t* tdim, 
				  ap_texpr0_t** texpr,
				  size_t size,
				  box_t* dest)
{
  return (box_t*)ap_generic_substitute_texpr_array(man,destructive,a,
						   tdim,texpr,size,
						   dest);
}

box_t* box_substitute_texpr(ap_manager_t* man,
			    bool destructive,
			    box_t* a,
			    ap_dim_t dim, ap_texpr0_t* texpr,
			    box_t* dest)
{
  return (box_t*)ap_generic_substitute_texpr_array(man,destructive,a,
						   &dim,&texpr,1,
						   dest);
}

