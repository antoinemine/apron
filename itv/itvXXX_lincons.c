/* -*- mode: c -*- */

/* ********************************************************************** */
/* itvXXX_lincons.c: */
/* ********************************************************************** */

#include "itvXXX_lincons.h"

#define _ITVXXX_MARK_

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

itvXXX_lincons_ptr itvXXX_lincons_alloc(size_t size)
{
  itvXXX_lincons_ptr res = (itvXXX_lincons_ptr)malloc(sizeof(itvXXX_lincons_struct));
  itvXXX_lincons_init(res,size);
  return res;
}
itvXXX_lincons_ptr itvXXX_lincons_alloc_set(itvXXX_lincons_t cons)
{
  itvXXX_lincons_ptr res = (itvXXX_lincons_ptr)malloc(sizeof(itvXXX_lincons_struct));
  itvXXX_lincons_init_set(res,cons);
  return res;
}
itvXXX_lincons_ptr itvXXX_lincons_alloc_set_linexpr(itvXXX_linexpr_t expr, itvconstyp_t constyp, mpq_ptr mpq)
{
  itvXXX_lincons_ptr res = (itvXXX_lincons_ptr)malloc(sizeof(itvXXX_lincons_struct));
  itvXXX_lincons_init_set_linexpr(res,expr,constyp,mpq);
  return res;
}
void itvXXX_lincons_free(itvXXX_lincons_ptr e)
{
  itvXXX_lincons_clear(e);
  free(e);
}

