/* ********************************************************************** */
/* box_internal.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"
#include "box_internal.h"
#include "box_representation.h"
#include "box_policy.h"

/* ============================================================ */
/* Policy */
/* ============================================================ */

ap_policy_manager_t* box_policy_manager_alloc(ap_manager_t* man)
{
  ap_policy_manager_t* pman;
  void** funptr;

  if (strcmp(man->library,"box")!=0){
    char str[160];
    snprintf(str,159,"\
box_policy_manager_alloc: the standard manager given in argument is not a box manager, but a %s manager",
	     man->library);
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_UNKNOWN,
			       str);
    return NULL;
  }
  pman = ap_policy_manager_alloc(man,NULL,NULL);
  funptr = pman->funptr;
  funptr[AP_FUNPOLICYID_UNKNOWN] = NULL;
  funptr[AP_FUNPOLICYID_ALLOC] = box_policy_alloc;
  funptr[AP_FUNPOLICYID_COPY] = box_policy_copy;
  funptr[AP_FUNPOLICYID_FREE] = box_policy_free;
  funptr[AP_FUNPOLICYID_FPRINT] = box_policy_fprint;
  funptr[AP_FUNPOLICYID_EQUAL] = box_policy_equal;
  funptr[AP_FUNPOLICYID_MEET] = box_policy_meet;
  funptr[AP_FUNPOLICYID_MEET_ARRAY] = box_policy_meet_array;
  funptr[AP_FUNPOLICYID_MEET_LINCONS_ARRAY] = box_policy_meet_lincons_array;
  funptr[AP_FUNPOLICYID_MEET_TCONS_ARRAY] = box_policy_meet_tcons_array;
  return pman;
}

void box_policy_resize(box_policy_t* boxpolicy, size_t nbdims, size_t size)
{
  size_t i,j;

  if (boxpolicy->size==size)
    return;
  else if (boxpolicy->size > size || boxpolicy->first==false){
    fprintf(stderr,"\nFunction %s in %s at line %i\n",__func__,__FILE__,__LINE__);
    abort();
  }
  boxpolicy->p = (box_policy_one_t*)realloc(boxpolicy->p,size*sizeof(box_policy_one_t));
  for (i=boxpolicy->size; i<size; i++){
    boxpolicy->p[i].p = (box_policy_dim_t*)malloc(nbdims*sizeof(box_policy_dim_t));
    boxpolicy->p[i].nbdims = nbdims;
    for (j=0; j<nbdims; j++){
      boxpolicy->p[i].p[j].inf = BOX_POLICY_1;
      boxpolicy->p[i].p[j].sup = BOX_POLICY_1;
    }
  }
  boxpolicy->size = size;
  boxpolicy->first = false;
}
ap_policy_t* box_policy_alloc(ap_policy_manager_t* man, ap_funid_t funid, size_t nbdims)
{
  ap_policy_t* policy;
  box_policy_t* boxpolicy = (box_policy_t*)malloc(sizeof(box_policy_t));
  boxpolicy->p = NULL;
  boxpolicy->size = 0;
  boxpolicy->first = true;
  box_policy_resize(boxpolicy,nbdims,1);
  boxpolicy->first = true;
  policy = (ap_policy_t*)malloc(sizeof(ap_policy_t));
  policy->value = (void*)boxpolicy;
  policy->man = ap_policy_manager_copy(man);
  return policy;
}
void box_policy_free(ap_policy_manager_t* man, ap_policy_t* policy)
{
  size_t i;
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;
  for (i=0; i<boxpolicy->size; i++){
    free(boxpolicy->p[i].p);
    boxpolicy->p[i].p = NULL;
  }
  free(boxpolicy->p); boxpolicy->p = NULL;
  free(boxpolicy); policy->value=NULL;
  ap_policy_manager_free(policy->man); policy->man = NULL;
  free(policy);
}
ap_policy_t* box_policy_copy(ap_policy_manager_t* man, ap_policy_t* policy)
{
  size_t i,j,nbdims;
  ap_policy_t* npolicy;
  box_policy_t* nboxpolicy = (box_policy_t*)malloc(sizeof(box_policy_t));
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;
  nboxpolicy->p = NULL;
  nboxpolicy->size = 0;
  nboxpolicy->first = true;
  if (boxpolicy->size>0){
    nbdims = boxpolicy->p[0].nbdims;
    box_policy_resize(nboxpolicy,nbdims,boxpolicy->size);
    nboxpolicy->first = true;
    for (i=0; i<nboxpolicy->size; i++){
      for (j=0; j<nbdims; j++){
	nboxpolicy->p[i].p[j] = boxpolicy->p[i].p[j];
      }
    }
  }
  npolicy = (ap_policy_t*)malloc(sizeof(ap_policy_t));
  npolicy->value = (void*)nboxpolicy;
  npolicy->man = ap_policy_manager_copy(policy->man);
  return npolicy;
}

