/* ************************************************************************* */
/* ap_abstract1.c: level 1 of interface */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

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
destructive.
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
void ap_abstract1_raise_invalid_expr(ap_manager_t* man,
					ap_funid_t funid)
{
  ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,"\
Linear/tree expression not defined on a (sub)environment of the abstract value\
");
}

bool ap_abstract1_check_env2(ap_funid_t funid,
			     ap_manager_t* man,
			     ap_abstract1_t* a1, ap_abstract1_t* a2)
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
				  ap_abstract1_t* tab, size_t size)
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
	//res = false;
	//break;

	char str[160];
	snprintf(str,159,"The %luth abstract value of the array has not \
the same environment as the 0th abstract value\
",(unsigned long)i);
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
				 ap_manager_t* man, ap_abstract1_t* tab, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    if (man->library != tab[i].abstract0->man->library){
      char str[160];
      snprintf(str,159,"\
The %luth abstract value of the array is of type %s and not of the type %s expected by the manager\
",
	       (unsigned long)i,tab[i].abstract0->man->library,man->library);
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 funid,
				 str);
      return false;
    }
  }
  return true;
}

void ap_box1_fprint(FILE* stream, ap_box1_t* box)
{
  size_t i;
  fprintf(stream,"Box1: (%lu,%lu)\n",
	  (unsigned long)box->env->intdim,(unsigned long)box->env->realdim);
  for (i=0; i<box->env->intdim+box->env->realdim;i++){
    ap_var_t var = ap_environment_var_of_dim(box->env,i);
    char* name = ap_var_operations->to_string(var);
    fprintf(stream,"  %s\tin",name);
    free(name);
    ap_interval_fprint(stream,box->p[i]);
    fprintf(stream,"\n");
  }
}

