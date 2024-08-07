/* ********************************************************************** */
/* itv_linexpr.c: */
/* ********************************************************************** */

#include "itv_linexpr.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void ITVFUN(itv_linexpr_init)(itv_linexpr_t* expr, size_t size)
{
  expr->linterm = NULL;
  expr->size = 0;
  itv_init(expr->cst);
  expr->equality = true;
  itv_linexpr_reinit(expr,size);
}
void ITVFUN(itv_linexpr_init_set)(itv_linexpr_t* res, itv_linexpr_t* expr)
{
  size_t i;

  itv_init_set(res->cst,expr->cst);
  res->equality = expr->equality;
  res->linterm = expr->size ? malloc(expr->size*sizeof(itv_linterm_t)) : NULL;
  for (i=0;i<expr->size;i++){
    itv_linterm_init_set(&res->linterm[i],&expr->linterm[i]);
  }
  res->size = expr->size;
}
void ITVFUN(itv_linexpr_set)(itv_linexpr_t* res, itv_linexpr_t* expr)
{
  size_t i,size;

  if (res==expr) return;

  itv_set(res->cst,expr->cst);
  res->equality = expr->equality;
  for  (i=expr->size; i<res->size; i++){
    itv_linterm_clear(&res->linterm[i]);
  }
  res->linterm = realloc(res->linterm, expr->size ? expr->size*sizeof(itv_linterm_t) : 1);
  size = res->size < expr->size ? res->size : expr->size;
  for (i=0;i<size;i++){
    itv_linterm_set(&res->linterm[i],&expr->linterm[i]);
  }
  for (i=size; i<expr->size;i++){
    itv_linterm_init_set(&res->linterm[i],&expr->linterm[i]);
  }
  res->size = expr->size;
}

void ITVFUN(itv_linexpr_reinit)(itv_linexpr_t* expr, size_t size)
{
  size_t i;

  for  (i=size; i<expr->size; i++){
    itv_linterm_clear(&expr->linterm[i]);
  }
  expr->linterm = realloc(expr->linterm, size ? size*sizeof(itv_linterm_t) : 1);
  for (i=expr->size;i<size;i++){
    itv_linterm_init(&expr->linterm[i]);
  }
  expr->size = size;
  return;
}
void ITVFUN(itv_linexpr_clear)(itv_linexpr_t* expr)
{
  size_t i;
  if (expr->linterm){
    for (i=0;i<expr->size;i++){
      itv_linterm_clear(&expr->linterm[i]);
    }
    free(expr->linterm);
    expr->linterm = NULL;
    expr->size = 0;
  }
  itv_clear(expr->cst);
}

void ITVFUN(itv_linexpr_fprint)(FILE* stream, itv_linexpr_t* expr, char** name)
{
  size_t i;
  ap_dim_t dim;
  itv_ptr pitv;
  bool* peq;
  (void)peq;
  itv_fprint(stream,expr->cst);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
    fprintf(stream, " + ");
    itv_fprint(stream,pitv);
    if (name) fprintf(stream,"%s",name[dim]);
    else fprintf(stream,"x%lu",(unsigned long)dim);
    (void)peq;
  }
}
void ITVFUN(itv_linexpr_print)(itv_linexpr_t* expr, char** name)
{
  ITVFUN(itv_linexpr_fprint)(stdout,expr,name);
}

void ITVFUN(itv_lincons_set_bool)(itv_lincons_t* lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  itv_linexpr_reinit(&lincons->linexpr,0);
  itv_set_int(lincons->linexpr.cst,value ? 0 : 1);
  lincons->linexpr.equality = true;
  lincons->constyp = AP_CONS_EQ;
}
void ITVFUN(itv_lincons_fprint)(FILE* stream, itv_lincons_t* cons, char** name)
{
  ITVFUN(itv_linexpr_fprint)(stream,&cons->linexpr,name);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	    (cons->constyp == AP_CONS_DISEQ ?
	     " != 0" :
	     "\"ERROR in itv_lincons_fprint\""))));
  if (cons->constyp == AP_CONS_EQMOD){
    fprintf(stream," mod ");
    num_fprint(stream,cons->num);
  }
}
void ITVFUN(itv_lincons_print)(itv_lincons_t* cons, char** name)
{
  ITVFUN(itv_lincons_fprint)(stdout,cons,name);
}

