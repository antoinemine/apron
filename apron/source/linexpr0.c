/* ************************************************************************* */
/* linexpr0.c: linear expressions */
/* ************************************************************************* */

#include "linexpr0.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

linexpr0_t* linexpr0_alloc(linexpr_discr_t lin_discr, size_t size)
{
  linexpr0_t* e;
  int i;

  e = (linexpr0_t*)malloc(sizeof(linexpr0_t));
  coeff_init(&e->cst,COEFF_SCALAR);
  e->discr = lin_discr;
  e->size = size;
  switch (lin_discr){
  case LINEXPR_DENSE:
    e->p.coeff = size==0 ? NULL : (coeff_t*)malloc(size*sizeof(coeff_t));
    for (i=0; i<size; i++) coeff_init(&e->p.coeff[i],COEFF_SCALAR);
    break;
  case LINEXPR_SPARSE:
    e->p.linterm = size==0 ? NULL : (linterm_t*)malloc(size*sizeof(linterm_t));
    for (i=0; i<size; i++){
      coeff_init(&e->p.linterm[i].coeff,COEFF_SCALAR);
      e->p.linterm[i].dim = DIM_MAX;
    }
    break;
  }
  return e;
}

void linexpr0_realloc(linexpr0_t* e, size_t size)
{
  size_t i;
  if (e->size==size)
    return;
  switch (e->discr){
  case LINEXPR_DENSE:
    for (i=size; i<e->size; i++){
      coeff_clear(&e->p.coeff[i]);
    }
    if (size==0){
      free(e->p.coeff);
      e->p.coeff = NULL;
    }
    else {
      e->p.coeff = realloc(e->p.coeff,size*sizeof(coeff_t));
      for (i=e->size; i<size; i++){
	coeff_init(&e->p.coeff[i],COEFF_SCALAR);
      }
    }
    break;
  case LINEXPR_SPARSE:
    for (i=size; i<e->size; i++){
      coeff_clear(&e->p.linterm[i].coeff);
    }
    if (size==0){
      free(e->p.linterm);
      e->p.linterm = NULL;
    }
    else {
      e->p.linterm = realloc(e->p.linterm, size*sizeof(linterm_t));
      for (i=e->size; i<size; i++){
	coeff_init(&e->p.linterm[i].coeff,COEFF_SCALAR);
	e->p.linterm[i].dim = DIM_MAX;
      }
    }
    break;
  }
  e->size = size;
}

void linexpr0_minimize(linexpr0_t* e)
{
  size_t i,j,nsize;

  if (e->discr==LINEXPR_DENSE){
    for (i=0; i<e->size; i++){
      coeff_reduce(&e->p.coeff[i]);
    }
  }
  else {
    nsize = 0;
    for (i=0; i<e->size; i++){
      coeff_t* p = &e->p.linterm[i].coeff;
      coeff_reduce(p);
      if (coeff_zero(p))
	nsize++;
    }
    if (nsize!=e->size){
      linterm_t* linterm = malloc(nsize*sizeof(linterm_t));
      j = 0;
      for (i=0; i<e->size; i++){
	coeff_t* p = &e->p.linterm[i].coeff;
	if (! coeff_zero(p)){
	  linterm[j] = e->p.linterm[i];
	  j++;
	}
	else 
	  coeff_clear(p);
      }
      free(e->p.linterm);
      e->p.linterm = linterm;
      e->size = nsize;
    }
  }
}

linexpr0_t* linexpr0_copy(const linexpr0_t* a)
{
  linexpr0_t* e;
  int i;

  e = (linexpr0_t*)malloc(sizeof(linexpr0_t));
  coeff_init_set(&e->cst,&((linexpr0_t*)a)->cst);
  e->discr = a->discr;
  e->size = a->size;
  switch (a->discr){
  case LINEXPR_DENSE:
    e->p.coeff = a->size==0 ? NULL : (coeff_t*)malloc(a->size*sizeof(coeff_t));
    for (i=0; i<a->size; i++)
      coeff_init_set(&e->p.coeff[i],&a->p.coeff[i]);
    break;
  case LINEXPR_SPARSE:
    e->p.linterm = a->size==0 ? NULL : (linterm_t*)malloc(a->size*sizeof(linterm_t));
    for (i=0; i<a->size; i++){
      coeff_init_set(&e->p.linterm[i].coeff,&a->p.linterm[i].coeff);
      e->p.linterm[i].dim = a->p.linterm[i].dim;
    }
    break;
  }
  e->size = a->size;
  return e;
}

