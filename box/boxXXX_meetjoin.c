/* ********************************************************************** */
/* boxXXX_meetjoin.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boundD.h"
#include "boundMPQ.h"
#include "boundMPFR.h"

#include "boxXXX_internal.h"

#include "ap_linconsXXX.h"
#include "ap_tcons0.h"

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

boxXXX_t* boxXXX_meet(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2)
{
  size_t i;
  bool exc;
  size_t nbdims;
  boxXXX_t* res;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  res = destructive ? a1 : boxXXX_alloc(a1->dim);
  if (a1->e->linterm==NULL || a2->e->linterm==NULL){
    boxXXX_set_bottom(res);
    return res;
  }
  if (!destructive){
    boxXXX_init(res);
  }
  nbdims = a1->dim.intd + a1->dim.reald;
  for (i=0; i<nbdims; i++){
    exc = eitvXXX_meet(res->e->linterm[i]->eitv,a1->e->linterm[i]->eitv,a2->e->linterm[i]->eitv,intern->num);
    if (exc){
      boxXXX_set_bottom(res);
      break;
    }
  }
  return res;
}

boxXXX_t* boxXXX_join(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2)
{
  size_t i;
  size_t nbdims;
  boxXXX_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a1 : boxXXX_alloc(a1->dim);
  if (a1->e->linterm==NULL){
    if (a2->e->linterm!=NULL){
      man->result.flag_exact = true;
      boxXXX_set(res,a2);
    }
    return res;
  }
  else if (a2->e->linterm==NULL){
    man->result.flag_exact = true;
    if (!destructive) boxXXX_set(res,a1);
    return res;
  }
  man->result.flag_exact = false;
  if (!destructive){
    boxXXX_init(res);
  }
  nbdims = a1->dim.intd + a2->dim.reald;
  for (i=0; i<nbdims; i++){
    eitvXXX_join(res->e->linterm[i]->eitv,a1->e->linterm[i]->eitv,a2->e->linterm[i]->eitv);
  }
  return res;
}

boxXXX_t* boxXXX_meet_array(ap_manager_t* man, boxXXX_t** tab, size_t size)
{
  size_t i;
  boxXXX_t* res;
  res = boxXXX_copy(man,tab[0]);
  for (i=1;i<size;i++){
    boxXXX_meet(man,true,res,tab[i]);
    if (res->e->linterm==NULL) break;
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return res;
}
boxXXX_t* boxXXX_join_array(ap_manager_t* man, boxXXX_t** tab, size_t size)
{
  size_t i;
  boxXXX_t* res;
  res = boxXXX_copy(man,tab[0]);
  for (i=1;i<size;i++){
    boxXXX_join(man,true,res,tab[i]);
  }
  man->result.flag_best = true;
  man->result.flag_exact = false;
  return res;
}

/* ============================================================ */
/* Add_ray_array */
/* ============================================================ */

boxXXX_t* boxXXX_add_ray_array(ap_manager_t* man,
			       bool destructive,
			       boxXXX_t* a,
			       ap_lingen0_array_t array)
{
  size_t i,j;
  ap_dim_t dim;
  boxXXX_t* res;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a : boxXXX_copy(man,a);
  if (a->e->linterm==NULL){
    man->result.flag_exact = true;
    return res;
  }
  MACRO_SWITCH(array->discr) ZZZ {
    ap_lingenZZZ_array_ptr tab = array->lingen_array.ZZZ;
    eitvZZZ_ptr eitvref;
    for (i=0;i<tab->size; i++){
      ap_lingenZZZ_ptr gen = tab->p[i];
      ap_linexprZZZ_ptr expr = gen->linexpr;
      assert(gen->gentyp != AP_GEN_VERTEX);
      ap_linexprZZZ_ForeachLinterm0(expr,j,dim,eitvref){
        assert(eitvref->eq);
        int sgn = boundZZZ_sgn(eitvref->itv->sup);
        if (sgn!=0){
          res->e->linterm[dim]->eitv->eq = false;
          if (sgn>0 || gen->gentyp==AP_GEN_LINE){
            boundXXX_set_infty(res->e->linterm[dim]->eitv->itv->sup,1);
          }
          if (sgn<0 || gen->gentyp==AP_GEN_LINE){
            boundXXX_set_infty(res->e->linterm[dim]->eitv->itv->neginf,1);
          }
        }
      }
    }
  }
  ENDMACRO;
  return res;
}


