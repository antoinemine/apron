/* ********************************************************************** */
/* pk_user.c: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pk_internal.h"
#include "ap_linexprMPQ.h"

/*
static void ap_coeff_set_scalar_numint(ap_coeff_t* coeff, numintMPQ_t num){
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPQ);
  mpq_set_numint(coeff->val.scalar->val.mpq,num);
}
*/
/* ********************************************************************** */
/* From ITV to PK */
/* ********************************************************************** */

/* Fills the vector with the constraint:
   dim <= num if sgn>0,
   dim = num if sgn=0
   dim >= -num if sgn<0 (an inferior bound is implicitly negated, as in itv).

   Returns false if equality of an integer dimension with a non-integer numbers
*/

bool vector_set_dim_bound(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_dim_t dim, numMPQ_t numrat, int mode,
    ap_dimension_t dimen, bool integer)
{
  numMPQ_t bound;
  size_t size;

  size = pk->dec+dimen.intd+dimen.reald;

  numMPQ_init(bound);
  if (integer && dim<dimen.intd){
    if (mode>0){
      numintMPQ_fdiv_q(numMPQ_numref(bound),
		       numMPQ_numref(numrat),numMPQ_denref(numrat));
      numintMPQ_set_int(numMPQ_denref(bound),1);
    }
    else if (mode<0){
      numintMPQ_cdiv_q(numMPQ_numref(bound),
		    numMPQ_numref(numrat),numMPQ_denref(numrat));
      numintMPQ_set_int(numMPQ_denref(bound),1);
    }
    else {
      if (numintMPQ_cmp_int(numMPQ_denref(numrat),1)!=0){
	numMPQ_clear(bound);
	return false;
      }
    }
  } else {
    numMPQ_set(bound,numrat);
  }
  /* Write the constraint num + den*x >= 0 */
  vector_clear(vec,size);
  numintMPQ_set_int(vec[0], (mode ? 1 : 0));
  numintMPQ_set(vec[polka_cst],numMPQ_numref(bound));
  numintMPQ_set(vec[pk->dec+dim],numMPQ_denref(bound));
  numMPQ_clear(bound);
  /* put the right sign now */
  if (mode>=0){
    numintMPQ_neg(vec[pk->dec+dim],vec[pk->dec+dim]);
  } 
  return true;
}


/* Fills the vector with the constraint:
   expr <= bound if sgn>0,
   expr = bound if sgn=0
   expr >= -bound if sgn<0 
   (an inferior bound is implicitly negated, as in itv).

   Returns false if equality with a non-integer numbers (like 2x=1).
*/

bool vector_set_linexpr_bound(
    pk_internal_t* pk, numintMPQ_t* vec,
    numintMPQ_t* vec2, numMPQ_t numrat, int mode,
    ap_dimension_t dim, bool integer)
{
  size_t i;
  size_t size;
  numintMPQ_t cst;

  assert (numintMPQ_sgn(numMPQ_denref(numrat)));

  size = pk->dec+dim.intd+dim.reald;

  if (vec!=vec2){
    vector_copy(vec,vec2,size);
  }

  if (numintMPQ_cmp_int(numMPQ_denref(numrat),1) != 0){
    for (i=1; i<size; i++){
      numintMPQ_mul(vec[i],vec[i],numMPQ_denref(numrat));
    }
  }
  numintMPQ_init(cst);
  numintMPQ_mul(cst,numMPQ_numref(numrat),vec[0]);
  if (mode>=0){
    numintMPQ_sub(vec[polka_cst],vec[polka_cst],cst);
    for (i=pk->dec; i<size; i++){
      numintMPQ_neg(vec[i],vec[i]);
    }
  }
  else {
    numintMPQ_add(vec[polka_cst],vec[polka_cst],cst);
  }
  numintMPQ_set_int(vec[0], mode ? 1 : 0);
  vector_normalize(pk,vec,size);
  if (integer) vector_normalize_constraint_int(pk,vec,dim);
  
  return true;
}

