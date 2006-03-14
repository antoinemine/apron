
/* ************************************************************************* */
/* expr0.c: datatypes for dimensions, expressions and constraints */
/* ************************************************************************* */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "coeff.h"
#include "expr0.h"

/* ********************************************************************** */
/* I. Linear and affine expressions  */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

linexpr0_t* linexpr0_alloc(coeff_discr_t coeff_discr,
			   linexpr_discr_t lin_discr, size_t size,
			   cst_discr_t cst_discr)
{
  linexpr0_t* e;
  int i;

  e = (linexpr0_t*)malloc(sizeof(linexpr0_t));
  cst_init(&e->cst,coeff_discr,cst_discr);
  e->discr = lin_discr;
  switch (lin_discr){
  case LINEXPR_DENSE:
    e->p.coeff = size==0 ? NULL : (coeff_t*)malloc(size*sizeof(coeff_t));
    for (i=0; i<size; i++) coeff_init(&e->p.coeff[i],coeff_discr);
    break;
  case LINEXPR_SPARSE:
    e->p.linterm = size==0 ? NULL : (linterm_t*)malloc(size*sizeof(linterm_t));
    for (i=0; i<size; i++) coeff_init(&e->p.linterm[i].coeff,coeff_discr);
    break;
  }
  e->size = size;
  return e;
}

void linexpr0_realloc(linexpr0_t* e, size_t size, coeff_discr_t coeff_discr)
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
	coeff_init(&e->p.coeff[i],coeff_discr);
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
	coeff_init(&e->p.linterm[i].coeff,coeff_discr);
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

  if (e->discr==LINEXPR_DENSE) return;
  
  nsize = 0;
  for (i=0; i<e->size; i++){
    coeff_t* p = &e->p.linterm[i].coeff;
    if (coeff_sgn(p)==0){
      coeff_clear(p);
      nsize++;
    }
  }
  if (nsize!=e->size){
    linterm_t* linterm = malloc(nsize*sizeof(linterm_t));
    j = 0;
    for (i=0; i<e->size; i++){
      coeff_t* p = &e->p.linterm[i].coeff;
      if (coeff_sgn(p)!=0){
	linterm[j] = e->p.linterm[i];
	j++;
      }
    }
    free(e->p.linterm);
    e->p.linterm = linterm;
    e->size = nsize;
  }
}

