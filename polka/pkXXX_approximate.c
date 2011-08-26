/* ********************************************************************** */
/* pkXXX_approximate.c: approximate function  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */


#include "pkXXX_internal.h"
#include "eitvMPQ.h"

static
void pkXXX_set_save_C(pkXXX_t* po, pkXXX_t* pa)
{
  if (po != pa){
    po->F = pa->F ? matrixXXX_copy(pa->F) : NULL;
    po->satC = pa->satC ? satmat_copy(pa->satC) : NULL;
    po->satF = pa->satF ? satmat_copy(pa->satF) : NULL;
    po->status = pa->status;
    po->nbeq = pa->nbeq;
    po->nbline = pa->nbline;
    po->status = pa->status;
  }
}

/* ---------------------------------------------------------------------- */
/* Normalize integer constraints.  Return true if there is some change. */
/* ---------------------------------------------------------------------- */
static
bool poly_approximate_n1(ap_manager_t* man, pkXXX_t* po, pkXXX_t* pa, int algorithm)
{
  if (po->dim.intd>0){
    pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
    bool change;

    pkXXX_obtain_C(man,pa,NULL);
    if (!pa->C){
      pkXXX_set(po,pa);
      return false;
    }
    if (po!=pa){
      po->C = matrixXXX_copy(pa->C);
    }
    change = matrixXXX_normalize_constraint_int(pk,po->C,po->dim);
    if (change){
      {
	/* Add positivity and strictness that may not be implied any more */
	size_t nbrows = po->C->nbrows;
	matrixXXX_resize_rows_lazy(po->C,nbrows+pk->dec-1);
	matrixXXX_fill_constraint_top(pk,po->C,nbrows);
      }
      if (po==pa){
	if (po->F){ matrixXXX_free(po->F); po->F = NULL; }
	if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
	if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
      }
      po->status = 0;
      po->C->_sorted = false;
      man->result.flag_exact = false;
    } else {
      pkXXX_set_save_C(po,pa);
    }
    return change;
  }
  else
    return false;
}

/* ---------------------------------------------------------------------- */
/* Remove constraints with coefficients of size greater than
   pk->max_coeff_size, if pk->max_coeff_size > 0 */
/* ---------------------------------------------------------------------- */
static
bool matrixXXX_approximate_constraint_1(pkXXX_internal_t* pk, matrixXXX_t* C)
{
  size_t i,j;
  bool change;
  change = false;
  for (i=0;i<C->nbrows; i++){
    if (numintXXX_sgn(C->p[i][0])){
      for (j=pk->dec; j<C->nbcolumns; j++){
	if (numintXXX_size2(C->p[i][j]) > pk->option.approximate_max_coeff_size){
	  change = true;
	  C->nbrows--;
	  matrixXXX_exch_rows(C,i,C->nbrows);
	  break;
	}
      }
    }
  }
  if (change){
    /* Add positivity and strictness that may not be implied any more */
    size_t nbrows = C->nbrows;
    matrixXXX_resize_rows_lazy(C,nbrows+pk->dec-1);
    matrixXXX_fill_constraint_top(pk,C,nbrows);
    C->_sorted = false;
  }
  return change;
}
static
bool poly_approximate_1(ap_manager_t* man, pkXXX_t* po, pkXXX_t* pa)
{
  bool change;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  pkXXX_obtain_C(man,pa,NULL);
  if (!pa->C){
    pkXXX_set(po,pa);
    return false;
  }
  if (po!=pa){
    po->C = matrixXXX_copy(pa->C);
  }
  change = matrixXXX_approximate_constraint_1(pk,po->C);
  if (change){
    if (po==pa){
      if (po->F){ matrixXXX_free(po->F); po->F = NULL; }
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
    }
    po->status = 0;
    man->result.flag_exact = false;
  } else {
    pkXXX_set_save_C(po,pa);
    man->result.flag_exact = true;
  }
  return change;
}

