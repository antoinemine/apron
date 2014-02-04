/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_H_
#define _T1P_H_

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

//ap_manager_t* t1p_manager_alloc(ap_manager_t* manNS);
ap_manager_t* t1p_manager_alloc(void);
typedef struct _t1p_t t1p_t;

#include "t1p_representation.h"
/* ********************************************************************** */
/* 1. Manager allocation */
/* ********************************************************************** */
//ap_manager_t* t1p_manager_alloc(ap_manager_t* manNS);
//ap_manager_t* t1p_manager_alloc(void);
/* ********************************************************************** */
/* 1. Memory */
/* ********************************************************************** */
/* ********************************************************************** */
/* 2. Control of internal representation */
/* ********************************************************************** */
/* ********************************************************************** */
/* 3. Printing */
/* ********************************************************************** */
/* ********************************************************************** */
/* 4. Serialisation */
/* ********************************************************************** */

/****************/
/* Constructors */
/****************/
/* 1.Basic constructors */
/* 2.Accessors */
/* 3.Tests */
/* 4.Extraction of properties */
#include "t1p_constructor.h"

/* Meet and Join */
/*****************/
/* 1.Meet */
/* 2.Join */
#include "t1p_meetjoin.h"

/* Assign and Substitute */
/*************************/
#include "t1p_assign.h"

/* Resize dimensions */
/*********************/
#include "t1p_resize.h"

/* Other functions */
/*******************/
#include "t1p_otherops.h"

//typedef struct _ap_abstract1_t ap_abstract1_t;
//typedef struct _ap_var_t ap_var_t;
//int ap_manager_t1p_get_nsym(ap_manager_t* man);
//void ap_abstract1_aff_build(ap_manager_t* man, ap_abstract1_t *abstract, ap_var_t var, unsigned int index, ap_interval_t *itv, bool isunion);
//void ap_abstract1_ns_meet_lincons_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_lincons0_array_t* lincons);
//void ap_abstract1_ns_meet_box_array(ap_manager_t* man, ap_abstract1_t* abstract1, ap_interval_t** box, size_t size);

/* retourne [0,+oo] */
/* for internal use */
/* pour test */
//ap_interval_t* t1p_create_pos(ap_manager_t* man);

#ifdef __cplusplus
}
#endif

#endif
