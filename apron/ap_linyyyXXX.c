/* ********************************************************************** */
/* ap_linexprXXX.c: */
/* ********************************************************************** */

#include "ap_linyyyXXX.h"
#include "num_internal.h"
#include "numMPQ.h"

#define _AP_yyy_MARK_

/* ********************************************************************** */
/* I. ap_linyyyXXX_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Constructor and Destructor */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
void ap_linexprXXX_init(ap_linexprXXX_t expr, size_t size)
{
  expr->linterm = NULL;
  expr->effsize = 0;
  expr->maxsize = 0;
  eitvXXX_init(expr->cst);
  ap_linexprXXX_resize_strict(expr,size);
}
void ap_linexprXXX_init_set(ap_linexprXXX_t res, ap_linexprXXX_t expr)
{
  size_t i,size;

  eitvXXX_init_set(res->cst,expr->cst);
  size = expr->effsize;
  res->linterm = ap_malloc(size*sizeof(ap_lintermXXX_t));
  for (i=0;i<size;i++){
    ap_lintermXXX_init_set(res->linterm[i],expr->linterm[i]);
  }
  res->effsize = size;
  res->maxsize = size;
}
void ap_linexprXXX_set(ap_linexprXXX_t res, ap_linexprXXX_t expr)
{
  size_t i,size;

  if (res==expr) return;

  eitvXXX_set(res->cst,expr->cst);
  if (res->maxsize < expr->effsize){
    res->linterm = ap_realloc(res->linterm,expr->effsize*sizeof(ap_lintermXXX_t));
    for (i=0;i<res->maxsize;i++){
      ap_lintermXXX_set(res->linterm[i],expr->linterm[i]);
    }
    for (i=res->maxsize; i<expr->effsize;i++){
      ap_lintermXXX_init_set(res->linterm[i],expr->linterm[i]);
    }
    res->effsize = res->maxsize = expr->effsize;
  }
  else {
    for (i=0;i<expr->effsize;i++){
      ap_lintermXXX_set(res->linterm[i],expr->linterm[i]);
    }
    res->effsize = expr->effsize;
  }
}

void ap_linexprXXX_resize_strict(ap_linexprXXX_t expr, size_t size)
{
  size_t i;

  if (size!=expr->maxsize){
    if (size<expr->maxsize){
      for (i=size; i<expr->maxsize; i++){
	ap_lintermXXX_clear(expr->linterm[i]);
      }
      expr->effsize = size;
      expr->maxsize = size;
    }
    else {
      expr->linterm = ap_realloc(expr->linterm,size*sizeof(ap_lintermXXX_t));
      for (i=expr->maxsize; i<size; i++){
	ap_lintermXXX_init(expr->linterm[i]);
      }
      expr->maxsize = size;
    }
  }
  return;
}
void ap_linexprXXX_minimize(ap_linexprXXX_t e)
{
  size_t ir,iw;
  ir = iw = 0;
  while (ir<e->effsize){
    ap_lintermXXX_ptr lin = e->linterm[ir];
    if (!eitvXXX_is_zero(lin->eitv)){
      if (iw<ir){
	ap_lintermXXX_set(e->linterm[iw],e->linterm[ir]);
      }
      iw++;
    }
    ir++;
  }
  ap_linexprXXX_resize_strict(e,iw);
}
void ap_linexprXXX_clear(ap_linexprXXX_t expr)
{
  size_t i;
  if (expr->linterm){
    for (i=0;i<expr->maxsize;i++){
      ap_lintermXXX_clear(expr->linterm[i]);
    }
    free(expr->linterm);
    expr->linterm = NULL;
    expr->effsize = 0;
    expr->maxsize = 0;
  }
  eitvXXX_clear(expr->cst);
}
#endif

