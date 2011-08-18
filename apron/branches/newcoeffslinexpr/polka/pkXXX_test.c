/* ********************************************************************** */
/* pkXXX_test.c: tests on polyhedra */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "ap_linconsXXX.h"
#include "ap_lingenXXX.h"
#include "ap_generic.h"

/* ====================================================================== */
/* Emptiness test */
/* ====================================================================== */

bool pkXXX_is_bottom(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_IS_BOTTOM);
  if (!po->C && !po->F){
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  if (po->F){
    man->result.flag_exact = man->result.flag_best =
      (po->dim.intd==0);
    return false;
  }
  else {
    if (pk->funopt->algorithm<0){
      man->result.flag_exact = man->result.flag_best = false;
      return (po->C ? false : true);
    }
    else {
      pkXXX_chernikova(man,po,NULL);
      if (pk->exn){
	man->result.flag_exact = man->result.flag_best = false;
	pk->exn = AP_EXC_NONE;
	return false;
      }
      man->result.flag_exact = man->result.flag_best =
	po->dim.intd>0 && po->F ? false : true;
      return (po->F == NULL);
    }
  }
}

/* ====================================================================== */
/* Universe test */
/* ====================================================================== */

bool pkXXX_is_top(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_IS_TOP);
  man->result.flag_exact = man->result.flag_best = true;

  if (pk->funopt->algorithm>=0)
    pkXXX_chernikova(man,po,NULL);
  if (!po->C && !po->F)
    return false;
  else if (po->C && po->F)
    return po->C->nbrows == pk->dec - 1;
  else {
    man->result.flag_exact = man->result.flag_best = false;
    return false;
  }
}

/* ====================================================================== */
/* Inclusion test */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Do generators satisfies a constraint ? */
/* ---------------------------------------------------------------------- */

/*
  F is suppposed to be a valid matrix of ray (i.e., corresponding
  to a non empty polyhedron.

  The epsilon component of the constraint is not taken into account.  The
  constraint is considered as strict only if the is_strict paramater telles so.
  This enables to test the satisfiability of a strict constraint in non-strict
  mode for the library.

*/
bool do_generators_sat_vectorXXX(pkXXX_internal_t* pk, matrixXXX_t* F, numintXXX_t* tab, bool is_strict)
{
  size_t i;

  if (numintXXX_sgn(tab[0])==0){
    /* 1. constraint is an equality */
    for (i=0; i<F->nbrows; i++){
      vectorXXX_product_strict(pk,pk->poly_prod,
			       F->p[i],
			       tab,F->nbcolumns);
      if (numintXXX_sgn(pk->poly_prod)) return false;
    }
    return true;
  }
  else {
    /* 2. constraint is an inequality */
    int sign;      /* sign of the scalar product */

    for (i=0; i<F->nbrows; i++){
      vectorXXX_product_strict(pk,pk->poly_prod,
			       F->p[i],
			       tab,F->nbcolumns);
      sign = numintXXX_sgn(pk->poly_prod);

      if (sign<0){
	return false;
      }
      else {
	if (numintXXX_sgn(F->p[i][0])==0){
	  /* line */
	  if (sign!=0) return false;
	}
	else {
	  /* ray or vertex */
	  if (is_strict && sign==0 &&
	      (pk->strict ? numintXXX_sgn(F->p[i][polka_eps])>0 : true))
	    return false;
	}
      }
    }
    return true;
  }
}

/* This test requires frames of pa and constraints of pb. The
   result is true if and only if all frames of pa verify the
   constraints of pb. We do not require minimality. */

bool pkXXX_is_leq(ap_manager_t* man, pkXXX_t* pa, pkXXX_t* pb)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_IS_LEQ);

  assert(pkXXX_check(pk,pa));
  assert(pkXXX_check(pk,pb));
  man->result.flag_exact = man->result.flag_best = false;
  if (pk->funopt->algorithm>0)
    pkXXX_chernikova(man,pa,"of the first argument");
  else
    pkXXX_obtain_F(man,pa,"of the first argument");

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!pa->F){ /* pa is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  if (pk->funopt->algorithm>0)
    pkXXX_chernikova(man,pb,"of the second argument");
  else
    pkXXX_obtain_C(man,pb,"of the second argument");

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!pb->C){/* pb is empty */
    man->result.flag_exact = man->result.flag_best = (pa->dim.intd==0);
    return false;
  }
  man->result.flag_exact = man->result.flag_best =
    pa->dim.intd==0;
  /* if both are mininmal, check the dimensions */
  if (pa->C && pa->F && pb->C && pb->F
      && (pa->nbeq < pb->nbeq || pa->nbline > pb->nbline))
    {
      man->result.flag_exact = man->result.flag_best = true;
      return false;
    }
  else {
    man->result.flag_exact = man->result.flag_best = (pa->dim.intd==0);
    /* does the frames of pa satisfy constraints of pb ? */
    size_t i;
    for (i=0; i<pb->C->nbrows; i++){
      bool sat = do_generators_sat_vectorXXX(pk,
					  pa->F,
					  pb->C->p[i],
					  pk->strict &&
					  numintXXX_sgn(pb->C->p[i][polka_eps])<0);
      if (sat==false) return false;
    }
    return true;
  }
}

