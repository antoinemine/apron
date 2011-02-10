/* ********************************************************************** */
/* pk_extract.c: property extraction */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pk_internal.h"
#include "itvMPQ.h"
#include "eitvMPQ.h"
#include "ap_generic.h"

/* Bounding the value of a dimension in a matrix of generators. */

void matrix_bound_dimension(pk_internal_t* pk,
			    eitvMPQ_t eitv,
			    ap_dim_t dim,
			    matrix_t* F)
{
  size_t i, index;
  int sgn;

  assert(pk->dec+dim<F->nbcolumns);

  eitv->eq = false;
  boundMPQ_set_infty(eitv->itv->neginf,-1);
  boundMPQ_set_infty(eitv->itv->sup,-1);
  index = pk->dec+dim;
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintMPQ_sgn(F->p[i][polka_eps])==0){
      sgn = numintMPQ_sgn(F->p[i][index]);
      if (numintMPQ_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  eitvMPQ_set_top(eitv);
	  return;
	}
      }
      else if (numintMPQ_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  boundMPQ_set_infty(eitv->itv->sup,+1);
	  if (boundMPQ_infty(eitv->itv->neginf) && boundMPQ_sgn(eitv->itv->neginf)>0)
	    return;
	}
	else if (sgn < 0){
	  boundMPQ_set_infty(eitv->itv->neginf,+1);
	  if (boundMPQ_infty(eitv->itv->sup) && boundMPQ_sgn(eitv->itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numMPQ_set_numintMPQ2(pk->poly_numrat,
			      F->p[i][index],
			      F->p[i][polka_cst]);
	if (boundMPQ_cmp_num(eitv->itv->sup,pk->poly_numrat)<0){
	  boundMPQ_set_num(eitv->itv->sup,pk->poly_numrat);
	}
	numMPQ_neg(pk->poly_numrat,pk->poly_numrat);
	if (boundMPQ_cmp_num(eitv->itv->neginf,pk->poly_numrat)<0){
	  boundMPQ_set_num(eitv->itv->neginf,pk->poly_numrat);
	}
      }	  
    }
  }
  eitv->eq = itvMPQ_is_point(eitv->itv);
  return;
}

void matrix_to_box(pk_internal_t* pk,
		   eitvMPQ_t* res,
		   matrix_t* F)
{
  size_t i,dim;

  assert(F);
  assert(F->nbcolumns>=pk->dec);
  dim = F->nbcolumns - pk->dec;
  for (i=0;i<dim;i++){
    matrix_bound_dimension(pk,res[i],i,F);
  }
}

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/

void matrix_bound_vector(pk_internal_t* pk,
			 eitvMPQ_t eitv,
			 numintMPQ_t* vec,
			 matrix_t* F)
{
  size_t i;
  int sgn;
  
  eitv->eq = false;
  boundMPQ_set_infty(eitv->itv->neginf,-1);
  boundMPQ_set_infty(eitv->itv->sup,-1);

  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintMPQ_sgn(F->p[i][polka_eps])==0 ){
      vector_product_strict(pk, pk->poly_prod, F->p[i], vec, F->nbcolumns);
      sgn = numintMPQ_sgn(pk->poly_prod);
      if (numintMPQ_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  eitvMPQ_set_top(eitv);
	  return;
	}
      }
      else if (numintMPQ_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  boundMPQ_set_infty(eitv->itv->sup,+1);
	  if (boundMPQ_infty(eitv->itv->neginf) && boundMPQ_sgn(eitv->itv->neginf)>0)
	    return;
	}
	else if (sgn < 0){
	  boundMPQ_set_infty(eitv->itv->neginf,+1);
	  if (boundMPQ_infty(eitv->itv->sup) && boundMPQ_sgn(eitv->itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numMPQ_set_numintMPQ2(pk->poly_numrat, 
			      pk->poly_prod,F->p[i][polka_cst]);
	if (boundMPQ_cmp_num(eitv->itv->sup,pk->poly_numrat)<0){
	  boundMPQ_set_num(eitv->itv->sup,pk->poly_numrat);
	}
	numMPQ_neg(pk->poly_numrat,pk->poly_numrat);
	if (boundMPQ_cmp_num(eitv->itv->neginf,pk->poly_numrat)<0){
	  boundMPQ_set_num(eitv->itv->neginf,pk->poly_numrat);
	}
      }	  
    }
  }
  if (!boundMPQ_infty(eitv->itv->neginf)){
    numintMPQ_mul(numMPQ_denref(boundMPQ_numref(eitv->itv->neginf)),
		  numMPQ_denref(boundMPQ_numref(eitv->itv->neginf)),
		  vec[0]);
    numMPQ_canonicalize(boundMPQ_numref(eitv->itv->neginf));
  }
  if (!boundMPQ_infty(eitv->itv->sup)){
    numintMPQ_mul(numMPQ_denref(boundMPQ_numref(eitv->itv->sup)),
		  numMPQ_denref(boundMPQ_numref(eitv->itv->sup)),
		  vec[0]);
    numMPQ_canonicalize(boundMPQ_numref(eitv->itv->sup));
  }
  eitv->eq = itvMPQ_is_point(eitv->itv);
}

