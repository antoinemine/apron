/* ************************************************************************* */
/* ap_pkgrid.c: reduced product of NewPolka polyhedra and PPL grids */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_global0.h"
#include "ap_reducedproduct.h"
#include "ap_reducedproduct.h"

#include "pk.h"
#include "pk_internal.h"
#include "pk_matrix.h"
#include "pk_satmat.h"
#include "pk_representation.h"
#include "pk_user.h"
#include "ppl_grid.h"

void ap_pkgrid_reduce(ap_manager_t* manager,
		      ap_reducedproduct_t* a)
{
  size_t i,index;
  ap_reducedproduct_internal_t* intern = 
    (ap_reducedproduct_internal_t*)manager->internal;
  ap_manager_t* manpoly = intern->tmanagers[0];
  ap_manager_t* mangrid = intern->tmanagers[1];
  pk_t* poly = (pk_t*)a->p[0];
  struct ppl_grid* grid = a->p[1];
  ap_lincons0_array_t array,array2;
  ap_interval_t* interval;
  mpq_t quotient,aprime,diff,prod;
  int cmp;
  ap_dimension_t dimension;

  mpq_init(diff);
  mpq_init(prod);
  mpq_init(quotient);
  mpq_init(aprime);
  dimension = pk_dimension(manpoly,poly);

  /* 1. Reduction from poly to grid:
     one add equalities of poly to grid */
  pk_canonicalize(manpoly,poly);
  if (pk_is_bottom(manpoly,poly)){
  ap_pkgrid_reduce_exit1:
    ap_ppl_grid_free(mangrid,grid);
    grid = ap_ppl_grid_bottom(mangrid,
			      dimension.intdim,dimension.realdim);
    goto ap_pkgrid_reduce_exit;
  }
  assert(poly->C->_sorted);
  if (poly->nbeq>0){
    array = ap_lincons0_array_make(poly->nbeq);
    for (i=0; i<poly->nbeq; i++){
      array.p[i] = lincons0_of_vector((pk_internal_t*)(manpoly->internal),
				      poly->C->p[i],poly->C->nbcolumns);
    }
    grid = ap_ppl_grid_meet_lincons_array(mangrid,true,grid,&array);
    ap_lincons0_array_clear(&array);
    if (ap_ppl_grid_is_bottom(mangrid,grid)){
    ap_pkgrid_reduce_exit2:
      pk_free(manpoly,poly);
      poly = pk_bottom(manpoly,
		       dimension.intdim,dimension.realdim);
      goto ap_pkgrid_reduce_exit;
    }
  }
  /* 2. Reduction from grid to poly:
     for each equality e=0 mod m, m>0,
     compute [a,b] = range(e) in poly
     if b-a > m, nothing to do;
     otherwise, compute [a',b'] where a' = a - sup(a/m) m, b' = b - sup(a/m) m
     (we have -m<a'<=0)
     first case: b-a=m and a=0: nothing to do
     second case: b-a=m and a!=0:  e = sup(a/m).m
     third case: b-a<m, a'<=0<=b': e = sup(a/m) m
     fourth case: b-a<m, b'=a'+(b-a)<0: bottom
  */
  /* 2.1 Extract constraints */
  ap_ppl_grid_canonicalize(mangrid,grid);
  if (ap_ppl_grid_is_bottom(mangrid,grid)){
    goto ap_pkgrid_reduce_exit2;
  }
  array = ap_ppl_grid_to_lincons_array(mangrid,grid);
  array2 = ap_lincons0_array_make(array.size);
  index = 0;
  for (i=0; i<array.size; i++){
    ap_lincons0_t cons = array.p[i];
    switch (cons.constyp){
    case AP_CONS_EQ:
      array2.p[index] = ap_lincons0_copy(&cons);
      index++;
      break;
    case AP_CONS_EQMOD:
      interval = pk_bound_linexpr(manpoly,poly,cons.linexpr0);
      if (ap_scalar_infty(interval->inf) || ap_scalar_infty(interval->sup)){
	ap_interval_free(interval);
	continue;
      }
      assert(interval->inf->discr==AP_SCALAR_MPQ &&
	     interval->sup->discr==AP_SCALAR_MPQ);
      assert(cons.scalar->discr==AP_SCALAR_MPQ);
      mpq_sub(diff,
	      interval->sup->val.mpq,
	      interval->inf->val.mpq);
      cmp = mpq_cmp(diff,cons.scalar->val.mpq);
      if (cmp<=0){ /* b-a<=m */
	mpq_div(quotient,interval->inf->val.mpq,cons.scalar->val.mpq);
	mpz_cdiv_q(mpq_numref(quotient),
		   mpq_numref(quotient),
		   mpq_denref(quotient));
	mpz_set_si(mpq_denref(quotient),1);
	mpq_mul(prod,quotient,cons.scalar->val.mpq);
	mpq_sub(aprime,interval->inf->val.mpq,prod);
	if (cmp==0 && mpq_sgn(aprime)!=0){
	  goto ap_pkgrid_reduce_red;
	}
	else if (cmp<0){
	  mpq_add(diff,diff,aprime);
	  if (mpq_sgn(diff)>=0){
	  ap_pkgrid_reduce_red:
	    assert(cons.linexpr0->cst.discr==AP_COEFF_SCALAR &&
		   cons.linexpr0->cst.val.scalar->discr==AP_SCALAR_MPQ);
	    array2.p[index] = ap_lincons0_copy(&cons);
	    cons = array2.p[index];
	    index++;
	    mpq_add(cons.linexpr0->cst.val.scalar->val.mpq,
		    cons.linexpr0->cst.val.scalar->val.mpq,
		    prod);
	    cons.constyp = AP_CONS_EQ;
	  }
	  else {
	    ap_lincons0_array_clear(&array);
	    ap_lincons0_array_clear(&array2);
	    ap_ppl_grid_free(mangrid,grid);
	    pk_free(manpoly,poly);
	    grid = ap_ppl_grid_bottom(mangrid,
				      dimension.intdim,dimension.realdim);
	    poly = pk_bottom(manpoly,
			     dimension.intdim,dimension.realdim);
	    ap_interval_free(interval);
	    goto ap_pkgrid_reduce_exit;
	  }
	}
      }
      ap_interval_free(interval);
      break;
    default:
      break;
    }
  }
  array2.size = index;
  poly = pk_meet_lincons_array(manpoly,true,poly,&array2);
  ap_lincons0_array_clear(&array);
  ap_lincons0_array_clear(&array2);
  
 ap_pkgrid_reduce_exit:
  mpq_clear(diff);
  mpq_clear(prod);
  mpq_clear(quotient);
  mpq_clear(aprime);
  a->p[0] = poly;
  a->p[1] = grid;
}

