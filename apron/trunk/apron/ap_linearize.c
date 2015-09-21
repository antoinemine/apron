/* ************************************************************************* */
/* ap_linearize: generic functions for (quasi)linearisation ofinterval expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_linearize.h"

#define NUM_MPQ
#include "ap_linearize_aux.h"
#undef  NUM_MPQ

#define NUM_DOUBLE
#include "ap_linearize_aux.h"
#undef  NUM_DOUBLE

#define NUM_LONGDOUBLE
#include "ap_linearize_aux.h"
#undef  NUM_LONGDOUBLE

#define NUM_MPFR
#include "ap_linearize_aux.h"
#undef  NUM_MPFR

/* ********************************************************************** */
/* I. Evaluation of interval linear expressions */
/* ********************************************************************** */

ap_interval_t*
ap_eval_linexpr0(ap_manager_t* man,
		 ap_abstract0_t* abs,
		 ap_linexpr0_t* expr,
		 ap_scalar_discr_t discr,
		 bool* pexact)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_eval_linexpr0_MPQ(man,abs,expr,pexact);
  case AP_SCALAR_DOUBLE:
    return ap_eval_linexpr0_D(man,abs,expr,pexact);
  case AP_SCALAR_MPFR:
    return ap_eval_linexpr0_MPFR(man,abs,expr,pexact);
  default:
    abort();
  }
}

/* ********************************************************************** */
/* II. Quasilinearization of interval linear expressions */
/* ********************************************************************** */

/* If the interval linear expression is already quasilinear, then return it.
   Otherwise, evaluate an interval linear expression on the abstract
   value such as to transform it into a new quasilinear expression.

   discr allows to choose the type of scalars used for computations and for the
   result.

   pexact is a pointer to a Boolean, which is set to true if all
   the conversions and computations were exact.
*/

ap_linexpr0_t* ap_quasilinearize_linexpr0(ap_manager_t* man,
					  void* abs,
					  ap_linexpr0_t* linexpr0,
					  bool* pexact,
					  ap_scalar_discr_t discr)
{
  if (ap_linexpr0_is_quasilinear(linexpr0)){
    *pexact = true;
    return linexpr0;
  }
  else {
    switch (discr){
    case AP_SCALAR_MPQ:
      return ap_quasilinearize_linexpr0_MPQ(man,abs,linexpr0,pexact);
    case AP_SCALAR_DOUBLE:
      return ap_quasilinearize_linexpr0_D(man,abs,linexpr0,pexact);
    case AP_SCALAR_MPFR:
      return ap_quasilinearize_linexpr0_MPFR(man,abs,linexpr0,pexact);
    default:
      abort();
    }
  }
}
/* Same for ap_lincons0_t */
ap_lincons0_t ap_quasilinearize_lincons0(ap_manager_t* man,
					 void* abs,
					 ap_lincons0_t* lincons0,
					 bool* pexact,
					 ap_scalar_discr_t discr,
					 bool meet)
{
  if (ap_linexpr0_is_quasilinear(lincons0->linexpr0)){
    *pexact = true;
    return *lincons0;
  }
  else {
    switch (discr){
    case AP_SCALAR_MPQ:
      return ap_quasilinearize_lincons0_MPQ(man,abs,lincons0,pexact,meet);
    case AP_SCALAR_DOUBLE:
      return ap_quasilinearize_lincons0_D(man,abs,lincons0,pexact,meet);
    case AP_SCALAR_MPFR:
      return ap_quasilinearize_lincons0_MPFR(man,abs,lincons0,pexact,meet);
    default:
      abort();
    }
  }
}

/* Same for arrays of ap_linexpr0_t */
ap_linexpr0_t**
ap_quasilinearize_linexpr0_array(ap_manager_t* man,
				 void* abs,
				 ap_linexpr0_t** texpr, size_t size,
				 bool* pexact,
				 ap_scalar_discr_t discr)
{
  if (ap_linexpr0_array_is_quasilinear(texpr,size)){
    *pexact = true;
    return texpr;
  }
  else {
    switch (discr){
    case AP_SCALAR_MPQ:
      return ap_quasilinearize_linexpr0_array_MPQ(man,abs,texpr,size,pexact);
    case AP_SCALAR_DOUBLE:
      return ap_quasilinearize_linexpr0_array_D(man,abs,texpr,size,pexact);
    case AP_SCALAR_MPFR:
      return ap_quasilinearize_linexpr0_array_MPFR(man,abs,texpr,size,pexact);
    default:
      abort();
    }
  }
}
/* Same for ap_lincons0_array_t */
ap_lincons0_array_t
ap_quasilinearize_lincons0_array(ap_manager_t* man,
				 void* abs,
				 ap_lincons0_array_t* array,
				 bool* pexact,
				 ap_scalar_discr_t discr,
				 bool linearize,
				 bool meet)
{
  ap_linexpr_type_t type = ap_lincons0_array_type(array);
  if ((linearize && type==AP_LINEXPR_LINEAR) ||
      type==AP_LINEXPR_QUASILINEAR){
    *pexact = true;
    return *array;
  }
  else {
    switch (discr){
    case AP_SCALAR_MPQ:
      return ap_quasilinearize_lincons0_array_MPQ(man,abs,array,pexact,linearize,meet);
    case AP_SCALAR_DOUBLE:
      return ap_quasilinearize_lincons0_array_D(man,abs,array,pexact,linearize,meet);
    case AP_SCALAR_MPFR:
      return ap_quasilinearize_lincons0_array_MPFR(man,abs,array,pexact,linearize,meet);
    default:
      abort();
    }
  }
}

