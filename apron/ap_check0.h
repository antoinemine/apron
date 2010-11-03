/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_check0.h: checks for level 0 */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_abstract0.h"

/* ********************************************************************** */
/* 2 Checking typing w.r.t. manager */
/* ********************************************************************** */

/*
  These functions return true if everything is OK, otherwise they raise an
  exception in the manager and return false.
*/

/* One abstract value */
void ap_check0_man1_raise(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t* a);
static inline
bool ap_check0_man1(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t* a);

/* Two abstract values */
bool ap_check0_man2(ap_funid_t funid,
		    ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2);

/* Array of abstract values */
bool ap_check0_man_array(ap_funid_t funid,
			 ap_manager_t* man, ap_abstract0_t** tab, size_t size);

/* ********************************************************************** */
/* 3. Checking compatibility of arguments: abstract values */
/* ********************************************************************** */

/* Getting dimensions without checks */
static inline
ap_dimension_t _ap_abstract0_dimension(ap_abstract0_t* a);

/* Check that the 2 abstract values have the same dimensionality */
bool ap_check0_abstract2(ap_funid_t funid, ap_manager_t* man,
			 ap_abstract0_t* a1, ap_abstract0_t* a2);

/* Check that the array of abstract values have the same dimensionality.*/
bool ap_check0_abstract_array(ap_funid_t funid, ap_manager_t* man,
			      ap_abstract0_t** tab, size_t size);

/* ********************************************************************** */
/* 4. Checking compatibility of arguments: dimensions */
/* ********************************************************************** */

/* Check that the dimension makes sense in the given dimensionality */
void ap_check0_dim_raise(ap_funid_t funid, ap_manager_t* man,
			 ap_dimension_t dimension, ap_dim_t dim,
			 char* prefix);
static inline
bool ap_check0_dim(ap_funid_t funid, ap_manager_t* man,
		   ap_dimension_t dimension, ap_dim_t dim);

/* Check that the array of dimensions make sense in the given dimensionality */
bool ap_check0_dim_array(ap_funid_t funid, ap_manager_t* man,
			 ap_dimension_t dimension, ap_dim_t* tdim, size_t size);

/* ********************************************************************** */
/* 5. Checking compatibility of arguments: expressions */
/* ********************************************************************** */

void ap_check0_expr_raise(ap_funid_t funid, ap_manager_t* man,
			  ap_dimension_t dimension,
			  ap_dim_t dim,
			  char* prefix);

/* Check that the linear expression makes sense in the given dimensionality */
ap_dim_t ap_check0_linexprD_dim(ap_dimension_t dimension,
				ap_linexprD_t expr);
ap_dim_t ap_check0_linexprMPQ_dim(ap_dimension_t dimension,
				  ap_linexprMPQ_t expr);
ap_dim_t ap_check0_linexprMPFR_dim(ap_dimension_t dimension,
				   ap_linexprMPFR_t expr);
ap_dim_t ap_check0_linexpr_dim(ap_dimension_t dimension,
			       ap_linexpr0_t expr);
ap_dim_t ap_check0_lincons_dim(ap_dimension_t dimension,
			       ap_lincons0_t cons);
bool ap_check0_linexpr(ap_funid_t funid, ap_manager_t* man,
		       ap_dimension_t dimension,
		       ap_linexpr0_t expr);
bool ap_check0_texpr(ap_funid_t funid, ap_manager_t* man,
		     ap_dimension_t dimension,
		     ap_texpr0_t* expr);
bool ap_check0_lincons(ap_funid_t funid, ap_manager_t* man,
		       ap_dimension_t dimension,
		       ap_lincons0_t cons);
bool ap_check0_tcons(ap_funid_t funid, ap_manager_t* man,
		     ap_dimension_t dimension,
		     ap_tcons0_t* cons);


/* ********************************************************************** */
/* 6. Checking compatibility of arguments: array of expressions/constraints/generators */
/* ********************************************************************** */

/* Check that array of linear expressions makes sense in the given dimensionality */
bool ap_check0_linexpr_array(ap_funid_t funid, ap_manager_t* man,
			     ap_dimension_t dimension,
			     ap_linexpr0_array_t array);

/* Check that array of linear constraint makes sense in the given dimensionality */
bool ap_check0_lincons_array(ap_funid_t funid, ap_manager_t* man,
			     ap_dimension_t dimension,
			     ap_lincons0_array_t array);

/* Check that array of generator makes sense in the given dimensionality */
bool ap_check0_lingen_array(ap_funid_t funid, ap_manager_t* man,
			    ap_dimension_t dimension, ap_lingen0_array_t array);

/* Check that array of tree expressions makes sense in the given dimensionality */
bool ap_check0_texpr_array(ap_funid_t funid, ap_manager_t* man,
			   ap_dimension_t dimension,
			   ap_texpr0_array_t* array);

/* Check that array of tree constraint makes sense in the given dimensionality */
bool ap_check0_tcons_array(ap_funid_t funid, ap_manager_t* man,
			   ap_dimension_t dimension,
			   ap_tcons0_array_t* array);

/* ********************************************************************** */
/* 7. Checking compatibility of arguments: dimchange */
/* ********************************************************************** */

bool ap_check0_dimchange_add(ap_funid_t funid, ap_manager_t* man,
			     ap_dimension_t dimension, ap_dimchange_t* dimchange);
bool ap_check0_dimchange_remove(ap_funid_t funid, ap_manager_t* man,
				ap_dimension_t dimension, ap_dimchange_t* dimchange);
bool ap_check0_dimperm(ap_funid_t funid, ap_manager_t* man,
		       ap_dimension_t dimension, ap_dimperm_t* perm);

static inline
bool ap_check0_man1(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t* a)
{
  if (man->library != a->man->library){
    ap_check0_man1_raise(funid,man,a);
    return false;
  }
  else
    return true;
}
static inline
bool ap_check0_dim(ap_funid_t funid, ap_manager_t* man,
		   ap_dimension_t dimension, ap_dim_t dim)
{
  if (dim>=dimension.intdim+dimension.realdim){
      ap_check0_dim_raise(funid,man,dimension,dim,"incompatible dimension");
      return false;
  }
  return true;
}
/* Getting dimensions without checks */
static inline
ap_dimension_t _ap_abstract0_dimension(ap_abstract0_t* a)
{
  ap_dimension_t (*ptr)(ap_manager_t*,...) = (ap_dimension_t (*) (ap_manager_t*,...))(a->man->funptr[AP_FUNID_DIMENSION]);
  return ptr(a->man,a->value);
}
