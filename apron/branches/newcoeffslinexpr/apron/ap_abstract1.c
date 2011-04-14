/* ************************************************************************* */
/* ap_abstract1.c: level 1 of interface */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

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
static inline
ap_abstract1_t* ap_abstract1_cons(ap_abstract0_t* a0, ap_environment_t* env)
{
  ap_abstract1_t* res = malloc(sizeof(ap_abstract1_t));
  res->abstract0 = a0;
  res->env = ap_environment_copy(env);
  return res;
}

/* Special constructor:
   Build a new abstract value level 1 from the old one and a new value level 0,
   depending on destructive. The environment is supposed unchanged.
*/
static
ap_abstract1_t* ap_abstract1_consres(bool destructive, ap_abstract1_t* a, ap_abstract0_t* value)
{
  if (destructive){
    a->abstract0 = value;
    return a;
  }
  else {
    return ap_abstract1_cons(value,a->env);
  }
}
/* Special constructor 2:

Build a new abstract value level 1 from the old one, a new
value level 0 and a new environment, depending on
destructive.
*/
static
ap_abstract1_t* ap_abstract1_consres2(bool destructive, ap_abstract1_t* a,
				      ap_abstract0_t* value,
				      ap_environment_t* env)
{
  if (destructive){
    a->abstract0 = value;
    if (a->env!=env){
      ap_environment_free(a->env);
      a->env = env;
    }
    return a;
  }
  else {
    ap_abstract1_t* res = malloc(sizeof(ap_abstract1_t));
    res->abstract0 = value;
    res->env = env;
    return res;
  }
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
				  ap_abstract1_t** tab, size_t size)
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
    ap_environment_t* env0 = tab[0]->env;
    for (i=1;i<size; i++){
      if (!ap_environment_is_eq(env0,tab[i]->env)){
	res = false;
	break;

	char str[160];
	snprintf(str,159,"The %luth abstract value of the array has not \
the same environement as the 0th abstract value\
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
				 ap_manager_t* man, ap_abstract1_t** tab, size_t size)
{
  size_t i;
  for (i=0;i<size;i++){
    if (man->library != tab[i]->abstract0->man->library){
      char str[160];
      snprintf(str,159,"\
The %luth abstract value of the array is of type %s and not of the type %s expected by the manager\
",
	       (unsigned long)i,tab[i]->abstract0->man->library,man->library);
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 funid,
				 str);
      return false;
    }
  }
  return true;
}

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

/* Return a copy of an abstract value, on
   which destructive update does not affect the initial value. */
ap_abstract1_t* ap_abstract1_copy(ap_manager_t* man, ap_abstract1_t* a){
  return ap_abstract1_cons(ap_abstract0_copy(man,a->abstract0),a->env);
}

/* Free all the memory used by the abstract value */
void ap_abstract1_free(ap_manager_t* man, ap_abstract1_t* a)
{
  ap_abstract0_free(man,a->abstract0);
  ap_environment_free(a->env);
  a->abstract0 = NULL;
  a->env = NULL;
  free(a);
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
			       "the 2 abstract values are not defined on the same environement");
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

/* DOIT-ON SÉRIALISER L'ENVIRONNEMENT ? */

/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

ap_membuf_t ap_abstract1_serialize_raw(ap_manager_t* man, ap_abstract1_t* a){
  ap_membuf_t membuf;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"");
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}

