/* ********************************************************************** */
/* pk_constructor.c: constructors and accessors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_user.h"
#include "pk_representation.h"
#include "pk_constructor.h"
#include "pk_meetjoin.h"

#include "itv.h"
#include "itv_linexpr.h"
#include "ap_generic.h"

/* ********************************************************************** */
/* I. Constructors */
/* ********************************************************************** */

/* ====================================================================== */
/* Empty polyhedron */
/* ====================================================================== */

void poly_set_bottom(pk_internal_t* pk, pk_t* po)
{
  if (po->C) pk_matrix_free(po->C);
  if (po->F) pk_matrix_free(po->F);
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

pk_t* pk_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  pk_t* po = poly_alloc(intdim,realdim);
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOTTOM);
  pk_internal_realloc_lazy(pk,intdim+realdim);
  po->status = pk_status_conseps | pk_status_minimaleps;
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ====================================================================== */
/* Universe polyhedron */
/* ====================================================================== */

void pk_matrix_fill_constraint_top(pk_internal_t* pk, pk_matrix_t* C, size_t start)
{
  if (pk->strict){
    /* constraints epsilon and xi-epsilon*/
    vector_clear(C->p[start+0],C->nbcolumns);
    vector_clear(C->p[start+1],C->nbcolumns);
    numint_set_int(C->p[start+0][0],1);
    numint_set_int(C->p[start+0][polka_eps],1);
    numint_set_int(C->p[start+1][0],1);
    numint_set_int(C->p[start+1][polka_cst],1);
    numint_set_int(C->p[start+1][polka_eps],(-1));
  }
  else {
    /* constraint \xi \geq 0 */
    vector_clear(C->p[start+0],C->nbcolumns);
    numint_set_int(C->p[start+0][0],1);
    numint_set_int(C->p[start+0][polka_cst],1);
  }
}

void poly_set_top(pk_internal_t* pk, pk_t* po)
{
  size_t i;
  size_t dim;

  if (po->C) pk_matrix_free(po->C);
  if (po->F) pk_matrix_free(po->F);
  if (po->satC) satmat_free(po->satC);
  if (po->satF) satmat_free(po->satF);

  po->status =
    pk_status_conseps |
    pk_status_consgauss |
    pk_status_gengauss |
    pk_status_minimaleps
    ;

  dim = po->intdim + po->realdim;

  po->C = pk_matrix_alloc(pk->dec-1, pk->dec+dim,true);
  po->F = pk_matrix_alloc(pk->dec+dim-1,pk->dec+dim,true);
  /* We have to ensure that the matrices are really sorted */
  po->satC = satmat_alloc(pk->dec+dim-1,bitindex_size(pk->dec-1));
  po->satF = 0;
  po->nbeq = 0;
  po->nbline = dim;

  /* constraints */
  pk_matrix_fill_constraint_top(pk,po->C,0);

  /* generators */
  /* lines $x_i$ */
  for(i=0; i<dim; i++){
    numint_set_int(po->F->p[i][pk->dec+dim-1-i],1);
  }
  if (pk->strict){
    /* rays xi and xi+epsilon */
    numint_set_int(po->F->p[dim][0],1);
    numint_set_int(po->F->p[dim][polka_cst],1);
    numint_set_int(po->F->p[dim+1][0],1);
    numint_set_int(po->F->p[dim+1][polka_cst],1);
    numint_set_int(po->F->p[dim+1][polka_eps],1);
    /* saturation matrix */
    po->satC->p[dim][0] = bitstring_msb >> 1;
    po->satC->p[dim+1][0] = bitstring_msb;
  }
  else {
    /* ray xi */
    numint_set_int(po->F->p[dim][0],1);
    numint_set_int(po->F->p[dim][polka_cst],1);
    /* saturation matrix */
    po->satC->p[dim][0] = bitstring_msb;
  }
  assert(poly_check(pk,po));
}

pk_t* pk_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  pk_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TOP);
  pk_internal_realloc_lazy(pk,intdim+realdim);

  po = poly_alloc(intdim,realdim);
  poly_set_top(pk,po);
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ====================================================================== */
/* Hypercube polyhedron */
/* ====================================================================== */


/* The matrix is supposed to be big enough */
static 
int pk_matrix_fill_constraint_box(pk_internal_t* pk,
				pk_matrix_t* C, size_t start,
				ap_interval_t** box,
				size_t intdim, size_t realdim,
				bool integer)
{
  size_t k;
  ap_dim_t i;
  bool ok;
  itv_t itv;
  k = start;

  itv_init(itv);
  for (i=0; i<intdim+realdim; i++){
    itv_set_ap_interval(pk->itv,itv,box[i]);
    if (itv_is_point(pk->itv,itv)){
      ok = vector_set_dim_bound(pk,C->p[k],
				 (ap_dim_t)i, bound_numref(itv->sup), 0,
				 intdim,realdim,
				 integer);
      if (!ok){
	itv_clear(itv);
	return -1;
      }
      k++;
    }
    else {
      /* inferior bound */
      if (!bound_infty(itv->inf)){
	vector_set_dim_bound(pk,C->p[k],
			     (ap_dim_t)i, bound_numref(itv->inf), -1,
			     intdim,realdim,
			     integer);
	k++;
      }
      /* superior bound */
      if (!bound_infty(itv->sup)){
	vector_set_dim_bound(pk,C->p[k],
			     (ap_dim_t)i, bound_numref(itv->sup), 1,
			     intdim,realdim,
			     integer);
	k++;
      }
    }
  }
  itv_clear(itv);
  return (int)k;
}

/* Abstract an hypercube defined by the array of intervals of size
   intdim+realdim.  */

pk_t* pk_of_box(ap_manager_t* man,
		size_t intdim, size_t realdim,
		ap_interval_t** array)
{
  int k;
  size_t dim;
  pk_t* po;

  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_OF_BOX);
  pk_internal_realloc_lazy(pk,intdim+realdim);

  dim = intdim + realdim;
  po = poly_alloc(intdim,realdim);
  po->status = pk_status_conseps;

  dim = intdim + realdim;
  po->C = pk_matrix_alloc(pk->dec-1 + 2*dim, pk->dec + dim, false);

  /* constraints */
  pk_matrix_fill_constraint_top(pk,po->C,0);
  k = pk_matrix_fill_constraint_box(pk,po->C,pk->dec-1,array,intdim,realdim,true);
  if (k==-1){
    pk_matrix_free(po->C);
    po->C = NULL;
    return po;
  }
  po->C->nbrows = (size_t)k;
  
  assert(poly_check(pk,po));
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ********************************************************************** */
/* II. Accessors */
/* ********************************************************************** */

/* Return the dimensions of the polyhedra */
ap_dimension_t pk_dimension(ap_manager_t* man, pk_t* po){
  ap_dimension_t res;
  res.intdim = po->intdim;
  res.realdim = po->realdim;
  return res;
}

