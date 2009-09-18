/* ********************************************************************** */
/* box_meetjoin.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box_representation.h"
#include "box_constructor.h"
#include "box_meetjoin.h"

#include "itv_linexpr.h"
#include "itv_linearize.h"

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

box_t* box_meet(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2)
{
  size_t i;
  bool exc;
  size_t nbdims;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  res = destructive ? a1 : box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL || a2->p==NULL){
    box_set_bottom(res);
    return res;
  }
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a1->realdim;
  for (i=0; i<nbdims; i++){
    exc = itv_meet(intern->itv,res->p[i],a1->p[i],a2->p[i]);
    if (exc){
      box_set_bottom(res);
      break;
    }
  }
  return res;
}

box_t* box_join(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2)
{
  size_t i;
  size_t nbdims;
  box_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a1 : box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL){
    if (a2->p!=NULL){
      man->result.flag_exact = true;
      box_set(res,a2);
    }
    return res;
  }
  else if (a2->p==NULL){
    man->result.flag_exact = true;
    if (!destructive) box_set(res,a1);
    return res;
  }
  man->result.flag_exact = false;
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a2->realdim;
  for (i=0; i<nbdims; i++){
    itv_join(res->p[i],a1->p[i],a2->p[i]);
  }
  return res;
}

box_t* box_meet_array(ap_manager_t* man, box_t** tab, size_t size)
{
  size_t i;
  box_t* res;
  res = box_copy(man,tab[0]);
  for (i=1;i<size;i++){
    box_meet(man,true,res,tab[i]);
    if (res->p==NULL) break;
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return res;
}
box_t* box_join_array(ap_manager_t* man, box_t** tab, size_t size)
{
  size_t i;
  box_t* res;
  res = box_copy(man,tab[0]);
  for (i=1;i<size;i++){
    box_join(man,true,res,tab[i]);
  }
  man->result.flag_best = true;
  man->result.flag_exact = false;
  return res;
}

/* ============================================================ */
/* Add_ray_array */
/* ============================================================ */


/* Generalized time elapse operator */
void box_add_ray(box_internal_t* intern,
		 box_t* a,
		 ap_generator0_t* gen)
{
  size_t i;
  int sgn;
  ap_coeff_t* coeff;
  ap_dim_t dim;
  ap_linexpr0_t* expr;

  assert(gen->gentyp != AP_GEN_VERTEX);
  if (a->p==NULL){
    box_set_bottom(a);
    return;
  }
  expr = gen->linexpr0;
  ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
    assert(coeff->discr==AP_COEFF_SCALAR);
    ap_scalar_t* scalar = coeff->val.scalar;
    sgn = ap_scalar_sgn(scalar);
    if (sgn!=0){
      if (sgn>0 || gen->gentyp==AP_GEN_LINE){
	bound_set_infty(a->p[dim]->sup,1);
      }
      if (sgn<0 || gen->gentyp==AP_GEN_LINE){
	bound_set_infty(a->p[dim]->inf,1);
      }
    }
  }
}

box_t* box_add_ray_array(ap_manager_t* man,
			 bool destructive,
			 box_t* a,
			 ap_generator0_array_t* array)
{
  size_t i;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_exact = true;
    return res;
  }
  for (i=0;i<array->size; i++){
    box_add_ray(intern,res,&array->p[i]);
  }
  return res;
}


/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

