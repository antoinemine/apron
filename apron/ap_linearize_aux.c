/* ************************************************************************* */
/* ap_linearize_aux.c: auxiliary functions for (quasi)linearisation */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* Auxiliary module to ap_linearize, which contains functions depending of the
   number representation */

#include "ap_linearize.h"
#include "ap_linearize_aux.h"

#include "num.h"
#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

static
bool intlinearize_alloc(ap_manager_t* man, ap_abstract0_t* abs,
			itv_internal_t* intern,
			itv_t** penv, ap_dimension_t* pdim)
{
  size_t size;
  ap_interval_t** tinterval = ap_abstract0_to_box(man,abs);
  bool exact = man->result.flag_exact;

  // might not be true if bottom checking is lazy (algo < 0)
  // assert(!ap_abstract0_is_bottom(man,abs));
    
  *pdim = ap_abstract0_dimension(man,abs);
  size = pdim->intdim+pdim->realdim;
  exact = itv_array_set_ap_interval_array(intern,penv,tinterval,size)
    && exact;
  ap_interval_array_free(tinterval,size);
  return exact;
}

static inline
void intlinearize_free(itv_internal_t* intern,
		       itv_t* env, ap_dimension_t dim)
{
  if (env) itv_array_free(env,dim.intdim+dim.realdim);
  itv_internal_clear(intern);
}

/* ********************************************************************** */
/* I. Evaluation of interval linear expressions */
/* ********************************************************************** */

ap_interval_t*
ITVFUN(ap_eval_linexpr0)(ap_manager_t* man,
			 ap_abstract0_t* abs, ap_linexpr0_t* expr,
			 bool* pexact)
{
  bool exact;
  itv_internal_t* intern;
  ap_dimension_t dim;
  ap_interval_t** aenv;
  itv_t* env;
  itv_t res;
  ap_interval_t* r = ap_interval_alloc();
  if (pexact) *pexact = true;
  aenv = ap_abstract0_to_box(man,abs);
  if (!aenv) {
    ap_interval_set_bottom(r);
    return r;
  }
  dim = ap_abstract0_dimension(man,abs);
  intern = itv_internal_alloc();
  itv_init(res);
  itv_array_set_ap_interval_array(intern,&env,aenv,dim.intdim+dim.realdim);
  exact = itv_eval_ap_linexpr0(intern,res,expr,env);
  if (pexact) *pexact = exact;
  ap_interval_set_itv(intern,r,res);
  itv_internal_free(intern);
  ap_interval_array_free(aenv,dim.intdim+dim.realdim);
  itv_array_free(env,dim.intdim+dim.realdim);
  itv_clear(res);
  return r;
}

/* ********************************************************************** */
/* II. Quasilinearization of interval linear expressions */
/* ********************************************************************** */

static
bool quasilinearize_alloc(ap_manager_t* man, ap_abstract0_t* abs,
			  itv_internal_t** pintern,
			  itv_lincons_t* plincons,
			  itv_t** penv, ap_dimension_t* pdim)
{
  bool exact,exact2;
  ap_interval_t** tinterval;
  size_t size;

  assert(!ap_abstract0_is_bottom(man,abs));
  exact = true;

  tinterval = ap_abstract0_to_box(man,abs);
  exact = man->result.flag_exact && exact;

  *pdim = ap_abstract0_dimension(man,abs);
  size = pdim->intdim+pdim->realdim;
  *pintern = itv_internal_alloc();
  exact = itv_array_set_ap_interval_array(*pintern,penv,tinterval,size)
    && exact;
  ap_interval_array_free(tinterval,size);
  itv_lincons_init(plincons);
  return exact;
}

static inline
void quasilinearize_free(itv_internal_t* intern,
			 itv_lincons_t* lincons,
			 itv_t* env, ap_dimension_t dim)
{
  itv_lincons_clear(lincons);
  itv_array_free(env,dim.intdim+dim.realdim);
  itv_internal_free(intern);
}

/* Evaluate a interval linear expression on the abstract
   value such as to transform it into a quasilinear expression.

   discr allows to choose the type of scalars used for computations and for the
   result.  pexact is a pointer to a Boolean, which is set to true if all
   the conversions and computations were exact.
*/

