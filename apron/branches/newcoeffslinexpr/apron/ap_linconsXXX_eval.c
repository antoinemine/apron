/* -*- mode: c -*- */

/* ********************************************************************** */
/* ap_linconsXXX_eval.c: */
/* ********************************************************************** */

#include "ap_linconsXXX.h"
#include "ap_tcons0.h"
#include "num_conv.h"

#define _ITVXXX_MARK_

/* ********************************************************************** */
/* I. Evaluation and simplification */
/* ********************************************************************** */

/* Evaluate a constraint, composed of a constant (interval) expression */
tbool_t ap_linconsXXX_evalcst(ap_linconsXXX_t lincons, num_internal_t intern)
{
  tbool_t res;
  eitvXXX_ptr cst = lincons->linexpr->cst;

  assert (lincons->linexpr->effsize==0);
  if (eitvXXX_is_bottom(cst, intern)){
    return tbool_false;
  }
  switch (lincons->constyp){
  case AP_CONS_EQ:
    if (cst->eq){
      int sgn = boundXXX_sgn(cst->itv->sup);
      res = (sgn==0 ? tbool_true : tbool_false);
    }
    else {
      if (boundXXX_sgn(cst->itv->sup)<0 ||
	  boundXXX_sgn(cst->itv->neginf)<0)
	res = tbool_false;
      else
	res = tbool_top;
    }
    break;
  case AP_CONS_DISEQ:
    res =
      (boundXXX_sgn(cst->itv->neginf)<0 ||
       boundXXX_sgn(cst->itv->sup)<0) ?
      tbool_true :
      tbool_top;
    break;
  case AP_CONS_SUPEQ:
    if (boundXXX_sgn(cst->itv->neginf)<=0)
      res = tbool_true;
    else if (boundXXX_sgn(cst->itv->sup)<0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case AP_CONS_SUP:
    if (boundXXX_sgn(cst->itv->neginf)<0)
      res = tbool_true;
    else if (boundXXX_sgn(cst->itv->sup)<=0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case AP_CONS_EQMOD:
    if (cst->eq){
      if (boundXXX_sgn(cst->itv->sup)==0){
	res = tbool_true;
      }
      else {
#if defined(_ITVRl_MARK_) || defined(_ITVRll_MARK_) || defined(_ITVMPQ_MARK_)
	numXXX_t numrat;
	numXXX_init(numrat);
	numXXX_set_mpq(numrat,lincons->mpq,intern);
	numXXX_div(numrat,boundXXX_numref(cst->itv->sup),numrat);
	if (numintXXX_cmp_int(numXXX_denref(numrat),1)==0){
	  res = tbool_true;
	}
	else {
	  res = tbool_false;
	}
	numXXX_clear(numrat);
#elif defined(_ITVIl_MARK_) || defined(_ITVIll_MARK_) || defined(_ITVMPZ_MARK_)
	numXXX_t numint;
	if (mpz_cmp_ui(mpq_denref(lincons->mpq),1)!=0){
	  res = tbool_top;
	}
	else {
	  numXXX_init(numint);
	  numXXX_set_mpz(numint,mpq_numref(lincons->mpq),intern);
	  numXXX_mod(numint,boundXXX_numref(cst->itv->sup),numint);
	  if (numXXX_sgn(numint)==0){
	    res = tbool_true;
	  }
	  else {
	    res = tbool_false;
	  }
	  numXXX_clear(numint);
	}
#elif defined(_ITVD_MARK_) || defined(_ITVDl_MARK_) || defined(_ITVMPFR_MARK_)
	res = tbool_top;
#else
#error "HERE"
#endif
      }
    }
    else {
      res = tbool_top;
    }
    break;
  default:
    abort();
  }
  return res;
}

bool ap_linconsXXX_sat_is_false(ap_linconsXXX_t lincons, num_internal_t intern)
{
  bool res = false;

  eitvXXX_ptr cst = lincons->linexpr->cst;
  bool inf = boundXXX_infty(cst->itv->neginf);

  switch (lincons->constyp){
  case AP_CONS_EQ:
  case AP_CONS_EQMOD:
    res = !cst->eq;
    break;
  case AP_CONS_DISEQ:
    res = inf && boundXXX_infty(cst->itv->sup);
    break;
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    res = inf;
    break;
  default:
    break;
  }
  if (!res &&
      lincons->linexpr->effsize==0 &&
      ap_linconsXXX_evalcst(lincons, intern)==tbool_false){
    res = true;
  }
  return res;
}

static
bool
ap_linconsXXX_is_useless_for_meet(ap_linconsXXX_t lincons, num_internal_t intern)
{
  bool res = false;
  eitvXXX_ptr cst = lincons->linexpr->cst;
  if (lincons->linexpr->effsize==0){
    if (ap_linconsXXX_evalcst(lincons, intern)!=tbool_false)
      res = true;
  }
  else {
    if (!cst->eq){
      bool sup = boundXXX_infty(cst->itv->sup);
      switch (lincons->constyp){
      case AP_CONS_EQ:
      case AP_CONS_DISEQ:
      case AP_CONS_EQMOD:
	{
	  bool inf = boundXXX_infty(cst->itv->neginf);
	  res = inf && sup;
	}
	break;
      case AP_CONS_SUPEQ:
      case AP_CONS_SUP:
	res = sup;
	break;
      default:
	break;
      }
    }
  }
  return res;
}

void ap_linconsXXX_reduce_integer(ap_linconsXXX_t cons,
				  size_t intdim, num_internal_t intern)
{
  ap_linexprXXX_ptr expr;
  size_t i;
  eitvXXX_ptr pitv;
  ap_dim_t dim;
  bool integer;

  switch (cons->constyp){
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    break;
  default:
    return;
  }
  expr = cons->linexpr;
  /* Tests if only integer variables are involved */
  if (!ap_linexprXXX_is_integer(expr,intdim))
    return;
  /* Check that there are only scalar coefficients for dimensions */
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
    if (!eitvXXX_is_point(pitv))
      return;
  }
#if defined(_ITVRl_MARK_) || defined(_ITVRll_MARK_) || defined(_ITVMPQ_MARK_)
  {
    /* compute lcm of denominators and gcd of numerators */
    numintXXX_set_int(numXXX_denref(intern->XXX.quasi_num),1);
    numintXXX_set_int(numXXX_numref(intern->XXX.quasi_num),0);
    ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
      numintXXX_lcm(numXXX_denref(intern->XXX.quasi_num),
		    numXXX_denref(intern->XXX.quasi_num),
		    numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_gcd(numXXX_numref(intern->XXX.quasi_num),
		    numXXX_numref(intern->XXX.quasi_num),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)));
    }
    if (numintXXX_sgn(numXXX_numref(intern->XXX.quasi_num))==0)
      return;
    ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(intern->XXX.quasi_num));
      numintXXX_mul(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_denref(intern->XXX.quasi_num));
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_set_int(numXXX_denref(boundXXX_numref(pitv->itv->sup)),
			1);
      boundXXX_neg(pitv->itv->neginf,pitv->itv->sup);
    }
    numXXX_inv(intern->XXX.quasi_num,intern->XXX.quasi_num);
    eitvXXX_mul_num(expr->cst,expr->cst,intern->XXX.quasi_num);
  }