#if defined(_AP_cons_MARK_)
void ap_linconsXXX_set_bool(ap_linconsXXX_t lincons, bool value)
{
  /* constraint 0=0 if value, 1=0 otherwise */
  ap_linexprXXX_resize_strict(lincons->linexpr,0);
  eitvXXX_set_int(lincons->linexpr->cst,value ? 0 : 1);
  lincons->constyp = AP_CONS_EQ;
}
void ap_linconsXXX_set_dim_num(ap_linconsXXX_t lincons, ap_dim_t dim, int sgn, numXXX_t num)
{
  ap_linconsXXX_set_zero(lincons);
  eitvXXX_ptr eitv = ap_linconsXXX_cstref(lincons);
  eitvXXX_set_num(eitv,num);
  eitv = ap_linconsXXX_eitvref0(lincons,dim,true);
  if (sgn>=0){
    eitvXXX_set_int(eitv,-1);
    lincons->constyp = (sgn>0) ? AP_CONS_SUPEQ : AP_CONS_EQ;
  } else {
    eitvXXX_set_int(eitv,1);
    lincons->constyp = AP_CONS_SUPEQ;
  }
}
#endif

ap_linyyyXXX_ptr ap_linyyyXXX_alloc(size_t size)
{
  ap_linyyyXXX_ptr res = (ap_linyyyXXX_ptr)malloc(sizeof(ap_linyyyXXX_struct));
  ap_linyyyXXX_init(res,size);
  return res;
}
ap_linyyyXXX_ptr ap_linyyyXXX_alloc_set(ap_linyyyXXX_t yyy)
{
  ap_linyyyXXX_ptr res = (ap_linyyyXXX_ptr)malloc(sizeof(ap_linyyyXXX_struct));
  ap_linyyyXXX_init_set(res,yyy);
  return res;
}
void ap_linyyyXXX_free(ap_linyyyXXX_ptr e)
{
  ap_linyyyXXX_clear(e);
  free(e);
}

#if defined(_AP_expr_MARK_)
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
#elif defined(_AP_cons_MARK_)
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
#elif defined(_AP_gen_MARK_)
void ap_lingenXXX_fprint(FILE* stream, ap_lingenXXX_t gen, char** name)
{
  char* str = NULL;
  switch(gen->gentyp){
  case AP_GEN_LINE:
    str = "LINE:    ";
    break;
  case AP_GEN_RAY:
    str = "RAY:     ";
    break;
  case AP_GEN_VERTEX:
    str = "VERTEX:  ";
    break;
  case AP_GEN_LINEMOD:
    str = "LINEMOD: ";
    break;
  case AP_GEN_RAYMOD:
    str = "RAYMOD:  ";
    break;
  default:
    abort();
  }
  fprintf(stream,"%s",str);
  ap_linexprXXX_fprint(stream,gen->linexpr,name);
}
#else
#error ""
#endif

