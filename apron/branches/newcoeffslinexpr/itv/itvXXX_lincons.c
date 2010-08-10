/* -*- mode: c -*- */

/* ********************************************************************** */
/* itvXXX_lincons.c: */
/* ********************************************************************** */

#include "itvXXX_lincons.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

itv_lincons_ptr itv_lincons_alloc(size_t size)
{
  itv_lincons_ptr res = (itv_lincons_ptr)malloc(sizeof(itv_lincons_struct));
  itv_lincons_init(res,size);
  return res;
}
itv_lincons_ptr itv_lincons_alloc_set(itv_lincons_t cons)
{
  itv_lincons_ptr res = (itv_lincons_ptr)malloc(sizeof(itv_lincons_struct));
  itv_lincons_init_set(res,cons);
  return res;
}
itv_lincons_ptr itv_lincons_alloc_set_linexpr(itv_linexpr_t expr, itvconstyp_t constyp, mpq_ptr mpq)
{
  itv_lincons_ptr res = (itv_lincons_ptr)malloc(sizeof(itv_lincons_struct));
  itv_lincons_init_set_linexpr(res,expr,constyp,mpq);
  return res;
}
void itv_lincons_free(itv_lincons_ptr e)
{
  itv_lincons_clear(e);
  free(e);
}

