/* ********************************************************************** */
/* ap_linexprXXX_eval.c: */
/* ********************************************************************** */

#include "ap_linexprXXX_eval.h"
#include "ap_linexprXXX.h"
#include "ap_texpr0.h"
#include "num_conv.h"

#define LOGDEBUG 0

/* ********************************************************************** */
/* I. Arithmetic */
/* ********************************************************************** */

void ap_linexprXXX_neg(ap_linexprXXX_t res, ap_linexprXXX_t expr)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  eitvXXX_ptr pitv;

  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_neg(res->cst, expr->cst);
  ap_linexprXXX_ForeachLinterm0(res,i,dim,pitv){
    eitvXXX_neg(pitv,pitv);
  }
  return;
}
bool ap_linexprXXX_scale(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, num_internal_t intern)
{
  bool exact = NUMXXX_EXACT;
  size_t i;
  ap_dim_t dim;
  bool* peq;
  eitvXXX_ptr peitv;

  coeff->eq = eitvXXX_is_point(coeff);
  if (eitvXXX_is_zero(coeff)){
    eitvXXX_set_int(res->cst,0);
    ap_linexprXXX_resize(res,0);
    return true;
  }
  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_mul(res->cst,res->cst,coeff, intern);
  if (eitvXXX_is_top(res->cst)){
    ap_linexprXXX_resize(res,0);
  }
  else {
    ap_linexprXXX_ForeachLinterm0(res,i,dim,peitv){
      eitvXXX_mul(peitv,peitv,coeff, intern);
    }
  }
  return exact;
}
bool ap_linexprXXX_div(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, num_internal_t intern)
{
  bool exact = NUMXXX_DIVEXACT;
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr peitv;

  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_div(res->cst,res->cst,coeff, intern);
  if (eitvXXX_is_top(res->cst)){
    ap_linexprXXX_resize(res,0);
  }
  else {
    ap_linexprXXX_ForeachLinterm0(res,i,dim,peitv){
      eitvXXX_div(peitv,peitv,coeff,intern);
    }
  }
  return exact;
}

bool ap_linexprXXX_add(ap_linexprXXX_t res,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, num_internal_t intern)
{
  bool exact = true;
  size_t i,j,k;
  ap_linexprXXX_struct expr_struct;
  ap_linexprXXX_ptr expr;
  bool endA,endB;

  if (res==exprA || res==exprB){
    expr = &expr_struct;
    ap_linexprXXX_init(expr,exprA->effsize+exprB->effsize);
  }
  else {
    expr = res;
    ap_linexprXXX_resize(expr,exprA->effsize+exprB->effsize);
  }
  i = j = k = 0;
  endA = endB = false;
  exact = NUMXXX_EXACT || eitvXXX_is_zero(exprA->cst) || eitvXXX_is_zero(exprB->cst);
  eitvXXX_add(expr->cst,exprA->cst,exprB->cst);
  if (eitvXXX_is_top(expr->cst))
    goto _ap_linexprXXX_add_return;
  while (true){
    endA = endA || (i==exprA->effsize) || exprA->linterm[i]->dim == AP_DIM_MAX;
    endB = endB || (j==exprB->effsize) || exprB->linterm[j]->dim == AP_DIM_MAX;
    if (endA && endB)
      break;
    if (endA || (!endB && exprB->linterm[j]->dim < exprA->linterm[i]->dim)){
      ap_lintermXXX_set(expr->linterm[k], exprB->linterm[j]);
      k++; j++;
    }
    else if (endB || (!endA && exprA->linterm[i]->dim < exprB->linterm[j]->dim)){
      ap_lintermXXX_set(expr->linterm[k], exprA->linterm[i]);
      k++; i++;
    }
    else {
      exact = NUMXXX_EXACT;
      eitvXXX_add(expr->linterm[k]->eitv, exprA->linterm[i]->eitv,exprB->linterm[j]->eitv);
      expr->linterm[k]->dim = exprA->linterm[i]->dim;
      if (!eitvXXX_is_zero(expr->linterm[k]->eitv)){
	k++;
      }
      i++; j++;
    }
  }
 _ap_linexprXXX_add_return:
  expr->effsize = k;
  if (k<expr->maxsize+4)
    ap_linexprXXX_resize_strict(expr,k);

  if (res==exprA || res==exprB){
    ap_linexprXXX_clear(res);
    *res = expr_struct;
  }
  return exact;
}
bool ap_linexprXXX_sub(ap_linexprXXX_t res,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, num_internal_t intern)
{
  bool exact;
  if (exprA==exprB){
    ap_linexprXXX_t expr;
    ap_linexprXXX_init(expr,0);
    ap_linexprXXX_neg(expr,exprB);
    exact = ap_linexprXXX_add(res,exprA, expr, intern);
    ap_linexprXXX_clear(expr);
  }
  else {
    ap_linexprXXX_neg(exprB,exprB);
    exact = ap_linexprXXX_add(res,exprA,exprB, intern);
    if (exprB!=res){
      ap_linexprXXX_neg(exprB,exprB);
    }
  }
  return exact;
}

