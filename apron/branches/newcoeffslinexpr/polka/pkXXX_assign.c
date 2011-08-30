/* ********************************************************************** */
/* pkXXX_assign.c: Assignements and Substitutions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "ap_linexprXXX.h"
#include "ap_generic.h"

#define _MARK_XXX_

/* ********************************************************************** */
/* I. Matrix operations */
/* ********************************************************************** */

/* ====================================================================== */
/* Matrix transformations: a variable and an expression */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Assignement of an expression to a variable */
/* ---------------------------------------------------------------------- */

/* Hypothesis:

   - either nmat is a matrix allocated with _matrixXXX_alloc_int,
   and his coefficients are not initialized,

   - or nmat==mat
*/
static
matrixXXX_t* matrixXXX_assign_variable(pkXXX_internal_t* pk,
				       bool destructive,
				       matrixXXX_t* mat,
				       ap_dim_t dim, numintXXX_t* tab)
{
  size_t i,j,var;
  bool den;
  matrixXXX_t* nmat;

  var = pk->dec + dim;
  den = numintXXX_cmp_int(tab[0],1)>0;

  nmat =
    destructive ?
    mat :
    _matrixXXX_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++){
    /* product for var column */
    vectorXXX_product(pk,pk->matrix_prod,
		      mat->p[i],
		      tab,mat->nbcolumns);
    /* columns != var */
    if (!destructive){
      /* Functional */
      numintXXX_init_set(nmat->p[i][0],mat->p[i][0]);
      for (j=1; j<mat->nbcolumns; j++){
	if (j!=var){
	  numintXXX_init_set(nmat->p[i][j],mat->p[i][j]);
	  if (den){
	    numintXXX_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  }
	}
      }
    }
    else {
      /* Side-effect */
      for (j=0; j<mat->nbcolumns; j++){
	if (j!=var){
	  if (den)
	    numintXXX_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  else
	    numintXXX_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
    /* var column */
    if (!destructive)
      numintXXX_init_set(nmat->p[i][var],pk->matrix_prod);
    else
      numintXXX_set(nmat->p[i][var],pk->matrix_prod);

    matrixXXX_normalize_row(pk,nmat,i);
  }
  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution of a variable by an expression */
/* ---------------------------------------------------------------------- */

/* Hypothesis:

   - either nmat is a matrix allocated with _matrixXXX_alloc_int,
   and his coefficients are not initialized,

   - or nmat==mat
*/
static
matrixXXX_t* matrixXXX_substitute_variable(pkXXX_internal_t* pk,
					   bool destructive,
					   matrixXXX_t* mat,
					   ap_dim_t dim, numintXXX_t* tab)
{
  size_t i,j,var;
  bool den;
  matrixXXX_t* nmat;

  var = pk->dec + dim;
  den = numintXXX_cmp_int(tab[0],1)>0;
  nmat =
    destructive ?
    mat :
    _matrixXXX_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++) {
    if (numintXXX_sgn(mat->p[i][var])) {
      /* The substitution must be done */
      if (!destructive){
	/* Functional */
	numintXXX_init_set(nmat->p[i][0],mat->p[i][0]);
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numintXXX_init(nmat->p[i][j]);
	      numintXXX_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	    }
	    else {
	      numintXXX_init_set(nmat->p[i][j],mat->p[i][j]);
	    }
	    numintXXX_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numintXXX_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numintXXX_init(nmat->p[i][var]);
	numintXXX_mul(nmat->p[i][var],mat->p[i][var],tab[var]);
      }
      else {
	/* Side-effect */
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numintXXX_mul(nmat->p[i][j],nmat->p[i][j],tab[0]);
	    }
	    numintXXX_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numintXXX_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numintXXX_mul(nmat->p[i][var],nmat->p[i][var],tab[var]);
      }
      matrixXXX_normalize_row(pk,nmat,i);
    }
    else {
      /* No substitution */
      if (!destructive){
	for (j=0; j<mat->nbcolumns; j++) {
	  numintXXX_init_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
  }
  return nmat;
}

/* ====================================================================== */
/* Matrix transformations: several variables and expressions */
/* ====================================================================== */

/* The list of pair (variable,expr) is given by an array of type
   equation_t.

   IMPRTANT: the array tdim should be sorted in ascending order.
*/

/* insertion sort for sorting the array tdim */
static
void pkXXX_asssub_isort(ap_dim_t* tdim, numintXXX_t** tvec, size_t size)
{
  size_t i,j;

  for (i=1; i<size; i++){
    ap_dim_t dim = tdim[i];
    numintXXX_t* vec = tvec[i];
    for (j=i; j>0; j--){
      if (tdim[j-1]>dim){
	tdim[j] = tdim[j-1];
	tvec[j] = tvec[j-1];
      }
      else
	break;
    }
    tdim[j]=dim;
    tvec[j]=vec;
  }
}



/* ---------------------------------------------------------------------- */
/* Assignement by an array of equations */
/* ---------------------------------------------------------------------- */
static
matrixXXX_t* matrixXXX_assign_variables(pkXXX_internal_t* pk,
					matrixXXX_t* mat,
					ap_dim_t* tdim,
					numintXXX_t** tvec,
					size_t size)
{
  size_t i,j,eindex;
  matrixXXX_t* nmat = _matrixXXX_alloc_int(mat->nbrows, mat->nbcolumns,false);
  numintXXX_t den;

  /* Computing common denominator */
  numintXXX_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numintXXX_mul(den,den,tvec[i][0]);
  }

  if (numintXXX_cmp_int(den,1)!=0){
    /* General case */
    numintXXX_t* vden = vectorXXX_alloc(size);
    for (i=0; i<size; i++){
      numintXXX_divexact(vden[i],den,tvec[i][0]);
    }
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numintXXX_init_set(nmat->p[i][0],mat->p[i][0]);
    }
    /* Other columns */
    eindex = 0;
    for (j=1; j<mat->nbcolumns; j++){
      if (eindex < size && pk->dec + tdim[eindex] == j){
	/* We are on an assigned column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  vectorXXX_product(pk,pk->matrix_prod,
			    mat->p[i],
			    tvec[eindex],mat->nbcolumns);
	  numintXXX_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	  /* Put the result */
	  numintXXX_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numintXXX_init(nmat->p[i][j]);
	  numintXXX_mul(nmat->p[i][j],mat->p[i][j],den);
	}
      }
    }
    vectorXXX_free(vden,size);
  }
  else {
    /* Special case: all denominators are 1 */
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numintXXX_init_set(nmat->p[i][0],mat->p[i][0]);
    }
    /* Other columns */
    eindex = 0;
    for (j=1; j<mat->nbcolumns; j++){
      if (eindex < size && pk->dec + tdim[eindex] == j){
	/* We are on a assigned column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  vectorXXX_product(pk,pk->matrix_prod,
			    mat->p[i],
			    tvec[eindex],mat->nbcolumns);
	  numintXXX_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numintXXX_init_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
  }
  numintXXX_clear(den);
  for (i=0; i<mat->nbrows; i++){
    matrixXXX_normalize_row(pk,nmat,i);
  }

  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution by an array of equations */
/* ---------------------------------------------------------------------- */

static
matrixXXX_t* matrixXXX_substitute_variables(pkXXX_internal_t* pk,
					    matrixXXX_t* mat,
					    ap_dim_t* tdim,
					    numintXXX_t** tvec,
					    size_t size)
{
  size_t i,j,eindex;
  matrixXXX_t* nmat = matrixXXX_alloc(mat->nbrows, mat->nbcolumns,false);
  numintXXX_t den;

  /* Computing common denominator */
  numintXXX_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numintXXX_mul(den,den,tvec[i][0]);
  }

  if (numintXXX_cmp_int(den,1)!=0){
    /* General case */
    numintXXX_t* vden = vectorXXX_alloc(size);
    for (i=0; i<size; i++){
      numintXXX_divexact(vden[i],den,tvec[i][0]);
    }
    /* For each row */
    for (i=0; i<mat->nbrows; i++) {
      /* Column 0 */
      numintXXX_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numintXXX_mul(nmat->p[i][j],mat->p[i][j],den);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numintXXX_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numintXXX_mul(pk->matrix_prod,
			  mat->p[i][pk->dec + tdim[eindex]],
			  tvec[eindex][j]);
	    numintXXX_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	    numintXXX_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
      }
    }
    vectorXXX_free(vden,size);
  }
  else {
    /* Special case: all denominators are 1 */
    /* For each row */
    for (i=0; i<mat->nbrows; i++) {
      /* Column 0 */
      numintXXX_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numintXXX_set(nmat->p[i][j],mat->p[i][j]);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numintXXX_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numintXXX_mul(pk->matrix_prod,
			  mat->p[i][pk->dec + tdim[eindex]],
			  tvec[eindex][j]);
	    numintXXX_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
      }
    }
  }
  numintXXX_clear(den);
  for (i=0; i<mat->nbrows; i++){
    matrixXXX_normalize_row(pk,nmat,i);
  }

  return nmat;
}

