/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_CONSTRUCTOR_H_
#define _T1P_CONSTRUCTOR_H_

/*
#include "ap_dimension.h"
#include "ap_lincons0.h"
#include "ap_manager.h"
#include "ap_interval.h"
#include "ap_tcons0.h"
#include "ap_generator0.h"
*/

#ifdef __cplusplus
extern "C" {
#endif

/****************/
/* Constructors */
/****************/
/* 1.Basic constructors */
t1p_t* t1p_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
t1p_t* t1p_top(ap_manager_t* man, size_t intdim, size_t realdim);
t1p_t* t1p_of_box(ap_manager_t* man, size_t intdim, size_t realdim, ap_interval_t** tinterval);

/* 2.Accessors */
ap_dimension_t t1p_dimension(ap_manager_t* man, t1p_t* a);

/* 3.Tests */
tbool_t t1p_is_bottom(ap_manager_t* man, t1p_t* a);
tbool_t t1p_is_top(ap_manager_t* man, t1p_t* a);

tbool_t t1p_is_leq(ap_manager_t* man, t1p_t* a, t1p_t* b);
tbool_t t1p_is_eq(ap_manager_t* man, t1p_t* a, t1p_t* b);
tbool_t t1p_is_dimension_unconstrained(ap_manager_t* man, t1p_t* a, ap_dim_t dim);
tbool_t t1p_sat_lincons(ap_manager_t* man, t1p_t* a, ap_lincons0_t* lincons);
tbool_t t1p_sat_interval(ap_manager_t* man, t1p_t* a, ap_interval_t* interval);
tbool_t t1p_sat_tcons(ap_manager_t* man, t1p_t* a, ap_tcons0_t* tcons);

/* 4.Extraction of properties */
ap_interval_t* t1p_bound_texpr(ap_manager_t* man, t1p_t* a, ap_texpr0_t* expr);
ap_interval_t* t1p_bound_dimension(ap_manager_t* man, t1p_t* a, ap_dim_t dim);
ap_interval_t* t1p_bound_linexpr(ap_manager_t* man, t1p_t* a, ap_linexpr0_t* expr);

ap_interval_t** t1p_to_box(ap_manager_t* man, t1p_t* a);
ap_tcons0_array_t t1p_to_tcons_array(ap_manager_t* man, t1p_t* a);
ap_lincons0_array_t t1p_to_lincons_array(ap_manager_t* man, t1p_t* a);
ap_generator0_array_t t1p_to_generator_array(ap_manager_t* man, t1p_t* a);

#ifdef __cplusplus
}
#endif

#endif
