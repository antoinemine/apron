/* ********************************************************************** */
/* pk_user.h: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* This header file defines operations on vectors. A vector is just an array of
   elements of type pkint_t, as a consequence functions need to be given also
   their size. */

#ifndef __PK_USER_H__
#define __PK_USER_H__

#include "pk_config.h"
#include "pk_internal.h"
#include "pk_vector.h"
#include "pk_matrix.h"

#ifdef __cplusplus
extern "C" {
#endif

/* From ITV to PK */

/* Fills the vector with the constraint:
   dim <= bound if sgn>0,
   dim = bound if sgn=0
   dim >= bound if sgn<0

   bound is assumed <> oo.

   Returns false if equality of an integer dimension with a non-integer numbers
*/

bool vector_set_dim_bound(pk_internal_t* pk,
			  numint_t* vec,
			  ap_dim_t dim,
			  numrat_t numrat,
			  int mode,
			  size_t intdim, size_t realdim,
			  bool integer);

bool vector_set_linexpr_bound(pk_internal_t* pk,
			      numint_t* vec,
			      numint_t* vec2,
			      numrat_t numrat,
			      int mode,
			      size_t intdim, size_t realdim,
			      bool integer);

/* Fills the vector with the quasi-linear expression (itv_linexpr) */
void vector_set_itv_linexpr(pk_internal_t* pk,
			    numint_t* vec,
			    itv_linexpr_t* expr,
			    size_t dim,
			    int mode);

/* Fills the vector(s) with the linear constraint cons */
void vector_set_itv_lincons(pk_internal_t* pk,
			    numint_t* vec,
			    itv_lincons_t* cons,
			    size_t intdim, size_t realdim,
			    bool integer);

/* Fills the vector(s) with the linear constraint cons for testing
   satisfiability. Returns false if unsatisfiable
 */
bool vector_set_itv_lincons_sat(pk_internal_t* pk,
				numint_t* vec,
				itv_lincons_t* cons,
				size_t intdim, size_t realdim,
				bool integer);

/* From APRON to PK */
/* Fills the vector with the generator gen. */
bool vector_set_ap_generator0(pk_internal_t* pk,
			      numint_t* vec,
			      ap_generator0_t* gen,
			      size_t intdim, size_t realdim);
bool pk_matrix_set_ap_generator0_array(pk_internal_t* pk,
				    pk_matrix_t** matrix,
				    ap_generator0_array_t* array,
				    size_t intdim, size_t realdim);
/*
bool pk_matrix_set_ap_lincons0_array(pk_internal_t* pk,
				  pk_matrix_t** mat,
				  size_t** tabindex, size_t* size,
				  ap_lincons0_array_t* array,
				  size_t intdim, size_t realdim,
				  bool integer);
bool pk_matrix_set_ap_intlincons0_array(pk_internal_t* pk,
				     pk_matrix_t** mat,
				     itv_t* titv,
				     ap_lincons0_array_t* array,
				     size_t* tab, size_t size,
				     size_t intdim, size_t realdim,
				     bool integer);
*/
/* From ITV to PK */
bool pk_matrix_set_itv_lincons_array(pk_internal_t* pk,
				  pk_matrix_t** mat,
				  itv_lincons_array_t* array,
				  size_t intdim, size_t realdim,
				  bool integer);



/* From PK to APRON */

ap_lincons0_t lincons0_of_vector(pk_internal_t* pk,
				 numint_t* q,
				 size_t size);
ap_generator0_t generator0_of_vector(pk_internal_t* pk,
				     numint_t* q,
				     size_t size);

#ifdef __cplusplus
}
#endif

#endif