/* ********************************************************************** */
/* II. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

/* Evaluate an interval linear expression */
bool eitvXXX_eval_ap_linexpr0(
    eitvXXX_t res, ap_linexpr0_t expr, ap_linexprXXX_t env,
    num_internal_t intern
)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t coeff;
  bool exact;

  MACRO_SWITCH(expr->discr) ZZZ {
    eitvZZZ_ptr eitv;

    exact = eitvXXX_set_eitvZZZ(res,expr->linexpr.ZZZ->cst,intern);
    ap_linexprZZZ_ForeachLinterm0(expr->linexpr.ZZZ,i,dim,eitv){
      eitvXXX_ptr envdim = ap_linexprXXX_eitvref0(env,dim,false);
      if (envdim==NULL){
	if (!eitvZZZ_is_zero(eitv)){
	  eitvXXX_set_top(res);
	  break;
	}
      }
      else {
	exact = eitvXXX_set_eitvZZZ(intern->XXX.eval_eitv,eitv,intern) && exact;
	eitvXXX_mul(intern->XXX.eval_eitv, intern->XXX.eval_eitv, envdim, intern);
	eitvXXX_add(res,res,intern->XXX.eval_eitv);
	if (eitvXXX_is_top(res))
	  break;
      }
    }
  }
  ENDMACRO;
#if NUMXXX_EXACT
  return exact;
#else
  return false;
#endif
}

bool ap_linexprXXX_eval(
    eitvXXX_t res, ap_linexprXXX_t expr, ap_linexprXXX_t env,
    num_internal_t intern
)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr eitv;

  assert(env);

  eitvXXX_set(res, expr->cst);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    eitvXXX_ptr envdim = ap_linexprXXX_eitvref0(env,dim,false);
    if (envdim==NULL){
      if (!eitvXXX_is_zero(eitv)){
	eitvXXX_set_top(res);
	break;
      }
    }
    else {
      eitvXXX_mul(intern->XXX.eval_eitv,envdim, eitv, intern);
      eitvXXX_add(res,res,intern->XXX.eval_eitv);
      if (eitvXXX_is_top(res))
	break;
    }
  }
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}

/* ====================================================================== */
/* II.2 Quasilinearisation */
/* ====================================================================== */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
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
eitvXXX_quasilinearize_choose_middle(
    numXXX_t middle, /* the result */
    eitvXXX_t coeff, /* the coefficient in which
			middle is to be picked */
    eitvXXX_t var,   /* the variable interval */
    bool for_meet_inequality /* is it for the
				linearisation of
				an inequality ? */
)
{
  if (boundXXX_infty(coeff->itv->neginf)){
    if (boundXXX_infty(coeff->itv->sup))
      numXXX_set_int(middle,0);
    else
      numXXX_set(middle,
		 boundXXX_numref(coeff->itv->sup));
  }
  else if (boundXXX_infty(coeff->itv->sup))
    numXXX_neg(middle,
	       boundXXX_numref(coeff->itv->neginf));
  else {
    /* if coeff = [inf,sup] */
    if (for_meet_inequality){
      if (boundXXX_infty(var->itv->neginf))
	numXXX_neg(middle,
		   boundXXX_numref(coeff->itv->neginf));
      else if (boundXXX_infty(var->itv->sup))
	numXXX_set(middle,
		   boundXXX_numref(coeff->itv->sup));
      else /* Arbitrary choice: we take the middle */
	goto eitvXXX_quasilinearize_choose_middle_default;
    }
    else {
      if (boundXXX_infty(var->itv->neginf) ?
	  !boundXXX_infty(var->itv->sup) :
	  boundXXX_infty(var->itv->sup)){
	if (boundXXX_sgn(coeff->itv->neginf)<=0)
	  numXXX_neg(middle,
		     boundXXX_numref(coeff->itv->neginf));
	else if (boundXXX_sgn(coeff->itv->sup)<=0)
	  numXXX_set(middle,
		     boundXXX_numref(coeff->itv->sup));
	else /* Arbitrary choice: we take the middle */
	  goto eitvXXX_quasilinearize_choose_middle_default;
      }
      else {
      eitvXXX_quasilinearize_choose_middle_default:
	numXXX_sub(middle,
		   boundXXX_numref(coeff->itv->sup),
		   boundXXX_numref(coeff->itv->neginf));
	numXXX_mul_2exp(middle,middle,-1);
      }
    }
  }
}

