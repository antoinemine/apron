/* ************************************************************************* */
/* ap_linexpr0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#include "ap_linexpr0.h"

#include <stdarg.h>

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr0_t* ap_linexpr0_alloc(ap_linexpr_discr_t lin_discr, size_t size)
{
  ap_linexpr0_t* e;
  size_t i;

  e = (ap_linexpr0_t*)malloc(sizeof(ap_linexpr0_t));
  ap_coeff_init(&e->cst,AP_COEFF_SCALAR);
  e->discr = lin_discr;
  e->size = size;
  switch (lin_discr){
  case AP_LINEXPR_DENSE:
    e->p.coeff = size==0 ? NULL : (ap_coeff_t*)malloc(size*sizeof(ap_coeff_t));
    for (i=0; i<size; i++) ap_coeff_init(&e->p.coeff[i],AP_COEFF_SCALAR);
    break;
  case AP_LINEXPR_SPARSE:
    e->p.linterm = size==0 ? NULL : (ap_linterm_t*)malloc(size*sizeof(ap_linterm_t));
    for (i=0; i<size; i++){
      ap_coeff_init(&e->p.linterm[i].coeff,AP_COEFF_SCALAR);
      e->p.linterm[i].dim = AP_DIM_MAX;
    }
    break;
  }
  return e;
}

void ap_linexpr0_realloc(ap_linexpr0_t* e, size_t size)
{
  size_t i;
  if (e->size==size)
    return;
  switch (e->discr){
  case AP_LINEXPR_DENSE:
    for (i=size; i<e->size; i++){
      ap_coeff_clear(&e->p.coeff[i]);
    }
    if (size==0){
      free(e->p.coeff);
      e->p.coeff = NULL;
    }
    else {
      e->p.coeff = realloc(e->p.coeff,size*sizeof(ap_coeff_t));
      for (i=e->size; i<size; i++){
	ap_coeff_init(&e->p.coeff[i],AP_COEFF_SCALAR);
      }
    }
    break;
  case AP_LINEXPR_SPARSE:
    for (i=size; i<e->size; i++){
      ap_coeff_clear(&e->p.linterm[i].coeff);
    }
    if (size==0){
      free(e->p.linterm);
      e->p.linterm = NULL;
    }
    else {
      e->p.linterm = realloc(e->p.linterm, size*sizeof(ap_linterm_t));
      for (i=e->size; i<size; i++){
	ap_coeff_init(&e->p.linterm[i].coeff,AP_COEFF_SCALAR);
	e->p.linterm[i].dim = AP_DIM_MAX;
      }
    }
    break;
  }
  e->size = size;
}

void ap_linexpr0_minimize(ap_linexpr0_t* e)
{
  size_t i,j,nsize;

  if (e->discr==AP_LINEXPR_DENSE){
    for (i=0; i<e->size; i++){
      ap_coeff_reduce(&e->p.coeff[i]);
    }
  }
  else {
    nsize = 0;
    for (i=0; i<e->size; i++){
      ap_coeff_t* p = &e->p.linterm[i].coeff;
      ap_coeff_reduce(p);
      if (!ap_coeff_zero(p) && e->p.linterm[i].dim!=AP_DIM_MAX)
	nsize++;
    }
    if (nsize!=e->size){
      ap_linterm_t* linterm = malloc(nsize*sizeof(ap_linterm_t));
      j = 0;
      for (i=0; i<e->size; i++){
	ap_coeff_t* p = &e->p.linterm[i].coeff;
	if (!ap_coeff_zero(p) && e->p.linterm[i].dim!=AP_DIM_MAX){
	  linterm[j] = e->p.linterm[i];
	  j++;
	}
	else
	  ap_coeff_clear(p);
      }
      free(e->p.linterm);
      e->p.linterm = linterm;
      e->size = nsize;
    }
  }
}

ap_linexpr0_t* ap_linexpr0_copy(ap_linexpr0_t* a)
{
  ap_linexpr0_t* e;
  size_t i;

  e = (ap_linexpr0_t*)malloc(sizeof(ap_linexpr0_t));
  ap_coeff_init_set(&e->cst,&a->cst);
  e->discr = a->discr;
  e->size = a->size;
  switch (a->discr){
  case AP_LINEXPR_DENSE:
    e->p.coeff = a->size==0 ? NULL : (ap_coeff_t*)malloc(a->size*sizeof(ap_coeff_t));
    for (i=0; i<a->size; i++)
      ap_coeff_init_set(&e->p.coeff[i],&a->p.coeff[i]);
    break;
  case AP_LINEXPR_SPARSE:
    e->p.linterm = a->size==0 ? NULL : (ap_linterm_t*)malloc(a->size*sizeof(ap_linterm_t));
    for (i=0; i<a->size; i++){
      ap_coeff_init_set(&e->p.linterm[i].coeff,&a->p.linterm[i].coeff);
      e->p.linterm[i].dim = a->p.linterm[i].dim;
    }
    break;
  }
  e->size = a->size;
  return e;
}

static
void ap_linexpr0_clear(ap_linexpr0_t* e)
{
  size_t i;

  if(e!=NULL){
    ap_coeff_clear(&e->cst);
    switch (e->discr){
    case AP_LINEXPR_DENSE:
      if (e->p.coeff!=NULL){
	for (i=0; i<e->size; i++) ap_coeff_clear(&e->p.coeff[i]);
	free(e->p.coeff);
      }
      break;
    case AP_LINEXPR_SPARSE:
      if (e->p.linterm!=NULL){
	for (i=0; i<e->size; i++) ap_coeff_clear(&e->p.linterm[i].coeff);
	free(e->p.linterm);
      }
      break;
    }
  }
}

void ap_linexpr0_free(ap_linexpr0_t* e)
{
  ap_linexpr0_clear(e);
  free(e);
}

void ap_linexpr0_print(ap_linexpr0_t* a, char** name_of_dim)
{ ap_linexpr0_fprint(stdout,a,name_of_dim); }
void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t* a, char** name_of_dim)
{
  size_t i;
  ap_scalar_t* pscalar = 0;
  ap_scalar_t* scalar;
  ap_coeff_t* coeff;
  ap_dim_t dim;
  bool first;
  int sgn;

  scalar = ap_scalar_alloc();

  first = true;
  ap_linexpr0_ForeachLinterm(a,i,dim,coeff){
    if (! ap_coeff_zero(coeff)){
      switch(coeff->discr){
      case AP_COEFF_SCALAR:
	pscalar = coeff->val.scalar;
	sgn = ap_scalar_sgn(pscalar);
	if (sgn > 0){
	  ap_scalar_set(scalar,pscalar);
	  if (!first)
	    fprintf(stream," + ");
	} else {
	  ap_scalar_neg(scalar,pscalar);
	  fprintf(stream, first ? "-" : " - ");
	}
	if (!ap_scalar_equal_int(scalar,1))
	  ap_scalar_fprint(stream,scalar);
	break;
      case AP_COEFF_INTERVAL:
	if (!first)
	  fprintf(stream," + ");
	ap_interval_fprint(stream,coeff->val.interval);
	break;
      }
      if (name_of_dim)
	fprintf(stream,"%s",name_of_dim[dim]);
      else
	fprintf(stream,"x%lu",(unsigned long)dim);
      first = false;
    }
  }
  /* Constant */
  if (first || !ap_coeff_zero(&a->cst)){
    switch (a->cst.discr){
    case AP_COEFF_SCALAR:
      pscalar = a->cst.val.scalar;
      sgn = ap_scalar_sgn(pscalar);
      if (sgn >= 0){
	ap_scalar_set(scalar,pscalar);
	if (!first)
	  fprintf(stream," + ");
      } else {
	ap_scalar_neg(scalar,pscalar);
	fprintf(stream, first ? "-" : " - ");
      }
      ap_scalar_fprint(stream,scalar);
      break;
    case AP_COEFF_INTERVAL:
      if (!first)
	fprintf(stream," + ");
      ap_interval_fprint(stream, a->cst.val.interval);
      break;
    }
  }
  ap_scalar_free(scalar);
}

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

