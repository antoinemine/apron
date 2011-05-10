/* ********************************************************************** */
/* pkXXX_expandfold.c: expanding and folding dimensions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "pkXXX_cherni.h"

/* ********************************************************************** */
/* I. Expand */
/* ********************************************************************** */

/* ---------------------------------------------------------------------- */
/* Matrix */
/* ---------------------------------------------------------------------- */

/* Expand the dimension dim of the matrix into (dimsup+1)
   dimensions, with dimsup new dimensions inserted just before
   offset. */
static
matrixXXX_t* matrixXXX_expand(pkXXX_internal_t* pk,
			      bool destructive,
			      matrixXXX_t* C,
			      ap_dim_t dim,
			      size_t offset,
			      size_t dimsup)
{
  ap_dimchange_t dimchange;
  size_t i,j,row,col,nb;
  size_t nbrows, nbcols;
  numintXXX_t** p;
  matrixXXX_t* nC;

  if (dimsup==0){
    return destructive ? C : matrixXXX_copy(C);
  }
  nbrows = C->nbrows;
  nbcols = C->nbcolumns;
  col = pk->dec + dim;
  /* Count the number of constraints to duplicate */
  nb=0;
  p = C->p;
  for (i=0; i<nbrows; i++){
    if (numintXXX_sgn(p[i][col]))
      nb++;
  }
  /* Redimension matrix */
  ap_dimchange_init(&dimchange,ap_dimension_make(0,dimsup));
  for (i=0;i<dimsup;i++){
    dimchange.p[i]=offset;
  }
  nC = matrixXXX_add_dimensions(pk,destructive,C,dimchange);
  ap_dimchange_clear(&dimchange);
  matrixXXX_resize_rows(nC,nbrows+nb*dimsup);
  if (nb==0)
    return nC;

  /* Duplicate constraints */
  p = nC->p;
  row = nbrows;
  for (i=0; i<nbrows; i++){
    if (numintXXX_sgn(p[i][col])){
      for (j=offset;j < offset+dimsup; j++){
	vectorXXX_copy(p[row],
		       p[i],
		       nbcols+dimsup);
	numintXXX_set(p[row][pk->dec+j],p[row][col]);
	numintXXX_set_int(p[row][col],0);
	row++;
      }
    }
  }
  nC->_sorted = false;
  return nC;
}

/* ---------------------------------------------------------------------- */
/* Polyhedra */
/* ---------------------------------------------------------------------- */

pkXXX_t* pkXXX_expand(ap_manager_t* man,
		      bool destructive, pkXXX_t* pa,
		      ap_dim_t dim, size_t nbdimsup)
{
  ap_dimension_t dimsup,ndim;
  pkXXX_t* po;

  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_EXPAND);
  pkXXX_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald+nbdimsup);
  man->result.flag_best = man->result.flag_exact = true;

  dimsup = dim<pa->dim.intd ?
    ap_dimension_make(nbdimsup,0) :
    ap_dimension_make(0,nbdimsup);
  ndim = ap_dimension_add(pa->dim,dimsup);

  if (nbdimsup==0){
    return (destructive ? pa : pkXXX_copy(man,pa));
  }

  /* Get the constraints system, and possibly minimize */
  if (pk->funopt->algorithm<0)
    pkXXX_obtain_C(man,pa,"of the argument");
  else
    pkXXX_chernikova(man,pa,"of the argument");

  if (destructive){
    po = pa;
    po->dim = ap_dimension_add(po->dim,dimsup);
    po->status &= ~pk_status_consgauss & ~pk_status_gengauss & ~pk_status_minimaleps;
  }
  else {
    po = pkXXX_alloc(ndim);
  }

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->C){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return po;
    }
    /* We can still proceed, although it is likely
       that the problem is only delayed
    */
  }
  /* if empty, return empty */
  if (!pa->C){
    pkXXX_set_bottom(pk,po);
    return po;
  }
  /* Prepare resulting matrix */
  if (destructive){
    if (po->F){ matrixXXX_free(po->F); po->F = NULL; }
    if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
    po->nbeq = po->nbline = 0;
    po->status &= ~pk_status_consgauss & ~pk_status_gengauss & ~pk_status_minimaleps;
  }
  po->C = matrixXXX_expand(pk, destructive, pa->C,
			   dim,
			   (dim + nbdimsup < po->dim.intd ?
			    po->dim.intd-nbdimsup :
			    po->dim.intd+po->dim.reald-nbdimsup),
			   nbdimsup);
  /* Minimize the result */
  if (pk->funopt->algorithm>0){
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      if (!po->C){
	man->result.flag_best = man->result.flag_exact = false;
	pkXXX_set_top(pk,po);
	return po;
      }
    }
  }
  assert(pkXXX_check(pk,po));
  return po;
}