#else
#if defined(_ITVD_MARK_) || defined(_ITVDl_MARK_) || defined(_ITVMPFR_MARK_)
  {
    /* Assuming that all coefficients are either integer,
       compute the pgcd */
    mpz_set_si(intern->XXX.reduce_lincons_gcd,0);
    ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
      if (!numXXX_integer(pitv->itv->sup))
	return;
      numMPZ_set_numXXX(intern->XXX.reduce_lincons_mpz,pitv->itv->sup,intern);
      mpz_gcd(intern->XXX.reduce_lincons_gcd,
	      intern->XXX.reduce_lincons_gcd,
	      intern->XXX.reduce_lincons_mpz);
    }
    if (mpz_sgn(intern->XXX.reduce_lincons_gcd)==0 ||
	mpz_cmp_si(intern->XXX.reduce_lincons_gcd,1)==0)
      return;
    bool exact = numXXX_set_mpz(intern->XXX.quasi_num,intern->XXX.reduce_lincons_gcd,intern);
    if (!exact) return;
  }
#elif defined(_ITVIl_MARK_) || defined(_ITVIll_MARK_) || defined(_ITVMPZ_MARK_)
  {
    numXXX_set_int(intern->XXX.quasi_num,0);
    ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
      numXXX_gcd(intern->XXX.quasi_num,
		 intern->XXX.quasi_num,
		 boundXXX_numref(pitv->itv->sup));
    }
    if (numXXX_sgn(intern->XXX.quasi_num)==0 ||
	numXXX_cmp_int(intern->XXX.quasi_num,1)==0)
      return;
  }