/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */
ap_abstract1_t* ap_abstract1_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size){
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
ap_abstract1_t* ap_abstract1_bottom(ap_manager_t* man, ap_environment_t* env)
{
  return ap_abstract1_cons(ap_abstract0_bottom(man,env->dim),env);
}

/* Create a top (universe) value */
ap_abstract1_t* ap_abstract1_top(ap_manager_t* man, ap_environment_t* env)
{
  return ap_abstract1_cons(ap_abstract0_top(man,env->dim),env);
}

/* Abstract an hypercube defined by the arrays tvar and tinterval,
   satisfying: forall i, tvar[i] in tinterval[i].

   If no inclusion is specified for a variable in the environement, its value
   is no constrained in the resulting abstract value.
*/
ap_abstract1_t* ap_abstract1_of_box(ap_manager_t* man,
				    ap_linexpr1_t box)
{
  return
    ap_abstract1_cons(
	ap_abstract0_of_box(man,box->env->dim,box->linexpr0),
	box->env
    );
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_environment_t* ap_abstract1_environment(ap_manager_t* man, ap_abstract1_t* a){
  return a->env;
}

ap_abstract0_t* ap_abstract1_abstract0(ap_manager_t* man, ap_abstract1_t* a){
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
bool ap_abstract1_sat_lincons(ap_manager_t* man, ap_abstract1_t* a, ap_lincons1_t cons)
{
  bool res;

  if (ap_environment_is_eq(a->env,cons->env)){
    res = ap_abstract0_sat_lincons(man,a->abstract0,cons->lincons0);
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
    ap_lincons0_add_dimensions(cons0,cons->lincons0,dimchange);
    res = ap_abstract0_sat_lincons(man,a->abstract0,cons0);
    ap_dimchange_free(dimchange);
    ap_lincons0_clear(cons0);
  }
  return res;
}
/* does the abstract value satisfy the tree expression constraint ? */
bool ap_abstract1_sat_tcons(ap_manager_t* man, ap_abstract1_t* a, ap_tcons1_t* cons)
{
  bool res;

  if (ap_environment_is_eq(a->env,cons->env)){
    res = ap_abstract0_sat_tcons(man,a->abstract0,cons->tcons0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_tcons0_t* cons0;

    dimchange = ap_environment_dimchange(cons->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_SAT_TCONS,
				 "the environment of the constraint is not a subset of the environment of the abstract value");
      man->result.flag_exact = false;
      return false;
    }
    cons0 = ap_tcons0_add_dimensions(cons->tcons0,dimchange);
    res = ap_abstract0_sat_tcons(man,a->abstract0,cons0);
    ap_dimchange_free(dimchange);
    ap_tcons0_free(cons0);
  }
  return res;
}

/* Is the dimension included in the interval in the abstract value ? */
bool ap_abstract1_sat_interval(ap_manager_t* man, ap_abstract1_t* a,
			       ap_var_t var, ap_coeff_t interval)
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
void ap_abstract1_bound_linexpr(ap_manager_t* man,
				ap_coeff_t res, ap_abstract1_t* a, ap_linexpr1_t expr)
{
  if (ap_environment_is_eq(a->env,expr->env)){
    ap_abstract0_bound_linexpr(man,res,a->abstract0,expr->linexpr0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_linexpr0_t expr0;

    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_BOUND_LINEXPR,
				 "the environment of the linear expression is not a subset of the environment of the abstract value");
      ap_coeff_set_top(res);
    }
    else {
      ap_linexpr0_add_dimensions(expr0,expr->linexpr0,dimchange);
      ap_abstract0_bound_linexpr(man,res,a->abstract0,expr0);
      ap_dimchange_free(dimchange);
      ap_linexpr0_clear(expr0);
    }
  }
}
/* Returns the interval taken by a tree expression
   over the abstract value */
void ap_abstract1_bound_texpr(ap_manager_t* man,
			      ap_coeff_t res, ap_abstract1_t* a, ap_texpr1_t* expr)
{
  if (ap_environment_is_eq(a->env,expr->env)){
    ap_abstract0_bound_texpr(man,res,a->abstract0,expr->texpr0);
  }
  else {
    ap_dimchange_t* dimchange;
    ap_texpr0_t* expr0;

    dimchange = ap_environment_dimchange(expr->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_BOUND_TEXPR,
				 "the environment of the tear expression is not a subset of the environment of the abstract value");
       ap_coeff_set_top(res);
    }
    else {
      expr0 = ap_texpr0_add_dimensions(expr->texpr0,dimchange);
      ap_abstract0_bound_texpr(man,res,a->abstract0,expr0);
      ap_dimchange_free(dimchange);
      ap_texpr0_free(expr0);
    }
  }
}

/* Returns the interval taken by the variable over the abstract
   value */
void ap_abstract1_bound_variable(ap_manager_t* man,
				 ap_coeff_t res, ap_abstract1_t* a, ap_var_t var)
{
  ap_dim_t dim;

  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_BOUND_DIMENSION,var);
    ap_coeff_set_top(res);
  }
  else {
    ap_abstract0_bound_dimension(man, res, a->abstract0, dim);
  }
}

/* Converts an abstract value to a polyhedra (conjunction of
   linear constraints). */
