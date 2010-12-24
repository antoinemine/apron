/* ************************************************************************* */
/* ap_tcons0.c: tree constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_tcons0.h"

/* ********************************************************************** */
/* I. Linear constraints */
/* ********************************************************************** */

void ap_tcons0_fprint(FILE* stream, ap_tcons0_t* cons, char** name_of_dim)
{
  ap_texpr0_fprint(stream,cons->texpr0,name_of_dim);
  fprintf(stream,
	  cons->constyp == AP_CONS_EQ || cons->constyp == AP_CONS_EQMOD ?
	  " = 0" :
	  ( cons->constyp == AP_CONS_SUPEQ ?
	    " >= 0" :
	    (cons->constyp == AP_CONS_SUP ?
	     " > 0" :
	     "\"ERROR in ap_tcons0_fprint\"")));
  if (cons->constyp == AP_CONS_EQMOD){
    fprintf(stream," mod ");
    mpq_out_str(stream,10,cons->mpq);
  }
}

ap_tcons0_t* ap_tcons0_make_unsat()
{
  ap_texpr0_t* expr = malloc(sizeof(ap_texpr0_t));
  expr->discr = AP_TEXPR_CST;
  ap_coeff_init(expr->val.cst,AP_SCALAR_D);
  ap_coeff_set_int(expr->val.cst,-1);
  return ap_tcons0_make(expr,AP_CONS_SUPEQ,NULL);
}

ap_tcons0_t* ap_tcons0_from_lincons0(ap_lincons0_t cons)
{
  ap_linexpr0_t linexpr0ref;
  ap_texpr0_t* texpr0;
  ap_constyp_t* constyp;
  mpq_ptr mpq;
  ap_lincons0_linexpr0ref(linexpr0ref,cons);
  texpr0 = ap_texpr0_from_linexpr0(linexpr0ref);
  constyp = ap_lincons0_constypref(cons);
  mpq =  ap_lincons0_mpqref(cons);
  return ap_tcons0_make(texpr0,*constyp,mpq);
}

/* ********************************************************************** */
/* II. Array of linear constraints */
/* ********************************************************************** */

ap_tcons0_array_t ap_tcons0_array_make(size_t size)
{
  ap_tcons0_array_t array;
  size_t i;
  array.p = (ap_tcons0_t**)malloc(size*sizeof(ap_tcons0_t*));
  array.size = size;
  for (i=0;i<size;i++) array.p[i] = NULL;
  return array;
}

void ap_tcons0_array_resize(ap_tcons0_array_t* array, size_t size)
{
  size_t i;

  for (i=size; i<array->size; i++){
    ap_tcons0_free(array->p[i]);
    array->p[i] = NULL;
  }
  array->p = (ap_tcons0_t**)realloc(array->p,size*sizeof(ap_tcons0_t*));
  for (i=array->size; i<size; i++){
    array->p[i] = NULL;
  }
  return;
}

ap_tcons0_array_t ap_tcons0_array_copy(ap_tcons0_array_t* array)
{
  ap_tcons0_array_t res;
  size_t i;
  res.p = (ap_tcons0_t**)malloc(array->size*sizeof(ap_tcons0_t*));
  res.size = array->size;
  for (i=0;i<res.size;i++) res.p[i] = ap_tcons0_copy(array->p[i]);
  return res;
}
void ap_tcons0_array_clear(ap_tcons0_array_t* array)
{
  if (array->p){
    size_t i;
    for (i=0; i<array->size; i++){
      if (array->p[i]) ap_tcons0_free(array->p[i]);
      array->p[i] = NULL;
    }
    free(array->p);
    array->p= NULL;
  }
}

void ap_tcons0_array_print(ap_tcons0_array_t* array,
			   char** name_of_dim)
{ ap_tcons0_array_fprint(stdout,array,name_of_dim); }

static bool ap_tcons0_array_is_template(ap_tcons0_array_t* array, bool (*is_template)(ap_tcons0_t* tcons))
{
  size_t i;
  bool res = true;
  for (i=0; i<array->size; i++){
    res = is_template(array->p[i]);
    if (!res) break;
  }
  return res;
}
bool ap_tcons0_array_is_interval_linear(ap_tcons0_array_t* array)
{
  return ap_tcons0_array_is_template(array,ap_tcons0_is_interval_linear);
}
bool ap_tcons0_array_is_interval_polynomial(ap_tcons0_array_t* array)
{
return ap_tcons0_array_is_template(array,ap_tcons0_is_interval_polynomial);
}
bool ap_tcons0_array_is_interval_polyfrac(ap_tcons0_array_t* array)
{
  return ap_tcons0_array_is_template(array,ap_tcons0_is_interval_polyfrac);
}
bool ap_tcons0_array_is_scalar(ap_tcons0_array_t* array)
{
  return ap_tcons0_array_is_template(array,ap_tcons0_is_scalar);
}


/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_tcons0_array_add_dimensions_with(ap_tcons0_array_t* array,
					 ap_dimchange_t* dimchange)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_texpr0_t* expr = array->p[i]->texpr0;
    if (expr) ap_texpr0_add_dimensions_with(expr,dimchange);
  }
}
ap_tcons0_array_t ap_tcons0_array_add_dimensions(ap_tcons0_array_t* array,
						 ap_dimchange_t* dimchange)
{
  size_t i;
  ap_tcons0_array_t narray;

  narray = ap_tcons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] = ap_tcons0_add_dimensions(array->p[i],dimchange);
  }
  return narray;
}

void ap_tcons0_array_permute_dimensions_with(ap_tcons0_array_t* array,
					     ap_dimperm_t* perm)
{
  size_t i;
  for(i=0; i<array->size; i++){
    ap_texpr0_t* expr = array->p[i]->texpr0;
    if (expr) ap_texpr0_permute_dimensions_with(expr,perm);
  }
}
ap_tcons0_array_t ap_tcons0_array_permute_dimensions(ap_tcons0_array_t* array,
						     ap_dimperm_t* perm)
{
  size_t i;
  ap_tcons0_array_t narray;

  narray = ap_tcons0_array_make(array->size);
  for(i=0; i<array->size; i++){
    narray.p[i] =  ap_tcons0_permute_dimensions(array->p[i],perm);
  }
  return narray;
}
