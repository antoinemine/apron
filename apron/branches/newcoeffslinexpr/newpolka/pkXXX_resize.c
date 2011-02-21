/* ********************************************************************** */
/* pkXXX_resize.c: change and permutation of dimensions  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"

/* ********************************************************************** */
/*  I. Vectors */
/* ********************************************************************** */

/*
  Apply the
  permutation permutation. The result is stored in newq. The array
  permutation is supposed to be of size size-pk->dec.

  BE CAUTIOUS: value 0 in the permutation means columns pk->dec.
*/
static
void vectorXXX_permute_dimensions(pkXXX_internal_t* pk,
				  numintXXX_t* newq, numintXXX_t* q, size_t size,
				  ap_dim_t* permut)
{
  bool destructive;
  size_t j,newj;
  numintXXX_t* nq;

  destructive = (newq==q);

  /* Where to write ?
     If destructive, we write in a temporary vector
     otherwise we write in the destination
  */
  nq = destructive ? pk->vector_numintp : newq;

  /* Fill the non-permuted fields */
  for (j=0; j<pk->dec && j<size; j++){
    numintXXX_set(nq[j],q[j]);
  }
  /* Permutation itself */
  for (j=pk->dec; j<size; j++){
    newj = permut[j - pk->dec] + pk->dec;
    numintXXX_set(nq[newj],q[j]);
  }
  if (destructive){
    for(j=0; j<size; j++){
      numintXXX_set(newq[j],nq[j]);
    }
  }
  return;
}
static
void vectorXXX_add_dimensions(pkXXX_internal_t* pk,
			      numintXXX_t* newq,
			      numintXXX_t* q, size_t size,
			      ap_dimchange_t* dimchange)
{
  int i,k,dimsup;

  if (newq!=q){
    for (i=0;i<(int)pk->dec && i<(int)size; i++)
      numintXXX_set(newq[i],q[i]);
  }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k = dimsup;
  for (i=(int)(size)-(int)pk->dec; i>=0; i--){
    if (i<(int)size-(int)pk->dec){
      numintXXX_set(newq[pk->dec+i+k],q[pk->dec+i]);
    }
    while (k>=1 && dimchange->p[k-1]==(ap_dim_t)i){
      k--;
      numintXXX_set_int(newq[pk->dec+i+k],0);
    }
  }
}

void vectorXXX_remove_dimensions(pkXXX_internal_t* pk,
				 numintXXX_t* newq,
				 numintXXX_t* q, size_t size,
				 ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;

  if (newq!=q){
    for (i=0;i<pk->dec && i<size; i++)
      numintXXX_set(newq[i],q[i]);
  }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k=0;
  for (i=0; i<size-dimsup-pk->dec; i++){
    while (k<dimsup && dimchange->p[k]==i+k){
      k++;
    }
    numintXXX_set(newq[pk->dec+i],q[pk->dec+i+k]);
  }
}

/* ====================================================================== */
/* II. Matrices */
/* ====================================================================== */

/* Modifications of the number of columns in-place */
static
void matrixXXX_resize_diffcols(matrixXXX_t* mat, int diff)
{
  if (diff != 0){
    size_t i;
    for(i=0; i<mat->_maxrows; i++){
      vectorXXX_realloc(&mat->p[i],
			mat->nbcolumns,
			mat->nbcolumns+diff);
    }
    mat->nbcolumns += diff;
  }
}