static
void linexpr0_clear(linexpr0_t* e)
{
  int i;

  if(e!=NULL){
    coeff_clear(&e->cst);
    switch (e->discr){
    case LINEXPR_DENSE:
      if (e->p.coeff!=NULL){
	for (i=0; i<e->size; i++) coeff_clear(&e->p.coeff[i]);
	free(e->p.coeff);
      }
      break;
    case LINEXPR_SPARSE:
      if (e->p.linterm!=NULL){
	for (i=0; i<e->size; i++) coeff_clear(&e->p.linterm[i].coeff);
	free(e->p.linterm);
      }
      break;
    }
  }
}

void linexpr0_free(linexpr0_t* e)
{
  linexpr0_clear(e);
  free(e);
}

void linexpr0_fprint(FILE* stream, const linexpr0_t* a, char** name_of_dim)
{
  size_t i;
  scalar_t* scalar = 0;
  scalar_t* scalar2;
  scalar_t* one;
  coeff_t* coeff;
  dim_t dim;
  bool first;
  int sgn;

  scalar2 = scalar_alloc();
  one = scalar_alloc();
  scalar_set_int(one,1);

  first = true;
  linexpr0_ForeachLinterm(a,i,dim,coeff){
    if (! coeff_zero(coeff)){
      switch(coeff->discr){
      case COEFF_SCALAR:
	scalar = coeff->val.scalar;
	sgn = scalar_sgn(scalar);
	if (sgn > 0){
	  scalar_set(scalar2,scalar);
	  if (!first)
	    fprintf(stream," + ");
	} else {
	  scalar_neg(scalar2,scalar);
	  fprintf(stream, first ? "-" : " - ");
	}
	if (!scalar_equal(scalar2,one))
	  scalar_fprint(stream,scalar2);
	first = false;
	break;
      case COEFF_INTERVAL:
	if (!first)
	  fprintf(stream," + ");
	interval_fprint(stream,coeff->val.interval);
	break;
      }
      if (name_of_dim)
	fprintf(stream,name_of_dim[dim]);
      else
	fprintf(stream,"x%d",dim);
    }
  }
  /* Constant */
  if (! coeff_zero(&a->cst)){
    switch (a->cst.discr){
    case COEFF_SCALAR:
      scalar = a->cst.val.scalar;
      sgn = scalar_sgn(scalar);
      if (sgn > 0){
	scalar_set(scalar2,scalar);
	if (!first)
	  fprintf(stream," + ");
      } else {
	scalar_neg(scalar2,scalar);
	fprintf(stream, first ? "-" : " - ");
      }
      scalar_fprint(stream,scalar2);
      break;
    case COEFF_INTERVAL:
      if (!first)
	fprintf(stream," + ");
      interval_fprint(stream, a->cst.val.interval);
      break;
    }
  }
  scalar_free(one);
  scalar_free(scalar);
}

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

/* Does the linear expression involve only real variables ? */
bool linexpr0_is_real(const linexpr0_t* a, size_t intdim)
{
  coeff_t* coeff;
  dim_t dim;
  size_t i;
  linexpr0_ForeachLinterm(a,i,dim,coeff){
    if (dim>=intdim) return true;
    if (! coeff_zero(coeff)) return false;
  }
  return true;
}

/* Does the linear expression involve only integer variables ? */
bool linexpr0_is_integer(const linexpr0_t* a, size_t intdim)
{
  size_t i;
  coeff_t* coeff;
  dim_t dim;

  switch (a->discr){
  case LINEXPR_DENSE:
    for (dim=intdim; dim<a->size; dim++){
      if (! coeff_zero(&a->p.coeff[dim]))
	return false;
    }
    break;
  case LINEXPR_SPARSE:
    linexpr0_ForeachLinterm(a,i,dim,coeff){
      if (dim>=intdim && ! coeff_zero(coeff))
	return false;
    }
    break;
  default:
    abort();
  }
  return true;
}

/* Return true iff all involved coefficients are scalars */
bool linexpr0_is_linear(const linexpr0_t* expr)
{
  size_t i;
  dim_t dim;
  coeff_t* coeff;
  bool res;

  res = (expr->cst.discr==COEFF_SCALAR);
  if (res){
    linexpr0_ForeachLinterm(e,i,dim,coeff){
      res = (coeff->discr==COEFF_SCALAR);
      if (!res) break;
    }
  }
  return res;
}
/* Return true iff all involved coefficients but the constant are scalars */
bool linexpr0_is_quasilinear(const linexpr0_t* a)
{
  size_t i;
  dim_t dim;
  coeff_t* coeff;
  bool res;

  res = true;
  if (res){
    linexpr0_ForeachLinterm(e,i,dim,coeff){
      res = (coeff->discr==COEFF_SCALAR);
      if (!res) break;
    }
  }
  return res;
}

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static size_t index_of_or_after_dim(dim_t dim, linterm_t* linterm, size_t size)
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