/* Fills the vector with the quasi-linear expression (ap_linexprMPQ) */
void vector_set_ap_linexprMPQ(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_linexprMPQ_t expr, ap_dimension_t dim, int mode)
{
  size_t i;
  ap_dim_t d;
  eitvMPQ_ptr eitv;

  /* compute lcm of denominators, in vec[0] */
  if (mode>=0){
    assert(!boundMPQ_infty(expr->cst->itv->sup));
    if (boundMPQ_sgn(expr->cst->itv->sup))
      numintMPQ_set(vec[0],
		    numMPQ_denref(boundMPQ_numref(expr->cst->itv->sup)));
    else
      numintMPQ_set_int(vec[0],1);
  } else {
    assert(!boundMPQ_infty(expr->cst->itv->neginf));
    if (boundMPQ_sgn(expr->cst->itv->neginf))
      numintMPQ_set(vec[0],
		    numMPQ_denref(boundMPQ_numref(expr->cst->itv->neginf)));
    else
      numintMPQ_set_int(vec[0],1);
  }
  ap_linexprMPQ_ForeachLinterm0(expr,i,d,eitv){
    assert(eitv->eq);
    numintMPQ_lcm(vec[0],vec[0],numMPQ_denref(boundMPQ_numref(eitv->itv->sup)));
  }

  /* Fill the vector */
  if (pk->strict) numintMPQ_set_int(vec[polka_eps],0);
  /* constant coefficient */
  if (mode>=0){
    numintMPQ_divexact(vec[polka_cst],
		       vec[0],
		       numMPQ_denref(boundMPQ_numref(expr->cst->itv->sup)));
    numintMPQ_mul(vec[polka_cst],
		  vec[polka_cst],
		  numMPQ_numref(boundMPQ_numref(expr->cst->itv->sup)));
  } else {
    numintMPQ_divexact(vec[polka_cst],
		       vec[0],
		       numMPQ_denref(boundMPQ_numref(expr->cst->itv->neginf)));
    numintMPQ_mul(vec[polka_cst],
		  vec[polka_cst],
		  numMPQ_numref(boundMPQ_numref(expr->cst->itv->neginf)));
    numintMPQ_neg(vec[polka_cst],vec[polka_cst]);
  }
  /* Other coefficients */
  for (i=pk->dec;i<pk->dec+dim.intd+dim.reald; i++){ 
    numintMPQ_set_int(vec[i],0);
  }
  ap_linexprMPQ_ForeachLinterm0(expr,i,d,eitv){
    size_t index = pk->dec + d;
    numintMPQ_divexact(vec[index],
		       vec[0],numMPQ_denref(boundMPQ_numref(eitv->itv->sup)));
    numintMPQ_mul(vec[index],
		  vec[index],numMPQ_numref(boundMPQ_numref(eitv->itv->sup)));
  }
  return;
}

/* Fills the vector(s) with the fully linear constraint cons */
void vector_set_ap_linconsMPQ(pk_internal_t* pk,
			    numintMPQ_t* vec,
			    ap_linconsMPQ_t cons,
			    ap_dimension_t dim,
			    bool integer)
{
  size_t i,nb;
  assert(cons->constyp == AP_CONS_EQ || 
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);
  assert(ap_linexprMPQ_is_scalar(&cons->linexpr));

  vector_set_ap_linexprMPQ(pk, vec, cons->linexpr, dim, 1);
  vector_normalize(pk,vec,pk->dec+dim.intd+dim.reald);
  if (cons->constyp == AP_CONS_EQ){
    numintMPQ_set_int(vec[0],0);
  }
  else {
    numintMPQ_set_int(vec[0],1);
  }
  if (cons->constyp == AP_CONS_SUP){
    if (pk->strict){
      numintMPQ_set_int(vec[polka_eps],-1);
    }
    else if (integer && vector_is_integer(pk, vec, dim)){
      numintMPQ_sub_uint(vec[polka_cst], vec[polka_cst], 1);
    }
  }
  if (integer)
    vector_normalize_constraint_int(pk,vec,dim);
}

/* Fills the vector(s) with the fully linear constraint cons for testing
   satisfiability.  

   Returns false if unsatisfiable
 */
