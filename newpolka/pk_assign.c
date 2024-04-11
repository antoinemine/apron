/* ********************************************************************** */
/* pk_assign.c: Assignements and Substitutions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_representation.h"
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_extract.h"
#include "pk_resize.h"
#include "pk_meetjoin.h"
#include "pk_assign.h"
#include "ap_generic.h"
#include "itv_linearize.h"

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

  - either nmat is a matrix allocated with _pk_matrix_alloc_int,
    and his coefficients are not initialized,

  - or nmat==mat
*/
static
pk_matrix_t* pk_matrix_assign_variable(pk_internal_t* pk,
				 bool destructive,
				 pk_matrix_t* mat,
				 ap_dim_t dim, numint_t* tab)
{
  size_t i,j,var;
  bool den;
  pk_matrix_t* nmat;

  var = pk->dec + dim;
  den = numint_cmp_int(tab[0],1)>0;

  nmat =
    destructive ?
    mat :
    _pk_matrix_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++){
    /* product for var column */
    vector_product(pk,pk->matrix_prod,
		   mat->p[i],
		   tab,mat->nbcolumns);
    /* columns != var */
    if (!destructive){
      /* Functional */
      numint_init_set(nmat->p[i][0],mat->p[i][0]);
      for (j=1; j<mat->nbcolumns; j++){
	if (j!=var){
	  numint_init_set(nmat->p[i][j],mat->p[i][j]);
	  if (den){
	    numint_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  }
	}
      }
    }
    else {
      /* Side-effect */
      for (j=0; j<mat->nbcolumns; j++){
	if (j!=var){
	  if (den)
	    numint_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  else
	    numint_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
    /* var column */
    if (!destructive)
      numint_init_set(nmat->p[i][var],pk->matrix_prod);
    else
      numint_set(nmat->p[i][var],pk->matrix_prod);

    pk_matrix_normalize_row(pk,nmat,i);
  }
  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution of a variable by an expression */
/* ---------------------------------------------------------------------- */

/* Hypothesis:

  - either nmat is a matrix allocated with _pk_matrix_alloc_int,
    and his coefficients are not initialized,

  - or nmat==mat
*/
static
pk_matrix_t* pk_matrix_substitute_variable(pk_internal_t* pk,
				     bool destructive,
				     pk_matrix_t* mat,
				     ap_dim_t dim, numint_t* tab)
{
  size_t i,j,var;
  bool den;
  pk_matrix_t* nmat;

  var = pk->dec + dim;
  den = numint_cmp_int(tab[0],1)>0;
  nmat =
    destructive ?
    mat :
    _pk_matrix_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++) {
    if (numint_sgn(mat->p[i][var])) {
      /* The substitution must be done */
      if (!destructive){
	/* Functional */
	numint_init_set(nmat->p[i][0],mat->p[i][0]);
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numint_init(nmat->p[i][j]);
	      numint_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	    }
	    else {
	      numint_init_set(nmat->p[i][j],mat->p[i][j]);
	    }
	    numint_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numint_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numint_init(nmat->p[i][var]);
	numint_mul(nmat->p[i][var],mat->p[i][var],tab[var]);
      }
      else {
	/* Side-effect */
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numint_mul(nmat->p[i][j],nmat->p[i][j],tab[0]);
	    }
	    numint_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numint_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numint_mul(nmat->p[i][var],nmat->p[i][var],tab[var]);
      }
      pk_matrix_normalize_row(pk,nmat,i);
    }
    else {
      /* No substitution */
      if (!destructive){
	for (j=0; j<mat->nbcolumns; j++) {
	  numint_init_set(nmat->p[i][j],mat->p[i][j]);
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
void pk_asssub_isort(ap_dim_t* tdim, numint_t** tvec, size_t size)
{
  size_t i,j;

  for (i=1; i<size; i++){
    ap_dim_t dim = tdim[i];
    numint_t* vec = tvec[i];
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
pk_matrix_t* pk_matrix_assign_variables(pk_internal_t* pk,
				  pk_matrix_t* mat,
				  ap_dim_t* tdim,
				  numint_t** tvec,
				  size_t size)
{
  size_t i,j,eindex;
  pk_matrix_t* nmat = _pk_matrix_alloc_int(mat->nbrows, mat->nbcolumns,false);
  numint_t den;

  /* Computing common denominator */
  numint_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numint_mul(den,den,tvec[i][0]);
  }

  if (numint_cmp_int(den,1)!=0){
    /* General case */
    numint_t* vden = vector_alloc(size);
    for (i=0; i<size; i++){
      numint_divexact(vden[i],den,tvec[i][0]);
    }
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numint_init_set(nmat->p[i][0],mat->p[i][0]);
    }
    /* Other columns */
    eindex = 0;
    for (j=1; j<mat->nbcolumns; j++){
      if (eindex < size && pk->dec + tdim[eindex] == j){
	/* We are on an assigned column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  vector_product(pk,pk->matrix_prod,
			 mat->p[i],
			 tvec[eindex],mat->nbcolumns);
	  numint_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	  /* Put the result */
	  numint_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numint_init(nmat->p[i][j]);
	  numint_mul(nmat->p[i][j],mat->p[i][j],den);
	}
      }
    }
    vector_free(vden,size);
  }
  else {
    /* Special case: all denominators are 1 */
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numint_init_set(nmat->p[i][0],mat->p[i][0]);
    }
    /* Other columns */
    eindex = 0;
    for (j=1; j<mat->nbcolumns; j++){
      if (eindex < size && pk->dec + tdim[eindex] == j){
	/* We are on a assigned column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  vector_product(pk,pk->matrix_prod,
			 mat->p[i],
			 tvec[eindex],mat->nbcolumns);
	  numint_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numint_init_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
  }
  numint_clear(den);
  for (i=0; i<mat->nbrows; i++){
    pk_matrix_normalize_row(pk,nmat,i);
  }

  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution by an array of equations */
/* ---------------------------------------------------------------------- */

static
pk_matrix_t* pk_matrix_substitute_variables(pk_internal_t* pk,
				      pk_matrix_t* mat,
				      ap_dim_t* tdim,
				      numint_t** tvec,
				      size_t size)
{
  size_t i,j,eindex;
  pk_matrix_t* nmat = pk_matrix_alloc(mat->nbrows, mat->nbcolumns,false);
  numint_t den;

  /* Computing common denominator */
  numint_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numint_mul(den,den,tvec[i][0]);
  }

  if (numint_cmp_int(den,1)!=0){
    /* General case */
    numint_t* vden = vector_alloc(size);
    for (i=0; i<size; i++){
      numint_divexact(vden[i],den,tvec[i][0]);
    }
    /* For each row */
    for (i=0; i<mat->nbrows; i++) {
      /* Column 0 */
      numint_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numint_mul(nmat->p[i][j],mat->p[i][j],den);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numint_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numint_mul(pk->matrix_prod,
		       mat->p[i][pk->dec + tdim[eindex]],
		       tvec[eindex][j]);
	    numint_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	    numint_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
      }
    }
    vector_free(vden,size);
  }
  else {
    /* Special case: all denominators are 1 */
    /* For each row */
    for (i=0; i<mat->nbrows; i++) {
      /* Column 0 */
      numint_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numint_set(nmat->p[i][j],mat->p[i][j]);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numint_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numint_mul(pk->matrix_prod,
		       mat->p[i][pk->dec + tdim[eindex]],
		       tvec[eindex][j]);
	    numint_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
      }
    }
  }
  numint_clear(den);
  for (i=0; i<mat->nbrows; i++){
    pk_matrix_normalize_row(pk,nmat,i);
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
void vector_invert_expr(pk_internal_t* pk,
			numint_t* ntab,
			ap_dim_t dim,
			numint_t* tab,
			size_t size)
{
  size_t i;
  size_t var = pk->dec+dim;
  int sgn = numint_sgn(tab[var]);

  assert(sgn!=0);
  if (sgn>0){
    numint_set(ntab[0], tab[var]);
    numint_set(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numint_neg(ntab[i],tab[i]);
    }
  } else {
    numint_neg(ntab[0], tab[var]);
    numint_neg(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numint_set(ntab[i],tab[i]);
    }
  }
  vector_normalize_expr(pk,ntab,size);
  return;
}

/* ********************************************************************** */
/* III. Assignement/Substitution of several dimensions */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by several *deterministic* linear expressions */
/* ====================================================================== */

pk_t* poly_asssub_linexpr_array_det(bool assign,
				    ap_manager_t* man,
				    bool destructive,
				    pk_t* pa,
				    ap_dim_t* tdim, ap_linexpr0_t** texpr,
				    size_t size)
{
  size_t i;
  ap_dim_t* tdim2;
  numint_t** tvec;
  size_t nbcols;
  pk_matrix_t* mat;
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);

  if (!assign) poly_dual(pa);

  /* Obtain the needed matrix */
  poly_obtain_F_dual(man,pa,"of the argument",assign);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    poly_set_top(pk,po);
    goto _poly_asssub_linexpr_array_det_exit;
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    poly_set_bottom(pk,po);
    return po;
  }
  /* Convert linear expressions */
  nbcols = pk->dec + pa->intdim + pa->realdim;
  tvec = (numint_t**)malloc(size*sizeof(numint_t*));
  for (i=0; i<size; i++){
    tvec[i] = vector_alloc(nbcols);
    itv_linexpr_set_ap_linexpr0(pk->itv,
				&pk->poly_itv_linexpr,
				texpr[i]);
    vector_set_itv_linexpr(pk,
			   tvec[i],
			   &pk->poly_itv_linexpr,
			   pa->intdim+pa->realdim,1);
  }
  /* Copy tdim because of sorting */
  tdim2 = (ap_dim_t*)malloc(size*sizeof(ap_dim_t));
  memcpy(tdim2,tdim,size*sizeof(ap_dim_t));
  pk_asssub_isort(tdim2,tvec,size);
  /* Perform the operation */
  mat =
    assign ?
    pk_matrix_assign_variables(pk, pa->F, tdim2, tvec, size) :
    pk_matrix_substitute_variables(pk, pa->F, tdim2, tvec, size);
  /* Free allocated stuff */
  for (i=0; i<size; i++){
    vector_free(tvec[i],nbcols);
  }
  free(tvec);
  free(tdim2);

  /* Update polyhedra */
  if (destructive){
    poly_clear(po);
  }
  po->F = mat;
  po->status = 0;
 _poly_asssub_linexpr_array_det_exit:
  if (!assign){
    poly_dual(pa);
    if (!destructive) poly_dual(po);
  }
  assert(poly_check(pk,po));
  return po;
}

/* ====================================================================== */
/* Assignement/Substitution by an array of linear expressions */
/* ====================================================================== */
static
pk_t* poly_asssub_linexpr_array(bool assign,
				bool lazy,
				ap_manager_t* man,
				bool destructive,
				pk_t* pa,
				ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				pk_t* pb)
{
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  /* Minimize the argument if option say so */
  if (!lazy){
    poly_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (destructive){
	poly_set_top(pk,pa);
	return pa;
      } else {
	return pk_top(man,pa->intdim,pa->realdim);
      }
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->intdim,pa->realdim);
  }
  /* Choose the right technique */
  if (ap_linexpr0_array_is_linear(texpr,size)){
    po = poly_asssub_linexpr_array_det(assign,man,destructive,pa,tdim,texpr,size);
    if (pb){
      poly_meet(true,lazy,man,po,po,pb);
    }
  }
  else {
    po = ap_generic_asssub_linexpr_array(assign,man,destructive,pa,tdim,texpr,size,pb);
  }
  /* Minimize the result if option say so */
  if (!lazy){
    poly_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (pb) poly_set(po,pb); else poly_set_top(pk,po);
      return po;
    }
  }
  /* Is the result exact or best ? */
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    size_t i;
    man->result.flag_best = true;
    for (i=0;i<size;i++){
      if (tdim[i] < pa->intdim || !ap_linexpr0_is_real(texpr[i], pa->intdim)){
	man->result.flag_best = false;
	break;
      }
    }
    man->result.flag_exact = man->result.flag_best;
  }
  else {
    man->result.flag_best = man->result.flag_exact = (pa->intdim==0);
  }
  return po;
}

/* ====================================================================== */
/* Assignement/Substitution by a linear expression */
/* ====================================================================== */
static
pk_t* poly_asssub_linexpr(bool assign,
			  bool lazy,
			  ap_manager_t* man,
			  bool destructive,
			  pk_t* pa,
			  ap_dim_t dim, ap_linexpr0_t* linexpr,
			  pk_t* pb)
{
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  pk_internal_realloc_lazy(pk,pa->intdim+pa->realdim+1);

  /* Minimize the argument if option say so */
  if (!lazy){
    poly_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (destructive){
	poly_set_top(pk,pa);
	return pa;
      } else {
	return pk_top(man,pa->intdim,pa->realdim);
      }
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->intdim,pa->realdim);
  }
  /* Choose the right technique */
  if (ap_linexpr0_is_linear(linexpr)){
    po = poly_asssub_linexpr_det(assign,man,destructive,pa,dim,linexpr);
    if (pb){
      poly_meet(true,lazy,man,po,po,pb);
    }
  }
  else {
    po = ap_generic_asssub_linexpr_array(assign,man,destructive,pa,&dim,&linexpr,1,pb);
  }
  /* Minimize the result if option say so */
  if (!lazy){
    poly_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (pb) poly_set(po,pb); else poly_set_top(pk,po);
      return po;
    }
  }
  /* Is the result exact or best ? */
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    man->result.flag_best = man->result.flag_exact =
      (dim < pa->intdim || !ap_linexpr0_is_real(linexpr, pa->intdim)) ?
      false :
      true;
  }
  else {
    man->result.flag_best = man->result.flag_exact = (pa->intdim==0);
  }
  return po;
}

