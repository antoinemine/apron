/* ************************************************************************* */
/* ap_abstract1.c: level 1 of interface */
/* ************************************************************************* */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_manager.h"
#include "ap_abstract0.h"
#include "ap_expr0.h"
#include "ap_expr1.h"
#include "ap_abstract1.h"

/* Normal constructor */
static inline ap_abstract1_t ap_abstract1_cons(ap_abstract0_t* a0, ap_environment_t* env)
{
  ap_abstract1_t res;
  res.abstract0 = a0;
  res.env = ap_environment_copy(env);
  return res;
}

/* Special constructor:
   Build a new abstract value level 1 from the old one and a new value level 0,
   depending on destructive. The environment is supposed unchanged. 
*/
static
ap_abstract1_t ap_abstract1_consres(bool destructive, ap_abstract1_t* a, ap_abstract0_t* value)
{
  ap_abstract1_t res;
  if (destructive){
    if (value==a->abstract0)
      res = *a;
    else {
      res.abstract0 = value;
      res.env = a->env;
    }
  }
  else {
    res = ap_abstract1_cons(value,a->env);
  }
  return res;
}
/* Special constructor 2:

   Build a new abstract value level 1 from the old one, a new
   value level 0 and a new environment, depending on
   destructive. The environment is supposed unchanged.
*/
static
ap_abstract1_t ap_abstract1_consres2(bool destructive, ap_abstract1_t* a, 
				     ap_abstract0_t* value, 
				     ap_environment_t* env)
{
  ap_abstract1_t res;
  if (destructive){
    if (value==a->abstract0 && env==a->env)
      res = *a;
    else {
      res.abstract0 = value;
      res.env = env;
      ap_environment_free(a->env);
    }
  }
  else {
      res.abstract0 = value;
      res.env = env;
  }
  return res;
}

void ap_abstract1_raise_invalid_var(ap_manager_t* man,
				    ap_funid_t funid,
				    ap_var_t var)
{
  char msg[160];
  char* b = ap_var_operations->to_string(var);
  snprintf(msg,159,"unknown variable %s in the environment",b);
  free(b);
  ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,msg);
}
void ap_abstract1_raise_invalid_linexpr(ap_manager_t* man,
					ap_funid_t funid)
{
  ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,"\
Linear expression not defined on a (sub)environment of the abstract value\
");
}

bool ap_abstract1_check_env2(ap_funid_t funid,
			     ap_manager_t* man,
			     const ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  if (ap_environment_is_eq(a1->env,a2->env))
    return true;
  else {
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       funid,
			       "The two abstract values have different environments");
    return false;
  }
}
bool ap_abstract1_check_env_array(ap_funid_t funid,ap_manager_t* man,
				  const ap_abstract1_t* tab, size_t size)
{
  bool res;
  if (size==0){
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       funid,"array of abstract values of size 0");
    res = false;
  }
  else {
    size_t i;
    ap_environment_t* env0 = tab[0].env;
    for (i=1;i<size; i++){
      if (!ap_environment_is_eq(env0,tab[i].env)){
	res = false;
	break;

	char str[160];
	snprintf(str,159,"The %dth abstract value of the array has not \
the same environement as the 0th abstract value\
",i);
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   funid,str);
	return false;
      }
    }
    res = true;
  }
  return res;
}
/* Array of abstract values */
bool ap_abstract1_checkman_array(ap_funid_t funid,
				 ap_manager_t* man, const ap_abstract1_t* tab, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    if (man != tab[i].abstract0->man){
      char str[160];
      snprintf(str,159,"\
The %dth abstract value of the array is of type %s and not of the type %s expected by the manager\
",
	       i,tab[i].abstract0->man->library,man->library);
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 funid,
				 str);
      return false;
    }
  }
  return true;
}

void box1_fprint(FILE* stream, const box1_t* box)
{
  size_t i;
  fprintf(stream,"Box1: (%d,%d)\n",
	  box->env->intdim,box->env->realdim);
  for (i=0; i<box->env->intdim+box->env->realdim;i++){
    ap_var_t var = ap_environment_var_of_dim(box->env,i);
    char* name = ap_var_operations->to_string(var);
    fprintf(stream,"  %s\tin",name);
    free(name);
    ap_interval_fprint(stream,box->p[i]);
    fprintf(stream,"\n");
  }
}

