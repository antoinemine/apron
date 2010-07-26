/* ********************************************************************** */
/* itv_linexpr.c: */
/* ********************************************************************** */

#include "itv_linexpr.h"
#include <stddef.h>
#include <stdarg.h>

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void itv_linexpr_init(itv_linexpr_t expr, size_t size)
{
  expr->linterm = NULL;
  expr->size = 0;
  eitv_init(expr->cst);
  itv_linexpr_resize(expr,size);
}
void itv_linexpr_init_set(itv_linexpr_t res, itv_linexpr_t expr)
{
  size_t i,size;

  eitv_init_set(res->cst,expr->cst);
  size = expr->size;
  res->linterm = malloc(size*sizeof(itv_linterm_t));
  for (i=0;i<size;i++){
    itv_linterm_init_set(res->linterm[i],expr->linterm[i]);
  }
  res->size = size;
}
void itv_linexpr_set(itv_linexpr_t res, itv_linexpr_t expr)
{
  size_t i,esize,size;

  if (res==expr) return;

  eitv_set(res->cst,expr->cst);
  for  (i=expr->size; i<res->size; i++){
    itv_linterm_clear(res->linterm[i]);
  }
  res->linterm = realloc(res->linterm,expr->size*sizeof(itv_linterm_t));
  size = res->size < expr->size ? res->size : expr->size;
  for (i=0;i<size;i++){
    itv_linterm_set(res->linterm[i],expr->linterm[i]);
  }
  for (i=size; i<expr->size;i++){
    itv_linterm_init_set(res->linterm[i],expr->linterm[i]);
  }
  res->size = expr->size;
}

void itv_linexpr_resize(itv_linexpr_t expr, size_t size)
{
  size_t i;

  for  (i=size; i<expr->size; i++){
    itv_linterm_clear(expr->linterm[i]);
  }
  expr->linterm = realloc(expr->linterm,size*sizeof(itv_linterm_t));
  for (i=expr->size;i<size;i++){
    itv_linterm_init(expr->linterm[i]);
  }
  expr->size = size;
  return;
}
void itv_linexpr_minimize(itv_linexpr_t e)
{
  size_t i,j,nsize;

  nsize = 0;
  for (i=0; i<e->size; i++){
    itv_linterm_ptr lin = e->linterm[i];
    if (lin->dim == AP_DIM_MAX)
      break;
    if (!eitv_is_zero(lin->eitv))
      nsize++;
    else
      lin->dim = AP_DIM_MAX;
  }
  if (nsize!=e->size){
    itv_linterm_t* linterm = malloc(nsize*sizeof(itv_linterm_t));
    j = 0;
    for (i=0; i<e->size; i++){
      itv_linterm_ptr lin = e->linterm[i];
      if (lin->dim != AP_DIM_MAX){
	*(linterm[j]) = *lin;
	j++;
      }
      else
	itv_linterm_clear(lin);
    }
    free(e->linterm);
    e->linterm = linterm;
    e->size = nsize;
  }
}
void itv_linexpr_clear(itv_linexpr_t expr)
{
  size_t i;
  if (expr->linterm){
    for (i=0;i<expr->size;i++){
      itv_linterm_clear(expr->linterm[i]);
    }
    free(expr->linterm);
    expr->linterm = NULL;
    expr->size = 0;
  }
  eitv_clear(expr->cst);
}
itv_linexpr_ptr itv_linexpr_alloc(size_t size)
{
  itv_linexpr_ptr res = (itv_linexpr_ptr)malloc(sizeof(itv_linexpr_struct));
  itv_linexpr_init(res,size);
  return res;
}
itv_linexpr_ptr itv_linexpr_alloc_set(itv_linexpr_t expr)
{
  itv_linexpr_ptr res = (itv_linexpr_ptr)malloc(sizeof(itv_linexpr_struct));
  itv_linexpr_init_set(res,expr);
  return res;
}
void itv_linexpr_free(itv_linexpr_ptr e)
{
  itv_linexpr_clear(e);
  free(e);
}

