/* ********************************************************************** */
/* box_internal.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stddef.h>

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
  funptr[AP_FUNPOLICYID_COPY] = box_policy_copy;
  funptr[AP_FUNPOLICYID_FREE] = box_policy_free;
  funptr[AP_FUNPOLICYID_FPRINT] = box_policy_fprint;
  funptr[AP_FUNPOLICYID_SPRINT] = box_policy_sprint;
  funptr[AP_FUNPOLICYID_DIMENSION] = box_policy_dimension;
  funptr[AP_FUNPOLICYID_EQUAL] = box_policy_equal;
  funptr[AP_FUNPOLICYID_HASH] = box_policy_hash;
  funptr[AP_FUNPOLICYID_MEET_APPLY] = box_policy_meet_apply;
  funptr[AP_FUNPOLICYID_MEET_ARRAY_APPLY] = box_policy_meet_array_apply;
  funptr[AP_FUNPOLICYID_MEET_LINCONS_ARRAY_APPLY] = box_policy_meet_lincons_array_apply;
  funptr[AP_FUNPOLICYID_MEET_TCONS_ARRAY_APPLY] = box_policy_meet_tcons_array_apply;
  funptr[AP_FUNPOLICYID_MEET_IMPROVE] = box_policy_meet_improve;
  funptr[AP_FUNPOLICYID_MEET_ARRAY_IMPROVE] = box_policy_meet_array_improve;
  funptr[AP_FUNPOLICYID_MEET_LINCONS_ARRAY_IMPROVE] = box_policy_meet_lincons_array_improve;
  funptr[AP_FUNPOLICYID_MEET_TCONS_ARRAY_IMPROVE] = box_policy_meet_tcons_array_improve;
  return pman;
}

static inline box_policy_one_t box_policy_one_make(size_t nbdims)
{
  box_policy_one_t res;
  res.nbdims = nbdims;
  res.p = (box_policy_dim_t*)malloc(nbdims*sizeof(box_policy_dim_t));
  return res;
}
static inline void box_policy_one_set_choice(box_policy_one_t* one,
					     box_policy_choice_t choice)
{
  size_t i;
  for (i=0; i<one->nbdims; i++){
    one->p[i].inf = choice;
    one->p[i].sup = choice;
  }
}
static inline void box_policy_one_set(box_policy_one_t* a, box_policy_one_t* b)
{
  if (b){
    assert(a->nbdims==b->nbdims);
    for (size_t i=0;i<a->nbdims;i++){
      a->p[i] = b->p[i];
    }
  }
}
static inline void box_policy_one_free(box_policy_one_t* one)
{ free(one->p); one->p = NULL; }

box_policy_t* box_policy_alloc(ap_policy_manager_t* man, size_t size, size_t nbdims)
{
  box_policy_t* boxpolicy = (box_policy_t*)malloc(sizeof(box_policy_t));
  boxpolicy->p =  (box_policy_one_t*)malloc(size*sizeof(box_policy_one_t));
  boxpolicy->size = size;
  boxpolicy->nbdims = nbdims;
  for (size_t i=0; i<size; i++){
    boxpolicy->p[i] = box_policy_one_make(nbdims);
  }
  return boxpolicy;
}
void box_policy_set_choice(box_policy_t* policy, box_policy_choice_t choice)
{
  size_t i;
  for (i=0; i<policy->size; i++){
    box_policy_one_set_choice(&policy->p[i],choice);
  }
}
void box_policy_free(ap_policy_manager_t* man, box_policy_t* boxpolicy)
{
  if (boxpolicy){
    size_t i;
    for (i=0; i<boxpolicy->size; i++){
      box_policy_one_free(&boxpolicy->p[i]);
    }
    free(boxpolicy->p); boxpolicy->p = NULL;
    free(boxpolicy);
  }
}
box_policy_t* box_policy_copy(ap_policy_manager_t* pman, box_policy_t* boxpolicy)
{
  if (boxpolicy==NULL){
    return NULL;
  } else {
    size_t i,nbdims;
    box_policy_t* nboxpolicy = box_policy_alloc(pman,boxpolicy->size,boxpolicy->nbdims);
    if (boxpolicy->size>0){
      for (i=0; i<nboxpolicy->size; i++){
	box_policy_one_set(&nboxpolicy->p[i],&boxpolicy->p[i]);
      }
    }
    return nboxpolicy;
  }
}
size_t box_policy_dimension(ap_policy_manager_t* man, box_policy_t* policy)
{
  return policy ? policy->nbdims : 0;
}

static inline
void box_policy_choice_sprint(char** ret, box_policy_choice_t choice)
{
  switch(choice){
  case BOX_POLICY_1:
    **ret = 'l';
    break;
  case BOX_POLICY_2:
    **ret = 'r';
    break;
  default:
    abort();
  }
  (*ret)++;
}
static inline
void box_policy_dim_sprint(char** ret, box_policy_dim_t* pdim)
{
  box_policy_choice_sprint(ret,pdim->inf);
  box_policy_choice_sprint(ret,pdim->sup);
}
void box_policy_one_sprint(char** ret, box_policy_one_t* policy)
{
  for (size_t j=0; j < policy->nbdims; j++){
    box_policy_dim_sprint(ret,&policy->p[j]);
    **ret = ' ';
    (*ret)++;
  }
}
char* box_policy_sprint(ap_policy_manager_t* man, box_policy_t* boxpolicy)
{
  if (boxpolicy){
    char* const s = malloc(boxpolicy->size * (3*boxpolicy->nbdims + 1) + 1);
    char* p = s;
    for (size_t i=0; i < boxpolicy->size; i++){
      box_policy_one_sprint(&p,&boxpolicy->p[i]);
      *p = '\n';
      p++;
    }
    *p = 0;
    assert((p-s) == (ptrdiff_t)(boxpolicy->size * (3*boxpolicy->nbdims + 1)));
    return s;
  } else {
    return strdup("NULL");
  }
}
void box_policy_fprint(FILE* stdoutput, ap_policy_manager_t* man, box_policy_t* boxpolicy)
{
  char* s = box_policy_sprint(man,boxpolicy);
  fputs(s,stdoutput);
  free(s);
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

bool box_policy_equal(ap_policy_manager_t* man, box_policy_t* boxpolicy1, box_policy_t* boxpolicy2)
{
  if (boxpolicy1==boxpolicy2)
    return true;
  else if (boxpolicy1==NULL || boxpolicy2==NULL)
    return false;

  bool res;
  size_t i;

  res = (boxpolicy1->nbdims == boxpolicy2->nbdims);
  if (!res) return res;
  res = (boxpolicy1->size == boxpolicy2->size);
  if (!res) return res;
  for (i=0; i<boxpolicy1->size; i++){
    res = box_policy_one_equal(&boxpolicy1->p[i],&boxpolicy2->p[i]);
    if (!res) return res;
  }
  return res;
}

static inline
long box_policy_dim_hash(box_policy_dim_t* policy)
{ return 5*(long)(policy->inf)+7*(long)policy->sup; }

long box_policy_one_hash(box_policy_one_t* policy)
{
  size_t i;
  long res;

  res = policy->nbdims;
  for (i=0; i<policy->nbdims; i++){
    long res1 = box_policy_dim_hash(&policy->p[i]);
    res += (res << 1) + res1;
  }
  return res;
}

long box_policy_hash(ap_policy_manager_t* man, box_policy_t* boxpolicy)
{
  if (boxpolicy){
    bool res;
    size_t i;

    res = boxpolicy->size;
    for (i=0; i<boxpolicy->size; i++){
      res += (res << 1) + (box_policy_one_hash(&boxpolicy->p[i]) >> 1);
    }
    return res;
  } else {
    return 0;
  }
}

/* ============================================================ */
/* Meet */
/* ============================================================ */