ap_linexpr0_t*
ITVFUN(ap_quasilinearize_linexpr0)(ap_manager_t* man,
				   ap_abstract0_t* abs,
				   ap_linexpr0_t* linexpr0,
				   bool* pexact)
{
  ap_dimension_t dim;
  ap_linexpr0_t* rlinexpr0;
  itv_internal_t* intern;
  itv_lincons_t lincons;
  itv_t* env;
  bool exact,exact2;

  exact = quasilinearize_alloc(man,abs,&intern,&lincons,&env,&dim);
  exact = itv_linexpr_set_ap_linexpr0(intern,&lincons.linexpr,linexpr0)
    && exact;
  exact = itv_quasilinearize_linexpr(intern,&lincons.linexpr,env,false)
    && exact;
  rlinexpr0 = NULL;
  ap_linexpr0_set_itv_linexpr(intern,&rlinexpr0,&lincons.linexpr);
  quasilinearize_free(intern,&lincons,env,dim);
  *pexact = exact;
  return rlinexpr0;
}

/* Same for ap_lincons0_t */
ap_lincons0_t
ITVFUN(ap_quasilinearize_lincons0)(ap_manager_t* man,
				   ap_abstract0_t* abs, ap_lincons0_t* lincons0,
				   bool* pexact, bool meet)
{
  ap_dimension_t dim;
  ap_lincons0_t rlincons0;
  itv_internal_t* intern;
  itv_lincons_t lincons;
  itv_t* env;
  bool exact,exact2;

  exact = quasilinearize_alloc(man,abs,&intern,&lincons,&env,&dim);
  exact = itv_lincons_set_ap_lincons0(intern,&lincons,lincons0)
    && exact;
  exact = itv_quasilinearize_lincons(intern,&lincons,env,meet)
    && exact;
  rlincons0 = ap_lincons0_make(AP_CONS_EQ,
			       ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0),
			       NULL);
  ap_lincons0_set_itv_lincons(intern,&rlincons0,&lincons);
  quasilinearize_free(intern,&lincons,env,dim);
  *pexact = exact;
  return rlincons0;
}

/* Same for arrays of ap_linexpr0_t */
ap_linexpr0_t**
ITVFUN(ap_quasilinearize_linexpr0_array)(ap_manager_t* man,
					 ap_abstract0_t* abs, ap_linexpr0_t** texpr, size_t size,
					 bool* pexact)
{
  ap_dimension_t dim;
  ap_linexpr0_t** tab;
  itv_internal_t* intern;
  itv_lincons_t lincons;
  itv_t* env;
  bool exact,exact2;
  size_t i;

  exact = quasilinearize_alloc(man,abs,&intern,&lincons,&env,&dim);
  tab = (ap_linexpr0_t**)malloc(size*sizeof(ap_linexpr0_t*));
  for (i=0; i<size; i++){
    exact = itv_linexpr_set_ap_linexpr0(intern,&lincons.linexpr,texpr[i])
      && exact;
    exact = itv_quasilinearize_linexpr(intern,&lincons.linexpr,env,false)
      && exact;
    tab[i] = NULL;
    ap_linexpr0_set_itv_linexpr(intern,&tab[i],&lincons.linexpr);
  }
  quasilinearize_free(intern,&lincons,env,dim);
  *pexact = exact;
  return tab;
}

/* Same for ap_lincons0_array_t */
ap_lincons0_array_t
ITVFUN(ap_quasilinearize_lincons0_array)(ap_manager_t* man,
					 ap_abstract0_t* abs, ap_lincons0_array_t* array,
					 bool* pexact, bool linearize, bool meet)
{
  itv_internal_t* intern;
  itv_lincons_t lincons;
  itv_t* env;
  ap_dimension_t dim;
  bool exact;
  itv_lincons_array_t tlincons;
  ap_lincons0_array_t res;
  size_t i;

  exact = quasilinearize_alloc(man,abs,&intern,&lincons,&env,&dim);
  itv_lincons_array_init(&tlincons,array->size);
  exact = itv_lincons_array_set_ap_lincons0_array(intern,&tlincons,array) && exact;
  exact = itv_quasilinearize_lincons_array(intern,&tlincons,env,meet);
  if (linearize) 
    itv_linearize_lincons_array(intern,&tlincons,meet);
  res = ap_lincons0_array_make(tlincons.size);
  for (i=0;i<tlincons.size;i++){
    ap_lincons0_set_itv_lincons(intern,&res.p[i],&tlincons.p[i]);
  }
  itv_lincons_array_clear(&tlincons);
  quasilinearize_free(intern,&lincons,env,dim);
  return res;
}

