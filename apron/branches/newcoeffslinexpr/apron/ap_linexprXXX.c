/* ********************************************************************** */
/* ap_linexprXXX.c: */
/* ********************************************************************** */

#include "eitvXXX.h"
#include "eitvD.h"
#include "eitvMPQ.h"
#include "eitvMPFR.h"
#include "itvXXX_conv.h"
#include "itvD_conv.h"
#include "itvMPQ_conv.h"
#include "itvMPFR_conv.h"
#include "ap_linexprconsXXX_conv.h"
#include "ap_linexprconsD_conv.h"
#include "ap_linexprconsMPQ_conv.h"
#include "ap_linexprconsMPFR_conv.h"

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

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

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

bool ap_linexprXXX_set_list_generic(eitvXXX_ptr (*get_eitvXXX_of_dimvar)(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va),
				    num_internal_t intern,
				    ap_linexprXXX_t expr, 
				    bool* perror,
				    ap_environment_t* env,
				    va_list* va)
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
      tag -= AP_CST;
    
    switch (tag){
    case AP_COEFF:
      {
	ap_coeff_ptr b = va_arg(*va,ap_coeff_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_ap_coeff(a,b,intern);
      }
      break;
    case AP_COEFF_EITVD:
      {
	eitvD_ptr b = va_arg(*va,eitvD_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_eitvD(a,b,intern);
      }
      break;
    case AP_COEFF_EITVMPQ:
      {
	eitvD_ptr b = va_arg(*va,eitvD_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_eitvD(a,b,intern);
      }
      break;
    case AP_COEFF_EITVMPFR:
      {
	eitvMPQ_ptr b = va_arg(*va,eitvMPQ_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_eitvMPQ(a,b,intern);
      }
      break;
    case AP_COEFF_NUMD:
      {
	numD_ptr b = va_arg(*va,numD_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numD(a,b,intern);
      }
      break;
    case AP_COEFF_NUMMPQ:
      {
	numMPQ_ptr b = va_arg(*va,numMPQ_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numMPQ(a,b,intern);
      }
      break;
    case AP_COEFF_NUMMPFR:
      {
	numMPFR_ptr b = va_arg(*va,numMPFR_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numMPFR(a,b,intern);
      }
      break;
    case AP_COEFF_NUMD2:
      {
	numD_ptr b = va_arg(*va,numD_ptr);
	numD_ptr c = va_arg(*va,numD_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numD2(a,b,c,intern);
      }
      break;
    case AP_COEFF_NUMMPQ2:
      {
	numMPQ_ptr b = va_arg(*va,numMPQ_ptr);
	numMPQ_ptr c = va_arg(*va,numMPQ_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numMPQ2(a,b,c,intern);
      }
      break;
    case AP_COEFF_NUMMPFR2:
      {
	numMPFR_ptr b = va_arg(*va,numMPFR_ptr);
	numMPFR_ptr c = va_arg(*va,numMPFR_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	eitvXXX_set_numMPFR2(a,b,c,intern);
      }
      break;
    case AP_COEFF_LINT:
      {
	long int b = va_arg(*va,long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_lint(a,b,intern) && res;
      }
      break;
    case AP_COEFF_LINT2:
      {
	long int b = va_arg(*va,long int);
	long int c = va_arg(*va,long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_lint2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_LLINT:
      {
	long long int b = va_arg(*va,long long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_llint(a,b,intern) && res;
      }
      break;
    case AP_COEFF_LLINT2:
      {
	long long int b = va_arg(*va,long long int);
	long long int c = va_arg(*va,long long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_llint2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_MPZ:
      {
	mpz_ptr b = va_arg(*va,mpz_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpz(a,b,intern) && res;
      }
      break;
    case AP_COEFF_MPZ2:
      {
	mpz_ptr b = va_arg(*va,mpz_ptr);
	mpz_ptr c = va_arg(*va,mpz_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpz2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_LFRAC:
      {
	long int i = va_arg(*va,long int);
	long int j = va_arg(*va,long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_lfrac(a,i,j,intern) && res;
      }
      break;
    case AP_COEFF_LFRAC2:
      {
	long int i = va_arg(*va,long int);
	long int j = va_arg(*va,long int);
	long int k = va_arg(*va,long int);
	long int l = va_arg(*va,long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_lfrac2(a,i,j,k,l,intern) && res;
      }
      break;
    case AP_COEFF_LLFRAC:
      {
	long long int i = va_arg(*va,long long int);
	long long int j = va_arg(*va,long long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_llfrac(a,i,j,intern) && res;
      }
      break;
    case AP_COEFF_LLFRAC2:
      {
	long long int i = va_arg(*va,long long int);
	long long int j = va_arg(*va,long long int);
	long long int k = va_arg(*va,long long int);
	long long int l = va_arg(*va,long long int);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_llfrac2(a,i,j,k,l,intern) && res;
      }
      break;
    case AP_COEFF_MPQ:
      {
	mpq_ptr b = va_arg(*va,mpq_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpq(a,b,intern) && res;
      }
      break;
    case AP_COEFF_MPQ2:
      {
	mpq_ptr b = va_arg(*va,mpq_ptr);
	mpq_ptr c = va_arg(*va,mpq_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpq2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_DOUBLE:
      {
	double b = va_arg(*va,double);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_double(a,b,intern) && res;
      }
      break;
    case AP_COEFF_DOUBLE2:
      {
	double b = va_arg(*va,double);
	double c = va_arg(*va,double);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_double2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_LDOUBLE:
      {
	long double b = va_arg(*va,long double);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_ldouble(a,b,intern) && res;
      }
      break;
    case AP_COEFF_LDOUBLE2:
      {
	long double b = va_arg(*va,long double);
	long double c = va_arg(*va,long double);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_ldouble2(a,b,c,intern) && res;
      }
      break;
    case AP_COEFF_MPFR:
      {
	mpfr_ptr b = va_arg(*va,mpfr_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpfr(a,b,intern) && res;
      }
      break;
    case AP_COEFF_MPFR2:
      {
	mpfr_ptr b = va_arg(*va,mpfr_ptr);
	mpfr_ptr c = va_arg(*va,mpfr_ptr);
	a = get_eitvXXX_of_dimvar(expr,env,cst,va);
	if (a==NULL) break;
	res = eitvXXX_set_mpfr2(a,b,c,intern) && res;
      }
      break;
   default:
      fprintf(stderr,
	      "ap_linexprXXX_set_list_generic: probably bad structure for the argument list\n");
      abort();
    }
    if (a==NULL) break;
  }
  *perror = (tag!=AP_END) && (a==NULL);
  return res && !(*perror);
}

eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_dim(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va)
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
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_var(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va)
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
				       intern,expr,perror,NULL,&va);
  va_end(va);
  return res;
}
bool ap_linexprXXX_set_list1(num_internal_t intern, ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ...)
{
  bool res;
  va_list va;
  va_start(va,env);
  res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
				       intern,expr,perror,env,&va);
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
void ap_linexprXXX_scale(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, itv_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  bool* peq;
  eitvXXX_ptr pitv;

  eitvXXX_is_point(coeff);
  if (eitvXXX_is_zero(coeff)){
    eitvXXX_set(expr->cst,coeff);
    ap_linexprXXX_resize(expr,0);
    return;
  }
  if (res!=expr){
    ap_linexprXXX_set(res,expr);
  }
  eitvXXX_mul(res->cst,res->cst,coeff, intern);
  if (eitvXXX_is_top(res->cst)){
    ap_linexprXXX_resize(res,0);
    return;
  }
  else {
    ap_linexprXXX_ForeachLinterm0(res,i,dim,pitv){
      eitvXXX_mul(pitv,pitv,coeff, intern);
    }
  }
  return;
}
void ap_linexprXXX_div(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, itv_internal_t intern)
{
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
  return;
}

void ap_linexprXXX_add(
			ap_linexprXXX_t res,
			ap_linexprXXX_t exprA,
			ap_linexprXXX_t exprB, itv_internal_t intern)
{
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
  return;
}
void ap_linexprXXX_sub(
		     ap_linexprXXX_t res,
		     ap_linexprXXX_t exprA,
		     ap_linexprXXX_t exprB, itv_internal_t intern)
{
  if (exprA==exprB){
    ap_linexprXXX_t expr;
    ap_linexprXXX_init(expr,0);
    ap_linexprXXX_neg(expr,exprB);
    ap_linexprXXX_add(res,exprA,expr, intern);
    ap_linexprXXX_clear(expr);
  }
  else {
    ap_linexprXXX_neg(exprB,exprB);
    ap_linexprXXX_add(res,exprA,exprB, intern);
    if (exprB!=res){
      ap_linexprXXX_neg(exprB,exprB);
    }
  }
}

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

/* Evaluate an interval linear expression */
bool ap_linexprXXX_eval(itvXXX_t res, ap_linexprXXX_t expr, itvXXX_t* env, itv_internal_t intern)
{
  size_t i;
  ap_dim_t dim;
  eitvXXX_ptr eitv;

  assert(env);

  itvXXX_set(res, expr->cst->itv);
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    itvXXX_mul(intern->XXX->eval_itv,env[dim],eitv->itv, intern);
    itvXXX_add(res,res,intern->XXX->eval_itv);
    if (itvXXX_is_top(res))
      break;
  }
#if NUMXXX_EXACT
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
void itvXXX_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk)
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
itvXXX_quasilinearize_choose_middle(numXXX_t middle, /* the result */
				    itvXXX_t coeff,    /* the coefficient in which
							  middle is to be picked */
				    itvXXX_t var,      /* the variable interval */
				    bool for_meet_inequality /* is it for the
								linearisation of
								an inequality ? */
				    )
{
  if (boundXXX_infty(coeff->neginf)){
    if (boundXXX_infty(coeff->sup))
      numXXX_set_int(middle,0);
    else
      numXXX_set(middle,
		 boundXXX_numref(coeff->sup));
  }
  else if (boundXXX_infty(coeff->sup))
    numXXX_neg(middle,
	       boundXXX_numref(coeff->neginf));
  else {
    /* if coeff = [inf,sup] */
    if (for_meet_inequality){
      if (boundXXX_infty(var->neginf))
	numXXX_neg(middle,
		   boundXXX_numref(coeff->neginf));
      else if (boundXXX_infty(var->sup))
	numXXX_set(middle,
		   boundXXX_numref(coeff->sup));
      else /* Arbitrary choice: we take the middle */
	goto itvXXX_quasilinearize_choose_middle_default;
    }
    else {
      if (boundXXX_infty(var->neginf) ?
	  !boundXXX_infty(var->sup) :
	  boundXXX_infty(var->sup)){
	if (boundXXX_sgn(coeff->neginf)<=0)
	  numXXX_neg(middle,
		     boundXXX_numref(coeff->neginf));
	else if (boundXXX_sgn(coeff->sup)<=0)
	  numXXX_set(middle,
		     boundXXX_numref(coeff->sup));
	else /* Arbitrary choice: we take the middle */
	  goto itvXXX_quasilinearize_choose_middle_default;
      }
      else {
      itvXXX_quasilinearize_choose_middle_default:
	numXXX_sub(middle,
		   boundXXX_numref(coeff->sup),
		   boundXXX_numref(coeff->neginf));
	numXXX_div_2(middle,
		     middle);
      }
    }
  }
}

bool ap_linexprXXX_quasilinearize(ap_linexprXXX_t linexpr, itvXXX_t* env,
				   bool for_meet_inequality, itv_internal_t intern)
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
    if (itvXXX_is_point(env[dim])){
      /* If a variable has a constant value, simplification */
      eitvXXX_mul_num(eitv,eitv,boundXXX_numref(env[dim]->sup));
      eitvXXX_add(linexpr->cst,linexpr->cst,eitv);
      eitvXXX_set_int(eitv,0);
      zero = true;
    }
    else if (!eitvXXX_is_point(eitv)){
      itvXXX_ptr itv = eitv->itv;
      /* Compute the middle of the interval */
      itvXXX_quasilinearize_choose_middle(intern->XXX->quasi_num,
					  itv,env[dim],for_meet_inequality);
      /* Residue (interval-middle) */
      itvXXX_sub_num(intern->XXX->eval_itv2,itv,intern->XXX->quasi_num);
      /* Multiplication of residue by variable range */
      itvXXX_mul(
		 intern->XXX->eval_itv,
		 intern->XXX->eval_itv2,
		 env[dim], intern);
      /* Addition to the constant coefficient */
      linexpr->cst->eq = false;
      itvXXX_add(linexpr->cst->itv,linexpr->cst->itv,intern->XXX->eval_itv);
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

bool ap_linexprXXX_array_quasilinearize(ap_linexprXXX_array_t array, itvXXX_t* env, itv_internal_t intern)
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

/* ====================================================================== */
/* VI. Change of dimensions and permutations */
/* ====================================================================== */

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
    *pdim = perm->dim[dim];
  }
  qsort(res->linterm,
	res->size,
	sizeof(ap_lintermXXX_t),
	&ap_lintermXXX_cmp);
}
/* ====================================================================== */
/* VII. Hashing, comparison */
/* ====================================================================== */

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