void itv_linexpr_fprint(FILE* stream, itv_linexpr_t expr, char** name)
{
  itv_linexpr_t e;
  size_t i;
  ap_dim_t dim;
  eitv_ptr pitv;
  eitv_fprint(stream,expr->cst);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv) {
    fprintf(stream, " + ");
    eitv_fprint(stream,pitv);
    if (name) fprintf(stream,"%s",name[dim]);
    else fprintf(stream,"x%lu",(unsigned long)dim);
  }
  fflush(stream);
}

void itv_linexpr_array_init(itv_linexpr_t* tab, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    itv_linexpr_init(tab[i],0);
  }
}
void itv_linexpr_array_clear(itv_linexpr_t* tab, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    itv_linexpr_clear(tab[i]);
  }
}
itv_linexpr_t* itv_linexpr_array_alloc(size_t size)
{
  itv_linexpr_t* res = (itv_linexpr_t*)malloc(size*sizeof(itv_linexpr_t));
  itv_linexpr_array_init(res,size);
  return res;
}
void itv_linexpr_array_free(itv_linexpr_t* tab, size_t size)
{
  itv_linexpr_array_clear(tab,size);
  free(tab);
}
void itv_linexpr_array_fprint(FILE* stream, itv_linexpr_t* tab, size_t size, char** name)
{
  size_t i;
  for (i=0; i<size; i++){
    itv_linexpr_fprint(stream,tab[i],name);
    fprintf(stream,"\n");
  }
}

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool itv_linexpr_is_integer(itv_linexpr_t expr, size_t intdim)
{
  size_t i,dim;
  eitv_ptr eitv;
  bool res;

  res = true;
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    if (dim>=intdim && !eitv_is_zero(eitv)){
      res = false;
      break;
    }
  }
  return res;
}
bool itv_linexpr_is_real(itv_linexpr_t expr, size_t intdim)
{
  size_t i,dim;
  eitv_ptr eitv;
  bool res;

  res = true;
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    if (dim<intdim){
      if (!eitv_is_zero(eitv)){
	res = false;
	break;
      }
    }
    else
      break;
  }
  return res;
}
bool itv_linexpr_is_quasilinear(itv_linexpr_t expr)
{
  size_t i,dim;
  eitv_ptr eitv;
  bool res;

  res = true;
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    res = eitv_is_point(eitv);
    if (!res) break;
  }
  return res;
}
itvlinexpr_type_t itv_linexpr_type(itv_linexpr_t a)
{
  if (itv_linexpr_is_quasilinear(a)){
    if (eitv_is_point(a->cst))
      return ITV_LINEXPR_LINEAR;
    else
      return ITV_LINEXPR_QUASILINEAR;
  }
  else
    return ITV_LINEXPR_INTLINEAR;
}
bool itv_linexpr_array_is_quasilinear(itv_linexpr_t* texpr, size_t size)
{
  size_t i;
  bool res;

  res = true;
  for (i=0;i<size;i++){
    res = itv_linexpr_is_quasilinear(texpr[i]);
    if (!res) break;
  }
  return res;
}
itvlinexpr_type_t itv_linexpr_array_type(itv_linexpr_t* texpr, size_t size)
{
  size_t i;
  itvlinexpr_type_t type;

  type = ITV_LINEXPR_LINEAR;
  for (i=0; i<size; i++){
    itvlinexpr_type_t t = itv_linexpr_type(texpr[i]);
    if (t<type){
      type = t;
      if (type==ITV_LINEXPR_INTLINEAR)
	break;
    }
  }
  return type;
}
bool itv_linexpr_array_is_linear(itv_linexpr_t* texpr, size_t size)
{
  size_t i;
  bool res;

  res = true;
  for (i=0;i<size;i++){
    res = itv_linexpr_is_linear(texpr[i]);
    if (!res) break;
  }
  return res;
}

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static size_t index_of_or_after_dim(ap_dim_t dim, itv_linterm_t* linterm, size_t size)
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

