/* ********************************************************************** */
/* itv_linearize.c: */
/* ********************************************************************** */

#ifndef _ITV_LINEARIZE_H_
#define _ITV_LINEARIZE_H_

#include "itv.h"
#include "itv_linexpr.h"
#include "ap_expr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Evaluation of expressions  */
/* ********************************************************************** */

static inline void itv_eval_linexpr(itv_internal_t* intern,
				    itv_t itv,
				    itv_linexpr_t* expr,
				    itv_t* env);
  /* Evaluate the interval linear expression, using the array p[]
     associating intervals to dimensions */

static inline bool itv_eval_ap_linexpr0(itv_internal_t* intern,
					itv_t itv,
					ap_linexpr0_t* expr,
					itv_t* env);
  /* Evaluate the interval linear expression, using the array p[]
     associating intervals to dimensions.

     Return true if all conversions were exact */

/* ********************************************************************** */
/* II. Boxization of interval linear expressions */
/* ********************************************************************** */

static inline bool itv_boxize_lincons_array(itv_internal_t* intern,
					    itv_t* res,
					    bool* tchange,
					    itv_lincons_array_t* array,
					    itv_t* env,size_t intdim,
					    size_t kmax,
					    bool intervalonly,
                                            bool* empty);
  /* Deduce interval constraints from a set of interval linear constraints.

     Return true if some bounds have been inferred.
     Sets *empty to true if the result is empty, to false otherwise.

     - The inferred bounds are stored in res (which may be equal to env)
     - If tchange!=NULL, tchange[2dim] (resp. 2dim+1) set to true indicates
       that the inf (resp. sup) bound of dimension dim has been improved.
     - env is the current bounds for variables
     - kmax specifies the maximum number of iterations
     - if intervalonly is true, deduces bounds from a constraint only when the
       coefficient associated to the current dimension is an interval.
  */

/* ********************************************************************** */
/* III. (Quasi)linearisation of interval linear expressions and constraints */
/* ********************************************************************** */

/* These functions return true if there were no rounding in computations.

   They may perform simplifications, which are valid
   - for intersecting constraints, when parameter meet is true
   - for testing satisfaction, when parameter meet is false
*/

static inline bool itv_quasilinearize_linexpr(itv_internal_t* intern, itv_linexpr_t* linexpr, itv_t* env, bool for_meet_inequality);
  /* Quasilinearize in-place linexpr using the bounding box itv */

static inline bool itv_quasilinearize_lincons(itv_internal_t* intern, itv_lincons_t* lincons, itv_t* env, bool meet);
  /* Quasilinearize in-place lincons using the bounding box itv */

static inline bool itv_quasilinearize_lincons_array(itv_internal_t* intern, itv_lincons_array_t* tlincons, itv_t* env, bool meet);
  /* (Quasi)linearize in-place tlincons using the bounding box itv */

static inline void itv_linearize_lincons_array(itv_internal_t* intern, itv_lincons_array_t* tlincons, bool meet);
  /* Linearize in-place quasilinear tlincons using the bounding box itv */

/* ********************************************************************** */
/* IV. Evaluation of tree expressions  */
/* ********************************************************************** */

static inline void itv_eval_ap_texpr0(itv_internal_t* intern, itv_t res, ap_texpr0_t* expr, itv_t* env);

/* ********************************************************************** */
/* V. Linearisation of linear tree expressions. */
/* ********************************************************************** */

/* Linearize a tree expression that is (syntaxically) linear with exact
   arithmetic. 

   Compared to itv_intlinearize_ap_XX() functions below, these functions does
   not require a bounding box for dimensions.

   If the precondition is violated, the result may be unsound.
   Return true in case of problem (non linear tree expression). 
*/

static inline bool itv_intlinearize_ap_texpr0_intlinear(itv_internal_t* intern, itv_linexpr_t* res, ap_texpr0_t* expr);
static inline bool itv_intlinearize_ap_tcons0_intlinear(itv_internal_t* intern, itv_lincons_t* res, ap_tcons0_t* cons);
static inline bool itv_intlinearize_ap_tcons0_array_intlinear(itv_internal_t* intern, itv_lincons_array_t* res, ap_tcons0_array_t* cons);

/* ********************************************************************** */
/* VI. Linearisation of tree expressions. */
/* ********************************************************************** */

/* These functions returns true if the linearization return in an empty
   expression (of type [1;-1]). This may happen when linearizing
   sqrt(-1), for instance.

   They may perform simplifications, which are valid
   - for intersecting constraints, when parameter meet is true
   - for testing satisfaction, when parameter meet is false
*/