#else
#error "HERE"
#endif
  /* divide by gcd put in intern->XXX.quasi_num */
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
    eitvXXX_div_num(pitv,pitv,intern->XXX.quasi_num);
  }
  eitvXXX_div_num(expr->cst,expr->cst,intern->XXX.quasi_num);
#endif
  /* Constrain bounds */
  if (!boundXXX_infty(expr->cst->itv->sup)){
    if (cons->constyp==AP_CONS_SUP){
      if (numXXX_integer(boundXXX_numref(expr->cst->itv->sup))){
	boundXXX_sub_uint(expr->cst->itv->sup,expr->cst->itv->sup,1);
      }
      else {
	numXXX_floor(boundXXX_numref(expr->cst->itv->sup),
		     boundXXX_numref(expr->cst->itv->sup));
      }
      cons->constyp = AP_CONS_SUPEQ;
    }
    else {
      numXXX_floor(boundXXX_numref(expr->cst->itv->sup),
		   boundXXX_numref(expr->cst->itv->sup));
    }
  }
  if (cons->constyp == AP_CONS_EQ){
    if (!boundXXX_infty(expr->cst->itv->neginf)){
      if (expr->cst->eq){
	boundXXX_neg(expr->cst->itv->neginf,
		     expr->cst->itv->sup);
      }
      else {
	numXXX_floor(boundXXX_numref(expr->cst->itv->neginf),
		     boundXXX_numref(expr->cst->itv->neginf));
      }
    }
    if (eitvXXX_is_bottom(expr->cst, intern)){
      ap_linconsXXX_set_bool(cons,false);
    }
  }
  else {
    if (!boundXXX_infty(expr->cst->itv->sup)){
      boundXXX_neg(expr->cst->itv->neginf,expr->cst->itv->sup);
      expr->cst->eq = true;
    }
  }
}

tbool_t ap_linconsXXX_array_reduce(ap_linconsXXX_array_t array, bool for_meet_inequality, num_internal_t intern)
{
  tbool_t res;
  size_t i,size;

  res = tbool_top;
  i = 0;
  size = array->size;
  while (i<size){
    if (array->p[i]->linexpr->effsize==0){
      tbool_t sat = ap_linconsXXX_evalcst(array->p[i], intern);
      if (sat==tbool_true){
      ap_linconsXXX_array_reduce_remove:
	size --;
	ap_linconsXXX_swap(array->p[i],array->p[size]);
	continue;
      }
      else if (sat==tbool_false){
      ap_linconsXXX_array_reduce_false:
	ap_linconsXXX_array_resize(array,1);
	ap_linconsXXX_set_bool(array->p[0],false);
	return tbool_false;
      }
    }
    if (for_meet_inequality && ap_linconsXXX_is_useless_for_meet(array->p[i], intern))
      goto ap_linconsXXX_array_reduce_remove;
    else if (!for_meet_inequality && ap_linconsXXX_sat_is_false(array->p[i], intern))
      goto ap_linconsXXX_array_reduce_false;
    else {
      i++;
    }
  }
  ap_linconsXXX_array_resize(array,size);
  if (size==0)
    res = tbool_true;
  else if (size==1 && array->p[0]->linexpr->effsize==0)
    res = ap_linconsXXX_evalcst(array->p[0], intern);
  return res;
}

tbool_t ap_linconsXXX_array_reduce_integer(ap_linconsXXX_array_t array,
					   size_t intdim, num_internal_t intern)
{
  size_t i;
  for (i=0; i<array->size; i++){
    ap_linconsXXX_reduce_integer(array->p[i],intdim, intern);
  }
  return ap_linconsXXX_array_reduce(array,true, intern);
}

/* ********************************************************************** */
/* II. Linearization */
/* ********************************************************************** */