/* Bounding the value of an (interval) linear expression (ap_linexprMPQ) in a
   generator vector
*/
static
void vector_bound_ap_linexprMPQ(pk_internal_t* pk,
				eitvMPQ_t eitv,
				ap_linexprMPQ_t linexpr,
				numintMPQ_t* vec, size_t size)
{
  size_t i,dim;
  eitvMPQ_ptr peitv;
  eitvMPQ_ptr prod;
  numMPQ_ptr rat;

  prod = pk->poly_eitv;
  rat = pk->poly_numrat;

  numMPQ_set_int(rat,1);
  eitvMPQ_set_int(eitv,0);
  ap_linexprMPQ_ForeachLinterm0(linexpr,i,dim,peitv){
    size_t index = pk->dec + dim;
    if (numintMPQ_sgn(vec[index])){
      numintMPQ_set(numMPQ_numref(rat),vec[index]);
      eitvMPQ_mul_num(prod,peitv,rat);
      eitvMPQ_add(eitv,eitv,prod);
    }
  }
  if (numintMPQ_sgn(vec[polka_cst])){
    numintMPQ_set(numMPQ_numref(rat),vec[polka_cst]);
    eitvMPQ_div_num(eitv,eitv,rat);
    eitvMPQ_add(eitv,eitv,linexpr->cst);
  }
  return;
}
			      
/* Bounding the value of an (interval) linear expression (ap_linexprMPQ) in a
   matrix of generators.
*/
static
void matrix_bound_ap_linexprMPQ(pk_internal_t* pk,
				eitvMPQ_t eitv,
				ap_linexprMPQ_t linexpr,
				matrix_t* F)
{
  size_t i;
  int sgn;
  eitvMPQ_t prod;

  eitv->eq = false;
  boundMPQ_set_infty(eitv->itv->neginf,-1);
  boundMPQ_set_infty(eitv->itv->sup,-1);

  eitvMPQ_init(prod);
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numintMPQ_sgn(F->p[i][polka_eps])==0 ){
      vector_bound_ap_linexprMPQ(pk, prod, linexpr, F->p[i], F->nbcolumns);
      if (numintMPQ_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (!eitvMPQ_is_zero(prod)){
	  eitvMPQ_set_top(eitv);
	  goto _matrix_bound_ap_linexprMPQ_exit;
	}
      }
      else if (numintMPQ_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (!eitvMPQ_is_zero(prod)){
	  if (boundMPQ_sgn(prod->itv->neginf)<0){
	    /* [inf,sup]>0 */
	    boundMPQ_set_infty(eitv->itv->sup,+1);
	    if (boundMPQ_infty(eitv->itv->neginf) && boundMPQ_sgn(eitv->itv->neginf)>0)
	      goto _matrix_bound_ap_linexprMPQ_exit;
	  }
	  else if (boundMPQ_sgn(prod->itv->sup)<0){
	    /* [inf,sup]<0 */
	    boundMPQ_set_infty(eitv->itv->neginf,+1);
	    if (boundMPQ_infty(eitv->itv->sup) && boundMPQ_sgn(eitv->itv->sup)>0)
	      goto _matrix_bound_ap_linexprMPQ_exit;
	  }
	  else {
	    eitvMPQ_set_top(eitv);
	    goto _matrix_bound_ap_linexprMPQ_exit;
	  }
	}
      }
      else {
	eitvMPQ_join(eitv,eitv,prod);
      }
    }
  }
 _matrix_bound_ap_linexprMPQ_exit:
  eitvMPQ_clear(prod);
  eitv->eq = itvMPQ_is_point(eitv->itv);  
  return;
}

/* ====================================================================== */
/* Bounding the value of a dimension in a polyhedra */
/* ====================================================================== */

void pk_bound_dimension(ap_manager_t* man,
			ap_coeff_t interval,
			pk_t* po,
			ap_dim_t dim)
{
  eitvMPQ_t eitv;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_DIMENSION);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

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

  eitvMPQ_init(eitv);
  matrix_bound_dimension(pk,eitv,dim,po->F);
  man->result.flag_exact = ap_coeff_set_eitvMPQ(interval,eitv,pk->num);
  eitvMPQ_clear(eitv);
  man->result.flag_exact &= dim<po->dim.intd;
  man->result.flag_best = man->result.flag_exact;
}

/* ====================================================================== */
/* Bounding the value of a linear expression in a polyhedra */
/* ====================================================================== */