/* ********************************************************************** */
/* II. Auxiliary functions */
/* ********************************************************************** */

/* ====================================================================== */
/* Inversion of a (deterministic) linear expression */
/* ====================================================================== */
static
void vectorXXX_invert_expr(pkXXX_internal_t* pk,
			   numintXXX_t* ntab,
			   ap_dim_t dim,
			   numintXXX_t* tab,
			   size_t size)
{
  size_t i;
  size_t var = pk->dec+dim;
  int sgn = numintXXX_sgn(tab[var]);

  assert(sgn!=0);
  if (sgn>0){
    numintXXX_set(ntab[0], tab[var]);
    numintXXX_set(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numintXXX_neg(ntab[i],tab[i]);
    }
  } else {
    numintXXX_neg(ntab[0], tab[var]);
    numintXXX_neg(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numintXXX_set(ntab[i],tab[i]);
    }
  }
  vectorXXX_normalize_expr(pk,ntab,size);
  return;
}

/* ********************************************************************** */
/* III. Assignement/Substitution of a single dimension */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by a *deterministic* linear expression */
/* ====================================================================== */

pkXXX_t* pkXXX_asssub_linexprMPQ_det(bool assign,
				    ap_manager_t* man, bool destructive,
				    pkXXX_t* pa, ap_dim_t dim, ap_linexprMPQ_t linexpr)
{
  int sgn;
  pkXXX_t* po;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  po = destructive ? pa : pkXXX_alloc(pa->dim);

  if (!assign) pkXXX_dual(pa);

  /* Convert linear expression */
  vectorXXX_set_linexprMPQ(pk, pk->numintXXXp, linexpr, pa->dim, 1);
  sgn = numintXXX_sgn(pk->numintXXXp[pk->dec + dim]);

  if (!sgn){ /* Expression is not invertible */
    /* Get the needed matrix */
    pkXXX_obtain_F_dual(man,pa,"of the argument",assign);
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      pkXXX_set_top(pk,po);
      man->result.flag_best = man->result.flag_exact = false;
      goto  _pkXXX_asssub_linear_linexpr_exit;
    }
    if (destructive){
      /* If side-effect, free everything but generators */
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
      if (po->C){ matrixXXX_free(po->C); po->C = NULL; }
    }
  }
  if (pa->F){
    /* Perform assignements on generators */
    po->F =
      assign ?
      matrixXXX_assign_variable(pk, destructive, pa->F, dim, pk->numintXXXp) :
      matrixXXX_substitute_variable(pk, destructive, pa->F, dim, pk->numintXXXp);
  }
  if (sgn && pa->C){ /* Expression is invertible and we have constraints */
    /* Invert the expression in pk->numintXXXp2 */
    vectorXXX_invert_expr(
	pk, pk->numintXXXp2, dim, pk->numintXXXp, pa->C->nbcolumns);
    /* Perform susbtitution on constraints */
    po->C =
      assign ?
      matrixXXX_substitute_variable(pk,destructive,pa->C, dim, pk->numintXXXp2) :
      matrixXXX_assign_variable(pk,destructive,pa->C, dim, pk->numintXXXp2);
  }
  if (po->C && po->F){
    po->nbeq = pa->nbeq;
    po->nbline = pa->nbline;
    po->satC = (destructive || pa->satC==NULL) ? pa->satC : satmat_copy(pa->satC);
    po->satF = (destructive || pa->satF==NULL) ? pa->satF : satmat_copy(pa->satF);
  } else {
    po->nbeq = 0;
    po->nbline = 0;
  }
  po->status = 0;
 _pkXXX_asssub_linear_linexpr_exit:
  if (!assign){
    pkXXX_dual(pa);
    if (!destructive) pkXXX_dual(po);
  }
  return po;
}

