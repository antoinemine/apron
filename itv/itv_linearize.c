/* ********************************************************************** */
/* itv_linearize.c: */
/* ********************************************************************** */

#include "itv_linearize.h"

#define LOGDEBUG 0

/* ********************************************************************** */
/* I. Evaluation of interval linear expressions  */
/* ********************************************************************** */

/* Evaluate an ITV interval linear expression */
void ITVFUN(itv_eval_linexpr)(itv_internal_t* intern,
			      itv_t itv,
			      itv_linexpr_t* expr,
			      itv_t* env)
{
  size_t i;
  ap_dim_t dim;
  itv_ptr pitv;
  bool* peq;
  assert(env);

  itv_set(intern->eval_itv2, expr->cst);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    if (*peq){
      if (bound_sgn(pitv->sup)!=0){
	itv_mul_bound(intern->eval_itv,
		      env[dim],
		      pitv->sup);
	itv_add(intern->eval_itv2, intern->eval_itv2, intern->eval_itv);
      }
    }
    else {
      itv_mul(intern,
	      intern->eval_itv,
	      env[dim],
	      pitv);
      itv_add(intern->eval_itv2, intern->eval_itv2, intern->eval_itv);
    }
    if (itv_is_top(intern->eval_itv2))
      break;
  }
  itv_set(itv,intern->eval_itv2);
}

/* Evaluate an APRON interval linear expression */
bool ITVFUN(itv_eval_ap_linexpr0)(itv_internal_t* intern,
				  itv_t itv,
				  ap_linexpr0_t* expr,
				  itv_t* env)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t* pcoeff;
  bool exact,res;
  assert(env);

  exact = itv_set_ap_coeff(intern, intern->eval_itv3, &expr->cst);
  res = exact;
  ap_linexpr0_ForeachLinterm(expr,i,dim,pcoeff){
    exact = itv_set_ap_coeff(intern,intern->eval_itv2,pcoeff);
    res = res && exact;
    bool eq = exact && pcoeff->discr==AP_COEFF_SCALAR;
    if (eq){
      if (bound_sgn(intern->eval_itv2->sup)!=0){
	itv_mul_bound(intern->eval_itv,
		      env[dim],
		      intern->eval_itv2->sup);
	itv_add(intern->eval_itv3, intern->eval_itv3, intern->eval_itv);
      }
    }
    else {
      itv_mul(intern,
	      intern->eval_itv,
	      env[dim],
	      intern->eval_itv2);
      itv_add(intern->eval_itv3, intern->eval_itv3, intern->eval_itv);
    }
    if (itv_is_top(intern->eval_itv3))
      break;
  }
  itv_set(itv,intern->eval_itv3);
  return res;
}

/* ********************************************************************** */
/* II. Boxization of interval linear expressions */
/* ********************************************************************** */