/* ---------------------------------------------------------------------- */
/* 1. Remove constraints with too big coefficients
   2. Add interval constraints
   3. Add octagonal constraints
*/
/* ---------------------------------------------------------------------- */
static
void poly_approximate_123(ap_manager_t* man, pkXXX_t* po, int algorithm)
{
  bool change;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  if (algorithm==1){
    poly_approximate_1(man,po,po);
  }
  else {
    size_t nbrows, nbrows2;
    ap_dim_t dim;
    ap_dim_t i,j;
    int sgn, sgny;
    eitvXXX_t eitv;

    pkXXX_t* pa = pkXXX_alloc(po->dim);

    change = poly_approximate_1(man,pa,po);
    if (!change){
      pkXXX_free(man,pa);
      return;
    }
    pkXXX_obtain_F(man,po,NULL);
    if (!po->F){
      if (!po->C){
	pkXXX_free(man,pa);
	pkXXX_set_bottom(pk,po);
	man->result.flag_exact = true;
	return;
      }
      else {
	pkXXX_clear(po);
	*po = *pa;
	free(pa);
      }
      return;
    }
    dim = pa->dim.intd + pa->dim.reald;
    nbrows = pa->C->nbrows;
    eitvXXX_init(eitv);
    if (algorithm>=2){ /* Add interval constraints */
      nbrows2 = 2*dim;
      matrixXXX_resize_rows_lazy(pa->C, nbrows + nbrows2);
      pa->C->_sorted = false;
      for (i=0; i<dim;i++){
	matrixXXX_bound_dimension(pk,eitv,i,po->F);
	if (!boundXXX_infty(eitv->itv->neginf)){
	  vectorXXX_set_dim_bound(pk,
				  pa->C->p[nbrows],
				  i, boundXXX_numref(eitv->itv->neginf),
				  -1, po->dim, false);
	  nbrows++;
	}
	if (!boundXXX_infty(eitv->itv->sup)){
	  vectorXXX_set_dim_bound(pk,
				  pa->C->p[nbrows],
				  i, boundXXX_numref(eitv->itv->sup),
				  1, po->dim, false);
	  nbrows++;
	}
      }
    }
    pa->C->nbrows = nbrows;
    if (algorithm>=3){ /* Add octagonal constraints */
      vectorXXX_clear(pk->numintXXXp,po->F->nbcolumns);
      numintXXX_set_int(pk->numintXXXp[0],1);
      for (i=0; i<dim;i++){
	numintXXX_set_int(pk->numintXXXp[pk->dec+i],1);
	nbrows2 = 2*(dim-i-1);
	matrixXXX_resize_rows_lazy(pa->C, nbrows + nbrows2);
	for (j=i+1; j<dim;j++){
	  for (sgny=-1; sgny<=1; sgny += 2){
	    numintXXX_set_int(pk->numintXXXp[pk->dec+j],sgny);
	    matrixXXX_bound_vector(pk,eitv,pk->numintXXXp,po->F);
	    if (!boundXXX_infty(eitv->itv->neginf)){
	      vectorXXX_set_linexpr_bound(pk,
					  pa->C->p[nbrows], pk->numintXXXp,
					  eitv->itv->neginf, -1, po->dim, false);
	      nbrows++;
	    }
	    if (!boundXXX_infty(eitv->itv->sup)){
	      vectorXXX_set_linexpr_bound(pk,
					  pa->C->p[nbrows], pk->numintXXXp,
					  eitv->itv->sup, 1, po->dim, false);
	      nbrows++;
	    }
	  }
	  numintXXX_set_int(pk->numintXXXp[pk->dec+j],0);
	}
	numintXXX_set_int(pk->numintXXXp[pk->dec+i],0);
      }
      pa->C->nbrows = nbrows;
    }
    eitvXXX_clear(eitv);
    pkXXX_clear(po);
    *po = *pa;
    free(pa);
    return;
  }
}

