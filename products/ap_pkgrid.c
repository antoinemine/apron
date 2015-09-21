/* ************************************************************************* */
/* ap_pkgrid.c: reduced product of NewPolka polyhedra and PPL grids */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under GPL license.  Please
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
  mpq_t quotient;
  int cmp;
  ap_dimension_t dimension;

  mpq_init(quotient);
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

     a <= e <= b and e=0 mod m means a<=e<=b and e=km, with k integer.
     possible range for k satisfies a <= km <= b <=> sup(a/m) <= k <= inf(b/m).
     so we can refine the constraint with
     sup(a/m)*m <= e <= inf(a/m)*m
  */
  /* 2.1 Extract constraints */
  ap_ppl_grid_canonicalize(mangrid,grid);
  if (ap_ppl_grid_is_bottom(mangrid,grid)){
    goto ap_pkgrid_reduce_exit2;
  }
  array = ap_ppl_grid_to_lincons_array(mangrid,grid);
  array2 = ap_lincons0_array_make(2*array.size);
  index = 0;
  for (i=0; i<array.size; i++){
    ap_lincons0_t cons = array.p[i];
    ap_lincons0_t* cons2;
    switch (cons.constyp){
    case AP_CONS_EQ:
      array2.p[index] = ap_lincons0_copy(&cons);
      index++;
      break;
    case AP_CONS_EQMOD:
      interval = pk_bound_linexpr(manpoly,poly,cons.linexpr0);
      assert(interval->inf->discr==AP_SCALAR_MPQ &&
	     interval->sup->discr==AP_SCALAR_MPQ);
      assert(cons.scalar->discr==AP_SCALAR_MPQ);
      if (!ap_scalar_infty(interval->inf)){
	/* lower  */
	mpq_div(quotient,
		interval->inf->val.mpq,
		cons.scalar->val.mpq);
	if (mpz_cmp_ui(mpq_denref(quotient),1)!=0){ /* a/m is not integer */
	  mpz_cdiv_q(mpq_numref(quotient),
		     mpq_numref(quotient),
		     mpq_denref(quotient));
	  mpz_set_ui(mpq_denref(quotient),1);
	  mpq_mul(quotient,quotient,cons.scalar->val.mpq);
	  array2.p[index] = ap_lincons0_copy(&cons);
	  cons2 = &array2.p[index];
	  cons2->constyp = AP_CONS_SUPEQ;
	  mpq_sub(cons2->linexpr0->cst.val.scalar->val.mpq,
		  cons2->linexpr0->cst.val.scalar->val.mpq,
		  quotient);
	  index++;
	}
      }
      if (!ap_scalar_infty(interval->sup)){
	/* Upper */
	mpq_div(quotient,
		interval->sup->val.mpq,
		cons.scalar->val.mpq);
	if (mpz_cmp_ui(mpq_denref(quotient),1)!=0){ /* a/m is not integer */
	  mpz_fdiv_q(mpq_numref(quotient),
		     mpq_numref(quotient),
		     mpq_denref(quotient));
	  mpz_set_ui(mpq_denref(quotient),1);
	  mpq_mul(quotient,quotient,cons.scalar->val.mpq);
	  array2.p[index] = ap_lincons0_copy(&cons);
	  cons2 = &array2.p[index];
	  cons2->constyp = AP_CONS_SUPEQ;
	  {
	    size_t k,dim;
	    ap_coeff_t* pcoeff;
	    ap_linexpr0_ForeachLinterm(cons2->linexpr0,i,dim,pcoeff){
	      ap_coeff_neg(pcoeff,pcoeff);
	    }
	    mpq_neg(cons2->linexpr0->cst.val.scalar->val.mpq,
		    cons2->linexpr0->cst.val.scalar->val.mpq);
	  }
	  mpq_add(cons2->linexpr0->cst.val.scalar->val.mpq,
		  cons2->linexpr0->cst.val.scalar->val.mpq,
		  quotient);
	  index++;
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
  mpq_clear(quotient);
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

  if ( (strcmp(manpk->library,"polka, loose mode") && !strict) ||
       strcmp(manpplgrid->library,"PPL::Grid") )
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