bool vector_set_ap_linconsMPQ_sat(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_linconsMPQ_t cons, ap_dimension_t dim, bool integer)
{
  bool sat;
  size_t i;

  if (cons->constyp == AP_CONS_EQ && cons->linexpr->cst->eq != true){
    return false;
  }

  assert(cons->constyp == AP_CONS_EQ || 
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);
  
  if (!boundMPQ_infty(cons->linexpr->cst->itv->neginf)){
    vector_set_ap_linexprMPQ(pk, vec, cons->linexpr, dim,-1);
    vector_normalize(pk,vec,pk->dec+dim.intd+dim.reald);
    if (cons->constyp == AP_CONS_EQ && cons->linexpr->cst->eq){
      numintMPQ_set_int(vec[0],0);
    }
    else {
      numintMPQ_set_int(vec[0],1);
    }
    if (cons->constyp == AP_CONS_SUP){
      if (pk->strict){
	numintMPQ_set_int(vec[polka_eps],-1);
      }
      else if (integer && vector_is_integer(pk, vec, dim)){
	numintMPQ_sub_uint(vec[polka_cst], vec[polka_cst], 1);
      }
    }
    if (integer)
      vector_normalize_constraint_int(pk,vec,dim);
    return true;
  }
  else {
    return false;
  }
 }

/* ********************************************************************** */
/* From APRON to PK */
/* ********************************************************************** */

/* Fills the vector with the generator gen. */
bool vector_set_ap_lingen0(
    pk_internal_t* pk,
    numintMPQ_t* vec, ap_lingen0_t lingen0, ap_dimension_t dim)
{
  bool res;
  ap_linexpr0_t linexpr0;
  ap_gentyp_t gentyp;

  gentyp = ap_lingen0_get_gentyp(lingen0);
  assert(gentyp == AP_GEN_LINE || 
	 gentyp == AP_GEN_LINEMOD ||
	 gentyp == AP_GEN_RAY || 
	 gentyp == AP_GEN_RAYMOD);
  
  ap_lingen0_linexpr0ref(linexpr0,lingen0);
  res = ap_linexprMPQ_set_linexpr0(pk->poly_linexprMPQ, linexpr0, pk->num);
  vector_set_ap_linexprMPQ(pk, vec, pk->poly_linexprMPQ, dim, +1);

  numintMPQ_set_int(vec[polka_cst],0);
  if (pk->strict)
    numintMPQ_set_int(vec[polka_eps],0);
  
  numintMPQ_set_int(vec[0], 
		    ( ((gentyp == AP_GEN_LINE) || 
		       (gentyp == AP_GEN_LINEMOD)) ?
		      0 : 
		      1 ));
  vector_normalize(pk,vec, pk->dec + dim.intd + dim.reald);
  return res && (
      (gentyp==AP_GEN_LINE) || 
      (gentyp==AP_GEN_RAY));
}

