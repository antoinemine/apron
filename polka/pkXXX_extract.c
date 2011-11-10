/* ********************************************************************** */
/* pkXXX_extract.c: property extraction */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "ap_linexprXXX.h"
#include "ap_generic.h"
#include "num_conv.h"

#define _MARK_numXXX_

/* Bounding the value of a dimension in a matrix of generators. */

void matrixXXX_bound_dimension(pkXXX_internal_t* pk,
			       eitvXXX_t eitv,
			       ap_dim_t dim,
			       matrixXXX_t* F)
{
  size_t i, index;
  int sgn;

  assert(pk->dec+dim<F->nbcolumns);

  eitv->eq = false;
  boundXXX_set_infty(eitv->itv->neginf,-1);
  boundXXX_set_infty(eitv->itv->sup,-1);
  index = pk->dec+dim;
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintXXX_sgn(F->p[i][polka_eps])==0){
      sgn = numintXXX_sgn(F->p[i][index]);
      if (numintXXX_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  eitvXXX_set_top(eitv);
	  return;
	}
      }
      else if (numintXXX_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  boundXXX_set_infty(eitv->itv->sup,+1);
	  if (boundXXX_infty(eitv->itv->neginf) && boundXXX_sgn(eitv->itv->neginf)>0)
	    return;
	}
	else if (sgn < 0){
	  boundXXX_set_infty(eitv->itv->neginf,+1);
	  if (boundXXX_infty(eitv->itv->sup) && boundXXX_sgn(eitv->itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numXXX_set_numintXXX2(pk->numratXXX,
			      F->p[i][index],
			      F->p[i][polka_cst]);
	if (boundXXX_cmp_num(eitv->itv->sup,pk->numratXXX)<0){
	  boundXXX_set_num(eitv->itv->sup,pk->numratXXX);
	}
	numXXX_neg(pk->numratXXX,pk->numratXXX);
	if (boundXXX_cmp_num(eitv->itv->neginf,pk->numratXXX)<0){
	  boundXXX_set_num(eitv->itv->neginf,pk->numratXXX);
	}
      }
    }
  }
  eitv->eq = itvXXX_is_point(eitv->itv);
  return;
}

void matrixXXX_to_box(pkXXX_internal_t* pk,
		      ap_linexprXXX_t res,
		      matrixXXX_t* F)
{
  size_t i,dim;

  assert(F);
  assert(F->nbcolumns>=pk->dec);
  dim = F->nbcolumns - pk->dec;
  for (i=0;i<dim;i++){
    matrixXXX_bound_dimension(pk,res->linterm[i]->eitv,i,F);
  }
}

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/

void matrixXXX_bound_vector(pkXXX_internal_t* pk,
			    eitvXXX_t eitv,
			    numintXXX_t* vec,
			    matrixXXX_t* F)
{
  size_t i;
  int sgn;

  eitv->eq = false;
  boundXXX_set_infty(eitv->itv->neginf,-1);
  boundXXX_set_infty(eitv->itv->sup,-1);

  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintXXX_sgn(F->p[i][polka_eps])==0 ){
      vectorXXX_product_strict(pk, pk->poly_prod, F->p[i], vec, F->nbcolumns);
      sgn = numintXXX_sgn(pk->poly_prod);
      if (numintXXX_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  eitvXXX_set_top(eitv);
	  return;
	}
      }
      else if (numintXXX_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  boundXXX_set_infty(eitv->itv->sup,+1);
	  if (boundXXX_infty(eitv->itv->neginf) && boundXXX_sgn(eitv->itv->neginf)>0)
	    return;
	}
	else if (sgn < 0){
	  boundXXX_set_infty(eitv->itv->neginf,+1);
	  if (boundXXX_infty(eitv->itv->sup) && boundXXX_sgn(eitv->itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numXXX_set_numintXXX2(pk->numratXXX,
			      pk->poly_prod,F->p[i][polka_cst]);
	if (boundXXX_cmp_num(eitv->itv->sup,pk->numratXXX)<0){
	  boundXXX_set_num(eitv->itv->sup,pk->numratXXX);
	}
	numXXX_neg(pk->numratXXX,pk->numratXXX);
	if (boundXXX_cmp_num(eitv->itv->neginf,pk->numratXXX)<0){
	  boundXXX_set_num(eitv->itv->neginf,pk->numratXXX);
	}
      }
    }
  }
  if (!boundXXX_infty(eitv->itv->neginf)){
    numintXXX_mul(numXXX_denref(boundXXX_numref(eitv->itv->neginf)),
		  numXXX_denref(boundXXX_numref(eitv->itv->neginf)),
		  vec[0]);
    numXXX_canonicalize(boundXXX_numref(eitv->itv->neginf));
  }
  if (!boundXXX_infty(eitv->itv->sup)){
    numintXXX_mul(numXXX_denref(boundXXX_numref(eitv->itv->sup)),
		  numXXX_denref(boundXXX_numref(eitv->itv->sup)),
		  vec[0]);
    numXXX_canonicalize(boundXXX_numref(eitv->itv->sup));
  }
  eitv->eq = itvXXX_is_point(eitv->itv);
}