/* ********************************************************************** */
/* IV. Assignement/Substitution of several dimensions */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by several *deterministic* linear expressions */
/* ====================================================================== */

pkXXX_t* pkXXX_asssub_linexprMPQ_array_det(
    bool assign, ap_manager_t* man, bool destructive,
    pkXXX_t* pa, ap_dim_t* tdim, ap_linexprMPQ_array_t array)
{
  size_t i;
  ap_dim_t* tdim2;
  numintXXX_t** tvec;
  size_t nbcols;
  matrixXXX_t* mat;
  pkXXX_t* po;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  po = destructive ? pa : pkXXX_alloc(pa->dim);
  if (!assign) pkXXX_dual(pa);

  /* Obtain the needed matrix */
  pkXXX_obtain_F_dual(man,pa,"of the argument",assign);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    pkXXX_set_top(pk,po);
    goto _pkXXX_asssub_linexprXXX_array_det_exit;
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    pkXXX_set_bottom(pk,po);
    return po;
  }
  /* Convert linear expressions */
  nbcols = pk->dec + pa->dim.intd + pa->dim.reald;
  tvec = (numintXXX_t**)malloc(array->size*sizeof(numintXXX_t*));
  for (i=0; i<array->size; i++){
    tvec[i] = vectorXXX_alloc(nbcols);
    vectorXXX_set_linexprMPQ(pk, tvec[i], array->p[i], pa->dim,1);
  }
  /* Copy tdim because of sorting */
  tdim2 = (ap_dim_t*)malloc(array->size*sizeof(ap_dim_t));
  memcpy(tdim2,tdim,array->size*sizeof(ap_dim_t));
  pkXXX_asssub_isort(tdim2,tvec,array->size);
  /* Perform the operation */
  mat =
    assign ?
    matrixXXX_assign_variables(pk, pa->F, tdim2, tvec, array->size) :
    matrixXXX_substitute_variables(pk, pa->F, tdim2, tvec, array->size);
  /* Free allocated stuff */
  for (i=0; i<array->size; i++){
    vectorXXX_free(tvec[i],nbcols);
  }
  free(tvec);
  free(tdim2);

  /* Update polyhedra */
  if (destructive){
    pkXXX_clear(po);
  }
  po->F = mat;
  po->status = 0;
 _pkXXX_asssub_linexprXXX_array_det_exit:
  if (!assign){
    pkXXX_dual(pa);
    if (!destructive) pkXXX_dual(po);
  }
  return po;
}

