/* ********************************************************************** */
/* pk_resize.c: change and permutation of dimensions  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pk_internal.h"

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
			       numintMPQ_t* newq, numintMPQ_t* q, size_t size,
			       ap_dim_t* permut)
{
  bool destructive;
  size_t j,newj;
  numintMPQ_t* nq;
  
  destructive = (newq==q);
  
  /* Where to write ?
     If destructive, we write in a temporary vector
     otherwise we write in the destination
  */
  nq = destructive ? pk->vector_numintp : newq;

  /* Fill the non-permuted fields */
  for (j=0; j<pk->dec && j<size; j++){
    numintMPQ_set(nq[j],q[j]);
  }
  /* Permutation itself */
  for (j=pk->dec; j<size; j++){
    newj = permut[j - pk->dec] + pk->dec;
    numintMPQ_set(nq[newj],q[j]);
  }
  if (destructive){
    for(j=0; j<size; j++){
      numintMPQ_set(newq[j],nq[j]);
    }
  }
  return;
}
static
void vector_add_dimensions(pk_internal_t* pk,
			   numintMPQ_t* newq, 
			   numintMPQ_t* q, size_t size,
			   ap_dimchange_t* dimchange)
{
  int i,k,dimsup;

  if (newq!=q){ 
    for (i=0;i<(int)pk->dec && i<(int)size; i++) 
      numintMPQ_set(newq[i],q[i]);
  }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k = dimsup;
  for (i=(int)(size)-(int)pk->dec; i>=0; i--){
    if (i<(int)size-(int)pk->dec){
      numintMPQ_set(newq[pk->dec+i+k],q[pk->dec+i]);
    }
    while (k>=1 && dimchange->p[k-1]==(ap_dim_t)i){
      k--;
      numintMPQ_set_int(newq[pk->dec+i+k],0);
    }
  }
}

void vector_remove_dimensions(pk_internal_t* pk,
			      numintMPQ_t* newq, 
			      numintMPQ_t* q, size_t size,
			      ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;
  
  if (newq!=q){ 
    for (i=0;i<pk->dec && i<size; i++) 
      numintMPQ_set(newq[i],q[i]);
  }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k=0;
  for (i=0; i<size-dimsup-pk->dec; i++){
    while (k<dimsup && dimchange->p[k]==i+k){
      k++;
    }
    numintMPQ_set(newq[pk->dec+i],q[pk->dec+i+k]);
  }
}

/* ====================================================================== */
/* II. Matrices */
/* ====================================================================== */

/* Modifications of the number of columns in-place */
static
void matrix_resize_diffcols(matrix_t* mat, int diff)
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