static inline
void box_policy_choice_fprint(FILE* stdout, box_policy_choice_t choice)
{
  switch(choice){
  case BOX_POLICY_1:
    fputc('l', stdout);
    break;
  case BOX_POLICY_2:
    fputc('r', stdout);
    break;
  default:
    abort();
  }
}
static inline
void box_policy_dim_fprint(FILE* stdout, box_policy_dim_t* pdim)
{
  box_policy_choice_fprint(stdout,pdim->inf);
  box_policy_choice_fprint(stdout,pdim->sup);
}
void box_policy_one_fprint(FILE* stdout, box_policy_one_t* policy)
{
  for (size_t j=0; j < policy->nbdims; j++){
    box_policy_dim_fprint(stdout,&policy->p[j]);
    fputc(' ', stdout);
  }
}
void box_policy_fprint(FILE* stdout, ap_policy_manager_t* man, ap_policy_t* policy)
{
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;
  for (size_t i=0; i < boxpolicy->size; i++){
    fprintf(stdout, "%zi=", i);
    box_policy_one_fprint(stdout,&boxpolicy->p[i]);
    fputc('\n', stdout);
  }
}

static inline
bool box_policy_dim_equal(box_policy_dim_t* policy1, box_policy_dim_t* policy2)
{ return policy1->inf==policy2->inf && policy1->sup==policy2->sup; }

bool box_policy_one_equal(box_policy_one_t* policy1, box_policy_one_t* policy2)
{
  size_t i;
  bool res;

  res = (policy1->nbdims==policy2->nbdims);
  if (!res) return res;
  for (i=0; i<policy1->nbdims; i++){
    res = box_policy_dim_equal(&policy1->p[i],&policy2->p[i]);
    if (!res) return res;
  }
  return res;
} 

bool box_policy_equal(ap_policy_manager_t* man, ap_policy_t* policy1, ap_policy_t* policy2)
{
  if (policy1==policy2) 
    return true;
  if (policy1->man->man!=policy2->man->man &&
      strcmp(policy1->man->man->library,policy2->man->man->library)!=0)
    return false;
  
  box_policy_t* boxpolicy1 = (box_policy_t*)policy1->value;
  box_policy_t* boxpolicy2 = (box_policy_t*)policy2->value;
  bool res;
  size_t i;

  res = (boxpolicy1->size == boxpolicy2->size);
  if (!res) return res;
  for (i=0; i<boxpolicy1->size; i++){
    res = box_policy_one_equal(&boxpolicy1->p[i],&boxpolicy2->p[i]);
    if (!res) return res;
  }
  return res;
}

/* ============================================================ */
/* Meet */
/* ============================================================ */

static inline
void bound_policy_meet(ap_policy_mode_t mode,
		       box_policy_choice_t* choice,
		       bound_t a, bound_t b, bound_t c)
{
  switch (mode){
  case AP_POLICY_APPLY:
    switch (*choice){
    case BOX_POLICY_1:
      bound_set(a, b);
      break;
    case BOX_POLICY_2:
      bound_set(a, c);
      break;
    default:
      abort();
    }
    break;
  case AP_POLICY_CHANGE:
    {
      int cmp = bound_cmp(b,c);
      *choice = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
    }
    break;
  default:
    abort();
  }
}

static inline
bool itv_policy_meet(itv_internal_t* intern,
		     ap_policy_mode_t mode,
		     box_policy_dim_t* policy,
		     itv_t a, itv_t b, itv_t c)
{
  bool exc;
  bound_policy_meet(AP_POLICY_APPLY,&policy->inf,a->inf,b->inf,c->inf);
  bound_policy_meet(AP_POLICY_APPLY,&policy->sup,a->sup,b->sup,c->sup);
  exc = itv_canonicalize(intern,a,false);
  if (mode==AP_POLICY_CHANGE){
    bound_policy_meet(mode,&policy->inf,a->inf,b->inf,c->inf);
    bound_policy_meet(mode,&policy->sup,a->sup,b->sup,c->sup);
  }
  return exc;
}
box_t* box_policy_meet_internal(ap_manager_t* man,
				box_policy_one_t* policy,
				ap_policy_mode_t mode,
				bool destructive, box_t* a1, box_t* a2)
{
  size_t i;
  bool exc = false;
  size_t nbdims;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = false;
  man->result.flag_exact = false;
  res = destructive ? a1 : (box_t*) box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL || a2->p==NULL){
    box_set_bottom(res);
    return res;
  }
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a1->realdim;

  if (policy->nbdims != nbdims) abort();
  for (i=0; i<nbdims; i++) {
    exc = itv_policy_meet(intern->itv,
			  mode,&policy->p[i],
			  res->p[i], a1->p[i], a2->p[i]);
    if (exc){
      box_set_bottom(res);
      break;
    }
  }
  return res;
}
box_t* box_policy_meet(ap_policy_manager_t* pman,
		       ap_policy_t* policy, ap_policy_mode_t mode,
		       bool destructive, box_t* a1, box_t* a2)
{
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;
  return box_policy_meet_internal(pman->man,
				  &boxpolicy->p[0],mode,
				  destructive,a1,a2);
}
box_t* box_policy_meet_array(ap_policy_manager_t* pman, 
			     ap_policy_t* policy, ap_policy_mode_t mode,
			     box_t** tab, size_t size)
{
  size_t i;
  box_t* res;
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;

  pman->man->result.flag_best = false;
  pman->man->result.flag_exact = false;
  if (size==1)
    return box_copy(pman->man,tab[0]);
  else if (size==2)
    return box_policy_meet(pman,policy,mode,false,tab[0],tab[1]);

  box_policy_resize(boxpolicy,boxpolicy->p[0].nbdims,size-1);
  res = box_copy(pman->man,tab[0]);
  for (i=1;i<size;i++){
    box_policy_meet_internal(pman->man,&boxpolicy->p[i-1],mode,true,res,tab[i]);
  }
  return res;
}