/* ********************************************************************** */
/* V. Assignement/Substitution: interface */
/* ********************************************************************** */

pkXXX_t* pkXXX_asssub_linexpr_array_linear(
    bool assign,
    ap_manager_t* man,
    bool destructive, pkXXX_t* pa,
    ap_dim_t* tdim, ap_linexpr0_array_t array,
    pkXXX_t* pb)
{
  pkXXX_t* po;
  bool exact;
  ap_linexprMPQ_array_t arrayMPQ;
  ap_linexprMPQ_array_ptr arrayMPQptr;

  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pkXXX_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald+1);
  ap_funid_t funid =
    assign ? AP_FUNID_ASSIGN_LINEXPR_ARRAY : AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY;

  /* Minimize the argument if option say so */
  if (!pk->option.op_lazy){
    pkXXX_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (destructive){
	pkXXX_set_top(pk,pa);
	return pa;
      } else {
	return pkXXX_top(man,pa->dim);
      }
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pkXXX_bottom(man,pa->dim);
  }

#ifdef _MARK_MPQ_
  if (array->discr==AP_SCALAR_MPQ){
    exact = true;
    arrayMPQptr = array->linexpr_array.MPQ;
  }
  else
#endif
    {
      ap_linexprMPQ_array_init(arrayMPQ,0);
      exact = ap_linexprMPQ_array_set_linexpr0_array(arrayMPQ,array,pk->num);
      arrayMPQptr = arrayMPQ;
    }
  assert(exact);
  if (arrayMPQptr->size==1){
    po = pkXXX_asssub_linexprMPQ_det(
	assign,man,destructive,pa,tdim[0],arrayMPQptr->p[0]);
  }
  else {
    po = pkXXX_asssub_linexprMPQ_array_det(
	assign,man,destructive,pa,tdim,arrayMPQptr);
  }