/* Bounding the value of an (interval) linear expression (ap_linexprXXX) in a
   generator vector
*/
#define _numXXX_
#if defined(_numMPQ_)
#define numXXX_set_numMPQ_special(a,b,c) (a)=(b)
#define eitvXXX_set_eitvMPQ_special(a,b,c) (a)=(b)
#else
#define numXXX_set_numMPQ_special(a,b,c) numXXX_set_numMPQ(a,b,c)
#define eitvXXX_set_eitvMPQ_special(a,b,c) eitvXXX_set_eitvMPQ(a,b,c)
#endif

static
void vectorXXX_bound_linexprMPQ(pkXXX_internal_t* pk,
				eitvXXX_t eitv,
				ap_linexprMPQ_t linexpr,
				numintXXX_t* vec, size_t size)
{
  size_t i,dim;
  eitvMPQ_ptr peitv;


  numXXX_ptr prod = pk->numratXXX;
  numXXX_ptr rat = pk->numrat2XXX;
  numXXX_ptr rat2 = pk->numrat3XXX;
  eitvXXX_ptr eitvXXX = pk->eitvXXX;

  numXXX_set_int(rat,1);
  numXXX_set_int(eitv->itv->sup,0);
  ap_linexprMPQ_ForeachLinterm0(linexpr,i,dim,peitv){
    assert(peitv->eq);
    size_t index = pk->dec + dim;
    if (numintXXX_sgn(vec[index])){
      numintXXX_set(numXXX_numref(rat),vec[index]);
      numXXX_set_numMPQ_special(rat2,peitv->itv->sup,pk->num);
      numXXX_mul(prod,rat,rat2);
      numXXX_add(eitv->itv->sup,eitv->itv->sup,prod);
    }
  }
  boundXXX_neg(eitv->itv->neginf,eitv->itv->sup);
  eitv->eq = true;
  if (numintXXX_sgn(vec[polka_cst])){
    numintXXX_set(numXXX_numref(rat),vec[polka_cst]);
    eitvXXX_div_num(eitv,eitv,rat);
    eitvXXX_set_eitvMPQ_special(eitvXXX,linexpr->cst,pk->num);
    eitvXXX_add(eitv,eitv,eitvXXX);
  }
  return;
}

/* Bounding the value of an (interval) linear expression (ap_linexprXXX) in a
   matrix of generators.
*/
static
void matrixXXX_bound_linexprMPQ(pkXXX_internal_t* pk,
				eitvXXX_t eitv,
				ap_linexprMPQ_t linexpr,
				matrixXXX_t* F)
{
  size_t i;
  int sgn;
  eitvXXX_t prod;

  eitv->eq = false;
  boundXXX_set_infty(eitv->itv->neginf,-1);
  boundXXX_set_infty(eitv->itv->sup,-1);

  eitvXXX_init(prod);
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintXXX_sgn(F->p[i][polka_eps])==0 ){
      vectorXXX_bound_linexprMPQ(pk, prod, linexpr, F->p[i], F->nbcolumns);
      if (numintXXX_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (!eitvXXX_is_zero(prod)){
	  eitvXXX_set_top(eitv);
	  goto _matrixXXX_bound_linexprMPQ_exit;
	}
      }
      else if (numintXXX_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (!eitvXXX_is_zero(prod)){
	  if (boundXXX_sgn(prod->itv->neginf)<0){
	    /* [inf,sup]>0 */
	    boundXXX_set_infty(eitv->itv->sup,+1);
	    if (boundXXX_infty(eitv->itv->neginf) && boundXXX_sgn(eitv->itv->neginf)>0)
	      goto _matrixXXX_bound_linexprMPQ_exit;
	  }
	  else if (boundXXX_sgn(prod->itv->sup)<0){
	    /* [inf,sup]<0 */
	    boundXXX_set_infty(eitv->itv->neginf,+1);
	    if (boundXXX_infty(eitv->itv->sup) && boundXXX_sgn(eitv->itv->sup)>0)
	      goto _matrixXXX_bound_linexprMPQ_exit;
	  }
	  else {
	    eitvXXX_set_top(eitv);
	    goto _matrixXXX_bound_linexprMPQ_exit;
	  }
	}
      }
      else {
	eitvXXX_join(eitv,eitv,prod);
      }
    }
  }
 _matrixXXX_bound_linexprMPQ_exit:
  eitvXXX_clear(prod);
  eitv->eq = itvXXX_is_point(eitv->itv);
  return;
}