/* Does the linear expression involve only real variables ? */
bool ap_linexpr0_is_real(ap_linexpr0_t* a, size_t intdim)
{
  ap_coeff_t* coeff;
  ap_dim_t dim;
  size_t i;
  ap_linexpr0_ForeachLinterm(a,i,dim,coeff){
    if (dim>=intdim) return true;
    if (! ap_coeff_zero(coeff)) return false;
  }
  return true;
}

/* Does the linear expression involve only integer variables ? */
bool ap_linexpr0_is_integer(ap_linexpr0_t* a, size_t intdim)
{
  size_t i;
  ap_coeff_t* coeff;
  ap_dim_t dim;

  switch (a->discr){
  case AP_LINEXPR_DENSE:
    for (dim=intdim; dim<a->size; dim++){
      if (! ap_coeff_zero(&a->p.coeff[dim]))
	return false;
    }
    break;
  case AP_LINEXPR_SPARSE:
    ap_linexpr0_ForeachLinterm(a,i,dim,coeff){
      if (dim>=intdim && ! ap_coeff_zero(coeff))
	return false;
    }
    break;
  default:
    abort();
  }
  return true;
}

/* Return the rtype of an expression */
ap_linexpr_type_t ap_linexpr0_type(ap_linexpr0_t* expr)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  ap_linexpr_type_t res = AP_LINEXPR_LINEAR;
  ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
    if (coeff->discr!=AP_COEFF_SCALAR){
      res = AP_LINEXPR_INTLINEAR;
      break;
    }
  }
  if (res==AP_LINEXPR_LINEAR && expr->cst.discr!=AP_COEFF_SCALAR){
    res = AP_LINEXPR_QUASILINEAR;
  }
  return res;
}
/* Return true iff all involved coefficients are scalars */
bool ap_linexpr0_is_linear(ap_linexpr0_t* expr)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  bool res;

  res = (expr->cst.discr==AP_COEFF_SCALAR);
  if (res){
    ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
      res = (coeff->discr==AP_COEFF_SCALAR);
      if (!res) break;
    }
  }
  return res;
}
/* Return true iff all involved coefficients but the constant are scalars */
bool ap_linexpr0_is_quasilinear(ap_linexpr0_t* expr)
{
  size_t i;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  bool res;

  res = true;
  ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
    res = (coeff->discr==AP_COEFF_SCALAR);
    if (!res) break;
  }
  return res;
}
ap_linexpr_type_t ap_linexpr0_array_type(ap_linexpr0_t** texpr, size_t size)
{
  size_t i;
  ap_linexpr_type_t res = AP_LINEXPR_LINEAR;
  for (i=0; i<size; i++){
    ap_linexpr_type_t type = ap_linexpr0_type(texpr[i]);
    if (type<res) res = type;
    if (res==AP_LINEXPR_INTLINEAR) break;
  }
  return res;
}
bool ap_linexpr0_array_is_linear(ap_linexpr0_t** texpr, size_t size)
{
  size_t i;
  bool res = true;
  for (i=0; i<size; i++){
    res = ap_linexpr0_is_linear(texpr[i]);
    if (!res) break;
  }
  return res;
}
bool ap_linexpr0_array_is_quasilinear(ap_linexpr0_t** texpr, size_t size)
{
  size_t i;
  bool res = true;
  for (i=0; i<size; i++){
    res = ap_linexpr0_is_quasilinear(texpr[i]);
    if (!res) break;
  }
  return res;
}