void itvXXX_lincons_set_bool(itvXXX_lincons_t lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  itvXXX_linexpr_resize(lincons->linexpr,0);
  eitvXXX_set_int(lincons->linexpr->cst,value ? 0 : 1);
  lincons->constyp = ITV_CONS_EQ;
}
void itvXXX_lincons_fprint(FILE* stream, itvXXX_lincons_t cons, char** name)
{
  itvXXX_linexpr_fprint(stream,cons->linexpr,name);
  fprintf(stream,
	  cons->constyp == ITV_CONS_EQ || cons->constyp == ITV_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == ITV_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == ITV_CONS_SUP ?
	     " > 0" :
	     "\"ERROR in itvXXX_lincons_fprint\"")));
  if (cons->constyp == ITV_CONS_EQMOD){
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
tbool_t itvXXX_lincons_evalcst(itvXXX_internal_t intern,
			       itvXXX_lincons_t lincons)
{
  tbool_t res;
  eitvXXX_ptr cst = lincons->linexpr->cst;

  assert (lincons->linexpr->size==0);
  if (eitvXXX_is_bottom(intern,cst)){
    return tbool_false;
  }
  switch (lincons->constyp){
  case ITV_CONS_EQ:
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
  case ITV_CONS_DISEQ:
    res =
      (boundXXX_sgn(cst->itv->neginf)<0 ||
       boundXXX_sgn(cst->itv->sup)<0) ?
      tbool_true :
      tbool_top;
    break;
  case ITV_CONS_SUPEQ:
    if (boundXXX_sgn(cst->itv->neginf)<=0)
      res = tbool_true;
    else if (boundXXX_sgn(cst->itv->sup)<0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case ITV_CONS_SUP:
    if (boundXXX_sgn(cst->itv->neginf)<0)
      res = tbool_true;
    else if (boundXXX_sgn(cst->itv->sup)<=0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case ITV_CONS_EQMOD:
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

bool itvXXX_lincons_sat_is_false(itvXXX_internal_t intern,
				 itvXXX_lincons_t lincons)
{
  bool res = false;

  eitvXXX_ptr cst = lincons->linexpr->cst;
  bool inf = boundXXX_infty(cst->itv->neginf);

  switch (lincons->constyp){
  case ITV_CONS_EQ:
  case ITV_CONS_EQMOD:
    res = !cst->eq;
    break;
  case ITV_CONS_DISEQ:
    res = inf && boundXXX_infty(cst->itv->sup);
    break;
  case ITV_CONS_SUPEQ:
  case ITV_CONS_SUP:
    res = inf;
    break;
  default:
    break;
  }
  if (!res &&
      lincons->linexpr->size==0 &&
      itvXXX_lincons_evalcst(intern,lincons)==tbool_false){
    res = true;
  }
  return res;
}

static bool
itvXXX_lincons_is_useless_for_meet(itvXXX_internal_t intern,
				   itvXXX_lincons_t lincons)
{
  bool res = false;
  eitvXXX_ptr cst = lincons->linexpr->cst;
  if (lincons->linexpr->size==0){
    if (itvXXX_lincons_evalcst(intern,lincons)!=tbool_false)
      res = true;
  }
  else {
    if (!cst->eq){
      bool sup = boundXXX_infty(cst->itv->sup);
      switch (lincons->constyp){
      case ITV_CONS_EQ:
      case ITV_CONS_DISEQ:
      case ITV_CONS_EQMOD:
	{
	  bool inf = boundXXX_infty(cst->itv->neginf);
	  res = inf && sup;
	}
	break;
      case ITV_CONS_SUPEQ:
      case ITV_CONS_SUP:
	res = sup;
	break;
      default:
	break;
      }
    }
  }
  return res;
}

void itvXXX_lincons_reduce_integer(itvXXX_internal_t intern,
				   itvXXX_lincons_t cons,
				   size_t intdim)
{
  itvXXX_linexpr_ptr expr;
  size_t i;
  eitvXXX_ptr pitv;
  ap_dim_t dim;
  bool integer;

  switch (cons->constyp){
  case ITV_CONS_EQ:
  case ITV_CONS_SUPEQ:
  case ITV_CONS_SUP:
    break;
  default:
    return;
  }
  expr = cons->linexpr;
  /* Tests if only integer variables are involved */
  if (!itvXXX_linexpr_is_integer(expr,intdim))
    return;
  /* Check that there are only scalar coefficients for dimensions */
  itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
    if (!eitvXXX_is_point(pitv))
      return;
  }
#if defined(_ITVRl_MARK_) || defined(_ITVRll_MARK_) || defined(_ITVMPQ_MARK_)
  {
    /* compute lcm of denominators and gcd of numerators */
    numintXXX_set_int(numXXX_denref(intern->quasi_num),1);
    numintXXX_set_int(numXXX_numref(intern->quasi_num),0);
    itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numintXXX_lcm(numXXX_denref(intern->quasi_num),
		    numXXX_denref(intern->quasi_num),
		    numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_gcd(numXXX_numref(intern->quasi_num),
		    numXXX_numref(intern->quasi_num),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)));
    }
    if (numintXXX_sgn(numXXX_numref(intern->quasi_num))==0)
      return;
    itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(intern->quasi_num));
      numintXXX_mul(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_numref(boundXXX_numref(pitv->itv->sup)),
		    numXXX_denref(intern->quasi_num));
      numintXXX_divexact(numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_numref(boundXXX_numref(pitv->itv->sup)),
			 numXXX_denref(boundXXX_numref(pitv->itv->sup)));
      numintXXX_set_int(numXXX_denref(boundXXX_numref(pitv->itv->sup)),
			1);
      boundXXX_neg(pitv->itv->neginf,pitv->itv->sup);
    }
    numXXX_inv(intern->quasi_num,intern->quasi_num);
    eitvXXX_mul_num(expr->cst,expr->cst,intern->quasi_num);
  }
#else
#if defined(_ITVD_MARK_) || defined(_ITVDl_MARK_) || defined(_ITVMPFR_MARK_)
  {
    /* Assuming that all coefficients are either integer,
       compute the pgcd */
    mpz_set_si(intern->reduce_lincons_gcd,0);
    itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      if (!numXXX_integer(pitv->itv->sup))
	return;
      mpz_set_numXXX(intern->reduce_lincons_mpz,pitv->itv->sup,intern->num);
      mpz_gcd(intern->reduce_lincons_gcd,
	      intern->reduce_lincons_gcd,
	      intern->reduce_lincons_mpz);
    }
    if (mpz_sgn(intern->reduce_lincons_gcd)==0 ||
	mpz_cmp_si(intern->reduce_lincons_gcd,1)==0)
      return;
    bool exact = numXXX_set_mpz(intern->quasi_num,intern->reduce_lincons_gcd,intern->num);
    if (!exact) return;
  }