static bool itv_boxize_lincons(itv_internal_t* intern,
			       itv_t* res,
			       bool* tchange,
			       itv_lincons_t* cons,
			       itv_t* env,
			       size_t intdim,
			       bool intervalonly,
                               bool* empty)
{
  size_t i;
  itv_linexpr_t* expr;
  bool change,globalchange;
  bool exc;

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  expr = &cons->linexpr;
  globalchange = false;
  *empty = false;

  /* Iterates on coefficients */
  itv_set_int(intern->boxize_lincons_itv,0);
  for (i=0; i<expr->size; i++){
    ap_dim_t dim = expr->linterm[i].dim;
    bool equality = expr->linterm[i].equality;
    /* 1. We decompose the expression e = ax+e' */
    itv_swap(intern->boxize_lincons_itv,expr->linterm[i].itv);
    expr->linterm[i].equality = true;
    /* 2. evaluate e' */
    itv_eval_linexpr(intern,
		     intern->boxize_lincons_eval,expr,env);
    /* 3. Perform deduction from ax+e' = [-m,M]x + [-e,E] >= 0
	  we can deduce [-m,M]x + E >= 0
	  (for equality, [-m,M]x - e <= 0)
    */
    change = false;
    if (!itv_is_top(intern->boxize_lincons_eval)){
      if (equality && !intervalonly){
	/* [-m,M]=[a,a] */
	int sgn = bound_sgn(intern->boxize_lincons_itv->sup);
	if (sgn!=0){
	  /* From ax+E >= 0, we deduce
	     (1) If a>0, then x>=-E/a
	     (2) If a<0, then x<=-E/a
	     From ax-e <= 0, we deduce
	     (3) If a>0, then x<=e/a
	     (4) If a<0, then x>=e/a
	  */
	  if (sgn>0 || cons->constyp == AP_CONS_EQ){
	    /* 1,4: inf bound
	       If we have a>0 (1), we compute E/a
	       If we have a<0 (4), we compute -e/a
	    */
	    if (sgn>0){
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_itv->sup);
	    }
	    else {
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->inf,
			intern->boxize_lincons_itv->inf);
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
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
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->inf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      bound_set(res[dim]->inf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgn<0 || cons->constyp == AP_CONS_EQ){
	    /* 2,3: sup bound
	       If we have a<0 (2), we compute -E/a
	       If we have a>0 (3), we compute e/a
	    */
	    if (sgn<0){
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->sup,
			intern->boxize_lincons_itv->inf);
	    }
	    else {
	      bound_div(intern->boxize_lincons_bound,
			intern->boxize_lincons_eval->inf,
			intern->boxize_lincons_itv->sup);
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
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
	/* We have a real interval [-m,M] */
	/* Case [-m,M]x+E >= 0
	  (1) If -m>0, we rewrite [-m,M]x >= -E, and we have -1/m > 1/M > 0
	  (1A) If E<=0 <=> -E>=0 then x >= -E/M
	  (1B) If E>0  <=> -E<0  then x >= E/m
	  (2) If M<0, we rewrite [-M,m]x <= E, and we have 0 < 1/m < -1/M
	  (2A) If E<=0           then x <= E/m
	  (2B) If E>0            then x <= -E/M
	  Case [-m,M]x-e <= 0
	  (3) If -m>0, we rewrite [-m,M]x <= e, and we have 0 < 1/M < -1/m
	  (3A) If e<=0           then x <= e/M
	  (3B) If e>0            then x <= -e/m
	  (4) If M<0, we rewrite [-M,m]x >= -e, and we have -1/M > 1/m > 0
	  (4A) If e<=0 <=> -e>=0 then x >= -e/m
	  (4B) If e>0  <=> -e<0  then x >= e/M
	*/
	int sgnitv =
	  bound_sgn(intern->boxize_lincons_itv->inf)<0 ?
	  1 :
	  ( bound_sgn(intern->boxize_lincons_itv->sup)<0 ?
	    -1 :
	    0 );
	if (sgnitv != 0){
	  int sgne = bound_sgn(intern->boxize_lincons_eval->inf);
	  int sgnE = bound_sgn(intern->boxize_lincons_eval->sup);
	  if (sgnitv>0 || (cons->constyp==AP_CONS_EQ && sgnitv<0)){
	    /* 1,4: inf bound */
	    if (sgnitv>0){ /* 1 */
	      if (sgnE<=0){ /* 1A */
		/* We compute E/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_itv->sup);
	      } else { /* 1B */
		/* We compute -E/m */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_itv->inf);
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound);
	      }
	    }
	    else { /* 4 */
	      if (sgne<=0){ /* 4A */
		/* We compute e/m */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->inf,
			  intern->boxize_lincons_itv->inf);
	      } else { /* 4B */
		/* We compute -e/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->inf,
			  intern->boxize_lincons_itv->sup);
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound);
	      }
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
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
	    if (bound_cmp(intern->boxize_lincons_bound, res[dim]->inf)<0){
	      change = true;
	      if (tchange) tchange[2*dim] = true;
	      bound_set(res[dim]->inf, intern->boxize_lincons_bound);
	    }
	  }
	  if (sgnitv<0 || (cons->constyp==AP_CONS_EQ && sgnitv>0)){
	    /* 2,3: sup bound */
	    if (sgnitv<0){ /* 2 */
	      if (sgnE>=0){ /* 2B */
		/* We compute -E/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_itv->sup);
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound);
	      } else { /* 2A */
		/* We compute E/m */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->sup,
			  intern->boxize_lincons_itv->inf);
	      }
	    }
	    else { /* 3 */
	      if (sgne>=0){ /* 3B */
		/* We compute -e/m */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->inf,
			  intern->boxize_lincons_itv->inf);
		bound_neg(intern->boxize_lincons_bound,
			  intern->boxize_lincons_bound);
	      }
	      else { /* 3A */
		/* We compute e/M */
		bound_div(intern->boxize_lincons_bound,
			  intern->boxize_lincons_eval->inf,
			  intern->boxize_lincons_itv->sup);
	      }
	    }
	    if (dim<intdim && !bound_infty(intern->boxize_lincons_bound)){
	      if (cons->constyp==AP_CONS_SUP &&
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
    itv_swap(intern->boxize_lincons_itv,expr->linterm[i].itv);
    expr->linterm[i].equality = equality;
    if (change){
      globalchange = true;
      exc = itv_canonicalize(intern,res[dim],dim<intdim);
      if (exc){
        *empty = true;
	return true;
      }
    }
  }
  if (expr->size==0 &&
      itv_eval_cstlincons(intern,cons)==tbool_false){
    *empty = true;
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
bool ITVFUN(itv_boxize_lincons_array)(itv_internal_t* intern,
				      itv_t* res,
				      bool* tchange,
				      itv_lincons_array_t* array,
				      itv_t* env,size_t intdim,
				      size_t kmax,
				      bool intervalonly,
                                      bool *empty)
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
      if (array->p[i].constyp==AP_CONS_EQ ||
	  array->p[i].constyp==AP_CONS_SUPEQ ||
	  array->p[i].constyp==AP_CONS_SUP){
	change =
	  itv_boxize_lincons(intern,res,tchange,&array->p[i],env,intdim,intervalonly,empty)
	  ||
	  change
	  ;
	globalchange = globalchange || change;
	if (*empty){
	  return true;
	}
      }
    }
    if (!change) break;
  }
  return globalchange;
}



/* ********************************************************************** */
/* III. (Quasi)linearisation of interval linear expressions */
/* ********************************************************************** */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by deducing
   things. If constraints are quasilinearized for testing satisfaction, meet
   should be set to false.
*/


/* Choose the middle of interval coefficient coeff for quasilinearisation */
/* Applies the following choice:

  if coeff=[-oo,+oo], choose 0
  if coeff=[-oo,x], choose x
  if coeff=[x,+oo], choose x
  if coeff = [inf,sup]
    if for_meet_inequality,
      (* always choose in favour of a finite sup bound in the constant
	 of the quasilinear expression *)
      if var=[-oo,a]
	choose inf,
	because it gives inf.var + [0,sup-inf].[-oo,a] = inf.var + [-oo,b]
      if var=[a,+oo]
	choose sup,
	because it gives sup.var + [inf-sup,0].[a,+oo] = sup.var + [-oo,b]
      if var=[a,b], choose middle
    else
      (* always choose in favour of at least a finite bound in the evaluation
	 of the quasilinear expression *)
      if var=[-oo,a]
	if inf >= 0, choose inf,
	  because inf.var + [0,sup-inf][-oo,a] evaluates to a finite sup bound
	if sup<=0, choose sup
	  because sup.var + [inf-sup,0][-oo,a] evaluates to a finite inf bound
	otherwise arbitrary choice (middle)
      if var=[a,+oo]
	if inf >= 0, choose inf,
	  because inf.var + [0,sup-inf][a,+oo] evaluates to a finite inf bound
	if sup <= 0, choose sup,
	  because sup.var + [inf-sup,0][a,+oo] evaluates to a finite sup bound
	otherwise arbitrary choice (middle)
      if var=[a,b], choose middle
*/

static void
itv_quasilinearize_choose_middle(num_t middle, /* the result */
				 itv_t coeff,    /* the coefficient in which
						    middle is to be picked */
				 itv_t var,      /* the variable interval */
				 bool for_meet_inequality /* is it for the
							     linearisation of
							     an inequality ? */
				 )
{
  if (bound_infty(coeff->inf)){
    if (bound_infty(coeff->sup))
      num_set_int(middle,0);
    else
      num_set(middle,
	      bound_numref(coeff->sup));
  }
  else if (bound_infty(coeff->sup))
    num_neg(middle,
	    bound_numref(coeff->inf));
  else {
    /* if coeff = [inf,sup] */
    if (for_meet_inequality){
      if (bound_infty(var->inf))
	num_neg(middle,
		bound_numref(coeff->inf));
      else if (bound_infty(var->sup))
	num_set(middle,
		bound_numref(coeff->sup));
      else /* Arbitrary choice: we take the middle */
	goto itv_quasilinearize_choose_middle_default;
    }
    else {
      if (bound_infty(var->inf) ?
	  !bound_infty(var->sup) :
	  bound_infty(var->sup)){
	if (bound_sgn(coeff->inf)<=0)
	  num_neg(middle,
		  bound_numref(coeff->inf));
	else if (bound_sgn(coeff->sup)<=0)
	  num_set(middle,
		  bound_numref(coeff->sup));
	else /* Arbitrary choice: we take the middle */
	  goto itv_quasilinearize_choose_middle_default;
      }
      else {
      itv_quasilinearize_choose_middle_default:
	num_sub(middle,
		bound_numref(coeff->sup),
		bound_numref(coeff->inf));
	num_div_2(middle,
		  middle);
      }
    }
  }
}

bool ITVFUN(itv_quasilinearize_linexpr)(itv_internal_t* intern, itv_linexpr_t* linexpr, itv_t* env, bool for_meet_inequality)
{
  size_t i,k,size;
  ap_dim_t dim;
  itv_ptr itv;
  bool* peq;

#if LOGDEBUG
  printf("itv_quasilinearize_linexpr:\n");
  itv_linexpr_print(linexpr,0); printf("\n");
#endif
  k = 0;
  i = 0;
  size = linexpr->size;
  while (i<size){
    dim = linexpr->linterm[i].dim;
    itv = linexpr->linterm[i].itv;
    peq = &(linexpr->linterm[i].equality);

    if (itv_is_point(intern,env[dim])){
      /* If a variable has a constant value, simplification */
      itv_mul_num(itv,itv,bound_numref(env[dim]->sup));
      itv_add(linexpr->cst,linexpr->cst,itv);

      size--;
      itv_linterm_t tmp = linexpr->linterm[i];
      linexpr->linterm[i] = linexpr->linterm[size];
      linexpr->linterm[size] = tmp;
    }
    else {
      if (*peq == false){
	/* Compute the middle of the interval */
	itv_quasilinearize_choose_middle(intern->quasi_num,
					 itv,env[dim],for_meet_inequality);
	/* Residue (interval-middle) */
	itv_sub_num(intern->eval_itv2,itv,intern->quasi_num);
	/* Multiplication of residue by variable range */
	itv_mul(intern,
		intern->eval_itv,
		intern->eval_itv2,
		env[dim]);
	/* Addition to the constant coefficient */
	itv_add(linexpr->cst,linexpr->cst,intern->eval_itv);
	linexpr->equality = linexpr->equality && itv_is_point(intern,linexpr->cst);
	if (itv_is_top(linexpr->cst)){
	  k = 0;
	  break;
	}
	/* Addition of the linear term */
	if (num_sgn(intern->quasi_num)!=0){
	  linexpr->linterm[k].equality = true;
	  linexpr->linterm[k].dim = dim;
	  itv_set_num(linexpr->linterm[k].itv,intern->quasi_num);
	  k++;
	}
      }
      else {
	if (k!=i){
	  linexpr->linterm[k].equality = *peq;
	  linexpr->linterm[k].dim = dim;
	  itv_set(linexpr->linterm[k].itv,itv);
	}
	k++;
      }
      i++;
    }
  }
  itv_linexpr_reinit(linexpr,k);
#if LOGDEBUG
  itv_linexpr_print(linexpr,0); printf("\n");
#endif
#if defined(NUM_FLOAT) || defined(NUM_DOUBLE) || defined(NUM_LONGDOUBLE) || defined(NUM_MPFR) || defined (NUM_NUMINT)
  return false;
#else
  return true;
#endif
}

bool ITVFUN(itv_quasilinearize_lincons)(itv_internal_t* intern, itv_lincons_t* lincons, itv_t* env, bool meet)
{
  if (lincons->linexpr.size==0){
    /* constant expression */
    tbool_t sat = itv_eval_cstlincons(intern,lincons);
    if (sat==tbool_true || sat==tbool_false){
      itv_lincons_set_bool(lincons,sat==tbool_true);
    }
    return true;
  }
  else {
    return itv_quasilinearize_linexpr(intern,&lincons->linexpr,env,
				      meet &&
				      (lincons->constyp==AP_CONS_SUP ||
				       lincons->constyp==AP_CONS_SUPEQ));
  }
}


bool ITVFUN(itv_quasilinearize_lincons_array)(itv_internal_t* intern, itv_lincons_array_t* array, itv_t* env, bool meet)
{
  size_t i,size;
  bool exact = true;

  itv_lincons_array_reduce(intern,array,meet);
  size = array->size;
  for (i=0; i<size; i++){
    if (meet &&
	array->p[i].constyp == AP_CONS_EQ &&
	!itv_linexpr_is_quasilinear(&array->p[i].linexpr)){
      /* Split an equality constraint into two inequalities if it is really
	 interval linear ==> better precision because quasilinearisation
	 choices differ between expr>=0 and expr<=0 */
      if (size>=array->size){
	itv_lincons_array_reinit(array,1+(5*array->size)/4);
      }
      array->p[i].constyp = AP_CONS_SUPEQ;
      itv_lincons_set(&array->p[size],&array->p[i]);
      itv_linexpr_neg(&array->p[size].linexpr);
      size++;
    }
    exact = itv_quasilinearize_lincons(intern,&array->p[i],env,meet) && exact;
    if (array->p[i].linexpr.size==0 &&
	itv_eval_cstlincons(intern,&array->p[i]) == tbool_false){
      itv_lincons_array_reinit(array,1);
      itv_lincons_set_bool(&array->p[0],false);
      return true;
    }
  }
  itv_lincons_array_reinit(array,size);
  return exact;
}

/* Transform sets of quasilinear constraint as follows:
   e.x + [a,b] >= 0 ==> e.x + b >= 0
   e.x + [a,b] > 0  ==> e.x + b > 0
   e.x + [a,b] = 0  ==> e.x + b >= 0 and e.x + a <= 0 added at the end
   e.x + [a,b] = 0 mod k ==> unchanged

   Also remove (some) trivially true constraints e.x + oo >= 0

*/
static void itv_lincons_select_sup(itv_lincons_t* cons)
{
  bound_neg(cons->linexpr.cst->inf,cons->linexpr.cst->sup);
  cons->linexpr.equality = true;
}
static void itv_lincons_select_inf(itv_lincons_t* cons)
{
  size_t i;
  bound_neg(cons->linexpr.cst->inf,cons->linexpr.cst->inf);
  bound_neg(cons->linexpr.cst->sup,cons->linexpr.cst->inf);
  cons->linexpr.equality = true;
  for (i=0;i<cons->linexpr.size;i++){
    itv_neg(cons->linexpr.linterm[i].itv,cons->linexpr.linterm[i].itv);
  }
}

void ITVFUN(itv_linearize_lincons_array)(itv_internal_t* intern, itv_lincons_array_t* array, bool meet)
{
  size_t index,size,sizeorg;

  tbool_t res = itv_lincons_array_reduce(intern,array,meet);
  if (res!=tbool_top) return;

  /* One now remove intervals when we can */
  sizeorg = array->size;
  size = sizeorg;
  for (index=0; index<sizeorg; index++){
    itv_lincons_t* cons = &array->p[index];
    if (!cons->linexpr.equality){
      itv_ptr cst = cons->linexpr.cst;
      bool sup = !bound_infty(cst->sup);
      switch (cons->constyp){
      case AP_CONS_EQ:
	assert (meet); /* otherwise, already removed */
	{
	  bool inf = !bound_infty(cst->inf);
	  assert (inf || sup); /* otherwise, already removed */
	  if (inf && sup){
	    if (size>=array->size){
	      itv_lincons_array_reinit(array,1+5*array->size/4);
	    }
	    /* Be cautious: cons and cst may be invalid now */
	    itv_lincons_set(&array->p[size],&array->p[index]);
	    array->p[index].constyp = AP_CONS_SUPEQ;
	    array->p[size].constyp  = AP_CONS_SUPEQ;
	    itv_lincons_select_sup(&array->p[index]);
	    itv_lincons_select_inf(&array->p[size]);
	    size++;
	  }
	  else if (inf){
	    array->p[index].constyp = AP_CONS_SUPEQ;
	    itv_lincons_select_inf(&array->p[index]);
	  }
	  else if (sup){
	    array->p[index].constyp = AP_CONS_SUPEQ;
	    itv_lincons_select_sup(&array->p[index]);
	  }
	  else
	    assert(false);
	}
	break;
      case AP_CONS_SUPEQ:
      case AP_CONS_SUP:
	if (meet){
	  assert(sup);
	  itv_lincons_select_sup(&array->p[index]);
	}
	else {
	  assert(!bound_infty(cst->inf));
	  bound_neg(cons->linexpr.cst->sup,cons->linexpr.cst->inf);
	  cons->linexpr.equality = true;
	}
	break;
      default:
	break;
      }
    }
  }
  itv_lincons_array_reinit(array,size);
}



/* ********************************************************************** */
/* IV. Evaluation of tree expressions  */
/* ********************************************************************** */

/* General rounding */
static void itv_round(itv_t res, itv_t arg,
		      ap_texpr_rtype_t t, ap_texpr_rdir_t d)
{
  switch (t) {

  case AP_RTYPE_REAL:
    if (&res!=&arg) itv_set(res,arg);
    break;

  case AP_RTYPE_INT:
    switch (d) {
    case AP_RDIR_ZERO:
      itv_trunc(res,arg);
      break;
    case AP_RDIR_UP:
      itv_ceil(res,arg);
      break;
    case AP_RDIR_DOWN:
      itv_floor(res,arg);
      break;
    case AP_RDIR_RND:
    case AP_RDIR_NEAREST: /* 'to nearest' case could be improved */
      itv_to_int(res,arg);
      break;
    default:
      assert(0);
    }
    break;

  case AP_RTYPE_SINGLE:
    /* directed rounding cases (+oo, -oo, 0) could be improved */
    itv_to_float(res,arg);
    break;

  case AP_RTYPE_QUAD:     /* 'round to quad' could be improved */
  case AP_RTYPE_EXTENDED: /* 'round to extended' could be improved */
  case AP_RTYPE_DOUBLE:
    /* directed rounding cases (+oo, -oo, 0) could be improved */
    itv_to_double(res,arg);
    break;

  default:
    assert(0);
  }
}

/* Evaluates node into interval res, assuming operator arguments are arg1 and
   (for binary operator) arg2
*/
static void
itv_eval_ap_texpr0_node(itv_internal_t* intern,
			ap_texpr0_node_t* n,
			itv_t res, itv_t arg1, itv_t arg2)
{
  switch (n->op) {
  case AP_TEXPR_NEG:
    itv_neg(res, arg1);
    return; /* no rounding */
  case AP_TEXPR_CAST:
    itv_set(res, arg1);
    break;
  case AP_TEXPR_SQRT:
    itv_sqrt(intern, res, arg1);
    break;
  case AP_TEXPR_ADD:
    itv_add(res, arg1, arg2);
    break;
  case AP_TEXPR_SUB:
    itv_sub(res, arg1, arg2);
    break;
  case AP_TEXPR_MUL:
    itv_mul(intern, res, arg1, arg2);
    break;
  case AP_TEXPR_DIV:
    itv_div(intern, res, arg1, arg2);
    break;
  case AP_TEXPR_POW:
    itv_pow(intern, res, arg1, arg2);
    break;
  case AP_TEXPR_MOD:
    itv_mod(intern, res, arg1, arg2, n->type==AP_RTYPE_INT);
    return; /* no rounding */
  default:
    assert(0);
  }
  itv_round(res,res,n->type,n->dir);
}

/* evaluates expr into intervalres,
   assuming env maps dimensions to interval values */
void ITVFUN(itv_eval_ap_texpr0)(itv_internal_t* intern,
				itv_t res,
				ap_texpr0_t* expr,
				itv_t* env)
{
  if (!expr) {
    itv_set_bottom(res);
    return;
  }

  switch(expr->discr){
  case AP_TEXPR_CST:
    itv_set_ap_coeff(intern,res,&expr->val.cst);
    break;
  case AP_TEXPR_DIM:
    itv_set(res,env[expr->val.dim]);
    break;
  case AP_TEXPR_NODE:
    if (expr->val.node->exprB) {
      /* binary */
      itv_t x;
      itv_init(x);
      itv_eval_ap_texpr0(intern,x,expr->val.node->exprA,env);
      itv_eval_ap_texpr0(intern,res,expr->val.node->exprB,env);
      if (itv_is_bottom(intern,x) || itv_is_bottom(intern,res)){
	itv_set_bottom(res);
      }
      else {
	itv_eval_ap_texpr0_node(intern,expr->val.node,res,x,res);
      }
      itv_clear(x);
    }
    else {
      /* unary */
      itv_eval_ap_texpr0(intern,res,expr->val.node->exprA,env);
      if (!itv_is_bottom(intern,res)){
	itv_eval_ap_texpr0_node(intern,expr->val.node,res,res,res);
      }
    }
    break;
  default:
    assert(false);
  }
}

/* ====================================================================== */
/* V. Interval linearization of interval linear tree expressions */
/* ====================================================================== */

/* preconditions:
   - lres is initialised

   postconditions:
   - stores an interval linear form in lres
   - returns true if input expression is not interval linear
*/
static bool
ap_texpr0_node_intlinearize_linear(itv_internal_t* intern,
				   ap_texpr0_node_t* n,
				   itv_linexpr_t* lres /* out */)
{
  itv_t i1;
  itv_linexpr_t l1;

  bool exc = n->type!=AP_RTYPE_REAL && n->op!=AP_TEXPR_NEG;
  if (exc) return exc;
  switch (n->op) {
  case AP_TEXPR_NEG:
    exc = itv_intlinearize_ap_texpr0_intlinear(intern,lres,n->exprA);
    itv_linexpr_neg(lres);
    break;

  case AP_TEXPR_CAST:
    exc = itv_intlinearize_ap_texpr0_intlinear(intern,lres,n->exprA);
    break;

  case AP_TEXPR_MOD:
  case AP_TEXPR_SQRT:
  case AP_TEXPR_POW:
    exc = true;
    break;

  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
    itv_linexpr_init(&l1,0);

    /* intlinearize arguments */
    exc = itv_intlinearize_ap_texpr0_intlinear(intern,&l1,n->exprA);
    exc = itv_intlinearize_ap_texpr0_intlinear(intern,lres,n->exprB) || exc;
    /* add/sub linear form & interval */
    if (n->op==AP_TEXPR_ADD)
      itv_linexpr_add(intern,lres,&l1,lres);
    else
      itv_linexpr_sub(intern,lres,&l1,lres);
    itv_linexpr_clear(&l1);
    break;

  case AP_TEXPR_MUL:
  case AP_TEXPR_DIV:
    itv_init(i1);
    if (ap_texpr0_is_interval_cst(n->exprB)){
      exc = itv_intlinearize_ap_texpr0_intlinear(intern,lres,n->exprA);
      itv_eval_ap_texpr0(intern,i1,n->exprB,NULL);
    }
    else if (n->op == AP_TEXPR_MUL && ap_texpr0_is_interval_cst(n->exprA)){
      exc = itv_intlinearize_ap_texpr0_intlinear(intern,lres,n->exprB);
      itv_eval_ap_texpr0(intern,i1,n->exprA,NULL);
    }
    else {
      exc = true;
      break;
    }
    if (n->op==AP_TEXPR_DIV){
      itv_t i2;
      itv_init(i2);
      itv_set_int(i2,1);
      itv_div(intern,i1,i2,i1);
      itv_clear(i2);
    }
    itv_linexpr_scale(intern,lres,i1);
    itv_clear(i1);
    break;

  default:
    assert(0);
  }
  return exc;
}

bool
ITVFUN(itv_intlinearize_ap_texpr0_intlinear)(itv_internal_t* intern,
					  itv_linexpr_t* lres,
					  ap_texpr0_t* expr)
{
  bool exc = false;
  assert(expr);

  switch(expr->discr){
  case AP_TEXPR_CST:
    itv_linexpr_reinit(lres,0);
    itv_set_ap_coeff(intern,lres->cst,&expr->val.cst);
    lres->equality = itv_is_point(intern,lres->cst);
    break;
  case AP_TEXPR_DIM:
    itv_linexpr_reinit(lres,1);
    itv_set_int(lres->cst,0);
    lres->linterm[0].dim = expr->val.dim;
    lres->linterm[0].equality = true;
    itv_set_int(lres->linterm[0].itv,1);
    break;
  case AP_TEXPR_NODE:
    if (ap_texpr0_is_interval_cst(expr)){
      itv_linexpr_reinit(lres,0);
      itv_eval_ap_texpr0(intern,lres->cst,expr,NULL);
      lres->equality = itv_is_point(intern,lres->cst);
    }
    else {
      exc = ap_texpr0_node_intlinearize_linear(intern,expr->val.node,lres);
    }
    break;
  default:
    exc = true;
    assert(false);
  }
  return exc;
}

bool
ITVFUN(itv_intlinearize_ap_tcons0_intlinear)(itv_internal_t* intern,
					     itv_lincons_t* res,
					     ap_tcons0_t* cons)
{
  bool exc = itv_intlinearize_ap_texpr0_intlinear(intern,&res->linexpr,cons->texpr0);
  if (exc){
    itv_lincons_set_bool(res,false);
  }
  else {
    res->constyp = cons->constyp;
    if (cons->scalar){
      num_set_ap_scalar(res->num,cons->scalar);
    }
    else {
      num_set_int(res->num,0);
    }
  }
  return exc;
}

bool
ITVFUN(itv_intlinearize_ap_tcons0_array_intlinear)(itv_internal_t* intern,
						   itv_lincons_array_t* res,
						   ap_tcons0_array_t* array)
{
  bool exc;
  size_t i;

  itv_lincons_array_reinit(res,array->size);
  exc = false;
  for (i=0; i<array->size;i++){
    bool exc = itv_intlinearize_ap_texpr0_intlinear(intern,&res->p[i].linexpr,array->p[i].texpr0);
    res->p[i].constyp = array->p[i].constyp;
    if (array->p[i].scalar){
      num_set_ap_scalar(res->p[i].num,array->p[i].scalar);
    }
    else {
      num_set_int(res->p[i].num,0);
    }
    if (exc){
      itv_lincons_array_reinit(res,1);
      itv_lincons_set_bool(&res->p[0],false);
      break;
    }
  }
  return exc;
}


/* ====================================================================== */
/* VI. Linearization of tree expressions */
/* ====================================================================== */

/* transform in-place
	[A0,B0] + sum Xi [Ai,Bi]
   into
       ([A0,B0] + [A0,B0][-ulp,ulp]) + [-mf,mf] +
       sum Xi ([Ai,Bi] + [Ai,Bi][-ulp,ulp])

   i.e., add a relative error of magnitude ulp as an interval linear form
*/
static void
itv_linexpr_round_float_lin(itv_internal_t* intern,
			    itv_linexpr_t* l /* in/out */,
			    float_const* f)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;
  itv_magnitude(intern->linear_bound,l->cst);
  bound_mul(intern->linear_bound,intern->linear_bound,f->ulp->sup);
  bound_add(intern->linear_bound,intern->linear_bound,f->min->sup);
  itv_enlarge_bound(l->cst,l->cst,intern->linear_bound);
  l->equality = false;
  itv_linexpr_ForeachLinterm(l,i,dim,pitv,peq) {
    *peq = false;
    itv_magnitude(intern->linear_bound,pitv);
    bound_mul(intern->linear_bound,intern->linear_bound,f->ulp->sup);
    itv_enlarge_bound(pitv,pitv,intern->linear_bound);
  }
}


/* transform in-place
	[A0,B0] + sum Xi [Ai,Bi]
   into
       [A0,B0] + ([A0,B0] + sum [min Xi,max Xi][Ai,Bi]) [-ulp,ulp] + [-mf,mf]
	sum Xi [Ai,Bi]

   i.e., adds a relative error of magnitude ulp flattened into an interval
*/
static void
itv_linexpr_round_float(itv_internal_t* intern,
			itv_linexpr_t* l /* in/out */,
			float_const* f, itv_t* env)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;
  itv_magnitude(intern->linear_bound,l->cst);
  l->equality = false;
  itv_linexpr_ForeachLinterm(l,i,dim,pitv,peq) {
    *peq = false;
    itv_magnitude(intern->linear_bound2,pitv);
    itv_magnitude(intern->linear_bound3,env[dim]);
    bound_mul(intern->linear_bound2,intern->linear_bound2,intern->linear_bound3);
    bound_add(intern->linear_bound,intern->linear_bound,intern->linear_bound2);
  }
  bound_mul(intern->linear_bound,intern->linear_bound,f->ulp->sup);
  bound_add(intern->linear_bound,intern->linear_bound,f->min->sup);
  itv_enlarge_bound(l->cst,l->cst,intern->linear_bound);
}

/* adds an absolute error to l corresponding to a conversion to int
   assumes that i overapproximates the values of l before conversion
 */
static void
ap_texpr0_to_int(itv_internal_t* intern,
		 itv_linexpr_t* l /* in/out */, itv_t i /* in */,
		 ap_texpr_rdir_t d)
{
  switch (d) {
  case AP_RDIR_UP:
    /* add [0,1] */
    bound_add_uint(l->cst->sup,l->cst->sup,1);
    break;
  case AP_RDIR_DOWN:
    /* add [-1,0] */
    bound_add_uint(l->cst->inf,l->cst->inf,1);
    break;
  case AP_RDIR_RND:
    /* add [-1,1] */
    bound_add_uint(l->cst->sup,l->cst->sup,1);
    bound_add_uint(l->cst->inf,l->cst->inf,1);
    break;
  case AP_RDIR_ZERO:
    /* UP or DOWN or RND, depending on sign of i */
    if (bound_sgn(i->inf)>0) bound_add_uint(l->cst->sup,l->cst->sup,1);
    if (bound_sgn(i->sup)>0) bound_add_uint(l->cst->inf,l->cst->inf,1);
    break;
  case AP_RDIR_NEAREST:
    /* [-0.5,0.5] */
    itv_add(l->cst,l->cst,intern->itv_half);
    break;
  default:
    assert(0);
  }
  l->equality = false;
}

/* adds rounding error to both l and i to go from type org to type dst */
static ap_texpr_rtype_t
ap_texpr0_round(itv_internal_t* intern,
		itv_linexpr_t* l /* in/out */, itv_t i /* in/out */,
		ap_texpr_rtype_t org,
		ap_texpr_rtype_t dst, ap_texpr_rdir_t d)
{
  if (dst==AP_RTYPE_REAL) return org;
  switch (dst) {
  case AP_RTYPE_INT:
    if (org==AP_RTYPE_INT) return org;
    ap_texpr0_to_int(intern,l,i,d);
    break;
  case AP_RTYPE_SINGLE:
    if (org==AP_RTYPE_SINGLE) return org;
    itv_linexpr_round_float_lin(intern,l,&intern->cst_single);
    break;
  case AP_RTYPE_DOUBLE:
    if (org==AP_RTYPE_SINGLE || org==AP_RTYPE_DOUBLE) return org;
    itv_linexpr_round_float_lin(intern,l,&intern->cst_double);
    break;
  case AP_RTYPE_EXTENDED:
    if (org==AP_RTYPE_SINGLE || org==AP_RTYPE_DOUBLE || org==AP_RTYPE_EXTENDED)
      return org;
    itv_linexpr_round_float_lin(intern,l,&intern->cst_extended);
    break;
  case AP_RTYPE_QUAD:
    if (org==AP_RTYPE_SINGLE || org==AP_RTYPE_DOUBLE ||
	org==AP_RTYPE_EXTENDED || org==AP_RTYPE_QUAD) return org;
    itv_linexpr_round_float_lin(intern,l,&intern->cst_quad);
    break;
  default:
    assert(0);
  }

  itv_round(i,i,dst,d);
  return dst;
}

/* reduce l and i:
   - intersect i with the interval evaluation of l
   - if l is constant, replace it with i
   - check for emptiness
 */
static void
ap_texpr0_reduce(itv_internal_t* intern, itv_t* env,
		 itv_linexpr_t* l /* in/out */, itv_t i /* in/out */)
{
  itv_t tmp;
  itv_init(tmp);
  itv_eval_linexpr(intern,tmp,l,env);
  itv_meet(intern,i,i,tmp);
  if (itv_is_bottom(intern,i) || itv_is_bottom(intern,l->cst)) {
    itv_set_bottom(i);
    itv_set_bottom(l->cst);
    if (l->size>0) itv_linexpr_reinit(l,0);
  }
  else if (l->size==0){
    itv_set(l->cst,i);
    l->equality = itv_is_point(intern,l->cst);
  }
  itv_clear(tmp);
}

/* multiplication heuristic: choose which interval to keep (0=a, 1=b) */
static int
ap_texpr0_cmp_range(itv_internal_t* intern,
		    itv_linexpr_t* la, itv_t ia,
		    itv_linexpr_t* lb, itv_t ib)
{
  int sgn_a,sgn_b;
  /* if one linear form is an interval keep it */
  if (la->size==0) return 0;
  if (lb->size==0) return 1;
  /* if only one interval has constant sign, keep it */
  sgn_a = itv_is_pos(ia) || itv_is_neg(ia);
  sgn_b = itv_is_pos(ib) || itv_is_neg(ib);
  if (sgn_a!=sgn_b) return sgn_a ? 0 : 1;
  /* otherwise, keep the interval with the smallest relative range */
  itv_range_rel(intern,intern->linear_bound,ia);
  itv_range_rel(intern,intern->linear_bound2,ib);
  if (bound_cmp(intern->linear_bound,intern->linear_bound2)<0) return 0;
  else return 1;
}


/* preconditions:
   - env assigns an interval to each dimension
   - dimensions strictly smaller that intdim are integer
   - lres is not initialised
   - ires is initialised

   postconditions:
   - stores an interval linear form in lres
   - stores an interval in ires
   both encompase the exact expression values (ires may be smaller than the
   interval evaluation of lres)
   - returns the expression type
*/

static ap_texpr_rtype_t
itv_intlinearize_texpr0_rec(itv_internal_t* intern,
			    ap_texpr0_t* expr,
			    itv_t* env, size_t intdim,
			    itv_linexpr_t* lres /* out */,
			    itv_t ires /* out */);

#if LOGDEBUG
static int debug_indent = 0;
#endif

static ap_texpr_rtype_t
ap_texpr0_node_intlinearize(itv_internal_t* intern,
			    ap_texpr0_node_t* n,
			    itv_t* env, size_t intdim,
			    itv_linexpr_t* lres /* out */, itv_t ires /* out */)
{
  itv_t i1;
  itv_linexpr_t l1;
  ap_texpr_rtype_t t1,t2;

  switch (n->op) {
  case AP_TEXPR_NEG:
    /* negate linear form & interval, no rounding */
    t1 = itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    itv_linexpr_neg(lres);
    itv_neg(ires,ires);
    return t1;

  case AP_TEXPR_CAST:
    /* round linear form & interval */
    t1 = itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    ap_texpr0_round(intern,lres,ires,t1,n->type,n->dir);
    ap_texpr0_reduce(intern,env,lres,ires);
    break;

  case AP_TEXPR_SQRT:
    /* intlinearize argument, lres is not used */
    itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    /* interval square root */
    itv_sqrt(intern,ires,ires);
    itv_round(ires,ires,n->type,n->dir);
    itv_linexpr_reinit(lres,0);
    itv_set(lres->cst,ires);
    lres->equality = itv_is_point(intern,lres->cst);
    break;

  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
    itv_init(i1);
    itv_linexpr_init(&l1,0);

    /* intlinearize arguments */
    t1 = itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,&l1,i1);
    t2 = itv_intlinearize_texpr0_rec(intern,n->exprB,env,intdim,lres,ires);
    if (itv_is_bottom(intern,i1) || itv_is_bottom(intern,ires)){
      itv_set_bottom(ires);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
    }
    else {
      /* add/sub linear form & interval */
      if (n->op==AP_TEXPR_ADD) {
	itv_linexpr_add(intern,lres,&l1,lres);
	itv_add(ires,i1,ires);
      }
      else {
	itv_linexpr_sub(intern,lres,&l1,lres);
	itv_sub(ires,i1,ires);
      }
      /* round */
      ap_texpr0_round(intern,lres,ires,
		      (t1==AP_RTYPE_INT && t2==AP_RTYPE_INT) ?
		      AP_RTYPE_INT : AP_RTYPE_REAL,
		      n->type,n->dir);
      /* reduce */
      ap_texpr0_reduce(intern,env,lres,ires);
    }
    itv_clear(i1);
    itv_linexpr_clear(&l1);
    break;

  case AP_TEXPR_DIV:
    itv_init(i1);
    itv_linexpr_init(&l1,0);
    /* intlinearize arguments, l1 is not used */
    itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    itv_intlinearize_texpr0_rec(intern,n->exprB,env,intdim,&l1,i1);
    if (itv_is_bottom(intern,i1) || itv_is_bottom(intern,ires)){
      itv_set_bottom(ires);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
    }
    else {
      /* divide linear form & interval */
      itv_linexpr_div(intern,lres,i1);
      itv_div(intern,ires,ires,i1);
      /* round */
      ap_texpr0_round(intern,lres,ires,AP_RTYPE_REAL,n->type,n->dir);
      /* reduce */
      ap_texpr0_reduce(intern,env,lres,ires);
    }
    itv_clear(i1);
    itv_linexpr_clear(&l1);
    break;

  case AP_TEXPR_MUL:
    itv_init(i1);
    itv_linexpr_init(&l1,0);
    /* intlinearize arguments */
    t1 = itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,&l1,i1);
    t2 = itv_intlinearize_texpr0_rec(intern,n->exprB,env,intdim,lres,ires);
    if (itv_is_bottom(intern,i1) || itv_is_bottom(intern,ires)){
      itv_set_bottom(ires);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
      itv_linexpr_clear(&l1);
    }
    else {
      /* multiply one linear form with the other interval */
      if (ap_texpr0_cmp_range(intern,&l1,i1,lres,ires))  {
	/* res = ires * l1 */
#if LOGDEBUG
	printf("%*s lin * inter\n",2*debug_indent,"");
#endif
	itv_linexpr_clear(lres);
	*lres = l1;
	itv_linexpr_scale(intern,lres,ires);
      }
      else {
	/* res = i1 * lres */
#if LOGDEBUG
	printf("%*s inter * lin\n",2*debug_indent,"");
#endif
	itv_linexpr_clear(&l1);
	itv_linexpr_scale(intern,lres,i1);
      }
      itv_mul(intern,ires,i1,ires);
      /* round */
      ap_texpr0_round(intern,lres,ires,
		      (t1==AP_RTYPE_INT && t2==AP_RTYPE_INT) ?
		      AP_RTYPE_INT : AP_RTYPE_REAL,
		      n->type,n->dir);
      /* reduce */
      ap_texpr0_reduce(intern,env,lres,ires);
    }
    itv_clear(i1);
    break;

  case AP_TEXPR_MOD:
    itv_init(i1);
    itv_linexpr_init(&l1,0);
    /* intlinearize arguments, lres & l1 are not used */
    itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    itv_intlinearize_texpr0_rec(intern,n->exprB,env,intdim,&l1,i1);
    if (itv_is_bottom(intern,i1) || itv_is_bottom(intern,ires)){
      itv_set_bottom(ires);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
    }
    else {
      /* interval modulo, no rounding */
      itv_mod(intern,ires,ires,i1,n->type==AP_RTYPE_INT);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
      lres->equality = itv_is_point(intern,lres->cst);
    }
    itv_clear(i1);
    itv_linexpr_clear(&l1);
    break;

  case AP_TEXPR_POW:
    itv_init(i1);
    itv_linexpr_init(&l1,0);
    /* intlinearize arguments, lres & l1 are not used */
    itv_intlinearize_texpr0_rec(intern,n->exprA,env,intdim,lres,ires);
    itv_intlinearize_texpr0_rec(intern,n->exprB,env,intdim,&l1,i1);
    if (itv_is_bottom(intern,i1) || itv_is_bottom(intern,ires)){
      itv_set_bottom(ires);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
    }
    else {
      itv_pow(intern,ires,ires,i1);
      itv_linexpr_reinit(lres,0);
      itv_set(lres->cst,ires);
      lres->equality = itv_is_point(intern,lres->cst);
    }
    itv_clear(i1);
    itv_linexpr_clear(&l1);
    break;

  default:
    assert(0);
  }

  return n->type;
}