bool ap_linexprXXX_quasilinearize(ap_linexprXXX_t linexpr, ap_linexprXXX_t env,
				  bool for_meet_inequality, num_internal_t intern)
{
  ap_dim_t size,ir,iw,dim;
  eitvXXX_ptr eitv;
  bool top;

#if LOGDEBUG
  printf("ap_linexprXXX_quasilinearize:\n");
  ap_linexprXXX_print(linexpr,0); printf("\n");
#endif
  top = false;
  ir = iw = 0;
  while (ir<linexpr->effsize){
    dim = linexpr->linterm[ir]->dim;
    eitv = linexpr->linterm[ir]->eitv;
    eitvXXX_ptr envdim = ap_linexprXXX_eitvref0(env,dim,false);
    if (envdim==NULL){
      if (!eitvXXX_is_zero(eitv)){
	eitvXXX_set_top(linexpr->cst);
	top = true;
	break;
      }
      else {
	ir++;
      }
    }
    else if (eitvXXX_is_point(envdim)){
      /* If a variable has a constant value, simplification */
      eitvXXX_mul_num(eitv,eitv,boundXXX_numref(envdim->itv->sup));
      eitvXXX_add(linexpr->cst,linexpr->cst,eitv);
      ir++;
    }
    else if (eitvXXX_is_point(eitv)){
      if (iw<ir){
	eitvXXX_set(linexpr->linterm[iw]->eitv,eitv);
	linexpr->linterm[iw]->dim = dim;
      }
      ir++; iw++;
    }
    else {
      /* Compute the middle of the interval */
      eitvXXX_quasilinearize_choose_middle(intern->XXX.quasi_num,
					   eitv,envdim,for_meet_inequality);
      /* Residue (interval-middle) */
      eitvXXX_sub_num(intern->XXX.eval_eitv2,eitv,intern->XXX.quasi_num);
      /* Multiplication of residue by variable range */
      eitvXXX_mul(intern->XXX.eval_eitv,
		  intern->XXX.eval_eitv2,
		  envdim, intern);
      /* Addition to the constant coefficient */
      eitvXXX_add(linexpr->cst,linexpr->cst,intern->XXX.eval_eitv);
      if (eitvXXX_is_top(linexpr->cst)){
	top = true;
	break;
      }
      /* Modification of the linear term */
      if (numXXX_sgn(intern->XXX.quasi_num)!=0){
	eitvXXX_set_num((iw==ir) ? eitv : linexpr->linterm[iw]->eitv,intern->XXX.quasi_num);
	linexpr->linterm[iw]->dim = dim;
	ir++; iw++;
      }
      else {
	ir++;
      }
    }
  }
  linexpr->effsize = top ? 0 : iw;

#if LOGDEBUG
  ap_linexprXXX_print(linexpr,NULL); printf("\n");
#endif
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}

bool ap_linexprXXX_array_quasilinearize(ap_linexprXXX_array_t array, ap_linexprXXX_t env, num_internal_t intern)
{
  size_t i;
  bool res;
  res = true;
  for (i=0; i<array->size; i++) {
    ap_linexprXXX_quasilinearize(array->p[i],env,false, intern);
  }
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}

/* ********************************************************************** */
/* III. Evaluation and interval linearisation of tree expressions */
/* ********************************************************************** */

/* ====================================================================== */
/* III.1 Conversion to interval linear expression */
/* ====================================================================== */

