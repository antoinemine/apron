/* ********************************************************************** */
/* box_constructor.h: constructors */
/* ********************************************************************** */

#ifndef _BOX_CONSTRUCTOR_H_
#define _BOX_CONSTRUCTOR_H_

#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

void box_bound_linexpr_internal(box_internal_t* intern,
				itv_t itv,
				box_t* a, 
				ap_linexpr0_t* expr);
box_t* box_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
box_t* box_top(ap_manager_t* man, size_t intdim, size_t realdim);
box_t* box_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
box_t* box_of_lincons_array(ap_manager_t* man,
			    size_t intdim, size_t realdim,
			    ap_lincons0_array_t* array);
box_t* box_of_tcons_array(ap_manager_t* man,
			  size_t intdim, size_t realdim,
			  ap_tcons0_array_t* array);
ap_dimension_t box_dimension(ap_manager_t* man, box_t* a);
bool box_is_bottom(ap_manager_t* man, box_t* a);
bool box_is_top(ap_manager_t* man, box_t* a);
bool box_is_leq(ap_manager_t* man, box_t* a, box_t* b);
bool box_is_eq(ap_manager_t* man, box_t* a, box_t* b);
bool box_is_dimension_unconstrained(ap_manager_t* man, box_t* a, ap_dim_t dim);
bool box_sat_interval(ap_manager_t* man, 
		      box_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
bool box_sat_lincons(ap_manager_t* man, 
		     box_t* a, ap_lincons0_t* cons);
bool box_sat_tcons(ap_manager_t* man, 
		   box_t* a, ap_tcons0_t* cons);
ap_interval_t* box_bound_dimension(ap_manager_t* man,
				   box_t* a, ap_dim_t dim);
ap_interval_t* box_bound_linexpr(ap_manager_t* man,
				 box_t* a, ap_linexpr0_t* expr);
ap_interval_t* box_bound_texpr(ap_manager_t* man,
			       box_t* a, ap_texpr0_t* expr);
ap_lincons0_array_t box_to_lincons_array(ap_manager_t* man, box_t* a);
ap_tcons0_array_t box_to_tcons_array(ap_manager_t* man, box_t* a);
ap_generator0_array_t box_to_generator_array(ap_manager_t* man, box_t* a);
ap_interval_t** box_to_box(ap_manager_t* man, box_t* a);

#ifdef __cplusplus
}
#endif

#endif
