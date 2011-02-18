/* ********************************************************************** */
/* ap_linexprXXX.c: */
/* ********************************************************************** */

#include "eitvXXX.h"
#include "eitvD.h"
#include "eitvMPQ.h"
#include "eitvMPFR.h"
#include "ap_linexprconsgenXXX_conv.h"
#include "ap_linexprconsgenD_conv.h"
#include "ap_linexprconsgenMPQ_conv.h"
#include "ap_linexprconsgenMPFR_conv.h"
#include "ap_texpr0.h"

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

extern inline void ap_lintermXXX_init(ap_lintermXXX_t term)
{ eitvXXX_init(term->eitv); term->dim = AP_DIM_MAX; }
extern inline void ap_lintermXXX_init_set(ap_lintermXXX_t res, ap_lintermXXX_t term)
{ eitvXXX_init_set(res->eitv,term->eitv); res->dim = term->dim; }
extern inline void ap_lintermXXX_set(ap_lintermXXX_t res, ap_lintermXXX_t term)
{ eitvXXX_set(res->eitv,term->eitv); res->dim = term->dim; }
extern inline void ap_lintermXXX_clear(ap_lintermXXX_t term)
{ eitvXXX_clear(term->eitv); }
extern inline void ap_lintermXXX_swap(ap_lintermXXX_t a, ap_lintermXXX_t b)
{ if (a!=b){ ap_lintermXXX_struct t=*a; *a=*b; *b=t; } }

void ap_linexprXXX_init(ap_linexprXXX_t expr, size_t size)
{
  expr->linterm = NULL;
  expr->size = 0;
  eitvXXX_init(expr->cst);
  ap_linexprXXX_resize(expr,size);
}
void ap_linexprXXX_init_set(ap_linexprXXX_t res, ap_linexprXXX_t expr)
{
  size_t i,size;

  eitvXXX_init_set(res->cst,expr->cst);
  size = expr->size;
  res->linterm = malloc(size*sizeof(ap_lintermXXX_t));
  for (i=0;i<size;i++){
    ap_lintermXXX_init_set(res->linterm[i],expr->linterm[i]);
  }
  res->size = size;
}
void ap_linexprXXX_set(ap_linexprXXX_t res, ap_linexprXXX_t expr)
{
  size_t i,esize,size;

  if (res==expr) return;

  eitvXXX_set(res->cst,expr->cst);
  for  (i=expr->size; i<res->size; i++){
    ap_lintermXXX_clear(res->linterm[i]);
  }
  res->linterm = realloc(res->linterm,expr->size*sizeof(ap_lintermXXX_t));
  size = res->size < expr->size ? res->size : expr->size;
  for (i=0;i<size;i++){
    ap_lintermXXX_set(res->linterm[i],expr->linterm[i]);
  }
  for (i=size; i<expr->size;i++){
    ap_lintermXXX_init_set(res->linterm[i],expr->linterm[i]);
  }
  res->size = expr->size;
}

void ap_linexprXXX_resize(ap_linexprXXX_t expr, size_t size)
{
  size_t i;

  for  (i=size; i<expr->size; i++){
    ap_lintermXXX_clear(expr->linterm[i]);
  }
  expr->linterm = realloc(expr->linterm,size*sizeof(ap_lintermXXX_t));
  for (i=expr->size;i<size;i++){
    ap_lintermXXX_init(expr->linterm[i]);
  }
  expr->size = size;
  return;
}
void ap_linexprXXX_minimize(ap_linexprXXX_t e)
{
  size_t i,j,nsize;

  nsize = 0;
  for (i=0; i<e->size; i++){
    ap_lintermXXX_ptr lin = e->linterm[i];
    if (lin->dim == AP_DIM_MAX)
      break;
    if (!eitvXXX_is_zero(lin->eitv))
      nsize++;
    else
      lin->dim = AP_DIM_MAX;
  }
  if (nsize!=e->size){
    ap_lintermXXX_t* linterm = malloc(nsize*sizeof(ap_lintermXXX_t));
    j = 0;
    for (i=0; i<e->size; i++){
      ap_lintermXXX_ptr lin = e->linterm[i];
      if (lin->dim != AP_DIM_MAX){
	*(linterm[j]) = *lin;
	j++;
      }
      else
	ap_lintermXXX_clear(lin);
    }
    free(e->linterm);
    e->linterm = linterm;
    e->size = nsize;
  }
}
void ap_linexprXXX_clear(ap_linexprXXX_t expr)
{
  size_t i;
  if (expr->linterm){
    for (i=0;i<expr->size;i++){
      ap_lintermXXX_clear(expr->linterm[i]);
    }
    free(expr->linterm);
    expr->linterm = NULL;
    expr->size = 0;
  }
  eitvXXX_clear(expr->cst);
}