static ap_texpr_rtype_t
itv_intlinearize_texpr0_rec(itv_internal_t* intern,
			    ap_texpr0_t* expr,
			    itv_t* env, size_t intdim,
			    itv_linexpr_t* lres /* out */, itv_t ires /* out */)
{
  ap_texpr_rtype_t t;
  assert(expr);

#if LOGDEBUG
  printf("%*s @ ",2*debug_indent,"");
  ap_texpr0_print(expr,NULL);
  printf("\n");
  debug_indent++;
#endif

  switch(expr->discr){
  case AP_TEXPR_CST:
    itv_set_ap_coeff(intern,ires,&expr->val.cst);
    itv_linexpr_reinit(lres,0);
    itv_set(lres->cst,ires);
    lres->equality = itv_is_point(intern,lres->cst);
    t = itv_is_int(intern,lres->cst) ? AP_RTYPE_INT : AP_RTYPE_REAL;
    break;
  case AP_TEXPR_DIM:
    itv_set(ires,env[expr->val.dim]);
    itv_linexpr_reinit(lres,1);
    itv_set_int(lres->cst,0);
    lres->linterm[0].dim = expr->val.dim;
    lres->linterm[0].equality = true;
    itv_set_int(lres->linterm[0].itv,1);
    t = (expr->val.dim<intdim) ? AP_RTYPE_INT : AP_RTYPE_REAL;
    break;
  case AP_TEXPR_NODE:
    t = ap_texpr0_node_intlinearize(intern,expr->val.node,env,intdim,lres,ires);
    break;
  default:
    t = 0;
    assert(false);
  }

#if LOGDEBUG
  debug_indent--;
  printf("%*s   = ",2*debug_indent,"");
  itv_linexpr_print(lres,NULL);
  printf(" /\\ ");
  itv_print(ires);
  printf(" t=%i\n",t);
#endif

  return t;
}

