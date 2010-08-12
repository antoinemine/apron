/* ********************************************************************** */
/* ap_linexprconsXXX.h: conversion between linear expressions and constraints */
/* ********************************************************************** */

#ifndef _AP_LINEXPRCONSXXX.H_
#define _AP_LINEXPRCONSXXX.H_

#include "ap_linexprXXX.h"
#include "ap_linconsXXX.h"
#include "ap_linexprD.h"
#include "ap_linconsD.h"
#include "ap_linexprMPQ.h"
#include "ap_linconsMPQ.h"
#include "ap_linexprMPFR.h"
#include "ap_linconsMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

MACROZ

bool itvXXX_set_numZZZ(itvXXX_t a, numZZZ_t b, numinternal_t intern);
bool itvXXX_set_numZZZ2(itvXXX_t a, numZZZ_t b, numZZZ_t c, numinternal_t intern);
bool eitvXXX_set_numZZZ(eitvXXX_t a, numZZZ_t b, numinternal_t intern);
bool eitvXXX_set_numZZZ2(eitvXXX_t a, numZZZ_t b, numZZZ_t c, numinternal_t intern);

bool itvXXX_set_itvZZZ(itvXXX_t a, itvZZZ_t b, numinternal_t intern);
bool eitvXXX_set_eitvZZZ(eitvXXX_t a, eitvZZZ_t b, numinternal_t intern);
bool ap_linexprXXX_set_ap_linexprZZZ(ap_linexprXXX_t a, ap_linexprZZZ_t b, numinternal_t intern);
bool ap_linexprXXX_array_set_ap_linexprZZZ_array(ap_linexprXXX_array_t a, ap_linexprZZZ_array_t b, numinternal_t intern);
bool ap_linconsXXX_set_ap_linconsZZZ(ap_linconsXXX_t a, ap_linconsZZZ_t b, numinternal_t intern);
bool ap_linconsXXX_array_set_ap_linconsZZZ_array(ap_linconsXXX_array_t a, ap_linconsZZZ_array_t b, numinternal_t intern);

bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, numinternal_t intern);
bool eitvZZZ_set_eitvXXX(eitvZZZ_t a, eitvXXX_t b, numinternal_t intern);
bool ap_linexprZZZ_set_ap_linexprXXX(ap_linexprZZZ_t a, ap_linexprXXX_t b, numinternal_t intern);
bool ap_linexprZZZ_array_set_ap_linexprXXX_array(ap_linexprZZZ_array_t a, ap_linexprXXX_array_t b, numinternal_t intern);
bool ap_linconsZZZ_set_ap_linconsXXX(ap_linconsZZZ_t a, ap_linconsXXX_t b, numinternal_t intern);
bool ap_linconsZZZ_array_set_ap_linconsXXX_array(ap_linconsZZZ_array_t a, ap_linconsXXX_array_t b, numinternal_t intern);

ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
