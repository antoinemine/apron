#line 14 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ************************************************************************* */
/* abstract1.h: generic operations on abstract values at level 1 */
/* ************************************************************************* */

/* GENERATED FROM abstract1.nw: DOT NOT MODIFY ! */

#ifndef _ABSTRACT1_H_
#define _ABSTRACT1_H_

#include "manager.h"
#include "environment.h"
#include "expr0.h"
#include "abstract0.h"
#include "expr1.h"
#line 43 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
typedef struct abstract1_t {
  abstract0_t* abstract0;
  environment_t* env;
} abstract1_t;
  /* data structure invariant:
     abstract0_integer_dimension(man,abstract0)== env->intdim &&
     abstract0_real_dimension(man,abstract0)== env->realdim */

#line 106 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

#line 116 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

abstract1_t abstract1_copy(manager_t* man, const abstract1_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void abstract1_clear(manager_t* man, abstract1_t* a);
  /* Free all the memory used by the abstract value */

size_t abstract1_size(manager_t* man, const abstract1_t* a);
  /* Return the abstract size of an abstract value (see manager_t) */


#line 137 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void abstract1_minimize(manager_t* man, const abstract1_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void abstract1_canonicalize(manager_t* man, const abstract1_t* a);
  /* Put the abstract value in canonical form. (not yet clear definition) */

void abstract1_approximate(manager_t* man, abstract1_t* a, int algorithm);
  /* Perform some transformation on the abstract value, guided by the
     field algorithm.

     The transformation may lose information.  The argument "algorithm"
     overrides the field algorithm of the structure of type foption_t
     associated to abstract1_approximate (commodity feature). */

tbool_t abstract1_is_minimal(manager_t* man, const abstract1_t* a);
tbool_t abstract1_is_canonical(manager_t* man, const abstract1_t* a);

#line 171 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void abstract1_fprint(FILE* stream,
                    manager_t* man,
                    const abstract1_t* a);
  /* Print the abstract value in a pretty way */

void abstract1_fprintdiff(FILE* stream,
                        manager_t* man,
                        const abstract1_t* a1, const abstract1_t* a2);
  /* Print the difference between a1 (old value) and a2 (new value).
     The meaning of difference is library dependent. */