/* ********************************************************************** */
/* III. Evaluation of tree expressions */
/* ********************************************************************** */

ap_interval_t*
ITVFUN(ap_eval_texpr0)(ap_manager_t* man,
		       ap_abstract0_t* abs, ap_texpr0_t* expr,
		       bool* pexact)
{
  itv_internal_t intern;
  itv_t res;
  itv_t* env = NULL;
  ap_dimension_t dim = {0,0};
  ap_interval_t* r = ap_interval_alloc();

  if (pexact) *pexact = false;

  itv_internal_init(&intern);
  itv_init(res);
  if (ap_texpr0_is_interval_cst(expr)){
    itv_eval_ap_texpr0(&intern,res,expr,NULL);
  }
  else {
    intlinearize_alloc(man,abs,&intern,&env,&dim);
    itv_eval_ap_texpr0(&intern,res,expr,env);
  }
  ap_interval_set_itv(&intern,r,res);
  itv_clear(res);
  intlinearize_free(&intern,env,dim);
  return r;
}

/* ********************************************************************** */
/* IV. Interval Linearization of linear tree expressions */
/* ********************************************************************** */

/* Linearize a tree expression that is (syntaxically) interval linear with
   exact arithmetic.

   Compared to ap_intlinearize_texpr0() function below, this functions does
   not require a bounding box for dimensions.

   If the precondition is violated, returns NULL.
*/

ap_linexpr0_t*
ITVFUN(ap_intlinearize_texpr0_intlinear)(ap_manager_t* man,
					 ap_texpr0_t* expr)
{
  bool exc;
  itv_internal_t* intern;
  ap_linexpr0_t* res;
  itv_linexpr_t linexpr;

  intern = itv_internal_alloc();
  itv_linexpr_init(&linexpr,0);
  exc = itv_intlinearize_ap_texpr0_intlinear(intern,&linexpr,expr);
  itv_internal_free(intern);
  if (exc){
    itv_linexpr_clear(&linexpr);
    return NULL;
  }
  ap_linexpr0_set_itv_linexpr(intern,&res,&linexpr);
  itv_linexpr_clear(&linexpr);
  return res;
}

/* ********************************************************************** */
/* V. Interval Linearization of tree expressions */
/* ********************************************************************** */

ap_linexpr0_t*
ITVFUN(ap_intlinearize_texpr0)(ap_manager_t* man,
			       ap_abstract0_t* abs, ap_texpr0_t* expr,
			       bool* pexact, bool quasilinearize)
{
  itv_internal_t intern;
  itv_t* env = NULL;
  ap_dimension_t dim = {0,0};
  itv_linexpr_t linexpr;
  ap_linexpr0_t* res = NULL;

  if (pexact) *pexact = false;
  itv_internal_init(&intern);
  itv_linexpr_init(&linexpr,0);
  if (ap_texpr0_is_interval_linear(expr)){
    itv_intlinearize_ap_texpr0_intlinear(&intern, &linexpr, expr);
  }
  else {
    intlinearize_alloc(man,abs,&intern,&env,&dim);
    itv_intlinearize_ap_texpr0(&intern, &linexpr, expr, env, 
			       dim.intdim);
  }
  if (quasilinearize && !itv_linexpr_is_quasilinear(&linexpr)){
    if (!env) intlinearize_alloc(man,abs,&intern,&env,&dim);
    itv_quasilinearize_linexpr(&intern,&linexpr,env,false);
  }
  ap_linexpr0_set_itv_linexpr(&intern,&res,&linexpr);
  itv_linexpr_clear(&linexpr);
  intlinearize_free(&intern,env,dim);
  return res;
}