void ITVFUN(itv_lincons_array_init)(itv_lincons_array_t* array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = malloc(size*sizeof(itv_lincons_t));
  for (i=0; i<size; i++) itv_lincons_init(&array->p[i]);
}
void ITVFUN(itv_lincons_array_reinit)(itv_lincons_array_t* array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      itv_lincons_clear(&array->p[i]);
    }
    array->p = realloc(array->p, size ? size*sizeof(itv_lincons_t) : 1);
  }
  else { /* size > array->size */
    array->p = realloc(array->p, size ? size*sizeof(itv_lincons_t) : 1);
    for (i=array->size; i<size; i++){
      itv_lincons_init(&array->p[i]);
    }
  }
  array->size = size;
  return;
}
void ITVFUN(itv_lincons_array_clear)(itv_lincons_array_t* array)
{
  size_t i;
  for (i=0; i<array->size; i++) itv_lincons_clear(&array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}
void ITVFUN(itv_lincons_array_fprint)(FILE* stream, itv_lincons_array_t* array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    itv_lincons_fprint(stream,&array->p[i],name);
    fprintf(stream,"\n");
  }
}
void ITVFUN(itv_lincons_array_print)(itv_lincons_array_t* array, char** name)
{
  ITVFUN(itv_lincons_array_fprint)(stdout,array,name);
}

/* ********************************************************************** */
/* II. Conversions from and to APRON datatypes */
/* ********************************************************************** */

bool ITVFUN(itv_linexpr_set_ap_linexpr0)(itv_internal_t* intern,
					 itv_linexpr_t* expr,
					 ap_linexpr0_t* linexpr0)
{

  size_t i,k,size;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  bool res,exact, eq;

  size=0;
  ap_linexpr0_ForeachLinterm(linexpr0,i,dim,coeff){
    size++;
  }
  itv_linexpr_reinit(expr,size);
  exact = itv_set_ap_coeff(intern, expr->cst, &linexpr0->cst);
  expr->equality = exact && linexpr0->cst.discr==AP_COEFF_SCALAR;
  res = exact;
  k = 0;
  ap_linexpr0_ForeachLinterm(linexpr0,i,dim,coeff){
    exact = itv_set_ap_coeff(intern,
			     expr->linterm[k].itv,
			     coeff);
    if (!itv_is_zero(expr->linterm[k].itv)){
      eq = exact && coeff->discr==AP_COEFF_SCALAR;
      res = res && exact;
      expr->linterm[k].equality = eq;
      expr->linterm[k].dim = dim;
      k++;
    }
  }
  itv_linexpr_reinit(expr,k);
  return res;
}

bool ITVFUN(itv_lincons_set_ap_lincons0)(itv_internal_t* intern,
					 itv_lincons_t* cons,
					 ap_lincons0_t* lincons0)
{
  bool exact1 = itv_linexpr_set_ap_linexpr0(intern,
					    &cons->linexpr,
					    lincons0->linexpr0);
  cons->constyp = lincons0->constyp;
  if (lincons0->scalar){
    bool exact2 = num_set_ap_scalar(cons->num,lincons0->scalar);
    return exact1 && exact2;
  }
  else {
    num_set_int(cons->num,0);
    return exact1;
  }
}

bool ITVFUN(itv_lincons_array_set_ap_lincons0_array)(itv_internal_t* intern,
						     itv_lincons_array_t* tcons,
						     ap_lincons0_array_t* tlincons0)
{
  size_t i;
  bool exact = true;

  itv_lincons_array_reinit(tcons,tlincons0->size);
  for (i=0; i<tlincons0->size; i++){
    exact = itv_lincons_set_ap_lincons0(intern,&tcons->p[i],&tlincons0->p[i]) && exact;
  }
  return exact;
}

void ITVFUN(ap_linexpr0_set_itv_linexpr)(itv_internal_t* intern,
					 ap_linexpr0_t** plinexpr0,
					 itv_linexpr_t* linexpr)
{
  ap_linexpr0_t* linexpr0;
  size_t i,k;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;

  linexpr0 = *plinexpr0;
  if (linexpr0==NULL){
    linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,linexpr->size);
  }
  else {
    ap_linexpr0_realloc(linexpr0,linexpr->size);
  }
  ap_coeff_set_itv(intern,&linexpr0->cst,linexpr->cst);
  k = 0;
  itv_linexpr_ForeachLinterm(linexpr,i,dim,pitv,peq){
    linexpr0->p.linterm[k].dim = dim;
    ap_coeff_set_itv(intern,&linexpr0->p.linterm[k].coeff,pitv);
    k++;
    (void)peq;
  }
  *plinexpr0 = linexpr0;
  return;
}