void abstract1_fdump(FILE* stream, manager_t* man, const abstract1_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes. */


#line 214 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

membuf_t abstract1_serialize_raw(manager_t* man, const abstract1_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

abstract1_t abstract1_deserialize_raw(manager_t* man, void* ptr);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */

#line 240 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

abstract1_t abstract1_bottom(manager_t* man, environment_t* env);
  /* Create a bottom (empty) value defined on the environment */

abstract1_t abstract1_top(manager_t* man, environment_t* env);
  /* Create a top (universe) value defined on the environment */

abstract1_t abstract1_of_box(manager_t* man,
                             environment_t* env,
                             var_t* tvar,
                             interval_t* tinterval,
                             size_t size);
  /* Abstract an hypercube defined by the arrays tvar and tinterval,
     satisfying: forall i, tvar[i] in tinterval[i].

     If no inclusion is specified for a variable in the environement, its value
     is no constrained in the resulting abstract value.
  */

abstract1_t abstract1_of_lincons_array(manager_t* man,
                                       environment_t* env,
                                       const lincons1_array_t* array);
  /* Abstract a convex polyhedra defined on the environment
     by the array of linear constraints
  */

#line 279 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

const environment_t* abstract1_environment(manager_t* man, const abstract1_t* a);
const abstract0_t* abstract1_abstract0(manager_t* man, const abstract1_t* a);

#line 293 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

/* If any of the following functions returns tbool_top, this means that
   an exception has occured, or that the exact computation was
   considered too expensive to be performed (according to the options).
   The flag exact and best should be cleared in such a case. */

tbool_t abstract1_is_bottom(manager_t* man, const abstract1_t* a);
tbool_t abstract1_is_top(manager_t* man, const abstract1_t* a);

tbool_t abstract1_is_leq(manager_t* man, const abstract1_t* a1, const abstract1_t* a2);
  /* inclusion check */

tbool_t abstract1_is_eq(manager_t* man, const abstract1_t* a1, const abstract1_t* a2);
  /* equality check */

tbool_t abstract1_sat_lincons(manager_t* man, const abstract1_t* a, const lincons1_t* lincons);
  /* does the abstract value satisfy the linear constraint ? */

tbool_t abstract1_sat_interval(manager_t* man, const abstract1_t* a,
                              var_t var, const interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? 
     - Raises an exception if var is unknown in the environment of the abstract value 
  */
tbool_t abstract1_is_dimension_unconstrained(manager_t* man, const abstract1_t* a,
                                             var_t var);
  /* is the variable unconstrained in the abstract value ?  
     - Raises an exception if var is unknown in the environment of the abstract value 
  */

#line 331 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

struct interval_t abstract1_bound_linexpr(manager_t* man,
                                          const abstract1_t* a, const linexpr1_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

struct interval_t abstract1_bound_dimension(manager_t* man,
                                    const abstract1_t* a, var_t var);
  /* Returns the interval taken by the dimension
     over the abstract value 
     - Raises an exception if var is unknown in the environment of the abstract value 
  */
                                                                                         
lincons1_array_t abstract1_to_lincons_array(manager_t* man, const abstract1_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints).
     - The environment of the result is a copy of the environment of the abstract value.
  */

box1_t abstract1_to_box(manager_t* man, const abstract1_t* a);
  /* Converts an abstract value to an interval/hypercube. */

generator1_array_t abstract1_to_generator_array(manager_t* man, const abstract1_t* a);
  /* Converts an abstract value to a system of generators.
     - The environment of the result is a copy of the environment of the abstract value.
 */

#line 372 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

abstract1_t abstract1_meet(manager_t* man, const abstract1_t* a1, const abstract1_t* a2);
abstract1_t abstract1_join(manager_t* man, const abstract1_t* a1, const abstract1_t* a2);
  /* Meet and Join of 2 abstract values 
     - The environment of the result is the lce of the arguments 
     - Raises an EXC_INVALID_ARGUMENT exception if the lce does not exists
  */

abstract1_t abstract1_meet_array(manager_t* man, const abstract1_t* tab, size_t size);
abstract1_t abstract1_join_array(manager_t* man, const abstract1_t* tab, size_t size);
  /* Meet and Join of an array of abstract values.
     - Raises an [[exc_invalid_argument]] exception if [[size==0]]
       (no way to define the dimensionality of the result in such a case 
     - The environment of the result is the lce of the arguments 
     - Raises an EXC_INVALID_ARGUMENT exception if the lce does not exists
  */

abstract1_t abstract1_meet_lincons_array(manager_t* man,
                                        const abstract1_t* a,
                                        const lincons1_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize abstract1_of_lincons_array) */

abstract1_t abstract1_add_ray_array(manager_t* man,
                                   const abstract1_t* a,
                                   const generator1_array_t* array);
  /* Generalized time elapse operator */

/* Imperative versions of the previous functions. */
void abstract1_meet_with(manager_t* man, abstract1_t* a1, const abstract1_t* a2);
void abstract1_join_with(manager_t* man, abstract1_t* a1, const abstract1_t* a2);
void abstract1_meet_lincons_array_with(manager_t* man,
                                             abstract1_t* a,
                                             const lincons1_array_t* array);
void abstract1_add_ray_array_with(manager_t* man,
                                        abstract1_t* a,
                                        const generator1_array_t* array);

#line 435 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

abstract1_t abstract1_assign_linexpr(manager_t* man,
                                    const abstract1_t* a,
                                    var_t var, const linexpr1_t* expr);
abstract1_t abstract1_substitute_linexpr(manager_t* man,
                                        const abstract1_t* a,
                                        var_t var, const linexpr1_t* expr);
  /* Assignement and Substitution of a single dimension by resp.
     a linear expression and a interval linear expression */
abstract1_t abstract1_assign_linexpr_array(manager_t* man,
                                          const abstract1_t* a,
                                          var_t* tvar,
                                          const linexpr1_t* texpr,
                                          size_t size);
abstract1_t abstract1_substitute_linexpr_array(manager_t* man,
                                              const abstract1_t* a,
                                              var_t* tvar,
                                              const linexpr1_t* texpr,
                                              size_t size);
  /* Parallel Assignement and Substitution of several dimensions by
     linear expressons. */

/* Imperative versions of the previous functions. */
void abstract1_assign_linexpr_with(manager_t* man,
                                    abstract1_t* a,
                                    var_t var, const linexpr1_t* expr);
void abstract1_substitute_linexpr_with(manager_t* man,
                                        abstract1_t* a,
                                        var_t var, const linexpr1_t* expr);
void abstract1_assign_linexpr_array_with(manager_t* man,
                                          abstract1_t* a,
                                          var_t* tvar,
                                          const linexpr1_t* texpr,
                                          size_t size);
void abstract1_substitute_linexpr_array_with(manager_t* man,
                                              abstract1_t* a,
                                              var_t* tvar,
                                              const linexpr1_t* texpr,
                                              size_t size);

#line 484 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

abstract1_t abstract1_project_array(manager_t* man,
                                   const abstract1_t* a, var_t* tvar, size_t size);
abstract1_t abstract1_forget_array(manager_t* man,
                                  const abstract1_t* a, var_t* tvar, size_t size);
/* Imperative versions of the previous functions. */
void abstract1_project_array_with(manager_t* man,
                                  abstract1_t* a, var_t* tvar, size_t size);
void abstract1_forget_array_with(manager_t* man,
                                  abstract1_t* a, var_t* tvar, size_t size);

#line 504 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.4 Change of environnement */
/* ============================================================ */

abstract1_t abstract1_change_environment(manager_t* man,
                                         const abstract1_t* a,
                                         environment_t* env,
                                         bool project);

abstract1_t abstract1_minimize_environment(manager_t* man,
                                           const abstract1_t* a);
  /* Remove from the environment of the abstract value 
     variables that are unconstrained in it. */

abstract1_t abstract1_rename_array(manager_t* man,
                                   const abstract1_t* a,
                                   var_t* var, var_t* nvar, size_t size);
  /* Parallel renaming. The new variables should not interfere with the variables that are not renamed. */

/* Imperative versions */
void abstract1_change_environment_with(manager_t* man,
                                       abstract1_t* a,
                                       environment_t* env,
                                       bool project);
void abstract1_minimize_environment_with(manager_t* man,
                                         abstract1_t* a);
void abstract1_rename_array_with(manager_t* man,
                                 const abstract1_t* a,
                                 var_t* var, var_t* nvar, size_t size);

#line 540 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

abstract1_t abstract1_expand(manager_t* man,
                             const abstract1_t* a,
                             var_t var,
                             var_t* tvar,
                             size_t size);
  /* Expand the variable var into itself + n additional variables, of 
     same type than var.

     It results in (n+1) unrelated variables having same
     relations with other variables. 

     The additional variables are added to the environment 
     of the argument for making the environment of the result.
  */

abstract1_t abstract1_fold(manager_t* man,
                           const abstract1_t* a,
                           var_t* tvar,
                           size_t size);
  /* Fold the dimensions in the array tvar of size n>=1 and put the result
     in the first variable in the array. 

     The other variables of the array are then forgot
     and removed from the environment. */

/* Imperative versions of the previous functions. */
void abstract1_expand_with(manager_t* man,
                                abstract1_t* a,
                                var_t var,
                                var_t* tvar,
                                size_t n);
void abstract1_fold_with(manager_t* man,
                              abstract1_t* a,
                              var_t* tvar,
                              size_t size);

#line 586 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening with threshold */
abstract1_t abstract1_widening(manager_t* man,
                                      const abstract1_t* a1, const abstract1_t* a2,
                                      const lincons1_array_t* array);


#line 602 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

abstract1_t abstract1_closure(manager_t* man, const abstract1_t* a);
void abstract1_closure_with(manager_t* man, abstract1_t* a);
#line 613 "/udd/bjeannet/dev/commoninterface/abstract1.nw"
#endif