linexpr0_t* linexpr0_copy(const linexpr0_t* a)
{
  linexpr0_t* e;
  int i;

  e = (linexpr0_t*)malloc(sizeof(linexpr0_t));
  cst_init_set(&e->cst,&a->cst);
  e->discr = a->discr;
  switch (a->discr){
  case LINEXPR_DENSE:
    e->p.coeff = a->size==0 ? NULL : (coeff_t*)malloc(a->size*sizeof(coeff_t));
    for (i=0; i<a->size; i++) coeff_init_set(&e->p.coeff[i],&a->p.coeff[i]);
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
    cst_clear(&e->cst);
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

void linexpr0_fprint(FILE* stream, const linexpr0_t* a, const var_t* name_of_dim)
{
  size_t i;
  coeff_t coeff, one;
  coeff_t* pcoeff;
  dim_t dim;
  bool first;
  int sgn;

  coeff_init(&one,COEFF_MPQ);
  coeff_set_int(&one,1);
  coeff_init(&coeff,COEFF_DOUBLE);

  first = true;
  linexpr0_ForeachLinterm(a,i,dim,pcoeff){
    sgn = coeff_sgn(pcoeff);
    if (sgn){
      if (sgn > 0){
	coeff_set(&coeff,pcoeff);
	if (!first)
	  fprintf(stream," + ");
      } else {
	coeff_neg(&coeff,pcoeff);
	fprintf(stream, first ? "-" : " - ");
      }
      if (!coeff_equal(&coeff,&one))
	coeff_fprint(stream,&coeff);
      if (name_of_dim)
	fprintf(stream,name_of_dim[dim]);
      else
	fprintf(stream,"x%d",dim);
      first = false;
    }
  }
  /* Constant */
  switch (a->cst.discr){
  case CST_COEFF:
    sgn = coeff_sgn(cst_coeffref(&a->cst));
    if (sgn){
      if (sgn > 0){
	coeff_set(&coeff,cst_coeffref(&a->cst));
	if (!first)
	  fprintf(stream," + ");
      } else {
	coeff_neg(&coeff,cst_coeffref(&a->cst));
	fprintf(stream, first ? "-" : " - ");
      }
      coeff_fprint(stream,&coeff);
    }
    break;
  case CST_INTERVAL:
    if (!first)
      fprintf(stream," + ");
    interval_fprint(stream,cst_intervalref(&a->cst));
    break;
  }
  coeff_clear(&one);
  coeff_clear(&coeff);
}

/* ====================================================================== */
/* I.2 Tests */
/* ====================================================================== */

/* Does the linear expression involve only real variables ? */
bool linexpr0_is_real(const linexpr0_t* a, size_t intdim)
{
  coeff_t* pcoeff;
  dim_t dim;
  size_t i;
  linexpr0_ForeachLinterm(a,i,dim,pcoeff){
    if (dim>=intdim) return true;
    if (coeff_sgn(pcoeff)) return false;
  }
  return true;
}

/* Does the linear expression involve only integer variables ? */
bool linexpr0_is_integer(const linexpr0_t* a, size_t intdim)
{
  size_t i;
  dim_t dim;

  switch (a->discr){
  case LINEXPR_DENSE:
    for (dim=intdim; dim<a->size; dim++){
      if (coeff_sgn(&a->p.coeff[dim]))
	return false;
    }
    break;
  case LINEXPR_SPARSE:
    for (i=0; i<a->size; i++){
      dim = a->p.linterm[i].dim;
      if (dim>=intdim && coeff_sgn(&a->p.linterm[i].coeff))
	return false;
    }
    break;
  default:
    abort();
  }
  return true;
}

/* ====================================================================== */
/* I.3 Access */
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
   assert(dim<expr->size);
   return &expr->p.coeff[dim];
  case LINEXPR_SPARSE:
    index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
    if (index>=expr->size || dim!=expr->p.linterm[index].dim){
      /* We have to insert a new linterm */
      /* We insert at the end */
      linexpr0_realloc(expr, expr->size+1, COEFF_DOUBLE);
      /* We move if necessary */
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
void linexpr0_get_coeff(coeff_t* coeff, const linexpr0_t* expr, dim_t dim)
{
  size_t index;
  switch(expr->discr){
  case LINEXPR_DENSE:
    assert(dim<expr->size);
    coeff_set(coeff,&expr->p.coeff[dim]);
    break;
  case LINEXPR_SPARSE:
    index = index_of_or_after_dim(dim,expr->p.linterm,expr->size);
    if (index<expr->size && dim==expr->p.linterm[index].dim){
      coeff_set(coeff, &expr->p.linterm[index].coeff);
    }
    else {
      coeff_set_int(coeff, 0);
    }
  }   
}

/* ====================================================================== */
/* I.4 Permutation */
/* ====================================================================== */

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
linexpr0_add_permute_dimensions(const linexpr0_t* expr,
                                size_t dimsup,
                                dim_t* perm)
{
  if (expr==NULL) return NULL;
  linexpr0_t* nexpr = linexpr0_copy(expr);
  switch(nexpr->discr){
  case LINEXPR_SPARSE:
    linexpr0_add_permute_dimensions_with(nexpr,dimsup,perm);
    break;
  case LINEXPR_DENSE:
    {
      size_t i;
      linexpr0_realloc(nexpr,
		       nexpr->size+dimsup,
 		       nexpr->size==0 ? 
		       COEFF_DOUBLE :
		       nexpr->p.coeff[0].discr);
      for (i=0; i<expr->size; i++){
	coeff_set(&nexpr->p.coeff[perm[i]],
		  &expr->p.coeff[i]);
      }
      for (i=expr->size; i<nexpr->size; i++){
	coeff_set_int(&nexpr->p.coeff[perm[i]],0);
      }
    }
    break;
  default:
    abort();
  }
  return nexpr;
}
void
linexpr0_add_permute_dimensions_with(linexpr0_t* expr,
                                     size_t dimsup,
                                     dim_t* perm)
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
      linexpr0_t* nexpr = linexpr0_add_permute_dimensions(expr,dimsup,perm);
      linexpr0_clear(expr);
      *expr = *nexpr;
    }
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* I.5 Comparison and hashing */
/* ====================================================================== */

int linexpr0_hash(const linexpr0_t* expr)
{
  if (expr->size==0){
    return cst_hash(&expr->cst);
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
int linexpr0_compare(const linexpr0_t* expr1,
                     const linexpr0_t* expr2)
{
  int res;
  size_t i1,i2;
  dim_t dim1,dim2;
  const coeff_t* pcoeff1;
  const coeff_t* pcoeff2;
  i1 = i2 = 0;
  while (i1<expr1->size || i2<expr2->size){
    if (i1<expr1->size){
      if (expr1->discr==LINEXPR_DENSE){
        dim1 = i1; pcoeff1 = &expr1->p.coeff[i1];
      } else {
        dim1 = expr1->p.linterm[i1].dim; pcoeff1 = &expr1->p.linterm[i1].coeff;
      }
    }
    else {
      dim1 = DIM_MAX;
      pcoeff1 = NULL;
    }
    if (i2<expr2->size){
      if (expr2->discr==LINEXPR_DENSE){
        dim2 = i2; pcoeff2 = &expr2->p.coeff[i2];
      } else {
        dim2 = expr2->p.linterm[i2].dim; pcoeff2 = &expr2->p.linterm[i2].coeff;
      }
    }
    else {
      dim2 = DIM_MAX;
      pcoeff2 = NULL;
    }
    if (dim1==dim2){
      i1++; i2++;
      res = coeff_cmp(pcoeff1,pcoeff2);
    }
    else if (dim1<dim2){
      i1++;
      res = coeff_sgn(pcoeff1);
    }
    else { /* if (dim2<dim1) */
      i2++;
      res = coeff_sgn(pcoeff2);
    }
    if(res) return (res>0 ? 1 : (-1));
  }
  res = cst_cmp(&expr1->cst,&expr2->cst);
  res = res>0 ? 2 : (res<0 ? (-2) : 0);
  return res;
}

/* ********************************************************************** */
/* II. Linear constraints */
/* ********************************************************************** */

void lincons0_fprint(FILE* stream, const lincons0_t* cons, const var_t* name_of_dim)
{
  linexpr0_fprint(stream,cons->linexpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == CONS_EQ ?
	  " = 0" :
	  ( cons->constyp == CONS_SUPEQ ?
	    " >= 0" :
	    " > 0" ));
}

lincons0_t lincons0_make_unsat()
{
  linexpr0_t* expr;
 
  expr = linexpr0_alloc(COEFF_DOUBLE,LINEXPR_DENSE,0,CST_COEFF);
  coeff_set_int(cst_coeffref(&expr->cst),-1);
  return lincons0_make(CONS_SUPEQ,expr);
}

bool lincons0_is_unsat(const lincons0_t* cons)
{
  linexpr0_t* expr = cons->linexpr0;
  if (expr->size==0){
    switch (expr->cst.discr){
    case CST_COEFF:
      return coeff_sgn(cst_coeffref(&expr->cst))<0;
    case CST_INTERVAL:
      return coeff_sgn(interval_supref(cst_intervalref(&expr->cst)))<0;
    default:
      return false;
    }
  }
  else
    return false;
}

/* ********************************************************************** */
/* III. Generators */
/* ********************************************************************** */

void generator0_fprint(FILE* stream, const generator0_t* gen, const var_t* name_of_dim)
{
  fprintf(stream,
	  gen->gentyp == GEN_LINE ?
	  "LINE:   " :
	  ( gen->gentyp == GEN_RAY ?
	    "RAY:    " :
	    "VERTEX: " ));
  linexpr0_fprint(stream,gen->linexpr0,name_of_dim);
}

/* ********************************************************************** */
/* IV. Array of linear constraints */
/* ********************************************************************** */

lincons0_array_t lincons0_array_make(size_t size)
{
  lincons0_array_t array;
  int i;
  array.size = size;
  array.p = (size==0) ? NULL : (lincons0_t*)malloc(size*sizeof(lincons0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void lincons0_array_clear(lincons0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      lincons0_clear(&array->p[i]);
    free(array->p);
    array->p=NULL;
  }
}

void lincons0_array_fprint(FILE* stream, 
                         const lincons0_array_t* array,
                         const var_t* name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of constraints\n");
  } else {
    fprintf(stream,"array of constraints of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      lincons0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}

/* ********************************************************************** */
/* V. Array of generators */
/* ********************************************************************** */

generator0_array_t generator0_array_make(size_t size)
{
  int i;
  generator0_array_t array;

  array.size = size;
  array.p = (size==0) ? NULL : (generator0_t*)malloc(size*sizeof(generator0_t));
  for (i=0; i<size; i++){
    array.p[i].linexpr0 = NULL;
  }
  return array;
}

void generator0_array_clear(generator0_array_t* array)
{
  int i;

  if (array->p!=NULL){
    for (i=0; i<array->size; i++)
      generator0_clear(&array->p[i]);
    free(array->p);
    array->p = NULL;
  }
  array->size=0;
}
void generator0_array_fprint(FILE* stream, 
                         const generator0_array_t* array,
                         const var_t* name_of_dim)
{
  int i;

  if (array->size==0){
    fprintf(stream,"empty array of generators\n");
  } else {
    fprintf(stream,"array of generator of size %d\n",array->size);
    for (i=0; i<array->size; i++){
      fprintf(stream,"%2d: ",i);
      generator0_fprint(stream,&array->p[i],name_of_dim);
      fprintf(stream,"\n");
    }
  }
}