matrixXXX_t* matrixXXX_add_dimensions(pkXXX_internal_t* pk,
				      bool destructive,
				      matrixXXX_t* mat,
				      ap_dimchange_t* dimchange)
{
  matrixXXX_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  if (destructive){
    nmat = mat;
    matrixXXX_resize_diffcols(nmat,(int)dimsup);
  }
  else {
    nmat = matrixXXX_alloc(mat->nbrows,mat->nbcolumns+dimsup,mat->_sorted);
  }
  for (i=0; i<mat->nbrows; i++){
    vectorXXX_add_dimensions(pk,nmat->p[i],mat->p[i],nmat->nbcolumns-dimsup,dimchange);
  }
  return nmat;
}
static
matrixXXX_t* matrixXXX_remove_dimensions(pkXXX_internal_t* pk,
					 bool destructive,
					 matrixXXX_t* mat,
					 ap_dimchange_t* dimchange)
{
  matrixXXX_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->dim.intd + dimchange->dim.reald;
  nmat =
    destructive ?
    mat :
    matrixXXX_alloc(mat->nbrows, mat->nbcolumns-dimsup, false);
  for (i=0; i<mat->nbrows; i++){
    vectorXXX_remove_dimensions(pk,
				nmat->p[i],
				mat->p[i],
				mat->nbcolumns,
				dimchange);
    vectorXXX_normalize(pk,nmat->p[i],mat->nbcolumns-dimsup);
  }
  if (destructive){
    matrixXXX_resize_diffcols(nmat, -(int)dimsup);
  }
  nmat->_sorted = false;
  return nmat;
}
static
matrixXXX_t* matrixXXX_permute_dimensions(pkXXX_internal_t* pk,
					  bool destructive,
					  matrixXXX_t* mat,
					  ap_dim_t* permutation)
{
  matrixXXX_t* nmat;
  size_t i;

  nmat = destructive ? mat : matrixXXX_alloc(mat->nbrows,mat->nbcolumns,false);
  for (i=0; i<mat->nbrows; i++){
    vectorXXX_permute_dimensions(pk,nmat->p[i],mat->p[i],mat->nbcolumns,permutation);
  }
  nmat->_sorted = false;
  return nmat;
}

/* ********************************************************************** */
/* III. Factorized forms */
/* ********************************************************************** */

/* Standard case of the following functions: addition of
   dimensions, and embedding the polyhedron in the new space.

   Dual version, with new dimensions sets at 0: exchanging
   constraints and generators.
*/
static
pkXXX_t* cherniXXX_add_dimensions(pkXXX_internal_t* pk,
			       bool destructive,
			       pkXXX_t* pa,
			       ap_dimchange_t* dimchange)
{
  pkXXX_t* po;
  int i,k;
  size_t dimsup;

  dimsup = dimchange->dim.intd + dimchange->dim.reald;
  if (destructive){
    po = pa;
    po->dim.intd += dimchange->dim.intd;
    po->dim.reald += dimchange->dim.reald;
  }
  else {
    po = pkXXX_alloc(ap_dimension_add(pa->dim,dimchange->dim));
    po->status = pa->status;
  }

  if (pa->C || pa->F){
    /* Get pa->satC if possible */
    if (pa->satF && pa->satC==NULL){
      (pa)->satC = satmat_transpose(pa->satF,pa->F->nbrows);
    }
    /* Extend constraints */
    if (pa->C){
      po->C = matrixXXX_add_dimensions(pk,destructive,pa->C,dimchange);
      po->C->_sorted = false;
    }
    /* Extend generators and add new lines */
    if (pa->F){
      size_t nbrows = pa->F->nbrows;
      po->F = matrixXXX_add_dimensions(pk,destructive,pa->F,dimchange);
      matrixXXX_resize_rows_lazy(po->F,po->F->nbrows+dimsup);
      /* translate rows [0,oldF->nbrows-1] to [dimsup,oldF->nbrows+dimsup-1] */
      matrixXXX_move_rows(po->F,dimsup,0,nbrows);
      /* addition of new lines at the beginning of the matrix */
      k=dimsup-1;
      for (i=po->dim.intd+po->dim.reald - dimsup; i>=0; i--){
	while (k>=0 && dimchange->p[k]==(ap_dim_t)i){
	  vectorXXX_clear(po->F->p[dimsup-1-k],po->F->nbcolumns);
	  numintXXX_set_int(po->F->p[dimsup-1-k][pk->dec+i+k], 1);
	  k--;
	}
	if (k<0) break;
      }
      po->F->_sorted =
	pa->F->_sorted &&
	(matrixXXX_compare_rows(pk,po->F,dimsup-1,dimsup) < 0);
    }
    if (pa->satC){
      if (destructive){
	satmat_resize_rows(po->satC,po->satC->nbrows+dimsup);
	/* translate rows [0,oldF->nbrows-1] to [dimsup,oldF->nbrows+dimsup-1] */
	satmat_move_rows(po->satC,dimsup,0,po->F->nbrows-dimsup);
	/* the first rows, corresponding to new lines, to zero */
	for (i=0; i<(int)dimsup; i++){
	  bitstring_clear(po->satC->p[i],po->satC->nbcolumns);
	}
      }
      else {
	po->satC = satmat_alloc(pa->satC->nbrows+dimsup, pa->satC->nbcolumns);
	/* the first rows, corresponding to new lines, are already zero */
	for (i=0; i<(int)pa->satC->nbrows; i++){
	  bitstring_copy(po->satC->p[dimsup+i],pa->satC->p[i],pa->satC->nbcolumns);
	}
      }
    }
    /* New saturation matrix: we cannot work with satF */
    if (destructive && dimsup>0 && po->satF){
      satmat_free(po->satF);
      po->satF=NULL;
    }
    if (pa->C && pa->F){
      po->nbeq = pa->nbeq;
      po->nbline = pa->nbline+dimsup;
    }
    else {
      po->nbeq = 0;
      po->nbline = 0;
    }
  }
  return po;
}