void box1_clear(box1_t* box)
{
  if (box->env==NULL){
    fprintf(stderr,"abstract1.c: box_clear: either non initialized or already deallocated box !\n");
    abort();
  }
  ap_interval_array_free(box->p,box->env->intdim+box->env->realdim);
  ap_environment_free(box->env);
  box->p=NULL;
  box->env=NULL;
}

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

/* Return a copy of an abstract value, on
   which destructive update does not affect the initial value. */
ap_abstract1_t ap_abstract1_copy(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract1_cons(ap_abstract0_copy(man,a->abstract0),a->env);
}

/* Free all the memory used by the abstract value */
void ap_abstract1_clear(ap_manager_t* man, ap_abstract1_t* a)
{
  ap_abstract0_free(man,a->abstract0);
  ap_environment_free(a->env);
  a->abstract0 = NULL;
  a->env = NULL;
}

/* Return the abstract size of an abstract value (see ap_manager_t) */
size_t ap_abstract1_size(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract0_size(man,a->abstract0);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */


/* Minimize the size of the representation of a.
   This may result in a later recomputation of internal information.
*/
void ap_abstract1_minimize(ap_manager_t* man, const ap_abstract1_t* a){
  ap_abstract0_minimize(man,a->abstract0);
}

/* Put the abstract value in canonical form. (not yet clear definition) */
void ap_abstract1_canonicalize(ap_manager_t* man, const ap_abstract1_t* a){
  ap_abstract0_canonicalize(man,a->abstract0);
}

/* Perform some transformation on the abstract value, guided by the
   field algorithm.

   The transformation may lose information.  The argument "algorithm"
   overrides the field algorithm of the structure of type foption_t
   associated to ap_abstract1_approximate (commodity feature). */
void ap_abstract1_approximate(ap_manager_t* man, ap_abstract1_t* a, int algorithm){
  ap_abstract0_approximate(man,a->abstract0, algorithm);
}

tbool_t ap_abstract1_is_minimal(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract0_is_minimal(man,a->abstract0);
}
tbool_t ap_abstract1_is_canonical(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract0_is_canonical(man,a->abstract0);
}


/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

/* Print the abstract value */
void ap_abstract1_fprint(FILE* stream,
			 ap_manager_t* man,
			 const ap_abstract1_t* a)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(a->env);
  ap_abstract0_fprint(stream,man, a->abstract0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* Print the difference between a1 (old value) and a2 (new value), using
   function name_of_dim to name dimensions.  The meaning of difference is
   library dependent. */
void ap_abstract1_fprintdiff(FILE* stream,
			  ap_manager_t* man,
			  const ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  ap_environment_name_of_dim_t* name_of_dim;
  if (!ap_environment_is_eq(a1->env,a2->env)){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_FPRINTDIFF,
			    "the 2 abstract values are not defined on the same environement");
    fprintf(stream,"unknown diff\n");
  }
  name_of_dim = ap_environment_name_of_dim_alloc(a1->env);
  ap_abstract0_fprintdiff(stream,man,a1->abstract0, a2->abstract0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* Dump the internal representation of an abstract value,
   for debugging purposes */
void ap_abstract1_fdump(FILE* stream, ap_manager_t* man, const ap_abstract1_t* a)
{
  fprintf(stream, "abstract value of level 1:\n");
  ap_environment_fdump(stream,a->env);
  ap_abstract0_fdump(stream,man,a->abstract0);
}


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

/* DOIT-ON SÉRIALISER L'ENVIRONNEMENT ? */

/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

ap_membuf_t ap_abstract1_serialize_raw(ap_manager_t* man, const ap_abstract1_t* a){
  ap_membuf_t membuf;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"");
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
ap_abstract1_t ap_abstract1_deserialize_raw(ap_manager_t* man, void* ptr){
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,"");
  return ap_abstract1_top(man,ap_environment_make_empty());
}


/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* Create a bottom (empty) value */
ap_abstract1_t ap_abstract1_bottom(ap_manager_t* man, ap_environment_t* env)
{
  return ap_abstract1_cons(ap_abstract0_bottom(man,env->intdim,env->realdim),env);
}

/* Create a top (universe) value */
ap_abstract1_t ap_abstract1_top(ap_manager_t* man, ap_environment_t* env)
{
  return ap_abstract1_cons(ap_abstract0_top(man,env->intdim,env->realdim),env);
}

/* Abstract an hypercube defined by the arrays tvar and tinterval,
   satisfying: forall i, tvar[i] in tinterval[i].

   If no inclusion is specified for a variable in the environement, its value
   is no constrained in the resulting abstract value.
*/
ap_abstract1_t ap_abstract1_of_box(ap_manager_t* man,
			     ap_environment_t* env,
			     ap_var_t* tvar,
			     ap_interval_t** tinterval,
			     size_t size)
{
  ap_abstract1_t a;
  size_t i;

  ap_interval_t** itv = ap_interval_array_alloc(env->intdim+env->realdim);
  for (i=0; i<env->intdim+env->realdim; i++){
    ap_interval_set_top(itv[i]);
  }
  for (i=0; i<size; i++){
    ap_dim_t dim = ap_environment_dim_of_var(env,tvar[i]);
    if (dim==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,AP_FUNID_OF_BOX,tvar[i]);
      ap_interval_array_free(itv,size);
      return ap_abstract1_top(man,env);
    }
    ap_interval_set(itv[dim],tinterval[i]);
  }
  a = ap_abstract1_cons(ap_abstract0_of_box(man,env->intdim,env->realdim,(const ap_interval_t**)itv),env);
  ap_interval_array_free(itv,env->intdim+env->realdim);
  return a;
}

/* Abstract a convex polyhedra defined by the array of linear constraints
   of size size */
ap_abstract1_t ap_abstract1_of_lincons_array(ap_manager_t* man,
					     ap_environment_t* env,
					     const ap_lincons1_array_t* array)
{
  ap_abstract0_t* value;
  ap_abstract1_t a;
  ap_lincons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (env==NULL)
    env = array->env;
  
  if (ap_environment_is_eq(env,array->env)){
    dimchange = NULL;
    array0 = array->lincons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_OF_LINCONS_ARRAY,
				 "environment of input constraints is not a subset of the requested environment");
      return ap_abstract1_top(man,env);
    }
    array0 = ap_lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  value = ap_abstract0_of_lincons_array(man,
					env->intdim,env->realdim,
					&array0);
  a = ap_abstract1_cons(value,env);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lincons0_array_clear(&array0);
  }
  return a;
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

