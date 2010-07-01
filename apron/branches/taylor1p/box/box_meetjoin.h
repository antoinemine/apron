/* ********************************************************************** */
/* box_meetjoin.h */
/* ********************************************************************** */

#ifndef _BOX_MEETJOIN_H_
#define _BOX_MEETJOIN_H_

#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

void box_add_ray(box_internal_t* intern,
		 box_t* a, ap_generator0_t* gen);

box_t* box_meet(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2);
box_t* box_join(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2);
box_t* box_meet_array(ap_manager_t* man, box_t** tab, size_t size);
box_t* box_join_array(ap_manager_t* man, box_t** tab, size_t size);
box_t* box_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_lincons0_array_t* array);
box_t* box_meet_tcons_array(ap_manager_t* man,
			    bool destructive,
			    box_t* a,
			    ap_tcons0_array_t* array);
box_t* box_add_ray_array(ap_manager_t* man,
			 bool destructive,
			 box_t* a, ap_generator0_array_t* array);

#ifdef __cplusplus
}
#endif

#endif
