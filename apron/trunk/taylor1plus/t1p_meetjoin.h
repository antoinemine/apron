/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_MEETJOIN_H_
#define _T1P_MEETJOIN_H_

/*
#include "t1p_representation.h"
#include "ap_manager.h"
#include "ap_tcons0.h"
#include "ap_lincons0.h"
#include "ap_generator0.h"
#include "ap_generic.h"
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Meet and Join */
/*****************/
/* 1.Meet */
t1p_t* t1p_meet(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2);
t1p_t* t1p_meet_array(ap_manager_t* man, t1p_t** tab, size_t size);
t1p_t* t1p_meet_lincons_array(ap_manager_t* man,
		bool destructive,
		t1p_t* a,
		ap_lincons0_array_t* array);
t1p_t* t1p_meet_tcons_array(ap_manager_t* man,
		bool destructive,
		t1p_t* a,
		ap_tcons0_array_t* array);

/* 2.Join */
t1p_t* t1p_join(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2);
t1p_t* t1p_join_array(ap_manager_t* man, t1p_t** tab, size_t size);
t1p_t* t1p_add_ray_array(ap_manager_t* man,
		bool destructive,
		t1p_t* a, ap_generator0_array_t* array);

#ifdef __cplusplus
}
#endif

#endif