static inline bool itv_intlinearize_ap_texpr0(itv_internal_t* intern, itv_linexpr_t* res, ap_texpr0_t* expr, itv_t* env, size_t intdim);
static inline bool itv_intlinearize_ap_tcons0(itv_internal_t* intern, itv_lincons_t* res, ap_tcons0_t* cons, itv_t* env, size_t intdim);
static inline bool itv_intlinearize_ap_tcons0_array(itv_internal_t* intern, itv_lincons_array_t* res, ap_tcons0_array_t* cons, itv_t* env, size_t intdim);


/* ====================================================================== */
/* VII. Backward evaluation of tree expressions. */
/* ====================================================================== */

static inline bool itv_meet_ap_tcons0_array(itv_internal_t* intern, ap_tcons0_array_t* array, itv_t* env, size_t intdim, int max_iter);
static inline bool itv_subst_ap_texpr0_array(itv_internal_t* intern, itv_t* res, itv_t* arg, ap_dim_t* dim, ap_texpr0_t** array, size_t size, size_t intdim, size_t realdim, int max_iter);



/* ********************************************************************** */
/* Prototypes of functions */
/* ********************************************************************** */

/* I. Evaluation of expressions  */
void ITVFUN(itv_eval_linexpr)(itv_internal_t* intern, itv_t itv, itv_linexpr_t* expr, itv_t* env);
bool ITVFUN(itv_eval_ap_linexpr0)(itv_internal_t* intern, itv_t itv, ap_linexpr0_t* expr, itv_t* env);

/* II. Boxization of interval linear expressions */
bool ITVFUN(itv_boxize_lincons_array)(itv_internal_t* intern, itv_t* res, bool* change, itv_lincons_array_t* array, itv_t* env, size_t intdim, size_t kmax, bool intervalonly, bool* empty);

/* III. (Quasi)linearisation of interval linear expressions and constraints */
bool ITVFUN(itv_quasilinearize_linexpr)(itv_internal_t* intern, itv_linexpr_t* linexpr, itv_t* env, bool for_meet_inequality);
bool ITVFUN(itv_quasilinearize_lincons)(itv_internal_t* intern, itv_lincons_t* lincons, itv_t* env, bool meet);
bool ITVFUN(itv_quasilinearize_lincons_array)(itv_internal_t* intern, itv_lincons_array_t* tlincons, itv_t* env, bool meet);
void ITVFUN(itv_linearize_lincons_array)(itv_internal_t* intern, itv_lincons_array_t* array, bool meet);

/* IV. Evaluation of tree expressions  */
void ITVFUN(itv_eval_ap_texpr0)(itv_internal_t* intern, itv_t itv, ap_texpr0_t* expr, itv_t* env);

/* V. Linearisation of linear tree expressions. */
bool ITVFUN(itv_intlinearize_ap_texpr0_intlinear)(itv_internal_t* intern, itv_linexpr_t* res, ap_texpr0_t* expr);
bool ITVFUN(itv_intlinearize_ap_tcons0_intlinear)(itv_internal_t* intern,itv_lincons_t* res, ap_tcons0_t* cons);
bool ITVFUN(itv_intlinearize_ap_tcons0_array_intlinear)(itv_internal_t* intern,itv_lincons_array_t* res, ap_tcons0_array_t* cons);

/* VI. Linearisation of tree expressions. */
bool ITVFUN(itv_intlinearize_ap_texpr0)(itv_internal_t* intern,itv_linexpr_t* res,ap_texpr0_t* expr,itv_t* env, size_t intdim);
bool ITVFUN(itv_intlinearize_ap_tcons0)(itv_internal_t* intern,itv_lincons_t* res, ap_tcons0_t* cons, itv_t* env, size_t intdim);
bool ITVFUN(itv_intlinearize_ap_tcons0_array)(itv_internal_t* intern,itv_lincons_array_t* res, ap_tcons0_array_t* cons, itv_t* env, size_t intdim);

/* VII. Backward evaluation of tree expressions. */
bool ITVFUN(itv_meet_ap_tcons0_array)(itv_internal_t* intern, ap_tcons0_array_t* array, itv_t* env, size_t intdim, int max_iter);
bool ITVFUN(itv_subst_ap_texpr0_array)(itv_internal_t* intern, itv_t* res, itv_t* arg, ap_dim_t* dim, ap_texpr0_t** array, size_t size, size_t intdim, size_t realdim, int max_iter);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */
/* I. Evaluation of expressions  */
static inline void itv_eval_linexpr(itv_internal_t* intern, itv_t itv, itv_linexpr_t* expr, itv_t* env)
{ ITVFUN(itv_eval_linexpr)(intern,itv,expr,env); }
static inline bool itv_eval_ap_linexpr0(itv_internal_t* intern, itv_t itv, ap_linexpr0_t* expr, itv_t* env)
{ return ITVFUN(itv_eval_ap_linexpr0)(intern,itv,expr,env); }

