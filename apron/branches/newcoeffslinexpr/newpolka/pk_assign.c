/* ********************************************************************** */
/* pk_assign.c: Assignements and Substitutions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pk_internal.h"
#include "ap_generic.h"

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

  - either nmat is a matrix allocated with _matrix_alloc_int,
    and his coefficients are not initialized,

  - or nmat==mat
*/
static
matrix_t* matrix_assign_variable(pk_internal_t* pk,
				 bool destructive,
				 matrix_t* mat,
				 ap_dim_t dim, numintMPQ_t* tab)
{
  size_t i,j,var;
  bool den;
  matrix_t* nmat;

  var = pk->dec + dim;
  den = numintMPQ_cmp_int(tab[0],1)>0;

  nmat =
    destructive ?
    mat :
    _matrix_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++){
    /* product for var column */
    vector_product(pk,pk->matrix_prod,
		   mat->p[i],
		   tab,mat->nbcolumns);
    /* columns != var */
    if (!destructive){
      /* Functional */
      numintMPQ_init_set(nmat->p[i][0],mat->p[i][0]);
      for (j=1; j<mat->nbcolumns; j++){
	if (j!=var){
	  numintMPQ_init_set(nmat->p[i][j],mat->p[i][j]);
	  if (den){
	    numintMPQ_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  }
	}
      }
    }
    else {
      /* Side-effect */
      for (j=0; j<mat->nbcolumns; j++){
	if (j!=var){
	  if (den)
	    numintMPQ_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	  else
	    numintMPQ_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
    /* var column */
    if (!destructive)
      numintMPQ_init_set(nmat->p[i][var],pk->matrix_prod);
    else
      numintMPQ_set(nmat->p[i][var],pk->matrix_prod);

    matrix_normalize_row(pk,nmat,i);
  }
  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution of a variable by an expression */
/* ---------------------------------------------------------------------- */

/* Hypothesis:

  - either nmat is a matrix allocated with _matrix_alloc_int,
    and his coefficients are not initialized,

  - or nmat==mat
*/
static
matrix_t* matrix_substitute_variable(pk_internal_t* pk,
				     bool destructive,
				     matrix_t* mat,
				     ap_dim_t dim, numintMPQ_t* tab)
{
  size_t i,j,var;
  bool den;
  matrix_t* nmat;

  var = pk->dec + dim;
  den = numintMPQ_cmp_int(tab[0],1)>0;
  nmat =
    destructive ?
    mat :
    _matrix_alloc_int(mat->nbrows,mat->nbcolumns,false);

  nmat->_sorted = false;

  for (i=0; i<mat->nbrows; i++) {
    if (numintMPQ_sgn(mat->p[i][var])) {
      /* The substitution must be done */
      if (!destructive){
	/* Functional */
	numintMPQ_init_set(nmat->p[i][0],mat->p[i][0]);
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numintMPQ_init(nmat->p[i][j]);
	      numintMPQ_mul(nmat->p[i][j],mat->p[i][j],tab[0]);
	    }
	    else {
	      numintMPQ_init_set(nmat->p[i][j],mat->p[i][j]);
	    }
	    numintMPQ_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numintMPQ_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numintMPQ_init(nmat->p[i][var]);
	numintMPQ_mul(nmat->p[i][var],mat->p[i][var],tab[var]);
      }
      else {
	/* Side-effect */
	/* columns != var */
	for (j=1; j<mat->nbcolumns; j++) {
	  if (j!=var){
	    if (den){
	      numintMPQ_mul(nmat->p[i][j],nmat->p[i][j],tab[0]);
	    }
	    numintMPQ_mul(pk->matrix_prod,mat->p[i][var],tab[j]);
	    numintMPQ_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
	/* var column */
	numintMPQ_mul(nmat->p[i][var],nmat->p[i][var],tab[var]);
      }
      matrix_normalize_row(pk,nmat,i);
    }
    else {
      /* No substitution */
      if (!destructive){
	for (j=0; j<mat->nbcolumns; j++) {
	  numintMPQ_init_set(nmat->p[i][j],mat->p[i][j]);
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
void pk_asssub_isort(ap_dim_t* tdim, numintMPQ_t** tvec, size_t size)
{
  size_t i,j;

  for (i=1; i<size; i++){
    ap_dim_t dim = tdim[i];
    numintMPQ_t* vec = tvec[i];
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
matrix_t* matrix_assign_variables(pk_internal_t* pk,
				  matrix_t* mat,
				  ap_dim_t* tdim,
				  numintMPQ_t** tvec,
				  size_t size)
{
  size_t i,j,eindex;
  matrix_t* nmat = _matrix_alloc_int(mat->nbrows, mat->nbcolumns,false);
  numintMPQ_t den;

  /* Computing common denominator */
  numintMPQ_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numintMPQ_mul(den,den,tvec[i][0]);
  }

  if (numintMPQ_cmp_int(den,1)!=0){
    /* General case */
    numintMPQ_t* vden = vector_alloc(size);
    for (i=0; i<size; i++){
      numintMPQ_divexact(vden[i],den,tvec[i][0]);
    }
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numintMPQ_init_set(nmat->p[i][0],mat->p[i][0]);
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
	  numintMPQ_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	  /* Put the result */
	  numintMPQ_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numintMPQ_init(nmat->p[i][j]);
	  numintMPQ_mul(nmat->p[i][j],mat->p[i][j],den);
	}
      }
    }
    vector_free(vden,size);
  }
  else {
    /* Special case: all denominators are 1 */
    /* Column 0: copy */
    for (i=0; i<mat->nbrows; i++){
      numintMPQ_init_set(nmat->p[i][0],mat->p[i][0]);
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
	  numintMPQ_init_set(nmat->p[i][j],pk->matrix_prod);
	}
	eindex++;
      }
      else {
	/* We are on a normal column */
	for (i=0; i<mat->nbrows; i++){ /* For each row */
	  numintMPQ_init_set(nmat->p[i][j],mat->p[i][j]);
	}
      }
    }
  }
  numintMPQ_clear(den);
  for (i=0; i<mat->nbrows; i++){
    matrix_normalize_row(pk,nmat,i);
  }

  return nmat;
}

/* ---------------------------------------------------------------------- */
/* Substitution by an array of equations */
/* ---------------------------------------------------------------------- */

static
matrix_t* matrix_substitute_variables(pk_internal_t* pk,
				      matrix_t* mat,
				      ap_dim_t* tdim,
				      numintMPQ_t** tvec,
				      size_t size)
{
  size_t i,j,eindex;
  matrix_t* nmat = matrix_alloc(mat->nbrows, mat->nbcolumns,false);
  numintMPQ_t den;

  /* Computing common denominator */
  numintMPQ_init_set(den,tvec[0][0]);
  for (i=1; i<size; i++){
    numintMPQ_mul(den,den,tvec[i][0]);
  }

  if (numintMPQ_cmp_int(den,1)!=0){
    /* General case */
    numintMPQ_t* vden = vector_alloc(size);
    for (i=0; i<size; i++){
      numintMPQ_divexact(vden[i],den,tvec[i][0]);
    }
    /* For each row */
    for (i=0; i<mat->nbrows; i++) {
      /* Column 0 */
      numintMPQ_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numintMPQ_mul(nmat->p[i][j],mat->p[i][j],den);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numintMPQ_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numintMPQ_mul(pk->matrix_prod,
		       mat->p[i][pk->dec + tdim[eindex]],
		       tvec[eindex][j]);
	    numintMPQ_mul(pk->matrix_prod,pk->matrix_prod,vden[eindex]);
	    numintMPQ_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
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
      numintMPQ_set(nmat->p[i][0],mat->p[i][0]);
      /* Other columns */
      /* First, copy the row and sets to zero substituted variables */
      eindex = 0;
      for (j=1; j<mat->nbcolumns; j++){
	if (eindex < size && pk->dec + tdim[eindex] == j)
	  eindex++;
	else
	  numintMPQ_set(nmat->p[i][j],mat->p[i][j]);
      }
      /* Second, add things coming from substitution */
      for (j=1; j<mat->nbcolumns; j++){
	for (eindex=0; eindex<size; eindex++){
	  if (numintMPQ_sgn(mat->p[i][pk->dec + tdim[eindex]])) {
	    numintMPQ_mul(pk->matrix_prod,
		       mat->p[i][pk->dec + tdim[eindex]],
		       tvec[eindex][j]);
	    numintMPQ_add(nmat->p[i][j],nmat->p[i][j],pk->matrix_prod);
	  }
	}
      }
    }
  }
  numintMPQ_clear(den);
  for (i=0; i<mat->nbrows; i++){
    matrix_normalize_row(pk,nmat,i);
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
			numintMPQ_t* ntab,
			ap_dim_t dim,
			numintMPQ_t* tab,
			size_t size)
{
  size_t i;
  size_t var = pk->dec+dim;
  int sgn = numintMPQ_sgn(tab[var]);

  assert(sgn!=0);
  if (sgn>0){
    numintMPQ_set(ntab[0], tab[var]);
    numintMPQ_set(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numintMPQ_neg(ntab[i],tab[i]);
    }
  } else {
    numintMPQ_neg(ntab[0], tab[var]);
    numintMPQ_neg(ntab[var], tab[0]);
    for (i=1; i<size; i++){
      if (i!=var)
	numintMPQ_set(ntab[i],tab[i]);
    }
  }
  vector_normalize_expr(pk,ntab,size);
  return;
}

/* ********************************************************************** */
/* III. Assignement/Substitution of a single dimension */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by a *deterministic* linear expression */
/* ====================================================================== */

pk_t* poly_asssub_linexprMPQ_det(bool assign,
				 ap_manager_t* man, bool destructive,
				 pk_t* pa, ap_dim_t dim, ap_linexprMPQ_t linexpr)
{
  int sgn;
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  
  po = destructive ? pa : poly_alloc(pa->dim);

  if (!assign) poly_dual(pa);

  /* Convert linear expression */
  vector_set_ap_linexprMPQ(
      pk, pk->poly_numintp, linexpr, pa->dim, 1);
  sgn = numintMPQ_sgn(pk->poly_numintp[pk->dec + dim]);

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
      if (po->C){ matrix_free(po->C); po->C = NULL; }
    }
  }
  if (pa->F){
    /* Perform assignements on generators */
    po->F =
      assign ?
      matrix_assign_variable(pk, destructive, pa->F, dim, pk->poly_numintp) :
      matrix_substitute_variable(pk, destructive, pa->F, dim, pk->poly_numintp);
  }
  if (sgn && pa->C){ /* Expression is invertible and we have constraints */
    /* Invert the expression in pk->poly_numintp2 */
    vector_invert_expr(
	pk, pk->poly_numintp2, dim, pk->poly_numintp, pa->C->nbcolumns);
    /* Perform susbtitution on constraints */
    po->C =
      assign ?
      matrix_substitute_variable(pk,destructive,pa->C, dim, pk->poly_numintp2) :
      matrix_assign_variable(pk,destructive,pa->C, dim, pk->poly_numintp2);
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
  return po;
}

/* ********************************************************************** */
/* IV. Assignement/Substitution of several dimensions */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement/Substitution by several *deterministic* linear expressions */
/* ====================================================================== */

pk_t* poly_asssub_linexprMPQ_array_det(
    bool assign, ap_manager_t* man, bool destructive,
    pk_t* pa, ap_dim_t* tdim, ap_linexprMPQ_array_t array)
{
  size_t i;
  ap_dim_t* tdim2;
  numintMPQ_t** tvec;
  size_t nbcols;
  matrix_t* mat;
  pk_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  po = destructive ? pa : poly_alloc(pa->dim);

  if (!assign) poly_dual(pa);

  /* Obtain the needed matrix */
  poly_obtain_F_dual(man,pa,"of the argument",assign);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    poly_set_top(pk,po);
    goto _poly_asssub_linexprMPQ_array_det_exit;
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    poly_set_bottom(pk,po);
    return po;
  }
  /* Convert linear expressions */
  nbcols = pk->dec + pa->dim.intd + pa->dim.reald;
  tvec = (numintMPQ_t**)malloc(array->size*sizeof(numintMPQ_t*));
  for (i=0; i<array->size; i++){
    tvec[i] = vector_alloc(nbcols);
    vector_set_ap_linexprMPQ(pk, tvec[i], array->p[i], pa->dim,1);
  }
  /* Copy tdim because of sorting */
  tdim2 = (ap_dim_t*)malloc(array->size*sizeof(ap_dim_t));
  memcpy(tdim2,tdim,array->size*sizeof(ap_dim_t));
  pk_asssub_isort(tdim2,tvec,array->size);
  /* Perform the operation */
  mat =
    assign ?
    matrix_assign_variables(pk, pa->F, tdim2, tvec, array->size) :
    matrix_substitute_variables(pk, pa->F, tdim2, tvec, array->size);
  /* Free allocated stuff */
  for (i=0; i<array->size; i++){
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
 _poly_asssub_linexprMPQ_array_det_exit:
  if (!assign){
    poly_dual(pa);
    if (!destructive) poly_dual(po);
  }
  return po;
}

/* ********************************************************************** */
/* V. Assignement/Substitution: interface */
/* ********************************************************************** */

pk_t* pk_asssub_linexpr_array(bool assign,
			      ap_manager_t* man,
			      bool destructive, pk_t* pa,
			      ap_dim_t* tdim, ap_linexpr0_array_t array,
			      pk_t* pb)
{ 
  pk_t* po;
  bool exact;

  ap_funid_t funid = 
    assign ? AP_FUNID_ASSIGN_LINEXPR_ARRAY : AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY;
  bool lazy = man->option.funopt[funid].algorithm<=0;
  pk_internal_t* pk = pk_init_from_manager(man,funid);

  pk_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald+1);
  
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
	return pk_top(man,pa->dim);
      }
    }
  }
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->dim);
  }
  /* Choose the right technique */
  if (ap_linexpr0_array_is_linear(array)){
    ap_linexprMPQ_array_t arrayMPQ;
    ap_linexprMPQ_array_ptr arrayMPQptr;
    if (array->discr==AP_SCALAR_MPQ){
      exact = true;
      arrayMPQptr = array->linexpr_array.MPQ;
    }
    else {
      ap_linexprMPQ_array_init(arrayMPQ,0);
      exact = ap_linexprMPQ_array_set_linexpr0_array(arrayMPQ,array,pk->num);
      arrayMPQptr = arrayMPQ;
    }
    if (arrayMPQptr->size==1){
      po = poly_asssub_linexprMPQ_det(
	  assign,man,destructive,pa,tdim[0],arrayMPQptr->p[0]);
    }
    else {
      po = poly_asssub_linexprMPQ_array_det(
	  assign,man,destructive,pa,tdim,arrayMPQptr);
    }
    if (array->discr!=AP_SCALAR_MPQ){
      ap_linexprMPQ_array_clear(arrayMPQ);
    }
    if (pb){
      poly_meet(true,lazy,man,po,po,pb);
    }
    man->result.flag_best = true;
    man->result.flag_exact = exact;
  }
  else {
    po = ap_generic_asssub_linexpr_array(assign,AP_SCALAR_MPQ,man,destructive,pa,tdim,array,pb);
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
  assert(poly_check(pk,po));
  return po;
}