eitv_ptr itv_linexpr_eitvref(itv_linexpr_t expr, ap_dim_t dim, bool create)
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
	itv_linexpr_resize(expr, expr->size+1);
      }
      /* We insert a linterm with AP_DIM_MAX at the right place */
      if (index<expr->size-1){
	itv_linterm_struct tmp = *(expr->linterm[expr->size-1]);
	memmove(&expr->linterm[index+1], &expr->linterm[index],
		(expr->size-index-1)*sizeof(itv_linterm_t));
	*(expr->linterm[index]) = tmp;
      }
      expr->linterm[index]->dim = dim;
    }
    return expr->linterm[index]->eitv;
  }
}

bool itv_linexpr_set_list_generic(eitv_ptr (*get_eitv_of_dimvar)(void* env,void* expr, va_list* va),
				  void* env,
				  numinternal_t intern,
				  void* expr, va_list* va)
{
  itvcoefftag_t tag;
  eitv_ptr a;
  bool res;

  res = true;
  while (true){
    tag = va_arg(*va,itvcoefftag_t);
    if (tag==ITV_END)
      break;

    switch (tag){
    case ITV_COEFF:
      {
	ap_coeff_ptr b = va_arg(*va,ap_coeff_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	eitv_set_ap_coeff(a,b,intern);
      }
      break;
    case ITV_EITV:
      {
	eitv_ptr b = va_arg(*va,eitv_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	eitv_set(a,b);
      }
      break;
    case ITV_NUM:
      {
	num_ptr b = va_arg(*va,num_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	eitv_set_num(a,b);
      }
      break;
    case ITV_NUM2:
      {
	num_ptr b = va_arg(*va,num_ptr);
	num_ptr c = va_arg(*va,num_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	eitv_set_num2(a,b,c);
      }
      break;
    case ITV_LINT:
      {
	long int b = va_arg(*va,long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_lint(a,b,intern) && res;
      }
      break;
    case ITV_LINT2:
      {
	long int b = va_arg(*va,long int);
	long int c = va_arg(*va,long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_lint2(a,b,c,intern) && res;
      }
      break;
    case ITV_LLINT:
      {
	long long int b = va_arg(*va,long long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_llint(a,b,intern) && res;
      }
      break;
    case ITV_LLINT2:
      {
	long long int b = va_arg(*va,long long int);
	long long int c = va_arg(*va,long long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_llint2(a,b,c,intern) && res;
      }
      break;
    case ITV_MPZ:
      {
	mpz_ptr b = va_arg(*va,mpz_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpz(a,b,intern) && res;
      }
      break;
    case ITV_MPZ2:
      {
	mpz_ptr b = va_arg(*va,mpz_ptr);
	mpz_ptr c = va_arg(*va,mpz_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpz2(a,b,c,intern) && res;
      }
      break;
    case ITV_LFRAC:
      {
	long int i = va_arg(*va,long int);
	long int j = va_arg(*va,long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_lfrac(a,i,j,intern) && res;
      }
      break;
    case ITV_LFRAC2:
      {
	long int i = va_arg(*va,long int);
	long int j = va_arg(*va,long int);
	long int k = va_arg(*va,long int);
	long int l = va_arg(*va,long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_lfrac2(a,i,j,k,l,intern) && res;
      }
      break;
    case ITV_LLFRAC:
      {
	long long int i = va_arg(*va,long long int);
	long long int j = va_arg(*va,long long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_llfrac(a,i,j,intern) && res;
      }
      break;
    case ITV_LLFRAC2:
      {
	long long int i = va_arg(*va,long long int);
	long long int j = va_arg(*va,long long int);
	long long int k = va_arg(*va,long long int);
	long long int l = va_arg(*va,long long int);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_llfrac2(a,i,j,k,l,intern) && res;
      }
      break;
    case ITV_MPQ:
      {
	mpq_ptr b = va_arg(*va,mpq_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpq(a,b,intern) && res;
      }
      break;
    case ITV_MPQ2:
      {
	mpq_ptr b = va_arg(*va,mpq_ptr);
	mpq_ptr c = va_arg(*va,mpq_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpq2(a,b,c,intern) && res;
      }
      break;
    case ITV_DOUBLE:
      {
	double b = va_arg(*va,double);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_double(a,b,intern) && res;
      }
      break;
    case ITV_DOUBLE2:
      {
	double b = va_arg(*va,double);
	double c = va_arg(*va,double);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_double2(a,b,c,intern) && res;
      }
      break;
    case ITV_LDOUBLE:
      {
	long double b = va_arg(*va,long double);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_ldouble(a,b,intern) && res;
      }
      break;
    case ITV_LDOUBLE2:
      {
	long double b = va_arg(*va,long double);
	long double c = va_arg(*va,long double);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_ldouble2(a,b,c,intern) && res;
      }
      break;
    case ITV_MPFR:
      {
	mpfr_ptr b = va_arg(*va,mpfr_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpfr(a,b,intern) && res;
      }
      break;
    case ITV_MPFR2:
      {
	mpfr_ptr b = va_arg(*va,mpfr_ptr);
	mpfr_ptr c = va_arg(*va,mpfr_ptr);
	a = get_eitv_of_dimvar(env,expr,va);
	res = eitv_set_mpfr2(a,b,c,intern) && res;
      }
      break;
   default:
      fprintf(stderr,
	      "itv_linexpr_set_list_generic: probably bad structure for the argument list\n");
      abort();
    }
  }
  return res;
}

eitv_ptr itv_linexpr_set_list_get_eitv_of_dim(void* env, void* expr, va_list* va)
{
  eitv_ptr ptr;
  ap_dim_t dim = va_arg(*va,ap_dim_t);
  if (dim==AP_DIM_MAX)
    ptr = ((itv_linexpr_ptr)expr)->cst;
  else
    ptr = itv_linexpr_eitvref(expr,dim,true);
  return ptr;
}

bool itv_linexpr_set_list(numinternal_t intern, itv_linexpr_t expr, ...)
{
  bool res;
  va_list va;
  va_start(va,expr);
  res = itv_linexpr_set_list_generic(itv_linexpr_set_list_get_eitv_of_dim,
				     NULL,intern,expr,&va);
  va_end(va);
  return res;
}

/* ********************************************************************** */
/* IV. Arithmetic */
/* ********************************************************************** */

void itv_linexpr_neg(itv_linexpr_t res, itv_linexpr_t expr)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  eitv_ptr pitv;

  if (res!=expr){
    itv_linexpr_set(res,expr);
  }
  eitv_neg(res->cst, expr->cst);
  itv_linexpr_ForeachLinterm(res,i,dim,pitv){
    eitv_neg(pitv,pitv);
  }
  return;
}
void itv_linexpr_scale(itv_internal_t* intern,
		       itv_linexpr_t res, itv_linexpr_t expr, eitv_t coeff)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  eitv_ptr pitv;

  eitv_is_point(coeff);
  if (eitv_is_zero(coeff)){
    eitv_set(expr->cst,coeff);
    itv_linexpr_resize(expr,0);
    return;
  }
  if (res!=expr){
    itv_linexpr_set(res,expr);
  }
  eitv_mul(intern,res->cst,res->cst,coeff);
  if (eitv_is_top(res->cst)){
    itv_linexpr_resize(res,0);
    return;
  }
  else {
    itv_linexpr_ForeachLinterm(res,i,dim,pitv){
      eitv_mul(intern,pitv,pitv,coeff);
    }
  }
  return;
}
void itv_linexpr_div(itv_internal_t* intern,
		     itv_linexpr_t res, itv_linexpr_t expr, eitv_t coeff)
{
  size_t i;
  ap_dim_t dim;
  eitv_ptr pitv;

  if (res!=expr){
    itv_linexpr_set(res,expr);
  }
  eitv_div(intern,res->cst,res->cst,coeff);
  itv_linexpr_ForeachLinterm(expr,i,dim,pitv){
    eitv_div(intern,pitv,pitv,coeff);
  }
  return;
}

void itv_linexpr_add(itv_internal_t* intern,
		     itv_linexpr_t res,
		     itv_linexpr_t exprA,
		     itv_linexpr_t exprB)
{
  size_t i,j,k;
  itv_linexpr_t expr;
  bool endA,endB;

  if (res==exprA || res==exprB){
    itv_linexpr_init(expr,exprA->size+exprB->size);
  }
  else {
    *expr = *res;
    itv_linexpr_resize(expr,exprA->size+exprB->size);
  }
  i = j = k = 0;
  endA = endB = false;
  eitv_add(expr->cst,exprA->cst,exprB->cst);
  if (eitv_is_top(expr->cst))
    goto _itv_linexpr_add_return;
  while (true){
    endA = endA || (i==exprA->size) || exprA->linterm[i]->dim == AP_DIM_MAX;
    endB = endB || (j==exprB->size) || exprB->linterm[j]->dim == AP_DIM_MAX;
    if (endA && endB)
      break;
    if (endA || (!endB && exprB->linterm[j]->dim < exprA->linterm[i]->dim)){
      itv_linterm_set(expr->linterm[k], exprB->linterm[j]);
      k++; j++;
    }
    else if (endB || (!endA && exprA->linterm[i]->dim < exprB->linterm[j]->dim)){
      itv_linterm_set(expr->linterm[k], exprA->linterm[i]);
      k++; i++;
    }
    else {
      eitv_add(expr->linterm[k]->eitv, exprA->linterm[i]->eitv,exprB->linterm[j]->eitv);
      expr->linterm[k]->dim = exprA->linterm[i]->dim;
      if (!eitv_is_zero(expr->linterm[k]->eitv)){
	k++;
      }
      i++; j++;
    }
  }
 _itv_linexpr_add_return:
  itv_linexpr_resize(expr,k);
  if (res==exprA || res==exprB){
    itv_linexpr_clear(res);
  }
  *res = *expr;
  return;
}
void itv_linexpr_sub(itv_internal_t* intern,
		     itv_linexpr_t res,
		     itv_linexpr_t exprA,
		     itv_linexpr_t exprB)
{
  if (exprA==exprB){
    itv_linexpr_t expr;
    itv_linexpr_init(expr,0);
    itv_linexpr_neg(expr,exprB);
    itv_linexpr_add(intern,res,exprA,expr);
    itv_linexpr_clear(expr);
  }
  else {
    itv_linexpr_neg(exprB,exprB);
    itv_linexpr_add(intern,res,exprA,exprB);
    if (exprB!=res){
      itv_linexpr_neg(exprB,exprB);
    }
  }
}

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

/* Evaluate an interval linear expression */
bool itv_linexpr_eval(itv_internal_t* intern,
		      itv_t res, itv_linexpr_t expr, itv_t* env)
{
  size_t i;
  ap_dim_t dim;
  eitv_ptr eitv;

  assert(env);

  itv_set(res, expr->cst->itv);
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    itv_mul(intern,
	    intern->eval_itv,env[dim],eitv->itv);
    itv_add(res,res,intern->eval_itv);
    if (itv_is_top(res))
      break;
  }
#if NUM_EXACT
  return true;
#else
  return false;
#endif
}

/* ====================================================================== */
/* V.1 Support */
/* ====================================================================== */

/* Merge buffers indexed by k0 and k1, and return the new buffer in the index
   *pk (normally, k2) */
void itv_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk)
{
  size_t k0 = *pk;
  size_t k1 = (k0+1)%3;
  size_t k2 = (k0+2)%3;
  if (k0==0)
    *pk = k1;
  else if (k1==0)
    *pk = k0;
  else {
    size_t i0,i1,i2;
    bool end0, end1;

    i0 = i1 = i2 = 0;
    end0 = end1 = false;
    while (true){
      end0 = end0 || i0>=tnb[k0];
      end1 = end1 || i1>=tnb[k1];
      if (end0 && end1)
	break;
      else if (end0 || (!end1 && ttdim[k1][i1] < ttdim[k0][i0])){
	ttdim[k2][i2] = ttdim[k1][i1];
	i2++; i1++;
      }
      else if (end1 || (!end0 && ttdim[k1][i1] > ttdim[k0][i0])){
	ttdim[k2][i2] = ttdim[k0][i0];
	i2++; i0++;
      }
      else {
	ttdim[k2][i2] = ttdim[k0][i0];
	i2++; i0++; i1++;
      }
    }
    tnb[k2] = i2;
    *pk = k2;
  }
}

size_t itv_linexpr_supportinterval(itv_linexpr_t expr, ap_dim_t* tdim)
{
  size_t i,dim;
  eitv_ptr eitv;
  size_t nb;

  nb = 0;
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    if (!eitv_is_point(eitv)){
      tdim[nb] = dim;
      nb++;
    }
  }
  return nb;
}
size_t itv_linexpr_array_supportinterval(itv_linexpr_t* texpr, size_t size,
					 ap_dim_t* tdim, size_t maxdim1)
{
  if (size==0){
    return 0;
  }
  else if (size==1){
    return itv_linexpr_supportinterval(texpr[0],tdim);
  }
  else {
    size_t i,k,nb;
    ap_dim_t* buffer;
    ap_dim_t* ttdim[3];
    size_t tnb[3];

    buffer = (ap_dim_t*)malloc(3*maxdim1*sizeof(ap_dim_t));
    for (i=0; i<3; i++){
      ttdim[i] = &buffer[i*maxdim1];
      tnb[i] = 0;
    }
    k = 0;
    for (i=0; i<size; i++){
      size_t k1 = (k+1)%3 ;
      tnb[k1] = itv_linexpr_supportinterval(texpr[i],ttdim[k1]);
      itv_support_merge(ttdim,tnb,&k);
    }
    nb = tnb[k];
    memcpy(tdim,&ttdim[k],nb*sizeof(ap_dim_t));
    free(buffer);
    return nb;
  }
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
itv_quasilinearize_choose_middle(num_t middle, /* the result */
				 itv_t coeff,    /* the coefficient in which
						    middle is to be picked */
				 itv_t var,      /* the variable interval */
				 bool for_meet_inequality /* is it for the
							     linearisation of
							     an inequality ? */
				 )
{
  if (bound_infty(coeff->neginf)){
    if (bound_infty(coeff->sup))
      num_set_int(middle,0);
    else
      num_set(middle,
	      bound_numref(coeff->sup));
  }
  else if (bound_infty(coeff->sup))
    num_neg(middle,
	    bound_numref(coeff->neginf));
  else {
    /* if coeff = [inf,sup] */
    if (for_meet_inequality){
      if (bound_infty(var->neginf))
	num_neg(middle,
		bound_numref(coeff->neginf));
      else if (bound_infty(var->sup))
	num_set(middle,
		bound_numref(coeff->sup));
      else /* Arbitrary choice: we take the middle */
	goto itv_quasilinearize_choose_middle_default;
    }
    else {
      if (bound_infty(var->neginf) ?
	  !bound_infty(var->sup) :
	  bound_infty(var->sup)){
	if (bound_sgn(coeff->neginf)<=0)
	  num_neg(middle,
		  bound_numref(coeff->neginf));
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
		bound_numref(coeff->neginf));
	num_div_2(middle,
		  middle);
      }
    }
  }
}

bool itv_linexpr_quasilinearize(itv_internal_t* intern,
				itv_linexpr_t linexpr, itv_t* env,
				bool for_meet_inequality)
{
  ap_dim_t size,i,dim;
  eitv_ptr eitv;
  bool top,zero;

#ifdef LOGDEBUG
  printf("itv_linexpr_quasilinearize:\n");
  itv_linexpr_print(linexpr,0); printf("\n");
#endif
  top = false; zero = false;
  itv_linexpr_ForeachLinterm(linexpr,i,dim,eitv){
    if (itv_is_point(env[dim])){
      /* If a variable has a constant value, simplification */
      eitv_mul_num(eitv,eitv,bound_numref(env[dim]->sup));
      eitv_add(linexpr->cst,linexpr->cst,eitv);
      eitv_set_int(eitv,0);
      zero = true;
    }
    else if (!eitv_is_point(eitv)){
      itv_ptr itv = eitv->itv;
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
      linexpr->cst->eq = false;
      itv_add(linexpr->cst->itv,linexpr->cst->itv,intern->eval_itv);
      if (eitv_is_top(linexpr->cst)){
	top = true;
	break;
      }
    }
  }
  if (top)
    itv_linexpr_resize(linexpr,0);
  else if (zero)
    itv_linexpr_minimize(linexpr);

#ifdef LOGDEBUG
  itv_linexpr_print(linexpr,NULL); printf("\n");
#endif
#if NUM_EXACT
  return true;
#else
  return false;
#endif
}

bool itv_linexpr_array_quasilinearize(itv_internal_t* intern,
				      itv_linexpr_t* linexpr, size_t size, itv_t* env,
				      bool for_meet_inequality)
{
  size_t i;
  bool res;
  res = true;
  for (i=0; i<size; i++) {
    itv_linexpr_quasilinearize(intern,linexpr[i],env,for_meet_inequality);
  }
#if NUM_EXACT
  return true;
#else
  return false;
#endif
}

/* ====================================================================== */
/* VI. Change of dimensions and permutations */
/* ====================================================================== */

/* This function adds dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void itv_linexpr_add_dimensions(itv_linexpr_t res,
				itv_linexpr_t expr,
				ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;
  if (res!=expr){
    itv_linexpr_set(res,expr);
  }
  dimsup = dimchange->intdim+dimchange->realdim;
  k=0;
  for (i=0; i<res->size; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
    if (*pdim==AP_DIM_MAX)
      break;
    while (k<dimsup && *pdim>=dimchange->dim[k]){
      k++;
    }
    *pdim += k;
  }
}

/* This function applies the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
static int itv_linterm_cmp(const void* a, const void* b)
{
  const itv_linterm_struct* aa = (const itv_linterm_struct*)a;
  const itv_linterm_struct* bb = (const itv_linterm_struct*)b;
  return
    (aa->dim > bb->dim) ? 1 :
    ( (aa->dim < bb->dim) ? -1 : 0 );
}

void itv_linexpr_permute_dimensions(itv_linexpr_t res,
				    itv_linexpr_t expr,
				    ap_dimperm_t* perm)
{
  size_t i,k,dimsup;
  if (res!=expr){
    itv_linexpr_set(res,expr);
  }
  for (i=0; i<res->size; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
    ap_dim_t dim = *pdim;
    if (*pdim==AP_DIM_MAX) continue;
    *pdim = perm->dim[dim];
  }
  qsort(res->linterm,
	res->size,
	sizeof(itv_linterm_t),
	&itv_linterm_cmp);
}

/* ====================================================================== */
/* VII. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int itv_linexpr_hash(itv_linexpr_t expr)
{
  eitv_ptr eitv;
  size_t size,i,dec;
  int res,res1;

  size = itv_linexpr_size(expr);
  res = size << 8;
  dec = 0;
  for (i=0; i<size; i += (expr->size+7)/8){
    eitv = expr->linterm[i]->eitv;
    res1 = eitv_hash(eitv);
    res += res1<<dec;
    dec++;
  }
  return res;
}
int itv_linexpr_compare(itv_linexpr_t exprA, itv_linexpr_t exprB)
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
      res = -eitv_cmp_zero(exprB->linterm[j]->eitv);
      j++;
    }
    else if (endB || (!endA && exprA->linterm[i]->dim < exprB->linterm[j]->dim)){
      res = -eitv_cmp_zero(exprA->linterm[i]->eitv);
      i++;
    }
    else {
      res = eitv_cmp(exprA->linterm[i]->eitv,exprB->linterm[j]->eitv);
      i++; j++;
    }
  }
  if (res==0){
    res = eitv_cmp(exprA->cst,exprB->cst);
  }
  return res;
}
