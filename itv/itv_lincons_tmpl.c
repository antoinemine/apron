/* ********************************************************************** */
/* itv_lincons.c: */
/* ********************************************************************** */

#include "itv_lincons.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void itv_lincons_set_bool(itv_lincons_t lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  itv_linexpr_reinit(lincons->linexpr,0);
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
    num_fprint(stream,cons->num);
  }
  fflush(stream);
}

void itv_lincons_array_init(itv_lincons_array_t array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = malloc(size*sizeof(itv_lincons_t));
  for (i=0; i<size; i++) itv_lincons_init(array->p[i]);
}
void itv_lincons_array_reinit(itv_lincons_array_t array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      itv_lincons_clear(array->p[i]);
    }
    array->p = realloc(array->p,size*sizeof(itv_lincons_t));
  }
  else { /* size > array->size */
    array->p = realloc(array->p,size*sizeof(itv_lincons_t));
    for (i=array->size; i<size; i++){
      itv_lincons_init(array->p[i]);
    }
  }
  array->size = size;
  return;
}
void itv_lincons_array_clear(itv_lincons_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) itv_lincons_clear(array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}
void itv_lincons_array_fprint(FILE* stream, itv_lincons_array_t array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    itv_lincons_fprint(stream,array->p[i],name);
    fprintf(stream,"\n");
  }
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool itv_lincons_array_is_linear(itv_lincons_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itv_lincons_is_linear(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool itv_lincons_array_is_quasilinear(itv_lincons_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itv_lincons_is_quasilinear(array->p[i]);
    if (!res) break;
  }
  return res;
}

/* Evaluate a constraint, composed of a constant (interval) expression */
tbool_t itv_eval_cstlincons(itv_internal_t* intern,
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
      else if (num_cmp_int(lincons->num,0)){
	res = (bound_sgn(cst->itv->sup)==0) ? tbool_true : tbool_top;
      }
      else {
#if NUM_NUMRAT
	numrat_t numrat;
	numrat_init(numrat);
	numrat_div(numrat,bound_numref(cst->itv->sup),lincons->num);
	if (numint_cmp_int(numrat_denref(numrat),1)==0){
	  res = tbool_true;
	}
	else {
	  res = tbool_top;
	}
	numrat_clear(numrat);
#elif NUM_NUMINT
	numint_t numint;
	numint_init(numint);
	numint_mod(numint,bound_numref(cst->itv->sup),lincons->num);
	if (numint_sgn(numint)==0){
	  res = tbool_true;
	}
	else {
	  res = tbool_top;
	}
	numint_clear(numint);
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

bool itv_sat_lincons_is_false(itv_internal_t* intern,
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
      itv_eval_cstlincons(intern,lincons)==tbool_false){
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
    if (itv_eval_cstlincons(intern,lincons)!=tbool_false)
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
      tbool_t sat = itv_eval_cstlincons(intern,array->p[i]);
      if (sat==tbool_true){
      itv_lincons_array_reduce_remove:
	size --;
	itv_lincons_swap(array->p[i],array->p[size]);
	continue;
      }
      else if (sat==tbool_false){
      itv_lincons_array_reduce_false:
	itv_lincons_array_reinit(array,1);
	itv_lincons_set_bool(array->p[0],false);
	return tbool_false;
      }
    }
    if (meet && itv_lincons_is_useless_for_meet(intern,array->p[i]))
      goto itv_lincons_array_reduce_remove;
    else if (!meet && itv_sat_lincons_is_false(intern,array->p[i]))
      goto itv_lincons_array_reduce_false;
    else {
      i++;
    }
  }
  itv_lincons_array_reinit(array,size);
  if (size==0)
    res = tbool_true;
  else if (size==1 && array->p[0]->linexpr->size==0)
    res = itv_eval_cstlincons(intern,array->p[0]);
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