/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static size_t index_of_or_after_dim(ap_dim_t dim, ap_linterm_t* linterm, size_t size)
{
  if (size<=10){
    size_t i;
    for (i=0; i<size; i++){
      if (dim<=linterm[i].dim)
	return i;
    }
    return size;
  }
  else {
    size_t mid = size/2;
    if (dim==linterm[mid].dim)
      return mid;
    else if (dim<linterm[mid].dim)
      return index_of_or_after_dim(dim, linterm, mid);
    else
      return mid + 1 + index_of_or_after_dim(dim, linterm+mid+1, size-mid-1);
  }
}

ap_coeff_t* ap_linexpr0_coeffref(ap_linexpr0_t* expr, ap_dim_t dim)
{
  size_t index;

  switch(expr->discr){
  case AP_LINEXPR_DENSE:
    return (dim<expr->size) ? &expr->p.coeff[dim] : NULL;
  case AP_LINEXPR_SPARSE:
    if (dim==AP_DIM_MAX) return NULL;
    index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
    if (index>=expr->size || dim!=expr->p.linterm[index].dim){
      if (index<expr->size && expr->p.linterm[index].dim==AP_DIM_MAX){
	/* We have a free linterm at the right place */
	expr->p.linterm[index].dim=dim;
	return &expr->p.linterm[index].coeff;
      }
      if (expr->size==0 || expr->p.linterm[expr->size-1].dim!=AP_DIM_MAX){
	/* We have to insert a new linterm at the end */
	ap_linexpr0_realloc(expr, expr->size+1);
      }
      /* We insert a linterm with AP_DIM_MAX at the right place */
      if (index<expr->size-1){
	ap_linterm_t tmp = expr->p.linterm[expr->size-1];
	memmove(&expr->p.linterm[index+1], &expr->p.linterm[index],
		(expr->size-index-1)*sizeof(ap_linterm_t));
	expr->p.linterm[index] = tmp;
      }
      expr->p.linterm[index].dim = dim;
    }
    return &expr->p.linterm[index].coeff;
  default:
    abort();
  }
}

