/* -*- mode: c -*- */

/* ********************************************************************** */
/* ap_linconsXXX.c: */
/* ********************************************************************** */

#include "ap_linconsXXX.h"

#define _ITVXXX_MARK_

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

ap_linconsXXX_ptr ap_linconsXXX_alloc(size_t size)
{
  ap_linconsXXX_ptr res = (ap_linconsXXX_ptr)malloc(sizeof(ap_linconsXXX_struct));
  ap_linconsXXX_init(res,size);
  return res;
}
ap_linconsXXX_ptr ap_linconsXXX_alloc_set(ap_linconsXXX_t cons)
{
  ap_linconsXXX_ptr res = (ap_linconsXXX_ptr)malloc(sizeof(ap_linconsXXX_struct));
  ap_linconsXXX_init_set(res,cons);
  return res;
}
ap_linconsXXX_ptr ap_linconsXXX_alloc_set_linexpr(ap_linexprXXX_t expr, ap_constyp_t constyp, mpq_ptr mpq)
{
  ap_linconsXXX_ptr res = (ap_linconsXXX_ptr)malloc(sizeof(ap_linconsXXX_struct));
  ap_linconsXXX_init_set_linexpr(res,expr,constyp,mpq);
  return res;
}
void ap_linconsXXX_free(ap_linconsXXX_ptr e)
{
  ap_linconsXXX_clear(e);
  free(e);
}

void ap_linconsXXX_set_bool(ap_linconsXXX_t lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  ap_linexprXXX_resize(lincons->linexpr,0);
  eitvXXX_set_int(lincons->linexpr->cst,value ? 0 : 1);
  lincons->constyp = AP_CONS_EQ;
}
void ap_linconsXXX_fprint(FILE* stream, ap_linconsXXX_t cons, char** name)
{
  ap_linexprXXX_fprint(stream,cons->linexpr,name);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	     "\"ERROR in ap_linconsXXX_fprint\"")));
  if (cons->constyp == AP_CONS_EQMOD){
    fprintf(stream," mod ");
    mpq_out_str(stream,10,cons->mpq);
  }
  fflush(stream);
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

/* ********************************************************************** */
/* III. Evaluation, simplification and linearisation */
/* ********************************************************************** */

/* Evaluate a constraint, composed of a constant (interval) expression */
tbool_t ap_linconsXXX_evalcst(
			       ap_linconsXXX_t lincons, itv_internal_t intern)
{
  tbool_t res;
  eitvXXX_ptr cst = lincons->linexpr->cst;

  assert (lincons->linexpr->size==0);
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
	numXXX_set_mpq(numrat,lincons->mpq,intern->num);
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
	  numXXX_set_mpz(numint,mpq_numref(lincons->mpq),intern->num);
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

bool ap_linconsXXX_sat_is_false(
				 ap_linconsXXX_t lincons, itv_internal_t intern)
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
      lincons->linexpr->size==0 &&
      ap_linconsXXX_evalcst(lincons, intern)==tbool_false){
    res = true;
  }
  return res;
}

static bool
ap_linconsXXX_is_useless_for_meet(
				   ap_linconsXXX_t lincons, itv_internal_t intern)
{
  bool res = false;
  eitvXXX_ptr cst = lincons->linexpr->cst;
  if (lincons->linexpr->size==0){
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

void ap_linconsXXX_reduce_integer(
				   ap_linconsXXX_t cons,
				   size_t intdim, itv_internal_t intern)
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
  ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
    if (!eitvXXX_is_point(pitv))
      return;
  }
#if defined(_ITVRl_MARK_) || defined(_ITVRll_MARK_) || defined(_ITVMPQ_MARK_)
  {
    /* compute lcm of denominators and gcd of numerators */
    numintXXX_set_int(numXXX_denref(intern->XXX->quasi_num),1);
    numintXXX_set_int(numXXX_numref(intern->XXX->quasi_num),0);
    ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
      numintXXX_lcm(numXXX_denref(intern->XXX->quasi_num),
		    numXXX_denref(intern->XXX->quasi_num),
		    numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_gcd(numXXX_numref(intern->XXX->quasi_num),
		    numXXX_numref(intern->XXX->quasi_num),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)));
    }
    if (numintXXX_sgn(numXXX_numref(intern->XXX->quasi_num))==0)
      return;
    ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(intern->XXX->quasi_num));
      numintXXX_mul(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_denref(intern->XXX->quasi_num));
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_set_int(numXXX_denref(boundXXX_numref(pitv->itv->sup)),
			1);
      boundXXX_neg(pitv->itv->neginf,pitv->itv->sup);
    }
    numXXX_inv(intern->XXX->quasi_num,intern->XXX->quasi_num);
    eitvXXX_mul_num(expr->cst,expr->cst,intern->XXX->quasi_num);
  }
