/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_ASSIGN_H_
#define _T1P_ASSIGN_H_

/*
#include "ap_dimension.h"
#include "ap_texpr0.h"
#include "ap_generic.h"
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Assign and Substitute */
/*************************/
t1p_t* t1p_assign_linexpr_array(ap_manager_t* man,
		bool destructive,
		t1p_t* org,
		ap_dim_t* tdim, ap_linexpr0_t** lexpr,
		size_t size,
		t1p_t* dest);

t1p_t* t1p_substitute_linexpr_array(ap_manager_t* man,
		bool destructive,
		t1p_t* org,
		ap_dim_t* tdim, ap_linexpr0_t** lexpr, 
		size_t size,
		t1p_t* dest);

t1p_t* t1p_assign_texpr_array(ap_manager_t* man,
		bool destructive,
		t1p_t* a,
		ap_dim_t* tdim,
		ap_texpr0_t** texpr,
		size_t size,
		t1p_t* dest);

t1p_t* t1p_substitute_texpr_array(ap_manager_t* man,
		bool destructive,
		t1p_t* org,
		ap_dim_t* tdim, ap_texpr0_t** texpr,
		size_t size,
		t1p_t* dest);

#ifdef __cplusplus
}
#endif

#endif