void ap_box1_clear(ap_box1_t* box)
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
ap_abstract1_t ap_abstract1_copy(ap_manager_t* man, ap_abstract1_t* a){
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
size_t ap_abstract1_size(ap_manager_t* man, ap_abstract1_t* a){
  return ap_abstract0_size(man,a->abstract0);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */


/* Minimize the size of the representation of a.
   This may result in a later recomputation of internal information.
*/
void ap_abstract1_minimize(ap_manager_t* man, ap_abstract1_t* a){
  ap_abstract0_minimize(man,a->abstract0);
}

/* Put the abstract value in canonical form. (not yet clear definition) */
void ap_abstract1_canonicalize(ap_manager_t* man, ap_abstract1_t* a){
  ap_abstract0_canonicalize(man,a->abstract0);
}
/* Return an hash code */
int ap_abstract1_hash(ap_manager_t* man, ap_abstract1_t* a)
{
  return (ap_environment_hash(a->env)*251 +
	  ap_abstract0_hash(man,a->abstract0)*19);
}

/* Perform some transformation on the abstract value, guided by the
   field algorithm.

   The transformation may lose information.  The argument "algorithm"
   overrides the field algorithm of the structure of type foption_t
   associated to ap_abstract1_approximate (commodity feature). */
void ap_abstract1_approximate(ap_manager_t* man, ap_abstract1_t* a, int algorithm){
  ap_abstract0_approximate(man,a->abstract0, algorithm);
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

/* Print the abstract value */
void ap_abstract1_fprint(FILE* stream,
			 ap_manager_t* man,
			 ap_abstract1_t* a)
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
			     ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  ap_environment_name_of_dim_t* name_of_dim;
  if (!ap_environment_is_eq(a1->env,a2->env)){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_FPRINTDIFF,
			       "the 2 abstract values are not defined on the same environment");
    fprintf(stream,"unknown diff\n");
  }
  name_of_dim = ap_environment_name_of_dim_alloc(a1->env);
  ap_abstract0_fprintdiff(stream,man,a1->abstract0, a2->abstract0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* Dump the internal representation of an abstract value,
   for debugging purposes */
void ap_abstract1_fdump(FILE* stream, ap_manager_t* man, ap_abstract1_t* a)
{
  fprintf(stream, "abstract value of level 1:\n");
  ap_environment_fdump(stream,a->env);
  ap_abstract0_fdump(stream,man,a->abstract0);
}


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

/* DOIT-ON SERIALISER L'ENVIRONNEMENT ? */

/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

ap_membuf_t ap_abstract1_serialize_raw(ap_manager_t* man, ap_abstract1_t* a){
  ap_membuf_t membuf;
  (void)a;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"");
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
ap_abstract1_t ap_abstract1_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size){
  (void)ptr;
  (void)size;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,"");
  return ap_abstract1_top(man,ap_environment_alloc_empty());
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

   If no inclusion is specified for a variable in the environment, its value
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
  a = ap_abstract1_cons(ap_abstract0_of_box(man,env->intdim,env->realdim,(ap_interval_t**)itv),env);
  ap_interval_array_free(itv,env->intdim+env->realdim);
  return a;
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_environment_t* ap_abstract1_environment(ap_manager_t* man, ap_abstract1_t* a){
  (void)man;
  return a->env;
}

ap_abstract0_t* ap_abstract1_abstract0(ap_manager_t* man, ap_abstract1_t* a)
{
  (void)man;
  return a->abstract0;
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool ap_abstract1_is_bottom(ap_manager_t* man, ap_abstract1_t* a){
  return ap_abstract0_is_bottom(man,a->abstract0);
}
bool ap_abstract1_is_top(ap_manager_t* man, ap_abstract1_t* a){
  return ap_abstract0_is_top(man,a->abstract0);
}

/* inclusion check */
bool ap_abstract1_is_leq(ap_manager_t* man, ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  if (ap_abstract1_check_env2(AP_FUNID_IS_LEQ,man,a1,a2))
    return ap_abstract0_is_leq(man,a1->abstract0,a2->abstract0);
  else {
    man->result.flag_exact = false;
    return false;
  }
}
/* equality check */
bool ap_abstract1_is_eq(ap_manager_t* man, ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  if (ap_abstract1_check_env2(AP_FUNID_IS_EQ,man,a1,a2))
    return ap_abstract0_is_eq(man,a1->abstract0,a2->abstract0);
  else {
    man->result.flag_exact = false;
    return false;
  }
}
/* does the abstract value satisfy the linear constraint ? */
bool ap_abstract1_sat_lincons(ap_manager_t* man, ap_abstract1_t* a, ap_lincons1_t* cons)
{
  bool res;

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
      man->result.flag_exact = false;
      return false;
    }
    cons0 = ap_lincons0_add_dimensions(&cons->lincons0,dimchange);
    res = ap_abstract0_sat_lincons(man,a->abstract0,&cons0);
    ap_dimchange_free(dimchange);
    ap_lincons0_clear(&cons0);
  }
  return res;
}
/* does the abstract value satisfy the tree expression constraint ? */
bool ap_abstract1_sat_tcons(ap_manager_t* man, ap_abstract1_t* a, ap_tcons1_t* cons)
{
  bool res;

  if (ap_environment_is_eq(a->env,cons->env)){
    res = ap_abstract0_sat_tcons(man,a->abstract0,&cons->tcons0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_tcons0_t cons0;

    dimchange = ap_environment_dimchange(cons->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_SAT_TCONS,
				 "the environment of the constraint is not a subset of the environment of the abstract value");
      man->result.flag_exact = false;
      return false;
    }
    cons0 = ap_tcons0_add_dimensions(&cons->tcons0,dimchange);
    res = ap_abstract0_sat_tcons(man,a->abstract0,&cons0);
    ap_dimchange_free(dimchange);
    ap_tcons0_clear(&cons0);
  }
  return res;
}

/* Is the dimension included in the interval in the abstract value ? */
bool ap_abstract1_sat_interval(ap_manager_t* man, ap_abstract1_t* a,
			       ap_var_t var, ap_interval_t* interval)
{
  ap_dim_t dim;
  bool res;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_SAT_INTERVAL,var);
    man->result.flag_exact = false;
    return false;
  }
  return ap_abstract0_sat_interval(man, a->abstract0, dim, interval);
}

/* Is the dimension included in the interval in the abstract value ? */
bool ap_abstract1_is_variable_unconstrained(ap_manager_t* man, ap_abstract1_t* a,
					    ap_var_t var)
{
  ap_dim_t dim;
  bool res;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_IS_DIMENSION_UNCONSTRAINED,var);
    man->result.flag_exact = false;
    return false;
  }
  return ap_abstract0_is_dimension_unconstrained(man, a->abstract0, dim);
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

/* Returns the interval taken by a linear expression
   over the abstract value */