void ap_linyyyXXX_print(ap_linyyyXXX_t a, char** name)
{ ap_linyyyXXX_fprint(stdout,a,name); fflush(stdout); }

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
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
size_t ap_linexprXXX_support(ap_linexprXXX_t expr, ap_dim_t* tdim, size_t size)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  size_t nb;

  nb = 0;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (!eitvXXX_is_zero(eitv)){
      tdim[nb] = dim;
      nb++;
    }
  }
  return nb;
}
size_t ap_linexprXXX_supportinterval(ap_linexprXXX_t expr, ap_dim_t* tdim, size_t size)
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
void ap_linexprXXX_support_mask(ap_linexprXXX_t expr, ap_dim_t* tdim)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (!eitvXXX_is_zero(eitv)){
      tdim[dim] = 1;
    }
  }
}
void ap_linexprXXX_supportinterval_mask(ap_linexprXXX_t expr, ap_dim_t* tdim)
{
  size_t i,dim;
  eitvXXX_ptr eitv;
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    if (!eitvXXX_is_point(eitv)){
      tdim[dim] = 1;
    }
  }
}
#endif

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
static size_t index_of_or_after_dim(ap_dim_t dim, ap_lintermXXX_t* linterm, size_t size)
{
  if (size<=6){
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

  index = index_of_or_after_dim(dim,expr->linterm,expr->effsize);
  if (index<expr->effsize && dim == expr->linterm[index]->dim)
    return expr->linterm[index]->eitv;
  else if (create==false){
    return NULL;
  }
  else {
    if (expr->effsize==expr->maxsize){
      unsigned int addsize = (expr->maxsize+3)/4;
      if (addsize<4) addsize=4;
      ap_linexprXXX_resize_strict(expr, expr->maxsize+addsize);
    }
    /* We insert a new linterm at the end */
    expr->linterm[expr->effsize]->dim=dim;
    /* We put it at the right place if needed */
    if (index < expr->effsize){
      ap_lintermXXX_struct tmp = *(expr->linterm[expr->effsize]);
      memmove(&expr->linterm[index+1], &expr->linterm[index],
	      (expr->effsize-index)*sizeof(ap_lintermXXX_t));
      *(expr->linterm[index]) = tmp;
    }
    expr->effsize++;
    return expr->linterm[index]->eitv;
  }
}
eitvXXX_ptr ap_linexprXXX_eitvref1(bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var, bool create)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (*perror)
    return NULL;
  else
    return ap_linexprXXX_eitvref0(expr,dim,create);
}
void ap_linexprXXX_get_eitv0(eitvXXX_t eitv, ap_linexprXXX_t expr, ap_dim_t dim)
{
  eitvXXX_ptr r = ap_linexprXXX_eitvref0(expr,dim,false);
  if (r==NULL)
    eitvXXX_set_int(eitv,0);
  else
    eitvXXX_set(eitv,r);
}
void ap_linexprXXX_get_eitv1(eitvXXX_t eitv, bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! *perror) ap_linexprXXX_get_eitv0(eitv,expr,dim);
}
void ap_linexprXXX_set_eitv0(ap_linexprXXX_t expr, ap_dim_t dim, eitvXXX_t eitv)
{
  eitvXXX_ptr r = ap_linexprXXX_eitvref0(expr,dim,true);
  eitvXXX_set(r,eitv);
}
void ap_linexprXXX_set_eitv1(ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ap_var_t var, eitvXXX_t eitv)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! *perror) ap_linexprXXX_set_eitv0(expr,dim,eitv);
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
  *perror = false;
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
	eitvXXX_set_generic(intern,intern->XXX.generic_eitv,(eitv_tag_t)tag,va)
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
#else
bool ap_linyyyXXX_set_list0(num_internal_t intern, ap_linyyyXXX_t a, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
				       a->linexpr,perror,NULL,&va,intern);
  va_end(va);
  return res;
}
bool ap_linyyyXXX_set_list1(num_internal_t intern, ap_linyyyXXX_t a, bool* perror, ap_environment_t* env, ...)
{
  bool res;
  va_list va;
  va_start(va,env);
  res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
				       a->linexpr,perror,env,&va,intern);
  va_end(va);
  return res;
}
#endif

/* ====================================================================== */
/* I.5. Change of dimensions and permutations */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
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
  for (i=0; i<res->effsize; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
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
  for (i=0; i<res->effsize; i++){
    ap_dim_t* pdim = &res->linterm[i]->dim;
    ap_dim_t dim = *pdim;
    if (*pdim==AP_DIM_MAX) continue;
    *pdim = perm->p[dim];
  }
  qsort(res->linterm,
	res->effsize,
	sizeof(ap_lintermXXX_t),
	&ap_lintermXXX_cmp);
}
#endif

/* ====================================================================== */
/* I.6 Hashing, comparison */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
/* Induces reduction of the coefficients */
int ap_linexprXXX_hash(ap_linexprXXX_t expr)
{
  eitvXXX_ptr eitv;
  size_t size,i,dec;
  int res,res1;

  size = expr->effsize;
  res = size << 8;
  dec = 0;
  for (i=0; i<size; i += (size+6)/7){
    eitv = expr->linterm[i]->eitv;
    res1 = eitvXXX_hash(eitv);
    res += res1<<dec;
    dec++;
  }
  return res;
}
#endif