bool
ITVFUN(itv_intlinearize_ap_texpr0)(itv_internal_t* intern,
				   itv_linexpr_t* res,
				   ap_texpr0_t* expr,
				   itv_t* env, size_t intdim)
{
  bool exc;
  itv_t i;
  itv_init(i);
  itv_intlinearize_texpr0_rec(intern,expr,env,intdim,res,i);
  if (!itv_is_bottom(intern,i) && !itv_is_bottom(intern,res->cst)) {
    if (res->size==0){
      itv_meet(intern,res->cst,res->cst,i);
      res->equality = itv_is_point(intern,res->cst);
    }
    exc = false;
  }
  else {
    exc = true;
  }
  itv_clear(i);
  return exc;
}

bool
ITVFUN(itv_intlinearize_ap_tcons0)(itv_internal_t* intern,
				   itv_lincons_t* res,
				   ap_tcons0_t* cons,
				   itv_t* env, size_t intdim)
{
  bool exc;
  itv_t i, bound;
  itv_init(i);
  itv_init(bound);
  itv_intlinearize_texpr0_rec(intern,cons->texpr0,env,intdim,&res->linexpr,i);

  /* checks that the constraint is satisfiable */
  switch (cons->constyp){
  case AP_CONS_EQ:
    itv_set_int(bound,0);
    itv_meet(intern,bound,i,bound);
    break;
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    itv_set_top(bound);
    bound_set_int(bound->inf,0);
    bound_set_infty(bound->sup,1);
    itv_meet(intern,bound,i,bound);
    break;
  default:
    break;
  }

  if (!itv_is_bottom(intern,bound) && !itv_is_bottom(intern,res->linexpr.cst)) {
    if (res->linexpr.size==0){
      itv_meet(intern,res->linexpr.cst,res->linexpr.cst,i);
      res->linexpr.equality = itv_is_point(intern,res->linexpr.cst);
    }
    res->constyp = cons->constyp;
    if (cons->scalar){
      num_set_ap_scalar(res->num,cons->scalar);
    }
    else {
      num_set_int(res->num,0);
    }
    exc = false;
  }
  else {
    itv_lincons_set_bool(res,false);
    exc = true;
  }
  itv_clear(i);
  itv_clear(bound);
  return exc;
}