ap_interval_t* ap_abstract1_bound_linexpr(ap_manager_t* man,
					  ap_abstract1_t* a, ap_linexpr1_t* expr)
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
/* Returns the interval taken by a tree expression
   over the abstract value */
ap_interval_t* ap_abstract1_bound_texpr(ap_manager_t* man,
					ap_abstract1_t* a, ap_texpr1_t* expr)
{
  ap_interval_t* res;

  if (ap_environment_is_eq(a->env,expr->env)){
    res = ap_abstract0_bound_texpr(man,a->abstract0,expr->texpr0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_texpr0_t* expr0;

    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_BOUND_TEXPR,
				 "the environment of the tear expression is not a subset of the environment of the abstract value");
      res = ap_interval_alloc();
      ap_interval_reinit(res,man->option.scalar_discr);
      ap_interval_set_top(res);
      return res;
    }
    expr0 = ap_texpr0_add_dimensions(expr->texpr0,dimchange);
    res = ap_abstract0_bound_texpr(man,a->abstract0,expr0);
    ap_dimchange_free(dimchange);
    ap_texpr0_free(expr0);
  }
  return res;
}

/* Returns the interval taken by the variable over the abstract
   value */
ap_interval_t* ap_abstract1_bound_variable(ap_manager_t* man,
					   ap_abstract1_t* a, ap_var_t var)
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
ap_lincons1_array_t ap_abstract1_to_lincons_array(ap_manager_t* man, ap_abstract1_t* a)
{
  ap_lincons1_array_t array;

  array.lincons0_array = ap_abstract0_to_lincons_array(man,a->abstract0);
  array.env = ap_environment_copy(a->env);
  return array;
}

/* Converts an abstract value to a conjunction of tree expressions constraints. */
ap_tcons1_array_t ap_abstract1_to_tcons_array(ap_manager_t* man, ap_abstract1_t* a)
{
  ap_tcons1_array_t array;

  array.tcons0_array = ap_abstract0_to_tcons_array(man,a->abstract0);
  array.env = ap_environment_copy(a->env);
  return array;
}

/* Converts an abstract value to an interval/hypercube.  The size
   of the resulting array is ap_abstract1_dimension(man,a).  This
   function can be reimplemented by using ap_abstract1_bound_linexpr
*/
ap_box1_t ap_abstract1_to_box(ap_manager_t* man, ap_abstract1_t* a)
{
  ap_box1_t box;

  box.p = ap_abstract0_to_box(man,a->abstract0);
  box.env = ap_environment_copy(a->env);
  return box;
}

/* Converts an abstract value to a system of generators. */
ap_generator1_array_t ap_abstract1_to_generator_array(ap_manager_t* man, ap_abstract1_t* a)
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

ap_abstract1_t ap_abstract1_meetjoin(ap_funid_t funid, ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2)
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
ap_abstract1_t ap_abstract1_meet(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_MEET,man,destructive,a1,a2);
}
ap_abstract1_t ap_abstract1_join(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_JOIN,man,destructive,a1,a2);
}
ap_abstract1_t ap_abstract1_meetjoin_array(ap_funid_t funid, ap_manager_t* man, ap_abstract1_t* tab, size_t size)
{
  ap_abstract1_t res;
  if (ap_abstract1_checkman_array(funid,man,tab,size) &&
      ap_abstract1_check_env_array(funid,man,tab,size)){
    size_t i;
    ap_abstract0_t* res0;
    void* (*ptr)(ap_manager_t*,void**,size_t) = man->funptr[funid];
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
    res = ap_abstract1_top(man, size==0 ? ap_environment_alloc_empty() : tab[0].env);
  }
  return res;
}
ap_abstract1_t ap_abstract1_meet_array(ap_manager_t* man, ap_abstract1_t* tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_MEET_ARRAY,man,tab,size);
}
ap_abstract1_t ap_abstract1_join_array(ap_manager_t* man, ap_abstract1_t* tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_JOIN_ARRAY,man,tab,size);
}

ap_abstract1_t ap_abstract1_meet_lincons_array(ap_manager_t* man,
					       bool destructive,
					       ap_abstract1_t* a, ap_lincons1_array_t* array)
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

