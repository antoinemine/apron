/* ********************************************************************** */
/* pkXXX_user.c: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "ap_linexprXXX.h"
#include "num_conv.h"


/* Fills the vector with the constraint:
   dim <= numrat if sgn>0,
   dim = numrat if sgn=0
   dim >= -numrat if sgn<0 (an inferior bound is implicitly negated, as in itv).

   Returns false if equality of an integer dimension with a non-integer numbers
*/

bool vectorXXX_set_dim_bound(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_dim_t dim, numXXX_t numrat, int mode,
    ap_dimension_t dimen, bool integer)
{
  numXXX_t bound;
  size_t size;

  size = pk->dec+dimen.intd+dimen.reald;

  numXXX_init(bound);
  if (integer && dim<dimen.intd){
    if (mode>0){
      numintXXX_fdiv_q(numXXX_numref(bound),
		       numXXX_numref(numrat),numXXX_denref(numrat));
      numintXXX_set_int(numXXX_denref(bound),1);
    }
    else if (mode<0){
      numintXXX_cdiv_q(numXXX_numref(bound),
		       numXXX_numref(numrat),numXXX_denref(numrat));
      numintXXX_set_int(numXXX_denref(bound),1);
    }
    else {
      if (numintXXX_cmp_int(numXXX_denref(numrat),1)!=0){
	numXXX_clear(bound);
	return false;
      }
    }
  } else {
    numXXX_set(bound,numrat);
  }
  /* Write the constraint num + den*x >= 0 */
  vectorXXX_clear(vec,size);
  numintXXX_set_int(vec[0], (mode ? 1 : 0));
  numintXXX_set(vec[polka_cst],numXXX_numref(bound));
  numintXXX_set(vec[pk->dec+dim],numXXX_denref(bound));
  numXXX_clear(bound);
  /* put the right sign now */
  if (mode>=0){
    numintXXX_neg(vec[pk->dec+dim],vec[pk->dec+dim]);
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

bool vectorXXX_set_linexpr_bound(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    numintXXX_t* vec2, numXXX_t numrat, int mode,
    ap_dimension_t dim, bool integer)
{
  size_t i;
  size_t size;
  numintXXX_t cst;

  assert (numintXXX_sgn(numXXX_denref(numrat)));

  size = pk->dec+dim.intd+dim.reald;

  if (vec!=vec2){
    vectorXXX_copy(vec,vec2,size);
  }

  if (numintXXX_cmp_int(numXXX_denref(numrat),1) != 0){
    for (i=1; i<size; i++){
      numintXXX_mul(vec[i],vec[i],numXXX_denref(numrat));
    }
  }
  numintXXX_init(cst);
  numintXXX_mul(cst,numXXX_numref(numrat),vec[0]);
  if (mode>=0){
    numintXXX_sub(vec[polka_cst],vec[polka_cst],cst);
    for (i=pk->dec; i<size; i++){
      numintXXX_neg(vec[i],vec[i]);
    }
  }
  else {
    numintXXX_add(vec[polka_cst],vec[polka_cst],cst);
  }
  numintXXX_set_int(vec[0], mode ? 1 : 0);
  vectorXXX_normalize(pk,vec,size);
  if (integer) vectorXXX_normalize_constraint_int(pk,vec,dim);

  return true;
}

/* ********************************************************************** */
/* From XXX to PK */
/* ********************************************************************** */

/* Fills the vector with the quasi-linear expression (ap_linexprMPQ) */
#define _numXXX_
#if defined(_numMPQ_)
#define numintXXX_set_numintMPQ_special(a,b,c) (a)=(b)
#else
#define numintXXX_set_numintMPQ_special(a,b,c) numintXXX_set_numMPZ(a,b,c)
#endif

void vectorXXX_set_linexprMPQ(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_linexprMPQ_t expr, ap_dimension_t dim, int mode)
{
  size_t i;
  ap_dim_t d;
  eitvMPQ_ptr eitv;
  numintXXX_ptr tmp = pk->vector_tmp[0];

  /* compute lcm of denominators, in vec[0] */
  if (mode>=0){
    assert(!boundMPQ_infty(expr->cst->itv->sup));
    if (boundMPQ_sgn(expr->cst->itv->sup))
      numintXXX_set_numMPZ(vec[0],
			   numMPQ_denref(boundMPQ_numref(expr->cst->itv->sup)),
			   pk->num);
    else
      numintXXX_set_int(vec[0],1);
  } else {
    assert(!boundMPQ_infty(expr->cst->itv->neginf));
    if (boundMPQ_sgn(expr->cst->itv->neginf))
      numintXXX_set_numMPZ(vec[0],
			   numMPQ_denref(boundMPQ_numref(expr->cst->itv->neginf)),
			   pk->num);
    else
      numintXXX_set_int(vec[0],1);
  }
  ap_linexprXXX_ForeachLinterm0(expr,i,d,eitv){
    assert(eitv->eq);
    numintXXX_set_numintMPQ_special(tmp,numMPQ_denref(boundMPQ_numref(eitv->itv->sup)),pk->num);
    numintXXX_lcm(vec[0],vec[0],tmp);
  }
  /* Initialize the vector */
  if (pk->strict) numintXXX_set_int(vec[polka_eps],0);
  for (i=pk->dec;i<pk->dec+dim.intd+dim.reald; i++){
    numintXXX_set_int(vec[i],0);
  }
  /* Fill the vector */
  /* constant coefficient */
  /* if we have p/q, put compute (lcm/q)*p = p*lcm/q */
  if (mode>=0){
    numintXXX_set_numintMPQ_special(tmp,numMPQ_denref(boundMPQ_numref(expr->cst->itv->sup)),pk->num);
    numintXXX_divexact(vec[polka_cst], vec[0], tmp);

    numintXXX_set_numintMPQ_special(tmp,numMPQ_numref(boundMPQ_numref(expr->cst->itv->sup)),pk->num);
    numintXXX_mul(vec[polka_cst], vec[polka_cst], tmp);
  } else {
    numintXXX_set_numintMPQ_special(tmp,numMPQ_denref(boundMPQ_numref(expr->cst->itv->neginf)),pk->num);
    numintXXX_divexact(vec[polka_cst], vec[0], tmp);

    numintXXX_set_numintMPQ_special(tmp,numMPQ_numref(boundMPQ_numref(expr->cst->itv->neginf)),pk->num);
    numintXXX_mul(vec[polka_cst], vec[polka_cst], tmp);

    numintXXX_neg(vec[polka_cst],vec[polka_cst]);
  }
  /* Other coefficients */
  ap_linexprXXX_ForeachLinterm0(expr,i,d,eitv){
    size_t index = pk->dec + d;
    numintXXX_set_numintMPQ_special(tmp,numMPQ_denref(boundMPQ_numref(eitv->itv->sup)),pk->num);
    numintXXX_divexact(vec[index],vec[0],tmp);

    numintXXX_set_numintMPQ_special(tmp,numMPQ_numref(boundMPQ_numref(eitv->itv->sup)),pk->num);
    numintXXX_mul(vec[index],vec[index],tmp);
  }
  return;
}

/* Fills the vector(s) with the fully linear constraint cons */
void vectorXXX_set_linconsMPQ(pkXXX_internal_t* pk,
				 numintXXX_t* vec,
				 ap_linconsMPQ_t cons,
				 ap_dimension_t dim,
				 bool integer)
{
  size_t i,size;
  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);
  assert(ap_linexprMPQ_is_linear(cons->linexpr));

  size = pk->dec+ap_dimension_size(dim);
  vectorXXX_set_linexprMPQ(pk, vec, cons->linexpr, dim, 1);
  vectorXXX_normalize(pk,vec,size);
  if (cons->constyp == AP_CONS_EQ){
    numintXXX_set_int(vec[0],0);
  }
  else {
    numintXXX_set_int(vec[0],1);
  }
  if (cons->constyp == AP_CONS_SUP){
    if (pk->strict){
      numintXXX_set_int(vec[polka_eps],-1);
    }
    else if (integer &&
	     vectorXXX_is_integer(pk, vec, dim) &&
	     !vectorXXX_is_cst(pk,vec,size)){
      numintXXX_sub_uint(vec[polka_cst], vec[polka_cst], 1);
    }
  }
  if (integer)
    vectorXXX_normalize_constraint_int(pk,vec,dim);
}

/* Fills the vector(s) with the fully linear constraint cons for testing
   satisfiability.

   Returns false if unsatisfiable
*/
bool vectorXXX_set_linconsMPQ_sat(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_linconsMPQ_t cons, ap_dimension_t dim, bool integer)
{
  bool sat;
  size_t i,size;

  if (cons->constyp == AP_CONS_EQ && cons->linexpr->cst->eq != true){
    return false;
  }

  assert(cons->constyp == AP_CONS_EQ ||
	 cons->constyp == AP_CONS_SUPEQ ||
	 cons->constyp == AP_CONS_SUP);

  if (!boundMPQ_infty(cons->linexpr->cst->itv->neginf)){
    size = pk->dec+ap_dimension_size(dim);
    vectorXXX_set_linexprMPQ(pk, vec, cons->linexpr, dim,-1);
    vectorXXX_normalize(pk,vec,size);
    if (cons->constyp == AP_CONS_EQ && cons->linexpr->cst->eq){
      numintXXX_set_int(vec[0],0);
    }
    else {
      numintXXX_set_int(vec[0],1);
    }
    if (cons->constyp == AP_CONS_SUP){
	if (pk->strict){
	  numintXXX_set_int(vec[polka_eps],-1);
	}
	else if (integer &&
		 vectorXXX_is_integer(pk, vec, dim) &&
		 !vectorXXX_is_cst(pk,vec,size)){
	  numintXXX_sub_uint(vec[polka_cst], vec[polka_cst], 1);
	}
    }
    if (integer)
      vectorXXX_normalize_constraint_int(pk,vec,dim);
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
bool vectorXXX_set_ap_lingen0(
    pkXXX_internal_t* pk,
    numintXXX_t* vec, ap_lingen0_t lingen0, ap_dimension_t dim)
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
  res = ap_linexprMPQ_set_linexpr0(pk->ap_linexprMPQ, linexpr0, pk->num);
  vectorXXX_set_linexprMPQ(pk, vec, pk->ap_linexprMPQ, dim, +1);

  numintXXX_set_int(vec[polka_cst],0);
  if (pk->strict)
    numintXXX_set_int(vec[polka_eps],0);

  numintXXX_set_int(vec[0],
		    ( ((gentyp == AP_GEN_LINE) ||
		       (gentyp == AP_GEN_LINEMOD)) ?
		      0 :
		      1 ));
  vectorXXX_normalize(pk,vec, pk->dec + dim.intd + dim.reald);
  return res && (
      (gentyp==AP_GEN_LINE) ||
      (gentyp==AP_GEN_RAY));
}

static
bool matrixXXX_append_ap_lingen0_array(
    pkXXX_internal_t* pk,
    matrixXXX_t* mat,ap_lingen0_array_t array,ap_dimension_t dim)
{
  bool res,exact;
  size_t i;
  ap_lingen0_t lingen0;

  const size_t size = ap_lingen0_array_size(array);
  const size_t nbrows = mat->nbrows;
  matrixXXX_resize_rows_lazy(mat,nbrows+size);
  res = true;
  for (i=0; i<size; i++){
    ap_lingen0_array_ref_index(lingen0,array,i);
    exact = vectorXXX_set_ap_lingen0(pk,mat->p[nbrows+i],lingen0,dim);
    res = res && exact;
  }
  return res;
}
bool matrixXXX_set_ap_lingen0_array(pkXXX_internal_t* pk,
				    matrixXXX_t** matrix,
				    ap_lingen0_array_t array,
				    ap_dimension_t dim)
{
  const size_t size = ap_lingen0_array_size(array);

  *matrix = matrixXXX_alloc(size,pk->dec+dim.intd+dim.reald,false);
  (*matrix)->nbrows = 0;
  return matrixXXX_append_ap_lingen0_array(pk,*matrix,array,dim);
}

static
bool matrixXXX_append_linconsMPQ_array(pkXXX_internal_t* pk,
					  matrixXXX_t* mat,
					  ap_linconsMPQ_array_t array,
					  ap_dimension_t dim,
					  bool integer)
{
  bool exact,res;
  size_t i,j;
  size_t* tab;

  const size_t nbrows = mat->nbrows;
  matrixXXX_resize_rows_lazy(mat,nbrows+array->size);

  res = true;
  j = nbrows;
  for (i=0; i<array->size; i++){
    assert(ap_linexprMPQ_is_linear(array->p[i]->linexpr));
    switch (array->p[i]->constyp){
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      vectorXXX_set_linconsMPQ(pk,mat->p[j], array->p[i],
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
bool matrixXXX_set_linconsMPQ_array(pkXXX_internal_t* pk,
				       matrixXXX_t** mat,
				       ap_linconsMPQ_array_t array,
				       ap_dimension_t dim,
				       bool integer)
{
  *mat = matrixXXX_alloc(array->size,pk->dec+dim.intd+dim.reald,false);
  (*mat)->nbrows = 0;
  return matrixXXX_append_linconsMPQ_array(pk,*mat,array,dim,integer);
}

/* ********************************************************************** */
/* From PK to APRON */
/* ********************************************************************** */

void linconsMPQ_set_vectorXXX(pkXXX_internal_t* pk,
			      ap_linconsMPQ_t lincons, numintXXX_t* q,size_t size)
{
  bool error;
  size_t i;
  ap_linexprMPQ_resize(lincons->linexpr,0);
  eitvMPQ_set_val(pk->num,lincons->linexpr->cst,EITV_NUMINTXXX,q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    if (numintXXX_sgn(q[i])!=0){
      ap_dim_t dim = i - pk->dec;
      ap_linexprMPQ_set_list0(pk->num,lincons->linexpr,&error,
			      AP_COEFF_NUMINTXXX,q[i],dim,AP_END);
      assert(!error);
    }
  }
  if (numintXXX_sgn(q[0])!=0){
    if (pk->strict && numintXXX_sgn(q[polka_eps])<0)
      lincons->constyp = AP_CONS_SUP;
    else
      lincons->constyp = AP_CONS_SUPEQ;
  }
  else {
    lincons->constyp = AP_CONS_EQ;
  }
  mpq_set_si(lincons->mpq,0,1);
}
bool lincons0_set_vectorXXX(pkXXX_internal_t* pk,
			    ap_lincons0_t lincons0, numintXXX_t* q,size_t size)
{
  if (lincons0->discr==AP_SCALAR_MPQ){
    linconsMPQ_set_vectorXXX(pk,lincons0->lincons.MPQ,q,size);
    return true;
  }
  else {
    linconsMPQ_set_vectorXXX(pk,pk->ap_linconsMPQ,q,size);
    return ap_lincons0_set_linconsMPQ(lincons0,pk->ap_linconsMPQ,pk->num);
  }
}

void lingenXXX_set_vector(pkXXX_internal_t* pk,
			  ap_lingenXXX_t lingen, numintXXX_t* q,size_t size)
{
  bool error;
  size_t i;
  ap_linexprXXX_resize(lingen->linexpr,0);
  eitvXXX_set_int(lingen->linexpr->cst,0);
  bool cst = numintXXX_sgn(q[polka_cst]);
  for (i=pk->dec; i<size; i++){
    if (numintXXX_sgn(q[i])!=0){
      ap_dim_t dim = i - pk->dec;
      if (cst!=0){
	ap_linexprXXX_set_list0(pk->num,lingen->linexpr,&error,
				AP_COEFF_MPZ2,q[i],q[polka_cst],dim,
				AP_END);
      }
      else {
	ap_linexprXXX_set_list0(pk->num,lingen->linexpr,&error,
				AP_COEFF_MPZ,q[i],dim,AP_END);
      }
    }
  }
  if (cst!=0){
    lingen->gentyp = AP_GEN_VERTEX;
  }
  else {
    if (numintXXX_sgn(q[0])!=0)
      lingen->gentyp = AP_GEN_RAY;
    else
      lingen->gentyp = AP_GEN_LINE;
  }
}

bool lingen0_set_vectorXXX(pkXXX_internal_t* pk,
			   ap_lingen0_t lingen0, numintXXX_t* q, size_t size)
{
  lingenXXX_set_vector(pk,pk->ap_lingenXXX,q,size);
  return ap_lingen0_set_lingenXXX(lingen0,pk->ap_lingenXXX,pk->num);
}