bool
ITVFUN(itv_intlinearize_ap_tcons0_array)(itv_internal_t* intern,
					 itv_lincons_array_t* res,
					 ap_tcons0_array_t* array,
					 itv_t* env, size_t intdim)
{
  bool exc;
  itv_t itv,bound;
  size_t i;

  itv_init(itv);
  itv_init(bound);
  itv_lincons_array_reinit(res,array->size);
  exc = false;
  for (i=0; i<array->size;i++){
    itv_intlinearize_texpr0_rec(intern,array->p[i].texpr0,
				env,intdim,&res->p[i].linexpr,itv);
    res->p[i].constyp = array->p[i].constyp;
    if (array->p[i].scalar){
      num_set_ap_scalar(res->p[i].num,array->p[i].scalar);
    }
    else {
      num_set_int(res->p[i].num,0);
    }

    /* checks that the constraint is satisfiable */
    switch (array->p[i].constyp){
    case AP_CONS_EQ:
      itv_set_int(bound,0);
      itv_meet(intern,bound,itv,bound);
      break;
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      itv_set_top(bound);
      bound_set_int(bound->inf,0);
      bound_set_infty(bound->sup,1);
      itv_meet(intern,bound,itv,bound);
      break;
    default:
      break;
    }

    if (itv_is_bottom(intern,bound) ||
	itv_is_bottom(intern,res->p[i].linexpr.cst) ||
	(res->p[i].linexpr.size==0 ?
	 itv_eval_cstlincons(intern,&res->p[i])==tbool_false :
	 false)){
      exc = true;
      itv_lincons_array_reinit(res,1);
      itv_lincons_set_bool(&res->p[0],false);
      break;
    }
  }
  itv_clear(itv);
  itv_clear(bound);
  return exc;
}




