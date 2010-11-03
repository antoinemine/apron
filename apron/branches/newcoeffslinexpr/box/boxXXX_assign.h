/* ********************************************************************** */
/* boxXXX_assign.h */
/* ********************************************************************** */

#ifndef _BOX_XXX_ASSIGN_H_
#define _BOX_XXX_ASSIGN_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

boxXXX_t* boxXXX_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				boxXXX_t* a,
				ap_dim_t* tdim, 
				ap_linexpr0_t** texpr,
				size_t size,
				boxXXX_t* dest);
boxXXX_t* boxXXX_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive,
				    boxXXX_t* a,
				    ap_dim_t* tdim, 
				    ap_linexpr0_t** texpr,
				    size_t size,
				    boxXXX_t* dest);
boxXXX_t* boxXXX_assign_texpr_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dim_t* tdim, 
			      ap_texpr0_t** texpr,
			      size_t size,
			      boxXXX_t* dest);
boxXXX_t* boxXXX_substitute_texpr_array(ap_manager_t* man,
				  bool destructive,
				  boxXXX_t* a,
				  ap_dim_t* tdim, 
				  ap_texpr0_t** texpr,
				  size_t size,
				  boxXXX_t* dest);


#ifdef __cplusplus
}
#endif

#endif