static inline
void bound_policy_meet_apply(box_policy_choice_t choice,
			     bound_t a, bound_t b, bound_t c)
{
  bound_set(a, (choice==BOX_POLICY_1 ? b :c));
}
static inline
box_policy_choice_t bound_policy_meet_improve(
    box_policy_choice_t* choice,
    bound_t b, bound_t c
)
{
  int cmp = bound_cmp(b,c);
  if (choice && cmp==0)
    return *choice;
  else
    return cmp<=0 ? BOX_POLICY_1 : BOX_POLICY_2;
}

static inline
bool itv_policy_meet_apply(itv_internal_t* intern,
			   box_policy_dim_t policy,
			   itv_t a, itv_t b, itv_t c)
{
  bool exc;
  bound_policy_meet_apply(policy.inf,a->inf,b->inf,c->inf);
  bound_policy_meet_apply(policy.sup,a->sup,b->sup,c->sup);
  exc = itv_canonicalize(intern,a,false);
  return exc;
}
static inline
box_policy_dim_t itv_policy_meet_improve(
    itv_internal_t* intern,
    box_policy_dim_t* policy,
    itv_t b, itv_t c)
{
  box_policy_dim_t res;
  res.inf = bound_policy_meet_improve(policy ? &policy->inf : NULL,b->inf,c->inf);
  res.sup = bound_policy_meet_improve(policy ? &policy->sup : NULL,b->sup,c->sup);
  return res;
}
box_t* box_policy_meet_internal_apply(ap_manager_t* man,
				      box_policy_one_t* policy,
				      bool destructive,
				      box_t* a1, box_t* a2)
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
    exc = itv_policy_meet_apply(intern->itv,
				policy->p[i],
				res->p[i], a1->p[i], a2->p[i]);
    if (exc){
      box_set_bottom(res);
      break;
    }
  }
  return res;
}
void box_policy_meet_internal_improve(
    ap_manager_t* man,
    box_policy_one_t* rpolicy,
    box_policy_one_t* policy,
    box_t* a1, box_t* a2
)
{
  size_t i;
  bool exc = false;
  size_t nbdims;
  box_internal_t* intern = (box_internal_t*)man->internal;

  if (a1->p==NULL || a2->p==NULL){
    box_policy_one_set_choice(rpolicy,a1->p==NULL ? BOX_POLICY_2 : BOX_POLICY_1);
    return;
  }
  nbdims = a1->intdim + a1->realdim;
  if (policy->nbdims != nbdims) abort();
  for (i=0; i<nbdims; i++) {
    rpolicy->p[i] =
      itv_policy_meet_improve(
	  intern->itv,
	  policy ? &policy->p[i] : NULL, a1->p[i], a2->p[i]
      );
  }
}
box_t* box_policy_meet_apply(ap_policy_manager_t* pman,
			     box_policy_t* boxpolicy,
			     bool destructive,
			     box_t* a1, box_t* a2)
{
  assert(boxpolicy->size==1);
  return box_policy_meet_internal_apply(pman->man,
					&boxpolicy->p[0],
					destructive,
					a1,a2);
}
box_policy_t* box_policy_meet_improve(
    ap_policy_manager_t* pman,
    box_policy_t* boxpolicy,
    box_t* a1, box_t* a2
)
{
  assert(boxpolicy ? boxpolicy->size==1 : true);
  box_policy_t* rboxpolicy = box_policy_alloc(pman,1,a1->intdim + a1->realdim);
  box_policy_meet_internal_improve(pman->man,
				   &rboxpolicy->p[0],
				   boxpolicy ? &boxpolicy->p[0] : NULL,
				   a1,a2);
  return rboxpolicy;
}
box_t* box_policy_meet_array_apply(ap_policy_manager_t* pman,
				   box_policy_t* boxpolicy,
				   box_t** tab, size_t size)
{
  size_t i;
  box_t* res;

  assert(size>0 && boxpolicy->size==size-1);
  pman->man->result.flag_best = false;
  pman->man->result.flag_exact = false;
  if (size==1)
    return box_copy(pman->man,tab[0]);
  else if (size==2)
    return box_policy_meet_apply(pman,boxpolicy,false,tab[0],tab[1]);

  res = box_copy(pman->man,tab[0]);
  for (i=1;i<size;i++){
    res = box_policy_meet_internal_apply(pman->man,&boxpolicy->p[i-1],true,res,tab[i]);
  }
  return res;
}
box_policy_t* box_policy_meet_array_improve(ap_policy_manager_t* pman,
					    box_policy_t* boxpolicy,
					    box_t** tab, size_t size)
{
  size_t i;
  box_policy_t* rboxpolicy;
  box_t* res;

  assert(size>0 && boxpolicy->size==size-1);

  if (size==1){
    return box_policy_alloc(pman,0,tab[0]->intdim+tab[0]->realdim);
  }
  else if (size==2)
    return box_policy_meet_improve(pman,boxpolicy,tab[0],tab[1]);
  else {
    res = box_copy(pman->man,tab[0]);
    rboxpolicy = box_policy_alloc(pman,size-1,tab[0]->intdim+tab[0]->realdim);
    for (i=1;i<size;i++){
      box_policy_meet_internal_improve(pman->man,&rboxpolicy->p[i-1],&boxpolicy->p[i-1],res,tab[i]);
      res = box_policy_meet_internal_apply(pman->man,&rboxpolicy->p[i-1],true,res,tab[i]);
    }
    box_free(pman->man,res);
  }
  return rboxpolicy;
}