void ap_pkgrid_approximate(ap_manager_t* manager,
			   ap_reducedproduct_t* a,
			   int n)
{
  ap_reducedproduct_internal_t* intern = 
    (ap_reducedproduct_internal_t*)manager->internal;
  ap_manager_t* manpoly = intern->tmanagers[0];
  ap_manager_t* mangrid = intern->tmanagers[1];
  pk_t* poly = (pk_t*)a->p[0];
  struct ppl_grid* grid = a->p[1];
  pk_approximate(manpoly,poly,n);
  ap_pkgrid_reduce(manager,a);
}

ap_manager_t* ap_pkgrid_manager_alloc(ap_manager_t* manpk, ap_manager_t* manpplgrid)
{
  ap_manager_t* tmanagers[2];
  bool strict;

  strict = (strcmp(manpk->library,"polka, strict mode")==0);
  
  if ( !(strcmp(manpk->library,"polka, loose mode")==0 ||
	 strict)
       ||
       !strcmp(manpplgrid->library,"PPL::Grid") )
    return NULL;

  tmanagers[0] = manpk;
  tmanagers[1] = manpplgrid;
  char* library = strict ? 
    "pkgrid: polka, strict mode and PPL::Grid" :
    "pkgrid: polka, loose mode and PPL::Grid";

  ap_manager_t* man = ap_reducedproduct_manager_alloc(library,
						      tmanagers,2,
						      &ap_pkgrid_reduce,
						      ap_pkgrid_approximate);
  return man;
}