#ifdef _MARK_MPQ_
  if (array->discr==AP_SCALAR_MPQ){}
  else
#endif
    ap_linexprMPQ_array_clear(arrayMPQ);
  if (pb){
    pkXXX_meetjoin(true,man,po,po,pb);
  }
  man->result.flag_best = true;
  man->result.flag_exact = exact;
  /* Minimize the result if option say so */
  if (!pk->option.op_lazy){
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (pb) pkXXX_set(po,pb); else pkXXX_set_top(pk,po);
      return po;
    }
  }
  assert(pkXXX_check(pk,po));
  return po;
}

pkXXX_t* pkXXX_asssub_linexpr_array(
    bool assign,
    ap_manager_t* man,
    bool destructive, pkXXX_t* pa,
    ap_dim_t* tdim, ap_linexpr0_array_t array,
    pkXXX_t* pb)
{
  pkXXX_t* po;

  if (ap_linexpr0_array_is_linear(array)){
    po = pkXXX_asssub_linexpr_array_linear(assign,man,destructive,pa,tdim,array,pb);
  } else {
    po = ap_generic_asssub_linexpr_array(assign,AP_SCALAR_MPQ,man,destructive,pa,tdim,array,pb);
  }
  return po;
}

pkXXX_t* pkXXX_assign_linexpr_array(ap_manager_t* man,
				    bool destructive, pkXXX_t* pa,
				    ap_dim_t* tdim, ap_linexpr0_array_t array,
				    pkXXX_t* pb)
{
  return pkXXX_asssub_linexpr_array(true,man,destructive,
				    pa,tdim,array,pb);
}

pkXXX_t* pkXXX_substitute_linexpr_array(ap_manager_t* man,
					bool destructive, pkXXX_t* pa,
					ap_dim_t* tdim, ap_linexpr0_array_t array,
					pkXXX_t* pb)
{
  return pkXXX_asssub_linexpr_array(false,man,destructive,
				    pa,tdim,array,pb);
}

pkXXX_t* pkXXX_asssub_texpr_array(bool assign,
				  ap_manager_t* man,
				  bool destructive, pkXXX_t* pa,
				  ap_dim_t* tdim, ap_texpr0_array_t* array,
				  pkXXX_t* pb)
{
  return (pkXXX_t*)ap_generic_asssub_texpr_array(assign, man, destructive,
						 pa,tdim,array,pb);
}

pkXXX_t* pkXXX_assign_texpr_array(ap_manager_t* man,
			    bool destructive, pkXXX_t* a,
			    ap_dim_t* tdim, ap_texpr0_array_t* array,
			    pkXXX_t* dest)
{
  return pkXXX_asssub_texpr_array(true,man,destructive,a,tdim,array,dest);
}
pkXXX_t* pkXXX_substitute_texpr_array(ap_manager_t* man,
				bool destructive, pkXXX_t* a,
				ap_dim_t* tdim, ap_texpr0_array_t* texpr,
				pkXXX_t* dest)
{
  return pkXXX_asssub_texpr_array(false,man,destructive,a,tdim,texpr ,dest);
}