ap_abstract1_t ap_abstract1_meet_tcons_array(ap_manager_t* man,
					     bool destructive,
					     ap_abstract1_t* a, ap_tcons1_array_t* array)
{
  ap_abstract1_t res;
  ap_tcons0_array_t array0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    array0 = array->tcons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_TCONS_ARRAY,
				 "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? *a : ap_abstract1_copy(man,a);
      return res;
    }
    array0 = ap_tcons0_array_add_dimensions(&array->tcons0_array,dimchange);
  }
  ap_abstract0_t* value = ap_abstract0_meet_tcons_array(man,destructive,a->abstract0,&array0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_tcons0_array_clear(&array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

ap_abstract1_t ap_abstract1_add_ray_array(ap_manager_t* man,
					  bool destructive,
					  ap_abstract1_t* a, ap_generator1_array_t* array)
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
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_abstract1_t ap_abstract1_asssub_linexpr_array(ap_funid_t funid,
						 ap_manager_t* man,
						 bool destructive, ap_abstract1_t* a,
						 ap_var_t* tvar, ap_linexpr1_t* linexpr, size_t size,
						 ap_abstract1_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract1_meet(man,destructive,a,dest);
    }
    else if (destructive){
      return *a;
    }
    else {
      return ap_abstract1_copy(man,a);
    }
  }
  else {
    ap_dim_t* tdim;
    ap_linexpr0_t** tlinexpr0;
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
	if (ap_environment_is_eq(a->env,linexpr[i].env)){
	  dimchange = NULL;
	  tlinexpr0[i] = linexpr[i].linexpr0;
	}
	else {
	  dimchange = ap_environment_dimchange(linexpr[i].env,a->env);
	  if (dimchange==NULL){
	    ap_abstract1_raise_invalid_expr(man,funid);
	    goto ap_abstract1_asssub_linexpr_array_exit;
	  }
	  tlinexpr0[i] = ap_linexpr0_add_dimensions(linexpr[i].linexpr0,dimchange);
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
	if (tlinexpr0[j]!=linexpr[j].linexpr0)
	  ap_linexpr0_free(tlinexpr0[j]);
      }
      free(tlinexpr0);
      free(tdim);
    }
    return res;
  }
}
ap_abstract1_t ap_abstract1_assign_linexpr_array(ap_manager_t* man,
					       bool destructive, ap_abstract1_t* a,
					       ap_var_t* tvar, ap_linexpr1_t* linexpr, size_t size,
					       ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,man,destructive,a,tvar,linexpr,size,dest);
}
ap_abstract1_t ap_abstract1_substitute_linexpr_array(ap_manager_t* man,
						   bool destructive, ap_abstract1_t* a,
						   ap_var_t* tvar,  ap_linexpr1_t* linexpr, size_t size,
						   ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,man,destructive,a,tvar,linexpr,size,dest);
}