static
bool matrix_append_ap_lingen0_array(
    pk_internal_t* pk,
    matrix_t* mat,ap_lingen0_array_t array,ap_dimension_t dim)
{
  bool res,exact;
  size_t i;
  ap_lingen0_t lingen0;

  const size_t size = ap_lingen0_array_size(array);
  const size_t nbrows = mat->nbrows;
  matrix_resize_rows_lazy(mat,nbrows+size);
  res = true;  
  for (i=0; i<size; i++){
    ap_lingen0_array_ref_index(lingen0,array,i);
    exact = vector_set_ap_lingen0(pk,mat->p[nbrows+i],lingen0,dim);
    res = res && exact;
  }
  return res;
}
bool matrix_set_ap_lingen0_array(pk_internal_t* pk,
				 matrix_t** matrix,
				 ap_lingen0_array_t array,
				 ap_dimension_t dim)
{
  const size_t size = ap_lingen0_array_size(array);  

  *matrix = matrix_alloc(size,pk->dec+dim.intd+dim.reald,false);
  (*matrix)->nbrows = 0;
  return matrix_append_ap_lingen0_array(pk,*matrix,array,dim);
}
/*
static
bool matrix_append_ap_lincons0_array(pk_internal_t* pk,
				     matrix_t* mat,
				     size_t** tabindex, size_t* size,
				     ap_lincons0_array_t* array,
				     ap_dimension_t dim,
				     bool integer)
{
  bool exact,res;
  size_t nbrows,i,j,k;
  size_t* tab;

  nbrows = mat->nbrows;
  matrix_resize_rows_lazy(mat,nbrows+2*array->size);

  res = true;
  tab = NULL;
  j = nbrows;
  k = 0;
  for (i=0; i<array->size; i++){
    if (ap_lincons0_is_unsat(&array->p[i])){
      if (tab){ 
	free(tab); tab=NULL; 
	k=0;
      }
      vector_clear(mat->p[0],mat->nbcolumns);
      numintMPQ_set_int(mat->p[0][0],1);
      numintMPQ_set_int(mat->p[0][polka_cst],-1);
      j = 1;
      res = true;
      break;
    }
    switch (array->p[i].constyp){
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      if (ap_linexpr0_is_quasilinear(array->p[i].linexpr0)){
	exact = ap_linconsMPQ_set_ap_lincons0(pk->num,
					    &pk->poly_ap_linconsMPQ,
					    &array->p[i]);
	res = res && exact;
	j += vector_set_ap_linconsMPQ(pk,
				    &mat->p[j], &pk->poly_ap_linconsMPQ,
				    dim,integer);
      }
      else {
	if (tab==NULL){
	  tab = (size_t*)malloc(array->size*sizeof(size_t));
	}
	tab[k] = i;
	k++;
      }
      break;
    default:
      res = false;
      break;
    }
  }
  mat->nbrows = j;
  if (tab){
    tab = (size_t*)realloc(tab,k*sizeof(size_t));
  }
  *tabindex = tab;
  *size = k;
  return res;
}

bool matrix_set_ap_lincons0_array(pk_internal_t* pk,
				  matrix_t** mat,
				  size_t** tabindex, size_t* size,
				  ap_lincons0_array_t* array,
				  ap_dimension_t dim,
				  bool integer)
{
  *mat = matrix_alloc(2*array->size,pk->dec+dim.intd+dim.reald,false);
  (*mat)->nbrows = 0;
  return matrix_append_ap_lincons0_array(pk,
					 *mat,
					 tabindex,size,
					 array,
					 dim,integer);
}
static
bool matrix_append_ap_intlincons0_array(pk_internal_t* pk,
					matrix_t* mat,
					itv_t* titv,
					ap_lincons0_array_t* array,
					size_t* tab, size_t size,
					ap_dimension_t dim,
					bool integer)
{
  bool exact;
  size_t nbrows,i,j;

  nbrows = mat->nbrows;
  matrix_resize_rows_lazy(mat,nbrows+2*array->size);
  exact = true;
  j = nbrows;
  for (i=0; i<size; i++){
    size_t index = tab[i];
    
    exact = ap_linconsMPQ_set_ap_lincons0(pk->num,
					&pk->poly_ap_linconsMPQ,
					&array->p[index])
      && exact;
    exact = itv_quasilinearize_lincons(pk->num,&pk->poly_ap_linconsMPQ,titv,true)
      && exact;
    j += vector_set_ap_linconsMPQ(pk,&mat->p[j],&pk->poly_ap_linconsMPQ,
				dim,integer);
  }
  mat->nbrows = j;
  return exact;
}
				 
bool matrix_set_ap_intlincons0_array(pk_internal_t* pk,
				     matrix_t** mat,	
				     itv_t* titv,
				     ap_lincons0_array_t* array,
				     size_t* tab, size_t size,
				     ap_dimension_t dim,
				     bool integer)
{
  *mat = matrix_alloc(2*array->size,pk->dec+dim.intd+dim.reald,false);
  (*mat)->nbrows = 0;
  return matrix_append_ap_intlincons0_array(pk,
					    *mat,
					    titv,
					    array,tab,size,
					    dim,integer);
}
*/
static
bool matrix_append_ap_linconsMPQ_array(pk_internal_t* pk,
				     matrix_t* mat,
				     ap_linconsMPQ_array_t array,
				     ap_dimension_t dim,
				     bool integer)
{
  bool exact,res;
  size_t i,j;
  size_t* tab;

  const size_t nbrows = mat->nbrows;
  matrix_resize_rows_lazy(mat,nbrows+array->size);

  res = true;
  j = nbrows;
  for (i=0; i<array->size; i++){
    assert(ap_linexprMPQ_is_scalar(&array->p[i].linexpr));
    switch (array->p[i]->constyp){
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      vector_set_ap_linconsMPQ(pk,mat->p[j], array->p[i],
			       dim,integer);
      j++;
      break;
    default:
      res = false;
      break;
    }
  }
  mat->nbrows = j;
  return res;
}
bool matrix_set_ap_linconsMPQ_array(pk_internal_t* pk,
				    matrix_t** mat,
				    ap_linconsMPQ_array_t array,
				    ap_dimension_t dim,
				    bool integer)
{
  *mat = matrix_alloc(array->size,pk->dec+dim.intd+dim.reald,false);
  (*mat)->nbrows = 0;
  return matrix_append_ap_linconsMPQ_array(pk,*mat,array,dim,integer);
}

