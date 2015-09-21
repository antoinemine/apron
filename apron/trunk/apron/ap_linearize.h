/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_LINEARIZE_H_
#define _AP_LINEARIZE_H_

#include "ap_manager.h"
#include "ap_expr0.h"
#include "ap_abstract0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Evaluation of interval linear expressions */
/* ********************************************************************** */

  /* evaluate to interval */
ap_interval_t* ap_eval_linexpr0(ap_manager_t* man,
				ap_abstract0_t* abs,
				ap_linexpr0_t* expr,
				ap_scalar_discr_t discr,
				bool* pexact);


/* These functions are dedicated to implementors of domains. They offer generic
   default implementations for some of the operations required by the APRON
   API, when there is no more specific and efficient implementation for the
   domain being implemented.

   To use them, the function allocating manager, which is specific to the domain,
   should put the corresponding pointers in the virtual table to them.

   They manipulated "unboxed" abstract values, which are native to the
   underlying library: they are not yet boxed with the manager in the type
   ap_abstract0_t.
*/

/* The following functions use the given abstract value for transforming
   interval linear expressions (resp. constraints, arrays of expressions,
   arrays od constraints) in quasilinear corresponding objects.

   They use to_box and dimension (and is_bottom if NDEBUG is undefined) generic
   functions.

   - discr allows to choose the type of scalars used for computations and for
     the result.

   - pexact is a pointer to a Boolean, which is set to true if all the
     conversions and computations were exact.

   For ap_quasilinearize_XXX functions, if the argument does not need any modification,
   then it is returned itself.

   Calling ap_linearize_linexpr0_array is more efficient than calling N times
   ap_linearize_linexpr0 because the conversion of abstract value to bounding
   boxes is done only once, as well as other internal allocations.
*/

/* ********************************************************************** */
/* II. Quasilinearization of interval linear expressions */
/* ********************************************************************** */

ap_linexpr0_t*
ap_quasilinearize_linexpr0(ap_manager_t* man,
			   void* abs,
			   ap_linexpr0_t* linexpr0,
			   bool* pexact,
			   ap_scalar_discr_t discr);

ap_lincons0_t
ap_quasilinearize_lincons0(ap_manager_t* man,
			   void* abs,
			   ap_lincons0_t* lincons0,
			   bool* pexact,
			   ap_scalar_discr_t discr,
			   bool meet);

ap_linexpr0_t**
ap_quasilinearize_linexpr0_array(ap_manager_t* man,
				 void* abs,
				 ap_linexpr0_t** texpr, size_t size,
				 bool* pexact,
				 ap_scalar_discr_t discr);

ap_lincons0_array_t
ap_quasilinearize_lincons0_array(ap_manager_t* man,
				 void* abs,
				 ap_lincons0_array_t* array,
				 bool* pexact,
				 ap_scalar_discr_t discr,
				 bool linearize,
				 bool meet);

/* ********************************************************************** */
/* III. Evaluation of tree expressions */
/* ********************************************************************** */

ap_interval_t* ap_eval_texpr0(ap_manager_t* man,
			      ap_abstract0_t* abs,
			      ap_texpr0_t* expr,
			      ap_scalar_discr_t discr,
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


ap_linexpr0_t* ap_intlinearize_texpr0_intlinear(ap_manager_t* man,
						ap_texpr0_t* expr,
						ap_scalar_discr_t discr);

/* ********************************************************************** */
/* V. Interval linearization of tree expressions */
/* ********************************************************************** */

ap_linexpr0_t* ap_intlinearize_texpr0(ap_manager_t* man,
				      ap_abstract0_t* abs,
				      ap_texpr0_t* expr,
				      bool* pexact,
				      ap_scalar_discr_t discr,
				      bool quasilinearize);

ap_linexpr0_t** ap_intlinearize_texpr0_array(ap_manager_t* man,
					     ap_abstract0_t* abs,
					     ap_texpr0_t** texpr, size_t size,
					     bool* pexact,
					     ap_scalar_discr_t discr,
					     bool quasilinearize);

ap_lincons0_t ap_intlinearize_tcons0(ap_manager_t* man,
				     ap_abstract0_t* abs,
				     ap_tcons0_t* cons,
				     bool* pexact,
				     ap_scalar_discr_t discr,
				     bool quasilinearize, bool meet);

ap_lincons0_array_t ap_intlinearize_tcons0_array(ap_manager_t* man,
						 ap_abstract0_t* abs,
						 ap_tcons0_array_t* array,
						 bool* pexact,
						 ap_scalar_discr_t discr,
						 ap_linexpr_type_t type, bool meet,
						 bool boxize, size_t kmax, bool intervalonly);

#ifdef __cplusplus
}
#endif

#endif
