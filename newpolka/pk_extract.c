/* ********************************************************************** */
/* pk_extract.c: property extraction */
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
#include "pk_extract.h"
#include "ap_generic.h"
#include "itv_linearize.h"

/* Bounding the value of a dimension in a matrix of generators. */

void pk_matrix_bound_dimension(pk_internal_t* pk,
			    itv_t itv,
			    ap_dim_t dim,
			    pk_matrix_t* F)
{
  size_t i, index;
  int sgn;

  assert(pk->dec+dim<F->nbcolumns);
  
  bound_set_infty(itv->inf,-1);
  bound_set_infty(itv->sup,-1);
  index = pk->dec+dim;
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numint_sgn(F->p[i][polka_eps])==0){
      sgn = numint_sgn(F->p[i][index]);
      if (numint_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  itv_set_top(itv);
	  return;
	}
      }
      else if (numint_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  bound_set_infty(itv->sup,+1);
	  if (bound_infty(itv->inf) && bound_sgn(itv->inf)>0)
	    return;
	}
	else if (sgn < 0){
	  bound_set_infty(itv->inf,+1);
	  if (bound_infty(itv->sup) && bound_sgn(itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numrat_set_numint2(pk->poly_numrat,
			   F->p[i][index],
			   F->p[i][polka_cst]);
	if (bound_cmp_num(itv->sup,pk->poly_numrat)<0){
	  bound_set_num(itv->sup,pk->poly_numrat);
	}
	numrat_neg(pk->poly_numrat,pk->poly_numrat);
	if (bound_cmp_num(itv->inf,pk->poly_numrat)<0){
	  bound_set_num(itv->inf,pk->poly_numrat);
	}
      }	  
    }
  }
}

itv_t* pk_matrix_to_box(pk_internal_t* pk,
		     pk_matrix_t* F)
{
  size_t i,dim;
  itv_t* res;

  assert(F);
  assert(F->nbcolumns>=pk->dec);
  dim = F->nbcolumns - pk->dec;
  res = itv_array_alloc(dim);
  for (i=0;i<dim;i++){
    pk_matrix_bound_dimension(pk,res[i],i,F);
  }
  return res;
}

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/

void pk_matrix_bound_vector(pk_internal_t* pk,
			 itv_t itv,
			 numint_t* vec,
			 pk_matrix_t* F)
{
  size_t i;
  int sgn;
  
  bound_set_infty(itv->inf,-1);
  bound_set_infty(itv->sup,-1);

  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numint_sgn(F->p[i][polka_eps])==0 ){
      vector_product_strict(pk,
			    pk->poly_prod,
			    F->p[i],
			    vec, F->nbcolumns);
      sgn = numint_sgn(pk->poly_prod);
      if (numint_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (sgn){
	  itv_set_top(itv);
	  return;
	}
      }
      else if (numint_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (sgn > 0){
	  bound_set_infty(itv->sup,+1);
	  if (bound_infty(itv->inf) && bound_sgn(itv->inf)>0)
	    return;
	}
	else if (sgn < 0){
	  bound_set_infty(itv->inf,+1);
	  if (bound_infty(itv->sup) && bound_sgn(itv->sup)>0)
	    return;
	}
      }
      else {
	/* point */
	numrat_set_numint2(pk->poly_numrat,
			   pk->poly_prod,
			   F->p[i][polka_cst]);
	if (bound_cmp_num(itv->sup,pk->poly_numrat)<0){
	  bound_set_num(itv->sup,pk->poly_numrat);
	}
	numrat_neg(pk->poly_numrat,pk->poly_numrat);
	if (bound_cmp_num(itv->inf,pk->poly_numrat)<0){
	  bound_set_num(itv->inf,pk->poly_numrat);
	}
      }	  
    }
  }
  if (!bound_infty(itv->inf)){
    numint_mul(numrat_denref(bound_numref(itv->inf)),
	       numrat_denref(bound_numref(itv->inf)),
	       vec[0]);
    numrat_canonicalize(bound_numref(itv->inf));
  }
  if (!bound_infty(itv->sup)){
    numint_mul(numrat_denref(bound_numref(itv->sup)),
	       numrat_denref(bound_numref(itv->sup)),
	       vec[0]);
    numrat_canonicalize(bound_numref(itv->sup));
  }
}