ap_linexprXXX_ptr ap_linexprXXX_alloc(size_t size)
{
  ap_linexprXXX_ptr res = (ap_linexprXXX_ptr)malloc(sizeof(ap_linexprXXX_struct));
  ap_linexprXXX_init(res,size);
  return res;
}
ap_linexprXXX_ptr ap_linexprXXX_alloc_set(ap_linexprXXX_t expr)
{
  ap_linexprXXX_ptr res = (ap_linexprXXX_ptr)malloc(sizeof(ap_linexprXXX_struct));
  ap_linexprXXX_init_set(res,expr);
  return res;
}
void ap_linexprXXX_free(ap_linexprXXX_ptr e)
{
  ap_linexprXXX_clear(e);
  free(e);
}

void ap_linexprXXX_fprint(FILE* stream, ap_linexprXXX_t expr, char** name)
{
  ap_linexprXXX_t e;
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr pitv;
  eitvXXX_fprint(stream,expr->cst);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv) {
    fprintf(stream, " + ");
    eitvXXX_fprint(stream,pitv);
    if (name) fprintf(stream,"%s",name[dim]);
    else fprintf(stream,"x%lu",(unsigned long)dim);
  }
  fflush(stream);
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool ap_linexprXXX_is_integer(ap_linexprXXX_t expr, size_t intdim)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  bool res;

  res = true;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (dim>=intdim && !eitvXXX_is_zero(eitv)){
      res = false;
      break;
    }
  }
  return res;
}
bool ap_linexprXXX_is_real(ap_linexprXXX_t expr, size_t intdim)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  bool res;

  res = true;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (dim<intdim){
      if (!eitvXXX_is_zero(eitv)){
	res = false;
	break;
      }
    }
    else
      break;
  }
  return res;
}
bool ap_linexprXXX_is_linear(ap_linexprXXX_t expr)
{
  return eitvXXX_is_point(expr->cst) && ap_linexprXXX_is_quasilinear(expr);
}
bool ap_linexprXXX_is_quasilinear(ap_linexprXXX_t expr)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  bool res;

  res = true;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    res = eitvXXX_is_point(eitv);
    if (!res) break;
  }
  return res;
}
ap_linexpr_type_t ap_linexprXXX_type(ap_linexprXXX_t a)
{
  if (ap_linexprXXX_is_quasilinear(a)){
    if (eitvXXX_is_point(a->cst))
      return AP_LINEXPR_LINEAR;
    else
      return AP_LINEXPR_QUASILINEAR;
  }
  else
    return AP_LINEXPR_INTLINEAR;
}

/* ********************************************************************** */
/* III. Access */
/* ********************************************************************** */

static size_t index_of_or_after_dim(ap_dim_t dim, ap_lintermXXX_t* linterm, size_t size)
{
  if (size<=10){
    size_t i;
    for (i=0; i<size; i++){
      if (dim<=linterm[i]->dim)
	return i;
    }
    return size;
  }
  else {
    size_t mid = size/2;
    if (dim==linterm[mid]->dim)
      return mid;
    else if (dim<linterm[mid]->dim)
      return index_of_or_after_dim(dim, linterm, mid);
    else
      return mid + 1 + index_of_or_after_dim(dim, linterm+mid+1, size-mid-1);
  }
}

