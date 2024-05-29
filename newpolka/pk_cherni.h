/* ********************************************************************** */
/* pk_cherni.h: Conversion from one representation to the dual one.  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

/* This header file define operations allowing to convert polyhedra
from one representation to the dual one. */

#ifndef __PK_CHERNI_H__
#define __PK_CHERNI_H__

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Functions meant to be internal */

/* ********************************************************************** */
/* Checking */
/* ********************************************************************** */
bool cherni_checksatmat(pk_internal_t* pk,
			bool con_to_ray,
			pk_matrix_t* C, pk_matrix_t* F,
			satmat_t* satC);
/* Recompute the saturation matrix of C and F and checks the equality with
   satC. */

bool cherni_checksat(pk_internal_t* pk,
		     bool con_to_ray,
		     pk_matrix_t* C, size_t nbequations,
		     pk_matrix_t* F, size_t nblines,
		     satmat_t* satC);
/* Check the saturation numbers of constraints and rays. */

/* ********************************************************************** */
/* Conversion algorithm */
/* ********************************************************************** */

size_t cherni_conversion(pk_internal_t* pk,
			 pk_matrix_t* con, size_t start,
			 pk_matrix_t* ray, satmat_t* satc, size_t nbline);
/*
   Compute the dual form of con.

   - con is the constraints matrix,
   - start indicates the number of constraints supposed to be already taken in
     account,
   - ray is the frames matrix, with the start first constraints taken in account,
   - satc is the saturation matrix initialized according to con and ray,
   - nbline indicates the number of lines in ray.

   For the saturation matrix, we suppose that Unused bits of existing rows are
   set to zero. Each time a row is added, this hypothese must remain true.

   The saturation matrix is organised as follows:
   - the rows are indexed by generators,
   - the columns by constraints.

   The result is given by ray, sat and the returned number of lines.
*/

size_t cherni_gauss(pk_internal_t* pk, pk_matrix_t* con, size_t nbeq);
/*
  The function finds a minimal system for equalities, and returns the rank *r*
  of this system, equations of which are numbered from 0 to *r-1*. Redundant
  (now null) equations are put between row *r* and row *nbeq*.  The function
  also fills the array *pk->cherni_intp* which indicates the ``main''
  coefficent of an equation, in this case the left-most non-zero one.
*/

void cherni_backsubstitute(pk_internal_t* pk, pk_matrix_t* con, size_t rank);
/*
  This function backsubstitute the coefficients according to the system of
  equations and the array pk->cherni_intp properly set by
  gauss.
 */

int  cherni_simplify(pk_internal_t* pk,
		     pk_matrix_t* con, pk_matrix_t* ray,
		     satmat_t* satf, const size_t nbline);
/*
We suppose that we just obtained ray and satc from con
with the chernikova algorithm. As a consequence the system of rays is
minimal. satf is the transposed matrix of satc, i.e. rows are
indexed by constraints. con is supposed to be non empty.

We have still to simplify con by detecting new equality constraints, removing
redundant inequalities, and obtaining a minimal system of equalities (by a
call to gauss). Then backsubstitution is performed on (minimal) inequalities.
*/
void cherni_minimize(pk_internal_t* pk,
		     bool con_to_ray,
		     pk_t* po);

void cherni_add_and_minimize(pk_internal_t* pk,
			     bool con_to_ray,
			     pk_t* po,
			     size_t start);
/* Misc */
void cherni_buildsatline(pk_internal_t* pk,
			 pk_matrix_t* con, numint_t* tab,
			 bitstring_t* satline);

/* Library is supposed to be in strict mode (pk->strict==true), polyhedron is
   supposed to be in double representation, with nbeq and nbline correctly
   initialized, and with saturation matrix satF (row i of satF refers to
   constraint i, column k refers to ray k).
 */
bool cherni_minimizeeps(pk_internal_t* pk, pk_t* po);

#ifdef __cplusplus
}
#endif

#endif