/* ====================================================================== */
/* VII. Backward evaluation of tree expressions */
/* ====================================================================== */

/* Tree expressions with nodes annotated with intervals */
typedef struct {
  ap_texpr0_t expr;
  itv_t val; /* value of expr */
} itv_expr_t;

static void
itv_expr_free(itv_internal_t* intern, itv_expr_t* arg)
{
  if (!arg) return;
  itv_clear(arg->val);
  switch (arg->expr.discr) {
  case AP_TEXPR_CST:
    ap_coeff_clear(&arg->expr.val.cst);
    break;
  case AP_TEXPR_DIM:
    break;
  case AP_TEXPR_NODE:
    itv_expr_free(intern, (itv_expr_t*)arg->expr.val.node->exprA);
    if (arg->expr.val.node->exprB) {
      itv_expr_free(intern, (itv_expr_t*)arg->expr.val.node->exprB);
    }
    free(arg->expr.val.node);
    break;
  default:
    assert(0);
  }
  free(arg);
}

/* Creates an initial annotated expression from an ap_texpr0_t.
   Performs a bottom-up evaluation.
 */
static itv_expr_t*
itv_expr_build(itv_internal_t* intern,
	       ap_texpr0_t* arg,
	       itv_t* env, size_t intdim)
{
  itv_expr_t* res;
  if (!arg) return NULL;
  res = malloc(sizeof(itv_expr_t));
  res->expr.discr = arg->discr;
  itv_init(res->val);
  switch (arg->discr) {
  case AP_TEXPR_CST:
    ap_coeff_init_set(&res->expr.val.cst, &arg->val.cst);
    itv_set_ap_coeff(intern, res->val, &arg->val.cst);
    break;
  case AP_TEXPR_DIM:
    res->expr.val.dim = arg->val.dim;
    itv_set(res->val, env[arg->val.dim]);
    break;
  case AP_TEXPR_NODE:
    {
      itv_expr_t *a, *b = NULL;
      ap_texpr0_node_t* narg = arg->val.node;
      ap_texpr0_node_t* nres = malloc(sizeof(ap_texpr0_node_t));
      a = itv_expr_build(intern, narg->exprA, env, intdim);
      if (narg->exprB) {
	b = itv_expr_build(intern, narg->exprB, env, intdim);
      }
      res->expr.val.node = nres;
      nres->op = narg->op;
      nres->type = narg->type;
      nres->dir = narg->dir;
      nres->exprA = (ap_texpr0_t*)a;
      nres->exprB = (ap_texpr0_t*)b;
      if (itv_is_bottom(intern, a->val) || (b && itv_is_bottom(intern, b->val))) {
	itv_set_bottom(res->val);
      }
      else {
	itv_eval_ap_texpr0_node(intern, narg, res->val, a->val, b?b->val:a->val);
      }
    }
    break;
  default:
    assert(0);
  }
  return res;
}

