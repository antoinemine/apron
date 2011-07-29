/* ********************************************************************** */
/* ap_linconsXXX_eval.h */
/* ********************************************************************** */

#ifndef _AP_LINCONSXXX_EVAL_H_
#define _AP_LINCONSXXX_EVAL_H_

#include "ap_lin_types.h"
#include "ap_linexprXXX_eval.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Evaluation and simplification */
/* ********************************************************************** */

tbool_t ap_linconsXXX_evalcst(ap_linconsXXX_t lincons, num_internal_t intern);
  /* Is the assumed constant constraint satisfied ? */

bool ap_linconsXXX_is_unsat(ap_linconsXXX_t lincons, num_internal_t intern);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itvXXX_eval_cstlincons returns tbool_false
  */

void ap_linconsXXX_reduce_integer(ap_linconsXXX_t cons, size_t intdim, num_internal_t intern);
  /* Transform a constraint involving only integer variables and only scalar
     coefficients (with the exception of the constant) as follows:
     - divide expr by the pgcd of non-constant coefficients
       details differ according to type of num (integer, rational,
       floating-point)

     - expr + [min,sup] >= 0  ==>  expr + floor(sup) >= 0
     - expr + [min,sup] > 0   ==>  expr + sup - 1 >=      if sup is integer
				   expr + floor(sup) >= 0 otherwise
     - expr + [min,sup] = 0   ==>  expr + [ceil(min),floor(sup)] = 0
  */

tbool_t ap_linconsXXX_array_reduce(ap_linconsXXX_array_t array, bool meet, num_internal_t intern);
  /* Simplify the array as follows:
     - remove trivially true constraints (like 1>=0)
     - if a constraint is trivially false, reduce the array to the constraint
       1=0
     Return
     - tbool_true if empty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t ap_linconsXXX_array_reduce_integer(ap_linconsXXX_array_t array,
					   size_t intdim,
					   num_internal_t intern);
  /* Apply first ap_linconsXXX_reduce_integer, and then
     ap_linconsXXX_array_reduce(.,.,true), not valid for testing satisfiablity */

/* ********************************************************************** */
/* II. Linearization */
/* ********************************************************************** */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
static inline
bool ap_linconsXXX_quasilinearize(ap_linconsXXX_t lincons,
				  ap_linexprXXX_t env,
				  bool for_meet_inequality, num_internal_t intern);
  /* Quasilinearize in-place lincons using the bounding box itv. Return true
     if no approximations. */

bool ap_linconsXXX_array_quasilinearize(ap_linconsXXX_array_t array,
					ap_linexprXXX_t env,
					bool meet,
					num_internal_t intern);
  /* Same for an array */

void ap_linconsXXX_array_linearize(ap_linconsXXX_array_t array,
				   bool meet,
				   num_internal_t intern);
  /* tranforms
     - e+[a,b]>=0 in e+b>=0
     - e+[a,b]==0 in e+b>=0 and e+a<=0
  */

struct ap_tcons0_t;
struct ap_tcons0_array_t;
bool ap_linconsXXX_set_tcons0(
    ap_linconsXXX_t lincons, bool* perror,
    struct ap_tcons0_t* cons, num_internal_t intern);
bool ap_linconsXXX_array_set_tcons0_array(
    ap_linconsXXX_array_t lincons, bool* perror,
    struct ap_tcons0_array_t* cons, num_internal_t intern);
  /* Linearize a tree constraint that is (syntaxically) interval linear .
     If the precondition is violated, sets *perror to true.
  */

void ap_linconsXXX_intlinearize_tcons0(
    ap_linconsXXX_t lincons, struct ap_tcons0_t* cons,
    ap_linexprXXX_t env, size_t intdim, num_internal_t intern);
void ap_linconsXXX_array_intlinearize_tcons0_array(
    ap_linconsXXX_array_t lincons, struct ap_tcons0_array_t* cons,
    ap_linexprXXX_t env, size_t intdim, num_internal_t intern);
  /* Return true if no approximations. */

/* ********************************************************************** */
/* III. Boxization of interval linear expressions */
/* ********************************************************************** */

bool ap_linconsXXX_array_boxize(ap_linexprXXX_t res,
				bool* tchange,
				ap_linconsXXX_array_t array,
				ap_linexprXXX_t env, size_t intdim,
				size_t kmax,
				bool intervalonly, num_internal_t intern);
  /* Deduce interval constraints from a set of interval linear constraints.

     Return true if some bounds have been inferred.

     - The inferred bounds are stored in res (which may be equal to env)
     - If tchange!=NULL, tchange[2dim] (resp. 2dim+1) set to true indicates
       that the inf (resp. sup) bound of dimension dim has been improved.
     - env is the current bounds for variables
     - kmax specifies the maximum number of iterations
     - if intervalonly is true, deduces bounds from a constraint only when the
       coefficient associated to the current dimension is an interval.
  */


/* ********************************************************************** */
/* ********************************************************************** */

static inline
bool ap_linconsXXX_quasilinearize(ap_linconsXXX_t lincons,
				  ap_linexprXXX_t env,
				  bool for_meet_inequality, num_internal_t intern)
{
  return ap_linexprXXX_quasilinearize(lincons->linexpr,env,
				      for_meet_inequality &&
				      (lincons->constyp==AP_CONS_SUP ||
				       lincons->constyp==AP_CONS_SUPEQ),
				      intern);
}

#ifdef __cplusplus
}
#endif

#endif