/* ====================================================================== */
/* Assignement/Substitution by an array of tree expressions */
/* ====================================================================== */
static
pk_t* poly_asssub_texpr_array(bool assign,
			      bool lazy,
			      ap_manager_t* man,
			      bool destructive,
			      pk_t* pa,
			      ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
			      pk_t* pb)
{
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  /* Minimize the argument if option say so */
  if (!lazy){
    poly_chernikova(man,pa,"of the argument");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (destructive){
	poly_set_top(pk,pa);
	return pa;
      } else {
	return pk_top(man,pa->intdim,pa->realdim);
      }
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->intdim,pa->realdim);
  }
  /* Choose the right technique */
  if (ap_texpr0_array_is_scalar(texpr,size) &&
      ap_texpr0_array_is_interval_linear(texpr,size)){
    ap_abstract0_t abs;
    abs.value = pa;
    abs.man = man;
    if (size==1){
      ap_linexpr0_t* linexpr0 =
	ap_intlinearize_texpr0(man,&abs,texpr[0],NULL,
			       AP_SCALAR_MPQ,false);
      po = poly_asssub_linexpr(assign,true,man,destructive,
                               pa, tdim[0], linexpr0,NULL);
      ap_linexpr0_free(linexpr0);
    }
    else {
      ap_linexpr0_t** tlinexpr =
	ap_intlinearize_texpr0_array(man,&abs,texpr,size,NULL,
				     AP_SCALAR_MPQ,false);
      po = poly_asssub_linexpr_array(assign,true,man,destructive,
                                     pa,tdim,tlinexpr,size,NULL);
      ap_linexpr0_array_free(tlinexpr,size);
    }
    if (pb){
      poly_meet(true,lazy,man,po,po,pb);
    }
  }
  else {
    po = ap_generic_asssub_texpr_array(assign,man,destructive,pa,tdim,texpr,size,pb);
  }
  /* Minimize the result if option say so */
  if (!lazy){
    poly_chernikova(man,po,"of the result");
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_best = man->result.flag_exact = false;
      if (pb) poly_set(po,pb); else poly_set_top(pk,po);
      return po;
    }
  }
  /* Is the result exact or best ? */
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    man->result.flag_best = ap_texpr0_array_is_interval_linear(texpr,size);
    man->result.flag_exact = man->result.flag_best;
  }
  else {
    man->result.flag_best = man->result.flag_exact = false;
  }
  return po;
}