eitvXXX_ptr ap_linexprXXX_eitvref0(ap_linexprXXX_t expr, ap_dim_t dim, bool create)
{
  size_t index;

  index = index_of_or_after_dim(dim,expr->linterm,expr->size);
  if (index<expr->size && dim == expr->linterm[index]->dim)
    return expr->linterm[index]->eitv;
  else if (create==false){
    return NULL;
  }
  else {
    if (index<expr->size && expr->linterm[index]->dim==AP_DIM_MAX){
      /* We have a free linterm at the right place */
      expr->linterm[index]->dim=dim;
    }
    else {
      if (expr->size==0 || expr->linterm[expr->size-1]->dim!=AP_DIM_MAX){
	/* We have to insert a new linterm at the end */
	ap_linexprXXX_resize(expr, expr->size+1);
      }
      /* We insert a linterm with AP_DIM_MAX at the right place */
      if (index<expr->size-1){
	ap_lintermXXX_struct tmp = *(expr->linterm[expr->size-1]);
	memmove(&expr->linterm[index+1], &expr->linterm[index],
		(expr->size-index-1)*sizeof(ap_lintermXXX_t));
	*(expr->linterm[index]) = tmp;
      }
      expr->linterm[index]->dim = dim;
    }
    return expr->linterm[index]->eitv;
  }
}

bool ap_linexprXXX_set_list_generic(
    eitvXXX_ptr (*get_eitvXXX_of_dimvar)(
	ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va
    ),
    ap_linexprXXX_t expr, bool* perror,
    ap_environment_t* env, va_list* va, num_internal_t intern
)
{
  ap_coeff_tag_t tag;
  bool cst;
  eitvXXX_ptr a=NULL;
  bool res;

  res = true;
  while (true){
    tag = va_arg(*va,ap_coeff_tag_t);
    if (tag==AP_END)
      break;
    cst = (tag>=AP_CST);
    if (cst)
      tag = AP_COEFF + (tag - AP_CST);
    if (tag==AP_COEFF){
      ap_coeff_ptr b = va_arg(*va,ap_coeff_ptr);
      res = eitvXXX_set_ap_coeff(intern->XXX.generic_eitv,b,intern) && res;
    }
    else {
      res =
	eitvXXX_set_val(intern,intern->XXX.generic_eitv,(eitv_tag_t)tag,&va)
	&& res;
    }
    a = get_eitvXXX_of_dimvar(expr,env,cst,va);
    if (a==NULL){
      *perror = true;
      break;
    }
    eitvXXX_set(a,intern->XXX.generic_eitv);
  }

  return res && !(*perror);
}

eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_dim(
    ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va)
{
  eitvXXX_ptr ptr;
  if (cst){
     ptr = expr->cst;
  } else {
    ap_dim_t dim = va_arg(*va,ap_dim_t);
    ptr = ap_linexprXXX_eitvref0(expr,dim,true);
  }
  return ptr;
}
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_var(
    ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va)
{
  eitvXXX_ptr ptr;
  if (cst){
    ptr = expr->cst;
  } else {
    ap_var_t var = va_arg(*va,ap_var_t);
    ap_dim_t dim = ap_environment_dim_of_var(env,var);
    if (dim==AP_DIM_MAX)
      ptr = NULL;
    else
      ptr = ap_linexprXXX_eitvref0(expr,dim,true);
  }
  return ptr;
}
bool ap_linexprXXX_set_list0(num_internal_t intern, ap_linexprXXX_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
				       expr,perror,NULL,&va,intern);
  va_end(va);
  return res;
}
bool ap_linexprXXX_set_list1(num_internal_t intern, ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ...)
{
  bool res;
  va_list va;
  va_start(va,env);
  res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
				       expr,perror,env,&va,intern);
  va_end(va);
  return res;
}

/* ********************************************************************** */
/* IV. Arithmetic */
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
  eitvXXX_ptr pitv;

  eitvXXX_is_point(coeff);
  if (eitvXXX_is_zero(coeff)){
    eitvXXX_set(expr->cst,coeff);
    ap_linexprXXX_resize(expr,0);
    return true;
  }
  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_mul(res->cst,res->cst,coeff, intern);
  if (eitvXXX_is_top(res->cst)){
    ap_linexprXXX_resize(res,0);
    return exact;
  }
  else {
    ap_linexprXXX_ForeachLinterm0(res,i,dim,pitv){
      eitvXXX_mul(pitv,pitv,coeff, intern);
    }
  }
  return exact;
}
bool ap_linexprXXX_div(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, num_internal_t intern)
{
  bool exact = NUMXXX_DIVEXACT;
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr pitv;

  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_div(res->cst,res->cst,coeff, intern);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,pitv){
    eitvXXX_div(pitv,pitv,coeff, intern);
  }
  return exact;
}

