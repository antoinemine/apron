/* ********************************************************************** */
/* boxXXX_constructor.h: constructors */
/* ********************************************************************** */

#ifndef _BOX_XXX_CONSTRUCTOR_H_
#define _BOX_XXX_CONSTRUCTOR_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

void boxXXX_bound_linexpr_internal(boxXXX_internal_t* intern,
				eitvXXX_t itv,
				boxXXX_t* a, 
				ap_linexpr0_t* expr);
boxXXX_t* boxXXX_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
boxXXX_t* boxXXX_top(ap_manager_t* man, size_t intdim, size_t realdim);
boxXXX_t* boxXXX_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
boxXXX_t* boxXXX_of_lincons_array(ap_manager_t* man,
			    size_t intdim, size_t realdim,
			    ap_lincons0_array_t* array);
boxXXX_t* boxXXX_of_tcons_array(ap_manager_t* man,
			  size_t intdim, size_t realdim,
			  ap_tcons0_array_t* array);
ap_dimension_t boxXXX_dimension(ap_manager_t* man, boxXXX_t* a);
bool boxXXX_is_bottom(ap_manager_t* man, boxXXX_t* a);
bool boxXXX_is_top(ap_manager_t* man, boxXXX_t* a);
bool boxXXX_is_leq(ap_manager_t* man, boxXXX_t* a, boxXXX_t* b);
bool boxXXX_is_eq(ap_manager_t* man, boxXXX_t* a, boxXXX_t* b);
bool boxXXX_is_dimension_unconstrained(ap_manager_t* man, boxXXX_t* a, ap_dim_t dim);
bool boxXXX_sat_interval(ap_manager_t* man, 
		      boxXXX_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
bool boxXXX_sat_lincons(ap_manager_t* man, 
		     boxXXX_t* a, ap_lincons0_t* cons);
bool boxXXX_sat_tcons(ap_manager_t* man, 
		   boxXXX_t* a, ap_tcons0_t* cons);
ap_interval_t* boxXXX_bound_dimension(ap_manager_t* man,
				   boxXXX_t* a, ap_dim_t dim);
ap_interval_t* boxXXX_bound_linexpr(ap_manager_t* man,
				 boxXXX_t* a, ap_linexpr0_t* expr);
ap_interval_t* boxXXX_bound_texpr(ap_manager_t* man,
			       boxXXX_t* a, ap_texpr0_t* expr);
ap_lincons0_array_t boxXXX_to_lincons_array(ap_manager_t* man, boxXXX_t* a);
ap_tcons0_array_t boxXXX_to_tcons_array(ap_manager_t* man, boxXXX_t* a);
ap_generator0_array_t boxXXX_to_generator_array(ap_manager_t* man, boxXXX_t* a);
ap_interval_t** boxXXX_to_box(ap_manager_t* man, boxXXX_t* a);

#ifdef __cplusplus
}
#endif

#endif