/* preconditions:
   - lres is initialised

   postconditions:
   - stores an interval linear form in lres
   - puts perror to true if input expression is not interval linear
*/
static
bool ap_linexprXXX_set_texpr0_node(ap_linexprXXX_t lres, bool* perror,
				   ap_texpr0_node_t* n,
				   num_internal_t intern)
{
  bool exact;
  eitvXXX_t i1;
  ap_linexprXXX_t l1;

  *perror = n->type!=AP_RTYPE_REAL;
  if (*perror) return false;
  switch (n->op) {
  case AP_TEXPR_NEG:
    exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprA,intern);
    ap_linexprXXX_neg(lres,lres);
    return exact;
  case AP_TEXPR_CAST:
    return ap_linexprXXX_set_texpr0(lres,perror,n->exprA,intern);
  case AP_TEXPR_MOD:
  case AP_TEXPR_SQRT:
    *perror = true;
    return false;
  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
    exact = NUMXXX_EXACT;
    ap_linexprXXX_init(l1,0);

    /* intlinearize arguments */
    exact = ap_linexprXXX_set_texpr0(l1,perror,n->exprA,intern);
    if (*perror){ exact=false; goto ap_linexprXXX_set_texpr0_node_endA; }
    if (l1->effsize==0 && eitvXXX_is_bottom(l1->cst)){
      exact = true;
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set_bottom(lres->cst);
      goto ap_linexprXXX_set_texpr0_node_endA;
    }

    exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprB,intern) && exact;
    if (*perror){exact=false;  goto ap_linexprXXX_set_texpr0_node_endA; }
    if (lres->effsize==0 && eitvXXX_is_bottom(lres->cst)){
      exact = true;
      goto ap_linexprXXX_set_texpr0_node_endA;
    }
    /* add/sub linear form & interval */
    if (n->op==AP_TEXPR_ADD)
      exact = ap_linexprXXX_add(lres,l1,lres,intern) && exact;
    else
      exact = ap_linexprXXX_sub(lres,l1,lres,intern) && exact;
  ap_linexprXXX_set_texpr0_node_endA:
    ap_linexprXXX_clear(l1);
    return exact;
  case AP_TEXPR_MUL:
  case AP_TEXPR_DIV:
    eitvXXX_init(i1);
    if (ap_texpr0_is_interval_cst(n->exprB)){
      exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprA,intern);
      if (*perror){ exact=false; goto ap_linexprXXX_set_texpr0_node_endB; }
      exact = eitvXXX_eval_ap_texpr0(i1,n->exprB,NULL,intern) && exact;
    }
    else if (n->op == AP_TEXPR_MUL && ap_texpr0_is_interval_cst(n->exprA)){
      exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprB,intern);
      if (*perror){ exact=false; goto ap_linexprXXX_set_texpr0_node_endB; }
      exact = eitvXXX_eval_ap_texpr0(i1,n->exprA,NULL,intern) && exact;
      if (eitvXXX_is_bottom(i1)){
	exact = true;
	ap_linexprXXX_resize(lres,0);
	eitvXXX_set_bottom(lres->cst);
	goto ap_linexprXXX_set_texpr0_node_endB;
      }
    }
    else {
      *perror = true; exact = false;
      goto ap_linexprXXX_set_texpr0_node_endB;
    }
    if (lres->effsize==0 && eitvXXX_is_bottom(lres->cst)){
      exact = true;
      goto ap_linexprXXX_set_texpr0_node_endB;
    }
    if (eitvXXX_is_bottom(i1)){
      exact = true;
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set_bottom(lres->cst);
      goto ap_linexprXXX_set_texpr0_node_endB;
    }
    if (n->op==AP_TEXPR_DIV){
      eitvXXX_t i2;
      eitvXXX_init(i2);
      eitvXXX_set_int(i2,1);
      eitvXXX_div(i1,i2,i1,intern);
      eitvXXX_clear(i2);
    }
    exact = ap_linexprXXX_scale(lres,lres,i1,intern) && exact;
  ap_linexprXXX_set_texpr0_node_endB:
    eitvXXX_clear(i1);
    return exact;
  default:
    assert(0);
    return false;
  }
}

struct ap_texpr0_t;
bool ap_linexprXXX_set_texpr0(ap_linexprXXX_t lres, bool* perror, struct ap_texpr0_t* expr, num_internal_t intern)
{
  ap_linexprXXX_t r;
  bool exact;

  *perror = false;
  switch(expr->discr){
  case AP_TEXPR_CST:
    ap_linexprXXX_resize(lres,0);
    exact = eitvXXX_set_ap_coeff(lres->cst,expr->val.cst,intern);
    break;
  case AP_TEXPR_DIM:
    exact = true;
    ap_linexprXXX_resize(lres,1);
    lres->effsize = 1;
    eitvXXX_set_int(lres->cst,0);
    lres->linterm[0]->dim = expr->val.dim;
    eitvXXX_set_int(lres->linterm[0]->eitv,1);
    break;
  case AP_TEXPR_NODE:
    if (ap_texpr0_is_interval_cst(expr)){
      ap_linexprXXX_resize(lres,0);
      exact = eitvXXX_eval_ap_texpr0(lres->cst,expr,NULL,intern);
    }
    else {
      exact = ap_linexprXXX_set_texpr0_node(lres,perror,expr->val.node,intern);
    }
    break;
  default:
    abort();
  }
  return exact;
}
bool ap_linexprXXX_array_set_texpr0_array(
    ap_linexprXXX_array_t linexpr, bool* perror,
    struct ap_texpr0_array_t* expr, num_internal_t intern)
{
  unsigned int i;
  bool error,exact;

  ap_linexprXXX_array_resize(linexpr,expr->size);
  exact = true;
  for (i=0;i<linexpr->size; i++){
    exact = ap_linexprXXX_set_texpr0(linexpr->p[i],&error,expr->p[i],intern) && exact;
    if (error) break;
  }
  return exact;
}

/* ====================================================================== */
/* III.2 Evaluation of tree expressions */
/* ====================================================================== */

/* General rounding */
static void eitvXXX_round(eitvXXX_t res, eitvXXX_t arg,
			  ap_texpr_rtype_t t, ap_texpr_rdir_t d,
			  num_internal_t intern)
{
  switch (t) {

  case AP_RTYPE_REAL:
    if (res!=arg) eitvXXX_set(res,arg);
    break;

  case AP_RTYPE_INT:
    switch (d) {
    case AP_RDIR_ZERO:
      eitvXXX_trunc(res,arg);
      break;
    case AP_RDIR_UP:
      eitvXXX_ceil(res,arg);
      break;
    case AP_RDIR_DOWN:
      eitvXXX_floor(res,arg);
      break;
    case AP_RDIR_RND:
    case AP_RDIR_NEAREST: /* 'to nearest' case could be improved */
      eitvXXX_to_int(res,arg);
      break;
    default:
      assert(0);
    }
    break;

  case AP_RTYPE_SINGLE:
    /* directed rounding cases (+oo, -oo, 0) could be improved */
    eitvXXX_to_float(res,arg,intern);
    break;

  case AP_RTYPE_QUAD:     /* 'round to quad' could be improved */
  case AP_RTYPE_EXTENDED: /* 'round to extended' could be improved */
  case AP_RTYPE_DOUBLE:
    /* directed rounding cases (+oo, -oo, 0) could be improved */
    eitvXXX_to_double(res,arg,intern);
    break;

  default:
    assert(0);
  }
}