#elif defined(_ITVIl_MARK_) || defined(_ITVIll_MARK_) || defined(_ITVMPZ_MARK_)
  {
    numXXX_set_int(intern->quasi_num,0);
    itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numXXX_gcd(intern->quasi_num,
		 intern->quasi_num,
		 boundXXX_numref(pitv->itv->sup));
    }
    if (numXXX_sgn(intern->quasi_num)==0 ||
	numXXX_cmp_int(intern->quasi_num,1)==0)
      return;
  }
#else
#error "HERE"
#endif
  /* divide by gcd put in intern->quasi_num */
  itvXXX_linexpr_ForeachLinterm(expr,i,dim,pitv) {
    eitvXXX_div_num(pitv,pitv,intern->quasi_num);
  }
  eitvXXX_div_num(expr->cst,expr->cst,intern->quasi_num);
#endif
  /* Constrain bounds */
  if (!boundXXX_infty(expr->cst->itv->sup)){
    if (cons->constyp==ITV_CONS_SUP){
      if (numXXX_integer(boundXXX_numref(expr->cst->itv->sup))){
	boundXXX_sub_uint(expr->cst->itv->sup,expr->cst->itv->sup,1);
      }
      else {
	numXXX_floor(boundXXX_numref(expr->cst->itv->sup),
		     boundXXX_numref(expr->cst->itv->sup));
      }
      cons->constyp = ITV_CONS_SUPEQ;
    }
    else {
      numXXX_floor(boundXXX_numref(expr->cst->itv->sup),
		   boundXXX_numref(expr->cst->itv->sup));
    }
  }
  if (cons->constyp == ITV_CONS_EQ){
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
    if (eitvXXX_is_bottom(intern,expr->cst)){
      itvXXX_lincons_set_bool(cons,false);
    }
  }
  else {
    if (!boundXXX_infty(expr->cst->itv->sup)){
      boundXXX_neg(expr->cst->itv->neginf,expr->cst->itv->sup);
      expr->cst->eq = true;
    }
  }
}

tbool_t itvXXX_lincons_array_reduce(itvXXX_internal_t intern,
				    itvXXX_lincons_array_t array, bool meet)
{
  tbool_t res;
  size_t i,size;

  res = tbool_top;
  i = 0;
  size = array->size;
  while (i<size){
    if (array->p[i]->linexpr->size==0){
      tbool_t sat = itvXXX_lincons_evalcst(intern,array->p[i]);
      if (sat==tbool_true){
      itvXXX_lincons_array_reduce_remove:
	size --;
	itvXXX_lincons_swap(array->p[i],array->p[size]);
	continue;
      }
      else if (sat==tbool_false){
      itvXXX_lincons_array_reduce_false:
	itvXXX_lincons_array_resize(array,1);
	itvXXX_lincons_set_bool(array->p[0],false);
	return tbool_false;
      }
    }
    if (meet && itvXXX_lincons_is_useless_for_meet(intern,array->p[i]))
      goto itvXXX_lincons_array_reduce_remove;
    else if (!meet && itvXXX_lincons_sat_is_false(intern,array->p[i]))
      goto itvXXX_lincons_array_reduce_false;
    else {
      i++;
    }
  }
  itvXXX_lincons_array_resize(array,size);
  if (size==0)
    res = tbool_true;
  else if (size==1 && array->p[0]->linexpr->size==0)
    res = itvXXX_lincons_evalcst(intern,array->p[0]);
  return res;
}

