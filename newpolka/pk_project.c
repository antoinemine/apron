/* ********************************************************************** */
/* pk_project.c: projections  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_bit.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_assign.h"
#include "pk_meetjoin.h"
#include "pk_representation.h"


/* ********************************************************************** */
/* I. Factorized form */
/* ********************************************************************** */

static
void poly_projectforget_array(bool project,
			      bool lazy,
			      ap_manager_t* man,	
			      pk_t* po, pk_t* pa, 
			      ap_dim_t* tdim, size_t size)
{
  bool res;
  pk_matrix_t* mat;
  size_t i,j;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  pk_internal_realloc_lazy(pk,pa->intdim+pa->realdim);
  
  res = false;

  /* Get the generator systems, and possibly minimize */
  if (lazy)
    poly_obtain_F(man,pa,"of the argument");
  else
    poly_chernikova(man,pa,"of the argument");
  
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      poly_set_top(pk,po);
      return;
    }
  }
  /* if empty, return empty */
  if (!pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    poly_set_bottom(pk,po);
    return;
  }
  
  if (project){
    /* Project: assign the dimension to zero */
    if (po==pa){
      /* Forget the other matrices */
      if (po->C){ pk_matrix_free(po->C); po->C = NULL; }
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    } else {
      /* Copy the old one */
      po->F = pk_matrix_copy(pa->F);
    }
    mat = po->F;
    for (i=0; i<mat->nbrows; i++){
      for (j=0; j<size; j++){
	numint_set_int(mat->p[i][pk->dec+tdim[j]],0);
      }
      pk_matrix_normalize_row(pk,mat,(size_t)i);
    }
    po->status = 0;
    if (!lazy){
      poly_chernikova(man,po,"of the result");
    }
  } 
  else {
    /* Forget */
    mat = pk_matrix_alloc(size,pa->F->nbcolumns,false);
    for (i=0; i<size; i++){
      numint_set_int(mat->p[i][pk->dec+tdim[i]],1);
    }
    pk_matrix_sort_rows(pk,mat);
    poly_dual(pa);
    if (po!=pa) poly_dual(po);
    if (!lazy) poly_obtain_satC(pa);
    res = poly_meet_matrix(false,lazy,man,po,pa,mat);
    poly_dual(pa);
    if (po!=pa) poly_dual(po);
    pk_matrix_free(mat);
  }
  if (res || pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!po->F){
      man->result.flag_best = man->result.flag_exact = false;
      poly_set_top(pk,po);
      return;
    }
  }
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    bool real = true;
    if (pa->intdim>0){
      for (i=0; i<size; i++){
	if (tdim[i]<pa->intdim){
	  real = false;
	  break;
	}
      }
    }
    man->result.flag_best = man->result.flag_exact = 
      real;
  }
  else {
    man->result.flag_best = man->result.flag_exact =
      pa->intdim==0;
  }
}

/* ********************************************************************** */
/* II. Exported functions */
/* ********************************************************************** */

pk_t* pk_forget_array(ap_manager_t* man, 
		      bool destructive, pk_t* pa, 
		      ap_dim_t* tdim, size_t size,
		      bool project)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_FORGET_ARRAY);
  pk_t* po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);
  poly_projectforget_array(project,
			   pk->funopt->algorithm<=0,
			   man,po,pa,tdim,size);
  assert(poly_check(pk,po));
  return po;
}
