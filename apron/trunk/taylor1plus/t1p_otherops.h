/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_OTHEROPS_H_
#define _T1P_OTHEROPS_H_

#include "ap_var.h"
#include "ap_abstract1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Other functions */
/*******************/

int ap_manager_t1p_get_nsym(ap_manager_t* man);
 
void ap_abstract1_aff_build(ap_manager_t* man, ap_abstract1_t * abstract, ap_var_t var, unsigned int index, ap_interval_t *itv, bool isunion);
void ap_abstract1_ns_meet_lincons_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_lincons0_array_t* lincons);
void ap_abstract1_ns_meet_box_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_interval_t** box, size_t size);

t1p_t* t1p_forget_array(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_dim_t* tdim, size_t size,
		bool project);

t1p_t t1p_expand(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_var_t var,
		ap_var_t* tvar, size_t size);

t1p_t t1p_fold(ap_manager_t* man,
		bool destructive, t1p_t* a,
		ap_var_t* tvar, size_t size);

t1p_t* t1p_widening(ap_manager_t* man,
		  t1p_t* a1, 
		  t1p_t* a2);

t1p_t t1p_closure(ap_manager_t* man, bool destructive, t1p_t* a);

/* retourne [0,+oo] */
/* for internal use */
/* pour test */
ap_interval_t* t1p_create_pos(ap_manager_t* man);

#ifdef __cplusplus
}
#endif

#endif