bool ap_linexprXXX_add(ap_linexprXXX_t res,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, num_internal_t intern)
{
  bool exact = true;
  size_t i,j,k;
  ap_linexprXXX_t expr;
  bool endA,endB;

  if (res==exprA || res==exprB){
    ap_linexprXXX_init(expr,exprA->size+exprB->size);
  }
  else {
    *expr = *res;
    ap_linexprXXX_resize(expr,exprA->size+exprB->size);
  }
  i = j = k = 0;
  endA = endB = false;
  exact = NUMXXX_EXACT || eitvXXX_is_zero(exprA->cst) || eitvXXX_is_zero(exprB->cst);
  eitvXXX_add(expr->cst,exprA->cst,exprB->cst);
  if (eitvXXX_is_top(expr->cst))
    goto _ap_linexprXXX_add_return;
  while (true){
    endA = endA || (i==exprA->size) || exprA->linterm[i]->dim == AP_DIM_MAX;
    endB = endB || (j==exprB->size) || exprB->linterm[j]->dim == AP_DIM_MAX;
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
  ap_linexprXXX_resize(expr,k);
  if (res==exprA || res==exprB){
    ap_linexprXXX_clear(res);
  }
  *res = *expr;
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
    exact = ap_linexprXXX_add(res,exprA,expr, intern);
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
/* Va. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

size_t ap_linexprXXX_supportinterval(ap_linexprXXX_t expr, ap_dim_t* tdim)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  size_t nb;

  nb = 0;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (!eitvXXX_is_point(eitv)){
      tdim[nb] = dim;
      nb++;
    }
  }
  return nb;
}

/* Evaluate an interval linear expression */
bool eitvXXX_eval_ap_linexpr0(eitvXXX_t res, ap_linexpr0_t expr, eitvXXX_t* env, num_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t coeff;
  bool exact;

  SWITCHZ(expr->discr)
    {
      eitvZZZ_ptr eitv;

      exact = eitvXXX_set_eitvZZZ(res,expr->linexpr.ZZZ->cst,intern);
      ap_linexprZZZ_ForeachLinterm0(expr->linexpr.ZZZ,i,dim,eitv){
	exact =  eitvXXX_set_eitvZZZ(intern->XXX.eval_eitv,eitv,intern) && exact;
	eitvXXX_mul(intern->XXX.eval_eitv, intern->XXX.eval_eitv, env[dim], intern);
	eitvXXX_add(res,res,intern->XXX.eval_eitv);
	if (eitvXXX_is_top(res))
	  break;
      }
    }
  ENDSWITCH
#if NUMXXX_EXACT
  return exact;
#else
  return false;
#endif
}



bool ap_linexprXXX_eval(eitvXXX_t res, ap_linexprXXX_t expr, eitvXXX_t* env, num_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr eitv;

  assert(env);

  eitvXXX_set(res, expr->cst);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    eitvXXX_mul(intern->XXX.eval_eitv,env[dim],eitv, intern);
    eitvXXX_add(res,res,intern->XXX.eval_eitv);
    if (eitvXXX_is_top(res))
      break;
  }
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}


/* ====================================================================== */
/* V.2 Quasilinearisation */
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
eitvXXX_quasilinearize_choose_middle(numXXX_t middle, /* the result */
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
	numXXX_div_2(middle,
		     middle);
      }
    }
  }
}