const ap_environment_t* ap_abstract1_environment(ap_manager_t* man, const ap_abstract1_t* a){
  return a->env;
}

const ap_abstract0_t* ap_abstract1_abstract0(ap_manager_t* man, const ap_abstract1_t* a){
  return a->abstract0;
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

/* If any of the following functions returns tbool_top, this means that
   an exception has occured, or that the exact computation was
   considered too expensive to be performed (according to the options).
   The flag exact and best should be cleared in such a case. */

tbool_t ap_abstract1_is_bottom(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract0_is_bottom(man,a->abstract0);
}
tbool_t ap_abstract1_is_top(ap_manager_t* man, const ap_abstract1_t* a){
  return ap_abstract0_is_top(man,a->abstract0);
}

/* inclusion check */
tbool_t ap_abstract1_is_leq(ap_manager_t* man, const ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  if (ap_abstract1_check_env2(AP_FUNID_IS_LEQ,man,a1,a2))
    return ap_abstract0_is_leq(man,a1->abstract0,a2->abstract0);
  else
    return tbool_top;
}
/* equality check */
tbool_t ap_abstract1_is_eq(ap_manager_t* man, const ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  if (ap_abstract1_check_env2(AP_FUNID_IS_EQ,man,a1,a2))
    return ap_abstract0_is_eq(man,a1->abstract0,a2->abstract0);
  else
    return tbool_top;
}
/* does the abstract value satisfy the linear constraint ? */
tbool_t ap_abstract1_sat_lincons(ap_manager_t* man, const ap_abstract1_t* a, const ap_lincons1_t* cons)
{
  tbool_t res;

  if (ap_environment_is_eq(a->env,cons->env)){
    res = ap_abstract0_sat_lincons(man,a->abstract0,&cons->lincons0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_lincons0_t cons0;

    dimchange = ap_environment_dimchange(cons->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_SAT_LINCONS,
			      "the environment of the constraint is not a subset of the environment of the abstract value");
      res = tbool_top;
      return res;
    }
    cons0 = ap_lincons0_add_dimensions(&cons->lincons0,dimchange);
    res = ap_abstract0_sat_lincons(man,a->abstract0,&cons0);
    ap_dimchange_free(dimchange);
    ap_lincons0_clear(&cons0);
  }
  return res;
}

/* Is the dimension included in the interval in the abstract value ? */
tbool_t ap_abstract1_sat_interval(ap_manager_t* man, const ap_abstract1_t* a,
			       ap_var_t var, const ap_interval_t* interval)
{
  ap_dim_t dim;
  tbool_t res;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_SAT_INTERVAL,var);
    res = tbool_top;
    return res;
  }
  return ap_abstract0_sat_interval(man, a->abstract0, dim, interval);
}