/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

/* Meet of an abstract value with a constraint */
bool boxXXX_meet_lincons_internal(boxXXX_internal_t* intern,
				  boxXXX_t* a,
				  ap_linconsXXX_t cons)
{
  size_t nbcoeffs,nbdims;
  ap_dim_t dim;
  size_t i;
  eitvXXX_ptr peitv;
  ap_linexprXXX_ptr expr;
  bool equality,change,globalchange;
  bool exc;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (a->e->linterm==NULL){
    boxXXX_set_bottom(a);
    return false;
  }
  nbdims = a->dim.intd + a->dim.reald;
  expr = cons->linexpr;
  globalchange = false;

  /* Iterates on coefficients */
  nbcoeffs = 0;
  eitvXXX_set_int(intern->meet_lincons_internal_itv2,0);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,peitv){
    nbcoeffs++;
    /* 1. We decompose the expression e = ax+e' */
    /* We save the linterm */
    eitvXXX_swap(intern->meet_lincons_internal_itv2,peitv);
    /* 2. evaluate e' */
    ap_linexprXXX_eval(intern->meet_lincons_internal_itv3,expr,a->e,intern->num);
    change = false;
    if (!eitvXXX_is_top(intern->meet_lincons_internal_itv3)){
      if (intern->meet_lincons_internal_itv2->eq){
	int sgn = boundXXX_sgn(intern->meet_lincons_internal_itv2->itv->sup);
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
	      boundXXX_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->itv->sup,
			intern->meet_lincons_internal_itv2->itv->sup);
	    }
	    else {
	      boundXXX_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->itv->neginf,
			intern->meet_lincons_internal_itv2->itv->neginf);
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->meet_lincons_internal_bound, a->e->linterm[dim]->eitv->itv->neginf)<0){
	      change = true;
	      boundXXX_set(a->e->linterm[dim]->eitv->itv->neginf, intern->meet_lincons_internal_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == AP_CONS_EQ){
	    /*
	       If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	       If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      boundXXX_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->itv->sup,
			intern->meet_lincons_internal_itv2->itv->neginf);
	    }
	    else {
	      boundXXX_div(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->itv->neginf,
			intern->meet_lincons_internal_itv2->itv->sup);
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->meet_lincons_internal_bound, a->e->linterm[dim]->eitv->itv->sup)<0){
	      change = true;
	      boundXXX_set(a->e->linterm[dim]->eitv->itv->sup, intern->meet_lincons_internal_bound);
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
	  boundXXX_sgn(intern->meet_lincons_internal_itv2->itv->neginf)<0 ?
	  1 :
	  ( boundXXX_sgn(intern->meet_lincons_internal_itv2->itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgncoeff != 0){
	  int sgninf = boundXXX_sgn(intern->meet_lincons_internal_itv3->itv->neginf);
	  int sgnsup = boundXXX_sgn(intern->meet_lincons_internal_itv3->itv->sup);
	  if (sgncoeff>0 || (cons->constyp==AP_CONS_EQ && sgncoeff<0)){
	    if (sgncoeff>0){
	      if (sgnsup<=0){
		/* We compute sup(e')/M */
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->sup,
			  intern->meet_lincons_internal_itv2->itv->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		boundXXX_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->sup);
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->itv->neginf);
	      }
	    }
	    else {
	      boundXXX_neg(intern->meet_lincons_internal_bound,
			intern->meet_lincons_internal_itv3->itv->neginf);
	      if (sgninf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->itv->neginf);
	      } else {
		/* We compute inf(e')/M) = - (-inf(e'))/M */
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->itv->sup);
		boundXXX_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->meet_lincons_internal_bound, a->e->linterm[dim]->eitv->itv->neginf)<0){
	      change = true;
	      boundXXX_set(a->e->linterm[dim]->eitv->itv->neginf, intern->meet_lincons_internal_bound);
	    }
	  }
	  if (sgncoeff<0 || (cons->constyp==AP_CONS_EQ && sgncoeff>0)){
	    if (sgncoeff<0){
	      if (sgnsup>=0){
		/* We compute sup(e')/-M */
		boundXXX_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->itv->sup);
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->sup,
			  intern->meet_lincons_internal_bound);
	      } else {
		/* We compute sup(e')/-m */
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->sup,
			  intern->meet_lincons_internal_itv2->itv->neginf);
	      }
	    }
	    else {
	      if (sgninf>=0){
		/* We compute -inf(e')/M */
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->neginf,
			  intern->meet_lincons_internal_itv2->itv->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		boundXXX_neg(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv3->itv->neginf);
		boundXXX_div(intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_bound,
			  intern->meet_lincons_internal_itv2->itv->neginf);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->meet_lincons_internal_bound, a->e->linterm[dim]->eitv->itv->sup)<0){
	      change = true;
	      boundXXX_set(a->e->linterm[dim]->eitv->itv->sup, intern->meet_lincons_internal_bound);
	    }
	  }
	}
      }
    }
    eitvXXX_swap(intern->meet_lincons_internal_itv2,peitv);
    if (change){
      globalchange = true;
      exc = eitvXXX_canonicalize(a->e->linterm[dim]->eitv,dim<a->dim.intd,intern->num);
      if (exc){
	boxXXX_set_bottom(a);
	goto _boxXXX_meet_boxXXX_lincons_exit;
      }
    }
  }
  if (nbcoeffs==0){ /* Maybe an unsatisfiable constraint */
    bool unsat;
    int sgnsup = boundXXX_sgn(expr->cst->itv->sup);
    int sgninf = expr->cst->eq ? sgnsup : -boundXXX_sgn(expr->cst->itv->neginf);
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
      boxXXX_set_bottom(a);
      globalchange = true;
    }
    else
      globalchange = false;
  }
 _boxXXX_meet_boxXXX_lincons_exit:
  return globalchange;
}