void ITVFUN(ap_lincons0_set_itv_lincons)(itv_internal_t* intern,
					 ap_lincons0_t* plincons0,
					 itv_lincons_t* lincons)
{
  ap_linexpr0_set_itv_linexpr(intern,&plincons0->linexpr0,&lincons->linexpr);
  plincons0->constyp = lincons->constyp;
  if (num_sgn(lincons->num)){
    if (plincons0->scalar==NULL){
      plincons0->scalar = ap_scalar_alloc();
    }
    ap_scalar_set_num(plincons0->scalar,lincons->num);
  }
  else {
    if (plincons0->scalar!=NULL){
      ap_scalar_free(plincons0->scalar);
      plincons0->scalar = NULL;
    }
  }
}


/* ********************************************************************** */
/* III. Arithmetic */
/* ********************************************************************** */

void ITVFUN(itv_linexpr_neg)(itv_linexpr_t* expr)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;

  itv_neg(expr->cst,expr->cst);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    itv_neg(pitv,pitv);
    (void)peq;
  }
  return;
}
void ITVFUN(itv_linexpr_scale)(itv_internal_t* intern,
			       itv_linexpr_t* expr, itv_t coeff)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;

  if (itv_is_zero(coeff)){
    itv_set(expr->cst,coeff);
    itv_linexpr_reinit(expr,0);
    return;
  }
  itv_mul(intern,expr->cst,expr->cst,coeff);
  if (itv_is_top(expr->cst)){
    itv_linexpr_reinit(expr,0);
    return;
  }
  else {
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
      itv_mul(intern,pitv,pitv,coeff);
      *peq = *peq /* speed-up */ && itv_is_point(intern,pitv);
    }
  }
  return;
}
void ITVFUN(itv_linexpr_div)(itv_internal_t* intern,
			     itv_linexpr_t* expr, itv_t coeff)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  itv_ptr pitv;

  itv_div(intern,expr->cst,expr->cst,coeff);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq){
    itv_div(intern,pitv,pitv,coeff);
    *peq = *peq /* speed-up */ && itv_is_point(intern,pitv);
  }
  return;
}

void ITVFUN(itv_linexpr_add)(itv_internal_t* intern,
			     itv_linexpr_t* res, 
			     itv_linexpr_t* exprA,
			     itv_linexpr_t* exprB)
{
  size_t i,j,k;
  itv_linexpr_t expr;
  bool endA,endB;

  if (res==exprA || res==exprB){
    itv_linexpr_init(&expr,exprA->size+exprB->size);
  }
  else {
    expr = *res;
    itv_linexpr_reinit(&expr,exprA->size+exprB->size);
  }
  i = j = k = 0;
  endA = endB = false;
  itv_add(expr.cst,exprA->cst,exprB->cst);
  expr.equality = exprA->equality && exprB->equality && itv_is_point(intern,expr.cst);
  if (itv_is_top(expr.cst))
    goto _itv_linexpr_add_return;
  while (true){
    endA = endA || (i==exprA->size) || exprA->linterm[i].dim == AP_DIM_MAX;
    endB = endB || (j==exprB->size) || exprB->linterm[j].dim == AP_DIM_MAX;
    if (endA && endB)
      break;
    if (endA || (!endB && exprB->linterm[j].dim < exprA->linterm[i].dim)){
      itv_set(expr.linterm[k].itv, exprB->linterm[j].itv);
      expr.linterm[k].equality = exprB->linterm[j].equality;
      expr.linterm[k].dim = exprB->linterm[j].dim;
      k++; j++;
    }
    else if (endB || (!endA && exprA->linterm[i].dim < exprB->linterm[j].dim)){
      itv_set(expr.linterm[k].itv, exprA->linterm[i].itv);
      expr.linterm[k].equality = exprA->linterm[i].equality;
      expr.linterm[k].dim = exprA->linterm[i].dim;
      k++; i++;
    }
    else {
      itv_add(expr.linterm[k].itv, exprA->linterm[i].itv,exprB->linterm[j].itv);
      expr.linterm[k].equality =
	exprA->linterm[i].equality && exprB->linterm[j].equality && /* speed-up */
	itv_is_point(intern,expr.linterm[k].itv);
      expr.linterm[k].dim = exprA->linterm[i].dim;
      if (!itv_is_zero(expr.linterm[k].itv)){
	k++;
      }
      i++; j++;
    }
  }
 _itv_linexpr_add_return:
  itv_linexpr_reinit(&expr,k);
  if (res==exprA || res==exprB){
    itv_linexpr_clear(res);
  }
  *res = expr;
  return;
}
void ITVFUN(itv_linexpr_sub)(itv_internal_t* intern,
			     itv_linexpr_t* res,
			     itv_linexpr_t* exprA,
			     itv_linexpr_t* exprB)
{
  if (exprA==exprB){
    itv_linexpr_t expr;
    itv_linexpr_init_set(&expr,exprB);
    itv_linexpr_neg(&expr);
    itv_linexpr_add(intern,res,exprA,&expr);
    itv_linexpr_clear(&expr);
  }
  else {
    itv_linexpr_neg(exprB);
    itv_linexpr_add(intern,res,exprA,exprB);
    if (exprB!=res){
      itv_linexpr_neg(exprB);
    }
  }
}