coeff_t* linexpr0_coeffref(linexpr0_t* expr, dim_t dim)
{
  size_t index;

  switch(expr->discr){
  case LINEXPR_DENSE:
    return (dim<expr->size) ? &expr->p.coeff[dim] : NULL;
  case LINEXPR_SPARSE:
    if (dim==DIM_MAX) return NULL;
    index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
    if (index>=expr->size || dim!=expr->p.linterm[index].dim){
      if (index<expr->size && expr->p.linterm[index].dim==DIM_MAX){
	/* We have a free linterm at the right place */
	expr->p.linterm[index].dim=dim;
	return &expr->p.linterm[index].coeff;
      }
      if (expr->size==0 || expr->p.linterm[expr->size-1].dim!=DIM_MAX){
	/* We have to insert a new linterm at the end */
	linexpr0_realloc(expr, expr->size+1);
      }
      /* We insert a linterm with DIM_MAX at the right place */
      if (index<expr->size-1){
	linterm_t tmp = expr->p.linterm[expr->size-1];
	memmove(&expr->p.linterm[index+1], &expr->p.linterm[index],
		(expr->size-index-1)*sizeof(linterm_t));
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
bool linexpr0_get_coeff(coeff_t* coeff, const linexpr0_t* expr, dim_t dim)
{
  size_t index;
  switch(expr->discr){
  case LINEXPR_DENSE:
    if (dim<expr->size){
      coeff_set(coeff,&expr->p.coeff[dim]);
      return false;
    } else {
      return true;
    }
  case LINEXPR_SPARSE:
    if (dim==DIM_MAX)
      return true;
    else {
      index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
      if (index<expr->size && dim==expr->p.linterm[index].dim){
	coeff_set(coeff, &expr->p.linterm[index].coeff);
      }
      else {
	coeff_set_scalar_double(coeff,0.0);
      }
      return false;
    }
  default:
    abort();
  }
}

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* Allocating a transformation */
dimchange_t* dimchange_alloc(size_t intdim, size_t realdim)
{ 
  dimchange_t* res = malloc(sizeof(dimchange_t)+(intdim+realdim)*sizeof(dim_t));
  res->intdim = intdim;
  res->realdim = realdim;
  return res;
}

/* Inverting in-place an adding transformation from the given dimensionality */
void dimchange_add_invert(dimchange_t* dimchange)
{
  size_t i;
  for (i=0;i<dimchange->intdim+dimchange->realdim; i++){
    dimchange->p[i] += i;
  }
}


/* Change current environment with a super-environment */

static int linterm_cmp(const void* a, const void* b)
{
  linterm_t* aa = (linterm_t*)a;
  linterm_t* bb = (linterm_t*)b;
  return
    (aa->dim > bb->dim) ? 1 :
    ( (aa->dim < bb->dim) ? -1 : 0 );
}

linexpr0_t*
linexpr0_add_dimensions(const linexpr0_t* expr,
			const dimchange_t* dimchange)
{
  if (expr==NULL) return NULL;
  linexpr0_t* nexpr = linexpr0_copy(expr);
  switch(nexpr->discr){
  case LINEXPR_SPARSE:
    linexpr0_add_dimensions_with(nexpr,dimchange);
    break;
  case LINEXPR_DENSE:
    {
      size_t i,k,dimsup;
      dimsup = dimchange->intdim+dimchange->realdim;
      linexpr0_realloc(nexpr,
		       nexpr->size+dimsup);
      k=0;
      for (i=0; i<expr->size; i++){
	while (k<dimsup && i==dimchange->dim[k]){
	  coeff_set_scalar_double(&nexpr->p.coeff[i+k],0.0);
	  k++;
	}
	coeff_set(&nexpr->p.coeff[i+k],&expr->p.coeff[i]);
      }
      while (k<dimsup){
	coeff_set_scalar_double(&nexpr->p.coeff[i+k],0.0);
	k++;
      }
    }
    break;
  default:
    abort();
  }
  return nexpr;
}

void
linexpr0_add_dimensions_with(linexpr0_t* expr,
			     const dimchange_t* dimchange)
{
  if (expr==NULL) return;
  switch(expr->discr){
  case LINEXPR_SPARSE:
    {
      size_t i,k,dimsup;
      dimsup = dimchange->intdim+dimchange->realdim;
      k=0;
      for (i=0; i<expr->size; i++){
	while (k<dimsup && i==dimchange->dim[k]){	
	  expr->p.linterm[i].dim++;
	  k++;
	}
      }
    }
    break;
  case LINEXPR_DENSE:
    {
      linexpr0_t* nexpr = linexpr0_add_dimensions(expr,dimchange);
      linexpr0_clear(expr);
      *expr = *nexpr;
    }
    break;
  default:
    abort();
  }
}

linexpr0_t*
linexpr0_permute_dimensions(const linexpr0_t* expr,
			    const dim_t* perm)
{
  if (expr==NULL) return NULL;
  linexpr0_t* nexpr = linexpr0_copy(expr);
  switch(nexpr->discr){
  case LINEXPR_SPARSE:
    linexpr0_permute_dimensions_with(nexpr,perm);
    break;
  case LINEXPR_DENSE:
    {
      size_t i;
      for (i=0; i<nexpr->size; i++){
	coeff_set(&nexpr->p.coeff[perm[i]],&expr->p.coeff[i]);
      }
    }
    break;
  default:
    abort();
  }
  return nexpr;
}
void
linexpr0_permute_dimensions_with(linexpr0_t* expr,
				 const dim_t* perm)
{
  if (expr==NULL) return;
  switch(expr->discr){
  case LINEXPR_SPARSE:
    {
      size_t i;
      for (i=0; i<expr->size; i++){
	dim_t dim = expr->p.linterm[i].dim;
	expr->p.linterm[i].dim = perm[dim];
      }
      qsort(expr->p.linterm,
	    expr->size,
	    sizeof(linterm_t),linterm_cmp);
    }
    break;
  case LINEXPR_DENSE:
    {
      linexpr0_t* nexpr = linexpr0_permute_dimensions(expr,perm);
      linexpr0_clear(expr);
      *expr = *nexpr;
    }
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

int linexpr0_hash(const linexpr0_t* expr)
{
  if (expr->size==0){
    return coeff_hash(&expr->cst);
  }
  else {
    size_t i,dec;
    long res;
    res = expr->size << 8;
    dec = 0;
    for (i=0; i<expr->size; i += (expr->size+7)/8){
      res += coeff_hash(expr->discr == LINEXPR_DENSE ?
			&expr->p.coeff[i] :
			&expr->p.linterm[i].coeff) << dec;
      dec++;
    }
    return res;
  }
}
bool linexpr0_equal(linexpr0_t* expr1,
		    linexpr0_t* expr2)
{
  bool res;
  size_t i1,i2;
  dim_t dim1,dim2;
  coeff_t* coeff1;
  coeff_t* coeff2;

  coeff_reduce(&expr1->cst);
  coeff_reduce(&expr2->cst);
  res = coeff_equal(&expr1->cst,&expr2->cst);
  while (res){
    i1 = i2 = 0;
    while (i1<expr1->size || i2<expr2->size){
      if (i1<expr1->size){
	if (expr1->discr==LINEXPR_DENSE){
	  dim1 = i1; coeff1 = &expr1->p.coeff[i1];
	} else {
	  dim1 = expr1->p.linterm[i1].dim; coeff1 = &expr1->p.linterm[i1].coeff;
	}
	coeff_reduce(coeff1);
      }
      else {
	dim1 = DIM_MAX;
	coeff1 = NULL;
      }
      if (i2<expr2->size){
	if (expr2->discr==LINEXPR_DENSE){
	  dim2 = i2; coeff2 = &expr2->p.coeff[i2];
	} else {
	  dim2 = expr2->p.linterm[i2].dim; coeff2 = &expr2->p.linterm[i2].coeff;
	}
	coeff_reduce(coeff2);
      }
      else {
	dim2 = DIM_MAX;
	coeff2 = NULL;
      }
      if (dim1==dim2){
	i1++; i2++;
	res = coeff_cmp(coeff1,coeff2);
      }
      else if (dim1<dim2){
	i1++;
	res = coeff_zero(coeff1);
      }
      else { /* if (dim2<dim1) */
	i2++;
	res = coeff_zero(coeff2);
      }
    }
  }
  return res;
}