#if defined(_AP_expr_MARK_)
int ap_linexprXXX_cmp(ap_linexprXXX_t exprA, ap_linexprXXX_t exprB)
{
  size_t i,j;
  bool endA,endB;
  int res;

  res = 0;
  i = j = 0;
  endA = endB = false;
  while (res==0){
    endA = endA || (i==exprA->effsize);
    endB = endB || (j==exprB->effsize);
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
#else
int ap_linyyyXXX_cmp(ap_linyyyXXX_t a1, ap_linyyyXXX_t a2)
{
  int res;
  res =
    a1->yyytyp > a2->yyytyp ? 1 :
    (a1->yyytyp==a2->yyytyp ? 0 : -1);
  if (!res){
    res = ap_linexprXXX_cmp(a1->linexpr,a2->linexpr);
#if defined(_AP_cons_MARK_)
    if (!res && a1->constyp==AP_CONS_EQMOD){
      res = mpq_cmp(a1->mpq,a2->mpq);
    }
#endif
  }
  return res;
}
#endif

/* ====================================================================== */
/* I.7 Serialization */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
size_t ap_lintermXXX_serialize(void* dst, ap_lintermXXX_t src)
{
  size_t n;
  n = eitvXXX_serialize(dst,src->eitv);
  num_dump_word32((char*)dst+n,src->dim);
  return n+4;
}
size_t ap_lintermXXX_deserialize(ap_lintermXXX_t dst, const void* src)
{
  size_t n;
  n = eitvXXX_deserialize(dst->eitv,src);
  dst->dim = num_undump_word32((const char*)src+n);
  return n+4;
}
extern inline size_t ap_lintermXXX_serialized_size(ap_lintermXXX_t src)
{
  return 4 + eitvXXX_serialized_size(src->eitv);
}
size_t ap_linyyyXXX_serialize(void* dst, ap_linyyyXXX_t src)
{
  size_t i,n;
  num_dump_word32(dst,src->effsize);
  n = 4;
  for (i=0;i<src->effsize;i++){
    n += ap_lintermXXX_serialize((char*)dst+n,src->linterm[i]);
  }
  n += eitvXXX_serialize((char*)dst+n,src->cst);
  return n;
}
size_t ap_linyyyXXX_deserialize(ap_linyyyXXX_t dst, const void* src)
{
  size_t i,n,effsize;
  effsize = num_undump_word32(src);
  n = 4;
  dst->linterm = ap_malloc(effsize*sizeof(ap_lintermXXX_t));
  dst->effsize = effsize;
  dst->maxsize = effsize;
  for (i=0;i<effsize;i++){
    n += ap_lintermXXX_deserialize(dst->linterm[i],(const char*)src+n);
  }
  n += eitvXXX_deserialize(dst->cst,(const char*)src+n);
  return n;
}
size_t ap_linyyyXXX_serialized_size(ap_linyyyXXX_t a)
{
  size_t i,n;
  n = 4;
  for (i=0; i<a->effsize; i++){
    n += ap_lintermXXX_serialized_size(a->linterm[i]);
  }
  n += eitvXXX_serialized_size(a->cst);
  return n;
}
#else
size_t ap_linyyyXXX_serialize(void* dst, ap_linyyyXXX_t src)
{
  size_t n;
  n = ap_linexprXXX_serialize(dst,src->linexpr);
  num_dump_word32((char*)dst+n,(unsigned)(src->yyytyp));
  n += 4;
#if defined (_AP_cons_MARK_)
  n += numMPQ_serialize((char*)dst+n,src->mpq);
#endif
  return n;
}
size_t ap_linyyyXXX_deserialize(ap_linyyyXXX_t dst, const void* src)
{
  size_t n;
  n = ap_linexprXXX_deserialize(dst->linexpr,src);
  unsigned int typ = num_undump_word32((const char*)src+n);
  dst->yyytyp = (ap_yyytyp_t)typ;
  n += 4;
#if defined (_AP_cons_MARK_)
  n += numMPQ_deserialize(dst->mpq,(const char*)src+n);
#endif
  return n;
}
size_t ap_linyyyXXX_serialized_size(ap_linyyyXXX_t a)
{
  size_t n = ap_linexprXXX_serialized_size(a->linexpr);
  n += 4;
 #if defined (_AP_cons_MARK_)
  n += numMPQ_serialized_size(a->mpq);
#endif
  return n;
}
#endif

/* ********************************************************************** */
/* II. ap_linyyyXXX_array_t */
/* ********************************************************************** */
/* ====================================================================== */
/* II.1 Constructor and Destructor */
/* ====================================================================== */

void ap_linyyyXXX_array_init(ap_linyyyXXX_array_t array, size_t size)
{
  size_t i;
  array->size = size;
  array->p = ap_malloc(size*sizeof(ap_linyyyXXX_t));
  for (i=0; i<size; i++) ap_linyyyXXX_init(array->p[i],0);
}
void ap_linyyyXXX_array_init_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array)
{
  size_t i;
  res->size = array->size;
  res->p = ap_malloc(array->size*sizeof(ap_linyyyXXX_t));
  for (i=0; i<res->size; i++) ap_linyyyXXX_init_set(res->p[i],array->p[i]);
}
void ap_linyyyXXX_array_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<res->size; i++) ap_linyyyXXX_set(res->p[i],array->p[i]);
}
void ap_linyyyXXX_array_resize(ap_linyyyXXX_array_t array, size_t size)
{
  size_t i;
  if (size == array->size) return;
  if (size < array->size){
    for (i=size; i<array->size; i++){
      ap_linyyyXXX_clear(array->p[i]);
    }
    array->p = ap_realloc(array->p,size*sizeof(ap_linyyyXXX_t));
  }
  else { /* size > array->size */
    array->p = ap_realloc(array->p,size*sizeof(ap_linyyyXXX_t));
    for (i=array->size; i<size; i++){
      ap_linyyyXXX_init(array->p[i],0);
    }
  }
  array->size = size;
  return;
}
void ap_linyyyXXX_array_minimize(ap_linyyyXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_linyyyXXX_minimize(array->p[i]);
}
void ap_linyyyXXX_array_clear(ap_linyyyXXX_array_t array)
{
  size_t i;
  for (i=0; i<array->size; i++) ap_linyyyXXX_clear(array->p[i]);
  free(array->p);
  array->size = 0;
  array->p = NULL;
}

ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc(size_t size)
{
  ap_linyyyXXX_array_ptr res = (ap_linyyyXXX_array_ptr)malloc(sizeof(ap_linyyyXXX_array_struct));
  ap_linyyyXXX_array_init(res,size);
  return res;
}
ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc_set(ap_linyyyXXX_array_t a)
{
  ap_linyyyXXX_array_ptr res = (ap_linyyyXXX_array_ptr)malloc(sizeof(ap_linyyyXXX_array_struct));
  ap_linyyyXXX_array_init_set(res,a);
  return res;
}
void ap_linyyyXXX_array_free(ap_linyyyXXX_array_t a)
{
  ap_linyyyXXX_array_clear(a);
  free(a);
}