/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

/* Meet of an abstract value with a constraint */
void box_policy_meet_lincons_internal(box_internal_t* intern,
				      box_policy_one_t* boxpolicy,
				      ap_policy_mode_t mode,
				      box_t* a,
				      itv_lincons_t* cons)
{
  size_t nbcoeffs,nbdims;
  ap_dim_t dim = AP_DIM_MAX;
  int previous_dim;
  size_t i;
  itv_ptr pitv;
  itv_linexpr_t* expr;
  bool *peq;
  bool equality;
  bool exc;
  int cmp;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (a->p==NULL){
    box_set_bottom(a);
  }
  nbdims = a->intdim + a->realdim;
  expr = &cons->linexpr;

  /* Iterates on coefficients */
  nbcoeffs = 0;
  itv_set_int(intern->meet_lincons_internal_itv2,0);
  previous_dim = -1;
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    {
      int j;
      for(j=previous_dim+1; j<(int)dim; j++){
	if (boxpolicy->p[j].inf == BOX_POLICY_2){
	  bound_set_infty(a->p[dim]->inf,1);
	  if (mode == AP_POLICY_CHANGE)
	    boxpolicy->p[j].inf = BOX_POLICY_1;
	}
	if (boxpolicy->p[j].sup == BOX_POLICY_2){
	  bound_set_infty(a->p[dim]->sup,1);
	  if (mode == AP_POLICY_CHANGE)
	    boxpolicy->p[j].sup = BOX_POLICY_1;
	}
      }
      previous_dim = (int)dim;
    }
    nbcoeffs++;
    box_policy_dim_t newpolicy_dim = boxpolicy->p[dim];

    /* 1. sign of a in the expression e = ax+e' */
    int sgncoeff =
      bound_sgn(pitv->inf)<0 ?
      1 :
      ( bound_sgn(pitv->sup)<0 ?
	-1 :
	0 );
    if (sgncoeff==0){
      if (boxpolicy->p[dim].inf == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->inf,1);
	if (mode == AP_POLICY_CHANGE)
	  newpolicy_dim.inf = BOX_POLICY_1;
      }
      if (boxpolicy->p[dim].sup == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->sup,1);
	if (mode == AP_POLICY_CHANGE)
	  newpolicy_dim.sup = BOX_POLICY_1;
      }
    } else { /* 2. We decompose the expression e = ax+e' */
      /* We save the linterm */
      itv_swap(intern->meet_lincons_internal_itv2,pitv);
      equality = *peq;
      *peq = true;
      /* 2. We evaluate e' */
      itv_eval_linexpr(intern->itv,
		       intern->meet_lincons_internal_itv3,expr,a->p);
      if (equality){
	/*
	  If we have ax+e' >= 0 with a>0
	  we can deduce that x>=-e'/a, or x>= inf(-e'/a)
	  If we have ax+e' >= 0 with a<0
	  we can deduce that -ax<=e', or x<= sup(e'/-a)
	  If we have ax+e'=0
	  we can deduce x=-e'/a, or inf(-e'/a)<= x <= sup(-e'/a)
	*/
	if ((mode == AP_POLICY_CHANGE || boxpolicy->p[dim].inf == BOX_POLICY_2)
	    &&
	    (sgncoeff>0 || cons->constyp == AP_CONS_EQ)){
	  /*
	    If we have a>0, we compute sup(e')/a=sup(e'/a)=-inf(-e'/a)
	    If we have a<0, we compute -inf(e')/(-a)=-inf(-e'/a)
	  */
	  if (sgncoeff>0){
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->sup,
		      intern->meet_lincons_internal_itv2->sup);
	  }
	  else {
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->inf,
		      intern->meet_lincons_internal_itv2->inf);
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    newpolicy_dim.inf = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (boxpolicy->p[dim].inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if ((mode == AP_POLICY_CHANGE || boxpolicy->p[dim].sup == BOX_POLICY_2) &&
	    (sgncoeff<0 || cons->constyp == AP_CONS_EQ)){
	  /*
	    If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	    If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	  */
	  if (sgncoeff<0){
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->sup,
		      intern->meet_lincons_internal_itv2->inf);
	  }
	  else {
	    bound_div(intern->meet_lincons_internal_bound,
		      intern->meet_lincons_internal_itv3->inf,
		      intern->meet_lincons_internal_itv2->sup);
	  }
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    newpolicy_dim.sup = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (boxpolicy->p[dim].sup == BOX_POLICY_2){
	    bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	  }
	}
      }
      else { /* We have an interval */
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
	int sgninf = bound_sgn(intern->meet_lincons_internal_itv3->inf);
	int sgnsup = bound_sgn(intern->meet_lincons_internal_itv3->sup);
	if (boxpolicy->p[dim].inf == BOX_POLICY_2 &&
	    (sgncoeff>0 || (cons->constyp==AP_CONS_EQ && sgncoeff<0))){
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
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    newpolicy_dim.inf = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (boxpolicy->p[dim].inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if (boxpolicy->p[dim].sup == BOX_POLICY_2 &&
	    (sgncoeff<0 || (cons->constyp==AP_CONS_EQ && sgncoeff>0))){
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
	  if (mode == AP_POLICY_CHANGE){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    newpolicy_dim.sup = cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
	  }
	  /* We update the interval */
	  if (boxpolicy->p[dim].sup == BOX_POLICY_2){
	    bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	  }
	}
      }
      if (mode == AP_POLICY_CHANGE){
	boxpolicy->p[dim] = newpolicy_dim;
      }
      itv_swap(intern->meet_lincons_internal_itv2,pitv);
      *peq = equality;
    }
    exc = itv_canonicalize(intern->itv,a->p[dim],dim<a->intdim);
    if (exc){
      box_set_bottom(a);
      return;
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
    }
    else {
      if (boxpolicy->p[dim].inf == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->inf,1);
	if (mode == AP_POLICY_CHANGE)
	  boxpolicy->p[dim].inf = BOX_POLICY_1;
      }
      if (boxpolicy->p[dim].sup == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->sup,1);
	if (mode == AP_POLICY_CHANGE)
	  boxpolicy->p[dim].sup = BOX_POLICY_1;
      }
    }
  }
}

