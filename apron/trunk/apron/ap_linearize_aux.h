/* ************************************************************************* */
/* ap_linearize_aux.h: auxiliary functions for (quasi)linearisation */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* Auxiliary module to ap_linearize, which contains functions depending of the
   number representation */

/* may be included several times, with different NUM_ defined */

#if !defined(NUM_MPQ) && !defined(NUM_DOUBLE) && !defined(NUM_LONGDOUBLE) && !defined(NUM_MPFR)
#error "Wrong NUM type"
#endif

#include "ap_manager.h"
#include "ap_expr0.h"
#include "ap_abstract0.h"
#include "itv_fun.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Evaluation of interval linear expressions */
/* ********************************************************************** */

ap_interval_t*
ITVFUN(ap_eval_linexpr0)(ap_manager_t* man,
			 ap_abstract0_t* abs, ap_linexpr0_t* expr,
			 bool* pexact);

/* ********************************************************************** */
/* II. Quasilinearization of interval linear expressions */
/* ********************************************************************** */
ap_linexpr0_t*
ITVFUN(ap_quasilinearize_linexpr0)(ap_manager_t* man,
				   ap_abstract0_t* abs, ap_linexpr0_t* linexpr0,
				   bool* pexact);

ap_lincons0_t
ITVFUN(ap_quasilinearize_lincons0)(ap_manager_t* man,
				   ap_abstract0_t* abs, ap_lincons0_t* lincons0,
				   bool* pexact, bool meet);

ap_linexpr0_t**
ITVFUN(ap_quasilinearize_linexpr0_array)(ap_manager_t* man,
					 ap_abstract0_t* abs, ap_linexpr0_t** texpr, size_t size,
					 bool* pexact);

ap_lincons0_array_t
ITVFUN(ap_quasilinearize_lincons0_array)(ap_manager_t* man,
					 ap_abstract0_t* abs, ap_lincons0_array_t* array, bool* pexact,
					 bool linearize, bool meet);

/* ********************************************************************** */
/* III. Evaluation of tree expressions */
/* ********************************************************************** */
ap_interval_t*
ITVFUN(ap_eval_texpr0)(ap_manager_t* man,
		       ap_abstract0_t* abs, ap_texpr0_t* expr,
		       bool* pexact);

/* ********************************************************************** */
/* IV. Interval linearization of linear tree expressions */
/* ********************************************************************** */

/* Linearize a tree expression that is (syntaxically) interval linear with
   exact arithmetic.

   Compared to ap_intlinearize_texpr0() function below, this functions does
   not require a bounding box for dimensions.

   If the precondition is violated, returns NULL.
*/

ap_linexpr0_t*
ITVFUN(ap_intlinearize_texpr0_intlinear)(ap_manager_t* man,
					 ap_texpr0_t* expr);

/* ********************************************************************** */
/* V. Interval linearization of tree expressions */
/* ********************************************************************** */

ap_linexpr0_t*
ITVFUN(ap_intlinearize_texpr0)(ap_manager_t* man,
			       ap_abstract0_t* abs, ap_texpr0_t* expr,
			       bool* pexact, bool quasilinearize);

ap_linexpr0_t**
ITVFUN(ap_intlinearize_texpr0_array)(ap_manager_t* man,
				     ap_abstract0_t* abs, ap_texpr0_t** texpr0, size_t size,
				     bool* pexact, bool quasilinearize);

ap_lincons0_t
ITVFUN(ap_intlinearize_tcons0)(ap_manager_t* man,
			       ap_abstract0_t* abs, ap_tcons0_t* cons,
			       bool* pexact, bool quasilinearize, bool meet);

ap_lincons0_array_t
ITVFUN(ap_intlinearize_tcons0_array)(ap_manager_t* man,
				     ap_abstract0_t* abs, ap_tcons0_array_t* array,
				     bool* pexact, 
				     ap_linexpr_type_t linearize, bool meet,
				     bool boxize, size_t kmax, bool intervalonly);

#ifdef __cplusplus
}
#endif