/* ====================================================================== */
/* Bounding the value of a dimension in a polyhedra */
/* ====================================================================== */

void pkXXX_bound_dimension(ap_manager_t* man,
			   ap_coeff_t interval,
			   pkXXX_t* po,
			   ap_dim_t dim)
{
  eitvXXX_t eitv;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_BOUND_DIMENSION);

  if (!pk->option.op_lazy)
    pkXXX_chernikova(man,po,NULL);
  else
    pkXXX_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    ap_coeff_set_top(interval);
    return;
  }

  if (!po->F){ /* po is empty */
    ap_coeff_set_bottom(interval);
    man->result.flag_exact = man->result.flag_best = true;
    return;
  }

  eitvXXX_init(eitv);
  matrixXXX_bound_dimension(pk,eitv,dim,po->F);
  man->result.flag_exact = ap_coeff_set_eitvXXX(interval,eitv,pk->num);
  eitvXXX_clear(eitv);
  man->result.flag_exact &= dim<po->dim.intd;
  man->result.flag_best = man->result.flag_exact;
}

/* ====================================================================== */
/* Bounding the value of a linear expression in a polyhedra */
/* ====================================================================== */

void pkXXX_bound_linexpr_quasilinear(ap_manager_t* man,
				     ap_coeff_t interval,
				     pkXXX_t* po,
				     ap_linexpr0_t expr)
{
  bool exact;
  eitvXXX_t eitv;
  eitvXXX_ptr peitv;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_BOUND_LINEXPR);
  ap_linexprMPQ_ptr linexprMPQptr;

  if (!pk->option.op_lazy)
    pkXXX_chernikova(man,po,NULL);
  else
    pkXXX_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    ap_coeff_set_top(interval);
    return;
  }

  if (!po->F){ /* po is empty */
    ap_coeff_set_bottom(interval);
    man->result.flag_exact = man->result.flag_best = true;
    return;
  }

  if (expr->discr==AP_SCALAR_MPQ){
    linexprMPQptr = expr->linexpr.MPQ;
    exact = true;
  } else {
    exact = ap_linexprMPQ_set_linexpr0(pk->ap_linexprMPQ, expr, pk->num);
    linexprMPQptr = pk->ap_linexprMPQ;
  }
#if defined(_MARK_numMPQ_)
  if (interval->discr==AP_SCALAR_MPQ){
    peitv = interval->eitv.MPQ;
  }
  else
#endif
    {
      eitvXXX_init(eitv);
      peitv = eitv;
    }
  matrixXXX_bound_linexprMPQ(pk,peitv,linexprMPQptr,po->F);
  if (peitv==eitv){
    exact = ap_coeff_set_eitvXXX(interval,eitv,pk->num) && exact;
    eitvXXX_clear(eitv);
  }
  man->result.flag_exact = man->result.flag_best =
    ( (pk->funopt->flag_exact_wanted || pk->funopt->flag_best_wanted) &&
      ap_linexprMPQ_is_real(linexprMPQptr,po->dim.intd) ) ?
    exact :
    false;

  return;
}

void pkXXX_bound_linexpr(ap_manager_t* man,
			 ap_coeff_t interval, pkXXX_t* po, ap_linexpr0_t linexpr)
{
  return
    ap_generic_bound_linexpr(
	man,interval,po,linexpr,AP_SCALAR_MPQ,true,AP_LINEXPR_INTLINEAR,
	(void (*)(ap_manager_t*,ap_coeff_t,void*,ap_linexpr0_t))pkXXX_bound_linexpr_quasilinear
    );
}
void pkXXX_bound_texpr(ap_manager_t* man,
		       ap_coeff_t interval, pkXXX_t* po, ap_texpr0_t* expr)
{
  return
    ap_generic_bound_texpr(
	man,interval,po,expr,AP_SCALAR_MPQ,true,AP_LINEXPR_INTLINEAR,
	(void (*)(ap_manager_t*,ap_coeff_t,void*,ap_linexpr0_t))pkXXX_bound_linexpr_quasilinear
);
}