tbool_t itvXXX_lincons_array_reduce_integer(itvXXX_internal_t intern,
					 itvXXX_lincons_array_t array,
					 size_t intdim)
{
  size_t i;
  for (i=0; i<array->size; i++){
    itvXXX_lincons_reduce_integer(intern,array->p[i],intdim);
  }
  return itvXXX_lincons_array_reduce(intern,array,true);
}

bool itvXXX_lincons_array_quasilinearize(itvXXX_internal_t intern,
					 itvXXX_lincons_array_t array, itvXXX_t* env,
					 bool for_meet_inequality)
{
  size_t i;
  bool res;
  res = true;
  for (i=0; i<array->size; i++) {
    itvXXX_lincons_quasilinearize(intern,array->p[i],env,for_meet_inequality);
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

static bool itvXXX_lincons_boxize(itvXXX_internal_t intern,
				  itvXXX_t* res,
				  bool* tchange,
				  itvXXX_lincons_t cons,
				  itvXXX_t* env,
				  size_t intdim,
				  bool intervalonly)
{
  size_t i;
  itvXXX_linexpr_ptr expr;
  eitvXXX_ptr eitv;
  ap_dim_t dim;
  bool change,globalchange;
  bool exc;

  assert(cons->constyp == ITV_CONS_EQ ||
	 cons->constyp == ITV_CONS_SUPEQ ||
	 cons->constyp == ITV_CONS_SUP);

  expr = cons->linexpr;
  globalchange = false;

  /* Iterates on coefficients */
  eitvXXX_set_int(intern->boxize_lincons_eitv,0);
  itvXXX_linexpr_ForeachLinterm(expr,i,dim,eitv){
    bool equality = eitv->eq;
    /* 1. We decompose the expression e = ax+e' */
    eitvXXX_swap(intern->boxize_lincons_eitv,eitv);
    /* 2. evaluate e' */
    itvXXX_linexpr_eval(intern,
			intern->boxize_lincons_eval,expr,env);
    /* 3. Perform deductions */
    change = false;
    if (!itvXXX_is_top(intern->boxize_lincons_eval)){
      if (equality && !intervalonly){
	int sgn = boundXXX_sgn(intern->boxize_lincons_eitv->itv->sup);
	if (sgn!=0){
	  /*
	    If we have ax+e' >= 0 with a>0
	    we can deduce that x>=-e'/a, or x>= inf(-e'/a)
	    If we have ax+e' >= 0 with a<0
	    we can deduce that -ax<=e', or x<= sup(e'/-a)
	    If we have ax+e'=0
	    we can deduce x=-e'/a, or inf(-e'/a)<= x <= sup(-e'/a)
	  */
	  if (sgn>0 || cons->constyp == ITV_CONS_EQ){
	    /*
	      If we have a>0, we compute sup(e')/a=sup(e'/a)=-inf(-e'/a)
	      If we have a<0, we compute -inf(e')/(-a)=-inf(-e'/a)
	    */
	    if (sgn>0){
	      boundXXX_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_eitv->itv->sup);
	    }
	    else {
	      boundXXX_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->neginf,
			intern->boxize_lincons_eitv->itv->neginf);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->boxize_lincons_bound),
			     boundXXX_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(res[dim]->neginf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == ITV_CONS_EQ){
	    /*
	      If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	      If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      boundXXX_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_eitv->itv->neginf);
	    }
	    else {
	      boundXXX_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->neginf,
			intern->boxize_lincons_eitv->itv->sup);
	    }
	    if (dim<intdim && !boundXXX_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->boxize_lincons_bound),
			     boundXXX_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(res[dim]->sup, intern->boxize_lincons_bound);
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
	  boundXXX_sgn(intern->boxize_lincons_eitv->itv->neginf)<0 ?
	  1 :
	  ( boundXXX_sgn(intern->boxize_lincons_eitv->itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgnitv != 0){
	  int sgnevalinf = boundXXX_sgn(intern->boxize_lincons_eval->neginf);
	  int sgnevalsup = boundXXX_sgn(intern->boxize_lincons_eval->sup);
	  if (sgnitv>0 || (cons->constyp==ITV_CONS_EQ && sgnitv<0)){
	    if (sgnitv>0){
	      if (sgnevalsup<=0){
		/* We compute sup(e')/M */
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_eitv->itv->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		boundXXX_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup);
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_eitv->itv->neginf);
	      } else {
		/* We compute inf(e')/M) = (-inf(e'))/(-M) */
		boundXXX_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_bound);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->boxize_lincons_bound),
			     boundXXX_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      boundXXX_set(res[dim]->neginf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgnitv<0 || (cons->constyp==ITV_CONS_EQ && sgnitv>0)){
	    if (sgnitv<0){
	      if (sgnevalsup>=0){
		/* We compute sup(e')/-M */
		boundXXX_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->sup);
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_bound);
	      } else {
		/* We compute sup(e')/-m */
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf>=0){
		/* We compute -inf(e')/M */
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_eitv->itv->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		boundXXX_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf);
		boundXXX_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    if (dim<intdim && !boundXXX_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  numXXX_integer(boundXXX_numref(intern->boxize_lincons_bound))){
		numXXX_sub_uint(boundXXX_numref(intern->boxize_lincons_bound),
			     boundXXX_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		boundXXX_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (boundXXX_cmp(intern->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      boundXXX_set(res[dim]->sup, intern->boxize_lincons_bound);
	    }
	  }
	}
      }
    }
    eitvXXX_swap(intern->boxize_lincons_eitv,eitv);
    if (change){
      globalchange = true;
      exc = itvXXX_canonicalize(intern,res[dim],dim<intdim);
      if (exc){
	itvXXX_set_bottom(res[0]);
	return true;
      }
    }
  }
  if (expr->size==0 &&
      itvXXX_lincons_evalcst(intern,cons)==tbool_false){
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
bool itvXXX_lincons_array_boxize(itvXXX_internal_t intern,
				 itvXXX_t* res,
				 bool* tchange,
				 itvXXX_lincons_array_t array,
				 itvXXX_t* env,size_t intdim,
				 size_t kmax,
				 bool intervalonly)
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
      if (array->p[i]->constyp==ITV_CONS_EQ ||
	  array->p[i]->constyp==ITV_CONS_SUPEQ ||
	  array->p[i]->constyp==ITV_CONS_SUP){
	change =
	  itvXXX_lincons_boxize(intern,res,tchange,array->p[i],env,intdim,intervalonly)
	  ||
	  change
	  ;
	globalchange = globalchange || change;
	if (itvXXX_is_bottom(intern,res[0])){
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

bool itvXXX_lincons_equal(itvXXX_lincons_t cons1,itvXXX_lincons_t cons2)
{
  return cons1->constyp==cons2->constyp &&
    (cons1->constyp==ITV_CONS_EQMOD ? mpq_equal(cons1->mpq,cons2->mpq) : true) &&
    itvXXX_linexpr_equal(cons1->linexpr,cons2->linexpr);
}

/* Lexicographic ordering, terminating by constant coefficients */
int itvXXX_lincons_compare(itvXXX_lincons_t cons1, itvXXX_lincons_t cons2)
{
  int res;
  res =
    cons1->constyp > cons2->constyp ? 1 :
    (cons1->constyp==cons2->constyp ? 0 : -1);
  if (!res){
    res = itvXXX_linexpr_compare(cons1->linexpr,cons2->linexpr);
    if (!res && cons1->constyp==ITV_CONS_EQMOD){
      res = mpq_cmp(cons1->mpq,cons2->mpq);
    }
  }
  return res;
}

#undef _ITVXXX_MARK_
