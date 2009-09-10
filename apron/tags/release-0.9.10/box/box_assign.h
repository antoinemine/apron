/* ********************************************************************** */
/* box_assign.h */
/* ********************************************************************** */

#ifndef _BOX_ASSIGN_H_
#define _BOX_ASSIGN_H_

#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

box_t* box_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				box_t* a,
				ap_dim_t* tdim, 
				ap_linexpr0_t** texpr,
				size_t size,
				box_t* dest);
box_t* box_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive,
				    box_t* a,
				    ap_dim_t* tdim, 
				    ap_linexpr0_t** texpr,
				    size_t size,
				    box_t* dest);
box_t* box_assign_texpr_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_dim_t* tdim, 
			      ap_texpr0_t** texpr,
			      size_t size,
			      box_t* dest);
box_t* box_substitute_texpr_array(ap_manager_t* man,
				  bool destructive,
				  box_t* a,
				  ap_dim_t* tdim, 
				  ap_texpr0_t** texpr,
				  size_t size,
				  box_t* dest);


#ifdef __cplusplus
}
#endif

#endif