/* ====================================================================== */
/* Converting to a set of constraints */
/* ====================================================================== */

void pkXXX_to_lincons_array(ap_manager_t* man,
			    ap_lincons0_array_t array,
			    pkXXX_t* po)
{
  matrixXXX_t* C;
  size_t i,k;
  ap_lincons0_t lincons0;
  bool exact,exact1;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  pkXXX_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    ap_lincons0_array_resize(array,0);
    return;
  }
  if (!po->C){ /* po is empty */
    ap_lincons0_array_resize(array,1);
    ap_lincons0_array_ref_index(lincons0,array,0);
    ap_lincons0_set_bool(lincons0,false);
    return;
  }
  pkXXX_obtain_sorted_C(pk,po);
  C = po->C;
  ap_lincons0_array_resize(array,C->nbrows);
  exact = true;
  for (i=0,k=0; i<C->nbrows; i++){
    if (! vectorXXX_is_dummy_constraint(pk, C->p[i], C->nbcolumns)){
      ap_lincons0_array_ref_index(lincons0,array,k);
      exact = lincons0_set_vectorXXX(pk, lincons0, C->p[i], C->nbcolumns) && exact;
      k++;
    }
  }
  ap_lincons0_array_resize(array,k);
  man->result.flag_exact = exact;
  man->result.flag_best = true;
}

ap_tcons0_array_t pkXXX_to_tcons_array(ap_manager_t* man, pkXXX_t* po)
{
  return ap_generic_to_tcons_array(man,po,AP_SCALAR_MPQ);
}

/* ====================================================================== */
/* Converting to a box */
/* ====================================================================== */

void pkXXX_to_box(ap_manager_t* man, ap_linexpr0_t box, pkXXX_t* po)
{
  size_t i,size;
  bool exact;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_TO_BOX);

  size = ap_dimension_size(po->dim);
  if (!pk->option.op_lazy)
    pkXXX_chernikova(man,po,NULL);
  else
    pkXXX_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    ap_linexpr0_resize(box,0);
    return;
  }
  if (!po->F){
    ap_linexpr0_set_zero(box);
    ap_coeff_t coeff;
    ap_linexpr0_cstref(coeff,box);
    ap_coeff_set_bottom(coeff);
    if (size>0){
      ap_linexpr0_resize(box,1);
      ap_linexpr0_coeffref(coeff,box,0);
      ap_coeff_set_bottom(coeff);
    }
    exact = true;
  }
  else {
    matrixXXX_to_box(pk,pk->envXXX,po->F);
    size_t oldsize = pk->envXXX->effsize;
    pk->envXXX->effsize = size;
    exact = ap_linexpr0_set_linexprXXX(box,pk->envXXX,pk->num);
    pk->envXXX->effsize = oldsize;
  }
  man->result.flag_exact = exact;
  man->result.flag_best = true;
  return;
}

/* ====================================================================== */
/* Converting to a set of generators */
/* ====================================================================== */

/* The function returns the set of generators for the topological closure of
   the polyhedron. */

void pkXXX_to_lingen_array(ap_manager_t* man,
			   ap_lingen0_array_t array, pkXXX_t* po)
{
  matrixXXX_t* F;
  size_t i,k;
  ap_lingen0_t lingen0;
  bool exact;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_TO_LINGEN_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  pkXXX_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    ap_lingen0_array_resize(array,0);
    return;
  }
  if (!po->F){ /* po is empty */
    ap_lingen0_array_resize(array,0);
    return;
  }
  F = po->F;
  pkXXX_obtain_sorted_F(pk,po);
  ap_lingen0_array_resize(array,F->nbrows);
  exact = true;
  for (i=0,k=0; i<F->nbrows; i++){
    if (! vectorXXX_is_dummy_or_strict_generator(pk, F->p[i], F->nbcolumns)){
      ap_lingen0_array_ref_index(lingen0,array,k);
      exact = lingen0_set_vectorXXX(pk, lingen0, F->p[i], F->nbcolumns) && exact;
      k++;
    }
  }
  ap_lingen0_array_resize(array,k);
  man->result.flag_exact = exact;
  man->result.flag_best = true;
}