void pk_bound_linexpr(ap_manager_t* man,
		      ap_coeff_t interval,
		      pk_t* po,
		      ap_linexpr0_t expr)
{
  bool exact;
  eitvMPQ_t eitv;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_LINEXPR);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

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
  
  /* we fill the vector with the expression, taking lower bound of the interval
     constant */
  exact = ap_linexprMPQ_set_linexpr0(pk->poly_linexprMPQ, expr, pk->num);
  eitvMPQ_init(eitv);
  matrix_bound_ap_linexprMPQ(pk,eitv,pk->poly_linexprMPQ,po->F);
  exact = ap_coeff_set_eitvMPQ(interval,eitv,pk->num) && exact;
  eitvMPQ_clear(eitv);
  
  man->result.flag_exact = man->result.flag_best = 
    ( (pk->funopt->flag_exact_wanted || pk->funopt->flag_best_wanted) &&
      ap_linexprMPQ_is_real(pk->poly_linexprMPQ,po->dim.intd) ) ? 
    exact :
    false;
  
  return;
}

void pk_bound_texpr(ap_manager_t* man,
		    ap_coeff_t interval, pk_t* po, ap_texpr0_t* expr)
{
  eitvMPQ_t eitv1,eitv2;
  bool exact;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_TEXPR);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

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
  matrix_to_box(pk,pk->env,po->F);
  ap_linexprMPQ_intlinearize_texpr0(
      pk->poly_linexprMPQ, expr, pk->env, po->dim.intd, pk->num);
  eitvMPQ_init(eitv1); 
  eitvMPQ_init(eitv2);
  matrix_bound_ap_linexprMPQ(pk,eitv1,pk->poly_linexprMPQ,po->F);
  eitvMPQ_eval_ap_texpr0(eitv2,expr,pk->env,pk->num);
  eitvMPQ_meet(eitv1,eitv1,eitv2,pk->num);
  exact = ap_coeff_set_eitvMPQ(interval,eitv1,pk->num);
  eitvMPQ_clear(eitv1); 
  eitvMPQ_clear(eitv2);
  man->result.flag_exact = exact && ap_linexprMPQ_is_quasilinear(pk->poly_linexprMPQ) && ap_linexprMPQ_is_real(pk->poly_linexprMPQ,po->dim.intd);
  
  man->result.flag_best = exact;
}


/* ====================================================================== */
/* Converting to a set of constraints */
/* ====================================================================== */

void pk_to_lincons_array(ap_manager_t* man,	
			 ap_lincons0_array_t array,
			 pk_t* po)
{
  matrix_t* C;
  size_t i,k;
  ap_lincons0_t lincons0;
  bool exact,exact1;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  poly_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    ap_lincons0_array_resize(array,0);
    return;
  }
  if (!po->C){ /* po is empty */
    ap_lincons0_array_resize(array,0);
    ap_lincons0_array_ref_index(lincons0,array,0);
    ap_lincons0_set_bool(lincons0,false);
    return;
  }
  poly_obtain_sorted_C(pk,po);
  C = po->C;
  ap_lincons0_array_resize(array,C->nbrows);
  exact = true;
  for (i=0,k=0; i<C->nbrows; i++){
    if (! vector_is_dummy_constraint(pk, C->p[i], C->nbcolumns)){
      ap_lincons0_array_ref_index(lincons0,array,k);
      exact = lincons0_set_vector(pk, lincons0, C->p[i], C->nbcolumns) && exact;
      k++;
    }
  }
  ap_lincons0_array_resize(array,k);
  man->result.flag_exact = exact;
  man->result.flag_best = true;
}

ap_tcons0_array_t pk_to_tcons_array(ap_manager_t* man, pk_t* po)
{
  return ap_generic_to_tcons_array(man,po,AP_SCALAR_MPQ);
}

/* ====================================================================== */
/* Converting to a box */
/* ====================================================================== */

void pk_to_box(ap_manager_t* man, ap_box0_t box, pk_t* po)
{
  size_t i,dim;
  bool exact;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_BOX);

  dim = po->dim.intd + po->dim.reald;
  ap_box0_resize(box,dim);
  if (pk->funopt->algorithm>=0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    ap_box0_set_top(box);
    return;
  }
  if (!po->F){
    ap_box0_set_bottom(box);
    exact = true;
  }
  else {
    matrix_to_box(pk,pk->env,po->F);
    exact = ap_box0_set_eitvMPQ_array(box,pk->env,dim,pk->num);
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

void pk_to_lingen_array(ap_manager_t* man,
			   ap_lingen0_array_t array, pk_t* po)
{
  matrix_t* F;
  size_t i,k;
  ap_lingen0_t lingen0;
  bool exact;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_LINGEN_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  poly_chernikova3(man,po,NULL);
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
  poly_obtain_sorted_F(pk,po);
  ap_lingen0_array_resize(array,F->nbrows);
  exact = true;
  for (i=0,k=0; i<F->nbrows; i++){
    if (! vector_is_dummy_or_strict_generator(pk, F->p[i], F->nbcolumns)){
      ap_lingen0_array_ref_index(lingen0,array,k);
      exact = lingen0_set_vector(pk, lingen0, F->p[i], F->nbcolumns) && exact;
      k++;
    }
  }
  ap_lingen0_array_resize(array,k);
  man->result.flag_exact = exact;
  man->result.flag_best = true;
}