ap_linexpr0_t**
ITVFUN(ap_intlinearize_texpr0_array)(ap_manager_t* man,
				     ap_abstract0_t* abs, ap_texpr0_t** texpr0, size_t size,
				     bool* pexact, bool quasilinearize)
{
  itv_internal_t intern;
  ap_dimension_t dim = {0,0};
  itv_t* env = NULL;
  itv_linexpr_t linexpr;
  ap_linexpr0_t** res;
  size_t i;

  if (pexact) *pexact = false;
  itv_internal_init(&intern);
  itv_linexpr_init(&linexpr,0);
  res = malloc(size*sizeof(ap_linexpr0_t*));
  for (i=0; i<size; i++){
    if (ap_texpr0_is_interval_linear(texpr0[i])){
      itv_intlinearize_ap_texpr0_intlinear(&intern, &linexpr, texpr0[i]);
    }
    else {
      if (!env) intlinearize_alloc(man,abs,&intern,&env,&dim);
      itv_intlinearize_ap_texpr0(&intern, &linexpr, texpr0[i], env, 
				 dim.intdim);
    }
    if (quasilinearize && !itv_linexpr_is_quasilinear(&linexpr)){
      if (!env) intlinearize_alloc(man,abs,&intern,&env,&dim);
      itv_quasilinearize_linexpr(&intern,&linexpr,env,false);
    }
    res[i] = NULL;
    ap_linexpr0_set_itv_linexpr(&intern,&res[i],&linexpr);
  }
  itv_linexpr_clear(&linexpr);
  intlinearize_free(&intern,env,dim);
  return res;
}

ap_lincons0_t
ITVFUN(ap_intlinearize_tcons0)(ap_manager_t* man,
			       ap_abstract0_t* abs, ap_tcons0_t* cons,
			       bool* pexact, bool quasilinearize, bool meet)
{
  itv_internal_t intern;
  ap_dimension_t dim = {0,0};
  itv_t* env = NULL;
  itv_lincons_t lincons;
  ap_lincons0_t res;

  if (pexact) *pexact = false;
  itv_internal_init(&intern);
  itv_lincons_init(&lincons);
  if (ap_tcons0_is_interval_linear(cons)){
    itv_intlinearize_ap_tcons0_intlinear(&intern, &lincons, cons);
  }
  else {
    intlinearize_alloc(man,abs,&intern,&env,&dim);
    itv_intlinearize_ap_tcons0(&intern,
			       &lincons, cons, env,
			       dim.intdim);
  }
  if (quasilinearize && !itv_lincons_is_quasilinear(&lincons)){
    if (!env) intlinearize_alloc(man,abs,&intern,&env,&dim);
    itv_quasilinearize_lincons(&intern,&lincons,env,meet);
  }
  res.linexpr0 = NULL;
  res.scalar = NULL;
  ap_lincons0_set_itv_lincons(&intern,&res,&lincons);
  itv_lincons_clear(&lincons);
  intlinearize_free(&intern,env,dim);
  return res;
}