bool ap_linconsXXX_array_quasilinearize(ap_linconsXXX_array_t array, ap_linexprXXX_t env, bool for_meet_inequality, num_internal_t intern)
{
  size_t i;
  bool res;
  res = true;
  for (i=0; i<array->size; i++) {
    ap_linconsXXX_quasilinearize(array->p[i],env,for_meet_inequality,intern);
  }
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}
static void ap_linconsXXX_select_sup(ap_linconsXXX_t cons)
{
  eitvXXX_ptr cst = cons->linexpr->cst;
  boundXXX_neg(cst->itv->neginf,cst->itv->sup);
  cst->eq = true;
}
static void ap_linconsXXX_select_inf(ap_linconsXXX_t cons)
{
  size_t i;
  eitvXXX_ptr cst = cons->linexpr->cst;
  boundXXX_neg(cst->itv->sup,cst->itv->neginf);
  cst->eq = true;
  ap_linexprXXX_neg(cons->linexpr,cons->linexpr);
}
void ap_linconsXXX_array_linearize(ap_linconsXXX_array_t array,
				   bool for_meet_inequality,
				   num_internal_t intern)
{
  size_t index,size,sizeorg;

  tbool_t res = ap_linconsXXX_array_reduce(array,for_meet_inequality,intern);
  if (res!=tbool_top) return;

  /* One now remove intervals when we can */
  sizeorg = array->size;
  size = sizeorg;
  for (index=0; index<sizeorg; index++){
    ap_linconsXXX_ptr cons = array->p[index];
    ap_linexprXXX_ptr linexpr = cons->linexpr;
    eitvXXX_ptr cst = linexpr->cst;
    if (!cst->eq){
      bool sup = !boundXXX_infty(cst->itv->sup);
      switch (cons->constyp){
      case AP_CONS_EQ:
	assert (for_meet_inequality); /* otherwise, already removed */
	{
	  bool inf = !boundXXX_infty(cst->itv->neginf);
	  assert (inf || sup); /* otherwise, already removed */
	  if (inf && sup){
	    if (size>=array->size){
	      ap_linconsXXX_array_resize(array,1+5*array->size/4);
	    }
	    /* Be cautious: cons and cst may be invalid now */
	    ap_linconsXXX_set(array->p[size],array->p[index]);
	    array->p[index]->constyp = AP_CONS_SUPEQ;
	    array->p[size]->constyp  = AP_CONS_SUPEQ;
	    ap_linconsXXX_select_sup(array->p[index]);
	    ap_linconsXXX_select_inf(array->p[size]);
	    size++;
	  }
	  else if (inf){
	    array->p[index]->constyp = AP_CONS_SUPEQ;
	    ap_linconsXXX_select_inf(array->p[index]);
	  }
	  else if (sup){
	    array->p[index]->constyp = AP_CONS_SUPEQ;
	    ap_linconsXXX_select_sup(array->p[index]);
	  }
	  else
	    assert(false);
	}
	break;
      case AP_CONS_SUPEQ:
      case AP_CONS_SUP:
	if (for_meet_inequality){
	  assert(sup);
	  ap_linconsXXX_select_sup(array->p[index]);
	}
	else {
	  assert(!boundXXX_infty(cst->itv->neginf));
	  boundXXX_neg(cst->itv->sup,cst->itv->neginf);
	  cst->eq = true;
	}
	break;
      default:
	break;
      }
    }
  }
  ap_linconsXXX_array_resize(array,size);
}

bool ap_linconsXXX_set_tcons0(
    ap_linconsXXX_t lincons, bool* perror,
    ap_tcons0_t* cons, num_internal_t intern)
{
  bool res =
    ap_linexprXXX_set_texpr0(lincons->linexpr, perror, cons->texpr0, intern);
  lincons->constyp = cons->constyp;
  mpq_set(lincons->mpq,cons->mpq);
  return res;
}
bool ap_linconsXXX_array_set_tcons0_array(
    ap_linconsXXX_array_t lincons, bool* perror,
    struct ap_tcons0_array_t* cons, num_internal_t intern)
{
  size_t i;
  bool res = true;
  ap_linconsXXX_array_resize(lincons,cons->size);
  for (i=0; i<cons->size; i++){
    res =
      ap_linconsXXX_set_tcons0(lincons->p[i],perror,cons->p[i],intern)
      && res;
    if (*perror) break;
  }
  return res;
}

void ap_linconsXXX_intlinearize_tcons0(
    ap_linconsXXX_t lincons, struct ap_tcons0_t* cons,
    ap_linexprXXX_t env, size_t intdim, num_internal_t intern)
{
  ap_linexprXXX_intlinearize_texpr0(
      lincons->linexpr, cons->texpr0, env, intdim, intern
  );
  lincons->constyp = cons->constyp;
  mpq_set(lincons->mpq,cons->mpq);
}
void ap_linconsXXX_array_intlinearize_tcons0_array(
    ap_linconsXXX_array_t lincons, struct ap_tcons0_array_t* cons,
    ap_linexprXXX_t env, size_t intdim, num_internal_t intern)
{
  size_t i;
  ap_linconsXXX_array_resize(lincons,cons->size);
  for (i=0; i<cons->size; i++){
    ap_linconsXXX_intlinearize_tcons0(lincons->p[i],cons->p[i],
				      env,intdim,intern);
  }
}

