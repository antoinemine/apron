/* ********************************************************************** */
/* boxXXX_resize.h */
/* ********************************************************************** */

#ifndef _BOX_XXX_RESIZE_H_
#define _BOX_XXX_RESIZE_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

boxXXX_t* boxXXX_add_dimensions(ap_manager_t* man,
			  bool destructive, boxXXX_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);

boxXXX_t* boxXXX_remove_dimensions(ap_manager_t* man,
			     bool destructive, boxXXX_t* a,
			     ap_dimchange_t* dimchange);

boxXXX_t* boxXXX_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dimperm_t* permutation);

#ifdef __cplusplus
}
#endif

#endif
