/* ********************************************************************** */
/* pk_test.h: tests on polyhedra */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_TEST_H_
#define _PK_TEST_H_

#include "pk_config.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
F is suppposed to be a valid matrix of ray (i.e., corresponding
to a non empty polyhedron.

The epsilon component of the constraint is not taken into account.  The
constraint is considered as strict only if the is_strict paramater telles so.
This enables to test the satisfiability of a strict constraint in non-strict
mode for the library.
*/
bool do_generators_sat_vector(pk_internal_t* pk,
			      pk_matrix_t* F,
			      numint_t* tab, bool is_strict);

#ifdef __cplusplus
}
#endif

#endif