ap_abstract1_t ap_abstract1_asssub_texpr_array(ap_funid_t funid,
					       ap_manager_t* man,
					       bool destructive, ap_abstract1_t* a,
					       ap_var_t* tvar, ap_texpr1_t* texpr, size_t size,
					       ap_abstract1_t* dest)
{
  if (size==0){
    if (dest){
      return ap_abstract1_meet(man,destructive,a,dest);
    }
    else if (destructive){
      return *a;
    }
    else {
      return ap_abstract1_copy(man,a);
    }
  }
  else {
    ap_dim_t* tdim;
    ap_texpr0_t** ttexpr0;
    ap_abstract1_t res;
    ap_dimchange_t* dimchange;
    size_t i;

    i = 0;
    tdim = NULL;
    ttexpr0 = NULL;
    if ( dest==NULL || ap_abstract1_check_env2(funid,man,a,dest)){
      tdim = malloc(size*sizeof(ap_dim_t));
      ttexpr0 = malloc(size*sizeof(ap_texpr0_t*));
      for (i=0; i<size; i++){
	tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
	if (tdim[i]==AP_DIM_MAX){
	  ap_abstract1_raise_invalid_var(man,funid,tvar[i]);
	  goto ap_abstract1_asssub_texpr_array_exit;
	}
	if (ap_environment_is_eq(a->env,texpr[i].env)){
	  dimchange = NULL;
	  ttexpr0[i] = texpr[i].texpr0;
	}
	else {
	  dimchange = ap_environment_dimchange(texpr[i].env,a->env);
	  if (dimchange==NULL){
	    ap_abstract1_raise_invalid_expr(man,funid);
	    goto ap_abstract1_asssub_texpr_array_exit;
	  }
	  ttexpr0[i] = ap_texpr0_add_dimensions(texpr[i].texpr0,dimchange);
	  ap_dimchange_free(dimchange);
	}
      }
      ap_abstract0_t* value = ap_abstract0_asssub_texpr_array(funid,man,
							      destructive,a->abstract0,
							      tdim, ttexpr0, size,
							      (dest!=NULL) ? dest->abstract0 : NULL);
      res = ap_abstract1_consres(destructive, a, value);
    }
    else {
    ap_abstract1_asssub_texpr_array_exit:
      res = ap_abstract1_top(man,a->env);
      if (destructive) ap_abstract1_clear(man,a);
    }
    if (ttexpr0!=NULL){
      size_t j;
      for (j=0; j<i; j++){
	if (ttexpr0[j]!=texpr[j].texpr0)
	  ap_texpr0_free(ttexpr0[j]);
      }
      free(ttexpr0);
      free(tdim);
    }
    return res;
  }
}
ap_abstract1_t ap_abstract1_assign_texpr_array(ap_manager_t* man,
					       bool destructive, ap_abstract1_t* a,
					       ap_var_t* tvar, ap_texpr1_t* texpr, size_t size,
					       ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
}
ap_abstract1_t ap_abstract1_substitute_texpr_array(ap_manager_t* man,
						   bool destructive, ap_abstract1_t* a,
						   ap_var_t* tvar,  ap_texpr1_t* texpr, size_t size,
						   ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,man,destructive,a,tvar,texpr,size,dest);
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
  ap_abstract1_t res;
  ap_dimchange2_t* dimchange2;
  ap_abstract0_t* value;

  dimchange2 = ap_environment_dimchange2(a->env,nenv);
  if (dimchange2==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_CHANGE_ENVIRONMENT,
			       "the abstract value and the new environment are incompatible");
    res = ap_abstract1_top(man,nenv);
    if (destructive) ap_abstract1_clear(man,a);
    return res;
  }
  value = ap_abstract0_apply_dimchange2(man,destructive,a->abstract0,dimchange2,project);
  ap_dimchange2_free(dimchange2);
  res = ap_abstract1_consres2(destructive,
			      a, value, ap_environment_copy(nenv));
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
  size_t i,size,nbdims;
  ap_var_t var;

  dim = ap_abstract0_dimension(man,a->abstract0);
  size = dim.intdim+dim.realdim;
  tvar = malloc(size*sizeof(ap_var_t));
  nbdims = 0;
  for (i=0; i<size;i++){
    bool b = ap_abstract0_is_dimension_unconstrained(man,a->abstract0,i);
    if (b){
      var = ap_environment_var_of_dim(a->env,i);
      tvar[nbdims] = var;
      nbdims++;
    }
  }
  if (nbdims==0){ /* No change */
    res = destructive ? *a : ap_abstract1_copy(man,a);
  } else {
    env = ap_environment_remove(a->env,tvar,nbdims);
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

static int compar_dim(const void* a, const void* b)
{
  int va = *((const ap_dim_t*)a);
  int vb = *((const ap_dim_t*)b);
  return (va-vb);
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
  ap_dimperm_t perm;

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
  nenv = ap_environment_remove(a->env, &tvar[1], size-1);
  if (nenv==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_FOLD,
			       "some variables to fold are unknown in the environment");
    goto ap_abstract1_fold_exit;
  }
  /* Translate variables to dimensions */
  tdim = malloc(size*sizeof(ap_dim_t));
  tdim[0] = dim;
  for (i=1; i<size; i++){
    tdim[i] = ap_environment_dim_of_var(a->env, tvar[i]);
    if (tdim[i]==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,AP_FUNID_FOLD,tvar[i]);
      free(tdim);
      goto ap_abstract1_fold_exit;
    }
  }
  /* Sort the array of dimensions */
  qsort(tdim,size,sizeof(ap_dim_t),compar_dim);
  /* Compute the permutation for "exchanging" dim and tdim[0] if necessary  */
  if (dim!=tdim[0]){
    size_t rank;
    /* Due to reordering of tdim, we have now folded the variables
       into tdim[0] instead of dim.

       To fix this, we construct a permutation to exchange dim and tdim[0].

       Due to folding, the position of dim has changed in the result.
       Then new position is dim-index, where index is the position of
       dim in tdim, i.e., the number of variables that have been removed
       before dim.

       We then rotate all indices between tdim[0] and dim-index left.

       Example:
       We want to fold 1 into 4 (dim).
       We first sort tdmin as [1;4].
       Level 0 folding will actuall fold 4 (dim) into 1 (tdim[0]),
       and remove 4.
       We thus rotate left variables at positions 1, 2, 3 (after folding).

                tdim[0]  dim
                    v     v
       original:  0 1 2 3 4 5
       folded:    0 f 2 3 5     (where f = 1 U 4)
       permuted:  0 2 3 f 5

       The permutation is: 0->0, 1->3, 2->1, 3->2, 5->5
    */
    /* look for the position of dim in tdim array */
    void* p = bsearch(&dim,&tdim[1],size-1,sizeof(ap_dim_t),compar_dim);
    assert(p>=(void*)(&tdim[1]));
    size_t index = (p-(void*)tdim)/sizeof(ap_dim_t);
    /* compute permutation */
    ap_dimperm_init(&perm, nenv->intdim+nenv->realdim);
    ap_dimperm_set_id(&perm);
    for (rank=tdim[0]+1; rank<=dim-index; rank++){
      perm.dim[rank] = rank-1;
    }
    perm.dim[tdim[0]] = dim-index;
  }

  /* Perform the operation */
  value = ap_abstract0_fold(man,destructive,a->abstract0,tdim,size);
  /* Apply the permutation if necessary */
  if (dim!=tdim[0]){
    value = ap_abstract0_permute_dimensions(man,true,value,&perm);
    ap_dimperm_clear(&perm);
  }
  free(tdim);
  res = ap_abstract1_consres2(destructive, a, value, nenv);
  return res;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */
ap_abstract1_t ap_abstract1_widening(ap_manager_t* man,
				     ap_abstract1_t* a1, ap_abstract1_t* a2)
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
					       ap_abstract1_t* a1, ap_abstract1_t* a2,
					       ap_lincons1_array_t* array)
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

/* ============================================================ */
/* IV. Additional functions */
/* ============================================================ */

/* These two function abstract a set of constraints */
ap_abstract1_t ap_abstract1_of_lincons_array(ap_manager_t* man,
					     ap_environment_t* env,
					     ap_lincons1_array_t* array)
{
  ap_abstract1_t res = ap_abstract1_top(man,env);
  res = ap_abstract1_meet_lincons_array(man,true,&res,array);
  return res;
}
ap_abstract1_t ap_abstract1_of_tcons_array(ap_manager_t* man,
					     ap_environment_t* env,
					     ap_tcons1_array_t* array)
{
  ap_abstract1_t res = ap_abstract1_top(man,env);
  res = ap_abstract1_meet_tcons_array(man,true,&res,array);
  return res;
}

/*
  These four functions implement assignment and substitution of a single
  dimension by a single expression.
*/
ap_abstract1_t ap_abstract1_assign_linexpr(ap_manager_t* man,
					   bool destructive, ap_abstract1_t* a,
					   ap_var_t var, ap_linexpr1_t* expr,
					   ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,man,destructive,a,&var,expr,1,dest);
}
ap_abstract1_t ap_abstract1_substitute_linexpr(ap_manager_t* man,
					       bool destructive, ap_abstract1_t* a,
					       ap_var_t var, ap_linexpr1_t* expr,
					       ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,man,destructive,a,&var,expr,1,dest);
}
ap_abstract1_t ap_abstract1_assign_texpr(ap_manager_t* man,
					 bool destructive, ap_abstract1_t* a,
					 ap_var_t var, ap_texpr1_t* expr,
					 ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,man,destructive,a,&var,expr,1,dest);
}
ap_abstract1_t ap_abstract1_substitute_texpr(ap_manager_t* man,
					     bool destructive, ap_abstract1_t* a,
					     ap_var_t var, ap_texpr1_t* expr,
					     ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,man,destructive,a,&var,expr,1,dest);
}

/* Unify two abstract values on their common variables, that is, embed them
   on the least common environment and then compute their meet. The result is
   defined on the least common environment. */