/* Is the dimension included in the interval in the abstract value ? */
tbool_t ap_abstract1_is_dimension_unconstrained(ap_manager_t* man, const ap_abstract1_t* a,
					     ap_var_t var)
{
  ap_dim_t dim;
  tbool_t res;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_IS_DIMENSION_UNCONSTRAINED,var);
    res = tbool_top;
    return res;
  }
  return ap_abstract0_is_dimension_unconstrained(man, a->abstract0, dim);
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

/* Returns the interval taken by a linear expression
   over the abstract value */
ap_interval_t* ap_abstract1_bound_linexpr(ap_manager_t* man,
				    const ap_abstract1_t* a, const ap_linexpr1_t* expr)
{
  ap_interval_t* res;

  if (ap_environment_is_eq(a->env,expr->env)){
    res = ap_abstract0_bound_linexpr(man,a->abstract0,expr->linexpr0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_linexpr0_t* expr0;

    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_BOUND_LINEXPR,
			      "the environment of the linear expression is not a subset of the environment of the abstract value");
      res = ap_interval_alloc();
      ap_interval_reinit(res,man->option.scalar_discr);
      ap_interval_set_top(res);
      return res;
    }
    expr0 = ap_linexpr0_add_dimensions(expr->linexpr0,dimchange);
    res = ap_abstract0_bound_linexpr(man,a->abstract0,expr0);
    ap_dimchange_free(dimchange);
    ap_linexpr0_free(expr0);
  }
  return res;
}

/* Returns the interval taken by the dimension over the abstract
   value */
ap_interval_t* ap_abstract1_bound_dimension(ap_manager_t* man,
				      const ap_abstract1_t* a, ap_var_t var)
{
  ap_dim_t dim;
  ap_interval_t* res;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_BOUND_DIMENSION,var);
    res = ap_interval_alloc();
    ap_interval_reinit(res,man->option.scalar_discr);
    ap_interval_set_top(res);
    return res;
  }
  return ap_abstract0_bound_dimension(man, a->abstract0, dim);
}

/* Converts an abstract value to a polyhedra (conjunction of
   linear constraints). */
ap_lincons1_array_t ap_abstract1_to_lincons_array(ap_manager_t* man, const ap_abstract1_t* a)
{
  ap_lincons1_array_t array;

  array.lincons0_array = ap_abstract0_to_lincons_array(man,a->abstract0);
  array.env = ap_environment_copy(a->env);
  return array;
}

/* Converts an abstract value to an interval/hypercube.  The size
   of the resulting array is ap_abstract1_dimension(man,a).  This
   function can be reimplemented by using ap_abstract1_bound_linexpr
   */
box1_t ap_abstract1_to_box(ap_manager_t* man, const ap_abstract1_t* a)
{
  box1_t box;

  box.p = ap_abstract0_to_box(man,a->abstract0);
  box.env = ap_environment_copy(a->env);
  return box;
}

/* Converts an abstract value to a system of generators. */
ap_generator1_array_t ap_abstract1_to_generator_array(ap_manager_t* man, const ap_abstract1_t* a)
{
  ap_generator1_array_t array;

  array.generator0_array = ap_abstract0_to_generator_array(man,a->abstract0);
  array.env = ap_environment_copy(a->env);
  return array;
}

/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