/* ====================================================================== */
/* Equality test */
/* ====================================================================== */
bool pkXXX_is_eq(ap_manager_t* man, pkXXX_t* pa, pkXXX_t* pb)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_IS_EQ);

  man->result.flag_exact = man->result.flag_best =
    (pa->dim.intd==0);
  if (pa->C && pa->F && pb->C && pb->F &&
      (pa->nbeq != pb->nbeq || pa->nbline != pb->nbline) ){
    return false;
  }
  if (pk->funopt->algorithm>0){
    pkXXX_chernikova3(man,pa,"of the first argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      return false;
    }
    pkXXX_chernikova3(man,pb,"of the first argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      return false;
    }
  }
  man->result.flag_exact = man->result.flag_best = true;
  if (pkXXX_is_canonical(man,pa) && pkXXX_is_canonical(man,pb)){
    bool res =
      (!pa->C && !pb->C) ||
      (pa->C && pb->C &&
       pa->C->nbrows == pb->C->nbrows && pa->F->nbrows == pb->F->nbrows &&
       (pa->C->nbrows <= pa->F->nbrows ? matrixXXX_equal(pa->C,pb->C) : matrixXXX_equal(pa->F,pb->F)));
    assert(res == (pkXXX_is_leq(man,pa,pb) && pkXXX_is_leq(man,pb,pa)));
    return res;
  }
  else {
    bool res = pkXXX_is_leq(man,pa,pb) && pkXXX_is_leq(man,pb,pa);
    return res;
  }
}

/* ====================================================================== */
/* Satisfiability of a linear constraint */
/* ====================================================================== */

bool pkXXX_sat_lincons_linear(ap_manager_t* man, pkXXX_t* po, ap_lincons0_t lincons0)
{
  bool sat;
  size_t dim;
  ap_constyp_t constyp;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_SAT_LINCONS);

  if (pk->funopt->algorithm>0)
    pkXXX_chernikova(man,po,NULL);
  else
    pkXXX_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  constyp = ap_lincons0_get_constyp(lincons0);
  switch (constyp){
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    break;
  default:
    man->result.flag_exact = man->result.flag_best = false;
    return false;
  }
  dim = po->dim.intd + po->dim.reald;
  ap_linconsMPQ_set_lincons0(pk->ap_linconsMPQ,lincons0,pk->num);
  if (pk->ap_linconsMPQ->linexpr->effsize==0){
    sat = (ap_linconsMPQ_evalcst(pk->ap_linconsMPQ,pk->num)==tbool_true);
    man->result.flag_exact = man->result.flag_best = true;
  }
  else {
    sat = vectorXXX_set_linconsMPQ_sat(
	pk, pk->numintXXXp, pk->ap_linconsMPQ, po->dim, true);
    if (sat){
      sat = do_generators_sat_vectorXXX(pk,po->F,
					pk->numintXXXp,
					pk->ap_linconsMPQ->constyp==AP_CONS_SUP);
    }
    man->result.flag_exact = man->result.flag_best =
      sat ?
      true :
      (
	  ( (pk->funopt->flag_exact_wanted || pk->funopt->flag_best_wanted) &&
	    ap_linconsMPQ_is_real(pk->ap_linconsMPQ,po->dim.intd) ) ?
	  true :
	  false );
  }
  return sat;
}

bool pkXXX_sat_lincons(ap_manager_t* man, pkXXX_t* po, ap_lincons0_t lincons0)
{
  if (ap_lincons0_is_linear(lincons0))
    return pkXXX_sat_lincons_linear(man,po,lincons0);
  else
    return ap_generic_sat_lincons(man,po,lincons0,AP_SCALAR_MPQ,AP_LINEXPR_LINEAR,AP_LINEXPR_INTLINEAR);
}
bool pkXXX_sat_tcons(ap_manager_t* man, pkXXX_t* po, ap_tcons0_t* cons)
{
  return ap_generic_sat_tcons(man,po,cons,AP_SCALAR_MPQ,AP_LINEXPR_LINEAR,AP_LINEXPR_INTLINEAR);
}