/* ********************************************************************** */
/* III. Boxization of interval linear expressions */
/* ********************************************************************** */

static bool ap_linconsXXX_boxize(ap_linexprXXX_t res,
				 bool* tchange,
				 ap_linconsXXX_t cons,
				 ap_linexprXXX_t env,
				 size_t intdim,
				 bool intervalonly, num_internal_t intern)
{
  size_t i;
  ap_linexprXXX_ptr expr;
  eitvXXX_ptr eitv,resdim;
  ap_dim_t dim;
  bool change,globalchange;
  bool exc;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  expr = cons->linexpr;
  globalchange = false;

  /* */
  if (res!=env){
    ap_linexprXXX_ForeachLinterm0(env,i,dim,eitv){
      resdim = ap_linexprXXX_eitvref0(res,dim,false);
      if (resdim==NULL){
	ap_linexprXXX_set_eitv0(res,dim,eitv);
      }
    }
  }
  /* Iterates on coefficients */
  eitvXXX_set_int(intern->XXX.boxize_lincons_eitv,0);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    bool equality = eitv->eq;
    /* 1. We decompose the expression e = ax+e' */
    eitvXXX_swap(intern->XXX.boxize_lincons_eitv,eitv);
    /* 2. evaluate e' */
    ap_linexprXXX_eval(intern->XXX.boxize_lincons_eval,expr, env, intern);
    /* 3. Perform deductions */
    change = false;
    if (!eitvXXX_is_top(intern->XXX.boxize_lincons_eval)){
      resdim = ap_linexprXXX_eitvref0(res,dim,false);
      if (equality && !intervalonly){
	int sgn = boundXXX_sgn(intern->XXX.boxize_lincons_eitv->itv->sup);
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
	      boundXXX_div(intern->XXX.boxize_lincons_bound,
			   intern->XXX.boxize_lincons_eval->itv->sup,
			   intern->XXX.boxize_lincons_eitv->itv->sup);
	    }
	    else {
	      boundXXX_div(intern->XXX.boxize_lincons_bound,
			   intern->XXX.boxize_lincons_eval->itv->neginf,
			   intern->XXX.boxize_lincons_eitv->itv->neginf);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX.boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX.boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX.boxize_lincons_bound),
				boundXXX_numref(intern->XXX.boxize_lincons_bound),
				1);
	      }
	      else {
		boundXXX_floor(intern->XXX.boxize_lincons_bound,
			       intern->XXX.boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX.boxize_lincons_bound, resdim->itv->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(resdim->itv->neginf, intern->XXX.boxize_lincons_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == AP_CONS_EQ){
	    /*
	      If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	      If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      boundXXX_div(intern->XXX.boxize_lincons_bound,
			   intern->XXX.boxize_lincons_eval->itv->sup,
			   intern->XXX.boxize_lincons_eitv->itv->neginf);
	    }
	    else {
	      boundXXX_div(intern->XXX.boxize_lincons_bound,
			   intern->XXX.boxize_lincons_eval->itv->neginf,
			   intern->XXX.boxize_lincons_eitv->itv->sup);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX.boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX.boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX.boxize_lincons_bound),
				boundXXX_numref(intern->XXX.boxize_lincons_bound),
				1);
	      }
	      else {
		boundXXX_floor(intern->XXX.boxize_lincons_bound,
			       intern->XXX.boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX.boxize_lincons_bound, resdim->itv->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(resdim->itv->sup, intern->XXX.boxize_lincons_bound);
	    }
	  }
	}
      }
      else if (!equality){
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
	int sgnitv =
	  boundXXX_sgn(intern->XXX.boxize_lincons_eitv->itv->neginf)<0 ?
	  1 :
	  ( boundXXX_sgn(intern->XXX.boxize_lincons_eitv->itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgnitv != 0){
	  int sgnevalinf = boundXXX_sgn(intern->XXX.boxize_lincons_eval->itv->neginf);
	  int sgnevalsup = boundXXX_sgn(intern->XXX.boxize_lincons_eval->itv->sup);
	  if (sgnitv>0 || (cons->constyp==AP_CONS_EQ && sgnitv<0)){
	    if (sgnitv>0){
	      if (sgnevalsup<=0){
		/* We compute sup(e')/M */
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->sup,
			     intern->XXX.boxize_lincons_eitv->itv->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		boundXXX_neg(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->sup);
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->neginf,
			     intern->XXX.boxize_lincons_eitv->itv->neginf);
	      } else {
		/* We compute inf(e')/M) = (-inf(e'))/(-M) */
		boundXXX_neg(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->neginf,
			     intern->XXX.boxize_lincons_bound);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX.boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX.boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX.boxize_lincons_bound),
				boundXXX_numref(intern->XXX.boxize_lincons_bound),
				1);
	      }
	      else {
		boundXXX_floor(intern->XXX.boxize_lincons_bound,
			       intern->XXX.boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX.boxize_lincons_bound, resdim->itv->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(resdim->itv->neginf, intern->XXX.boxize_lincons_bound);
	    }
	  }
	  if (sgnitv<0 || (cons->constyp==AP_CONS_EQ && sgnitv>0)){
	    if (sgnitv<0){
	      if (sgnevalsup>=0){
		/* We compute sup(e')/-M */
		boundXXX_neg(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->sup,
			     intern->XXX.boxize_lincons_bound);
	      } else {
		/* We compute sup(e')/-m */
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->sup,
			     intern->XXX.boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf>=0){
		/* We compute -inf(e')/M */
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->neginf,
			     intern->XXX.boxize_lincons_eitv->itv->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		boundXXX_neg(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eval->itv->neginf);
		boundXXX_div(intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_bound,
			     intern->XXX.boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX.boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX.boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX.boxize_lincons_bound),
				boundXXX_numref(intern->XXX.boxize_lincons_bound),
				1);
	      }
	      else {
		boundXXX_floor(intern->XXX.boxize_lincons_bound,
			       intern->XXX.boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX.boxize_lincons_bound, resdim->itv->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(resdim->itv->sup, intern->XXX.boxize_lincons_bound);
	    }
	  }
	}
      }
    }
    eitvXXX_swap(intern->XXX.boxize_lincons_eitv,eitv);
    if (change){
      globalchange = true;
      exc = itvXXX_canonicalize(resdim->itv,dim<intdim, intern);
      if (exc){
	eitvXXX_ptr res0 = ap_linexprXXX_eitvref0(res,0,true);
	eitvXXX_set_bottom(res0);
	return true;
      }
      else {
	resdim->eq = itvXXX_is_point(resdim->itv);
      }
    }
  }
  if (expr->effsize==0 &&
      ap_linconsXXX_evalcst(cons, intern)==tbool_false){
    eitvXXX_ptr res0 = ap_linexprXXX_eitvref0(res,0,true);
    eitvXXX_set_bottom(res0);
    globalchange = true;
  }
  return globalchange;
}