#else
#if defined(_ITVD_MARK_) || defined(_ITVDl_MARK_) || defined(_ITVMPFR_MARK_)
  {
    /* Assuming that all coefficients are either integer,
       compute the pgcd */
    mpz_set_si(intern->XXX->reduce_lincons_gcd,0);
    ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
      if (!numXXX_integer(pitv->itv->sup))
	return;
      mpz_set_numXXX(intern->XXX->reduce_lincons_mpz,pitv->itv->sup,intern->num);
      mpz_gcd(intern->XXX->reduce_lincons_gcd,
	      intern->XXX->reduce_lincons_gcd,
	      intern->XXX->reduce_lincons_mpz);
    }
    if (mpz_sgn(intern->XXX->reduce_lincons_gcd)==0 ||
	mpz_cmp_si(intern->XXX->reduce_lincons_gcd,1)==0)
      return;
    bool exact = numXXX_set_mpz(intern->XXX->quasi_num,intern->XXX->reduce_lincons_gcd,intern->num);
    if (!exact) return;
  }
#elif defined(_ITVIl_MARK_) || defined(_ITVIll_MARK_) || defined(_ITVMPZ_MARK_)
  {
    numXXX_set_int(intern->XXX->quasi_num,0);
    ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
      numXXX_gcd(intern->XXX->quasi_num,
		 intern->XXX->quasi_num,
		 boundXXX_numref(pitv->itv->sup));
    }
    if (numXXX_sgn(intern->XXX->quasi_num)==0 ||
	numXXX_cmp_int(intern->XXX->quasi_num,1)==0)
      return;
  }
#else
#error "HERE"
#endif
  /* divide by gcd put in intern->XXX->quasi_num */
  ap_linexprXXX_ForeachLinterm(expr,i,dim,pitv) {
    eitvXXX_div_num(pitv,pitv,intern->XXX->quasi_num);
  }
  eitvXXX_div_num(expr->cst,expr->cst,intern->XXX->quasi_num);
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

