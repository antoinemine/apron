/* ********************************************************************** */
/* pk_test.c: tests on polyhedra */
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
#include "pk_representation.h"
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_extract.h"
#include "pk_test.h"
#include "itv_linearize.h"

/* ====================================================================== */
/* Emptiness test */
/* ====================================================================== */

bool pk_is_bottom(ap_manager_t* man, pk_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_IS_BOTTOM);
  if (!po->C && !po->F){
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  if (po->F){
      man->result.flag_exact = man->result.flag_best =
	(po->intdim==0);
      return false;
  }
  else {
    if (pk->funopt->algorithm<0){
      man->result.flag_exact = man->result.flag_best = false;
      return (po->C ? false : true);
    }
    else {
      poly_chernikova(man,po,NULL);
      if (pk->exn){
	man->result.flag_exact = man->result.flag_best = false;
	pk->exn = AP_EXC_NONE;
	return false;
      }
      man->result.flag_exact = man->result.flag_best =
	po->intdim>0 && po->F ? false : true;
      return (po->F == NULL);
    }
  }
}

/* ====================================================================== */
/* Universe test */
/* ====================================================================== */

bool pk_is_top(ap_manager_t* man, pk_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_IS_TOP);
  man->result.flag_exact = man->result.flag_best = true;

  if (pk->funopt->algorithm>=0)
    poly_chernikova(man,po,NULL);
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
bool do_generators_sat_vector(pk_internal_t* pk, pk_matrix_t* F, numint_t* tab, bool is_strict)
{
  size_t i;

  if (numint_sgn(tab[0])==0){
    /* 1. constraint is an equality */
    for (i=0; i<F->nbrows; i++){
      vector_product_strict(pk,pk->poly_prod,
			    F->p[i],
			    tab,F->nbcolumns);
      if (numint_sgn(pk->poly_prod)) return false;
    }
    return true;
  }
  else {
    /* 2. constraint is an inequality */
    int sign;      /* sign of the scalar product */

    for (i=0; i<F->nbrows; i++){
      vector_product_strict(pk,pk->poly_prod,
			    F->p[i],
			    tab,F->nbcolumns);
      sign = numint_sgn(pk->poly_prod);

      if (sign<0){
	return false;
      }
      else {
	if (numint_sgn(F->p[i][0])==0){
	  /* line */
	  if (sign!=0) return false;
	}
	else {
	  /* ray or vertex */
	  if (is_strict && sign==0 &&
	      (pk->strict ? numint_sgn(F->p[i][polka_eps])>0 : true))
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

bool pk_is_leq(ap_manager_t* man, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_IS_LEQ);

  assert(poly_check(pk,pa));
  assert(poly_check(pk,pb));
  man->result.flag_exact = man->result.flag_best = false;
  if (pk->funopt->algorithm>0)
    poly_chernikova(man,pa,"of the first argument");
  else
    poly_obtain_F(man,pa,"of the first argument");

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!pa->F){ /* pa is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  if (pk->funopt->algorithm>0)
    poly_chernikova(man,pb,"of the second argument");
  else
    poly_obtain_C(man,pb,"of the second argument");

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!pb->C){/* pb is empty */
    man->result.flag_exact = man->result.flag_best = (pa->intdim==0);
    return false;
  }
  man->result.flag_exact = man->result.flag_best =
    pa->intdim==0;
  /* if both are mininmal, check the dimensions */
  if (pa->C && pa->F && pb->C && pb->F
      && (pa->nbeq < pb->nbeq || pa->nbline > pb->nbline))
    {
     man->result.flag_exact = man->result.flag_best = true;
     return false;
    }
  else {
    man->result.flag_exact = man->result.flag_best = (pa->intdim==0);
    /* does the frames of pa satisfy constraints of pb ? */
    size_t i;
    for (i=0; i<pb->C->nbrows; i++){
      bool sat = do_generators_sat_vector(pk,
					  pa->F,
					  pb->C->p[i],
					  pk->strict &&
					  numint_sgn(pb->C->p[i][polka_eps])<0);
      if (sat==false) return false;
    }
    return true;
  }
}

/* ====================================================================== */
/* Equality test */
/* ====================================================================== */
bool pk_is_eq(ap_manager_t* man, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_IS_EQ);

  man->result.flag_exact = man->result.flag_best =
    (pa->intdim==0);
  if (pa->C && pa->F && pb->C && pb->F &&
      (pa->nbeq != pb->nbeq || pa->nbline != pb->nbline) ){
    return false;
  }
  if (pk->funopt->algorithm>0){
    poly_chernikova3(man,pa,"of the first argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      return false;
    }
    poly_chernikova3(man,pb,"of the first argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      return false;
    }
  }
  man->result.flag_exact = man->result.flag_best = true;
  if (pk_is_canonical(man,pa) && pk_is_canonical(man,pb)){
    bool res =
      (!pa->C && !pb->C) ||
      (pa->C && pb->C &&
       pa->C->nbrows == pb->C->nbrows && pa->F->nbrows == pb->F->nbrows &&
       (pa->C->nbrows <= pa->F->nbrows ? pk_matrix_equal(pa->C,pb->C) : pk_matrix_equal(pa->F,pb->F)));
    assert(res == (pk_is_leq(man,pa,pb) && pk_is_leq(man,pb,pa)));
    return res;
  }
  else {
    bool res = pk_is_leq(man,pa,pb) && pk_is_leq(man,pb,pa);
    return res;
  }
}

/* ====================================================================== */
/* Satisfiability of a linear constraint */
/* ====================================================================== */

bool pk_sat_lincons(ap_manager_t* man, pk_t* po, ap_lincons0_t* lincons0)
{
  bool exact;
  bool sat;
  size_t dim;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SAT_LINCONS);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  switch (lincons0->constyp){
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    break;
  default:
    man->result.flag_exact = man->result.flag_best = false;
    return false;
  }
  dim = po->intdim + po->realdim;

  if (!ap_linexpr0_is_quasilinear(lincons0->linexpr0)){
    itv_t* env = pk_matrix_to_box(pk,po->F);
    exact = itv_lincons_set_ap_lincons0(pk->itv,
					&pk->poly_itv_lincons,
					lincons0);
    exact = itv_quasilinearize_lincons(pk->itv,
				       &pk->poly_itv_lincons,
				       env,
				       false)
      && exact;
    itv_array_free(env,dim);
  }
  else {
    exact = itv_lincons_set_ap_lincons0(pk->itv,
					&pk->poly_itv_lincons,
					lincons0);
  }
  sat = vector_set_itv_lincons_sat(pk,
				   pk->poly_numintp,
				   &pk->poly_itv_lincons,
				   po->intdim, po->realdim, true);
  if (sat){
    sat = do_generators_sat_vector(pk,po->F,
				   pk->poly_numintp,
				   lincons0->constyp==AP_CONS_SUP);
  }
  man->result.flag_exact = man->result.flag_best =
    sat ?
    true :
    (
     ( (pk->funopt->flag_exact_wanted || pk->funopt->flag_best_wanted) &&
       exact && ap_linexpr0_is_real(lincons0->linexpr0,po->intdim) ) ?
     true :
     false );

  return sat;
}

bool pk_sat_tcons(ap_manager_t* man, pk_t* po, ap_tcons0_t* cons)
{
  size_t dim;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SAT_LINCONS);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  switch (cons->constyp){
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    break;
  default:
    man->result.flag_exact = man->result.flag_best = false;
    return false;
  }
  dim = po->intdim + po->realdim;

  itv_t* env = pk_matrix_to_box(pk,po->F);
  itv_intlinearize_ap_tcons0(pk->itv,&pk->poly_itv_lincons,
			     cons,env,po->intdim);
  itv_quasilinearize_lincons(pk->itv,&pk->poly_itv_lincons,env,false);
  itv_array_free(env,po->intdim+po->realdim);
  bool sat = vector_set_itv_lincons_sat(pk,
					pk->poly_numintp,
					&pk->poly_itv_lincons,
					po->intdim, po->realdim, true);
  if (sat){
    sat = do_generators_sat_vector(pk,po->F,
				   pk->poly_numintp,
				   cons->constyp==AP_CONS_SUP);
  }
  man->result.flag_exact = man->result.flag_best = sat;
  return sat;
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

bool do_generators_sat_bound(pk_internal_t* pk, pk_matrix_t* F,
			     ap_dim_t dim, numrat_t bound,
			     int sgn)
{
  size_t i,index;
  int sgn2;

  index  = pk->dec + dim;
  for (i=0; i<F->nbrows; i++){
    sgn2 = numint_sgn(F->p[i][index]);
    if (numint_sgn(F->p[i][0])==0){
      /* line */
      if (sgn2) return false;
    }
    else if (numint_sgn(F->p[i][polka_cst])==0){
      /* ray */
      if ( (sgn>=0 && sgn2>0) || (sgn<=0 && sgn2<0) )
	return false;
    }
    else {
      /* vertex */
      numrat_set_numint2(pk->poly_numrat,
			 F->p[i][index],
			 F->p[i][polka_cst]);
      if (sgn==0){
	if (!numrat_equal(pk->poly_numrat,bound))
	  return false;
      }
      else {
	if (sgn<0) numrat_neg(pk->poly_numrat,pk->poly_numrat);
	sgn2 = numrat_cmp(pk->poly_numrat,bound);
	if (sgn2>0)
	  return false;
      }
    }
  }
  return true;
}

bool pk_sat_interval(ap_manager_t* man, pk_t* po,
			ap_dim_t dim, ap_interval_t* interval)
{
  bool sat;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SAT_INTERVAL);

  if (pk->funopt->algorithm>0)
    poly_chernikova(man,po,NULL);
  else
    poly_obtain_F(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    return false;
  }
  if (!po->F){ /* po is empty */
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  itv_set_ap_interval(pk->itv,
		      pk->poly_itv, interval);
  if (itv_is_point(pk->itv, pk->poly_itv)){
    /* interval is a point */
    sat = do_generators_sat_bound(pk,po->F,dim,pk->poly_itv->sup,0);
  }
  else {
    sat = true;
    /* inferior bound */
    if (!bound_infty(pk->poly_itv->inf)){
      sat = do_generators_sat_bound(pk,po->F,dim,pk->poly_itv->inf,-1);
      if (!sat) goto poly_sat_interval_exit0;
    }
    /* superior bound */
    if (!bound_infty(pk->poly_itv->sup)){
      sat = do_generators_sat_bound(pk,po->F,dim,pk->poly_itv->sup,1);
    }
  }
 poly_sat_interval_exit0:
  man->result.flag_exact = man->result.flag_best =
    sat ? true :
    (dim < po->intdim ? false : true);
  return sat;
}

/* ====================================================================== */
/* Is a dimension unconstrained ? */
/* ====================================================================== */

bool pk_is_dimension_unconstrained(ap_manager_t* man, pk_t* po,
					ap_dim_t dim)
{
  size_t i,j;
  bool res;
  pk_matrix_t* F;
  pk_matrix_t* C;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SAT_INTERVAL);

  poly_chernikova3(man,po,NULL);
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
    if (numint_sgn(F->p[i][pk->dec+dim])){
      res = true;
      for(j=pk->dec; j<F->nbcolumns; j++){
	if (j!=pk->dec+dim && numint_sgn(F->p[i][j])){
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
