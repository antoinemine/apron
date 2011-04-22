/* ********************************************************************** */
/* ap_linexprXXX_eval.h */
/* ********************************************************************** */

#ifndef _AP_LINEXPRXXX_EVAL_H_
#define _AP_LINEXPRXXX_EVAL_H_

#include <stdarg.h>
#include "ap_lin_types.h"
#include "eitvXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Arithmetic */
/* ********************************************************************** */

void ap_linexprXXX_neg(ap_linexprXXX_t res, ap_linexprXXX_t expr);
  /* Negate an expression */
bool ap_linexprXXX_scale(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, num_internal_t intern);
bool ap_linexprXXX_div(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, num_internal_t intern);
  /* Scale an expression by an interval */

bool ap_linexprXXX_add(ap_linexprXXX_t expr,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, num_internal_t intern);
bool ap_linexprXXX_sub(ap_linexprXXX_t expr,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, num_internal_t intern);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* II. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

struct ap_linexpr0_struct;
bool eitvXXX_eval_ap_linexpr0(eitvXXX_t res, struct ap_linexpr0_struct* expr, ap_linexprXXX_t env, num_internal_t intern);

bool ap_linexprXXX_eval(eitvXXX_t res, ap_linexprXXX_t expr, ap_linexprXXX_t env, num_internal_t intern);
  /* Evaluate an interval linear expression. Return true if no
     approximations. */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
bool ap_linexprXXX_quasilinearize(ap_linexprXXX_t linexpr,
				  ap_linexprXXX_t env,
				  bool for_meet_inequality, num_internal_t intern);
  /* Quasilinearize in-place linexpr using the bounding box itv. Return true
     if no approximations. */

bool ap_linexprXXX_array_quasilinearize(ap_linexprXXX_array_t array,
					ap_linexprXXX_t env, num_internal_t intern);
  /* Same for an array */

/* ********************************************************************** */
/* III. Evaluation and interval linearisation of tree expressions */
/* ********************************************************************** */

struct ap_texpr0_t;
struct ap_texpr0_array_t;
bool ap_linexprXXX_set_texpr0(
    ap_linexprXXX_t linexpr, bool* perror,
    struct ap_texpr0_t* expr, num_internal_t intern);
bool ap_linexprXXX_array_set_texpr0_array(
    ap_linexprXXX_array_t linexpr, bool* perror,
    struct ap_texpr0_array_t* expr, num_internal_t intern);
  /* Linearize a tree expression that is (syntaxically) interval linear.
     If the precondition is violated, sets *perror to true.

     Return true if no approximations.
  */

bool eitvXXX_eval_ap_texpr0(
    eitvXXX_t res, struct ap_texpr0_t* texpr, ap_linexprXXX_t env,
    num_internal_t intern);
void ap_linexprXXX_intlinearize_texpr0(
    ap_linexprXXX_t linexpr,
    struct ap_texpr0_t* expr, ap_linexprXXX_t env, size_t intdim,
    num_internal_t intern);
void ap_linexprXXX_array_intlinearize_texpr0_array(
    ap_linexprXXX_array_t linexpr,
    struct ap_texpr0_array_t* texpr, ap_linexprXXX_t env, size_t intdim,
    num_internal_t intern);
  /* Return true if no approximations. */


#ifdef __cplusplus
}
#endif

#endif