void ap_abstract1_to_lincons_array(ap_manager_t* man, ap_lincons1_array_t array, ap_abstract1_t* a)
{
  ap_abstract0_to_lincons_array(man,array->lincons0_array,a->abstract0);
  if (array->env != a->env){
    ap_environment_free(array->env);
    array->env = ap_environment_copy(a->env);
  }
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
void ap_abstract1_to_box(ap_manager_t* man, ap_linexpr1_t box, ap_abstract1_t* a)
{
  ap_abstract0_to_box(man,box->linexpr0,a->abstract0);
  if (box->env != a->env){
    ap_environment_free(box->env);
    box->env = ap_environment_copy(a->env);
  }
}

/* Converts an abstract value to a system of generators. */
void ap_abstract1_to_lingen_array(ap_manager_t* man, ap_lingen1_array_t array, ap_abstract1_t* a)
{
  ap_abstract0_to_lingen_array(man,array->lingen0_array,a->abstract0);
  if (array->env != a->env){
    ap_environment_free(array->env);
    array->env = ap_environment_copy(a->env);
  }
}

/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

ap_abstract1_t* ap_abstract1_meetjoin(ap_funid_t funid, ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  ap_abstract1_t* res;
  if (ap_abstract1_check_env2(funid,man,a1,a2)){
    ap_abstract0_t* value = ap_abstract0_meetjoin(funid,man,destructive,a1->abstract0,a2->abstract0);
    res = ap_abstract1_consres(destructive, a1, value);
  }
  else {
    res = ap_abstract1_top(man,a1->env);
    if (destructive) ap_abstract1_free(man,a1);
  }
  return res;
}
ap_abstract1_t* ap_abstract1_meet(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_MEET,man,destructive,a1,a2);
}
ap_abstract1_t* ap_abstract1_join(ap_manager_t* man, bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2){
  return ap_abstract1_meetjoin(AP_FUNID_JOIN,man,destructive,a1,a2);
}
ap_abstract1_t* ap_abstract1_meetjoin_array(ap_funid_t funid, ap_manager_t* man, ap_abstract1_t** tab, size_t size)
{
  ap_abstract1_t* res;
  if (ap_abstract1_checkman_array(funid,man,tab,size) &&
      ap_abstract1_check_env_array(funid,man,tab,size)){
    size_t i;
    ap_abstract0_t* res0;
    void* (*ptr)(ap_manager_t*,...) = man->funptr[funid];
    void** ntab = malloc(size*sizeof(void*));
    for (i=0;i<size;i++)
      ntab[i] = tab[i]->abstract0->value;
    res0 = ap_abstract0_cons(man,ptr(man,ntab,size));
    res = ap_abstract1_cons(res0,tab[0]->env);
    free(ntab);
  }
  else {
    res = ap_abstract1_top(man, size==0 ? ap_environment_alloc_empty() : tab[0]->env);
  }
  return res;
}
ap_abstract1_t* ap_abstract1_meet_array(ap_manager_t* man, ap_abstract1_t** tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_MEET_ARRAY,man,tab,size);
}
ap_abstract1_t* ap_abstract1_join_array(ap_manager_t* man, ap_abstract1_t** tab, size_t size){
  return ap_abstract1_meetjoin_array(AP_FUNID_JOIN_ARRAY,man,tab,size);
}

ap_abstract1_t* ap_abstract1_meet_lincons_array(ap_manager_t* man,
					       bool destructive,
					       ap_abstract1_t* a, ap_lincons1_array_t array)
{
  ap_abstract1_t* res;
  ap_lincons0_array_t array0;
  ap_lincons0_array_ptr parray0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    parray0 = array->lincons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_LINCONS_ARRAY,
				 "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? a : ap_abstract1_copy(man,a);
      return res;
    }
    ap_lincons0_array_init(array0,array->lincons0_array->discr,0);
    ap_lincons0_array_add_dimensions(array0,array->lincons0_array,dimchange);
    parray0 = array0;
  }
  ap_abstract0_t* value = ap_abstract0_meet_lincons_array(man,destructive,a->abstract0,parray0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lincons0_array_clear(array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

ap_abstract1_t* ap_abstract1_meet_tcons_array(ap_manager_t* man,
					     bool destructive,
					     ap_abstract1_t* a, ap_tcons1_array_t* array)
{
  ap_abstract1_t* res;
  ap_tcons0_array_t array0;
  ap_tcons0_array_t* parray0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    parray0 = &array->tcons0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_MEET_TCONS_ARRAY,
				 "environment of array of constraints is not a subset of the environment of the abstract value");
      res = destructive ? a : ap_abstract1_copy(man,a);
      return res;
    }
    array0 = ap_tcons0_array_add_dimensions(&array->tcons0_array,dimchange);
    parray0 = &array0;
  }
  ap_abstract0_t* value = ap_abstract0_meet_tcons_array(man,destructive,a->abstract0,parray0);
  res = ap_abstract1_consres(destructive, a, value);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_tcons0_array_clear(&array0);
  }
  return res;
}