/* Bounding the value of an (interval) linear expression (itv_linexpr) in a
   generator vector
*/
static
void vector_bound_itv_linexpr(pk_internal_t* pk,
			      itv_t itv,
			      itv_linexpr_t* linexpr,
			      numint_t* vec, size_t size)
{
  size_t i,dim;
  bool *peq;
  itv_ptr pitv;
  itv_ptr prod;
  numrat_t* rat;

  prod = pk->poly_itv;
  rat = &pk->poly_numrat;

  numrat_set_int(*rat,1);
  itv_set_int(itv,0);
  itv_linexpr_ForeachLinterm(linexpr,i,dim,pitv,peq){
    size_t index = pk->dec + dim;
    if (numint_sgn(vec[index])){
      numint_set(numrat_numref(*rat),vec[index]);
      itv_mul_num(prod,pitv,*rat);
      itv_add(itv,itv,prod);
    }
  }
  if (numint_sgn(vec[polka_cst])){
    numint_set(numrat_numref(*rat),vec[polka_cst]);
    itv_div_num(itv,itv,*rat);
    itv_add(itv,itv,linexpr->cst);
  }
  return;
}
			      
/* Bounding the value of an (interval) linear expression (itv_linexpr) in a
   matrix of generators.
*/
static
void pk_matrix_bound_itv_linexpr(pk_internal_t* pk,
			      itv_t itv,
			      itv_linexpr_t* linexpr,
			      pk_matrix_t* F)
{
  size_t i;
  int sgn;
  itv_t prod;

  bound_set_infty(itv->inf,-1);
  bound_set_infty(itv->sup,-1);

  itv_init(prod);
  for (i=0; i<F->nbrows; i++){
    if (!pk->strict || numint_sgn(F->p[i][polka_eps])==0 ){
      vector_bound_itv_linexpr(pk, prod, linexpr, F->p[i], F->nbcolumns);
      if (numint_sgn(F->p[i][0])==0){
	/* line: result should be zero */
	if (!itv_is_zero(prod)){
	  itv_set_top(itv);
	  goto _matrix_bound_itv_linexpr_exit;
	}
      }
      else if (numint_sgn(F->p[i][polka_cst])==0){
	/* ray */
	if (!itv_is_zero(prod)){
	  if (bound_sgn(prod->inf)<0){
	    /* [inf,sup]>0 */
	    bound_set_infty(itv->sup,+1);
	    if (bound_infty(itv->inf) && bound_sgn(itv->inf)>0)
	      goto _matrix_bound_itv_linexpr_exit;
	  }
	  else if (bound_sgn(prod->sup)<0){
	    /* [inf,sup]<0 */
	    bound_set_infty(itv->inf,+1);
	    if (bound_infty(itv->sup) && bound_sgn(itv->sup)>0)
	      goto _matrix_bound_itv_linexpr_exit;
	  }
	  else {
	    itv_set_top(itv);
	    goto _matrix_bound_itv_linexpr_exit;
	  }
	}
      }
      else {
	itv_join(itv,itv,prod);
      }
    }
  }
 _matrix_bound_itv_linexpr_exit:
  itv_clear(prod);
  return;
}

/* ====================================================================== */
/* Bounding the value of a dimension in a polyhedra */
/* ====================================================================== */

ap_interval_t* pk_bound_dimension(ap_manager_t* man,
				  pk_t* po,
				  ap_dim_t dim)
{
  itv_t itv;
  ap_interval_t* interval;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_DIMENSION);

  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_MPQ);
  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    ap_interval_set_top(interval);
    return interval;
  }

  if (!po->F){ /* po is empty */
    ap_interval_set_bottom(interval);
    man->result.flag_exact = man->result.flag_best = true;
    return interval;
  }

  itv_init(itv);
  pk_matrix_bound_dimension(pk,itv,dim,po->F);
  ap_interval_set_itv(pk->itv,interval, itv);
  itv_clear(itv);
  man->result.flag_exact = man->result.flag_best = 
    dim<po->intdim ? false : true;

  return interval;
}

/* ====================================================================== */
/* Bounding the value of a linear expression in a polyhedra */
/* ====================================================================== */

ap_interval_t* pk_bound_linexpr(ap_manager_t* man,
				pk_t* po,
				ap_linexpr0_t* expr)
{
  bool exact;
  ap_interval_t* interval;
  itv_t itv;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_LINEXPR);

  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_MPQ);
  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    ap_interval_set_top(interval);
    return interval;
  }

  if (!po->F){ /* po is empty */
    ap_interval_set_bottom(interval);
    man->result.flag_exact = man->result.flag_best = true;
    return interval;
  }
  
  /* we fill the vector with the expression, taking lower bound of the interval
     constant */
  exact = itv_linexpr_set_ap_linexpr0(pk->itv,
				      &pk->poly_itv_linexpr,
				      expr);
  itv_init(itv);
  pk_matrix_bound_itv_linexpr(pk,itv,&pk->poly_itv_linexpr,po->F);
  ap_interval_set_itv(pk->itv,interval,itv);
  itv_clear(itv);
  
  man->result.flag_exact = man->result.flag_best = 
    ( (pk->funopt->flag_exact_wanted || pk->funopt->flag_best_wanted) &&
      ap_linexpr0_is_real(expr,po->intdim) ) ? 
    exact :
    false;
  
  return interval;
}