static bool
eitvXXX_eval_ap_texpr0_node(eitvXXX_t res,
			    ap_texpr0_node_t* n,
			    eitvXXX_t arg1, eitvXXX_t arg2,
			    num_internal_t intern)
{
  bool exact = true;
  switch (n->op) {
  case AP_TEXPR_NEG:
    eitvXXX_neg(res, arg1);
    return true; /* no rounding */
  case AP_TEXPR_CAST:
    eitvXXX_set(res, arg1);
    exact = true;
    break;
  case AP_TEXPR_SQRT:
    exact = eitvXXX_sqrt(res, arg1, intern);
    if (eitvXXX_is_bottom(res)) return false;
    break;
  case AP_TEXPR_ADD:
    eitvXXX_add(res, arg1, arg2);
    exact = NUMXXX_EXACT;
    break;
  case AP_TEXPR_SUB:
    eitvXXX_sub(res, arg1, arg2);
    exact = NUMXXX_EXACT;
    break;
  case AP_TEXPR_MUL:
    eitvXXX_mul(res, arg1, arg2, intern);
    exact = NUMXXX_EXACT;
    break;
  case AP_TEXPR_DIV:
    eitvXXX_div(res, arg1, arg2, intern);
    exact = NUMXXX_DIVEXACT;
    break;
  case AP_TEXPR_MOD:
    eitvXXX_mod(res, arg1, arg2, n->type==AP_RTYPE_INT, intern);
    return false; /* no rounding */
  default:
    assert(0);
  }
  eitvXXX_round(res,res,n->type,n->dir,intern);
  exact = exact && (n->type==AP_RTYPE_REAL);
  return exact;
}


bool eitvXXX_eval_ap_texpr0(eitvXXX_t res, struct ap_texpr0_t* expr, ap_linexprXXX_t env, num_internal_t intern)
{
  bool exact;
  switch(expr->discr){
  case AP_TEXPR_CST:
    exact = eitvXXX_set_ap_coeff(res,expr->val.cst,intern);
    break;
  case AP_TEXPR_DIM:
    ap_linexprXXX_get_eitv0(res,env,expr->val.dim);
    exact = true;
    break;
  case AP_TEXPR_NODE:
    if (expr->val.node->exprB) {
      /* binary */
      eitvXXX_t x;
      eitvXXX_init(x);
      exact = eitvXXX_eval_ap_texpr0(x,expr->val.node->exprA,env,intern);
      exact = eitvXXX_eval_ap_texpr0(res,expr->val.node->exprB,env,intern) && exact;
      if (eitvXXX_is_bottom(x) || eitvXXX_is_bottom(res)){
	eitvXXX_set_bottom(res);
      }
      else {
	exact = eitvXXX_eval_ap_texpr0_node(res,expr->val.node,x,res,intern);
      }
      eitvXXX_clear(x);
    }
    else {
      /* unary */
      exact = eitvXXX_eval_ap_texpr0(res,expr->val.node->exprA,env,intern);
      if (!eitvXXX_is_bottom(res)){
	eitvXXX_eval_ap_texpr0_node(res,expr->val.node,res,res,intern) && exact;
      }
    }
    break;
  default:
    abort();
  }
  return exact;
}

/* ====================================================================== */
/* III.3 Linearisation of tree expressions */
/* ====================================================================== */