ap_lincons0_array_t
ITVFUN(ap_intlinearize_tcons0_array)(ap_manager_t* man,
				     ap_abstract0_t* abs, ap_tcons0_array_t* array,
				     bool* pexact, 
				     ap_linexpr_type_t linearize, bool meet,
				     bool boxize, size_t kmax, bool intervalonly)
{
  itv_internal_t intern;
  ap_dimension_t dim = {0,0};
  itv_t* env = NULL;
  itv_lincons_array_t tlincons;
  ap_lincons0_array_t res;
  size_t i;
  bool change = false;
  bool empty = false;
  bool* tchange = NULL;

  if (pexact) *pexact = false;
  itv_internal_init(&intern);
  intlinearize_alloc(man,abs,&intern,&env,&dim);
  itv_lincons_array_init(&tlincons,array->size);
  itv_intlinearize_ap_tcons0_array(&intern,&tlincons,
				   array,env, dim.intdim);
  if (tlincons.size==1 && 
      tlincons.p[0].linexpr.size==0)
    goto ap_intlinearize_tcons0_array_exit;
  if (meet && boxize && 
      (intervalonly ? !itv_lincons_array_is_quasilinear(&tlincons) : true)){
    tchange = malloc((dim.intdim+dim.realdim)*2);
    for (i=0;i<(dim.intdim+dim.realdim)*2;i++) tchange[i]=false;
    change = itv_boxize_lincons_array(&intern,env,tchange,&tlincons,env,dim.intdim,kmax,intervalonly,&empty);
  }
  switch(linearize){
  case AP_LINEXPR_INTLINEAR:
    break;
  case AP_LINEXPR_QUASILINEAR:
    itv_quasilinearize_lincons_array(&intern,&tlincons,env,meet);
    break;
  case AP_LINEXPR_LINEAR:
    itv_quasilinearize_lincons_array(&intern,&tlincons,env,meet);
    itv_linearize_lincons_array(&intern,&tlincons,meet);
    break;
  }
  if (tlincons.size==1 && 
      tlincons.p[0].linexpr.size==0 &&
      itv_eval_cstlincons(&intern,&tlincons.p[0])==tbool_false)
    goto ap_intlinearize_tcons0_array_exit;
  
  if (change){
    if (empty){
      itv_lincons_array_reinit(&tlincons,1);
      itv_lincons_set_bool(&tlincons.p[0],false);
      goto ap_intlinearize_tcons0_array_exit;
    }
    size_t size = tlincons.size;
    for (i=0;i<dim.intdim+dim.realdim; i++){
      if (tchange[2*i] || tchange[2*i+1]){
	/* we add a new row and prepare it */
	if (size>=tlincons.size) 
	  itv_lincons_array_reinit(&tlincons,1+(5*tlincons.size)/4);
	itv_linexpr_reinit(&tlincons.p[size].linexpr,1);
	tlincons.p[size].linexpr.linterm[0].dim = i;
	tlincons.p[size].linexpr.linterm[0].equality = true;
	tlincons.p[size].linexpr.equality = true;
      }	
      if ((tchange[2*i] || tchange[2*i+1]) && itv_is_point(&intern,env[i])){
	/* We have a point */
	tlincons.p[size].constyp = AP_CONS_EQ;
	itv_set_int(tlincons.p[size].linexpr.linterm[0].itv,-1);
	itv_set(tlincons.p[size].linexpr.cst, env[i]);
	size++;
      }
      else {
	if (tchange[2*i]){
	  /* inf bound */
	  assert(!bound_infty(env[i]->inf));
	  tlincons.p[size].constyp = AP_CONS_SUPEQ;
	  itv_set_int(tlincons.p[size].linexpr.linterm[0].itv,1);
	  itv_set_num(tlincons.p[size].linexpr.cst,bound_numref(env[i]->inf));
	  size ++;
	}
	if (tchange[2*i+1]){
	  /* sup bound */
	  if (tchange[2*i]){
	    /* we add a new row and prepare it */
	    if (size>=tlincons.size) 
	      itv_lincons_array_reinit(&tlincons,1+(5*tlincons.size)/4);
	    itv_linexpr_reinit(&tlincons.p[size].linexpr,1);
	    tlincons.p[size].linexpr.linterm[0].dim = i;
	    tlincons.p[size].linexpr.linterm[0].equality = true;
	    tlincons.p[size].linexpr.equality = true;
	  }	    
	  assert(!bound_infty(env[i]->sup));
	  tlincons.p[size].constyp = AP_CONS_SUPEQ;
	  itv_set_int(tlincons.p[size].linexpr.linterm[0].itv,-1);
	  itv_set_num(tlincons.p[size].linexpr.cst, bound_numref(env[i]->sup));
	  size++;
	}
      }
    }
    itv_lincons_array_reinit(&tlincons,size);
  }
 ap_intlinearize_tcons0_array_exit:
  res = ap_lincons0_array_make(tlincons.size);
  for (i=0;i<tlincons.size;i++){
    ap_lincons0_set_itv_lincons(&intern,&res.p[i],&tlincons.p[i]);
  }
  if (tchange) free(tchange);
  itv_lincons_array_clear(&tlincons);
  intlinearize_free(&intern,env,dim);
  return res;
}