/* If dense representation, coefficients should be already present, otherwise
   undefined behaviour */
bool ap_linexpr0_get_coeff(ap_coeff_t* coeff, ap_linexpr0_t* expr, ap_dim_t dim)
{
  size_t index;
  switch(expr->discr){
  case AP_LINEXPR_DENSE:
    if (dim<expr->size){
      ap_coeff_set(coeff,&expr->p.coeff[dim]);
      return false;
    } else {
      return true;
    }
  case AP_LINEXPR_SPARSE:
    if (dim==AP_DIM_MAX)
      return true;
    else {
      index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
      if (index<expr->size && dim==expr->p.linterm[index].dim){
	ap_coeff_set(coeff, &expr->p.linterm[index].coeff);
      }
      else {
	ap_coeff_set_scalar_double(coeff,0.0);
      }
      return false;
    }
  default:
    abort();
  }
}

bool ap_linexpr0_set_list_generic(ap_coeff_t* (*get_pcoeff)(void* expr, bool cst, va_list* va),
				  void* expr, va_list* va)
{
  bool cst;
  ap_coeff_t* coeff;
  ap_coeff_t* pcoeff;
  ap_scalar_t *scalar,*scalar1,*scalar2;
  ap_interval_t* interval;
  int num,num1,num2,den,den1,den2;
  double k,k1,k2;
  MP_RAT *mpq,*mpq1,*mpq2;
  __mpfr_struct *mpfr,*mpfr1,*mpfr2;
  ap_coefftag_t tag;

  while (true){
    tag = va_arg(*va,ap_coefftag_t);
    if (tag==AP_END)
      break;

    cst = (tag>=AP_CST);
    if (cst)
      tag-=AP_CST;

    switch (tag){
    case AP_COEFF:
      coeff = va_arg(*va,ap_coeff_t*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set(pcoeff,coeff);
      break;
    case AP_COEFF_S:
      scalar = va_arg(*va,ap_scalar_t*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar(pcoeff,scalar);
      break;
    case AP_COEFF_S_MPQ:
      mpq = va_arg(*va,MP_RAT*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar_mpq(pcoeff,mpq);
      break;
    case AP_COEFF_S_MPFR:
      mpfr = va_arg(*va,__mpfr_struct*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar_mpfr(pcoeff,mpfr);
      break;
    case AP_COEFF_S_INT:
      num = va_arg(*va,int);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar_int(pcoeff,num);
      break;
    case AP_COEFF_S_FRAC:
      num = va_arg(*va,int);
      den = va_arg(*va,int);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar_frac(pcoeff,num,den);
      break;
    case AP_COEFF_S_DOUBLE:
      k = va_arg(*va,double);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_scalar_double(pcoeff,k);
      break;
    case AP_COEFF_I:
      interval = va_arg(*va,ap_interval_t*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval(pcoeff,interval);
      break;
    case AP_COEFF_I_SCALAR:
      scalar1 = va_arg(*va,ap_scalar_t*);
      scalar2 = va_arg(*va,ap_scalar_t*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_scalar(pcoeff,scalar1,scalar2);
      break;
    case AP_COEFF_I_MPQ:
      mpq1 = va_arg(*va,MP_RAT*);
      mpq2 = va_arg(*va,MP_RAT*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_mpq(pcoeff,mpq1,mpq2);
      break;
    case AP_COEFF_I_MPFR:
      mpfr1 = va_arg(*va,__mpfr_struct*);
      mpfr2 = va_arg(*va,__mpfr_struct*);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_mpfr(pcoeff,mpfr1,mpfr2);
      break;
    case AP_COEFF_I_INT:
      num1 = va_arg(*va,int);
      num2 = va_arg(*va,int);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_int(pcoeff,num1,num2);
      break;
    case AP_COEFF_I_FRAC:
      num1 = va_arg(*va,int);
      den1 = va_arg(*va,int);
      num2 = va_arg(*va,int);
      den2 = va_arg(*va,int);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_frac(pcoeff,num1,den1,num2,den2);
      break;
    case AP_COEFF_I_DOUBLE:
      k1 = va_arg(*va,double);
      k2 = va_arg(*va,double);
      pcoeff = get_pcoeff(expr,cst,va);
      if (pcoeff==NULL) return true;
      ap_coeff_set_interval_double(pcoeff,k1,k2);
      break;
    default:
      fprintf(stderr,
	      "ap_linexpr0_set_list_generic: probably bad structure for the argument list\n");
      abort();
    }
  }
  return false;
}

ap_coeff_t* ap_linexpr0_set_list_get_pcoeff(void* expr, bool cst, va_list* va)
{
  ap_coeff_t* pcoeff;
  if (cst){
    pcoeff = ap_linexpr0_cstref(expr);
  } else {
    ap_dim_t dim = va_arg(*va,ap_dim_t);
    pcoeff = ap_linexpr0_coeffref(expr,dim);
  }
  return pcoeff;
}

bool ap_linexpr0_set_list(ap_linexpr0_t* expr, ...)
{
  va_list va;
  bool res;
  va_start(va,expr);
  res = ap_linexpr0_set_list_generic(ap_linexpr0_set_list_get_pcoeff,
				     expr,&va);
  va_end(va);
  return res;
}

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* Change current environment with a super-environment */

static int ap_linterm_cmp(const void* a, const void* b)
{
  const ap_linterm_t* aa = (const ap_linterm_t*)a;
  const ap_linterm_t* bb = (const ap_linterm_t*)b;
  return
    (aa->dim > bb->dim) ? 1 :
    ( (aa->dim < bb->dim) ? -1 : 0 );
}

ap_linexpr0_t*
ap_linexpr0_add_dimensions(ap_linexpr0_t* expr,
			   ap_dimchange_t* dimchange)
{
  ap_linexpr0_t* nexpr;

  if (expr==NULL) return NULL;
  nexpr = ap_linexpr0_copy(expr);
  ap_linexpr0_add_dimensions_with(nexpr,dimchange);
  return nexpr;
}

void
ap_linexpr0_add_dimensions_with(ap_linexpr0_t* expr,
				ap_dimchange_t* dimchange)
{
  if (expr==NULL) return;
  switch(expr->discr){
  case AP_LINEXPR_SPARSE:
    {
      size_t i,k,dimsup;
      dimsup = dimchange->intdim+dimchange->realdim;
      k=0;
      for (i=0; i<expr->size; i++){
	ap_dim_t* pdim = &expr->p.linterm[i].dim;
	if (*pdim==AP_DIM_MAX)
	  break;
	while (k<dimsup && *pdim>=dimchange->dim[k]){
	  k++;
	}
	*pdim += k;
      }
    }
    break;
  case AP_LINEXPR_DENSE:
    {
      int i,k;
      size_t size,dimsup;

      size = expr->size;
      dimsup = dimchange->intdim+dimchange->realdim;
      ap_linexpr0_realloc(expr,
			  size+dimsup);
      k = dimsup;
      for (i=size; i>=0; i--){
	if (i<(int)size){
	  ap_coeff_set(&expr->p.coeff[i+k],&expr->p.coeff[i]);
	}
	while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
	  k--;
	  ap_coeff_set_scalar_double(&expr->p.coeff[i+k],0.0);
	}
      }
    }
    break;
  default:
    abort();
  }
}

ap_linexpr0_t*
ap_linexpr0_permute_dimensions(ap_linexpr0_t* expr,
			       ap_dimperm_t* perm)
{
  if (expr==NULL) return NULL;
  ap_linexpr0_t* nexpr = ap_linexpr0_copy(expr);
  switch(nexpr->discr){
  case AP_LINEXPR_SPARSE:
    ap_linexpr0_permute_dimensions_with(nexpr,perm);
    break;
  case AP_LINEXPR_DENSE:
    {
      size_t i;
      for (i=0; i<nexpr->size; i++){
	ap_coeff_set(&nexpr->p.coeff[perm->dim[i]],&expr->p.coeff[i]);
      }
    }
    break;
  default:
    abort();
  }
  return nexpr;
}
void
ap_linexpr0_permute_dimensions_with(ap_linexpr0_t* expr,
				    ap_dimperm_t* perm)
{
  if (expr==NULL) return;
  switch(expr->discr){
  case AP_LINEXPR_SPARSE:
    {
      size_t i;
      for (i=0; i<expr->size; i++){
	ap_dim_t dim = expr->p.linterm[i].dim;
	if (dim==AP_DIM_MAX) continue;
	expr->p.linterm[i].dim = perm->dim[dim];
      }
      qsort(expr->p.linterm,
	    expr->size,
	    sizeof(ap_linterm_t),
	    &ap_linterm_cmp);
    }
    break;
  case AP_LINEXPR_DENSE:
    {
      ap_linexpr0_t* nexpr = ap_linexpr0_permute_dimensions(expr,perm);
      ap_linexpr0_clear(expr);
      *expr = *nexpr;
      free(nexpr);
    }
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

long ap_linexpr0_hash(ap_linexpr0_t* expr)
{
  if (expr->size==0){
    return ap_coeff_hash(&expr->cst);
  }
  else {
    ap_coeff_t* pcoeff;
    size_t i,dec;
    long res,res1;
    res = expr->size << 8;
    dec = 0;
    for (i=0; i<expr->size; i += (expr->size+7)/8){
      pcoeff = ap_linexpr0_coeffref(expr,i);
      res1 = (pcoeff==NULL) ? 0 : ap_coeff_hash(pcoeff);
      res += res1<<dec;
      dec++;
    }
    return res;
  }
}
bool ap_linexpr0_equal(ap_linexpr0_t* expr1,
		       ap_linexpr0_t* expr2)
{
  bool res;
  size_t i1,i2;
  ap_dim_t dim1,dim2;
  ap_coeff_t* coeff1;
  ap_coeff_t* coeff2;

  ap_coeff_reduce(&expr1->cst);
  ap_coeff_reduce(&expr2->cst);
  res = ap_coeff_equal(&expr1->cst,&expr2->cst);
  i1 = i2 = 0;
  while (res && (i1<expr1->size || i2<expr2->size)){
    if (i1<expr1->size){
      if (expr1->discr==AP_LINEXPR_DENSE){
	dim1 = i1; coeff1 = &expr1->p.coeff[i1];
      } else {
	dim1 = expr1->p.linterm[i1].dim; coeff1 = &expr1->p.linterm[i1].coeff;
      }
      ap_coeff_reduce(coeff1);
    }
    else {
      dim1 = AP_DIM_MAX;
      coeff1 = NULL;
    }
    if (i2<expr2->size){
      if (expr2->discr==AP_LINEXPR_DENSE){
	dim2 = i2; coeff2 = &expr2->p.coeff[i2];
      } else {
	dim2 = expr2->p.linterm[i2].dim; coeff2 = &expr2->p.linterm[i2].coeff;
      }
      ap_coeff_reduce(coeff2);
    }
    else {
      dim2 = AP_DIM_MAX;
      coeff2 = NULL;
    }
    if (dim1==dim2){
      i1++; i2++;
      res = ap_coeff_equal(coeff1,coeff2);
    }
    else if (dim1<dim2){
      i1++;
      res = ap_coeff_zero(coeff1);
    }
    else { /* if (dim2<dim1) */
      i2++;
      res = ap_coeff_zero(coeff2);
    }
  }
  return res;
}

int ap_linexpr0_compare(ap_linexpr0_t* expr1,
			ap_linexpr0_t* expr2)
{
  bool res;
  size_t i1,i2;
  ap_dim_t dim1,dim2;
  ap_coeff_t* coeff1;
  ap_coeff_t* coeff2;
  ap_coeff_t* coeffzero;

  coeffzero = ap_coeff_alloc(AP_COEFF_SCALAR);
  ap_coeff_set_scalar_double(coeffzero,0.0);
  res = 0;
  i1 = i2 = 0;
  while (res==0 && (i1<expr1->size || i2<expr2->size)){
    if (i1<expr1->size){
      if (expr1->discr==AP_LINEXPR_DENSE){
	dim1 = i1; coeff1 = &expr1->p.coeff[i1];
      } else {
	dim1 = expr1->p.linterm[i1].dim; coeff1 = &expr1->p.linterm[i1].coeff;
      }
      ap_coeff_reduce(coeff1);
    }
    else {
      dim1 = AP_DIM_MAX;
      coeff1 = NULL;
    }
    if (i2<expr2->size){
      if (expr2->discr==AP_LINEXPR_DENSE){
	dim2 = i2; coeff2 = &expr2->p.coeff[i2];
      } else {
	dim2 = expr2->p.linterm[i2].dim; coeff2 = &expr2->p.linterm[i2].coeff;
      }
      ap_coeff_reduce(coeff2);
    }
    else {
      dim2 = AP_DIM_MAX;
      coeff2 = NULL;
    }
    if (dim1==dim2){
      i1++; i2++;
      res = ap_coeff_cmp(coeff1,coeff2);
    }
    else if (dim1<dim2){
      i1++;
      res = ap_coeff_cmp(coeff1,coeffzero);
    }
    else { /* if (dim2<dim1) */
      i2++;
      res = ap_coeff_cmp(coeffzero,coeff2);
    }
  }
  if (res==0){
    ap_coeff_reduce(&expr1->cst);
    ap_coeff_reduce(&expr2->cst);
    res = ap_coeff_cmp(&expr1->cst,&expr2->cst);
  }
  ap_coeff_free(coeffzero);
  return res;
}

/* ====================================================================== */
/* Vb. Array of expressions */
/* ====================================================================== */

void ap_linexpr0_array_free(ap_linexpr0_t** texpr, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    if (texpr[i]){
      ap_linexpr0_free(texpr[i]);
      texpr[i] = NULL;
    }
  }
  free(texpr);
}
