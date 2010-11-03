/* ********************************************************************** */
/* boxXXX_meetjoin.h */
/* ********************************************************************** */

#ifndef _BOX_XXX_MEETJOIN_H_
#define _BOX_XXX_MEETJOIN_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

void boxXXX_add_ray(boxXXX_internal_t* intern,
		 boxXXX_t* a, ap_generator0_t* gen);

boxXXX_t* boxXXX_meet(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2);
boxXXX_t* boxXXX_join(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2);
boxXXX_t* boxXXX_meet_array(ap_manager_t* man, boxXXX_t** tab, size_t size);
boxXXX_t* boxXXX_join_array(ap_manager_t* man, boxXXX_t** tab, size_t size);
boxXXX_t* boxXXX_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_lincons0_array_t* array);
boxXXX_t* boxXXX_meet_tcons_array(ap_manager_t* man,
			    bool destructive,
			    boxXXX_t* a,
			    ap_tcons0_array_t* array);
boxXXX_t* boxXXX_add_ray_array(ap_manager_t* man,
			 bool destructive,
			 boxXXX_t* a, ap_generator0_array_t* array);

#ifdef __cplusplus
}
#endif

#endif