ap_abstract1_t ap_abstract1_unify(ap_manager_t* man,
				  bool destructive,
				  ap_abstract1_t* a1,ap_abstract1_t* a2)
{
  ap_dimchange_t* dimchange1;
  ap_dimchange_t* dimchange2;
  ap_environment_t* env;
  ap_abstract0_t* value1;
  ap_abstract0_t* value2;
  ap_abstract0_t* value;
  ap_abstract1_t res;

  if (ap_environment_is_eq(a1->env,a2->env)){
    return ap_abstract1_meet(man,destructive,a1,a2);
  }
  env = ap_environment_lce(a1->env,a2->env,&dimchange1,&dimchange2);
  if (env==NULL){
   ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_UNKNOWN,
			      "\
ap_abstract1_unify: a variable is defined with different types \
in the two abstract values");
   if (destructive)
     return *a1;
   else
     return ap_abstract1_copy(man,a1);
  }
  assert(dimchange1 || dimchange2);

  value1 =
    dimchange1 ?
    ap_abstract0_add_dimensions(man,destructive,a1->abstract0,dimchange1,false) :
    (destructive ?
     a1->abstract0 :
     ap_abstract0_copy(man,a1->abstract0));
  ;
  value2 =
    dimchange2 ?
    ap_abstract0_add_dimensions(man,false,a2->abstract0,dimchange2,false) :
    a2->abstract0;
  ;
  value = ap_abstract0_meet(man,true,value1,value2);
  res = ap_abstract1_consres2(destructive, a1, value, env);
  if (dimchange1) ap_dimchange_free(dimchange1);
  if (dimchange2){
    ap_dimchange_free(dimchange2);
    ap_abstract0_free(man,value2);
  }
  return res;
}


/* Evaluate the interval linear expression expr on the abstract value a and
   approximate it by a quasilinear expression. discr indicates which type of
   numbers should be used for computations.

   This implies calls to ap_abstract0_bound_dimension. */

ap_linexpr1_t ap_abstract1_quasilinear_of_intlinear (ap_manager_t* man, ap_abstract1_t* a, ap_linexpr1_t* expr, ap_scalar_discr_t discr)
{
  ap_linexpr0_t* linexpr0;
  ap_dimchange_t* dimchange;
  ap_linexpr1_t res;
  bool exact;

  if (ap_environment_is_eq(a->env,expr->env)){
    dimchange = NULL;
    linexpr0 = expr->linexpr0;
  }
  else {
    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_abstract1_raise_invalid_expr(man,AP_FUNID_UNKNOWN);
      goto ap_abstract1_quasilinear_of_intlinear_exit;
    }
    linexpr0 = ap_linexpr0_add_dimensions(expr->linexpr0,dimchange);
  }
  res.linexpr0 = ap_quasilinearize_linexpr0(man,a->abstract0->value,linexpr0,&exact,discr);
  res.env = ap_environment_copy(a->env);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_linexpr0_free(linexpr0);
  }
  return res;
 ap_abstract1_quasilinear_of_intlinear_exit:
  res.linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0);
  res.env = ap_environment_copy(a->env);
  ap_coeff_set_interval_top(&res.linexpr0->cst);
  return res;
}

/* Evaluate the tree expression expr on the abstract value a and approximate it
   by an interval linear (resp. quasilinear if quasilinearize is true)
   expression. discr indicates which type of numbers should be used for
   computations.

   This implies calls to ap_abstract0_bound_dimension. */

ap_linexpr1_t ap_abstract1_intlinear_of_tree(ap_manager_t* man, ap_abstract1_t* a, ap_texpr1_t* expr, ap_scalar_discr_t discr, bool quasilinearize)
{
  ap_texpr0_t* texpr0;
  ap_dimchange_t* dimchange;
  ap_linexpr1_t res;
  bool exact;

  if (ap_environment_is_eq(a->env,expr->env)){
    dimchange = NULL;
    texpr0 = expr->texpr0;
  }
  else {
    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_abstract1_raise_invalid_expr(man,AP_FUNID_UNKNOWN);
      goto ap_abstract1_quasilinear_of_intlinear_exit;
    }
    texpr0 = ap_texpr0_add_dimensions(expr->texpr0,dimchange);
  }
  res.linexpr0 = ap_intlinearize_texpr0(man,a->abstract0->value,texpr0,&exact,discr,quasilinearize);
  res.env = ap_environment_copy(a->env);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_texpr0_free(texpr0);
  }
  return res;
 ap_abstract1_quasilinear_of_intlinear_exit:
  res.linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0);
  res.env = ap_environment_copy(a->env);
  ap_coeff_set_interval_top(&res.linexpr0->cst);
  return res;
}