boxXXX_t* boxXXX_meet_lincons_array(ap_manager_t* man,
				    bool destructive,
				    boxXXX_t* a,
				    ap_lincons0_array_t array)
{
  boxXXX_t* res;
  size_t kmax;
  ap_linconsXXX_array_t tlincons;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;

  res = destructive ? a : boxXXX_copy(man,a);
  if (a->e->linterm==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = (ap_lincons0_array_size(array)==1);
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;

    ap_linconsXXX_array_init(tlincons,0);
    ap_linconsXXX_array_set_lincons0_array(tlincons,array,intern->num);
    tbool_t tb = ap_linconsXXX_array_reduce_integer(tlincons,a->dim.intd,intern->num);
    if (tb==tbool_false){
      goto _boxXXX_meet_lincons_array_bottom;
    }
    ap_linconsXXX_array_boxize(res->e,NULL,
			       tlincons,res->e,a->dim.intd,kmax,false,
			       intern->num);
    if (eitvXXX_is_bottom(res->e->linterm[0]->eitv,intern->num)){
    _boxXXX_meet_lincons_array_bottom:
      boxXXX_set_bottom(res);
    }
    ap_linconsXXX_array_clear(tlincons);
  }
  return res;
}

boxXXX_t* boxXXX_meet_tcons_array(ap_manager_t* man,
				  bool destructive,
				  boxXXX_t* a,
				  ap_tcons0_array_t* array)
{
  boxXXX_t* res;
  size_t kmax;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;
  ap_linconsXXX_array_t tlincons;

  res = destructive ? a : boxXXX_copy(man,a);
  if (a->e->linterm==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_TCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;

    ap_linconsXXX_array_init(tlincons,0);
    ap_linconsXXX_array_intlinearize_tcons0_array(tlincons,array,
						  res->e,res->dim.intd,
						  intern->num);
    tbool_t tb = ap_linconsXXX_array_reduce_integer(tlincons,a->dim.intd,intern->num);
    if (tb==tbool_false){
      goto _boxXXX_meet_tcons_array_bottom;
    }
    ap_linconsXXX_array_boxize(res->e,NULL,
			       tlincons,res->e,a->dim.intd,kmax,false,
			       intern->num);
    if (eitvXXX_is_bottom(res->e->linterm[0]->eitv,intern->num)){
    _boxXXX_meet_tcons_array_bottom:
      boxXXX_set_bottom(res);
    }
    ap_linconsXXX_array_clear(tlincons);
  }
  return res;
}
