/* ********************************************************************** */
/* pk_assign.h: Assignements and Substitutions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license 
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_ASSIGN_H_
#define _PK_ASSIGN_H_

#include "pk_config.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

pk_t* poly_asssub_linexpr_det(bool assign,
			      ap_manager_t* man,
			      bool destructive,
			      pk_t* pa,
			      ap_dim_t dim, ap_linexpr0_t* linexpr);

pk_t* poly_asssub_linexpr_array_det(bool assign,
				    ap_manager_t* man,
				    bool destructive,
				    pk_t* pa,
				    ap_dim_t* tdim, ap_linexpr0_t** texpr, 
				    size_t size);

#ifdef __cplusplus
}
#endif

#endif