pk_t* pk_assign_linexpr_array(ap_manager_t* man,
			      bool destructive, pk_t* pa,
			      ap_dim_t* tdim, ap_linexpr0_array_t array,
			      pk_t* pb)
{
  return pk_asssub_linexpr_array(true,man,destructive,
				 pa,tdim,array,pb);
}

pk_t* pk_substitute_linexpr_array(ap_manager_t* man,
				  bool destructive, pk_t* pa,
				  ap_dim_t* tdim, ap_linexpr0_array_t array,
				  pk_t* pb)
{
  return pk_asssub_linexpr_array(false,man,destructive,
				 pa,tdim,array,pb);
}

pk_t* pk_asssub_texpr_array(bool assign,
			    ap_manager_t* man,
			    bool destructive, pk_t* pa,
			    ap_dim_t* tdim, ap_texpr0_array_t* array,
			    pk_t* pb)
{
  bool error;
  ap_linexpr0_array_t linexpr0_array;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  
  if (ap_texpr0_array_is_interval_linear(array) &&
      ap_texpr0_array_is_scalar(array)){
    ap_linexprMPQ_array_set_texpr0_array(pk->linexprMPQ_array,&error,array,pk->num);
    assert(!error);
    ap_linexpr0_array_cons_MPQ(linexpr0_array,pk->linexprMPQ_array);
    return pk_asssub_linexpr_array(assign,man,destructive,
				   pa,tdim,linexpr0_array,pb);
  }
  return (pk_t*)ap_generic_asssub_texpr_array(assign, man, destructive, 
					      pa,tdim,array,pb);
}

pk_t* pk_assign_texpr_array(ap_manager_t* man,
			    bool destructive, pk_t* a,
			    ap_dim_t* tdim, ap_texpr0_array_t* array,
			    pk_t* dest)
{
  return pk_asssub_texpr_array(true,man,destructive,a,tdim,array,dest);
}
pk_t* pk_substitute_texpr_array(ap_manager_t* man,
				bool destructive, pk_t* a,
				ap_dim_t* tdim, ap_texpr0_array_t* texpr,
				pk_t* dest)
{
  return pk_asssub_texpr_array(false,man,destructive,a,tdim,texpr ,dest);
}