matrix_t* matrix_add_dimensions(pk_internal_t* pk,
				bool destructive,
				matrix_t* mat,
				ap_dimchange_t* dimchange)
{
  matrix_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  if (destructive){
    nmat = mat;
    matrix_resize_diffcols(nmat,(int)dimsup);
  }
  else {
    nmat = matrix_alloc(mat->nbrows,mat->nbcolumns+dimsup,mat->_sorted);
  }
  for (i=0; i<mat->nbrows; i++){
    vector_add_dimensions(pk,nmat->p[i],mat->p[i],nmat->nbcolumns-dimsup,dimchange);
  }
  return nmat;
}
static
matrix_t* matrix_remove_dimensions(pk_internal_t* pk,
				   bool destructive,
				   matrix_t* mat,
				   ap_dimchange_t* dimchange)
{
  matrix_t* nmat;
  size_t i,dimsup;

  dimsup = dimchange->dim.intd + dimchange->dim.reald;
  nmat = 
    destructive ? 
    mat : 
    matrix_alloc(mat->nbrows, mat->nbcolumns-dimsup, false);
  for (i=0; i<mat->nbrows; i++){
    vector_remove_dimensions(pk,
			     nmat->p[i],
			     mat->p[i],
			     mat->nbcolumns,
			     dimchange);
    vector_normalize(pk,nmat->p[i],mat->nbcolumns-dimsup);
  }
  if (destructive){
    matrix_resize_diffcols(nmat, -(int)dimsup);
  }
  nmat->_sorted = false;
  return nmat;
}
static
matrix_t* matrix_permute_dimensions(pk_internal_t* pk,
				    bool destructive,
				    matrix_t* mat,
				    ap_dim_t* permutation)
{
  matrix_t* nmat;
  size_t i;

  nmat = destructive ? mat : matrix_alloc(mat->nbrows,mat->nbcolumns,false);
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

  dimsup = dimchange->dim.intd + dimchange->dim.reald;
  if (destructive){
    po = pa;
    po->dim.intd += dimchange->dim.intd;
    po->dim.reald += dimchange->dim.reald;
  }
  else {
    po = poly_alloc(ap_dimension_add(pa->dim,dimchange->dim));
    po->status = pa->status;
  }
  
  if (pa->C || pa->F){
    /* Get pa->satC if possible */
    if (pa->satF && pa->satC==NULL){
      (pa)->satC = satmat_transpose(pa->satF,pa->F->nbrows);
    }
    /* Extend constraints */
    if (pa->C){
      po->C = matrix_add_dimensions(pk,destructive,pa->C,dimchange);
      po->C->_sorted = false;
    }
    /* Extend generators and add new lines */
    if (pa->F){
      size_t nbrows = pa->F->nbrows;
      po->F = matrix_add_dimensions(pk,destructive,pa->F,dimchange);
      matrix_resize_rows_lazy(po->F,po->F->nbrows+dimsup);
      /* translate rows [0,oldF->nbrows-1] to [dimsup,oldF->nbrows+dimsup-1] */
      matrix_move_rows(po->F,dimsup,0,nbrows);
      /* addition of new lines at the beginning of the matrix */
      k=dimsup-1;
      for (i=po->dim.intd+po->dim.reald - dimsup; i>=0; i--){
	while (k>=0 && dimchange->p[k]==(ap_dim_t)i){
	  vector_clear(po->F->p[dimsup-1-k],po->F->nbcolumns);
	  numintMPQ_set_int(po->F->p[dimsup-1-k][pk->dec+i+k], 1);
	  k--;
	}
	if (k<0) break;
      }
      po->F->_sorted =
	pa->F->_sorted && 
	(matrix_compare_rows(pk,po->F,dimsup-1,dimsup) < 0);
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
  pk_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald+dimchange->dim.intd+dimchange->dim.reald);

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
      pa->dim.intd += dimchange->dim.intd;
      pa->dim.reald += dimchange->dim.reald;
      return pa;
    }
    else {
      return pk_bottom(man,ap_dimension_add(pa->dim,dimchange->dim));
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
    po->dim.intd -= dimchange->dim.intd;
    po->dim.reald -= dimchange->dim.reald;
    po->status = 0;
    po->nbeq = po->nbline = 0;
  }
  else {
    po = poly_alloc(ap_dimension_sub(pa->dim,dimchange->dim));
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
  if (po->C){ matrix_free(po->C); po->C = NULL; }
  if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
  if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  po->F = matrix_remove_dimensions(pk,destructive,pa->F,dimchange);
  if (pk->funopt->algorithm>0){
    poly_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
    }
  }
  man->result.flag_best = man->result.flag_exact =
    dimchange->dim.intd==0;
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
  po = destructive ? pa : poly_alloc(pa->dim);
  if (pa->C){
    po->C = matrix_permute_dimensions(pk,destructive,pa->C,permutation->p);
  }
  if (pa->F){
    po->F = matrix_permute_dimensions(pk,destructive,pa->F,permutation->p);
  }
  if (!destructive){
    po->satC = pa->satC ? satmat_copy(pa->satC) : NULL;
    po->satF = pa->satF ? satmat_copy(pa->satF) : NULL;
    po->nbline = pa->nbline;
    po->nbeq = pa->nbeq;
    po->status = pa->status;
  }
  assert(poly_check(pk,po));
  return po;
}