/* Update arg with a bottom-up evaluation pass.
   Return true if some node value has changed.
   Use intern->eval_itv.
 */
static bool
itv_expr_bottom_up(itv_internal_t* intern,
		   itv_expr_t* arg,
		   itv_t* env, size_t intdim)
{
  if (!arg) return false;
  switch (arg->expr.discr) {
  case AP_TEXPR_CST:
    /* nothing to do: arg->val should be up to date */
    return false;
  case AP_TEXPR_DIM:
    itv_meet(intern, intern->eval_itv, arg->val, env[arg->expr.val.dim]);
    if (itv_is_eq(intern->eval_itv, arg->val)) {
      return false;
    }
    else {
      itv_set(arg->val, intern->eval_itv);
      return true;
    }
  case AP_TEXPR_NODE:
    {
      bool r1, r2 = false;
      ap_texpr0_node_t* n = arg->expr.val.node;
      itv_expr_t* a = (itv_expr_t*)n->exprA;
      itv_expr_t* b = (itv_expr_t*)n->exprB;
      r1 = itv_expr_bottom_up(intern, a, env, intdim);
      if (b) {
	r2 = itv_expr_bottom_up(intern, b, env, intdim);
      }
      if (itv_is_bottom(intern, a->val) || (b && itv_is_bottom(intern, b->val))) {
	itv_set_bottom(intern->eval_itv);
      }
      else {
	itv_eval_ap_texpr0_node(intern, n, intern->eval_itv, a->val, b?b->val:a->val);
	itv_meet(intern, intern->eval_itv, intern->eval_itv, arg->val);
      }
      if (itv_is_eq(intern->eval_itv, arg->val)) {
	return r1 || r2;
      }
      else {
	itv_set(arg->val, intern->eval_itv);
	return true;
      }
    }
  default:
    assert(0);
  }
  return false;
}

/* Backward rounding.
   Store in res the interval of values that, after rounding, can yield a
   value in arg.
 */
static void
itv_unround(itv_internal_t* intern, itv_t res, itv_t arg,
	    ap_texpr_rtype_t t, ap_texpr_rdir_t d)
{
  (void)intern;
  switch (t) {
  case AP_RTYPE_REAL:
    itv_set(res,arg);
    break;
  case AP_RTYPE_INT:
    switch (d) {
    case AP_RDIR_ZERO:
      itv_untrunc(res,arg);
      break;
    case AP_RDIR_UP:
      itv_unceil(res,arg);
      break;
    case AP_RDIR_DOWN:
      itv_unfloor(res,arg);
      break;
    case AP_RDIR_RND:
    case AP_RDIR_NEAREST:
      itv_from_int(res,arg);
      break;
    default:
      assert(0);
    }
    break;
  case AP_RTYPE_SINGLE:
    itv_from_float(res,arg);
    break;
  case AP_RTYPE_QUAD:
  case AP_RTYPE_EXTENDED:
  case AP_RTYPE_DOUBLE:
    itv_from_double(res,arg);
    break;
  default:
    assert(0);
  }
}

/* Backward refine a node.
   Given arg1, arg2, and a refined res, refine arg1 and arg2 and store the
   result in arg1r and arg2r.
   Use intern->eval_itv3.
 */
static void
itv_refine_ap_texpr0_node(itv_internal_t* intern,
			  ap_texpr0_node_t* n,
			  itv_t res, itv_t arg1, itv_t arg2,
			  itv_t arg1r, itv_t arg2r)
{
  switch (n->op) {
  case AP_TEXPR_NEG:
    /* res = -arg1 => arg1 = -res */
    itv_neg(arg1r, res);
    itv_meet(intern, arg1r, arg1r, arg1);
    break;
  case AP_TEXPR_ADD:
    /* res = round(arg1+arg2) =>
       arg1 = unround(res) - arg2 /\ arg2 = unround(res) - arg1
    */
    itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
    itv_sub(arg1r, intern->eval_itv3, arg2);
    itv_sub(arg2r, intern->eval_itv3, arg1);
    itv_meet(intern, arg1r, arg1r, arg1);
    itv_meet(intern, arg2r, arg2r, arg2);
    break;
  case AP_TEXPR_SUB:
    /* res = round(arg1-arg2) =>
       arg1 = arg2 + unround(res) /\ arg2 = arg1 - unround(res)
    */
    itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
    itv_add(arg1r, arg2, intern->eval_itv3);
    itv_sub(arg2r, arg1, intern->eval_itv3);
    itv_meet(intern, arg1r, arg1r, arg1);
    itv_meet(intern, arg2r, arg2r, arg2);
    break;
  case AP_TEXPR_CAST:
    /* res = round(arg1) => arg1 = unround(res) */
    itv_unround(intern, arg1r, res, n->type, n->dir);
    itv_meet(intern, arg1r, arg1r, arg1);
    break;
  case AP_TEXPR_SQRT:
    /* res = round(sqrt(arg1)) => arg1 = unround(arg1) * unround(arg1) */
    itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
    itv_mul(intern, arg1r, intern->eval_itv3, intern->eval_itv3);
    itv_meet(intern, arg1r, arg1r, arg1);
    break;
  case AP_TEXPR_MUL:
    itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
    if (bound_sgn(arg2->inf) >= 0 && bound_sgn(arg2->sup) >= 0) {
      /* arg2 may be 0 => keep arg1 intact */
      itv_set(arg1r, arg1);
    }
    else {
      /* res = round(arg1*arg2) => arg1 = unround(res)/arg2 */
      itv_div(intern, arg1r, intern->eval_itv3, arg2);
      itv_meet(intern, arg1r, arg1r, arg1);
    }
    if (bound_sgn(arg1->inf) >= 0 && bound_sgn(arg1->sup) >= 0) {
      /* arg1 may be 0 => keep arg2 intact */
      itv_set(arg2r, arg2);
    }
    else {
      /* res = round(arg1*arg2) => arg2 = unround(res)/arg1 */
      itv_div(intern, arg2r, intern->eval_itv3, arg1);
      itv_meet(intern, arg2r, arg2r, arg2);
    }
    break;
  case AP_TEXPR_DIV:
   itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
   /* res = round(arg1/arg2) => arg1 = unround(res)*arg2 */
   itv_mul(intern, arg1r, intern->eval_itv3, arg2);
   itv_meet(intern, arg1r, arg1r, arg1);
    if (bound_sgn(intern->eval_itv3->inf) >= 0 && bound_sgn(intern->eval_itv3->sup) >= 0) {
      /* res may be 0 => keep arg2 intact */
      itv_set(arg2r, arg2);
    }
    else {
      /* res = round(arg1/arg2) => arg2 = arg1/unround(res) */
      itv_div(intern, arg2r, arg1, intern->eval_itv3);
      itv_meet(intern, arg2r, arg2r, arg2);
    }
    break;

    break;

  case AP_TEXPR_POW:
    /* unround */
    itv_unround(intern, intern->eval_itv3, res, n->type, n->dir);
    /* keep right argument */
    itv_set(arg2r, arg2);
    /* refine left argument */
    itv_inv_pow(intern, arg1r, arg1, res, arg2);
    itv_meet(intern, arg1r, arg1r, arg1);
    break;

  case AP_TEXPR_MOD:
    /* fall-back: no refinement */
    itv_set(arg1r, arg1);
    itv_set(arg2r, arg2);
    break;
  default:
    assert(0);
  }
}