/* ********************************************************************** */
/* From PK to APRON */
/* ********************************************************************** */

void linconsMPQ_set_vector(pk_internal_t* pk,
			   ap_linconsMPQ_t lincons, numintMPQ_t* q,size_t size)
{
      bool error;
  size_t i;
  ap_linexprMPQ_resize(lincons->linexpr,0);
  eitvMPQ_set_val(pk->num,lincons->linexpr->cst,EITV_MPZ,q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    if (numintMPQ_sgn(q[i])!=0){
      ap_dim_t dim = i - pk->dec;
      ap_linexprMPQ_set_list0(pk->num,lincons->linexpr,&error,
			      AP_COEFF_MPZ,q[i],dim,AP_END);
      assert(!error);
    }
  }
  if (numintMPQ_sgn(q[0])!=0){
    if (pk->strict && numintMPQ_sgn(q[polka_eps])<0)
      lincons->constyp = AP_CONS_SUP;
    else
      lincons->constyp = AP_CONS_SUPEQ;
  }
  else {
    lincons->constyp = AP_CONS_EQ;
  }
  mpq_set_si(lincons->mpq,0,1);
}
bool lincons0_set_vector(pk_internal_t* pk,
			ap_lincons0_t lincons0, numintMPQ_t* q,size_t size)
{
  linconsMPQ_set_vector(pk,pk->poly_linconsMPQ,q,size);
  return ap_lincons0_set_linconsMPQ(lincons0,pk->poly_linconsMPQ,pk->num);
}

void lingenMPQ_set_vector(pk_internal_t* pk,
			   ap_lingenMPQ_t lingen, numintMPQ_t* q,size_t size)
{
  bool error;
  size_t i;
  ap_linexprMPQ_resize(lingen->linexpr,0);
  eitvMPQ_set_int(lingen->linexpr->cst,0);
  bool cst = numintMPQ_sgn(q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    if (numintMPQ_sgn(q[i])!=0){
      ap_dim_t dim = i - pk->dec;
      if (cst!=0){
	ap_linexprMPQ_set_list0(pk->num,lingen->linexpr,&error,
				AP_COEFF_MPZ2,q[i],q[polka_cst],dim,
				AP_END);
      }
      else {
	ap_linexprMPQ_set_list0(pk->num,lingen->linexpr,&error,
				AP_COEFF_MPZ,q[i],dim,AP_END);
      }
    }
  }
  if (cst!=0){
    lingen->gentyp = AP_GEN_VERTEX;
  }
  else {
    if (numintMPQ_sgn(q[0])!=0)
      lingen->gentyp = AP_GEN_RAY;
    else
      lingen->gentyp = AP_GEN_LINE;
  }
}

bool vector_set_lingen0(pk_internal_t* pk,
			ap_lingen0_t lingen0, numintMPQ_t* q, size_t size)
{
  lingenMPQ_set_vector(pk,pk->poly_lingenMPQ,q,size);
  return ap_lingen0_set_lingenMPQ(lingen0,pk->poly_lingenMPQ,pk->num);
}