ap_abstract1_t ap_abstract1_meetjoin(ap_funid_t funid, ap_manager_t* man, bool destructive, ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  ap_abstract1_t res;
  if (ap_abstract1_check_env2(funid,man,a1,a2)){
    ap_abstract0_t* value = ap_abstract0_meetjoin(funid,man,destructive,a1->abstract0,a2->abstract0);
    res = ap_abstract1_consres(destructive, a1, value);
  }
  else {
    res = ap_abstract1_top(man,a1->env);
    if (destructive) ap_abstract1_clear(man,a1);
  }
  return res;
}
ap_abstract1_t ap_abstract1_meet(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, const ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_MEET,man,destructive,a1,a2);
}
ap_abstract1_t ap_abstract1_join(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, const ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_JOIN,man,destructive,a1,a2);
}
ap_abstract1_t ap_abstract1_meetjoin_array(ap_funid_t funid, ap_manager_t* man, const ap_abstract1_t* tab, size_t size)
{
  ap_abstract1_t res;
  if (ap_abstract1_checkman_array(funid,man,tab,size) &&
      ap_abstract1_check_env_array(funid,man,tab,size)){
    size_t i;
     ap_abstract0_t* res0;
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void** ntab = malloc(size*sizeof(void*));
    for (i=0;i<size;i++) ntab[i] = tab[i].abstract0->value;
    res0 = malloc(sizeof(ap_abstract0_t));
    res0->man = ap_manager_copy(man);
    res0->value = ptr(man,ntab,size);
    res.abstract0 = res0;
    res.env = ap_environment_copy(tab[0].env);
    free(ntab);
  }
  else {
    res = ap_abstract1_top(man, size==0 ? ap_environment_make_empty() : tab[0].env);
  }
  return res;
}
ap_abstract1_t ap_abstract1_meet_array(ap_manager_t* man, const ap_abstract1_t* tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_MEET_ARRAY,man,tab,size);
}
ap_abstract1_t ap_abstract1_join_array(ap_manager_t* man, const ap_abstract1_t* tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_JOIN_ARRAY,man,tab,size);
}