/* ********************************************************************** */
/* IV. Assignement/Substitution of a single dimension */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by a *deterministic* linear expression */
/* ====================================================================== */

pk_t* poly_asssub_linexpr_det(bool assign,
			      ap_manager_t* man,
			      bool destructive,
			      pk_t* pa,
			      ap_dim_t dim, ap_linexpr0_t* linexpr0)
{
  int sgn;
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);

  if (!assign) poly_dual(pa);

  /* Convert linear expression */
  itv_linexpr_set_ap_linexpr0(pk->itv,
			      &pk->poly_itv_linexpr,
			      linexpr0);
  vector_set_itv_linexpr(pk,
			 pk->poly_numintp,
			 &pk->poly_itv_linexpr,
			 pa->intdim+pa->realdim,1);
  sgn = numint_sgn(pk->poly_numintp[pk->dec + dim]);

  if (!sgn){ /* Expression is not invertible */
    /* Get the needed matrix */
    poly_obtain_F_dual(man,pa,"of the argument",assign);
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      poly_set_top(pk,po);
      man->result.flag_best = man->result.flag_exact = false;
      goto  _poly_asssub_linear_linexpr_exit;
    }
    if (destructive){
      /* If side-effect, free everything but generators */
      if (po->satC){ satmat_free(po->satC); po->satC = NULL; }
      if (po->satF){ satmat_free(po->satF); po->satF = NULL; }
      if (po->C){ pk_matrix_free(po->C); po->C = NULL; }
    }
  }
  if (pa->F){
    /* Perform assignements on generators */
    po->F =
      assign ?
      pk_matrix_assign_variable(pk, destructive, pa->F, dim, pk->poly_numintp) :
      pk_matrix_substitute_variable(pk, destructive, pa->F, dim, pk->poly_numintp);
  }
  if (sgn && pa->C){ /* Expression is invertible and we have constraints */
    /* Invert the expression in pk->poly_numintp2 */
    vector_invert_expr(pk,
		       pk->poly_numintp2,
		       dim, pk->poly_numintp,
		       pa->C->nbcolumns);
    /* Perform susbtitution on constraints */
    po->C =
      assign ?
      pk_matrix_substitute_variable(pk,destructive,pa->C, dim, pk->poly_numintp2) :
      pk_matrix_assign_variable(pk,destructive,pa->C, dim, pk->poly_numintp2);
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
 _poly_asssub_linear_linexpr_exit:
  if (!assign){
    poly_dual(pa);
    if (!destructive) poly_dual(po);
  }
  assert(poly_check(pk,po));
  return po;
}

