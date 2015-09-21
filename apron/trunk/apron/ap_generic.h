
/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_GENERIC_H_
#define _AP_GENERIC_H_

#include "ap_manager.h"
#include "ap_expr0.h"
#include "ap_abstract0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* These functions are dedicated to implementors of domains. They offer generic
   default implementations for some of the operations required by the APRON
   API, when there is no more specific and efficient implementation for the
   domain being implemented.

   To use them, the function allocating manager, which is specific to the domain,
   should put the corresponding pointers in the virtual table to them.

   They manipulated "unboxed" abstract values, which are native to the
   underlying library: they are not yet boxed with the manager in the type
   ap_abstract0_t.
*/

/* ********************************************************************** */
/* I. Constructors, accessors, tests and property extraction */
/* ********************************************************************** */

bool ap_generic_sat_tcons(ap_manager_t* man, void* abs, ap_tcons0_t* cons,
			  ap_scalar_discr_t discr, bool quasilinearize);
  /* This function implements a generic sat_tcons operation using
     ap_linearize_texpr0 and sat_lincons operations. */

ap_interval_t* ap_generic_bound_texpr(ap_manager_t* man, void* abs, ap_texpr0_t* expr,
				      ap_scalar_discr_t discr, bool quasilinearize);
  /* This function implements a generic bound_texpr operation using to_box and 
     ap_eval_texpr0 operations. */
  
ap_tcons0_array_t ap_generic_to_tcons_array(ap_manager_t* man,
					    void* abs);
  /* This function implements a generic to_tcons_array operation using
     to_lincons_array operation. */

/* ********************************************************************** */
/* II. Operations */
/* ********************************************************************** */

/* ============================================================ */
/*  Meet/Join on arrays of abstract values */
/* ============================================================ */

void* ap_generic_meetjoin_array(bool meet,
				ap_manager_t* man,
				void** tab, size_t size);
  /* This function implements a generic meet/join_array operation using copy and
     meet/join operations. */

static inline
void* ap_generic_meet_array(ap_manager_t* man,
			    void** tab, size_t size);
  /* This function implements a generic meet_array operation using copy and
     meet operations. */
static inline
void* ap_generic_join_array(ap_manager_t* man,
			    void** tab, size_t size);
  /* This function implements a generic join_array operation using copy and
     meet operations. */

/* ============================================================ */
/*  Meet with array of constraints */
/* ============================================================ */

void* ap_generic_meet_quasilinearize_lincons_array(ap_manager_t* man,
						   bool destructive, void* abs, ap_lincons0_array_t* array,
						   ap_scalar_discr_t discr, bool linearize,
						   void* (*meet_lincons_array)(ap_manager_t*, 
									       bool, void*,ap_lincons0_array_t*));

void*
ap_generic_meet_intlinearize_tcons_array(ap_manager_t* man,
					 bool destructive, void* abs, ap_tcons0_array_t* array,
					 ap_scalar_discr_t discr, ap_linexpr_type_t linearize,
					 void* (*meet_lincons_array)(ap_manager_t*,
								     bool, void*,
								     ap_lincons0_array_t*));

/* ============================================================ */
/*  Assignments/Substitutions */
/* ============================================================ */

void* ap_generic_asssub_linexpr_array(bool assign,
				      ap_manager_t* man,
				      bool destructive, void* abs, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				      void* dest);
void* ap_generic_asssub_texpr_array(bool assign,
				    ap_manager_t* man,
				    bool destructive, void* abs, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				    void* dest);
  /*
    These functions implement generic parallel assignment/substitution
    operations by:
    1. introducing primed dimensions
    2. transforming linear expressions into equality constraints relating the
    assigned primed dimension and the linear expression
    If dest!=NULL
      3. introducing primed dimensions in dest
      4. exchanging primed and unprimed dimensions in dest
      5. intersecting the abstract value with the modified dest
    6. intersecting the obtained abstract value with the constraints
    7. exchanging primed and unprimed dimensions
    8. removing the introduced (primed) dimensions
    
   It relies on: is_bottom, copy, dimension, add_dimensions,
   permute_dimensions, remove_dimensions, meet_lincons_array/meet_tcons_array, meet and free
   abstract operations.
   
   Meaning of parameters:
   - assign selects the operation: true means assignment, false substitution
   - The other parameters have the meaning they have for parallel
     assignment/substitution
*/
static inline
void* ap_generic_assign_linexpr_array(ap_manager_t* man,
				      bool destructive, void* abs, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				      void* dest);
static inline
void* ap_generic_assign_texpr_array(ap_manager_t* man,
				    bool destructive, void* abs, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				    void* dest);
  /*
     These functions implement generic parallel assignment operations by
     relying on is_bottom, copy, dimension, add_dimensions, permute_dimensions,
     remove_dimensions, meet_lincons_array or meet_tcons_array abstract
     operations.
  */
static inline
void* ap_generic_substitute_linexpr_array(ap_manager_t* man,
					  bool destructive, void* abs, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
					  void* dest);
static inline
void* ap_generic_substitute_texpr_array(ap_manager_t* man,
					bool destructive, void* abs, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
					void* dest);
  /*
     These functions implement generic parallel assignment operations by
     relying on is_bottom, copy, dimension, add_dimensions, permute_dimensions,
     remove_dimensions, meet_lincons_array or meet_tcons_array abstract
     operations.
  */

/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */
static inline 
void* ap_generic_meet_array(ap_manager_t* man,
			    void** tab, size_t size)
{ return ap_generic_meetjoin_array(true,man,tab,size); }

static inline 
void* ap_generic_join_array(ap_manager_t* man,
			    void** tab, size_t size)
{ return ap_generic_meetjoin_array(false,man,tab,size); }

static inline
void* ap_generic_assign_linexpr_array(ap_manager_t* man,
				      bool destructive, void* abs, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				      void* dest)
{
  return ap_generic_asssub_linexpr_array(true,
					 man, destructive, abs, tdim, texpr, size,
					 dest);
}
static inline
void* ap_generic_substitute_linexpr_array(ap_manager_t* man,
					  bool destructive, void* abs, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
					  void* dest)
{
  return ap_generic_asssub_linexpr_array(false,
					 man, destructive, abs, tdim, texpr, size,
					 dest);
}

static inline
void* ap_generic_assign_texpr_array(ap_manager_t* man,
				    bool destructive, void* abs, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				    void* dest)
{
  return ap_generic_asssub_texpr_array(true,
				       man, destructive, abs, tdim, texpr, size,
				       dest);
}
static inline
void* ap_generic_substitute_texpr_array(ap_manager_t* man,
					bool destructive,
					void* abs,
					ap_dim_t* tdim,
					ap_texpr0_t** texpr,
					size_t size,
					void* dest)
{
  return ap_generic_asssub_texpr_array(false,
				       man, destructive, abs, tdim, texpr, size,
				       dest);
}

#ifdef __cplusplus
}
#endif

#endif