/* ====================================================================== */
/* Inclusion of a dimension in an interval */
/* ====================================================================== */
/* ---------------------------------------------------------------------- */
/* Do generators satisfies a bound for one dimension ? */
/* ---------------------------------------------------------------------- */

/*
  F is suppposed to be a valid matrix of ray (i.e., corresponding
  to a non empty polyhedron.

  Assume coeff is not an infinite number.

  tests if:
  - dim <= bound if sgn>0
  - dim = bound if sgn=0
  - dim >= -bound if sgn<0
*/

bool do_generators_sat_boundXXX(
    pkXXX_internal_t* pk, matrixXXX_t* F,
    ap_dim_t dim, numXXX_t bound,
    int sgn
)
{
  size_t i,index;
  int sgn2;

  index  = pk->dec + dim;
  for (i=0; i<F->nbrows; i++){
    sgn2 = numintXXX_sgn(F->p[i][index]);
    if (numintXXX_sgn(F->p[i][0])==0){
      /* line */
      if (sgn2) return false;
    }
    else if (numintXXX_sgn(F->p[i][polka_cst])==0){
      /* ray */
      if ( (sgn>=0 && sgn2>0) || (sgn<=0 && sgn2<0) )
	return false;
    }
    else {
      /* vertex */
      numXXX_set_numintXXX2(pk->numratXXX,
			    F->p[i][index],
			    F->p[i][polka_cst]);
      if (sgn==0){
	if (!numXXX_equal(pk->numratXXX,bound))
	  return false;
      }
      else {
	if (sgn<0) numXXX_neg(pk->numratXXX,pk->numratXXX);
	sgn2 = numXXX_cmp(pk->numratXXX,bound);
	if (sgn2>0)
	  return false;
      }
    }
  }
  return true;
}

bool pkXXX_sat_interval(ap_manager_t* man, pkXXX_t* po,
			ap_dim_t dim, ap_coeff_t interval)
{
  bool sat;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_SAT_INTERVAL);

  if (pk->funopt->algorithm>0)
    pkXXX_chernikova(man,po,NULL);
  else
    pkXXX_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  eitvXXX_set_ap_coeff(pk->eitvXXX, interval, pk->num);
  if (eitvXXX_is_point(pk->eitvXXX)){
    /* interval is a point */
    sat = do_generators_sat_boundXXX(pk,po->F,dim,pk->eitvXXX->itv->sup,0);
  }
  else {
    sat = true;
    /* inferior bound */
    if (!boundXXX_infty(pk->eitvXXX->itv->neginf)){
      sat = do_generators_sat_boundXXX(pk,po->F,dim,pk->eitvXXX->itv->neginf,-1);
      if (!sat) goto poly_sat_interval_exit0;
    }
    /* superior bound */
    if (!boundXXX_infty(pk->eitvXXX->itv->sup)){
      sat = do_generators_sat_boundXXX(pk,po->F,dim,pk->eitvXXX->itv->sup,1);
    }
  }
 poly_sat_interval_exit0:
  man->result.flag_exact = man->result.flag_best =
    sat ? true :
    (dim < po->dim.intd ? false : true);
  return sat;
}

/* ====================================================================== */
/* Is a dimension unconstrained ? */
/* ====================================================================== */

bool pkXXX_is_dimension_unconstrained(ap_manager_t* man, pkXXX_t* po,
				      ap_dim_t dim)
{
  size_t i,j;
  bool res;
  matrixXXX_t* F;
  matrixXXX_t* C;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_SAT_INTERVAL);

  pkXXX_chernikova3(man,po,NULL);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return false;
  }
  /* We test if there exists the line of direction dim */
  F = po->F;
  res = false;
  for (i=0; i<po->nbline; i++){
    if (numintXXX_sgn(F->p[i][pk->dec+dim])){
      res = true;
      for(j=pk->dec; j<F->nbcolumns; j++){
	if (j!=pk->dec+dim && numintXXX_sgn(F->p[i][j])){
	  res = false;
	  break;
	}
      }
      break;
    }
  }
  man->result.flag_exact = man->result.flag_best = true;
  return res;
}