/* ********************************************************************** */
/* V. Assignement/Substitution: interface */
/* ********************************************************************** */

pk_t* pk_assign_linexpr_array(ap_manager_t* man,
			      bool destructive, pk_t* pa,
			      ap_dim_t* tdim, ap_linexpr0_t** texpr,
			      size_t size,
			      pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY);
  pk_t* po;
  po =
    size==1 ?
    poly_asssub_linexpr(true,
			pk->funopt->algorithm<=0,
			man,destructive,pa,tdim[0],texpr[0],pb)
    :
    poly_asssub_linexpr_array(true,
			      pk->funopt->algorithm<=0,
			      man,destructive,pa,tdim,texpr,size,pb);
  assert(poly_check(pk,po));
  return po;
}

pk_t* pk_substitute_linexpr_array(ap_manager_t* man,
				  bool destructive, pk_t* pa,
				  ap_dim_t* tdim, ap_linexpr0_t** texpr,
				  size_t size,
				  pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY);
  pk_t* po;
  po =
    size==1 ?
    poly_asssub_linexpr(false,
			pk->funopt->algorithm<=0,
			man,destructive,pa,tdim[0],texpr[0],pb)
    :
    poly_asssub_linexpr_array(false,
			      pk->funopt->algorithm<=0,
			      man,destructive,pa,tdim,texpr,size,pb);
  assert(poly_check(pk,po));
  return po;
}

pk_t* pk_assign_texpr_array(ap_manager_t* man,
			    bool destructive, pk_t* a,
			    ap_dim_t* tdim,
			    ap_texpr0_t** texpr,
			    size_t size,
			    pk_t* dest)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_ASSIGN_TEXPR_ARRAY);
  pk_t* po;
  po = poly_asssub_texpr_array(true,
			       pk->funopt->algorithm<=0,
			       man,destructive,a,tdim,texpr,size,dest);
  assert(poly_check(pk,po));
  return po;
}
pk_t* pk_substitute_texpr_array(ap_manager_t* man,
				bool destructive, pk_t* a,
				ap_dim_t* tdim,
				ap_texpr0_t** texpr,
				size_t size,
				pk_t* dest)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_SUBSTITUTE_TEXPR_ARRAY);
  pk_t* po;
  po = poly_asssub_texpr_array(false,
			       pk->funopt->algorithm<=0,
			       man,destructive,a,tdim,texpr,size,dest);
  assert(poly_check(pk,po));
  return po;
}
