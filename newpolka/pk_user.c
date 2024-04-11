/* ********************************************************************** */
/* pk_user.c: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_matrix.h"
#include "pk_internal.h"
#include "pk_user.h"
#include "itv_linearize.h"

static void ap_coeff_set_scalar_numint(ap_coeff_t* coeff, numint_t num){
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPQ);
  mpq_set_numint(coeff->val.scalar->val.mpq,num);
}

/* ********************************************************************** */
/* From ITV to PK */
/* ********************************************************************** */

/* Fills the vector with the constraint:
   dim <= num if sgn>0,
   dim = num if sgn=0
   dim >= -num if sgn<0 (an inferior bound is implicitly negated, as in itv).

   Returns false if equality of an integer dimension with a non-integer numbers
*/

bool vector_set_dim_bound(pk_internal_t* pk,
			  numint_t* vec,
			  ap_dim_t dim,
			  numrat_t numrat,
			  int mode,
			  size_t intdim, size_t realdim,
			  bool integer)
{
  numrat_t bound;
  size_t size;

  size = pk->dec+intdim+realdim;

  numrat_init(bound);
  if (integer && dim<intdim){
    if (mode>0){
      numint_fdiv_q(numrat_numref(bound),
		    numrat_numref(numrat),numrat_denref(numrat));
      numint_set_int(numrat_denref(bound),1);
    }
    else if (mode<0){
      numint_cdiv_q(numrat_numref(bound),
		    numrat_numref(numrat),numrat_denref(numrat));
      numint_set_int(numrat_denref(bound),1);
    }
    else {
      if (numint_cmp_int(numrat_denref(numrat),1)!=0){
	numrat_clear(bound);
	return false;
      }
      numrat_set(bound,numrat);
    }
  } else {
    numrat_set(bound,numrat);
  }
  /* Write the constraint num + den*x >= 0 */
  vector_clear(vec,size);
  numint_set_int(vec[0], (mode ? 1 : 0));
  numint_set(vec[polka_cst],numrat_numref(bound));
  numint_set(vec[pk->dec+dim],numrat_denref(bound));
  numrat_clear(bound);
  /* put the right sign now */
  if (mode>=0){
    numint_neg(vec[pk->dec+dim],vec[pk->dec+dim]);
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

bool vector_set_linexpr_bound(pk_internal_t* pk,
			      numint_t* vec,
			      numint_t* vec2,
			      numrat_t numrat,
			      int mode,
			      size_t intdim, size_t realdim,
			      bool integer)
{
  size_t i;
  size_t size;
  numint_t cst;

  assert (numint_sgn(numrat_denref(numrat)));

  size = pk->dec+intdim+realdim;

  if (vec!=vec2){
    vector_copy(vec,vec2,size);
  }

  if (numint_cmp_int(numrat_denref(numrat),1) != 0){
    for (i=1; i<size; i++){
      numint_mul(vec[i],vec[i],numrat_denref(numrat));
    }
  }
  numint_init(cst);
  numint_mul(cst,numrat_numref(numrat),vec[0]);
  if (mode>=0){
    numint_sub(vec[polka_cst],vec[polka_cst],cst);
    for (i=pk->dec; i<size; i++){
      numint_neg(vec[i],vec[i]);
    }
  }
  else {
    numint_add(vec[polka_cst],vec[polka_cst],cst);
  }
  numint_set_int(vec[0], mode ? 1 : 0);
  vector_normalize(pk,vec,size);
  if (integer) vector_normalize_constraint_int(pk,vec,intdim,realdim);

  return true;
}

/* Fills the vector with the quasi-linear expression (itv_linexpr) */
void vector_set_itv_linexpr(pk_internal_t* pk,
			    numint_t* vec,
			    itv_linexpr_t* expr,
			    size_t dim,
			    int mode)
{
  size_t i,d;
  bool* peq;
  itv_ptr pitv;

  /* compute lcm of denominators, in vec[0] */
  if (mode>=0){
    assert(!bound_infty(expr->cst->sup));
    if (bound_sgn(expr->cst->sup))
      numint_set(vec[0],
		 numrat_denref(bound_numref(expr->cst->sup)));
    else
      numint_set_int(vec[0],1);
  } else {
    assert(!bound_infty(expr->cst->inf));
    if (bound_sgn(expr->cst->inf))
      numint_set(vec[0],
		 numrat_denref(bound_numref(expr->cst->inf)));
    else
      numint_set_int(vec[0],1);
  }
  itv_linexpr_ForeachLinterm(expr,i,d,pitv,peq){
    assert(*peq);
    numint_lcm(vec[0],vec[0],numrat_denref(bound_numref(pitv->sup)));
  }

  /* Fill the vector */
  if (pk->strict) numint_set_int(vec[polka_eps],0);
  /* constant coefficient */
  if (mode>=0){
    numint_divexact(vec[polka_cst],
		    vec[0],
		    numrat_denref(bound_numref(expr->cst->sup)));
    numint_mul(vec[polka_cst],
	       vec[polka_cst],
	       numrat_numref(bound_numref(expr->cst->sup)));
  } else {
    numint_divexact(vec[polka_cst],
		    vec[0],
		    numrat_denref(bound_numref(expr->cst->inf)));
    numint_mul(vec[polka_cst],
	       vec[polka_cst],
	       numrat_numref(bound_numref(expr->cst->inf)));
    numint_neg(vec[polka_cst],vec[polka_cst]);
  }
  /* Other coefficients */
  for (i=pk->dec;i<pk->dec+dim; i++){
    numint_set_int(vec[i],0);
  }
  itv_linexpr_ForeachLinterm(expr,i,d,pitv,peq){
     size_t index = pk->dec + d;
     numint_divexact(vec[index],vec[0],numrat_denref(bound_numref(pitv->sup)));
     numint_mul(vec[index],vec[index],numrat_numref(bound_numref(pitv->sup)));
  }
  return;
}

/* Fills the vector(s) with the fully linear constraint cons */
void vector_set_itv_lincons(pk_internal_t* pk,
			    numint_t* vec,
			    itv_lincons_t* cons,
			    size_t intdim, size_t realdim,
			    bool integer)
{
  size_t i,nb;
  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);
  assert(itv_linexpr_is_scalar(&cons->linexpr));

  vector_set_itv_linexpr(pk, vec, &cons->linexpr, intdim+realdim,1);
  vector_normalize(pk,vec,pk->dec+intdim+realdim);
  if (cons->constyp == AP_CONS_EQ){
    numint_set_int(vec[0],0);
  }
  else {
    numint_set_int(vec[0],1);
  }
  if (cons->constyp == AP_CONS_SUP){
    if (pk->strict){
      numint_set_int(vec[polka_eps],-1);
    }
    else if (integer && vector_is_integer(pk, vec, intdim, realdim)){
      numint_sub_uint(vec[polka_cst], vec[polka_cst], 1);
    }
  }
  if (integer)
    vector_normalize_constraint_int(pk,vec,intdim,realdim);
}

/* Fills the vector(s) with the fully linear constraint cons for testing
   satisfiability.

   Returns false if unsatisfiable
 */
bool vector_set_itv_lincons_sat(pk_internal_t* pk,
				numint_t* vec,
				itv_lincons_t* cons,
				size_t intdim, size_t realdim,
				bool integer)
{
  bool sat;
  size_t i;

  if (cons->constyp == AP_CONS_EQ && cons->linexpr.equality != true){
    return false;
  }

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (!bound_infty(cons->linexpr.cst->inf)){
    vector_set_itv_linexpr(pk, vec, &cons->linexpr, intdim+realdim,-1);
    vector_normalize(pk,vec,pk->dec+intdim+realdim);
    if (cons->constyp == AP_CONS_EQ && cons->linexpr.equality){
      numint_set_int(vec[0],0);
    }
    else {
      numint_set_int(vec[0],1);
    }
    if (cons->constyp == AP_CONS_SUP){
      if (pk->strict){
	numint_set_int(vec[polka_eps],-1);
      }
      else if (integer && vector_is_integer(pk, vec, intdim, realdim)){
	numint_sub_uint(vec[polka_cst], vec[polka_cst], 1);
      }
    }
    if (integer)
      vector_normalize_constraint_int(pk,vec,intdim,realdim);
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
bool vector_set_ap_generator0(pk_internal_t* pk,
			      numint_t* vec,
			      ap_generator0_t* gen,
			      size_t intdim, size_t realdim)
{
  assert(gen->gentyp != AP_GEN_VERTEX);
  itv_linexpr_set_ap_linexpr0(pk->itv,
			      &pk->poly_itv_linexpr,
			      gen->linexpr0);
  vector_set_itv_linexpr(pk, vec, &pk->poly_itv_linexpr, intdim+realdim, +1);

  if (pk->strict)
    numint_set_int(vec[polka_eps],0);

  numint_set_int(vec[0],
		 ( ((gen->gentyp == AP_GEN_LINE) || (gen->gentyp == AP_GEN_LINEMOD)) ?
		   0 :
		   1 ));
  if (gen->gentyp != AP_GEN_VERTEX){
    numint_set_int(vec[polka_cst],0);
    vector_normalize(pk,vec, pk->dec + intdim + realdim);
  }
  switch (gen->gentyp){
  case AP_GEN_VERTEX:
  case AP_GEN_RAY:
  case AP_GEN_LINE:
    return true;
  default:
    return false;
  }
}

static
bool pk_matrix_append_ap_generator0_array(pk_internal_t* pk,
				       pk_matrix_t* mat,
				       ap_generator0_array_t* array,
				       size_t intdim, size_t realdim)
{
  bool res,exact;
  size_t i,nbrows;

  nbrows = mat->nbrows;
  pk_matrix_resize_rows_lazy(mat,nbrows+array->size);
  res = true;
  for (i=0; i<array->size; i++){
    exact = vector_set_ap_generator0(pk,mat->p[nbrows+i],&array->p[i],intdim,realdim);
    res = res && exact;
  }
  return res;
}
bool pk_matrix_set_ap_generator0_array(pk_internal_t* pk,
				    pk_matrix_t** matrix,
				    ap_generator0_array_t* array,
				    size_t intdim, size_t realdim)
{
  *matrix = pk_matrix_alloc(array->size,pk->dec+intdim+realdim,false);
  (*matrix)->nbrows = 0;
  return pk_matrix_append_ap_generator0_array(pk,*matrix,array,intdim,realdim);
}
/*
static
bool pk_matrix_append_ap_lincons0_array(pk_internal_t* pk,
				     pk_matrix_t* mat,
				     size_t** tabindex, size_t* size,
				     ap_lincons0_array_t* array,
				     size_t intdim, size_t realdim,
				     bool integer)
{
  bool exact,res;
  size_t nbrows,i,j,k;
  size_t* tab;

  nbrows = mat->nbrows;
  pk_matrix_resize_rows_lazy(mat,nbrows+2*array->size);

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
      numint_set_int(mat->p[0][0],1);
      numint_set_int(mat->p[0][polka_cst],-1);
      j = 1;
      res = true;
      break;
    }
    switch (array->p[i].constyp){
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      if (ap_linexpr0_is_quasilinear(array->p[i].linexpr0)){
	exact = itv_lincons_set_ap_lincons0(pk->itv,
					    &pk->poly_itv_lincons,
					    &array->p[i]);
	res = res && exact;
	j += vector_set_itv_lincons(pk,
				    &mat->p[j], &pk->poly_itv_lincons,
				    intdim,realdim,integer);
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

bool pk_matrix_set_ap_lincons0_array(pk_internal_t* pk,
				  pk_matrix_t** mat,
				  size_t** tabindex, size_t* size,
				  ap_lincons0_array_t* array,
				  size_t intdim, size_t realdim,
				  bool integer)
{
  *mat = pk_matrix_alloc(2*array->size,pk->dec+intdim+realdim,false);
  (*mat)->nbrows = 0;
  return pk_matrix_append_ap_lincons0_array(pk,
					 *mat,
					 tabindex,size,
					 array,
					 intdim,realdim,integer);
}
static
bool pk_matrix_append_ap_intlincons0_array(pk_internal_t* pk,
					pk_matrix_t* mat,
					itv_t* titv,
					ap_lincons0_array_t* array,
					size_t* tab, size_t size,
					size_t intdim, size_t realdim,
					bool integer)
{
  bool exact;
  size_t nbrows,i,j;

  nbrows = mat->nbrows;
  pk_matrix_resize_rows_lazy(mat,nbrows+2*array->size);
  exact = true;
  j = nbrows;
  for (i=0; i<size; i++){
    size_t index = tab[i];

    exact = itv_lincons_set_ap_lincons0(pk->itv,
					&pk->poly_itv_lincons,
					&array->p[index])
      && exact;
    exact = itv_quasilinearize_lincons(pk->itv,&pk->poly_itv_lincons,titv,true)
      && exact;
    j += vector_set_itv_lincons(pk,&mat->p[j],&pk->poly_itv_lincons,
				intdim,realdim,integer);
  }
  mat->nbrows = j;
  return exact;
}

bool pk_matrix_set_ap_intlincons0_array(pk_internal_t* pk,
				     pk_matrix_t** mat,
				     itv_t* titv,
				     ap_lincons0_array_t* array,
				     size_t* tab, size_t size,
				     size_t intdim, size_t realdim,
				     bool integer)
{
  *mat = pk_matrix_alloc(2*array->size,pk->dec+intdim+realdim,false);
  (*mat)->nbrows = 0;
  return pk_matrix_append_ap_intlincons0_array(pk,
					    *mat,
					    titv,
					    array,tab,size,
					    intdim,realdim,integer);
}
*/
static
bool pk_matrix_append_itv_lincons_array(pk_internal_t* pk,
				     pk_matrix_t* mat,
				     itv_lincons_array_t* array,
				     size_t intdim, size_t realdim,
				     bool integer)
{
  bool exact,res;
  size_t nbrows,i,j;
  size_t* tab;

  nbrows = mat->nbrows;
  pk_matrix_resize_rows_lazy(mat,nbrows+array->size);

  res = true;
  j = nbrows;
  for (i=0; i<array->size; i++){
    if (bound_infty(array->p[i].linexpr.cst->sup)) continue;
    assert(itv_linexpr_is_scalar(&array->p[i].linexpr));
    switch (array->p[i].constyp){
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      vector_set_itv_lincons(pk,
			     mat->p[j], &array->p[i],
			     intdim,realdim,integer);
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
bool pk_matrix_set_itv_lincons_array(pk_internal_t* pk,
				  pk_matrix_t** mat,
				  itv_lincons_array_t* array,
				  size_t intdim, size_t realdim,
				  bool integer)
{


  *mat = pk_matrix_alloc(array->size,pk->dec+intdim+realdim,false);
  (*mat)->nbrows = 0;
  return pk_matrix_append_itv_lincons_array(pk,
					 *mat,array,
					 intdim,realdim,integer);
}

/* ********************************************************************** */
/* From PK to APRON */
/* ********************************************************************** */

ap_lincons0_t lincons0_of_vector(pk_internal_t* pk,
				 numint_t* q,
				 size_t size)
{
  ap_lincons0_t lincons;
  ap_linexpr0_t* linexpr;
  size_t i;

  linexpr = ap_linexpr0_alloc(AP_LINEXPR_DENSE, size - pk->dec);

  ap_coeff_set_scalar_numint(&linexpr->cst, q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    ap_dim_t dim = i - pk->dec;
    ap_coeff_set_scalar_numint(&linexpr->p.coeff[dim], q[i]);
  }
  if (numint_sgn(q[0])){
    if (pk->strict && numint_sgn(q[polka_eps])<0)
      lincons.constyp = AP_CONS_SUP;
    else
      lincons.constyp = AP_CONS_SUPEQ;
  }
  else {
    lincons.constyp = AP_CONS_EQ;
  }
  lincons.linexpr0 = linexpr;
  lincons.scalar = NULL;
  return lincons;
}


ap_generator0_t generator0_of_vector(pk_internal_t* pk,
				     numint_t* q,
				     size_t size)
{
  ap_generator0_t generator;
  ap_linexpr0_t* linexpr;
  size_t i;
  int sgn;

  linexpr = ap_linexpr0_alloc(AP_LINEXPR_DENSE, size - pk->dec);
  ap_coeff_set_scalar_int(&linexpr->cst, 0);
  sgn = numint_sgn(q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    ap_dim_t dim = i - pk->dec;
    ap_coeff_set_scalar_numint(&linexpr->p.coeff[dim],q[i]);
    if (sgn){
      mpz_set_numint(mpq_denref(linexpr->p.coeff[dim].val.scalar->val.mpq),
		     q[polka_cst]);
      mpq_canonicalize(linexpr->p.coeff[dim].val.scalar->val.mpq);
    }
  }
  if (!numint_sgn(q[0])){
    generator.gentyp = AP_GEN_LINE;
  }
  else if (sgn==0){
    generator.gentyp = AP_GEN_RAY;
  }
  else {
    generator.gentyp = AP_GEN_VERTEX;
  }
  generator.linexpr0 = linexpr;
  return generator;
}