/* ********************************************************************** */
/* II. Fold */
/* ********************************************************************** */

/* ---------------------------------------------------------------------- */
/* Matrix */
/* ---------------------------------------------------------------------- */

/* Fold the last dimsup dimensions with dimension dim (not in the last dimsup
   ones) in the matrix */

/* the array tdim is assumed to be sorted */
static
matrixXXX_t* matrixXXX_fold(pkXXX_internal_t* pk,
			    bool destructive,
			    matrixXXX_t* F,
			    ap_dim_t* tdim, size_t size)
{
  matrixXXX_t* nF;
  size_t i,j,row,col;
  size_t nbrows, nbcols, dimsup;
  ap_dimchange_t dimchange;

  dimsup = size-1;
  if (dimsup==0){
    return destructive ? F : matrixXXX_copy(F);
  }
  nbrows = F->nbrows;
  nbcols = F->nbcolumns;
  col = pk->dec + tdim[0];

  nF = matrixXXX_alloc( size*nbrows,
			nbcols - dimsup,
			false );
  ap_dimchange_init(&dimchange,ap_dimension_make(0,dimsup));
  for (i=0;i<dimsup;i++){
    dimchange.p[i]=tdim[i+1];
  }
  row = 0;
  for(i=0; i<nbrows; i++){
    vectorXXX_remove_dimensions(pk,nF->p[row],F->p[i],nbcols,
				&dimchange);
    vectorXXX_normalize(pk,nF->p[row],nbcols-dimsup);
    row++;
    for (j=0;j<dimsup;j++){
      if (numintXXX_cmp(F->p[i][col],
			F->p[i][pk->dec+tdim[j+1]])!=0){
	vectorXXX_remove_dimensions(pk,
				    nF->p[row],F->p[i],nbcols,
				    &dimchange);
	numintXXX_set(nF->p[row][col],F->p[i][pk->dec+tdim[j+1]]);
	vectorXXX_normalize(pk,nF->p[row],nbcols-dimsup);
	row++;
      }
    }
  }
  nF->nbrows = row;
  nF->_sorted = false;
  if (destructive){
    matrixXXX_free(F);
  }
  ap_dimchange_clear(&dimchange);
  return nF;
}

/* the array tdim is assumed to be sorted */

pkXXX_t* pkXXX_fold(ap_manager_t* man,
		    bool destructive, pkXXX_t* pa,
		    ap_dim_t* tdim, size_t size)
{
  ap_dimension_t dimsup;
  pkXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_FOLD);
  man->result.flag_best = man->result.flag_exact = true;

  dimsup =
    (tdim[0]<pa->dim.intd) ?
    ap_dimension_make(size-1,0) :
    ap_dimension_make(0,size-1);

  if (pk->funopt->algorithm<0)
    pkXXX_obtain_F(man,pa,"of the argument");
  else
    pkXXX_chernikova(man,pa,"of the argument");

  if (destructive){
    po = pa;
    po->dim = ap_dimension_sub(pa->dim,dimsup);
  }
  else {
    po = pkXXX_alloc(ap_dimension_sub(pa->dim,dimsup));
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return po;
    }
  }
  /* if empty, return empty */
  if (!pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    pkXXX_set_bottom(pk,po);
    return po;
  }

  /* Prepare resulting matrix */
  if (destructive){
    if (po->C){ matrixXXX_free(po->C); po->C = NULL; }
    if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
    po->nbeq = po->nbline = 0;
    po->status &= ~pk_status_consgauss & ~pk_status_gengauss & ~pk_status_minimaleps;
  }

  po->F = matrixXXX_fold(pk, destructive, pa->F,
			 tdim, size);
  /* Minimize the result */
  if (pk->funopt->algorithm>0){
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      if (!po->C){
	man->result.flag_best = man->result.flag_exact = false;
	pkXXX_set_top(pk,po);
	return po;
      }
    }
  }

  man->result.flag_best = (dimsup.intd==0);
  man->result.flag_exact = (dimsup.intd==0);
  assert(pkXXX_check(pk,po));
  return po;
}