ap_abstract1_t ap_abstract1_meet_lincons_array(ap_manager_t* man,
					 bool destructive,
					 ap_abstract1_t* a, const ap_lincons1_array_t* array)
{
  ap_abstract1_t res;
  ap_lincons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    array0 = array->lincons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
			      AP_EXC_INVALID_ARGUMENT,
			      AP_FUNID_MEET_LINCONS_ARRAY,
			      "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? *a : ap_abstract1_copy(man,a);
      return res;
    }
    array0 = ap_lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  ap_abstract0_t* value = ap_abstract0_meet_lincons_array(man,destructive,a->abstract0,&array0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lincons0_array_clear(&array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

ap_abstract1_t ap_abstract1_add_ray_array(ap_manager_t* man,
				    bool destructive,
				    ap_abstract1_t* a, const ap_generator1_array_t* array)
{
  ap_abstract1_t res;
  ap_generator0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    array0 = array->generator0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
			      AP_EXC_INVALID_ARGUMENT,
			      AP_FUNID_ADD_RAY_ARRAY,
			      "environment of array of generators is not a subset of the environment of the abstract value");
      res = ap_abstract1_top(man,a->env);
      if (destructive) ap_abstract1_clear(man,a);
      return res;
    }
    array0 = ap_generator0_array_add_dimensions(&array->generator0_array,dimchange);
  }
  ap_abstract0_t* value = ap_abstract0_add_ray_array(man,destructive,a->abstract0,&array0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_generator0_array_clear(&array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

ap_abstract1_t ap_abstract1_asssub_linexpr(ap_funid_t funid, ap_manager_t* man,
				     bool destructive, ap_abstract1_t* a,
				     ap_var_t var, const ap_linexpr1_t* expr,
				     const ap_abstract1_t* dest)
{
  ap_dim_t dim;
  ap_abstract1_t res;
  ap_linexpr0_t* linexpr0;
  ap_dimchange_t* dimchange;

  if ( dest==NULL || ap_abstract1_check_env2(funid,man,a,dest)){
    dim = ap_environment_dim_of_var(a->env,var);
    if (dim==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,funid,var);
      goto ap_abstract1_asssub_linexpr_exit;
    }
    if (ap_environment_is_eq(a->env,expr->env)){
      dimchange = NULL;
      linexpr0 = expr->linexpr0;
    }
    else {
      dimchange = ap_environment_dimchange(expr->env,a->env);
      if (dimchange==NULL){
	ap_abstract1_raise_invalid_linexpr(man,funid);
	goto ap_abstract1_asssub_linexpr_exit;
      }
      linexpr0 = ap_linexpr0_add_dimensions(expr->linexpr0,dimchange);
    }
    ap_abstract0_t* value = ap_abstract0_asssub_linexpr(funid,man,destructive,a->abstract0,dim,linexpr0,dest ? dest->abstract0 : NULL);
    if (dimchange){
      ap_dimchange_free(dimchange);
      ap_linexpr0_free(linexpr0);
    }
    res = ap_abstract1_consres(destructive, a, value);
  }
  else {
  ap_abstract1_asssub_linexpr_exit:
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_clear(man,a);
  }
  return res;
}

ap_abstract1_t ap_abstract1_assign_linexpr(ap_manager_t* man,
				     bool destructive, ap_abstract1_t* a,
				     ap_var_t var, const ap_linexpr1_t* expr,
				     const ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr(AP_FUNID_ASSIGN_LINEXPR,man,destructive,a,var,expr,dest);
}
ap_abstract1_t ap_abstract1_substitute_linexpr(ap_manager_t* man,
					 bool destructive, ap_abstract1_t* a,
					 ap_var_t var, const ap_linexpr1_t* expr,
					 const ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr(AP_FUNID_SUBSTITUTE_LINEXPR,man,destructive,a,var,expr,dest);
}

ap_abstract1_t ap_abstract1_asssub_linexpr_array(ap_funid_t funid,
					   ap_manager_t* man,
					   bool destructive, ap_abstract1_t* a,
					   ap_var_t* tvar, const ap_linexpr1_t* texpr, size_t size,
					   const ap_abstract1_t* dest)
{
  ap_dim_t* tdim;
  const ap_linexpr0_t** tlinexpr0;
  ap_abstract1_t res;
  ap_dimchange_t* dimchange;
  size_t i;

  i = 0;
  tdim = NULL;
  tlinexpr0 = NULL;
  if ( dest==NULL || ap_abstract1_check_env2(funid,man,a,dest)){
    tdim = malloc(size*sizeof(ap_dim_t));
    tlinexpr0 = malloc(size*sizeof(ap_linexpr0_t*));
    for (i=0; i<size; i++){
      tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
      if (tdim[i]==AP_DIM_MAX){
	ap_abstract1_raise_invalid_var(man,funid,tvar[i]);
	goto ap_abstract1_asssub_linexpr_array_exit;
      }
      if (ap_environment_is_eq(a->env,texpr[i].env)){
	dimchange = NULL;
	tlinexpr0[i] = texpr[i].linexpr0;
      }
      else {
	dimchange = ap_environment_dimchange(texpr[i].env,a->env);
	if (dimchange==NULL){
	  ap_abstract1_raise_invalid_linexpr(man,funid);
	  goto ap_abstract1_asssub_linexpr_array_exit;
	}
	tlinexpr0[i] = ap_linexpr0_add_dimensions(texpr[i].linexpr0,dimchange);
	ap_dimchange_free(dimchange);
      }
    }
    ap_abstract0_t* value = ap_abstract0_asssub_linexpr_array(funid,man,
							destructive,a->abstract0,
							tdim, tlinexpr0, size,
							(dest!=NULL) ? dest->abstract0 : NULL);
    res = ap_abstract1_consres(destructive, a, value);
  }
  else {
  ap_abstract1_asssub_linexpr_array_exit:
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_clear(man,a);
  }
  if (tlinexpr0!=NULL){
    size_t j;
    for (j=0; j<i; j++){
      if (tlinexpr0[j]!=texpr[j].linexpr0)
	ap_linexpr0_free((ap_linexpr0_t*)tlinexpr0[j]);
    }
    free(tlinexpr0);
    free(tdim);
  }
  return res;
}

ap_abstract1_t ap_abstract1_assign_linexpr_array(ap_manager_t* man,
						 bool destructive, ap_abstract1_t* a,
						 ap_var_t* tvar, const ap_linexpr1_t* texpr, size_t size,
						 const ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
}
ap_abstract1_t ap_abstract1_substitute_linexpr_array(ap_manager_t* man,
						     bool destructive, ap_abstract1_t* a,
						     ap_var_t* tvar,  const ap_linexpr1_t* texpr, size_t size,
						     const ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_abstract1_t ap_abstract1_forget_array(ap_manager_t* man,
				   bool destructive, ap_abstract1_t* a,
				   ap_var_t* tvar, size_t size,
				   bool project)
{
  ap_dim_t* tdim;
  size_t i;
  ap_abstract1_t res;

  tdim = malloc(size*sizeof(ap_dim_t));
  for (i=0; i<size; i++){
    tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
    if (tdim[i]==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,AP_FUNID_FORGET_ARRAY,tvar[i]);
      res = ap_abstract1_top(man,a->env);
      if (destructive) ap_abstract1_clear(man,a);
      goto ap_abstract1_forget_array_exit;
    }
  }
  ap_abstract0_t* value = ap_abstract0_forget_array(man,destructive,a->abstract0,tdim,size,project);
  res = ap_abstract1_consres(destructive, a, value);
 ap_abstract1_forget_array_exit:
  free(tdim);
  return res;
}

/* ============================================================ */
/* III.4 Change of environnement */
/* ============================================================ */

ap_abstract1_t ap_abstract1_change_environment(ap_manager_t* man,
					       bool destructive, ap_abstract1_t* a,
					       ap_environment_t* nenv,
					       bool project)
{
  ap_environment_t* env;
  ap_abstract1_t res;
  ap_dimchange_t* dimchange1;
  ap_dimchange_t* dimchange2;
  ap_abstract0_t* value;

  if (ap_environment_is_eq(a->env,nenv)){
    res = destructive ? *a : ap_abstract1_copy(man,a);
    return res;
  }
  dimchange1 = dimchange2 = NULL;
  env = ap_environment_lce(a->env,nenv,&dimchange1,&dimchange2);
  if (env==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			    AP_FUNID_CHANGE_ENVIRONMENT,
			    "the abstract value and the new environment are incompatible");
    res = ap_abstract1_top(man,nenv);
    if (destructive) ap_abstract1_clear(man,a);
    return res;
  }
  assert(dimchange1 || dimchange2);
  value =
    dimchange1 ?
    ap_abstract0_add_dimensions(man,destructive,a->abstract0,dimchange1) :
    a->abstract0;
  ;
  if (dimchange2){
    ap_dimchange_add_invert(dimchange2);
    value = ap_abstract0_remove_dimensions(man,destructive || dimchange1,value,dimchange2);
    free(dimchange2);
  }
  if (dimchange1)
    free(dimchange1);
  res = ap_abstract1_consres2(destructive, a, 
			      value, ap_environment_copy(nenv));
  return res;
}

