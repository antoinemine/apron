/* ********************************************************************** */
/* pkXXX_project.c: projections  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"

/* ********************************************************************** */
/* I. Factorized form */
/* ********************************************************************** */

static
void poly_projectforget_array(bool project,
			      ap_manager_t* man,
			      pkXXX_t* po, pkXXX_t* pa,
			      ap_dim_t* tdim, size_t size)
{
  bool res;
  matrixXXX_t* mat;
  size_t i,j;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pkXXX_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald);

  res = false;

  /* Get the generator systems, and possibly minimize */
  if (pk->option.forget_lazy)
    pkXXX_obtain_F(man,pa,"of the argument");
  else
    pkXXX_chernikova(man,pa,"of the argument");

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return;
    }
  }
  /* if empty, return empty */
  if (!pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    pkXXX_set_bottom(pk,po);
    return;
  }

  if (project){
    /* Project: assign the dimension to zero */
    if (po==pa){
      /* Forget the other matrices */
      if (po->C){ matrixXXX_free(po->C); po->C = NULL; }
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    } else {
      /* Copy the old one */
      po->F = matrixXXX_copy(pa->F);
    }
    mat = po->F;
    for (i=0; i<mat->nbrows; i++){
      for (j=0; j<size; j++){
	numintXXX_set_int(mat->p[i][pk->dec+tdim[j]],0);
      }
      matrixXXX_normalize_row(pk,mat,(size_t)i);
    }
    mat->_sorted = false; /* renormalization modifies relative order */
    po->status = 0;
    if (!pk->option.forget_lazy){
      pkXXX_chernikova(man,po,"of the result");
    }
  }
  else {
    /* Forget */
    mat = matrixXXX_alloc(size,pa->F->nbcolumns,false);
    for (i=0; i<size; i++){
      numintXXX_set_int(mat->p[i][pk->dec+tdim[i]],1);
    }
    matrixXXX_sort_rows(pk,mat);
    pkXXX_dual(pa);
    if (po!=pa) pkXXX_dual(po);
    if (!pk->option.forget_lazy) pkXXX_obtain_satC(pa);
    res = pkXXX_meet_matrix(false,man,po,pa,mat);
    pkXXX_dual(pa);
    if (po!=pa) pkXXX_dual(po);
    matrixXXX_free(mat);
  }
  if (res || pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!po->F){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return;
    }
  }
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    bool real = true;
    if (pa->dim.intd>0){
      for (i=0; i<size; i++){
	if (tdim[i]<pa->dim.intd){
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
      pa->dim.intd==0;
  }
}

/* ********************************************************************** */
/* II. Exported functions */
/* ********************************************************************** */

pkXXX_t* pkXXX_forget_array(ap_manager_t* man,
			    bool destructive, pkXXX_t* pa,
			    ap_dim_t* tdim, size_t size,
			    bool project)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_FORGET_ARRAY);
  pkXXX_t* po = destructive ? pa : pkXXX_alloc(pa->dim);
  poly_projectforget_array(project, man,po,pa,tdim,size);
  assert(pkXXX_check(pk,po));
  return po;
}
