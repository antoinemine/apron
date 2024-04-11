/* ********************************************************************** */
/* pk_resize.c: change and permutation of dimensions  */
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
#include "pk_representation.h"
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_resize.h"

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
void vector_permute_dimensions(pk_internal_t* pk,
			       numint_t* newq, numint_t* q, size_t size,
			       ap_dim_t* permut)
{
  bool destructive;
  size_t j,newj;
  numint_t* nq;
  
  destructive = (newq==q);
  
  /* Where to write ?
     If destructive, we write in a temporary vector
     otherwise we write in the destination
  */
  nq = destructive ? pk->vector_numintp : newq;

  /* Fill the non-permuted fields */
  for (j=0; j<pk->dec && j<size; j++){
    numint_set(nq[j],q[j]);
  }
  /* Permutation itself */
  for (j=pk->dec; j<size; j++){
    newj = permut[j - pk->dec] + pk->dec;
    numint_set(nq[newj],q[j]);
  }
  if (destructive){
    for(j=0; j<size; j++){
      numint_set(newq[j],nq[j]);
    }
  }
  return;
}
static
void vector_add_dimensions(pk_internal_t* pk,
			   numint_t* newq, 
			   numint_t* q, size_t size,
			   ap_dimchange_t* dimchange)
{
  int i,k,dimsup;

  if (newq!=q){ 
    for (i=0;i<(int)pk->dec && i<(int)size; i++) 
      numint_set(newq[i],q[i]);
  }
  dimsup = dimchange->intdim+dimchange->realdim;
  k = dimsup;
  for (i=(int)(size)-(int)pk->dec; i>=0; i--){
    if (i<(int)size-(int)pk->dec){
      numint_set(newq[pk->dec+i+k],q[pk->dec+i]);
    }
    while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
      k--;
      numint_set_int(newq[pk->dec+i+k],0);
    }
  }
}

void vector_remove_dimensions(pk_internal_t* pk,
			      numint_t* newq, 
			      numint_t* q, size_t size,
			      ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;
  
  if (newq!=q){ 
    for (i=0;i<pk->dec && i<size; i++) 
      numint_set(newq[i],q[i]);
  }
  dimsup = dimchange->intdim+dimchange->realdim;
  k=0;
  for (i=0; i<size-dimsup-pk->dec; i++){
    while (k<dimsup && dimchange->dim[k]==i+k){
      k++;
    }
    numint_set(newq[pk->dec+i],q[pk->dec+i+k]);
  }
}

/* ====================================================================== */
/* II. Matrices */
/* ====================================================================== */

/* Modifications of the number of columns in-place */
static
void pk_matrix_resize_diffcols(pk_matrix_t* mat, int diff)
{
  if (diff != 0){
    size_t i;
    for(i=0; i<mat->_maxrows; i++){
      vector_realloc(&mat->p[i],
		     mat->nbcolumns,
		     mat->nbcolumns+diff);
    }
    mat->nbcolumns += diff;
  }
}

