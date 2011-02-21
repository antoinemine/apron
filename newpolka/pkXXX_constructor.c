/* ********************************************************************** */
/* pkXXX_constructor.c: constructors and accessors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "eitvMPQ.h"
#include "ap_generic.h"

/* ********************************************************************** */
/* I. Constructors */
/* ********************************************************************** */

/* ====================================================================== */
/* Empty polyhedron */
/* ====================================================================== */

void pkXXX_set_bottom(pkXXX_internal_t* pk, pkXXX_t* po)
{
  if (po->C) matrixXXX_free(po->C);
  if (po->F) matrixXXX_free(po->F);
  if (po->satC) satmat_free(po->satC);
  if (po->satF) satmat_free(po->satF);
  po->C = po->F = NULL;
  po->satC = po->satF = NULL;
  po->status = pk_status_conseps | pk_status_minimaleps;
  po->nbeq = po->nbline = 0;
}

/*
  The empty polyhedron is just defined by the absence of both
  constraints matrix and frames matrix.
*/

pkXXX_t* pkXXX_bottom(ap_manager_t* man, ap_dimension_t dim)
{
  pkXXX_t* po = pkXXX_alloc(dim);
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_BOTTOM);
  pkXXX_internal_realloc_lazy(pk,dim.intd+dim.reald);
  po->status = pk_status_conseps | pk_status_minimaleps;
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ====================================================================== */
/* Universe polyhedron */
/* ====================================================================== */

void matrixXXX_fill_constraint_top(pkXXX_internal_t* pk, matrixXXX_t* C, size_t start)
{
  if (pk->strict){
    /* constraints epsilon and xi-epsilon*/
    vectorXXX_clear(C->p[start+0],C->nbcolumns);
    vectorXXX_clear(C->p[start+1],C->nbcolumns);
    numintXXX_set_int(C->p[start+0][0],1);
    numintXXX_set_int(C->p[start+0][polka_eps],1);
    numintXXX_set_int(C->p[start+1][0],1);
    numintXXX_set_int(C->p[start+1][polka_cst],1);
    numintXXX_set_int(C->p[start+1][polka_eps],(-1));
  }
  else {
    /* constraint \xi \geq 0 */
    vectorXXX_clear(C->p[start+0],C->nbcolumns);
    numintXXX_set_int(C->p[start+0][0],1);
    numintXXX_set_int(C->p[start+0][polka_cst],1);
  }
}

void pkXXX_set_top(pkXXX_internal_t* pk, pkXXX_t* po)
{
  size_t i;
  size_t dim;

  if (po->C) matrixXXX_free(po->C);
  if (po->F) matrixXXX_free(po->F);
  if (po->satC) satmat_free(po->satC);
  if (po->satF) satmat_free(po->satF);

  po->status =
    pk_status_conseps |
    pk_status_consgauss |
    pk_status_gengauss |
    pk_status_minimaleps
    ;

  dim = po->dim.intd + po->dim.reald;

  po->C = matrixXXX_alloc(pk->dec-1, pk->dec+dim,true);
  po->F = matrixXXX_alloc(pk->dec+dim-1,pk->dec+dim,true);
  /* We have to ensure that the matrices are really sorted */
  po->satC = satmat_alloc(pk->dec+dim-1,bitindex_size(pk->dec-1));
  po->satF = 0;
  po->nbeq = 0;
  po->nbline = dim;

  /* constraints */
  matrixXXX_fill_constraint_top(pk,po->C,0);

  /* generators */
  /* lines $x_i$ */
  for(i=0; i<dim; i++){
    numintXXX_set_int(po->F->p[i][pk->dec+dim-1-i],1);
  }
  if (pk->strict){
    /* rays xi and xi+epsilon */
    numintXXX_set_int(po->F->p[dim][0],1);
    numintXXX_set_int(po->F->p[dim][polka_cst],1);
    numintXXX_set_int(po->F->p[dim+1][0],1);
    numintXXX_set_int(po->F->p[dim+1][polka_cst],1);
    numintXXX_set_int(po->F->p[dim+1][polka_eps],1);
    /* saturation matrix */
    po->satC->p[dim][0] = bitstring_msb >> 1;
    po->satC->p[dim+1][0] = bitstring_msb;
  }
  else {
    /* ray xi */
    numintXXX_set_int(po->F->p[dim][0],1);
    numintXXX_set_int(po->F->p[dim][polka_cst],1);
    /* saturation matrix */
    po->satC->p[dim][0] = bitstring_msb;
  }
  assert(pkXXX_check(pk,po));
}

