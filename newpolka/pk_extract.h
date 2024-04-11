/* ********************************************************************** */
/* pk_extract.h: property extraction */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_EXTRACT_H_
#define _PK_EXTRACT_H_

#include "pk_config.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bounding by a itv box a matrix of generators. */
itv_t* pk_matrix_to_box(pk_internal_t* pk, pk_matrix_t* F);

/* Bounding the value of a dimension in a matrix of generators.
   mode == 1: sup bound
   mode == -1: inf bound
*/
void pk_matrix_bound_dimension(pk_internal_t* pk,
			    itv_t itv,
			    ap_dim_t dim,
			    pk_matrix_t* F);

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/
void pk_matrix_bound_vector(pk_internal_t* pk,
			 itv_t itv,
			 numint_t* vec,
			 pk_matrix_t* F);

#ifdef __cplusplus
}
#endif

#endif