/* ********************************************************************** */
/* III. Evaluation of tree expressions */
/* ********************************************************************** */

ap_interval_t*
ap_eval_texpr0(ap_manager_t* man,
	       struct ap_abstract0_t* abs,
	       ap_texpr0_t* expr,
	       ap_scalar_discr_t discr,
	       bool* pexact)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_eval_texpr0_MPQ(man,abs,expr,pexact);
  case AP_SCALAR_DOUBLE:
    return ap_eval_texpr0_D(man,abs,expr,pexact);
  case AP_SCALAR_MPFR:
    return ap_eval_texpr0_MPFR(man,abs,expr,pexact);
  default:
    assert(false);
    return NULL;
  }
 }

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
ap_intlinearize_texpr0_intlinear(ap_manager_t* man,
				 ap_texpr0_t* expr,
				 ap_scalar_discr_t discr)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_intlinearize_texpr0_intlinear_MPQ(man,expr);
  case AP_SCALAR_DOUBLE:
    return ap_intlinearize_texpr0_intlinear_D(man,expr);
  case AP_SCALAR_MPFR:
    return ap_intlinearize_texpr0_intlinear_MPFR(man,expr);
  default:
    assert(false);
    return NULL;
  }
}

/* ********************************************************************** */
/* V. Interval linearization of tree expressions */
/* ********************************************************************** */

ap_linexpr0_t*
ap_intlinearize_texpr0(ap_manager_t* man,
		       ap_abstract0_t* abs,
		       ap_texpr0_t* expr,
		       bool* pexact,
		       ap_scalar_discr_t discr,
		       bool quasilinearize)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_intlinearize_texpr0_MPQ(man,abs,expr,pexact,quasilinearize);
  case AP_SCALAR_DOUBLE:
    return ap_intlinearize_texpr0_D(man,abs,expr,pexact,quasilinearize);
  case AP_SCALAR_MPFR:
    return ap_intlinearize_texpr0_MPFR(man,abs,expr,pexact,quasilinearize);
  default:
    assert(false);
    return NULL;
  }
}

ap_linexpr0_t** ap_intlinearize_texpr0_array(ap_manager_t* man,
					     ap_abstract0_t* abs,
					     ap_texpr0_t** texpr, size_t size,
					     bool* pexact,
					     ap_scalar_discr_t discr,
					     bool quasilinearize)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_intlinearize_texpr0_array_MPQ(man,abs,texpr,size,pexact,quasilinearize);
  case AP_SCALAR_DOUBLE:
    return ap_intlinearize_texpr0_array_D(man,abs,texpr,size,pexact,quasilinearize);
  case AP_SCALAR_MPFR:
    return ap_intlinearize_texpr0_array_MPFR(man,abs,texpr,size,pexact,quasilinearize);
  default:
    assert(false);
    return NULL;
  }
}

ap_lincons0_t ap_intlinearize_tcons0(ap_manager_t* man,
				     ap_abstract0_t* abs,
				     ap_tcons0_t* cons,
				     bool* pexact,
				     ap_scalar_discr_t discr,
				     bool quasilinearize, bool meet)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_intlinearize_tcons0_MPQ(man,abs,cons,pexact,quasilinearize,meet);
  case AP_SCALAR_DOUBLE:
    return ap_intlinearize_tcons0_D(man,abs,cons,pexact,quasilinearize,meet);
  case AP_SCALAR_MPFR:
    return ap_intlinearize_tcons0_MPFR(man,abs,cons,pexact,quasilinearize,meet);
  default:
    assert(false);
    return ap_lincons0_make(AP_CONS_EQ,NULL,NULL);
  }
}

ap_lincons0_array_t ap_intlinearize_tcons0_array(ap_manager_t* man,
						 ap_abstract0_t* abs,
						 ap_tcons0_array_t* array,
						 bool* pexact,
						 ap_scalar_discr_t discr,
						 ap_linexpr_type_t linearize, bool meet,
						 bool boxize, size_t kmax, bool intervalonly)
{
  switch (discr){
  case AP_SCALAR_MPQ:
    return ap_intlinearize_tcons0_array_MPQ(man,abs,array,pexact,linearize,meet,boxize,kmax,intervalonly);
  case AP_SCALAR_DOUBLE:
    return ap_intlinearize_tcons0_array_D(man,abs,array,pexact,linearize,meet,boxize,kmax,intervalonly);
  case AP_SCALAR_MPFR:
    return ap_intlinearize_tcons0_array_MPFR(man,abs,array,pexact,linearize,meet,boxize,kmax,intervalonly);
  default:
    assert(false);
    return ap_lincons0_array_make(0);
  }
}
