/* ********************************************************************** */
/* pkXXX_cherni.h: Conversion from one representation to the dual one.  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* This header file define operations allowing to convert polyhedra
from one representation to the dual one. */

#ifndef __PKXXX_CHERNI_H__
#define __PKXXX_CHERNI_H__

#include "pkXXX_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Functions meant to be internal */

/* ********************************************************************** */
/* Checking */
/* ********************************************************************** */
bool cherniXXX_checksatmat(pkXXX_internal_t* pk,
			   bool con_to_ray,
			   matrixXXX_t* C, matrixXXX_t* F,
			   satmat_t* satC);
/* Recompute the saturation matrix of C and F and checks the equality with
   satC. */

bool cherniXXX_checksat(pkXXX_internal_t* pk,
			bool con_to_ray,
			matrixXXX_t* C, size_t nbequations,
			matrixXXX_t* F, size_t nblines,
			satmat_t* satC);
/* Check the saturation numbers of constraints and rays. */

/* ********************************************************************** */
/* Conversion algorithm */
/* ********************************************************************** */

size_t cherniXXX_conversion(pkXXX_internal_t* pk,
			    matrixXXX_t* con, size_t start,
			    matrixXXX_t* ray, satmat_t* satc, size_t nbline);
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

size_t cherniXXX_gauss(pkXXX_internal_t* pk, matrixXXX_t* con, size_t nbeq);
/*
  The function finds a minimal system for equalities, and returns the rank *r*
  of this system, equations of which are numbered from 0 to *r-1*. Redundant
  (now null) equations are put between row *r* and row *nbeq*.  The function
  also fills the array *pk->cherni_intp* which indicates the ``main''
  coefficent of an equation, in this case the left-most non-zero one.
*/

void cherniXXX_backsubstitute(pkXXX_internal_t* pk, matrixXXX_t* con, size_t rank);
/*
  This function backsubstitute the coefficients according to the system of
  equations and the array pk->cherni_intp properly set by
  gauss.
 */

int  cherniXXX_simplify(pkXXX_internal_t* pk,
			matrixXXX_t* con, matrixXXX_t* ray,
			satmat_t* satf, size_t nbline);
/*
We suppose that we just obtained ray and satc from con
with the chernikova algorithm. As a consequence the system of rays is
minimal. satf is the transposed matrix of satc, i.e. rows are
indexed by constraints. con is supposed to be non empty.

We have still to simplify con by detecting new equality constraints, removing
redundant inequalities, and obtaining a minimal system of equalities (by a
call to gauss). Then backsubstitution is performed on (minimal) inequalities.
*/
void cherniXXX_minimize(pkXXX_internal_t* pk,
			bool con_to_ray,
			pkXXX_t* po);

void cherniXXX_add_and_minimize(pkXXX_internal_t* pk,
				bool con_to_ray,
				pkXXX_t* po,
				size_t start);
/* Misc */
void cherniXXX_buildsatline(pkXXX_internal_t* pk,
			    matrixXXX_t* con, numintXXX_t* tab,
			    bitstring_t* satline);

/* Library is supposed to be in strict mode (pk->strict==true), polyhedron is
   supposed to be in double representation, with nbeq and nbline correctly
   initialized, and with saturation matrix satF (row i of satF refers to
   constraint i, column k refers to ray k).
 */
bool cherniXXX_minimizeeps(pkXXX_internal_t* pk, pkXXX_t* po);

#ifdef __cplusplus
}
#endif

#endif
