/* ********************************************************************** */
/* pk_constructor.h: constructors and accessors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_CONSTRUCTOR_H_
#define _PK_CONSTRUCTOR_H_

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Fill the first (pk->dec-1) rows of the matrix with the constraints of the
   universe polyhedron */
void pk_matrix_fill_constraint_top(pk_internal_t* pk, pk_matrix_t* C, size_t start);

/* Assign with GMP semantics the given polyhedron with the empty
   (resp. universe) polyhedron, of same dimensions */
void poly_set_bottom(pk_internal_t* pk, pk_t* po);
void poly_set_top(pk_internal_t* pk, pk_t* po);

#ifdef __cplusplus
}
#endif

#endif