/* Meet of an abstract value with a constraint */
bool box_meet_lincons_internal(box_internal_t* intern,
			       box_t* a,
			       itv_lincons_t* cons)
{
  size_t nbcoeffs,nbdims;
  ap_dim_t dim;
  size_t i;
  itv_ptr pitv;
  itv_linexpr_t* expr;
  bool *peq;
  bool equality,change,globalchange;
  bool exc;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (a->p==NULL){
    box_set_bottom(a);
    return false;
  }
  nbdims = a->intdim + a->realdim;
  expr = &cons->linexpr;
  globalchange = false;

  /* Iterates on coefficients */
  nbcoeffs = 0;
  itv_set_int(intern->meet_lincons_internal_itv2,0);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    nbcoeffs++;
    /* 1. We decompose the expression e = ax+e' */
    /* We save the linterm */
    itv_swap(intern->meet_lincons_internal_itv2,pitv);
    equality = *peq;
    *peq = true;
    /* 2. evaluate e' */
    itv_eval_linexpr(intern->itv,
		     intern->meet_lincons_internal_itv3,expr,a->p);
    change = false;
    if (!itv_is_top(intern->meet_lincons_internal_itv3)){
      if (equality){
	int sgn = bound_sgn(intern->meet_lincons_internal_itv2->sup);
	if (sgn!=0){
	  /*
	    If we have ax+e' >= 0 with a>0
	    we can deduce that x>=-e'/a, or x>= inf(-e'/a)
	    If we have ax+e' >= 0 with a<0
	    we can deduce that -ax<=e', or x<= sup(e'/-a)
	    If we have ax+e'=0
	    we can deduce x=-e'/a, or inf(-e'/a)<= x <= sup(-e'/a)
	  */
	  if (sgn>0 || cons->constyp == AP_CONS_EQ){
	    /*
	       If we have a>0, we compute sup(e')/a=sup(e'/a)=-inf(-e'/a)
	       If we have a<0, we compute -inf(e')/(-a)=-inf(-e'/a)
	    */
	    if (sgn>0){
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup,
			intern->meet_lincons_internal_itv2->sup);
	    }
	    else {
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->inf,
			intern->meet_lincons_internal_itv2->inf);
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->meet_lincons_internal_bound, a->p[dim]->inf)<0){
	      change = true;
	      bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == AP_CONS_EQ){
	    /*
	       If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	       If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->sup,
			intern->meet_lincons_internal_itv2->inf);
	    }
	    else {
	      bound_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->inf,
			intern->meet_lincons_internal_itv2->sup);
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->meet_lincons_internal_bound, a->p[dim]->sup)<0){
	      change = true;
	      bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    }
	  }
	}
      }
      else {
	/* We have an interval */
	/*
	  - If we have [m;M]x+e' >= 0 with m>0, 
	    then [m,M]x>=inf(-e'), or [m,M]x>=-sup(e')
	    so we need at least
	    * if -sup(e')>=0: x>=-sup(e')/M
	    * if -sup(e')<=0: x>=-sup(e')/m
	  - If we have [m,M]x+e'<=0 with M<0, then [-M,-m]x>=inf(e')
	    * inf(e')>=0: x>=inf(e')/-m
	    * inf(e')<=0: x>=inf(e')/-M

	  - If we have [m;M]x+e' >= 0 with M<0, 
	    then [-M,-m]x<=sup(e'), so we need at least
	    * if sup(e')>=0: x<=sup(e')/-M
	    * if sup(e')<=0: x<=sup(e')/-m
	  - If we have [m,M]x+e'<=0 with m>0, then [m,M]x<=sup(-e')
	    or [m,M]x<=-inf(e')
	    * -inf(e')>=0: x<=inf(e')/-M
	    * -inf(e')<=0: x<=inf(e')/-m
	*/
	int sgncoeff =
	  bound_sgn(intern->meet_lincons_internal_itv2->inf)<0 ?
	  1 :
	  ( bound_sgn(intern->meet_lincons_internal_itv2->sup)<0 ?
	    -1 :
	    0 );
	if (sgncoeff != 0){
	  int sgninf = bound_sgn(intern->meet_lincons_internal_itv3->inf);
	  int sgnsup = bound_sgn(intern->meet_lincons_internal_itv3->sup);
	  if (sgncoeff>0 || (cons->constyp==AP_CONS_EQ && sgncoeff<0)){
	    if (sgncoeff>0){
	      if (sgnsup<=0){
		/* We compute sup(e')/M */
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->sup,
			  intern->meet_lincons_internal_itv2->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		bound_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->sup);
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->inf);
	      }
	    }
	    else {
	      bound_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->inf);
	      if (sgninf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->inf);
	      } else {
		/* We compute inf(e')/M) = - (-inf(e'))/M */
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->sup);
		bound_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->meet_lincons_internal_bound, a->p[dim]->inf)<0){
	      change = true;
	      bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    }
	  }
	  if (sgncoeff<0 || (cons->constyp==AP_CONS_EQ && sgncoeff>0)){
	    if (sgncoeff<0){
	      if (sgnsup>=0){
		/* We compute sup(e')/-M */
		bound_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->sup);
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->sup,
			  intern->meet_lincons_internal_bound);
	      } else {
		/* We compute sup(e')/-m */
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->sup,
			  intern->meet_lincons_internal_itv2->inf);
	      }
	    }
	    else {
	      if (sgninf>=0){		
		/* We compute -inf(e')/M */
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->inf,
			  intern->meet_lincons_internal_itv2->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		bound_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->inf);
		bound_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->inf);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->meet_lincons_internal_bound, a->p[dim]->sup)<0){
	      change = true;
	      bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    }
	  }
	}
      }
    }
    itv_swap(intern->meet_lincons_internal_itv2,pitv);
    *peq = equality;
    if (change){
      globalchange = true;
      exc = itv_canonicalize(intern->itv,a->p[dim],dim<a->intdim);
      if (exc){
	box_set_bottom(a);
	goto _box_meet_box_lincons_exit;
      }
    }
  }
  if (nbcoeffs==0){ /* Maybe an unsatisfiable constraint */
    bool unsat;
    int sgnsup = bound_sgn(expr->cst->sup);
    int sgninf = expr->equality ? sgnsup : -bound_sgn(expr->cst->inf);
    switch(cons->constyp){
    case AP_CONS_EQ:
      unsat = sgnsup < 0 || sgninf > 0;
      break;
    case AP_CONS_SUPEQ:
      unsat = sgnsup<0;
      break;
    case AP_CONS_SUP:
      unsat = (sgnsup<=0);
      break;
    default:
      abort();
    }
    if (unsat){
      box_set_bottom(a);
      globalchange = true;
    }
    else
      globalchange = false;
  }
 _box_meet_box_lincons_exit:
  return globalchange;
}

box_t* box_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_lincons0_array_t* array)
{
  box_t* res;
  size_t kmax;
  itv_lincons_array_t tlincons;
  box_internal_t* intern = (box_internal_t*)man->internal;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern->itv,&tlincons,array);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      goto _box_meet_lincons_array_bottom;
    }
    itv_boxize_lincons_array(intern->itv,
			     res->p,NULL,
			     &tlincons,res->p,a->intdim,kmax,false);
    if (itv_is_bottom(intern->itv,res->p[0])){
    _box_meet_lincons_array_bottom:
      box_set_bottom(res);
    }
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}

box_t* box_meet_tcons_array(ap_manager_t* man,
			    bool destructive,
			    box_t* a,
			    ap_tcons0_array_t* array)
{
  box_t* res;
  size_t kmax;
  box_internal_t* intern = (box_internal_t*)man->internal;
  itv_lincons_array_t tlincons;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_TCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    
    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern->itv,&tlincons,
				     array,res->p,res->intdim);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      goto _box_meet_tcons_array_bottom;
    }
    itv_boxize_lincons_array(intern->itv,
			     res->p,NULL,
			     &tlincons,res->p,a->intdim,kmax,false);
    if (itv_is_bottom(intern->itv,res->p[0])){
    _box_meet_tcons_array_bottom:
      box_set_bottom(res);
    }
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}