box_t* box_policy_meet_lincons_array(ap_policy_manager_t* pman, 
				     ap_policy_t* policy, ap_policy_mode_t mode,
				     bool destructive,
				     box_t* a,
				     ap_lincons0_array_t* array)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;

  man->result.flag_best = false;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p!=NULL){
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    box_policy_resize(boxpolicy,boxpolicy->p[0].nbdims,kmax*array->size);

    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern->itv,&tlincons,array);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      box_set_bottom(res);
    }
    else {
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
	  box_policy_meet_lincons_internal(intern,
					   &boxpolicy->p[k*array->size+i], mode,
					   res,
					   &tlincons.p[i]);
	  if (box_is_bottom(man, res))
	    goto box_meet_lincons_array_policy_exit;
	}
      }
    }
  box_meet_lincons_array_policy_exit:
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}

box_t* box_policy_meet_tcons_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				   bool destructive,
				   box_t* a,
				   ap_tcons0_array_t* array)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;
  box_policy_t* boxpolicy = (box_policy_t*)policy->value;

  man->result.flag_best = false;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p!=NULL){
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    box_policy_resize(boxpolicy,boxpolicy->p[0].nbdims,kmax*array->size);

    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern->itv,&tlincons,
				     array,res->p,res->intdim);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      box_set_bottom(res);
    }
    else {
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
	  box_policy_meet_lincons_internal(intern,
					   &boxpolicy->p[k*array->size+i], mode,
					   res,
					   &tlincons.p[i]);
	  if (box_is_bottom(man, res))
	    goto box_meet_tcons_array_policy_exit;
	}
      }
    }
  box_meet_tcons_array_policy_exit:
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}

/*

box_t*
box_assign_linexpr_array_policy(ap_manager_t* man,
				ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				bool destructive,
				box_t* a, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size, box_t* dest)
{
  return box_assign_linexpr_array(man, destructive, a, tdim, texpr, size, dest);

}
box_t*
box_assign_texpr_array_policy(ap_manager_t* man,
			      ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
			      bool destructive,
			      box_t* a, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size, box_t* dest)
{
  return box_assign_texpr_array(man, destructive, a, tdim, texpr, size, dest);

}
*/