void ap_linyyyXXX_array_fprint(FILE* stream, ap_linyyyXXX_array_t array, char** name)
{
  size_t i;
  fprintf(stream,"array of size %d\n",(int)array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_fprint(stream,array->p[i],name);
    fprintf(stream,"\n");
  }
}
void ap_linyyyXXX_array_print(ap_linyyyXXX_array_t array, char** name)
{ ap_linyyyXXX_array_fprint(stdout,array,name); }


/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

bool ap_linyyyXXX_array_is_linear(ap_linyyyXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_linyyyXXX_is_linear(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool ap_linyyyXXX_array_is_quasilinear(ap_linyyyXXX_array_t array)
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = ap_linyyyXXX_is_quasilinear(array->p[i]);
    if (!res) break;
  }
  return res;
}
ap_linexpr_type_t ap_linyyyXXX_array_type(ap_linyyyXXX_array_t array)
{
  size_t i;
  ap_linexpr_type_t type;

  type = AP_LINEXPR_LINEAR;
  for (i=0; i<array->size; i++){
    ap_linexpr_type_t t = ap_linyyyXXX_type(array->p[i]);
    if (t<type){
      type = t;
      if (type==AP_LINEXPR_INTLINEAR)
	break;
    }
  }
  return type;
}

ap_dim_t ap_linyyyXXX_array_max_dim(ap_linyyyXXX_array_t array)
{
  size_t i;
  ap_dim_t res;
  res = 0;
  for (i=0;i<array->size;i++){
    ap_dim_t nres = ap_linyyyXXX_max_dim(array->p[i]);
    if (nres>res) res = nres;
  }
  return res;
}

size_t ap_linyyyXXX_array_support_generic(
    void (*support_mask)(ap_linyyyXXX_t a, ap_dim_t* tdim),
    ap_linyyyXXX_array_t array,
    ap_dim_t* tdim, size_t size
)
{
  size_t i;
  ap_dimsupport_mask_clear(tdim,size);
  for (i=0;i<array->size;i++){
    support_mask(array->p[i],tdim);
  }
  return ap_dimsupport_std_of_mask(tdim,size);
}
size_t ap_linyyyXXX_array_support(ap_linyyyXXX_array_t array,
				  ap_dim_t* tdim, size_t size)
{
  return ap_linyyyXXX_array_support_generic(ap_linyyyXXX_support_mask,array,tdim,size);
}
size_t ap_linyyyXXX_array_supportinterval(ap_linyyyXXX_array_t array,
					  ap_dim_t* tdim, size_t size)
{
  return ap_linyyyXXX_array_support_generic(ap_linyyyXXX_supportinterval_mask,array,tdim,size);
}