/* ********************************************************************** */
/* IV. Tests */
/* ********************************************************************** */

bool ITVFUN(itv_linexpr_is_scalar)(itv_linexpr_t* expr)
{
  bool res = expr->equality;
  if (res){
    size_t i;
    for (i=0; i<expr->size; i++){
      res = expr->linterm[i].equality;
      if (!res) break;
    }
  }
  return res;
}
bool ITVFUN(itv_linexpr_is_quasilinear)(itv_linexpr_t* expr)
{
  bool res;
  if (expr->size==0)
    res = true;
  else {
    size_t i;
    for (i=0; i<expr->size; i++){
      res = expr->linterm[i].equality;
      if (!res) break;
    }
  }
  return res;
}
bool ITVFUN(itv_lincons_array_is_scalar)(itv_lincons_array_t* array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itv_lincons_is_scalar(&array->p[i]);
    if (!res) break;
  }
  return res;
}
bool ITVFUN(itv_lincons_array_is_quasilinear)(itv_lincons_array_t* array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = itv_lincons_is_quasilinear(&array->p[i]);
    if (!res) break;
  }
  return res;
}

bool ITVFUN(itv_linexpr_is_integer)(itv_linexpr_t* expr, size_t intdim)
{
  size_t i;
  bool res;

  res = true;
  for (i=0; i<expr->size; i++){
    if (expr->linterm[i].dim>=intdim){
      res = false;
      break;
    }
  }
  return res;
}
bool ITVFUN(itv_lincons_is_integer)(itv_lincons_t* cons, size_t intdim)
{
  return ITVFUN(itv_linexpr_is_integer)(&cons->linexpr,intdim);
}
/* Evaluate a constraint, composed of a constant (interval) expression */
tbool_t ITVFUN(itv_eval_cstlincons)(itv_internal_t* intern,
				    itv_lincons_t* lincons)
{
  tbool_t res;
  itv_ptr cst = lincons->linexpr.cst;
  bool equality = lincons->linexpr.equality;

  assert (lincons->linexpr.size==0);
  if (itv_is_bottom(intern,cst)){
    return tbool_false;
  }
  switch (lincons->constyp){
  case AP_CONS_EQ:
    if (equality){
      int sgn = bound_sgn(cst->sup);
      res = (sgn==0 ? tbool_true : tbool_false);
    }
    else {
      if (bound_sgn(cst->sup)<0 ||
	  bound_sgn(cst->inf)<0)
	res = tbool_false;
      else
	res = tbool_top;
    }
    break;
  case AP_CONS_DISEQ:
    res =
      (bound_sgn(cst->inf)<0 ||
       bound_sgn(cst->sup)<0) ?
      tbool_true :
      tbool_top;
    break;
  case AP_CONS_SUPEQ:
    if (bound_sgn(cst->inf)<=0)
      res = tbool_true;
    else if (bound_sgn(cst->sup)<0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case AP_CONS_SUP:
    if (bound_sgn(cst->inf)<0)
      res = tbool_true;
    else if (bound_sgn(cst->sup)<=0)
      res = tbool_false;
    else
      res = tbool_top;
    break;
  case AP_CONS_EQMOD:
    if (equality){
      if (bound_sgn(cst->sup)==0){
	res = tbool_true;
      }
      else if (num_cmp_int(lincons->num,0)){
	res = (bound_sgn(cst->sup)==0) ? tbool_true : tbool_top;
      }
      else {
#if defined(NUM_NUMRAT)
	numrat_t numrat;
	numrat_init(numrat);
	numrat_div(numrat,bound_numref(cst->sup),lincons->num);
	if (numint_cmp_int(numrat_denref(numrat),1)==0){
	  res = tbool_true;
	}
	else {
	  res = tbool_top;
	}
	numrat_clear(numrat);
#elif defined(NUM_NUMINT)
	numint_t numint;
	numint_init(numint);
	numint_mod(numint,bound_numref(cst->sup),lincons->num);
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

bool ITVFUN(itv_sat_lincons_is_false)(itv_internal_t* intern,
				      itv_lincons_t* lincons)
{
  bool res = false;

  bool equality = lincons->linexpr.equality;
  itv_ptr cst = lincons->linexpr.cst;
  bool inf = bound_infty(cst->inf);
  
  switch (lincons->constyp){
  case AP_CONS_EQ:
  case AP_CONS_EQMOD:
    res = !equality;
    break;
  case AP_CONS_DISEQ:
    res = inf && bound_infty(cst->sup);
    break;
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    res = inf;
    break;
  default:
    break;
  }
  if (!res && 
      lincons->linexpr.size==0 && 
      itv_eval_cstlincons(intern,lincons)==tbool_false){
    res = true;
  }
  return res;
}

static bool 
itv_lincons_is_useless_for_meet(itv_internal_t* intern,
				itv_lincons_t* lincons)
{
  bool res = false;
  itv_ptr cst = lincons->linexpr.cst;
  if (lincons->linexpr.size==0){
    if (itv_eval_cstlincons(intern,lincons)!=tbool_false)
      res = true;
  }
  else {
    if (!lincons->linexpr.equality){
      bool sup = bound_infty(cst->sup);
      switch (lincons->constyp){
      case AP_CONS_EQ:
      case AP_CONS_DISEQ:
      case AP_CONS_EQMOD:
	{
	  bool inf = bound_infty(cst->inf);
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

void ITVFUN(itv_lincons_reduce_integer)(itv_internal_t* intern,
					itv_lincons_t* cons,
					size_t intdim)
{
  itv_linexpr_t* expr;
  size_t i;
  itv_ptr pitv;
  ap_dim_t dim;
  bool* peq;

  switch (cons->constyp){
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    break;
  default:
    return;
  }
  expr = &cons->linexpr;
  /* Tests if only integer variables are involved */
  if (!itv_linexpr_is_integer(expr,intdim))
    return;
  /* Check that there are only scalar coefficients for dimensions */
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
    if (!(*peq))
      return;
  }
#if defined(NUM_NUMRAT)
  {
    /* compute lcm of denominators and gcd of numerators */
    numint_set_int(numrat_denref(intern->quasi_num),1);
    numint_set_int(numrat_numref(intern->quasi_num),0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
      numint_lcm(numrat_denref(intern->quasi_num),
		 numrat_denref(intern->quasi_num),
		 numrat_denref(bound_numref(pitv->sup)));
      numint_gcd(numrat_numref(intern->quasi_num),
		 numrat_numref(intern->quasi_num),
		 numrat_numref(bound_numref(pitv->sup)));
    }
    if (numint_sgn(numrat_numref(intern->quasi_num))==0)
      return;
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
      numint_divexact(numrat_numref(bound_numref(pitv->sup)),
		      numrat_numref(bound_numref(pitv->sup)),
		      numrat_numref(intern->quasi_num));
      numint_mul(numrat_numref(bound_numref(pitv->sup)),
		 numrat_numref(bound_numref(pitv->sup)),
		 numrat_denref(intern->quasi_num));
      numint_divexact(numrat_numref(bound_numref(pitv->sup)),
		      numrat_numref(bound_numref(pitv->sup)),
		      numrat_denref(bound_numref(pitv->sup)));
      numint_set_int(numrat_denref(bound_numref(pitv->sup)),
		     1);
      bound_neg(pitv->inf,pitv->sup);
    }
    numrat_inv(intern->quasi_num,intern->quasi_num);
    itv_mul_num(expr->cst,expr->cst,intern->quasi_num);
  }
#else
#if defined(NUM_NUMFLT)
  {
    /* Assuming that all coefficients are either integer,
       compute the pgcd */
    mpz_set_si(intern->reduce_lincons_gcd,0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
      if (!numflt_integer(pitv->sup)) 
	return;
      mpz_set_numflt(intern->reduce_lincons_mpz,pitv->sup);
      mpz_gcd(intern->reduce_lincons_gcd,
	      intern->reduce_lincons_gcd,
	      intern->reduce_lincons_mpz);
    }
    if (mpz_sgn(intern->reduce_lincons_gcd)==0 ||
	mpz_cmp_si(intern->reduce_lincons_gcd,1)==0)
      return;
    bool exact = numflt_set_mpz(intern->quasi_num,intern->reduce_lincons_gcd);
    if (!exact) return;
  }
#elif defined(NUM_NUMINT)
  {
    num_set_int(intern->quasi_num,0);
    itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
      numint_gcd(intern->quasi_num,
		 intern->quasi_num,
		 bound_numref(pitv->sup));
    }
    if (numint_sgn(intern->quasi_num)==0 ||
	numint_cmp_int(intern->quasi_num,1)==0)
      return;
  }
#else
#error "HERE"
#endif
  /* divide by gcd put in intern->quasi_num */
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv,peq) {
    itv_div_num(pitv,pitv,intern->quasi_num);
  }
  itv_div_num(expr->cst,expr->cst,intern->quasi_num);
#endif 
  /* Constrain bounds */
  if (!bound_infty(expr->cst->sup)){
    if (cons->constyp==AP_CONS_SUP){
      if (num_integer(bound_numref(expr->cst->sup))){
	bound_sub_uint(expr->cst->sup,expr->cst->sup,1);
      }
      else {
	num_floor(bound_numref(expr->cst->sup),
		  bound_numref(expr->cst->sup));
      }
      cons->constyp = AP_CONS_SUPEQ;
    }
    else {
      num_floor(bound_numref(expr->cst->sup),
		bound_numref(expr->cst->sup));
    }
  }
  if (cons->constyp == AP_CONS_EQ){
    if (!bound_infty(expr->cst->inf)){
      if (expr->equality){
	bound_neg(expr->cst->inf,
		  expr->cst->sup);
      }
      else {
	num_floor(bound_numref(expr->cst->inf),
		  bound_numref(expr->cst->inf));
      }
    }
    if (itv_is_bottom(intern,expr->cst)){
      itv_lincons_set_bool(cons,false);
    }
  }
  else {
    if (!bound_infty(expr->cst->sup)){
      bound_neg(expr->cst->inf,expr->cst->sup);
      expr->equality = true;
    }
  }
}

tbool_t ITVFUN(itv_lincons_array_reduce)(itv_internal_t* intern,
					 itv_lincons_array_t* array, bool meet)
{
  tbool_t res;
  size_t i,size;

  res = tbool_top;
  i = 0;
  size = array->size;
  while (i<size){
    if (array->p[i].linexpr.size==0){
      tbool_t sat = itv_eval_cstlincons(intern,&array->p[i]);
      if (sat==tbool_true){
      itv_lincons_array_reduce_remove:
	size --;
	itv_lincons_swap(&array->p[i],&array->p[size]);
	continue;
      }
      else if (sat==tbool_false){
      itv_lincons_array_reduce_false:
	itv_lincons_array_reinit(array,1);
	itv_lincons_set_bool(&array->p[0],false);
	return tbool_false;
      }
    }
    if (meet && itv_lincons_is_useless_for_meet(intern,&array->p[i]))
      goto itv_lincons_array_reduce_remove;
    else if (!meet && itv_sat_lincons_is_false(intern,&array->p[i]))
      goto itv_lincons_array_reduce_false;
    else {
      i++;
    }
  }
  itv_lincons_array_reinit(array,size);
  if (size==0) 
    res = tbool_true;
  else if (size==1 && array->p[0].linexpr.size==0)
    res = itv_eval_cstlincons(intern,&array->p[0]);
  return res;
}

tbool_t ITVFUN(itv_lincons_array_reduce_integer)(itv_internal_t* intern,
						 itv_lincons_array_t* array,
						 size_t intdim)
{
  size_t i;
  for (i=0; i<array->size; i++){
    itv_lincons_reduce_integer(intern,&array->p[i],intdim);
  }
  return itv_lincons_array_reduce(intern,array,true);
}