void itv_lincons_set_bool(itv_lincons_t lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  itv_linexpr_resize(lincons->linexpr,0);
  eitv_set_int(lincons->linexpr->cst,value ? 0 : 1);
  lincons->constyp = ITV_CONS_EQ;
}
void itv_lincons_fprint(FILE* stream, itv_lincons_t cons, char** name)
{
  itv_linexpr_fprint(stream,cons->linexpr,name);
  fprintf(stream,
	  cons->constyp == ITV_CONS_EQ || cons->constyp == ITV_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == ITV_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == ITV_CONS_SUP ?
	     " > 0" :
	     "\"ERROR in itv_lincons_fprint\"")));
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
tbool_t itv_lincons_evalcst(itv_internal_t* intern,
			    itv_lincons_t lincons)
{
  tbool_t res;
  eitv_ptr cst = lincons->linexpr->cst;

  assert (lincons->linexpr->size==0);
  if (eitv_is_bottom(intern,cst)){
    return tbool_false;
  }
  switch (lincons->constyp){
  case ITV_CONS_EQ:
    if (cst->eq){
      int sgn = bound_sgn(cst->itv->sup);
      res = (sgn==0 ? tbool_true : tbool_false);
    }
    else {
      if (bound_sgn(cst->itv->sup)<0 ||
	  bound_sgn(cst->itv->neginf)<0)
	res = tbool_false;
      else
	res = tbool_top;
    }
    break;
  case ITV_CONS_DISEQ:
    res =
      (bound_sgn(cst->itv->neginf)<0 ||
       bound_sgn(cst->itv->sup)<0) ?
      tbool_true :
      tbool_top;
    break;
  case ITV_CONS_SUPEQ:
    if (bound_sgn(cst->itv->neginf)<=0)
      res = tbool_true;
    else if (bound_sgn(cst->itv->sup)<0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case ITV_CONS_SUP:
    if (bound_sgn(cst->itv->neginf)<0)
      res = tbool_true;
    else if (bound_sgn(cst->itv->sup)<=0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case ITV_CONS_EQMOD:
    if (cst->eq){
      if (bound_sgn(cst->itv->sup)==0){
	res = tbool_true;
      }
      else {
#if NUM_NUMRAT
	numrat_t numrat;
	numrat_init(numrat);
	num_set_mpq(numrat,lincons->mpq,intern->num);
	numrat_div(numrat,bound_numref(cst->itv->sup),numrat);
	if (numint_cmp_int(numrat_denref(numrat),1)==0){
	  res = tbool_true;
	}
	else {
	  res = tbool_false;
	}
	numrat_clear(numrat);
#elif NUM_NUMINT
	numint_t numint;
	if (mpz_cmp_ui(mpq_denref(lincons->mpq),1)!=0){
	  res = tbool_top;
	}
	else {
	  numint_init(numint);
	  num_set_mpz(numint,mpq_numref(lincons->mpq),intern->num);
	  numint_mod(numint,bound_numref(cst->itv->sup),numint);
	  if (numint_sgn(numint)==0){
	    res = tbool_true;
	  }
	  else {
	    res = tbool_false;
	  }
	  numint_clear(numint);
	}
#else
	res = tbool_top;
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

bool itv_lincons_sat_is_false(itv_internal_t* intern,
			      itv_lincons_t lincons)
{
  bool res = false;

  eitv_ptr cst = lincons->linexpr->cst;
  bool inf = bound_infty(cst->itv->neginf);

  switch (lincons->constyp){
  case ITV_CONS_EQ:
  case ITV_CONS_EQMOD:
    res = !cst->eq;
    break;
  case ITV_CONS_DISEQ:
    res = inf && bound_infty(cst->itv->sup);
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
      itv_lincons_evalcst(intern,lincons)==tbool_false){
    res = true;
  }
  return res;
}

static bool
itv_lincons_is_useless_for_meet(itv_internal_t* intern,
				itv_lincons_t lincons)
{
  bool res = false;
  eitv_ptr cst = lincons->linexpr->cst;
  if (lincons->linexpr->size==0){
    if (itv_lincons_evalcst(intern,lincons)!=tbool_false)
      res = true;
  }
  else {
    if (!cst->eq){
      bool sup = bound_infty(cst->itv->sup);
      switch (lincons->constyp){
      case ITV_CONS_EQ:
      case ITV_CONS_DISEQ:
      case ITV_CONS_EQMOD:
	{
	  bool inf = bound_infty(cst->itv->neginf);
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

void itv_lincons_reduce_integer(itv_internal_t* intern,
				itv_lincons_t cons,
				size_t intdim)
{
  itv_linexpr_ptr expr;
  size_t i;
  eitv_ptr pitv;
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
  if (!itv_linexpr_is_integer(expr,intdim))
    return;
  /* Check that there are only scalar coefficients for dimensions */
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
    if (!eitv_is_point(pitv))
      return;
  }
#if NUM_NUMRAT
  {
    /* compute lcm of denominators and gcd of numerators */
    numint_set_int(numrat_denref(intern->quasi_num),1);
    numint_set_int(numrat_numref(intern->quasi_num),0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numint_lcm(numrat_denref(intern->quasi_num),
		 numrat_denref(intern->quasi_num),
		 numrat_denref(bound_numref(pitv->itv->sup)));
      numint_gcd(numrat_numref(intern->quasi_num),
		 numrat_numref(intern->quasi_num),
		 numrat_numref(bound_numref(pitv->itv->sup)));
    }
    if (numint_sgn(numrat_numref(intern->quasi_num))==0)
      return;
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numint_divexact(numrat_numref(bound_numref(pitv->itv->sup)),
		      numrat_numref(bound_numref(pitv->itv->sup)),
		      numrat_numref(intern->quasi_num));
      numint_mul(numrat_numref(bound_numref(pitv->itv->sup)),
		 numrat_numref(bound_numref(pitv->itv->sup)),
		 numrat_denref(intern->quasi_num));
      numint_divexact(numrat_numref(bound_numref(pitv->itv->sup)),
		      numrat_numref(bound_numref(pitv->itv->sup)),
		      numrat_denref(bound_numref(pitv->itv->sup)));
      numint_set_int(numrat_denref(bound_numref(pitv->itv->sup)),
		     1);
      bound_neg(pitv->itv->neginf,pitv->itv->sup);
    }
    numrat_inv(intern->quasi_num,intern->quasi_num);
    eitv_mul_num(expr->cst,expr->cst,intern->quasi_num);
  }
#else
#if NUM_NUMFLT
  {
    /* Assuming that all coefficients are either integer,
       compute the pgcd */
    mpz_set_si(intern->reduce_lincons_gcd,0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      if (!numflt_integer(pitv->itv->sup))
	return;
      mpz_set_num(intern->reduce_lincons_mpz,pitv->itv->sup,intern->num);
      mpz_gcd(intern->reduce_lincons_gcd,
	      intern->reduce_lincons_gcd,
	      intern->reduce_lincons_mpz);
    }
    if (mpz_sgn(intern->reduce_lincons_gcd)==0 ||
	mpz_cmp_si(intern->reduce_lincons_gcd,1)==0)
      return;
    bool exact = num_set_mpz(intern->quasi_num,intern->reduce_lincons_gcd,intern->num);
    if (!exact) return;
  }
#elif NUM_NUMINT
  {
    num_set_int(intern->quasi_num,0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
      numint_gcd(intern->quasi_num,
		 intern->quasi_num,
		 bound_numref(pitv->itv->sup));
    }
    if (numint_sgn(intern->quasi_num)==0 ||
	numint_cmp_int(intern->quasi_num,1)==0)
      return;
  }
#else
#error "HERE"
#endif
  /* divide by gcd put in intern->quasi_num */
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
    eitv_div_num(pitv,pitv,intern->quasi_num);
  }
  eitv_div_num(expr->cst,expr->cst,intern->quasi_num);
#endif
  /* Constrain bounds */
  if (!bound_infty(expr->cst->itv->sup)){
    if (cons->constyp==ITV_CONS_SUP){
      if (num_integer(bound_numref(expr->cst->itv->sup))){
	bound_sub_uint(expr->cst->itv->sup,expr->cst->itv->sup,1);
      }
      else {
	num_floor(bound_numref(expr->cst->itv->sup),
		  bound_numref(expr->cst->itv->sup));
      }
      cons->constyp = ITV_CONS_SUPEQ;
    }
    else {
      num_floor(bound_numref(expr->cst->itv->sup),
		bound_numref(expr->cst->itv->sup));
    }
  }
  if (cons->constyp == ITV_CONS_EQ){
    if (!bound_infty(expr->cst->itv->neginf)){
      if (expr->cst->eq){
	bound_neg(expr->cst->itv->neginf,
		  expr->cst->itv->sup);
      }
      else {
	num_floor(bound_numref(expr->cst->itv->neginf),
		  bound_numref(expr->cst->itv->neginf));
      }
    }
    if (eitv_is_bottom(intern,expr->cst)){
      itv_lincons_set_bool(cons,false);
    }
  }
  else {
    if (!bound_infty(expr->cst->itv->sup)){
      bound_neg(expr->cst->itv->neginf,expr->cst->itv->sup);
      expr->cst->eq = true;
    }
  }
}

tbool_t itv_lincons_array_reduce(itv_internal_t* intern,
				 itv_lincons_array_t array, bool meet)
{
  tbool_t res;
  size_t i,size;

  res = tbool_top;
  i = 0;
  size = array->size;
  while (i<size){
    if (array->p[i]->linexpr->size==0){
      tbool_t sat = itv_lincons_evalcst(intern,array->p[i]);
      if (sat==tbool_true){
      itv_lincons_array_reduce_remove:
	size --;
	itv_lincons_swap(array->p[i],array->p[size]);
	continue;
      }
      else if (sat==tbool_false){
      itv_lincons_array_reduce_false:
	itv_lincons_array_resize(array,1);
	itv_lincons_set_bool(array->p[0],false);
	return tbool_false;
      }
    }
    if (meet && itv_lincons_is_useless_for_meet(intern,array->p[i]))
      goto itv_lincons_array_reduce_remove;
    else if (!meet && itv_lincons_sat_is_false(intern,array->p[i]))
      goto itv_lincons_array_reduce_false;
    else {
      i++;
    }
  }
  itv_lincons_array_resize(array,size);
  if (size==0)
    res = tbool_true;
  else if (size==1 && array->p[0]->linexpr->size==0)
    res = itv_lincons_evalcst(intern,array->p[0]);
  return res;
}

tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern,
					 itv_lincons_array_t array,
					 size_t intdim)
{
  size_t i;
  for (i=0; i<array->size; i++){
    itv_lincons_reduce_integer(intern,array->p[i],intdim);
  }
  return itv_lincons_array_reduce(intern,array,true);
}

/* ********************************************************************** */
/* IV. Boxization of interval linear expressions */
/* ********************************************************************** */

static bool itv_lincons_boxize(itv_internal_t* intern,
			       itv_t* res,
			       bool* tchange,
			       itv_lincons_t cons,
			       itv_t* env,
			       size_t intdim,
			       bool intervalonly)
{
  size_t i;
  itv_linexpr_ptr expr;
  eitv_ptr eitv;
  ap_dim_t dim;
  bool change,globalchange;
  bool exc;

  assert(cons->constyp == ITV_CONS_EQ ||
	 cons->constyp == ITV_CONS_SUPEQ ||
	 cons->constyp == ITV_CONS_SUP);

  expr = cons->linexpr;
  globalchange = false;

  /* Iterates on coefficients */
  eitv_set_int(intern->boxize_lincons_eitv,0);
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    bool equality = eitv->eq;
    /* 1. We decompose the expression e = ax+e' */
    eitv_swap(intern->boxize_lincons_eitv,eitv);
    /* 2. evaluate e' */
    itv_linexpr_eval(intern,
		     intern->boxize_lincons_eval,expr,env);
    /* 3. Perform deductions */
    change = false;
    if (!itv_is_top(intern->boxize_lincons_eval)){
      if (equality && !intervalonly){
	int sgn = bound_sgn(intern->boxize_lincons_eitv->itv->sup);
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
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_eitv->itv->sup);
	    }
	    else {
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->neginf,
			intern->boxize_lincons_eitv->itv->neginf);
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  num_integer(bound_numref(intern->boxize_lincons_bound))){
		num_sub_uint(bound_numref(intern->boxize_lincons_bound),
			     bound_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		bound_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      bound_set(res[dim]->neginf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == ITV_CONS_EQ){
	    /*
	      If we have a<0, we compute sup(e')/(-a)=sup(e'/-a)
	      If we have a>0, we compute -inf(e')/a=-inf(e'/a)=sup(e'/-a)
	    */
	    if (sgn<0){
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_eitv->itv->neginf);
	    }
	    else {
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->neginf,
			intern->boxize_lincons_eitv->itv->sup);
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  num_integer(bound_numref(intern->boxize_lincons_bound))){
		num_sub_uint(bound_numref(intern->boxize_lincons_bound),
			     bound_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		bound_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      bound_set(res[dim]->sup, intern->boxize_lincons_bound);
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
	  bound_sgn(intern->boxize_lincons_eitv->itv->neginf)<0 ?
	  1 :
	  ( bound_sgn(intern->boxize_lincons_eitv->itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgnitv != 0){
	  int sgnevalinf = bound_sgn(intern->boxize_lincons_eval->neginf);
	  int sgnevalsup = bound_sgn(intern->boxize_lincons_eval->sup);
	  if (sgnitv>0 || (cons->constyp==ITV_CONS_EQ && sgnitv<0)){
	    if (sgnitv>0){
	      if (sgnevalsup<=0){
		/* We compute sup(e')/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_eitv->itv->sup);
	      } else {
		/* We compute sup(e')/m = (-sup(e'))/(-m) */
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup);
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf<=0){
		/* We compute inf(e')/m = (-inf(e'))/(-m) */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_eitv->itv->neginf);
	      } else {
		/* We compute inf(e')/M) = (-inf(e'))/(-M) */
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->sup);
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_bound);
	      }
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  num_integer(bound_numref(intern->boxize_lincons_bound))){
		num_sub_uint(bound_numref(intern->boxize_lincons_bound),
			     bound_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		bound_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->neginf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      bound_set(res[dim]->neginf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgnitv<0 || (cons->constyp==ITV_CONS_EQ && sgnitv>0)){
	    if (sgnitv<0){
	      if (sgnevalsup>=0){
		/* We compute sup(e')/-M */
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->sup);
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_bound);
	      } else {
		/* We compute sup(e')/-m */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    else {
	      if (sgnevalinf>=0){
		/* We compute -inf(e')/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf,
			  intern->boxize_lincons_eitv->itv->sup);
	      }
	      else {
		/* We compute -inf(e')/m = inf(e')/(-m) */
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->neginf);
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound,
			  intern->boxize_lincons_eitv->itv->neginf);
	      }
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==ITV_CONS_SUP &&
		  num_integer(bound_numref(intern->boxize_lincons_bound))){
		num_sub_uint(bound_numref(intern->boxize_lincons_bound),
			     bound_numref(intern->boxize_lincons_bound),
			     1);
	      }
	      else {
		bound_floor(intern->boxize_lincons_bound,
			    intern->boxize_lincons_bound);
	      }
	    }
	    /* We update the interval */
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->sup)<0){
	      change = true;
	      if (tchange) tchange[2*dim+1] = true;
	      bound_set(res[dim]->sup, intern->boxize_lincons_bound);
	    }
	  }
	}
      }
    }
    eitv_swap(intern->boxize_lincons_eitv,eitv);
    if (change){
      globalchange = true;
      exc = itv_canonicalize(intern,res[dim],dim<intdim);
      if (exc){
	itv_set_bottom(res[0]);
	return true;
      }
    }
  }
  if (expr->size==0 &&
      itv_lincons_evalcst(intern,cons)==tbool_false){
    itv_set_bottom(res[0]);
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
bool itv_lincons_array_boxize(itv_internal_t* intern,
			      itv_t* res,
			      bool* tchange,
			      itv_lincons_array_t array,
			      itv_t* env,size_t intdim,
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
	  itv_lincons_boxize(intern,res,tchange,array->p[i],env,intdim,intervalonly)
	  ||
	  change
	  ;
	globalchange = globalchange || change;
	if (itv_is_bottom(intern,res[0])){
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

bool itv_lincons_equal(itv_lincons_t cons1,itv_lincons_t cons2)
{
  return cons1->constyp==cons2->constyp &&
    (cons1->constyp==ITV_CONS_EQMOD ? mpq_equal(cons1->mpq,cons2->mpq) : true) &&
    itv_linexpr_equal(cons1->linexpr,cons2->linexpr);
}

/* Lexicographic ordering, terminating by constant coefficients */
int itv_lincons_compare(itv_lincons_t cons1, itv_lincons_t cons2)
{
  int res;
  res =
    cons1->constyp > cons2->constyp ? 1 :
    (cons1->constyp==cons2->constyp ? 0 : -1);
  if (!res){
    res = itv_linexpr_compare(cons1->linexpr,cons2->linexpr);
    if (!res && cons1->constyp==ITV_CONS_EQMOD){
      res = mpq_cmp(cons1->mpq,cons2->mpq);
    }
  }
  return res;
}