pkXXX_t* pkXXX_top(ap_manager_t* man, ap_dimension_t dim)
{
  pkXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_TOP);
  pkXXX_internal_realloc_lazy(pk,dim.intd+dim.reald);

  po = pkXXX_alloc(dim);
  pkXXX_set_top(pk,po);
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ====================================================================== */
/* Hypercube polyhedron */
/* ====================================================================== */


/* The matrix is supposed to be big enough */
static
int matrixXXX_fill_constraint_box(
    pkXXX_internal_t* pk,
    matrixXXX_t* C, size_t start, ap_box0_t box, ap_dimension_t dim, bool integer)
{
  size_t k;
  ap_dim_t i;
  bool ok;
  eitvXXX_t eitv;
  ap_coeff_t coeff;

  k = start;
  eitvXXX_init(eitv);
  for (i=0; i<dim.intd+dim.reald; i++){
    ap_box0_ref_index(coeff,box,i);
    eitvXXX_set_ap_coeff(eitv,coeff,pk->num);
    if (eitvXXX_is_point(eitv)){
      ok = vectorXXX_set_dim_bound(pk,C->p[k],
				   (ap_dim_t)i, boundXXX_numref(eitv->itv->sup), 0,
				   dim,
				   integer);
      if (!ok){
	eitvXXX_clear(eitv);
	return -1;
      }
      k++;
    }
    else {
      /* inferior bound */
      if (!boundXXX_infty(eitv->itv->neginf)){
	vectorXXX_set_dim_bound(pk,C->p[k],
				(ap_dim_t)i, boundXXX_numref(eitv->itv->neginf), -1,
				dim,
				integer);
	k++;
      }
      /* superior bound */
      if (!boundXXX_infty(eitv->itv->sup)){
	vectorXXX_set_dim_bound(pk,C->p[k],
				(ap_dim_t)i, boundXXX_numref(eitv->itv->sup), 1,
				dim,
				integer);
	k++;
      }
    }
  }
  eitvXXX_clear(eitv);
  return (int)k;
}

/* Abstract an hypercube defined by the array of intervals of size
   dim.intd+dim.reald.  */

pkXXX_t* pkXXX_of_box(ap_manager_t* man,
		      ap_dimension_t dim,
		      ap_box0_t box)
{
  int k;
  size_t nbdims;
  pkXXX_t* po;

  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_OF_BOX);
  pkXXX_internal_realloc_lazy(pk,dim.intd+dim.reald);

  nbdims = dim.intd + dim.reald;
  po = pkXXX_alloc(dim);
  po->status = pk_status_conseps;

  po->C = matrixXXX_alloc(pk->dec-1 + 2*nbdims, pk->dec + nbdims, false);

  /* constraints */
  matrixXXX_fill_constraint_top(pk,po->C,0);
  k = matrixXXX_fill_constraint_box(pk,po->C,pk->dec-1,box,dim,true);
  if (k==-1){
    matrixXXX_free(po->C);
    po->C = NULL;
    return po;
  }
  po->C->nbrows = (size_t)k;

  assert(pkXXX_check(pk,po));
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ********************************************************************** */
/* II. Accessors */
/* ********************************************************************** */

/* Return the dimensions of the polyhedra */
ap_dimension_t pkXXX_dimension(ap_manager_t* man, pkXXX_t* po){
  ap_dimension_t res;
  res.intd = po->dim.intd;
  res.reald = po->dim.reald;
  return res;
}