/* Update arg and env with a top-down backward refinement pass.
   Return true if some variable in env has changed.
   Use intern->eval_itv, intern->eval_itv2.
 */
static bool
itv_expr_top_down(itv_internal_t* intern,
		  itv_expr_t* arg,
		  itv_t* env, size_t intdim)
{
  if (!arg) return false;
  if (itv_is_bottom(intern, arg->val)) return true;
  switch (arg->expr.discr) {
  case AP_TEXPR_CST:
    /* nothing to refine */
    break;
  case AP_TEXPR_DIM:
    itv_meet(intern, arg->val, env[arg->expr.val.dim], arg->val);
    itv_canonicalize(intern, arg->val, arg->expr.val.dim<intdim);
    if (itv_is_eq(arg->val, env[arg->expr.val.dim])) {
      return false;
    }
    else {
      itv_set(env[arg->expr.val.dim], arg->val);
      return true;
    }
    break;
  case AP_TEXPR_NODE:
    {
      bool r1 = false, r2 = false;
      ap_texpr0_node_t* n = arg->expr.val.node;
      itv_expr_t* a = (itv_expr_t*)n->exprA;
      itv_expr_t* b = (itv_expr_t*)n->exprB;
      itv_refine_ap_texpr0_node(intern, n, arg->val, a->val, b?b->val:a->val,
				intern->eval_itv, intern->eval_itv2);
      if (!itv_is_eq(intern->eval_itv, a->val)) {
	itv_set(a->val, intern->eval_itv);
	r1 = true;
      }
      if (b && !itv_is_eq(intern->eval_itv2, b->val)) {
	itv_set(b->val, intern->eval_itv2);
	r2 = true;
      }
      r1 = r1 && itv_expr_top_down(intern, a, env, intdim);
      r2 = r2 && itv_expr_top_down(intern, b, env, intdim);
      return r1 || r2;
    }
  default:
    assert(0);
  }
  return false;
}


/* Update expr and env by assuming that the condition on expr holds,
   by calling itv_expr_top_down.
   Return true if env was updated.
*/
static bool
itv_refine_cons(itv_internal_t* intern,
		itv_expr_t* expr, ap_constyp_t cons,
		itv_t* env, size_t intdim)
{
  switch (cons) {
  case AP_CONS_EQ:
    if (itv_is_zero(expr->val)) return false;
    itv_set_int(intern->eval_itv, 0);
    itv_meet(intern, expr->val, expr->val, intern->eval_itv);
    return itv_expr_top_down(intern, expr, env, intdim);
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP: /* approximated as >= */
    if (itv_is_pos(expr->val)) return false;
    itv_set_top(intern->eval_itv);
    bound_set_int(intern->eval_itv->inf, 0);
    itv_meet(intern, expr->val, expr->val, intern->eval_itv);
    return itv_expr_top_down(intern, expr, env, intdim);
  case AP_CONS_EQMOD:
  case AP_CONS_DISEQ:
    /* ignored */
    return false;
  default:
    assert(0);
  }
  return false;
}

/* Udate expr and env by assuming that expr evaluates to val,
   by calling itv_expr_top_down.
   Return true if env was updated.
*/
static bool
itv_refine_expr(itv_internal_t* intern,
		itv_expr_t* expr, itv_t val,
		itv_t* env, size_t intdim)
{
  itv_meet(intern, intern->eval_itv, expr->val, val);
  if (itv_is_eq(intern->eval_itv, expr->val)) return false;
  itv_set(expr->val, intern->eval_itv);
  return itv_expr_top_down(intern, expr, env, intdim);
}


/* Refine env by applying the constraints following a HC4-style algorithm.
   The constraints are applied in sequence, with local iterations until
   env stabilizes or max_iter is reached.
   Return true if the result is empty.
 */
bool
ITVFUN(itv_meet_ap_tcons0_array)(itv_internal_t* intern,
				 ap_tcons0_array_t* array,
				 itv_t* env, size_t intdim,
				 int max_iter)
{
  bool empty = false;
  size_t i;
  int n;
  itv_expr_t** tab = malloc(array->size*sizeof(itv_expr_t*));
  /* annotate expressions */
  for (i=0; i<array->size; i++) {
    tab[i] = itv_expr_build(intern, array->p[i].texpr0, env, intdim);
  }
  /* refine expressions and env with iteration */
  for (n=0; n<max_iter; n++) {
    bool stable = true;
    for (i=0; i<array->size; i++) {
      if (itv_refine_cons(intern, tab[i], array->p[i].constyp, env, intdim)) {
	stable = false;
      }
    }
    if (stable) break;
    stable = true;
    for (i=0; i<array->size; i++) {
      if (itv_expr_bottom_up(intern, tab[i], env, intdim)) {
	stable = false;
      }
      if (itv_is_bottom(intern, tab[i]->val)) {
	stable = true;
	empty = true;
	break;
      }
    }
    if (stable) break;
  }
  /* clean up */
  for (i=0; i<array->size; i++) {
    itv_expr_free(intern, tab[i]);
  }
  free(tab);
  return empty;
}


/* Substitute dim[i] with array[i] in arg, intersect with res, and store the
   result environment in res.
   res is used as an over-approximation of the result and must always be
   given (it may be top, though).
   Return true if the result is empty.
 */
bool
ITVFUN(itv_subst_ap_texpr0_array)(itv_internal_t* intern,
				  itv_t* res, /* in-out */
				  itv_t* arg,
				  ap_dim_t* dim,
				  ap_texpr0_t** array,
				  size_t size, /* size of dim & array */
				  size_t intdim,
				  size_t realdim,
				  int max_iter)
{
  bool empty = false;
  size_t i;
  int n;
  itv_expr_t** tab = malloc(size*sizeof(itv_expr_t*));
  char* d;
  /* intersect res with arg for dimensions not in dim */
  d = malloc(intdim+realdim);
  memset(d, 0, intdim+realdim);
  for (i=0; i<size; i++) {
    d[dim[i]] = 1;
  }
  for (i=0; i<intdim+realdim; i++) {
    if (d[dim[i]]) continue;
    itv_meet(intern, res[i], res[i], arg[i]);
  }
  free(d);
  /* annotate expressions */
  for (i=0; i<size; i++) {
    tab[i] = itv_expr_build(intern, array[i], res, intdim);
  }
  /* refine expressions and env with iteration */
  for (n=0; n<max_iter; n++) {
    bool stable = true;
    for (i=0; i<size; i++) {
      if (itv_refine_expr(intern, tab[i], arg[i], res, intdim)) {
	stable = false;
      }
    }
    if (stable) break;
    stable = true;
    for (i=0; i<size; i++) {
      if (itv_expr_bottom_up(intern, tab[i], res, intdim)) {
	stable = false;
      }
      if (itv_is_bottom(intern, tab[i]->val)) {
	stable = true;
	empty = true;
	break;
      }
    }
    if (stable) break;
  }
  /* clean up */
  for (i=0; i<size; i++) {
    itv_expr_free(intern, tab[i]);
  }
  free(tab);
  return empty;
}