/* ---------------------------------------------------------------------- */
/* 10. Round constraints with too big coefficients */
/* ---------------------------------------------------------------------- */
static
bool matrixXXX_approximate_constraint_10(pkXXX_internal_t* pk, matrixXXX_t* C, matrixXXX_t* F)
{
  size_t i,j,size;
  bool change,removed;
  eitvXXX_t eitv;

  eitvXXX_init(eitv);
  change = false;
  i = 0;
  while (i<C->nbrows){
    removed = false;
    if (numintXXX_sgn(C->p[i][0]) &&
	(pk->strict ? numintXXX_sgn(C->p[i][polka_eps])<=0 : true)){
      /* Look for a too big coefficient in the row */
      size=0; /* for next test */
      for (j=pk->dec; j<C->nbcolumns; j++){
	size = numintXXX_size2(C->p[i][j]);
	if (size > pk->option.approximate_max_coeff_size){
	  /* Too big coefficient detected in the row */
	  break;
	}
      }
      if (size > pk->option.approximate_max_coeff_size){
	/* Too big coefficient detected in the row */
	/* A. Compute maximum magnitude */
	size_t maxsize = size;
	for (j=j+1; j<C->nbcolumns; j++){
	  size = numintXXX_size2(C->p[i][j]);
	  if (size>maxsize) maxsize=size;
	}
	/* B. Relax if constraint is strict */
	if (pk->strict){
	  numintXXX_set_int(C->p[i][polka_eps],0);
	}
	/* C. Perform rounding of non constant coefficients */
	size = maxsize - pk->option.approximate_max_coeff_size;
	for (j=pk->dec; j<C->nbcolumns; j++){
	  numintXXX_fdiv_q_2exp(C->p[i][j],C->p[i][j], size);
	}
	/* D. Compute new constant coefficient */
	numintXXX_set_int(C->p[i][0],1);
	numintXXX_set_int(C->p[i][polka_cst],0);
	matrixXXX_bound_vector(pk,eitv,C->p[i],F);
	if (boundXXX_infty(eitv->itv->neginf)){
	  /* If no bound, we remove the constraint */
	  C->nbrows--;
	  matrixXXX_exch_rows(C,i,C->nbrows);
	  removed = true;
	}
	else {
	  /* Otherwise, we round the constant to an integer */
	  boundXXX_neg(eitv->itv->neginf,eitv->itv->neginf);
	  numintXXX_fdiv_q(numXXX_numref(boundXXX_numref(eitv->itv->neginf)),
			   numXXX_numref(boundXXX_numref(eitv->itv->neginf)),
			   numXXX_denref(boundXXX_numref(eitv->itv->neginf)));
	  numintXXX_neg(C->p[i][polka_cst],numXXX_numref(boundXXX_numref(eitv->itv->neginf)));
	  vectorXXX_normalize(pk,C->p[i],C->nbcolumns);
	}
	change = true;
      }
    }
    if (!removed) i++;
  }
  if (change){
    /* Add positivity and strictness that may not be implied any more */
    size_t nbrows = C->nbrows;
    matrixXXX_resize_rows_lazy(C,nbrows+pk->dec-1);
    matrixXXX_fill_constraint_top(pk,C,nbrows);
    C->_sorted = false;
  }
  eitvXXX_clear(eitv);
  return change;
}
static
void poly_approximate_10(ap_manager_t* man, pkXXX_t* po)
{
  bool change;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  pkXXX_chernikova(man,po,"of the argument");
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return;
  }
  if (!po->C && !po->F){
    return;
  }
  assert(po->C && po->F);
  change = matrixXXX_approximate_constraint_10(pk, po->C, po->F);
  if (change){
    if (po->F) matrixXXX_free(po->F);
    if (po->satC) satmat_free(po->satC);
    if (po->satF) satmat_free(po->satF);
    po->F = NULL;
    po->satC = NULL;
    po->satF = NULL;
    man->result.flag_exact = false;
  }
  else {
    man->result.flag_exact = true;
  }
}

/*
  Approximation:

  - algorithm==0: do nothing

  - algorithm==-1: normalize integer minimal constraints (induces a smaller
  polyhedron)

  - algorithm==1: remove constraints with coefficients of size greater than
  max_coeff_size, if max_coeff_size > 0
  - algorithm==2: in addition, keep same bounding box (more precise)
  - algorithm==3: in addition, keep same bounding octagon (even more precise)

  - algorithm==10: round constraints with too big coefficients, of size greater than
  approximate_max_coeff_size, if approximate_max_coeff_size>0

*/
void pkXXX_approximate(ap_manager_t* man, pkXXX_t* po, int algorithm)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_APPROXIMATE);

  man->result.flag_exact = false;
  man->result.flag_best = false;

  if (!po->C && !po->F)
    return;

  switch (algorithm){
  case -1:
    if (po->dim.intd>0)
      poly_approximate_n1(man,po,po,algorithm);
    break;
  case 1:
  case 2:
  case 3:
    if (pk->option.approximate_max_coeff_size>0)
      poly_approximate_123(man,po,algorithm);
    break;
  case 10:
    if (pk->option.approximate_max_coeff_size>0)
      poly_approximate_10(man,po);
    break;
  }
  assert(pkXXX_check(pk,po));
}
