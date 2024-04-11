/* ********************************************************************** */
/* pk_resize.h: change and permutation of dimensions  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

/* This header file define operations allowing to convert polyhedra
from one representation to the dual one. */

#ifndef __PK_RESIZE_H__
#define __PK_RESIZE_H__

#include "pk_config.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif


void vector_remove_dimensions(pk_internal_t* pk,
			      numint_t* newq, 
			      numint_t* q, size_t size,
			      ap_dimchange_t* dimchange);

pk_matrix_t* pk_matrix_add_dimensions(pk_internal_t* pk,
				bool destructive, pk_matrix_t* mat,
				ap_dimchange_t* dimchange);

#ifdef __cplusplus
}
#endif

#endif
