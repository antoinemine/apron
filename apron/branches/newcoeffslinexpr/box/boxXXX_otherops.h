/* ********************************************************************** */
/* boxXXX_otherops.h */
/* ********************************************************************** */

#ifndef _BOX_XXX_OTHEROPS_H_
#define _BOX_XXX_OTHEROPS_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

boxXXX_t* boxXXX_forget_array(ap_manager_t* man,
			bool destructive,
			boxXXX_t* a,
			ap_dim_t* tdim,
			size_t size,
			bool project);
boxXXX_t* boxXXX_expand(ap_manager_t* man,
		  bool destructive,
		  boxXXX_t* a,
		  ap_dim_t dim,
		  size_t n);
boxXXX_t* boxXXX_fold(ap_manager_t* man,
		bool destructive,
		boxXXX_t* a,
		ap_dim_t* tdim,
		size_t size);

boxXXX_t* boxXXX_widening(ap_manager_t* man,
		    boxXXX_t* a1, boxXXX_t* a2);

boxXXX_t* boxXXX_closure(ap_manager_t* man, bool destructive, boxXXX_t* a);

#ifdef __cplusplus
}
#endif

#endif