ap_abstract1_t ap_abstract1_minimize_environment(ap_manager_t* man,
						 bool destructive,
						 ap_abstract1_t* a)
{
  ap_abstract1_t res;
  ap_environment_t* env;
  ap_var_t* tvar;
  ap_dimension_t dim;
  size_t i,size,intdim,realdim;
  ap_var_t var;

  dim = ap_abstract0_dimension(man,a->abstract0);
  size = dim.intdim+dim.realdim;
  tvar = malloc(size*sizeof(ap_var_t));
  intdim=0;
  realdim=0;
  for (i=0; i<size;i++){
    tbool_t tb = ap_abstract0_is_dimension_unconstrained(man,a->abstract0,i);
    if (tb==tbool_true){
      var = ap_environment_var_of_dim(a->env,i);
      if (i<dim.intdim){
	tvar[intdim] = var;
	intdim++;
      } else {
	tvar[intdim+realdim] = var;
	realdim++;
      }
    }
  }
  if (intdim+realdim==0){ /* No change */
    res = destructive ? *a : ap_abstract1_copy(man,a);
  } else {
    env = ap_environment_remove(a->env,&(tvar[0]),intdim,&(tvar[intdim]),realdim);
    if (env==NULL){
      fprintf(stderr,"ap_abstract1.c: ap_abstract1_minimize_environment: internal error\n");
      abort();
    }
    res = ap_abstract1_change_environment(man,destructive,a,env,false);
    ap_environment_free(env);
  }
  free(tvar);
  return res;
}

ap_abstract1_t ap_abstract1_rename_array(ap_manager_t* man,
					 bool destructive,
					 ap_abstract1_t* a,
					 ap_var_t* tvar1, ap_var_t* tvar2, size_t size)
{
  ap_abstract1_t res;
  ap_dimperm_t perm;
  ap_environment_t* env;
  ap_abstract0_t* value;

  env = ap_environment_rename(a->env,tvar1,tvar2,size,&perm);
  if (env==NULL){
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_RENAME_ARRAY,
			       "renaming specification invalid");
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_clear(man,a);
  }
  else {
    value = ap_abstract0_permute_dimensions(man,destructive,a->abstract0,&perm);
    res = ap_abstract1_consres2(destructive, a, value, env);
    ap_dimperm_clear(&perm);
  }
  return res;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

ap_abstract1_t ap_abstract1_expand(ap_manager_t* man,
				   bool destructive,
				   ap_abstract1_t* a,
				   ap_var_t var,
				   ap_var_t* tvar,
				   size_t size)
{
  ap_dim_t dim;
  ap_dimperm_t perm;
  ap_environment_t* nenv;
  ap_abstract0_t* value;
  ap_abstract1_t res;

  /* particular case */
  if (size==0){
    res = destructive ? *a : ap_abstract1_copy(man,a);
    return res;
  }
  /* Getting the dimension to expand */
  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_EXPAND,var);
  ap_abstract1_expand_exit:
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_clear(man,a);
    return res;
  }
  /* Doing the expansion at level 0 */
  value = ap_abstract0_expand(man,destructive,a->abstract0,dim,size);

  /* Building the resulting environment and permutation to apply */
  nenv =
    (dim<a->env->intdim) ?
    ap_environment_add_perm(a->env, tvar, size, NULL,0, &perm) :
    ap_environment_add_perm(a->env, NULL,0, tvar, size, &perm);
  if (nenv==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_EXPAND,
			       "some new variables resulting from the expansion already exist in current environment");
    goto ap_abstract1_expand_exit;
  }

  /* Apply permutation and return */
  value = ap_abstract0_permute_dimensions(man,true,value,&perm);
  ap_dimperm_clear(&perm);
  res = ap_abstract1_consres2(destructive, a, value,nenv);
  return res;
}