/* transform in-place
	[A0,B0] + sum Xi [Ai,Bi]
   into
       ([A0,B0] + [A0,B0][-ulp,ulp]) + [-mf,mf] +
       sum Xi ([Ai,Bi] + [Ai,Bi][-ulp,ulp])

   i.e., add a relative error of magnitude ulp as an interval linear form
*/
static void
ap_linexprXXX_round_float_lin(ap_linexprXXX_t l /* in/out */,
			      numXXX_floatconst* f,
			      num_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr peitv;
  eitvXXX_magnitude(intern->XXX.linear_bound,l->cst);
  boundXXX_mul(intern->XXX.linear_bound,intern->XXX.linear_bound,f->ulp->sup);
  boundXXX_add(intern->XXX.linear_bound,intern->XXX.linear_bound,f->min->sup);
  eitvXXX_enlarge_bound(l->cst,l->cst,intern->XXX.linear_bound);
  ap_linexprXXX_ForeachLinterm0(l,i,dim,peitv) {
    eitvXXX_magnitude(intern->XXX.linear_bound,peitv);
    boundXXX_mul(intern->XXX.linear_bound,intern->XXX.linear_bound,f->ulp->sup);
    eitvXXX_enlarge_bound(peitv,peitv,intern->XXX.linear_bound);
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
ap_linexprXXX_round_float(ap_linexprXXX_t l /* in/out */,
			  ap_linexprXXX_t env,
			  numXXX_floatconst* f, num_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr peitv;
  eitvXXX_magnitude(intern->XXX.linear_bound,l->cst);
  ap_linexprXXX_ForeachLinterm0(l,i,dim,peitv) {
    eitvXXX_ptr envdim = ap_linexprXXX_eitvref0(env,dim,false);
    eitvXXX_magnitude(intern->XXX.linear_bound2,peitv);
    if (envdim){
      eitvXXX_magnitude(intern->XXX.linear_bound3,envdim);
    } else {
      boundXXX_set_infty(intern->XXX.linear_bound3,+1);
    }
    boundXXX_mul(intern->XXX.linear_bound2,intern->XXX.linear_bound2,intern->XXX.linear_bound3);
    boundXXX_add(intern->XXX.linear_bound,intern->XXX.linear_bound,intern->XXX.linear_bound2);
  }
  boundXXX_mul(intern->XXX.linear_bound,intern->XXX.linear_bound,f->ulp->sup);
  boundXXX_add(intern->XXX.linear_bound,intern->XXX.linear_bound,f->min->sup);
  eitvXXX_enlarge_bound(l->cst,l->cst,intern->XXX.linear_bound);
}

/* adds an absolute error to l corresponding to a conversion to int
   assumes that i overapproximates the values of l before conversion
 */
static void
ap_linexprXXX_texpr0_to_int(ap_linexprXXX_t l /* in/out */,
			    eitvXXX_t i /* in */,
			    ap_texpr_rdir_t d,
			    num_internal_t intern)
{
  switch (d) {
  case AP_RDIR_UP:
    /* add [0,1] */
    boundXXX_add_uint(l->cst->itv->sup,l->cst->itv->sup,1);
    l->cst->eq = false;
    break;
  case AP_RDIR_DOWN:
    /* add [-1,0] */
    boundXXX_add_uint(l->cst->itv->neginf,l->cst->itv->neginf,1);
    l->cst->eq = false;
    break;
  case AP_RDIR_RND:
    /* add [-1,1] */
    boundXXX_add_uint(l->cst->itv->sup,l->cst->itv->sup,1);
    boundXXX_add_uint(l->cst->itv->neginf,l->cst->itv->neginf,1);
    l->cst->eq = false;
    break;
  case AP_RDIR_ZERO:
    /* UP or DOWN or RND, depending on sign of i */
    if (boundXXX_sgn(i->itv->neginf)>0) boundXXX_add_uint(l->cst->itv->sup,l->cst->itv->sup,1);
    if (boundXXX_sgn(i->itv->sup)>0) boundXXX_add_uint(l->cst->itv->neginf,l->cst->itv->neginf,1);
    l->cst->eq = l->cst->eq && itvXXX_is_point(l->cst->itv);
    break;
  case AP_RDIR_NEAREST:
    /* [-0.5,0.5] */
    eitvXXX_add(l->cst,l->cst,intern->XXX.eitvXXX_half);
    break;
  default:
    assert(0);
  }
}

/* adds rounding error to both l and i to go from type org to type dst */
static ap_texpr_rtype_t
ap_linexprXXX_texpr0_round(ap_linexprXXX_t l /* in/out */,
			   eitvXXX_t i /* in/out */,
			   ap_texpr_rtype_t org,
			   ap_texpr_rtype_t dst, ap_texpr_rdir_t d,
			   num_internal_t intern)
{
  if (dst==AP_RTYPE_REAL) return org;
  switch (dst) {
  case AP_RTYPE_INT:
    if (org==AP_RTYPE_INT) return org;
    ap_linexprXXX_texpr0_to_int(l,i,d, intern);
    break;
  case AP_RTYPE_SINGLE:
    if (org==AP_RTYPE_SINGLE) return org;
    ap_linexprXXX_round_float_lin(l,&intern->XXX.cst_single, intern);
    break;
  case AP_RTYPE_DOUBLE:
    if (org==AP_RTYPE_SINGLE || org==AP_RTYPE_DOUBLE) return org;
    ap_linexprXXX_round_float_lin(l,&intern->XXX.cst_double, intern);
    break;
  case AP_RTYPE_EXTENDED:
    if (org==AP_RTYPE_SINGLE && org==AP_RTYPE_DOUBLE && org==AP_RTYPE_EXTENDED)
      return org;
    ap_linexprXXX_round_float_lin(l,&intern->XXX.cst_extended, intern);
    break;
  case AP_RTYPE_QUAD:
    if (org==AP_RTYPE_SINGLE || org==AP_RTYPE_DOUBLE ||
	org==AP_RTYPE_EXTENDED || org==AP_RTYPE_QUAD) return org;
    ap_linexprXXX_round_float_lin(l,&intern->XXX.cst_quad, intern);
    break;
  default:
    assert(0);
  }
  eitvXXX_round(i,i,dst,d,intern);
  return dst;
}

/* reduce l and i:
   - intersect i with the interval evaluation of l
   - if l is constant, replace it with i
   - check for emptiness
 */
static void
ap_linexprXXX_texpr0_reduce(ap_linexprXXX_t l /* in/out */, eitvXXX_t i /* in/out */,
			    ap_linexprXXX_t env,
			    num_internal_t intern)
{
  eitvXXX_t tmp;
  eitvXXX_init(tmp);
  ap_linexprXXX_eval(tmp,l,env, intern);
  eitvXXX_meet(i,i,tmp);
  if (eitvXXX_is_bottom(i) || eitvXXX_is_bottom(l->cst)) {
    eitvXXX_set_bottom(i);
    eitvXXX_set_bottom(l->cst);
    if (l->effsize>0) ap_linexprXXX_resize(l,0);
  }
  else if (l->effsize==0){
    eitvXXX_set(l->cst,i);
  }
  eitvXXX_clear(tmp);
}

/* multiplication heuristic: choose which interval to keep (0=a, 1=b) */
static int
ap_linexprXXX_texpr0_cmp_range(ap_linexprXXX_t la, eitvXXX_t ia,
			       ap_linexprXXX_t lb, eitvXXX_t ib,
			       num_internal_t intern)
{
  int sgn_a,sgn_b;
  /* if one linear form is an interval keep it */
  if (la->effsize==0) return 0;
  if (lb->effsize==0) return 1;
  /* if only one interval has constant sign, keep it */
  sgn_a = eitvXXX_is_pos(ia) || eitvXXX_is_neg(ia);
  sgn_b = eitvXXX_is_pos(ib) || eitvXXX_is_neg(ib);
  if (sgn_a!=sgn_b) return sgn_a ? 0 : 1;
  /* otherwise, keep the interval with the smallest relative range */
  eitvXXX_range_rel(intern->XXX.linear_bound,ia, intern);
  eitvXXX_range_rel(intern->XXX.linear_bound2,ib, intern);
  if (boundXXX_cmp(intern->XXX.linear_bound,intern->XXX.linear_bound2)<0) return 0;
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
ap_linexprXXX_intlinearize_texpr0_rec(ap_linexprXXX_t lres /* out */,
				      eitvXXX_t ires /* out */,
				      ap_texpr0_t* expr,
				      ap_linexprXXX_t env, size_t intdim,
				      num_internal_t intern);

static int debug_indent = 0;

static ap_texpr_rtype_t
ap_linexprXXX_intlinearize_texpr0_node(ap_linexprXXX_t lres /* out */, eitvXXX_t ires /* out */,
				       ap_texpr0_node_t* n,
				       ap_linexprXXX_t env, size_t intdim,
				       num_internal_t intern)
{
  eitvXXX_t i1,i2;
  ap_linexprXXX_t l1;
  ap_texpr_rtype_t t1,t2;

  switch (n->op) {
  case AP_TEXPR_NEG:
  case AP_TEXPR_CAST:
  case AP_TEXPR_SQRT:
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    if (!eitvXXX_is_bottom(ires)){
      assert(!eitvXXX_is_bottom(lres->cst));
      switch (n->op){
      case AP_TEXPR_NEG:
	/* negate linear form & interval, no rounding */
	ap_linexprXXX_neg(lres,lres);
	eitvXXX_neg(ires,ires);
	return t1;
      case AP_TEXPR_CAST:
	/* round linear form & interval */
	ap_linexprXXX_texpr0_round(lres,ires,t1,n->type,n->dir, intern);
	ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
	return n->type;
      case AP_TEXPR_SQRT:
	/* intlinearize argument, lres is not used */
	/* interval square root */
	eitvXXX_sqrt(ires,ires, intern);
	if (!eitvXXX_is_bottom(ires))
	  eitvXXX_round(ires,ires,n->type,n->dir,intern);
	ap_linexprXXX_resize(lres,0);
	eitvXXX_set(lres->cst,ires);
	return n->type;
      default: abort();
      }
    }
    else {
      assert(lres->effsize==0 && eitvXXX_is_bottom(lres->cst));
      return n->type;
    }
    abort();
  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
  case AP_TEXPR_MUL:
  case AP_TEXPR_DIV:
  case AP_TEXPR_MOD:
    eitvXXX_init(i1);
    ap_linexprXXX_init(l1,0);
    /* intlinearize arguments */
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(l1,i1,n->exprA,env,intdim, intern);
    t2 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprB,env,intdim, intern);
    if (eitvXXX_is_bottom(i1) || eitvXXX_is_bottom(ires)){
      eitvXXX_set_bottom(ires);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
    } else {
      switch (n->op){
	/* add/sub linear form & interval */
      case AP_TEXPR_ADD:
	ap_linexprXXX_add(lres,l1,lres, intern);
	eitvXXX_add(ires,i1,ires);
	break;
      case AP_TEXPR_SUB:
	ap_linexprXXX_sub(lres,l1,lres, intern);
	eitvXXX_sub(ires,i1,ires);
	break;
      case AP_TEXPR_MUL:
	/* multiply one linear form with the other interval */
	if (ap_linexprXXX_texpr0_cmp_range(l1,i1,lres,ires, intern))  {
	  /* res = ires * l1 */
#if LOGDEBUG
	  printf("%*s lin * inter\n",2*debug_indent,"");
#endif
	  ap_linexprXXX_clear(lres);
	  *lres = *l1;
	  ap_linexprXXX_scale(lres,lres,ires, intern);
	}
	else {
	  /* res = i1 * lres */
#if LOGDEBUG
	  printf("%*s inter * lin\n",2*debug_indent,"");
#endif
	  ap_linexprXXX_clear(l1);
	  ap_linexprXXX_scale(lres,lres,i1, intern);
	}
	eitvXXX_mul(ires,i1,ires, intern);
	break;
      case AP_TEXPR_DIV:
	/* divide linear form & interval */
	ap_linexprXXX_div(lres,l1,ires, intern);
	eitvXXX_div(ires,i1,ires, intern);
	break;
      case AP_TEXPR_MOD:
	/* interval modulo, no rounding */
	eitvXXX_mod(ires,i1,ires,n->type==AP_RTYPE_INT, intern);
	ap_linexprXXX_resize(lres,0);
	eitvXXX_set(lres->cst,ires);
	break;
      default: abort();
      }
      if (n->op != AP_TEXPR_MOD){
	/* round */
	ap_linexprXXX_texpr0_round(lres,ires,
				   (t1==AP_RTYPE_INT && t2==AP_RTYPE_INT) ?
				   AP_RTYPE_INT : AP_RTYPE_REAL,
				   n->type,n->dir,intern);
	/* reduce */
	ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
      }
      eitvXXX_clear(i1);
      if (n->op != AP_TEXPR_MUL){
	ap_linexprXXX_clear(l1);
      }
    }
    return n->type;
  default: abort();
  }
}

static ap_texpr_rtype_t
ap_linexprXXX_intlinearize_texpr0_rec(ap_linexprXXX_t lres /* out */,
				      eitvXXX_t ires /* out */,
				      ap_texpr0_t* expr,
				      ap_linexprXXX_t env, size_t intdim,
				      num_internal_t intern)
{
  ap_linexprXXX_t r;
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
    eitvXXX_set_ap_coeff(ires,expr->val.cst, intern);
    ap_linexprXXX_resize(lres,0);
    eitvXXX_set(lres->cst,ires);
    t = eitvXXX_is_int(lres->cst, intern) ? AP_RTYPE_INT : AP_RTYPE_REAL;
    break;
  case AP_TEXPR_DIM:
    ap_linexprXXX_get_eitv0(ires,env,expr->val.dim);
    ap_linexprXXX_resize(lres,1);
    lres->effsize = 1;
    eitvXXX_set_int(lres->cst,0);
    lres->linterm[0]->dim = expr->val.dim;
    eitvXXX_set_int(lres->linterm[0]->eitv,1);
    t = (expr->val.dim<intdim) ? AP_RTYPE_INT : AP_RTYPE_REAL;
    break;
  case AP_TEXPR_NODE:
    t = ap_linexprXXX_intlinearize_texpr0_node(lres,ires,expr->val.node,env,intdim, intern);
    break;
  default:
    t = 0;
    assert(false);
  }

#if LOGDEBUG
  debug_indent--;
  printf("%*s   = ",2*debug_indent,"");
  ap_linexprXXX_print(lres,NULL);
  printf(" /\\ ");
  eitvXXX_print(ires);
  printf(" t=%i\n",t);
#endif

  return t;
}

void ap_linexprXXX_intlinearize_texpr0(ap_linexprXXX_t res,
				       ap_texpr0_t* expr,
				       ap_linexprXXX_t env, size_t intdim,
				       num_internal_t intern)
{
  eitvXXX_t i;
  eitvXXX_init(i);
  ap_linexprXXX_intlinearize_texpr0_rec(res,i,expr,env,intdim,intern);
  if (!eitvXXX_is_bottom(i) && !eitvXXX_is_bottom(res->cst)) {
    if (res->effsize==0){
      eitvXXX_meet(res->cst,res->cst,i);
    }
  }
  else {
    ap_linexprXXX_resize(res,0);
    eitvXXX_set_bottom(res->cst);
  }
  eitvXXX_clear(i);
}