/* ====================================================================== */
/* ====================================================================== */

/* ********************************************************************** */
/* IV. Exported functions */
/* ********************************************************************** */

pkXXX_t* pkXXX_add_dimensions(ap_manager_t* man,
			      bool destructive,
			      pkXXX_t* pa,
			      ap_dimchange_t* dimchange,
			      bool project)
{
  pkXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS);
  pkXXX_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald+dimchange->dim.intd+dimchange->dim.reald);

  if (pk->funopt->algorithm>0){
    /* Minimize the argument */
    pkXXX_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      /* we can still operate on the available matrix */
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    if (destructive){
      pa->dim.intd += dimchange->dim.intd;
      pa->dim.reald += dimchange->dim.reald;
      return pa;
    }
    else {
      return pkXXX_bottom(man,ap_dimension_add(pa->dim,dimchange->dim));
    }
  }
  if (project){
    pkXXX_dual(pa);
    po = cherniXXX_add_dimensions(pk, destructive, pa, dimchange);
    pkXXX_dual(po);
    if (!destructive)
      pkXXX_dual(pa);
  }
  else {
    po = cherniXXX_add_dimensions(pk, destructive, pa, dimchange);
  }
  assert(pkXXX_check(pk,po));
  return po;
}

pkXXX_t* pkXXX_remove_dimensions(ap_manager_t* man,
				 bool destructive,
				 pkXXX_t* pa,
				 ap_dimchange_t* dimchange)
{
  pkXXX_t* po;
  size_t dimsup;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS);

  if (pk->funopt->algorithm<=0){
    pkXXX_obtain_F(man,pa,"of the argument");
  } else {
    pkXXX_chernikova(man,pa,"of the argument");
  }
  if (destructive){
    po = pa;
    po->dim.intd -= dimchange->dim.intd;
    po->dim.reald -= dimchange->dim.reald;
    po->status = 0;
    po->nbeq = po->nbline = 0;
  }
  else {
    po = pkXXX_alloc(ap_dimension_sub(pa->dim,dimchange->dim));
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return po;
    }
    /* We can still work with pa->F */
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    if (!destructive){
      pkXXX_set_bottom(pk,po);
    }
    return po;
  }
  if (po->C){ matrixXXX_free(po->C); po->C = NULL; }
  if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
  if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  po->F = matrixXXX_remove_dimensions(pk,destructive,pa->F,dimchange);
  if (pk->funopt->algorithm>0){
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
    }
  }
  man->result.flag_best = man->result.flag_exact =
    dimchange->dim.intd==0;
  assert(pkXXX_check(pk,po));
  return po;
}

pkXXX_t* pkXXX_permute_dimensions(ap_manager_t* man,
				  bool destructive,
				  pkXXX_t* pa,
				  ap_dimperm_t* permutation)
{
  pkXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_PERMUTE_DIMENSIONS);

  if (pk->funopt->algorithm>0){
    /* Minimize the argument */
    pkXXX_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      /* we can still operate on the available matrix */
    }
  }
  man->result.flag_best = man->result.flag_exact = true;
  po = destructive ? pa : pkXXX_alloc(pa->dim);
  if (pa->C){
    po->C = matrixXXX_permute_dimensions(pk,destructive,pa->C,permutation->p);
  }
  if (pa->F){
    po->F = matrixXXX_permute_dimensions(pk,destructive,pa->F,permutation->p);
  }
  if (!destructive){
    po->satC = pa->satC ? satmat_copy(pa->satC) : NULL;
    po->satF = pa->satF ? satmat_copy(pa->satF) : NULL;
    po->nbline = pa->nbline;
    po->nbeq = pa->nbeq;
    po->status = pa->status;
  }
  assert(pkXXX_check(pk,po));
  return po;
}