pk_matrix_t* pk_matrix_add_dimensions(pk_internal_t* pk,
				bool destructive,
				pk_matrix_t* mat,
				ap_dimchange_t* dimchange)
{
  pk_matrix_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->intdim+dimchange->realdim;
  if (destructive){
    nmat = mat;
    pk_matrix_resize_diffcols(nmat,(int)dimsup);
  }
  else {
    nmat = pk_matrix_alloc(mat->nbrows,mat->nbcolumns+dimsup,mat->_sorted);
  }
  for (i=0; i<mat->nbrows; i++){
    vector_add_dimensions(pk,nmat->p[i],mat->p[i],nmat->nbcolumns-dimsup,dimchange);
  }
  return nmat;
}
static
pk_matrix_t* pk_matrix_remove_dimensions(pk_internal_t* pk,
				   bool destructive,
				   pk_matrix_t* mat,
				   ap_dimchange_t* dimchange)
{
  pk_matrix_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->intdim + dimchange->realdim;
  nmat = 
    destructive ? 
    mat : 
    pk_matrix_alloc(mat->nbrows, mat->nbcolumns-dimsup, false);
  for (i=0; i<mat->nbrows; i++){
    vector_remove_dimensions(pk,
			     nmat->p[i],
			     mat->p[i],
			     mat->nbcolumns,
			     dimchange);
    vector_normalize(pk,nmat->p[i],mat->nbcolumns-dimsup);
  }
  if (destructive){
    pk_matrix_resize_diffcols(nmat, -(int)dimsup);
  }
  nmat->_sorted = false;
  return nmat;
}
static
pk_matrix_t* pk_matrix_permute_dimensions(pk_internal_t* pk,
				    bool destructive,
				    pk_matrix_t* mat,
				    ap_dim_t* permutation)
{
  pk_matrix_t* nmat;
  size_t i;

  nmat = destructive ? mat : pk_matrix_alloc(mat->nbrows,mat->nbcolumns,false);
  for (i=0; i<mat->nbrows; i++){
    vector_permute_dimensions(pk,nmat->p[i],mat->p[i],mat->nbcolumns,permutation);
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
pk_t* cherni_add_dimensions(pk_internal_t* pk,
			    bool destructive,
			    pk_t* pa,
			    ap_dimchange_t* dimchange)
{
  pk_t* po;
  int i,k;
  size_t dimsup;

  dimsup = dimchange->intdim + dimchange->realdim;
  if (destructive){
    po = pa;
    po->intdim += dimchange->intdim;
    po->realdim += dimchange->realdim;
  }
  else {
    po = poly_alloc(pa->intdim + dimchange->intdim,
		    pa->realdim + dimchange->realdim);
    po->status = pa->status;
  }
  
  if (pa->C || pa->F){
    /* Get pa->satC if possible */
    if (pa->satF && pa->satC==NULL){
      (pa)->satC = satmat_transpose(pa->satF,pa->F->nbrows);
    }
    /* Extend constraints */
    if (pa->C){
      po->C = pk_matrix_add_dimensions(pk,destructive,pa->C,dimchange);
      po->C->_sorted = false;
    }
    /* Extend generators and add new lines */
    if (pa->F){
      size_t nbrows = pa->F->nbrows;
      po->F = pk_matrix_add_dimensions(pk,destructive,pa->F,dimchange);
      pk_matrix_resize_rows_lazy(po->F,po->F->nbrows+dimsup);
      /* translate rows [0,oldF->nbrows-1] to [dimsup,oldF->nbrows+dimsup-1] */
      pk_matrix_move_rows(po->F,dimsup,0,nbrows);
      /* addition of new lines at the beginning of the matrix */
      k=dimsup-1;
      for (i=po->intdim+po->realdim - dimsup; i>=0; i--){
	while (k>=0 && dimchange->dim[k]==(ap_dim_t)i){
	  vector_clear(po->F->p[dimsup-1-k],po->F->nbcolumns);
	  numint_set_int(po->F->p[dimsup-1-k][pk->dec+i+k], 1);
	  k--;
	}
	if (k<0) break;
      }
      po->F->_sorted =
	pa->F->_sorted && 
	(pk_matrix_compare_rows(pk,po->F,dimsup-1,dimsup) < 0);
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

pk_t* pk_add_dimensions(ap_manager_t* man,
			bool destructive,
			pk_t* pa,
			ap_dimchange_t* dimchange,
			bool project)
{
  pk_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS);
  pk_internal_realloc_lazy(pk,pa->intdim+pa->realdim+dimchange->intdim+dimchange->realdim);

  if (pk->funopt->algorithm>0){
    /* Minimize the argument */
    poly_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      /* we can still operate on the available matrix */
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    if (destructive){
      pa->intdim += dimchange->intdim;
      pa->realdim += dimchange->realdim;
      return pa;
    }
    else {
      return pk_bottom(man,
		       pa->intdim + dimchange->intdim,
		       pa->realdim + dimchange->realdim);
    }
  }
  if (project){
    poly_dual(pa);
    po = cherni_add_dimensions(pk, destructive, pa, dimchange);
    poly_dual(po);
    if (!destructive)
      poly_dual(pa);
  }
  else {
    po = cherni_add_dimensions(pk, destructive, pa, dimchange);
  }
  assert(poly_check(pk,po));
  return po;
}

pk_t* pk_remove_dimensions(ap_manager_t* man,
			   bool destructive,
			   pk_t* pa,
			   ap_dimchange_t* dimchange)
{
  pk_t* po;
  size_t dimsup;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS);

  if (pk->funopt->algorithm<=0){
    poly_obtain_F(man,pa,"of the argument");
  } else {
    poly_chernikova(man,pa,"of the argument");
  }
  if (destructive){
    po = pa;
    po->intdim -= dimchange->intdim;
    po->realdim -= dimchange->realdim;
    po->status = 0;
    po->nbeq = po->nbline = 0;
  }
  else {
    po = poly_alloc(pa->intdim - dimchange->intdim,
		    pa->realdim - dimchange->realdim);
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      poly_set_top(pk,po);
      return po;
    }
    /* We can still work with pa->F */
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    if (!destructive){
      poly_set_bottom(pk,po);
    }
    return po;
  }
  if (po->C){ pk_matrix_free(po->C); po->C = NULL; }
  if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
  if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
  dimsup = dimchange->intdim+dimchange->realdim;
  po->F = pk_matrix_remove_dimensions(pk,destructive,pa->F,dimchange);
  if (pk->funopt->algorithm>0){
    poly_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
    }
  }
  man->result.flag_best = man->result.flag_exact =
    dimchange->intdim==0;
  assert(poly_check(pk,po));
  return po;
}

pk_t* pk_permute_dimensions(ap_manager_t* man,
			    bool destructive,
			    pk_t* pa,
			    ap_dimperm_t* permutation)
{
  pk_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_PERMUTE_DIMENSIONS);

  if (pk->funopt->algorithm>0){
    /* Minimize the argument */
    poly_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      /* we can still operate on the available matrix */
    }
  }
  man->result.flag_best = man->result.flag_exact = true;
  po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);
  if (pa->C){
    po->C = pk_matrix_permute_dimensions(pk,destructive,pa->C,permutation->dim);
  }
  if (pa->F){
    po->F = pk_matrix_permute_dimensions(pk,destructive,pa->F,permutation->dim);
  }
  if (!destructive){
    po->satC = pa->satC ? satmat_copy(pa->satC) : NULL;
    po->satF = pa->satF ? satmat_copy(pa->satF) : NULL;
    po->nbline = pa->nbline;
    po->nbeq = pa->nbeq;
  }
  po->status = pa->status & ~pk_status_consgauss & ~pk_status_gengauss;
  assert(poly_check(pk,po));
  return po;
}
