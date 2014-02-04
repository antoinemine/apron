/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_RESIZE_H_
#define _T1P_RESIZE_H_

#include "ap_dimension.h"
#include "ap_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Resize dimensions */
/*********************/
t1p_t* t1p_add_dimensions(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_dimchange_t* dimchange,
		bool project);

t1p_t* t1p_remove_dimensions(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_dimchange_t* dimchange);

t1p_t* t1p_permute_dimensions(ap_manager_t* man,
		bool destructive,
		t1p_t* a,
		ap_dimperm_t* permutation);

#ifdef __cplusplus
}
#endif

#endif