/* II. Boxization of interval linear expressions */
static inline bool itv_boxize_lincons_array(itv_internal_t* intern, itv_t* res, bool* tchange, itv_lincons_array_t* array, itv_t* env, size_t intdim, size_t kmax, bool intervalonly, bool* empty)
{ return ITVFUN(itv_boxize_lincons_array)(intern,res,tchange,array,env,intdim,kmax,intervalonly,empty); }

/* III. (Quasi)linearisation of interval linear expressions and constraints */
static inline bool itv_quasilinearize_linexpr(itv_internal_t* intern, itv_linexpr_t* linexpr, itv_t* env, bool for_ineq)
  { return ITVFUN(itv_quasilinearize_linexpr)(intern,linexpr,env, for_ineq); }
static inline bool itv_quasilinearize_lincons(itv_internal_t* intern, itv_lincons_t* lincons, itv_t* env, bool meet)
{ return ITVFUN(itv_quasilinearize_lincons)(intern,lincons,env,meet); }
  static inline bool itv_quasilinearize_lincons_array(itv_internal_t* intern, itv_lincons_array_t* tlincons, itv_t* env, bool meet)
{ return ITVFUN(itv_quasilinearize_lincons_array)(intern,tlincons,env,meet); }
static inline void itv_linearize_lincons_array(itv_internal_t* intern, itv_lincons_array_t* array, bool meet)
{ ITVFUN(itv_linearize_lincons_array)(intern,array,meet); }

/* IV. Evaluation of tree expressions  */
static inline void itv_eval_ap_texpr0(itv_internal_t* intern, itv_t itv, ap_texpr0_t* expr, itv_t* env)
{ ITVFUN(itv_eval_ap_texpr0)(intern,itv,expr,env); }

/* V. Linearisation of linear tree expressions. */
static inline bool itv_intlinearize_ap_texpr0_intlinear(itv_internal_t* intern, itv_linexpr_t* linexpr, ap_texpr0_t* expr)
{ return ITVFUN(itv_intlinearize_ap_texpr0_intlinear)(intern,linexpr,expr); }
static inline bool itv_intlinearize_ap_tcons0_intlinear(itv_internal_t* intern, itv_lincons_t* lincons, ap_tcons0_t* cons)
  { return ITVFUN(itv_intlinearize_ap_tcons0_intlinear)(intern,lincons,cons); }
static inline bool itv_intlinearize_ap_tcons0_array_intlinear(itv_internal_t* intern, itv_lincons_array_t* tlincons, ap_tcons0_array_t* tcons)
  { return ITVFUN(itv_intlinearize_ap_tcons0_array_intlinear)(intern,tlincons,tcons); }

/* VI. Linearisation of tree expressions. */
static inline bool itv_intlinearize_ap_texpr0(itv_internal_t* intern, itv_linexpr_t* linexpr, ap_texpr0_t* expr, itv_t* env, size_t intdim)
{ return ITVFUN(itv_intlinearize_ap_texpr0)(intern,linexpr,expr,env,intdim); }
static inline bool itv_intlinearize_ap_tcons0(itv_internal_t* intern, itv_lincons_t* lincons, ap_tcons0_t* cons, itv_t* env, size_t intdim)
  { return ITVFUN(itv_intlinearize_ap_tcons0)(intern,lincons,cons,env,intdim); }
static inline bool itv_intlinearize_ap_tcons0_array(itv_internal_t* intern, itv_lincons_array_t* tlincons, ap_tcons0_array_t* tcons, itv_t* env, size_t intdim)
  { return ITVFUN(itv_intlinearize_ap_tcons0_array)(intern,tlincons,tcons,env,intdim); }

/* VII. Backward evaluation of tree expressions. */
static inline bool itv_meet_ap_tcons0_array(itv_internal_t* intern, ap_tcons0_array_t* array, itv_t* env, size_t intdim, int max_iter)
{ return ITVFUN(itv_meet_ap_tcons0_array)(intern,array,env,intdim,max_iter); }
static inline bool itv_subst_ap_texpr0_array(itv_internal_t* intern, itv_t* res, itv_t* arg, ap_dim_t* dim, ap_texpr0_t** array, size_t size, size_t intdim, size_t realdim, int max_iter)
{ return ITVFUN(itv_subst_ap_texpr0_array)(intern,res,arg,dim,array,size,intdim,realdim,max_iter); }


#ifdef __cplusplus
}
#endif

#endif