ap_interval_t* pk_bound_texpr(ap_manager_t* man,
			      pk_t* po,
			      ap_texpr0_t* expr)
{
  itv_t itv1,itv2;
  itv_t* env;
  ap_interval_t* interval;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_BOUND_TEXPR);

  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_MPQ);
  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    ap_interval_set_top(interval);
    return interval;
  }
  if (!po->F){ /* po is empty */
    ap_interval_set_bottom(interval);
    man->result.flag_exact = man->result.flag_best = true;
    return interval;
  }
  env = pk_matrix_to_box(pk,po->F);
  itv_intlinearize_ap_texpr0(pk->itv,&pk->poly_itv_linexpr,
			     expr,env,po->intdim);
  itv_init(itv1); itv_init(itv2);
  pk_matrix_bound_itv_linexpr(pk,itv1,&pk->poly_itv_linexpr,po->F);
  itv_eval_ap_texpr0(pk->itv,itv2,expr,env);
  itv_meet(pk->itv,itv1,itv1,itv2);
  ap_interval_set_itv(pk->itv,interval,itv1);
  itv_clear(itv1); itv_clear(itv2);
  itv_array_free(env,po->intdim+po->realdim);
  man->result.flag_exact = man->result.flag_best = ap_texpr0_is_interval_linear(expr);

  return interval;
}


/* ====================================================================== */
/* Converting to a set of constraints */
/* ====================================================================== */

ap_lincons0_array_t pk_to_lincons_array(ap_manager_t* man,
					pk_t* po)
{
  ap_lincons0_array_t array;
  pk_matrix_t* C;
  size_t i,k;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  poly_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    array = ap_lincons0_array_make(0);
    return array;
  }
  if (!po->C){ /* po is empty */
    array = ap_lincons0_array_make(1);
    array.p[0] = ap_lincons0_make_unsat();
    return array;
  }
  poly_obtain_sorted_C(pk,po);
  C = po->C;
  array = ap_lincons0_array_make(C->nbrows);
  for (i=0,k=0; i<C->nbrows; i++){
    if (! vector_is_dummy_constraint(pk,
				     C->p[i], C->nbcolumns)){
      array.p[k] = lincons0_of_vector(pk, C->p[i], C->nbcolumns);
      k++;
    }
  }
  array.size = k;
  return array;
}

ap_tcons0_array_t pk_to_tcons_array(ap_manager_t* man,
				    pk_t* po)
{
  return ap_generic_to_tcons_array(man,po);
}

/* ====================================================================== */
/* Converting to a box */
/* ====================================================================== */

ap_interval_t** pk_to_box(ap_manager_t* man,
			  pk_t* po)
{
  ap_interval_t** interval;
  itv_t* titv;
  size_t i,dim;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_BOX);

  dim = po->intdim + po->realdim;
  if (pk->funopt->algorithm>=0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    interval = ap_interval_array_alloc(dim);
    for (i=0; i<dim; i++){
      ap_interval_set_top(interval[i]);
    }
    return interval;
  }
  interval = ap_interval_array_alloc(dim);
  if (!po->F){
    for (i=0; i<dim; i++){
      ap_interval_set_bottom(interval[i]);
    }
  }
  else {
    titv = pk_matrix_to_box(pk,po->F);
    for (i=0; i<dim; i++){
      ap_interval_set_itv(pk->itv,interval[i],titv[i]);
    }
    itv_array_free(titv,dim);
  }
  man->result.flag_exact = man->result.flag_best = true;
  return interval;
}

/* ====================================================================== */
/* Converting to a set of generators */
/* ====================================================================== */

/* The function returns the set of generators for the topological closure of
   the polyhedron. */

ap_generator0_array_t pk_to_generator_array(ap_manager_t* man,
					    pk_t* po)
{
  ap_generator0_array_t array;
  pk_matrix_t* F;
  size_t i,k;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_TO_GENERATOR_ARRAY);

  man->result.flag_exact = man->result.flag_best = true;

  poly_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    array = ap_generator0_array_make(0);
    return array;
  }
  if (!po->F){ /* po is empty */
    array = ap_generator0_array_make(0);
    return array;
  }
  F = po->F;
  poly_obtain_sorted_F(pk,po);
  array = ap_generator0_array_make(F->nbrows);
  for (i=0,k=0; i<F->nbrows; i++){
    if (! vector_is_dummy_or_strict_generator(pk,
					      F->p[i], F->nbcolumns)){
      array.p[k] = generator0_of_vector(pk, F->p[i], F->nbcolumns);
      k++;
    }
  }
  array.size = k;
  return array;
}