tbool_t ap_linconsXXX_array_reduce(
				    ap_linconsXXX_array_t array, bool meet, itv_internal_t intern)
{
  tbool_t res;
  size_t i,size;

  res = tbool_top;
  i = 0;
  size = array->size;
  while (i<size){
    if (array->p[i]->linexpr->size==0){
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
    if (meet && ap_linconsXXX_is_useless_for_meet(array->p[i], intern))
      goto ap_linconsXXX_array_reduce_remove;
    else if (!meet && ap_linconsXXX_sat_is_false(array->p[i], intern))
      goto ap_linconsXXX_array_reduce_false;
    else {
      i++;
    }
  }
  ap_linconsXXX_array_resize(array,size);
  if (size==0)
    res = tbool_true;
  else if (size==1 && array->p[0]->linexpr->size==0)
    res = ap_linconsXXX_evalcst(array->p[0], intern);
  return res;
}

tbool_t ap_linconsXXX_array_reduce_integer(
					 ap_linconsXXX_array_t array,
					 size_t intdim, itv_internal_t intern)
{
  size_t i;
  for (i=0; i<array->size; i++){
    ap_linconsXXX_reduce_integer(array->p[i],intdim, intern);
  }
  return ap_linconsXXX_array_reduce(array,true, intern);
}

bool ap_linconsXXX_array_quasilinearize(
					 ap_linconsXXX_array_t array, itvXXX_t* env,
					 bool for_meet_inequality, itv_internal_t intern)
{
  size_t i;
  bool res;
  res = true;
  for (i=0; i<array->size; i++) {
    ap_linconsXXX_quasilinearize(array->p[i],env,for_meet_inequality, intern);
  }
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}

/* ********************************************************************** */
/* IV. Boxization of interval linear expressions */
/* ********************************************************************** */

static bool ap_linconsXXX_boxize(
				  itvXXX_t* res,
				  bool* tchange,
				  ap_linconsXXX_t cons,
				  itvXXX_t* env,
				  size_t intdim,
				  bool intervalonly, itv_internal_t intern)
{
  size_t i;
  ap_linexprXXX_ptr expr;
  eitvXXX_ptr eitv;
  ap_dim_t dim;
  bool change,globalchange;
  bool exc;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  expr = cons->linexpr;
  globalchange = false;

  /* Iterates on coefficients */
  eitvXXX_set_int(intern->XXX->boxize_lincons_eitv,0);
  ap_linexprXXX_ForeachLinterm(expr,i,dim,eitv){
    bool equality = eitv->eq;
    /* 1. We decompose the expression e = ax+e' */
    eitvXXX_swap(intern->XXX->boxize_lincons_eitv,eitv);
    /* 2. evaluate e' */
    ap_linexprXXX_eval(
			intern->XXX->boxize_lincons_eval,expr,env, intern);
    /* 3. Perform deductions */
    change = false;
    if (!itvXXX_is_top(intern->XXX->boxize_lincons_eval)){
      if (equality && !intervalonly){
	int sgn = boundXXX_sgn(intern->XXX->boxize_lincons_eitv->itv->sup);
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
	      boundXXX_div(intern->XXX->boxize_lincons_bound,
			intern->XXX->boxize_lincons_eval->sup,
			intern->XXX->boxize_lincons_eitv->itv->sup);
	    }
	    else {
	      boundXXX_div(intern->XXX->boxize_lincons_bound,
			intern->XXX->boxize_lincons_eval->neginf,
			intern->XXX->boxize_lincons_eitv->itv->neginf);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->XXX->boxize_lincons_bound,
			    intern->XXX->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(res[dim]->neginf, intern->XXX->boxize_lincons_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == AP_CONS_EQ){
	    /*
	      If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	      If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      boundXXX_div(intern->XXX->boxize_lincons_bound,
			intern->XXX->boxize_lincons_eval->sup,
			intern->XXX->boxize_lincons_eitv->itv->neginf);
	    }
	    else {
	      boundXXX_div(intern->XXX->boxize_lincons_bound,
			intern->XXX->boxize_lincons_eval->neginf,
			intern->XXX->boxize_lincons_eitv->itv->sup);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->XXX->boxize_lincons_bound,
			    intern->XXX->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(res[dim]->sup, intern->XXX->boxize_lincons_bound);
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
	  boundXXX_sgn(intern->XXX->boxize_lincons_eitv->itv->neginf)<0 ?
	  1 :
	  ( boundXXX_sgn(intern->XXX->boxize_lincons_eitv->itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgnitv != 0){
	  int sgnevalinf = boundXXX_sgn(intern->XXX->boxize_lincons_eval->neginf);
	  int sgnevalsup = boundXXX_sgn(intern->XXX->boxize_lincons_eval->sup);
	  if (sgnitv>0 || (cons->constyp==AP_CONS_EQ && sgnitv<0)){
	    if (sgnitv>0){
	      if (sgnevalsup<=0){
		/* We compute sup(e')/M */
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->sup,
			  intern->XXX->boxize_lincons_eitv->itv->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		boundXXX_neg(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->sup);
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->neginf,
			  intern->XXX->boxize_lincons_eitv->itv->neginf);
	      } else {
		/* We compute inf(e')/M) = (-inf(e'))/(-M) */
		boundXXX_neg(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->neginf,
			  intern->XXX->boxize_lincons_bound);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->XXX->boxize_lincons_bound,
			    intern->XXX->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(res[dim]->neginf, intern->XXX->boxize_lincons_bound);
	    }
	  }
	  if (sgnitv<0 || (cons->constyp==AP_CONS_EQ && sgnitv>0)){
	    if (sgnitv<0){
	      if (sgnevalsup>=0){
		/* We compute sup(e')/-M */
		boundXXX_neg(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->sup,
			  intern->XXX->boxize_lincons_bound);
	      } else {
		/* We compute sup(e')/-m */
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->sup,
			  intern->XXX->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf>=0){
		/* We compute -inf(e')/M */
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->neginf,
			  intern->XXX->boxize_lincons_eitv->itv->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		boundXXX_neg(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eval->neginf);
		boundXXX_div(intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_bound,
			  intern->XXX->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->XXX->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->XXX->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     boundXXX_numref(intern->XXX->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->XXX->boxize_lincons_bound,
			    intern->XXX->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->XXX->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(res[dim]->sup, intern->XXX->boxize_lincons_bound);
	    }
	  }
	}
      }
    }
    eitvXXX_swap(intern->XXX->boxize_lincons_eitv,eitv);
    if (change){
      globalchange = true;
      exc = itvXXX_canonicalize(res[dim],dim<intdim, intern);
      if (exc){
	itvXXX_set_bottom(res[0]);
	return true;
      }
    }
  }
  if (expr->size==0 &&
      ap_linconsXXX_evalcst(cons, intern)==tbool_false){
    itvXXX_set_bottom(res[0]);
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
bool ap_linconsXXX_array_boxize(itvXXX_t* res,
				bool* tchange,
				ap_linconsXXX_array_t array,
				itvXXX_t* env,size_t intdim,
				size_t kmax,
				bool intervalonly, itv_internal_t intern)
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
	if (itvXXX_is_bottom(res[0], intern)){
	  return true;
	}
      }
    }
    if (!change) break;
  }
  return globalchange;
}

/* ********************************************************************** */
/* V. Change of dimensions and permutations */
/* ********************************************************************** */

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

bool ap_linconsXXX_equal(ap_linconsXXX_t cons1,ap_linconsXXX_t cons2)
{
  return cons1->constyp==cons2->constyp &&
    (cons1->constyp==AP_CONS_EQMOD ? mpq_equal(cons1->mpq,cons2->mpq) : true) &&
    ap_linexprXXX_equal(cons1->linexpr,cons2->linexpr);
}

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linconsXXX_compare(ap_linconsXXX_t cons1, ap_linconsXXX_t cons2)
{
  int res;
  res =
    cons1->constyp > cons2->constyp ? 1 :
    (cons1->constyp==cons2->constyp ? 0 : -1);
  if (!res){
    res = ap_linexprXXX_compare(cons1->linexpr,cons2->linexpr);
    if (!res && cons1->constyp==AP_CONS_EQMOD){
      res = mpq_cmp(cons1->mpq,cons2->mpq);
    }
  }
  return res;
}

#undef _ITVXXX_MARK_