ap_abstract1_t* ap_abstract1_add_ray_array(ap_manager_t* man,
					  bool destructive,
					  ap_abstract1_t* a, ap_lingen1_array_t array)
{
  ap_abstract1_t* res;
  ap_lingen0_array_t array0;
  ap_lingen0_array_ptr parray0;
  ap_dimchange_t* dimchange;

  if (ap_environment_is_eq(a->env,array->env)){
    dimchange = NULL;
    parray0 = array->lingen0_array;
  }
  else {
    dimchange = ap_environment_dimchange(array->env,a->env);
    if (dimchange==NULL){
      ap_manager_raise_exception(man,
				 AP_EXC_INVALID_ARGUMENT,
				 AP_FUNID_ADD_RAY_ARRAY,
				 "environment of array of generators is not a subset of the environment of the abstract value");
      res = ap_abstract1_top(man,a->env);
      if (destructive) ap_abstract1_free(man,a);
      return res;
    }
    ap_lingen0_array_init(array0,array->lingen0_array->discr,0);
    ap_lingen0_array_add_dimensions(array0,array->lingen0_array,dimchange);
    parray0 = array0;
  }
  ap_abstract0_t* value = ap_abstract0_add_ray_array(man,destructive,a->abstract0,parray0);
  if (dimchange){
    ap_dimchange_free(dimchange);
    ap_lingen0_array_clear(array0);
  }
  res = ap_abstract1_consres(destructive, a, value);
  return res;
}

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_abstract1_t* ap_abstract1_asssub_linexpr_array(ap_funid_t funid,
						  ap_manager_t* man,
						  bool destructive, ap_abstract1_t* a,
						  ap_var_t* tvar, ap_linexpr1_array_t array,
						  ap_abstract1_t* dest)
{
  ap_abstract1_t* res;
  size_t size = ap_linexpr1_array_size(array);
  if (size==0){
  ap_abstract1_asssub_linexpr_array_exit:
    res = (
	dest ?
	ap_abstract1_meet(man,destructive,a,dest) :
	(destructive ? a : ap_abstract1_copy(man,a))
    );
    return res;
  }
  else {
    ap_linexpr0_array_t array0;
    ap_linexpr0_array_ptr parray0;
    ap_dimchange_t* dimchange;

    ap_dim_t* tdim = malloc(size*sizeof(ap_dim_t));
    size_t i = 0;
    for (i=0; i<size; i++){
      tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
      if (tdim[i]==AP_DIM_MAX){
	free(tdim);
	ap_abstract1_raise_invalid_var(man,funid,tvar[i]);
	goto ap_abstract1_asssub_linexpr_array_exit;
      }
    }
    if (ap_environment_is_eq(a->env,array->env)){
      dimchange = NULL;
      parray0 = array->linexpr0_array;
    }
    else {
      dimchange = ap_environment_dimchange(array->env,a->env);
      if (dimchange==NULL){
	free(tdim);
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   funid,
				   "environment of array of linear expressions is not a subset of the environment of the abstract value");
	goto ap_abstract1_asssub_linexpr_array_exit;
      }
      ap_linexpr0_array_init(array0,array->linexpr0_array->discr,0);
      ap_linexpr0_array_add_dimensions(array0,array->linexpr0_array,dimchange);
      parray0 = array0;
    }
    ap_abstract0_t* value = ap_abstract0_asssub_linexpr_array(
	funid,man,
	destructive,a->abstract0,
	tdim, parray0,
	(dest!=NULL) ? dest->abstract0 : NULL
    );
    res = ap_abstract1_consres(destructive, a, value);
    free(tdim);
    if (dimchange){
      ap_dimchange_free(dimchange);
      ap_linexpr0_array_clear(array0);
    }
    return res;
  }
}
ap_abstract1_t* ap_abstract1_assign_linexpr_array(ap_manager_t* man,
						  bool destructive, ap_abstract1_t* a,
						  ap_var_t* tvar, ap_linexpr1_array_t array,
						  ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,man,destructive,a,tvar,array,dest);
}
ap_abstract1_t* ap_abstract1_substitute_linexpr_array(ap_manager_t* man,
						   bool destructive, ap_abstract1_t* a,
						   ap_var_t* tvar, ap_linexpr1_array_t array,
						   ap_abstract1_t* dest){
  return ap_abstract1_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,man,destructive,a,tvar,array,dest);
}

ap_abstract1_t* ap_abstract1_asssub_texpr_array(ap_funid_t funid,
						ap_manager_t* man,
						bool destructive, ap_abstract1_t* a,
						ap_var_t* tvar, ap_texpr1_array_t* array,
						ap_abstract1_t* dest)
{
  ap_abstract1_t* res;
  size_t size = ap_texpr1_array_size(array);
  if (size==0){
  ap_abstract1_asssub_texpr_array_exit:
    res = (
	dest ?
	ap_abstract1_meet(man,destructive,a,dest) :
	(destructive ? a : ap_abstract1_copy(man,a))
    );
  }
  else {
    ap_texpr0_array_t array0;
    ap_texpr0_array_t* parray0;
    ap_dimchange_t* dimchange;

    ap_dim_t* tdim = malloc(size*sizeof(ap_dim_t));
    size_t i = 0;
    for (i=0; i<size; i++){
      tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
      if (tdim[i]==AP_DIM_MAX){
	free(tdim);
	ap_abstract1_raise_invalid_var(man,funid,tvar[i]);
	goto ap_abstract1_asssub_texpr_array_exit;
      }
    }
    if (ap_environment_is_eq(a->env,array->env)){
      dimchange = NULL;
      parray0 = &array->texpr0_array;
    }
    else {
      dimchange = ap_environment_dimchange(array->env,a->env);
      if (dimchange==NULL){
	free(tdim);
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   funid,
				   "environment of array of expressions is not a subset of the environment of the abstract value");
	goto ap_abstract1_asssub_texpr_array_exit;
      }
      array0 = ap_texpr0_array_add_dimensions(&array->texpr0_array,dimchange);
      parray0 = &array0;
    }
    ap_abstract0_t* value = ap_abstract0_asssub_texpr_array(
	funid,man,
	destructive,a->abstract0,
	tdim, parray0,
	(dest!=NULL) ? dest->abstract0 : NULL
    );
    res = ap_abstract1_consres(destructive, a, value);
    free(tdim);
    if (dimchange){
      ap_dimchange_free(dimchange);
      ap_texpr0_array_clear(&array0);
    }
  }
  return res;
}

