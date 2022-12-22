/* ************************************************************************* */
/* ap_abstract0.h: generic operations on numerical abstract values */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_ABSTRACT0_H_
#define _AP_ABSTRACT0_H_

typedef struct ap_abstract0_t ap_abstract0_t;

#include "ap_manager.h"
#include "ap_expr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic abstract value at level 0 */
struct ap_abstract0_t {
  void* value;       /* Abstract value of the underlying library */
  ap_manager_t* man; /* Used to identify the effective type of value */
};

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_copy(ap_manager_t* man, ap_abstract0_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void ap_abstract0_free(ap_manager_t* man, ap_abstract0_t* a);
  /* Free all the memory used by the abstract value */

size_t ap_abstract0_size(ap_manager_t* man, ap_abstract0_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */


void ap_abstract0_minimize(ap_manager_t* man, ap_abstract0_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void ap_abstract0_canonicalize(ap_manager_t* man, ap_abstract0_t* a);
  /* Put the abstract value in canonical form. (not yet clear definition) */

int ap_abstract0_hash(ap_manager_t* man, ap_abstract0_t* a);
  /* Return an hash value for the abstract value.  Two abstract values in
     canonical from (according to @code{ap_abstract1_canonicalize}) and
     considered as equal by the function ap_abstract0_is_eq should be given the
     same hash value (this implies more or less a canonical form).
  */


void ap_abstract0_approximate(ap_manager_t* man, ap_abstract0_t* a, int algorithm);
  /* Perform some transformation on the abstract value, guided by the
     field algorithm.

     The transformation may lose information.  The argument "algorithm"
     overrides the field algorithm of the structure of type ap_funopt_t
     associated to ap_abstract0_approximate (commodity feature). */

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */


void ap_abstract0_fprint(FILE* stream,
			 ap_manager_t* man, ap_abstract0_t* a, char** name_of_dim);
  /* Print the abstract value in a pretty way, using function name_of_dim to
     name dimensions. If name_of_dim==NULL, use x0, x1, ... */

void ap_abstract0_fprintdiff(FILE* stream,
			     ap_manager_t* man,
			     ap_abstract0_t* a1, ap_abstract0_t* a2,
			     char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */


void ap_abstract0_fdump(FILE* stream, ap_manager_t* man, ap_abstract0_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */


ap_membuf_t ap_abstract0_serialize_raw(ap_manager_t* man, ap_abstract0_t* a);
  /* Allocate a memory buffer (with malloc), output the abstract value in raw
     binary format to it and return a pointer on the memory buffer and the size
     of bytes written.  It is the user responsibility to free the memory
     afterwards (with free). */


ap_abstract0_t* ap_abstract0_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
  /* Return the abstract value read in raw binary format from the input stream
     and store in size the number of bytes read */

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

ap_abstract0_t* ap_abstract0_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */


ap_abstract0_t* ap_abstract0_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */

ap_abstract0_t* ap_abstract0_of_box(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim.
     If any interval is empty, the resulting abstract element is empty (bottom).
     In case of a 0-dimensional element (intdim+realdim=0), the abstract element is always top (not bottom).
  */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t ap_abstract0_dimension(ap_manager_t* man, ap_abstract0_t* a);
  /* Return the dimensionality of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

/* In abstract tests,

   - true means that the predicate is certainly true.

   - false means by default don't know (an exception has occurred, or the exact
     computation was considered too expensive to be performed).

     However, if the flag exact in the manager is true, then false means really
     that the predicate is false.
*/

bool ap_abstract0_is_bottom(ap_manager_t* man, ap_abstract0_t* a);
bool ap_abstract0_is_top(ap_manager_t* man, ap_abstract0_t* a);


bool ap_abstract0_is_leq(ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2);
  /* inclusion check */
bool ap_abstract0_is_eq(ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2);
  /* equality check */

bool ap_abstract0_sat_lincons(ap_manager_t* man, ap_abstract0_t* a, ap_lincons0_t* lincons);
bool ap_abstract0_sat_tcons(ap_manager_t* man, ap_abstract0_t* a, ap_tcons0_t* tcons);
  /* does the abstract value satisfy the constraint ? */
bool ap_abstract0_sat_interval(ap_manager_t* man, ap_abstract0_t* a,
			      ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */

bool ap_abstract0_is_dimension_unconstrained(ap_manager_t* man,
					     ap_abstract0_t* a, ap_dim_t dim);
  /* is the dimension unconstrained in the abstract value ?  If it is the case,
     we have forget(man,a,dim) == a */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* ap_abstract0_bound_linexpr(ap_manager_t* man,
					  ap_abstract0_t* a, ap_linexpr0_t* expr);
ap_interval_t* ap_abstract0_bound_texpr(ap_manager_t* man,
					ap_abstract0_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by the expression
     over the abstract value */


ap_interval_t* ap_abstract0_bound_dimension(ap_manager_t* man,
					    ap_abstract0_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */


ap_lincons0_array_t ap_abstract0_to_lincons_array(ap_manager_t* man, ap_abstract0_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints).

     The constraints are normally guaranteed to be really linear (without intervals) */

ap_tcons0_array_t ap_abstract0_to_tcons_array(ap_manager_t* man, ap_abstract0_t* a);
  /* Converts an abstract value to conjunction of tree expressions constraints.

     The constraints are normally guaranteed to be scalar (without intervals) */

ap_interval_t** ap_abstract0_to_box(ap_manager_t* man, ap_abstract0_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is ap_abstract0_dimension(man,a).

     In case of an empty (bottom) abstract element of size n, the array contains n empty intervals.
     For 0-dimensional abstract elements, the array has size 0, and it is impossible to distinguish a 0-dimensional bottom element from a 0-dimensional non-bottom (i.e., top) element. Converting it back to an abstract element with ap_abstract0_of_box will then always construct a 0-dimensional top element.

     This function can be reimplemented by using ap_abstract0_bound_linexpr.
  */


ap_generator0_array_t ap_abstract0_to_generator_array(ap_manager_t* man, ap_abstract0_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */


ap_abstract0_t* ap_abstract0_meet(ap_manager_t* man,
				  bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2);

ap_abstract0_t* ap_abstract0_join(ap_manager_t* man,
				  bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2);
  /* Meet and Join of 2 abstract values */


ap_abstract0_t* ap_abstract0_meet_array(ap_manager_t* man,
					ap_abstract0_t** tab, size_t size);

ap_abstract0_t* ap_abstract0_join_array(ap_manager_t* man,
					ap_abstract0_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */


ap_abstract0_t*
ap_abstract0_meet_lincons_array(ap_manager_t* man,
				bool destructive, ap_abstract0_t* a, ap_lincons0_array_t* array);
ap_abstract0_t*
ap_abstract0_meet_tcons_array(ap_manager_t* man,
				bool destructive, ap_abstract0_t* a, ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of constraints */

ap_abstract0_t*
ap_abstract0_add_ray_array(ap_manager_t* man,
			   bool destructive, ap_abstract0_t* a, ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_abstract0_t*
ap_abstract0_assign_linexpr_array(ap_manager_t* man,
				  bool destructive,
				  ap_abstract0_t* org,
				  ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				  ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_assign_texpr_array(ap_manager_t* man,
				bool destructive,
				ap_abstract0_t* org,
				ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_substitute_linexpr_array(ap_manager_t* man,
				      bool destructive,
				      ap_abstract0_t* org,
				      ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				      ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_substitute_texpr_array(ap_manager_t* man,
				    bool destructive,
				    ap_abstract0_t* org,
				    ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				    ap_abstract0_t* dest);

  /* Parallel Assignment and Substitution of several dimensions by
     linear/tree expressions in abstract value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     octagons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_abstract0_t*
ap_abstract0_forget_array(ap_manager_t* man,
			  bool destructive,
			  ap_abstract0_t* a, ap_dim_t* tdim, size_t size,
			  bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

ap_abstract0_t*
ap_abstract0_add_dimensions(ap_manager_t* man,
			    bool destructive,
			    ap_abstract0_t* a,ap_dimchange_t* dimchange,
			    bool project);
ap_abstract0_t*
ap_abstract0_remove_dimensions(ap_manager_t* man,
			       bool destructive,
			       ap_abstract0_t* a, ap_dimchange_t* dimchange);
  /* Size of the permutation is supposed to be equal to
     the dimension of the abstract value */
ap_abstract0_t*
ap_abstract0_permute_dimensions(ap_manager_t* man,
				bool destructive,
				ap_abstract0_t* a, ap_dimperm_t* perm);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */


ap_abstract0_t*
ap_abstract0_expand(ap_manager_t* man,
		    bool destructive,
		    ap_abstract0_t* a, ap_dim_t dim, size_t n);
  /* Expand the dimension dim into itself + n additional dimensions.
     It results in (n+1) unrelated dimensions having same
     relations with other dimensions. The (n+1) dimensions are put as follows:

     - original dimension dim

     - if the dimension is integer, the n additional dimensions are put at the
       end of integer dimensions; if it is real, at the end of the real
       dimensions.
  */


ap_abstract0_t*
ap_abstract0_fold(ap_manager_t* man,
		  bool destructive,
		  ap_abstract0_t* a, ap_dim_t* tdim, size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using ap_abstract0_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

ap_abstract0_t* ap_abstract0_widening(ap_manager_t* man,
				      ap_abstract0_t* a1, ap_abstract0_t* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */
ap_abstract0_t* ap_abstract0_closure(ap_manager_t* man, bool destructive, ap_abstract0_t* a);

/* ********************************************************************** */
/* IV. Functions offered by the APRON interface */
/* ********************************************************************** */

/* These functions do not correspond to functions in the underlying library. */

static inline
ap_manager_t* ap_abstract0_manager(ap_abstract0_t* a)
  { return a->man; }
  /* Return a reference to the manager contained in the abstract value.
     The reference should not be freed */

ap_abstract0_t* ap_abstract0_of_lincons_array(ap_manager_t* man,
					      size_t intdim, size_t realdim,
					      ap_lincons0_array_t* array);
ap_abstract0_t* ap_abstract0_of_tcons_array(ap_manager_t* man,
					    size_t intdim, size_t realdim,
					    ap_tcons0_array_t* array);
  /* Abstract a conjunction of tree constraints */

ap_abstract0_t* ap_abstract0_assign_linexpr(ap_manager_t* man,
					    bool destructive,
					    ap_abstract0_t* org,
					    ap_dim_t dim, ap_linexpr0_t* expr,
					    ap_abstract0_t* dest);
ap_abstract0_t* ap_abstract0_assign_texpr(ap_manager_t* man,
					  bool destructive,
					  ap_abstract0_t* org,
					  ap_dim_t dim, ap_texpr0_t* expr,
					  ap_abstract0_t* dest);
ap_abstract0_t* ap_abstract0_substitute_linexpr(ap_manager_t* man,
						bool destructive,
						ap_abstract0_t* org,
						ap_dim_t dim, ap_linexpr0_t* expr,
						ap_abstract0_t* dest);
ap_abstract0_t* ap_abstract0_substitute_texpr(ap_manager_t* man,
					      bool destructive,
					      ap_abstract0_t* org,
					      ap_dim_t dim, ap_texpr0_t* expr,
					      ap_abstract0_t* dest);
  /* Assignment and Substitution of a single dimension by an expression in
     abstract value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     octagons.
 */

/* Applying an ap_dimchange2_t transformation (dimension adding followed by
   dimension removal/projection).  If project is true, the newly added
   dimension are projected on their 0-hyperplane. */
ap_abstract0_t* ap_abstract0_apply_dimchange2(ap_manager_t* man,
					      bool destructive,
					      ap_abstract0_t* a, ap_dimchange2_t* dimchange2,
					      bool project);

/* Widening with threshold */
ap_abstract0_t*
ap_abstract0_widening_threshold(ap_manager_t* man,
				ap_abstract0_t* a1, ap_abstract0_t* a2,
				ap_lincons0_array_t* array);

/* ********************************************************************** */
/* ********************************************************************** */
/* Internal functions */
/* ********************************************************************** */
/* ********************************************************************** */

/* ********************************************************************** */
/* 0. Utility and checking functions */
/* ********************************************************************** */

/* Constructor for ap_abstract0_t */

static inline
ap_abstract0_t* ap_abstract0_cons(ap_manager_t* man, void* value)
{
  ap_abstract0_t* res = (ap_abstract0_t*)malloc(sizeof(ap_abstract0_t));
  res->value = value;
  res->man = ap_manager_copy(man);
  return res;
}
static inline
void _ap_abstract0_free(ap_abstract0_t* a)
{
  void (*ptr)(ap_manager_t*,ap_abstract0_t*) = (void (*) (ap_manager_t*,ap_abstract0_t*))(a->man->funptr[AP_FUNID_FREE]);
  ptr(a->man,(ap_abstract0_t*)a->value);
  ap_manager_free(a->man);
  free(a);
}
static inline
ap_abstract0_t* ap_abstract0_cons2(ap_manager_t* man, bool destructive, ap_abstract0_t* oldabs, void* newvalue)
{
  if (destructive){
    if (oldabs->man != man){
      ap_manager_free(oldabs->man);
      oldabs->man = ap_manager_copy(man);
    }
    oldabs->value = newvalue;
    return oldabs;
  }
  else {
    return ap_abstract0_cons(man,newvalue);
  }
}

/* ====================================================================== */
/* 0.1 Checking typing w.r.t. manager */
/* ====================================================================== */

/*
  These functions return true if everything is OK, otherwise they raise an
  exception in the manager and return false.
*/

/* One abstract value */

void ap_abstract0_checkman1_raise(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t* a);

static inline
bool ap_abstract0_checkman1(ap_funid_t funid, ap_manager_t* man, ap_abstract0_t* a)
{
  if (man->library != a->man->library){
    ap_abstract0_checkman1_raise(funid,man,a);
    return false;
  }
  else
    return true;
}

/* Two abstract values */
bool ap_abstract0_checkman2(ap_funid_t funid,
			    ap_manager_t* man, ap_abstract0_t* a1, ap_abstract0_t* a2);

/* Array of abstract values */
bool ap_abstract0_checkman_array(ap_funid_t funid,
				 ap_manager_t* man, ap_abstract0_t** tab, size_t size);

/* ====================================================================== */
/* 0.2 Checking compatibility of arguments: abstract values */
/* ====================================================================== */

/* Getting dimensions without checks */
static inline
ap_dimension_t _ap_abstract0_dimension(ap_abstract0_t* a)
{
  ap_dimension_t (*ptr)(ap_manager_t*, void*) = (ap_dimension_t (*) (ap_manager_t*, void*))(a->man->funptr[AP_FUNID_DIMENSION]);
  return ptr(a->man,a->value);
}

/* Check that the 2 abstract values have the same dimensionality */
bool ap_abstract0_check_abstract2(ap_funid_t funid, ap_manager_t* man,
				  ap_abstract0_t* a1, ap_abstract0_t* a2);

/* Check that the array of abstract values have the same dimensionality.*/
bool ap_abstract0_check_abstract_array(ap_funid_t funid, ap_manager_t* man,
				       ap_abstract0_t** tab, size_t size);

/* ====================================================================== */
/* 0.3 Checking compatibility of arguments: dimensions */
/* ====================================================================== */

/* Check that the dimension makes sense in the given dimensionality */
void ap_abstract0_check_dim_raise(ap_funid_t funid, ap_manager_t* man,
				  ap_dimension_t dimension, ap_dim_t dim,
				  const char* prefix);
static inline
bool ap_abstract0_check_dim(ap_funid_t funid, ap_manager_t* man,
			    ap_dimension_t dimension, ap_dim_t dim)
{
  if (dim>=dimension.intdim+dimension.realdim){
    ap_abstract0_check_dim_raise(funid,man,dimension,dim,
				 "incompatible dimension for the abstract value");
    return false;
  } else {
    return true;
  }
}

/* Check that the array of dimensions make sense in the given dimensionality */
bool ap_abstract0_check_dim_array(ap_funid_t funid, ap_manager_t* man,
				  ap_dimension_t dimension, ap_dim_t* tdim, size_t size);

/* ====================================================================== */
/* 0.4 Checking compatibility of arguments: expressions */
/* ====================================================================== */

void ap_abstract0_check_expr_raise(ap_funid_t funid, ap_manager_t* man,
				   ap_dimension_t dimension,
				   ap_dim_t dim,
				   char* prefix);

/* Check that the linear expression makes sense in the given dimensionality */
ap_dim_t ap_abstract0_check_linexpr_check(ap_dimension_t dimension,
					  ap_linexpr0_t* expr);
bool ap_abstract0_check_linexpr(ap_funid_t funid, ap_manager_t* man,
				ap_dimension_t dimension,
				ap_linexpr0_t* expr);

/* Check that the tree expression makes sense in the given dimensionality */
ap_dim_t ap_abstract0_check_texpr_check(ap_dimension_t dimension,
					ap_texpr0_t* expr);
bool ap_abstract0_check_texpr(ap_funid_t funid, ap_manager_t* man,
			      ap_dimension_t dimension,
			      ap_texpr0_t* expr);

/* ====================================================================== */
/* 0.5 Checking compatibility of arguments: array of expressions/constraints/generators */
/* ====================================================================== */

/* Check that array of linear expressions makes sense in the given dimensionality */
bool ap_abstract0_check_linexpr_array(ap_funid_t funid, ap_manager_t* man,
				      ap_dimension_t dimension,
				      ap_linexpr0_t** texpr, size_t size);

/* Check that array of linear constraint makes sense in the given dimensionality */
bool ap_abstract0_check_lincons_array(ap_funid_t funid, ap_manager_t* man,
				      ap_dimension_t dimension,
				      ap_lincons0_array_t* array);

/* Check that array of generator makes sense in the given dimensionality */
bool ap_abstract0_check_generator_array(ap_funid_t funid, ap_manager_t* man,
					ap_dimension_t dimension, ap_generator0_array_t* array);

/* Check that array of tree expressions makes sense in the given dimensionality */
bool ap_abstract0_check_texpr_array(ap_funid_t funid, ap_manager_t* man,
				    ap_dimension_t dimension,
				    ap_texpr0_t** texpr, size_t size);

/* Check that array of tree constraint makes sense in the given dimensionality */
bool ap_abstract0_check_tcons_array(ap_funid_t funid, ap_manager_t* man,
				    ap_dimension_t dimension,
				    ap_tcons0_array_t* array);


ap_abstract0_t*
ap_abstract0_meetjoin(ap_funid_t funid,
		      /* either meet or join */
		      ap_manager_t* man, bool destructive,
		      ap_abstract0_t* a1, ap_abstract0_t* a2);
ap_abstract0_t*
ap_abstract0_asssub_linexpr(ap_funid_t funid,
			    /* either assign or substitute */
			    ap_manager_t* man,
			    bool destructive,
			    ap_abstract0_t* a,
			    ap_dim_t dim, ap_linexpr0_t* expr,
			    ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_asssub_linexpr_array(ap_funid_t funid,
				  /* either assign or substitute */
				  ap_manager_t* man,
				  bool destructive,
				  ap_abstract0_t* a,
				  ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				  ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_asssub_texpr(ap_funid_t funid,
			    /* either assign or substitute */
			  ap_manager_t* man,
			  bool destructive,
			  ap_abstract0_t* a,
			  ap_dim_t dim, ap_texpr0_t* expr,
			  ap_abstract0_t* dest);
ap_abstract0_t*
ap_abstract0_asssub_texpr_array(ap_funid_t funid,
				/* either assign or substitute */
				ap_manager_t* man,
				bool destructive,
				ap_abstract0_t* a,
				ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				ap_abstract0_t* dest);
#ifdef __cplusplus
}
#endif

#endif