bool ap_linexprXXX_quasilinearize(ap_linexprXXX_t linexpr, eitvXXX_t* env,
				  bool for_meet_inequality, num_internal_t intern)
{
  ap_dim_t size,i,dim;
  eitvXXX_ptr eitv;
  bool top,zero;

#ifdef LOGDEBUG
  printf("ap_linexprXXX_quasilinearize:\n");
  ap_linexprXXX_print(linexpr,0); printf("\n");
#endif
  top = false; zero = false;
  ap_linexprXXX_ForeachLinterm0(linexpr,i,dim,eitv){
    if (eitvXXX_is_point(env[dim])){
      /* If a variable has a constant value, simplification */
      eitvXXX_mul_num(eitv,eitv,boundXXX_numref(env[dim]->itv->sup));
      eitvXXX_add(linexpr->cst,linexpr->cst,eitv);
      eitvXXX_set_int(eitv,0);
      zero = true;
    }
    else if (!eitvXXX_is_point(eitv)){
      /* Compute the middle of the interval */
      eitvXXX_quasilinearize_choose_middle(intern->XXX.quasi_num,
					   eitv,env[dim],for_meet_inequality);
      /* Residue (interval-middle) */
      eitvXXX_sub_num(intern->XXX.eval_eitv2,eitv,intern->XXX.quasi_num);
      /* Multiplication of residue by variable range */
      eitvXXX_mul(intern->XXX.eval_eitv,
		  intern->XXX.eval_eitv2,
		  env[dim], intern);
      /* Addition to the constant coefficient */
      eitvXXX_add(linexpr->cst,linexpr->cst,intern->XXX.eval_eitv);
      if (eitvXXX_is_top(linexpr->cst)){
	top = true;
	break;
      }
    }
  }
  if (top)
    ap_linexprXXX_resize(linexpr,0);
  else if (zero)
    ap_linexprXXX_minimize(linexpr);

#ifdef LOGDEBUG
  ap_linexprXXX_print(linexpr,NULL); printf("\n");
#endif
#if NUMXXX_EXACT
  return true;
#else
  return false;
#endif
}

bool ap_linexprXXX_array_quasilinearize(ap_linexprXXX_array_t array, eitvXXX_t* env, num_internal_t intern)
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
/* Vb. Evaluation and interval linearisation of tree expressions */
/* ********************************************************************** */

/* ====================================================================== */
/* Vb.1 Conversion to interval linear expression */
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
  bool exact = true;
  eitvXXX_t i1;
  ap_linexprXXX_t l1;

  *perror = n->type!=AP_RTYPE_REAL;
  if (*perror) return false;
  switch (n->op) {
  case AP_TEXPR_NEG:
    exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprA,intern);
    ap_linexprXXX_neg(lres,lres);
    break;
  case AP_TEXPR_CAST:
    break;
  case AP_TEXPR_MOD:
  case AP_TEXPR_SQRT:
    *perror = true;
    break;

  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
    exact = NUMXXX_EXACT;
    ap_linexprXXX_init(l1,0);

    /* intlinearize arguments */
    exact  =ap_linexprXXX_set_texpr0(l1,perror,n->exprA,intern);
    if (*perror) break;
    exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprB,intern) && exact;
    if (*perror) break;
    /* add/sub linear form & interval */
    if (n->op==AP_TEXPR_ADD)
      exact = ap_linexprXXX_add(lres,l1,lres,intern) && exact;
    else
      exact = ap_linexprXXX_sub(lres,l1,lres,intern) && exact;
    ap_linexprXXX_clear(l1);
    break;
  case AP_TEXPR_MUL:
  case AP_TEXPR_DIV:
    eitvXXX_init(i1);
    if (ap_texpr0_is_interval_cst(n->exprB)){
      exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprA,intern);
      if (*perror) break;
      exact = eitvXXX_eval_ap_texpr0(i1,n->exprB,NULL,intern) && exact;
    }
    else if (n->op == AP_TEXPR_MUL && ap_texpr0_is_interval_cst(n->exprA)){
      exact = ap_linexprXXX_set_texpr0(lres,perror,n->exprB,intern);
      if (*perror) break;
      exact = eitvXXX_eval_ap_texpr0(i1,n->exprA,NULL,intern) && exact;
    }
    else {
      *perror = true;
      break;
    }
    if (n->op==AP_TEXPR_DIV){
      eitvXXX_t i2;
      eitvXXX_init(i2);
      eitvXXX_set_int(i2,1);
      eitvXXX_div(i1,i2,i1,intern);
      eitvXXX_clear(i2);
    }
    exact = ap_linexprXXX_scale(lres,lres,i1,intern) && exact;
    eitvXXX_clear(i1);
    break;

  default:
    assert(0);
  }
  return exact;
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

/* ====================================================================== */
/* Vb.2 Evaluation of tree expressions */
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
    return exact; /* no rounding */
  case AP_TEXPR_CAST:
    eitvXXX_set(res, arg1);
    break;
  case AP_TEXPR_SQRT:
    eitvXXX_sqrt(res, arg1, intern);
    exact = false;
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
    exact = false;
    return exact; /* no rounding */
  default:
    assert(0);
  }
  eitvXXX_round(res,res,n->type,n->dir,intern);
  exact = exact && (n->type==AP_RTYPE_REAL);
  return exact;
}