ap_abstract1_t* ap_abstract1_assign_texpr_array(ap_manager_t* man,
						bool destructive, ap_abstract1_t* a,
						ap_var_t* tvar, ap_texpr1_array_t* array,
						ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,man,destructive,a,tvar,array,dest);
}
ap_abstract1_t* ap_abstract1_substitute_texpr_array(ap_manager_t* man,
						    bool destructive, ap_abstract1_t* a,
						    ap_var_t* tvar, ap_texpr1_array_t* array,
						    ap_abstract1_t* dest){
  return ap_abstract1_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,man,destructive,a,tvar,array,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_abstract1_t* ap_abstract1_forget_array(ap_manager_t* man,
					  bool destructive, ap_abstract1_t* a,
					  ap_var_t* tvar, size_t size,
					  bool project)
{
  ap_dim_t* tdim;
  size_t i;
  ap_abstract1_t* res;

  tdim = malloc(size*sizeof(ap_dim_t));
  for (i=0; i<size; i++){
    tdim[i] = ap_environment_dim_of_var(a->env,tvar[i]);
    if (tdim[i]==AP_DIM_MAX){
      ap_abstract1_raise_invalid_var(man,AP_FUNID_FORGET_ARRAY,tvar[i]);
      res = ap_abstract1_top(man,a->env);
      if (destructive) ap_abstract1_free(man,a);
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

ap_abstract1_t* ap_abstract1_change_environment(ap_manager_t* man,
						bool destructive, ap_abstract1_t* a,
						ap_environment_t* nenv,
						bool project)
{
  ap_abstract1_t* res;
  ap_dimchange2_t* dimchange2;
  ap_abstract0_t* value;

  dimchange2 = ap_environment_dimchange2(a->env,nenv);
  if (dimchange2==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_CHANGE_ENVIRONMENT,
			       "the abstract value and the new environment are incompatible");
    res = ap_abstract1_top(man,nenv);
    if (destructive) ap_abstract1_free(man,a);
    return res;
  }
  value = ap_abstract0_apply_dimchange2(man,destructive,a->abstract0,dimchange2,project);
  ap_dimchange2_free(dimchange2);
  res = ap_abstract1_consres2(destructive,
			      a, value, ap_environment_copy(nenv));
  return res;
}

ap_abstract1_t* ap_abstract1_minimize_environment(ap_manager_t* man,
						  bool destructive,
						  ap_abstract1_t* a)
{
  ap_abstract1_t* res;
  ap_environment_t* env;
  ap_var_t* tvar;
  ap_dimension_t dim;
  size_t i,size,nbdims;
  ap_var_t var;

  dim = ap_abstract0_dimension(man,a->abstract0);
  size = ap_dimension_size(dim);
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
    res = destructive ? a : ap_abstract1_copy(man,a);
  } else {
    env = ap_environment_remove(a->env,tvar,nbdims);
    if (env==NULL){
      free(tvar);
      fprintf(stderr,"ap_abstract1.c: ap_abstract1_minimize_environment: internal error\n");
      abort();
    }
    res = ap_abstract1_change_environment(man,destructive,a,env,false);
    ap_environment_free(env);
  }
  free(tvar);
  return res;
}

ap_abstract1_t* ap_abstract1_rename_array(ap_manager_t* man,
					  bool destructive,
					  ap_abstract1_t* a,
					  ap_var_t* tvar1, ap_var_t* tvar2, size_t size)
{
  ap_abstract1_t* res;
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
    if (destructive) ap_abstract1_free(man,a);
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

ap_abstract1_t* ap_abstract1_expand(ap_manager_t* man,
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
  ap_abstract1_t* res;

  /* particular case */
  if (size==0){
    res = destructive ? a : ap_abstract1_copy(man,a);
    return res;
  }
  /* Getting the dimension to expand */
  dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX){
    ap_abstract1_raise_invalid_var(man,AP_FUNID_EXPAND,var);
  ap_abstract1_expand_exit:
    res = ap_abstract1_top(man,a->env);
    if (destructive) ap_abstract1_free(man,a);
    return res;
  }
  /* Doing the expansion at level 0 */
  value = ap_abstract0_expand(man,destructive,a->abstract0,dim,size);

  /* Building the resulting environment and permutation to apply */
  nenv =
    (dim<a->env->dim.intd) ?
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
  int va = *((ap_dim_t*)a);
  int vb = *((ap_dim_t*)b);
  return (va-vb);
}
ap_abstract1_t* ap_abstract1_fold(ap_manager_t* man,
				  bool destructive, ap_abstract1_t* a,
				  ap_var_t* tvar, size_t size)
{
  ap_abstract1_t* res;
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
    if (destructive) ap_abstract1_free(man,a);
    return res;
  }
  else if (size==1){
    res = destructive ? a : ap_abstract1_copy(man,a);
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
			       "some variables to fold are unkown in the environment");
    goto ap_abstract1_fold_exit;
  }
  /* Translate variables to dimensions */
  tdim = malloc(size*sizeof(ap_dim_t));
  tdim[0] = dim;
  for (i=1; i<size; i++){
    tdim[i] = ap_environment_dim_of_var(a->env, tvar[i]);
    if (tdim[i]==AP_DIM_MAX){
      free(tdim);
      ap_abstract1_raise_invalid_var(man,AP_FUNID_FOLD,tvar[i]);
      goto ap_abstract1_fold_exit;
    }
  }
  /* Sort the array of dimensions */
  qsort(tdim,size,sizeof(ap_dim_t),compar_dim);
  /* Compute the permutation for "exchanging" dim and tdim[0] if necessary  */
  if (dim!=tdim[0]){
    size_t rank;
    /* We have the following situation

       Initially:
       env: A B C D E F G
       dim: 0 1 2 3 4 5 6
       gen: a b c d e f g
       We fold with F,B,D, translated to [5,1,3] and reordered to [1,3,5]
       env: A C E F G
       dim: 0 1 2 3 4
       gen: a b c e g
       a d c e g
       a f c e g
       We need now to apply the permutation
       rank: 0 1 2 3 4
       perm: 0 2 3 1 4

       var: Y Z Q
       dim: 0 1 2
       gen: b a e
       b c e
       b d e

       So we have to perform a to the right between dim[0] and dim-(rank of dim in
       tdim)
    */
    /* look for the position of dim in tdim array */
    void* p = bsearch(&dim,&tdim[1],size-1,sizeof(ap_dim_t),compar_dim);
    assert(p>=(void*)(&tdim[1]));
    size_t index = (p-(void*)tdim)/sizeof(ap_dim_t);
    /* compute permutation */
    ap_dimperm_init(&perm, nenv->dim.intd+nenv->dim.reald);
    ap_dimperm_set_id(&perm);
    for (rank=tdim[0]; rank<dim-index; rank++){
      perm.p[rank] = rank+1;
    }
    perm.p[dim-index] = tdim[0];
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
ap_abstract1_t* ap_abstract1_widening(ap_manager_t* man,
				      ap_abstract1_t* a1, ap_abstract1_t* a2)
{
  ap_abstract1_t* res;
  if (ap_abstract1_check_env2(AP_FUNID_WIDENING,man,a1,a2)){
    res = ap_abstract1_cons(
	ap_abstract0_widening(man,a1->abstract0,a2->abstract0),
	a1->env
    );
  }
  else {
    res = ap_abstract1_top(man,a1->env);
  }
  return res;
}
/* Widening with threshold */
ap_abstract1_t* ap_abstract1_widening_threshold(ap_manager_t* man,
						ap_abstract1_t* a1, ap_abstract1_t* a2,
						ap_lincons1_array_t array)
{
  ap_abstract1_t* res;
  ap_lincons0_array_t array0;
  ap_lincons0_array_ptr parray0;
  ap_dimchange_t* dimchange;

  if (ap_abstract1_check_env2(AP_FUNID_WIDENING,man,a1,a2)){
    if (ap_environment_is_eq(a1->env,array->env)){
      dimchange = NULL;
      parray0 = array->lincons0_array;
    }
    else {
      dimchange = ap_environment_dimchange(array->env,a1->env);
      if (dimchange==NULL){
	ap_manager_raise_exception(man,
				   AP_EXC_INVALID_ARGUMENT,
				   AP_FUNID_WIDENING,
				   "environment of array of constraints is not a subset of the environment of the abstract value");
	res = ap_abstract1_cons(
	    ap_abstract0_widening(man,a1->abstract0,a2->abstract0),
	    a1->env
	);
	return res;
      }
      ap_lincons0_array_init(array0,array->lincons0_array->discr,0);
      ap_lincons0_array_add_dimensions(array0,array->lincons0_array,dimchange);
      parray0 = array0;
    }
    ap_abstract0_t* value = ap_abstract0_widening_threshold(
	man,a1->abstract0,a2->abstract0,parray0
    );
    if (dimchange){
      ap_dimchange_free(dimchange);
      ap_lincons0_array_clear(array0);
    }
    res = ap_abstract1_cons(value,a1->env);
  }
  else {
    res = ap_abstract1_top(man,a1->env);
  }
  return res;
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

ap_abstract1_t* ap_abstract1_closure(ap_manager_t* man, bool destructive, ap_abstract1_t* a)
{
  ap_abstract0_t* value;
  value = ap_abstract0_closure(man,destructive,a->abstract0);
  return ap_abstract1_consres(destructive, a, value);
}

/* ============================================================ */
/* IV. Additional functions */
/* ============================================================ */

/* These two function abstract a set of constraints */
ap_abstract1_t* ap_abstract1_of_lincons_array(ap_manager_t* man,
					      ap_environment_t* env,
					      ap_lincons1_array_t array)
{
  ap_abstract1_t* res = ap_abstract1_top(man,env);
  res = ap_abstract1_meet_lincons_array(man,true,res,array);
  return res;
}
ap_abstract1_t* ap_abstract1_of_tcons_array(ap_manager_t* man,
					    ap_environment_t* env,
					    ap_tcons1_array_t* array)
{
  ap_abstract1_t* res = ap_abstract1_top(man,env);
  res = ap_abstract1_meet_tcons_array(man,true,res,array);
  return res;
}

/*
  These four functions implement assignment and substitution of a single
  dimension by a single expression.
*/
ap_abstract1_t* ap_abstract1_asssub_linexpr(ap_funid_t funid,
					    ap_manager_t* man,
					    bool destructive,
					    ap_abstract1_t* a,
					    ap_var_t var, ap_linexpr1_t expr,
					    ap_abstract1_t* dest)
{
  ap_abstract1_t* res;
  ap_linexpr1_array_t array;
  ap_linexpr0_array_ptr array0;
  ap_linexprD_array_t arrayD;
  ap_linexprMPQ_array_t arrayMPQ;
  ap_linexprMPFR_array_t arrayMPFR;

  array->env = expr->env;

  array0 = array->linexpr0_array;
  array0->discr = expr->linexpr0->discr;
  switch (array0->discr){
  case AP_SCALAR_D:
    array0->linexpr_array.D = arrayD;
    arrayD->size = 1;
    arrayD->p = (ap_linexprD_t*)&(expr->linexpr0->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    array0->linexpr_array.MPQ = arrayMPQ;
    arrayMPQ->size = 1;
    arrayMPQ->p = (ap_linexprMPQ_t*)&(expr->linexpr0->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    array0->linexpr_array.MPFR = arrayMPFR;
    arrayMPFR->size = 1;
    arrayMPFR->p = (ap_linexprMPFR_t*)&(expr->linexpr0->linexpr.MPFR);
    break;
  default:
    abort();
  }
  return ap_abstract1_asssub_linexpr_array(funid,
					   man,destructive,a,&var,array,dest);
}
ap_abstract1_t* ap_abstract1_assign_linexpr(ap_manager_t* man,
					    bool destructive,
					    ap_abstract1_t* a,
					    ap_var_t var, ap_linexpr1_t expr,
					    ap_abstract1_t* dest)
{
  return ap_abstract1_asssub_linexpr(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
				     man,destructive,a,var,expr,dest);
}
ap_abstract1_t* ap_abstract1_substitute_linexpr(ap_manager_t* man,
						bool destructive,
						ap_abstract1_t* a,
						ap_var_t var, ap_linexpr1_t expr,
						ap_abstract1_t* dest)
{
  return ap_abstract1_asssub_linexpr(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
				     man,destructive,a,var,expr,dest);
}
ap_abstract1_t* ap_abstract1_assign_texpr(ap_manager_t* man,
					  bool destructive, ap_abstract1_t* a,
					  ap_var_t var, ap_texpr1_t* expr,
					  ap_abstract1_t* dest)
{
  ap_texpr1_array_t array;
  array.texpr0_array.size = 1;
  array.texpr0_array.p = &expr->texpr0;
  array.env = expr->env;
  return ap_abstract1_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,man,destructive,a,&var,&array,dest);
}
ap_abstract1_t* ap_abstract1_substitute_texpr(ap_manager_t* man,
					      bool destructive, ap_abstract1_t* a,
					      ap_var_t var, ap_texpr1_t* expr,
					      ap_abstract1_t* dest)
{
  ap_texpr1_array_t array;
  array.texpr0_array.size = 1;
  array.texpr0_array.p = &expr->texpr0;
  array.env = expr->env;
  return ap_abstract1_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,man,destructive,a,&var,&array,dest);
}

/* Unify two abstract values on their common variables, that is, embed them
   on the least common environment and then compute their meet. The result is
   defined on the least common environment. */
ap_abstract1_t* ap_abstract1_unify(ap_manager_t* man,
				   bool destructive,
				   ap_abstract1_t* a1,ap_abstract1_t* a2)
{
  ap_dimchange_t* dimchange1;
  ap_dimchange_t* dimchange2;
  ap_environment_t* env;
  ap_abstract0_t* value1;
  ap_abstract0_t* value2;
  ap_abstract0_t* value;
  ap_abstract1_t* res;

  if (ap_environment_is_eq(a1->env,a2->env)){
    return ap_abstract1_meet(man,destructive,a1,a2);
  }
  env = ap_environment_lce(a1->env,a2->env,&dimchange1,&dimchange2);
  if (env==NULL){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,AP_FUNID_UNKNOWN,
			       "\
ap_abstract1_unify: a variable is defined with different types  \
in the two abstract values");
    return destructive ? a1 : ap_abstract1_copy(man,a1);
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

/* /\* Evaluate the interval linear expression expr on the abstract value a and */
/*    approximate it by a quasilinear expression. discr indicates which type of */
/*    numbers should be used for computations. */

/*    This implies calls to ap_abstract0_bound_dimension. *\/ */

/* void ap_abstract1_quasilinear_of_intlinear( */
/*     ap_manager_t* man, */
/*     ap_linexpr1_t res, ap_abstract1_t* a, ap_linexpr1_t expr, ap_scalar_discr_t discr) */
/* { */
/*   ap_linexpr0_t linexpr0; */
/*   ap_linexpr0_ptr plinexpr0; */
/*   ap_dimchange_t* dimchange; */
/*   bool exact; */

/*   if (ap_environment_is_eq(a->env,expr->env)){ */
/*     dimchange = NULL; */
/*     plinexpr0 = expr->linexpr0; */
/*   } */
/*   else { */
/*     dimchange = ap_environment_dimchange(expr->env,a->env); */
/*     if (dimchange==NULL){ */
/*       ap_abstract1_raise_invalid_expr(man,AP_FUNID_UNKNOWN); */
/*       goto ap_abstract1_quasilinear_of_intlinear_exit; */
/*     } */
/*     ap_linexpr0_add_dimensions(linexpr0,expr->linexpr0,dimchange); */
/*     plinexpr0 = linexpr0; */
/*   } */
/*   ap_quasilinearize_linexpr0(man,res->linexpr0,a->abstract0->value,plinexpr0,&exact,discr); */
/*   res->env = ap_environment_copy(a->env); */
/*   if (dimchange){ */
/*     ap_dimchange_free(dimchange); */
/*     ap_linexpr0_clear(linexpr0); */
/*   } */
/*   return res; */
/*  ap_abstract1_quasilinear_of_intlinear_exit: */
/*   ap_linexpr0_resize(res,0); */
/*   { */
/*     ap_coeff_ptr pcoeff; */
/*     ap_linexpr0_cstref(pcoeff,res); */
/*     ap_coeff_set_top(pcoeff); */
/*   } */
/*   return res; */
/* } */

/* /\* Evaluate the tree expression expr on the abstract value a and approximate it */
/*    by an interval linear (resp. quasilinear if quasilinearize is true) */
/*    expression. discr indicates which type of numbers should be used for */
/*    computations. */

/*    This implies calls to ap_abstract0_bound_dimension. *\/ */

/* ap_linexpr1_t ap_abstract1_intlinear_of_tree(ap_manager_t* man, ap_abstract1_t* a, ap_texpr1_t* expr, ap_scalar_discr_t discr, bool quasilinearize) */
/* { */
/*   ap_texpr0_t* texpr0; */
/*   ap_dimchange_t* dimchange; */
/*   ap_linexpr1_t res; */
/*   bool exact; */

/*   if (ap_environment_is_eq(a->env,expr->env)){ */
/*     dimchange = NULL; */
/*     texpr0 = expr->texpr0; */
/*   } */
/*   else { */
/*     dimchange = ap_environment_dimchange(expr->env,a->env); */
/*     if (dimchange==NULL){ */
/*       ap_abstract1_raise_invalid_expr(man,AP_FUNID_UNKNOWN); */
/*       goto ap_abstract1_quasilinear_of_intlinear_exit; */
/*     } */
/*     texpr0 = ap_texpr0_add_dimensions(expr->texpr0,dimchange); */
/*   } */
/*   res.linexpr0 = ap_intlinearize_texpr0(man,a->abstract0->value,texpr0,&exact,discr,quasilinearize); */
/*   res.env = ap_environment_copy(a->env); */
/*   if (dimchange){ */
/*     ap_dimchange_free(dimchange); */
/*     ap_texpr0_free(texpr0); */
/*   } */
/*   return res; */
/*  ap_abstract1_quasilinear_of_intlinear_exit: */
/*   res.linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0); */
/*   res.env = ap_environment_copy(a->env); */
/*   ap_coeff_set_interval_top(&res.linexpr0->cst); */
/*   return res; */
/* } */