/* This function deduces interval constraints from a set of interval linear
   constraints.

   Return true if some (better if res==env) bounds have been inferred.

   - The inferred bounds are stored in res (which may be equal to env)
   - If tchange!=NULL *and initialized to false*,
   tchange[2dim] (resp. 2dim+1) set to true indicates
   that the inf (resp. sup) bound of dimension dim has been improved.
   - env is the current bounds for variables
   - kmax specifies the maximum number of iterations, when res==env
   - if intervalonly is true, deduces bounds from a constraint only when the
   coefficient associated to the current dimension is an interval.
*/
bool ap_linconsXXX_array_boxize(ap_linexprXXX_t res,
				bool* tchange,
				ap_linconsXXX_array_t array,
				ap_linexprXXX_t env, size_t intdim,
				size_t kmax,
				bool intervalonly, num_internal_t intern)
{
  size_t i,k;
  bool change,globalchange;

  if (kmax<1) kmax=1;
  if (res!=env) kmax=1;

  globalchange = false;
  /* we possibly perform kmax passes */
  for (k=0;k<(size_t)kmax;k++){
    change = false;
    for (i=0; i<array->size; i++){
      if (array->p[i]->constyp==AP_CONS_EQ ||
	  array->p[i]->constyp==AP_CONS_SUPEQ ||
	  array->p[i]->constyp==AP_CONS_SUP){
	change =
	  ap_linconsXXX_boxize(res,tchange,array->p[i],env,intdim,intervalonly, intern)
	  ||
	  change
	  ;
	globalchange = globalchange || change;
	{
	  eitvXXX_ptr res0 = ap_linexprXXX_eitvref0(res,0,false);
	  if (res0 && eitvXXX_is_bottom(res0, intern)){
	    return true;
	  }
	}
      }
    }
    if (!change) break;
  }
  return globalchange;
}


#undef _ITVXXX_MARK_