bool eitvXXX_eval_ap_texpr0(eitvXXX_t res, struct ap_texpr0_t* expr, eitvXXX_t* env, num_internal_t intern)
{
  bool exact;
  switch(expr->discr){
  case AP_TEXPR_CST:
    exact = eitvXXX_set_ap_coeff(res,expr->val.cst,intern);
    break;
  case AP_TEXPR_DIM:
    eitvXXX_set(res,env[expr->val.dim]);
    exact = true;
    break;
  case AP_TEXPR_NODE:
    if (expr->val.node->exprB) {
      /* binary */
      eitvXXX_t x;
      eitvXXX_init(x);
      exact = eitvXXX_eval_ap_texpr0(x,expr->val.node->exprA,env,intern);
      exact = eitvXXX_eval_ap_texpr0(res,expr->val.node->exprB,env,intern) && exact;
      if (eitvXXX_is_bottom(x,intern) || eitvXXX_is_bottom(res,intern)){
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
      if (!eitvXXX_is_bottom(res,intern)){
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
/* Vb.3 Linearisation of tree expressions */
/* ====================================================================== */

#define LOGDEBUG 0

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
			  eitvXXX_t* env,
			  numXXX_floatconst* f, num_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr peitv;
  eitvXXX_magnitude(intern->XXX.linear_bound,l->cst);
  ap_linexprXXX_ForeachLinterm0(l,i,dim,peitv) {
    eitvXXX_magnitude(intern->XXX.linear_bound2,peitv);
    eitvXXX_magnitude(intern->XXX.linear_bound3,env[dim]);
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
			    eitvXXX_t* env,
			    num_internal_t intern)
{
  eitvXXX_t tmp;
  eitvXXX_init(tmp);
  ap_linexprXXX_eval(tmp,l,env, intern);
  eitvXXX_meet(i,i,tmp, intern);
  if (eitvXXX_is_bottom(i, intern) || eitvXXX_is_bottom(l->cst, intern)) {
    eitvXXX_set_bottom(i);
    eitvXXX_set_bottom(l->cst);
    if (l->size>0) ap_linexprXXX_resize(l,0);
  }
  else if (l->size==0){
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
  if (la->size==0) return 0;
  if (lb->size==0) return 1;
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
				      eitvXXX_t* env, size_t intdim,
				      num_internal_t intern);

static int debug_indent = 0;

static ap_texpr_rtype_t
ap_linexprXXX_intlinearize_texpr0_node(ap_linexprXXX_t lres /* out */, eitvXXX_t ires /* out */,
				       ap_texpr0_node_t* n,
				       eitvXXX_t* env, size_t intdim,
				       num_internal_t intern)
{
  eitvXXX_t i1,i2;
  ap_linexprXXX_t l1;
  ap_texpr_rtype_t t1,t2;

  switch (n->op) {
  case AP_TEXPR_NEG:
    /* negate linear form & interval, no rounding */
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    ap_linexprXXX_neg(lres,lres);
    eitvXXX_neg(ires,ires);
    return t1;

  case AP_TEXPR_CAST:
    /* round linear form & interval */
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    ap_linexprXXX_texpr0_round(lres,ires,t1,n->type,n->dir, intern);
    ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
    break;

  case AP_TEXPR_SQRT:
    /* intlinearize argument, lres is not used */
    ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    /* interval square root */
    eitvXXX_sqrt(ires,ires, intern);
    eitvXXX_round(ires,ires,n->type,n->dir,intern);
    ap_linexprXXX_resize(lres,0);
    eitvXXX_set(lres->cst,ires);
    break;

  case AP_TEXPR_ADD:
  case AP_TEXPR_SUB:
    eitvXXX_init(i1);
    ap_linexprXXX_init(l1,0);

    /* intlinearize arguments */
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(l1,i1,n->exprA,env,intdim, intern);
    t2 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprB,env,intdim, intern);
    if (eitvXXX_is_bottom(i1, intern) || eitvXXX_is_bottom(ires, intern)){
      eitvXXX_set_bottom(ires);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
    }
    else {
      /* add/sub linear form & interval */
      if (n->op==AP_TEXPR_ADD) {
	ap_linexprXXX_add(lres,l1,lres, intern);
	eitvXXX_add(ires,i1,ires);
      }
      else {
	ap_linexprXXX_sub(lres,l1,lres, intern);
	eitvXXX_sub(ires,i1,ires);
      }
      /* round */
      ap_linexprXXX_texpr0_round(lres,ires,
				 (t1==AP_RTYPE_INT && t2==AP_RTYPE_INT) ?
				 AP_RTYPE_INT : AP_RTYPE_REAL,
				 n->type,n->dir,intern);
      /* reduce */
      ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
    }
    eitvXXX_clear(i1);
    ap_linexprXXX_clear(l1);
    break;

  case AP_TEXPR_DIV:
    eitvXXX_init(i1);
    ap_linexprXXX_init(l1,0);
    /* intlinearize arguments, l1 is not used */
    ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    ap_linexprXXX_intlinearize_texpr0_rec(l1,i1,n->exprB,env,intdim, intern);
    if (eitvXXX_is_bottom(i1, intern) || eitvXXX_is_bottom(ires, intern)){
      eitvXXX_set_bottom(ires);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
    }
    else {
      /* divide linear form & interval */
      ap_linexprXXX_div(lres,lres,i1, intern);
      eitvXXX_div(ires,ires,i1, intern);
      /* round */
      ap_linexprXXX_texpr0_round(lres,ires,AP_RTYPE_REAL,n->type,n->dir, intern);
      /* reduce */
      ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
    }
    eitvXXX_clear(i1);
    ap_linexprXXX_clear(l1);
    break;

  case AP_TEXPR_MUL:
    eitvXXX_init(i1);
    ap_linexprXXX_init(l1,0);
    /* intlinearize arguments */
    t1 = ap_linexprXXX_intlinearize_texpr0_rec(l1,i1,n->exprA,env,intdim, intern);
    t2 = ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprB,env,intdim, intern);
    if (eitvXXX_is_bottom(i1, intern) || eitvXXX_is_bottom(ires, intern)){
      eitvXXX_set_bottom(ires);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
      ap_linexprXXX_clear(l1);
    }
    else {
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
      /* round */
      ap_linexprXXX_texpr0_round(lres,ires,
				 (t1==AP_RTYPE_INT && t2==AP_RTYPE_INT) ?
				 AP_RTYPE_INT : AP_RTYPE_REAL,
				 n->type,n->dir,
				 intern);
      /* reduce */
      ap_linexprXXX_texpr0_reduce(lres,ires,env, intern);
    }
    eitvXXX_clear(i1);
    break;

  case AP_TEXPR_MOD:
    eitvXXX_init(i1);
    ap_linexprXXX_init(l1,0);
    /* intlinearize arguments, lres & l1 are not used */
    ap_linexprXXX_intlinearize_texpr0_rec(lres,ires,n->exprA,env,intdim, intern);
    ap_linexprXXX_intlinearize_texpr0_rec(l1,i1,n->exprB,env,intdim, intern);
    if (eitvXXX_is_bottom(i1, intern) || eitvXXX_is_bottom(ires, intern)){
      eitvXXX_set_bottom(ires);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
    }
    else {
      /* interval modulo, no rounding */
      eitvXXX_mod(ires,ires,i1,n->type==AP_RTYPE_INT, intern);
      ap_linexprXXX_resize(lres,0);
      eitvXXX_set(lres->cst,ires);
    }
    eitvXXX_clear(i1);
    ap_linexprXXX_clear(l1);
    break;

  default:
    assert(0);
  }

  return n->type;
}

static ap_texpr_rtype_t
ap_linexprXXX_intlinearize_texpr0_rec(ap_linexprXXX_t lres /* out */,
				      eitvXXX_t ires /* out */,
				      ap_texpr0_t* expr,
				      eitvXXX_t* env, size_t intdim,
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
    eitvXXX_set(ires,env[expr->val.dim]);
    ap_linexprXXX_resize(lres,1);
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
				       eitvXXX_t* env, size_t intdim,
				       num_internal_t intern)
{
  eitvXXX_t i;
  eitvXXX_init(i);
  ap_linexprXXX_intlinearize_texpr0_rec(res,i,expr,env,intdim,intern);
  if (!eitvXXX_is_bottom(i, intern) && !eitvXXX_is_bottom(res->cst, intern)) {
    if (res->size==0){
      eitvXXX_meet(res->cst,res->cst,i, intern);
    }
  }
  else {
    abort();
  }
  eitvXXX_clear(i);
}

/* ********************************************************************** */
/* VI. Change of dimensions and permutations */
/* ********************************************************************** */

/* This function adds dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexprXXX_add_dimensions(ap_linexprXXX_t res,
				  ap_linexprXXX_t expr,
				  ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;
  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  dimsup = dimchange->dim.intd+dimchange->dim.reald;
  k=0;
  for (i=0; i<res->size; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
    if (*pdim==AP_DIM_MAX)
      break;
    while (k<dimsup && *pdim>=dimchange->p[k]){
      k++;
    }
    *pdim += k;
  }
}
void ap_linexprXXX_extend_environment(ap_linexprXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linexprXXX_t expr,
				      ap_environment_t* env)
{
  bool error;
  if (env==nenv){
    if (res!=expr) ap_linexprXXX_set(res,expr);
    error = false;
  } else {
    ap_dimchange_t* dimchange = ap_environment_dimchange(env,nenv);
    error = (dimchange==NULL);
    if (!error){
      ap_linexprXXX_add_dimensions(res,expr,dimchange);
      ap_dimchange_free(dimchange);
    }
  }
  if (perror) *perror=error;
}

/* This function applies the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
static int ap_lintermXXX_cmp(const void* a, const void* b)
{
  const ap_lintermXXX_struct* aa = (const ap_lintermXXX_struct*)a;
  const ap_lintermXXX_struct* bb = (const ap_lintermXXX_struct*)b;
  return
    (aa->dim > bb->dim) ? 1 :
    ( (aa->dim < bb->dim) ? -1 : 0 );
}

void ap_linexprXXX_permute_dimensions(ap_linexprXXX_t res,
				      ap_linexprXXX_t expr,
				      ap_dimperm_t* perm)
{
  size_t i,k,dimsup;
  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  for (i=0; i<res->size; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
    ap_dim_t dim = *pdim;
    if (*pdim==AP_DIM_MAX) continue;
    *pdim = perm->p[dim];
  }
  qsort(res->linterm,
	res->size,
	sizeof(ap_lintermXXX_t),
	&ap_lintermXXX_cmp);
}

/* ********************************************************************** */
/* VII. Hashing, comparison */
/* ********************************************************************** */

/* Induces reduction of the coefficients */

int ap_linexprXXX_hash(ap_linexprXXX_t expr)
{
  eitvXXX_ptr eitv;
  size_t size,i,dec;
  int res,res1;

  size = ap_linexprXXX_size(expr);
  res = size << 8;
  dec = 0;
  for (i=0; i<size; i += (expr->size+7)/8){
    eitv = expr->linterm[i]->eitv;
    res1 = eitvXXX_hash(eitv);
    res += res1<<dec;
    dec++;
  }
  return res;
}

int ap_linexprXXX_compare(ap_linexprXXX_t exprA, ap_linexprXXX_t exprB)
{
  size_t i,j;
  bool endA,endB;
  int res;

  res = 0;
  i = j = 0;
  endA = endB = false;
  while (res==0){
    endA = endA || (i==exprA->size) || exprA->linterm[i]->dim == AP_DIM_MAX;
    endB = endB || (j==exprB->size) || exprB->linterm[j]->dim == AP_DIM_MAX;
    if (endA && endB)
      break;
    if (endA || (!endB && exprB->linterm[j]->dim < exprA->linterm[i]->dim)){
      res = -eitvXXX_cmp_zero(exprB->linterm[j]->eitv);
      j++;
    }
    else if (endB || (!endA && exprA->linterm[i]->dim < exprB->linterm[j]->dim)){
      res = -eitvXXX_cmp_zero(exprA->linterm[i]->eitv);
      i++;
    }
    else {
      res = eitvXXX_cmp(exprA->linterm[i]->eitv,exprB->linterm[j]->eitv);
      i++; j++;
    }
  }
  if (res==0){
    res = eitvXXX_cmp(exprA->cst,exprB->cst);
  }
  return res;
}