/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

/* Meet of an abstract value with a constraint

   If boxpolicy!=NULL, reduce a with the constraint according to the policy.
   If rboxpolicy!=NULL, set it to the optimal policy
   It is supposed that either boxpolicy!=NULL or rboxpolicy!=NULL.
 */
void box_policy_meet_lincons_internal(box_internal_t* intern,
				      box_policy_one_t* rboxpolicy,
				      box_policy_one_t* boxpolicy,
				      box_t* a,
				      itv_lincons_t* cons)
{
  size_t nbcoeffs,nbdims;
  ap_dim_t dim = AP_DIM_MAX;
  int previous_dim;
  box_policy_dim_t* rpolicy_dim;
  box_policy_dim_t* policy_dim;
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
	if (rboxpolicy){
	  rboxpolicy->p[j].inf = BOX_POLICY_1;
	  rboxpolicy->p[j].sup = BOX_POLICY_1;
	}
	else {
	  if (boxpolicy->p[j].inf == BOX_POLICY_2){
	    bound_set_infty(a->p[j]->inf,1);
	  }
	  if (boxpolicy->p[j].sup == BOX_POLICY_2){
	    bound_set_infty(a->p[j]->sup,1);
	  }
	}
      }
      previous_dim = (int)dim;
    }
    nbcoeffs++;
    if (rboxpolicy){
      rpolicy_dim = &rboxpolicy->p[dim];
      rpolicy_dim->inf = BOX_POLICY_1;
      rpolicy_dim->sup = BOX_POLICY_1;
      policy_dim = rpolicy_dim;
    }
    else {
      rpolicy_dim = NULL;
      policy_dim = &boxpolicy->p[dim];
    }
    /* 1. sign of a in the expression e = ax+e' */
    int sgncoeff =
      bound_sgn(pitv->inf)<0 ?
      1 :
      ( bound_sgn(pitv->sup)<0 ?
	-1 :
	0 );

    if (sgncoeff==0){
      if (policy_dim->inf == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->inf,1);
      }
      if (policy_dim->sup == BOX_POLICY_2){
	bound_set_infty(a->p[dim]->sup,1);
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
	if ((rboxpolicy || policy_dim->inf == BOX_POLICY_2)
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
	  if (rboxpolicy){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    rpolicy_dim->inf = (cmp==0 && boxpolicy) ? policy_dim->inf : (cmp<0 ? BOX_POLICY_1 : BOX_POLICY_2);
	  }
	  /* We update the interval */
	  if (policy_dim->inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if ((rboxpolicy || policy_dim->sup == BOX_POLICY_2) &&
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
	  if (rboxpolicy){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    rpolicy_dim->sup = (cmp==0 && boxpolicy) ? policy_dim->sup : (cmp<0 ? BOX_POLICY_1 : BOX_POLICY_2);
	  }
	  /* We update the interval */
	  if (policy_dim->sup == BOX_POLICY_2){
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
	if (policy_dim->inf == BOX_POLICY_2 &&
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
	  if (rboxpolicy){
	    cmp = bound_cmp(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	    rpolicy_dim->inf = (cmp==0 && boxpolicy) ? policy_dim->inf : (cmp<0 ? BOX_POLICY_1 : BOX_POLICY_2);
	  }
	  /* We update the interval */
	  if (policy_dim->inf == BOX_POLICY_2){
	    bound_set(a->p[dim]->inf, intern->meet_lincons_internal_bound);
	  }
	}
	if (policy_dim->sup == BOX_POLICY_2 &&
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
	  if (rboxpolicy){
	    cmp = bound_cmp(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	    rpolicy_dim->sup = (cmp==0 && boxpolicy) ? policy_dim->sup : (cmp<0 ? BOX_POLICY_1 : BOX_POLICY_2);
	  }
	  /* We update the interval */
	  if (policy_dim->sup == BOX_POLICY_2){
	    bound_set(a->p[dim]->sup, intern->meet_lincons_internal_bound);
	  }
	}
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
  for (int j=previous_dim+1; j<(int)nbdims; j++){
    if (rboxpolicy){
      rboxpolicy->p[j].inf = BOX_POLICY_1;
      rboxpolicy->p[j].sup = BOX_POLICY_1;
    }
    else {
      if (boxpolicy->p[j].inf == BOX_POLICY_2){
	bound_set_infty(a->p[j]->inf,1);
      }
      if (boxpolicy->p[j].sup == BOX_POLICY_2){
	bound_set_infty(a->p[j]->sup,1);
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
    }
  }
}

box_t* box_policy_meet_lincons_array_apply(ap_policy_manager_t* pman,
					   box_policy_t* boxpolicy,
					   bool destructive,
					   box_t* a,
					   ap_lincons0_array_t* array)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = false;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p!=NULL){
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    assert(boxpolicy->size==kmax*array->size);
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
					   NULL,
					   &boxpolicy->p[k*array->size+i],
					   res, &tlincons.p[i]);
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
box_policy_t* box_policy_meet_lincons_array_improve(
    ap_policy_manager_t* pman,
    box_policy_t* boxpolicy,
    box_t* a, ap_lincons0_array_t* array
)
{
  box_t* res;
  box_policy_t* rboxpolicy;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;

  kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
  if (kmax<1) kmax=2;
  rboxpolicy = box_policy_alloc(pman,kmax*array->size,a->intdim+a->realdim);
  box_policy_set_choice(rboxpolicy,BOX_POLICY_1);
  if (a->p!=NULL){
    res = box_copy(man,a);
    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern->itv,&tlincons,array);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb!=tbool_false){
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
	  box_policy_meet_lincons_internal(
	      intern,
	      &rboxpolicy->p[k*array->size+i],
	      boxpolicy ? &boxpolicy->p[k*array->size+i] : NULL,
	      res,
	      &tlincons.p[i]);
	  if (box_is_bottom(man, res))
	    goto box_meet_lincons_array_policy_exit;
	}
      }
    }
  box_meet_lincons_array_policy_exit:
    box_free(man,res);
    itv_lincons_array_clear(&tlincons);
  }
  return rboxpolicy;
}

box_t* box_policy_meet_tcons_array_apply(
    ap_policy_manager_t* pman, box_policy_t* boxpolicy,
    bool destructive,
    box_t* a, ap_tcons0_array_t* array
)
{
  box_t* res;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = false;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p!=NULL){
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    assert(boxpolicy->size==kmax*array->size);

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
					   NULL,
					   &boxpolicy->p[k*array->size+i],
					   res, &tlincons.p[i]);
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
box_policy_t* box_policy_meet_tcons_array_improve(
    ap_policy_manager_t* pman,
    box_policy_t* boxpolicy,
    box_t* a, ap_tcons0_array_t* array
)
{
  box_t* res;
  box_policy_t* rboxpolicy;
  size_t kmax,i,k;
  itv_lincons_array_t tlincons;
  ap_manager_t* man = pman->man;
  box_internal_t* intern = (box_internal_t*)man->internal;

  kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
  if (kmax<1) kmax=2;
  rboxpolicy = box_policy_alloc(pman,kmax*array->size,a->intdim+a->realdim);
  box_policy_set_choice(rboxpolicy,BOX_POLICY_1);

  if (a->p!=NULL){
    res = box_copy(man,a);
    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern->itv,&tlincons,
				     array,res->p,res->intdim);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb!=tbool_false){
      for (k=0; k<kmax; k++){
	for (i=0; i<array->size; i++){
	  box_policy_meet_lincons_internal(
	      intern,
	      &rboxpolicy->p[k*array->size+i],
	      boxpolicy ? &boxpolicy->p[k*array->size+i] : NULL,
	      res, &tlincons.p[i]);
	  if (box_is_bottom(man, res))
	    goto box_meet_tcons_array_policy_exit;
	}
      }
    }
  box_meet_tcons_array_policy_exit:
    box_free(man,res);
    itv_lincons_array_clear(&tlincons);
  }
  return rboxpolicy;
}