/* ====================================================================== */
/* II.4 Conversions */
/* ====================================================================== */

/* See ap_lin_conv.c.tmpl */

/* ====================================================================== */
/* II.5 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyyXXX_array_add_dimensions(ap_linyyyXXX_array_t res,
				       ap_linyyyXXX_array_t array,
				       ap_dimchange_t* dimchange)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_add_dimensions(res->p[i],array->p[i],dimchange);
  }
}

void ap_linyyyXXX_array_permute_dimensions(ap_linyyyXXX_array_t res,
					   ap_linyyyXXX_array_t array,
					   ap_dimperm_t* dimperm)
{
  size_t i;
  ap_linyyyXXX_array_resize(res,array->size);
  for (i=0; i<array->size; i++){
    ap_linyyyXXX_permute_dimensions(res->p[i],array->p[i],dimperm);
  }
}
void ap_linyyyXXX_array_extend_environment(ap_linyyyXXX_array_t res,
					   bool* perror,
					   ap_environment_t* nenv,
					   ap_linyyyXXX_array_t array,
					   ap_environment_t* env)
{
  if (env==nenv){
    if (res!=array) ap_linyyyXXX_array_set(res,array);
    *perror = false;
  }
  else {
    size_t i;
    ap_dimchange_t* dimchange = ap_environment_dimchange(env,nenv);
    ap_linyyyXXX_array_resize(res,array->size);
    *perror = (dimchange==NULL);
    if (!(*perror)){
      for (i=0; i<array->size; i++){
	ap_linyyyXXX_add_dimensions(res->p[i],array->p[i],dimchange);
	ap_dimchange_free(dimchange);
      }
    }
  }
}

/* ====================================================================== */
/* II.6 Hashing, comparison */
/* ====================================================================== */

int ap_linyyyXXX_array_hash(ap_linyyyXXX_array_t a)
{
  unsigned int i;
  int res = a->size*3;
  for (i=0; i<a->size; i+=(a->size+4)/5){
    res = res*7;
    res += ap_linyyyXXX_hash(a->p[i]);
  }
  return res;
}
bool ap_linyyyXXX_array_equal(ap_linyyyXXX_array_t a1,ap_linyyyXXX_array_t a2)
{
  if (a1->size!=a2->size)
    return false;
  else {
    unsigned int i;
    for (i=0; i<a1->size; i++){
      if (!ap_linyyyXXX_equal(a1->p[i],a2->p[i]))
	return false;
    }
    return true;
  }
}
int ap_linyyyXXX_array_cmp(ap_linyyyXXX_array_t a1, ap_linyyyXXX_array_t a2)
{
  if (a1->size<a2->size)
    return -1;
  else if (a1->size>a2->size)
    return 1;
  else {
    unsigned int i;
    for (i=0; i<a1->size; i++){
      int res = ap_linyyyXXX_cmp(a1->p[i],a2->p[i]);
      if (res)
	return res;
    }
    return 0;
  }
}

/* ====================================================================== */
/* II.7 Serialization */
/* ====================================================================== */

size_t ap_linyyyXXX_array_serialize(void* dst, ap_linyyyXXX_array_t src)
{
  size_t i,n;
  num_dump_word32(dst,src->size);
  n = 4;
  for (i=0;i<src->size;i++){
    n += ap_linyyyXXX_serialize((char*)dst+n,src->p[i]);
  }
  return n;
}
size_t ap_linyyyXXX_array_deserialize(ap_linyyyXXX_array_t dst, const void* src)
{
  size_t i,n;
  dst->size = num_undump_word32(src);
  dst->p = ap_malloc(dst->size*sizeof(ap_linyyyXXX_t));
  n = 4;
  for (i=0;i<dst->size;i++){
    n += ap_linyyyXXX_deserialize(dst->p[i],(const char*)src+n);
  }
  return n;
}
size_t ap_linyyyXXX_array_serialized_size(ap_linyyyXXX_array_t a)
{
  size_t i,n;
  n = 4;
  for (i=0;i<a->size;i++){
    n += ap_linyyyXXX_serialized_size(a->p[i]);
  }
  return n;
}