ap_abstract1_t ap_abstract1_fold(ap_manager_t* man,
		    bool destructive, ap_abstract1_t* a,
		    ap_var_t* tvar, size_t size)
{
  ap_abstract1_t res;
  ap_dim_t* tdim;
  ap_environment_t* nenv;
  size_t i;
  ap_dim_t dim;
  ap_abstract0_t* value;

  if (size==0){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_FOLD,
			       "array of variables to fold of size 0");
  ap_abstract1_fold_exit:
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_clear(man,a);
    return res;
  }
  else if (size==1){
    res = destructive ? *a : ap_abstract1_copy(man,a);
    return res;
  }

  /* Compute resulting environment */
  dim = ap_environment_dim_of_var(a->env,tvar[0]);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_FOLD,tvar[0]);
    goto ap_abstract1_fold_exit;
  }
  nenv =
    (dim<a->env->intdim) ?
    ap_environment_remove(a->env, &tvar[1], size-1, NULL,0) :
    ap_environment_remove(a->env, NULL,0, &tvar[1], size-1);
  if (nenv==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_FOLD,
			       "some variables to fold are unkown in the environment");
    goto ap_abstract1_fold_exit;
  }
  /* Translate variables to dimensions */
  tdim = malloc(size*sizeof(ap_dim_t));
  tdim[0] = dim;
  for (i=1; i<size; i++){
    tdim[i] = ap_environment_dim_of_var(nenv, tvar[i]);
    if (tdim[i]==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,AP_FUNID_FOLD,tvar[i]);
      free(tdim);
      goto ap_abstract1_fold_exit;
    }
  }
  /* Perform the operation */
  value = ap_abstract0_fold(man,destructive,a->abstract0,tdim,size);
  free(tdim);
  res = ap_abstract1_consres2(destructive, a, value, nenv);
  return res;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */
ap_abstract1_t ap_abstract1_widening(ap_manager_t* man,
			       const ap_abstract1_t* a1, const ap_abstract1_t* a2)
{
  ap_abstract1_t res;
  if (ap_abstract1_check_env2(AP_FUNID_WIDENING,man,a1,a2)){
    res.abstract0 = ap_abstract0_widening(man,a1->abstract0,a2->abstract0);
    res.env = ap_environment_copy(a1->env);
  }
  else {
    res = ap_abstract1_top(man,a1->env);
  }
  return res;
}
/* Widening with threshold */
ap_abstract1_t ap_abstract1_widening_threshold(ap_manager_t* man,
					 const ap_abstract1_t* a1, const ap_abstract1_t* a2,
					 const ap_lincons1_array_t* array)
{
  ap_abstract1_t res;
  ap_lincons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a1->env,array->env)){
    dimchange=NULL;
    array0 = array->lincons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a1->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_LINCONS_ARRAY,
				 "\
environment of array of constraints is not a subset \
of the environment of the abstract value\
"
				 );
      res = ap_abstract1_top(man,a1->env);
      return res;
    }
    array0 = ap_lincons0_array_add_dimensions(&array->lincons0_array,dimchange);
  }
  if (ap_abstract1_check_env2(AP_FUNID_WIDENING,man,a1,a2)){
    ap_abstract0_t* value = ap_abstract0_widening_threshold(man,a1->abstract0,a2->abstract0,&array0);
    res = ap_abstract1_cons(value,a1->env);
  }
  else {
    res = ap_abstract1_top(man,a1->env);
  }
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lincons0_array_clear(&array0);
  }
  return res;
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

ap_abstract1_t ap_abstract1_closure(ap_manager_t* man, bool destructive, ap_abstract1_t* a)
{
  ap_abstract0_t* value;
  value = ap_abstract0_closure(man,destructive,a->abstract0);
  return ap_abstract1_consres(destructive, a, value);
}
