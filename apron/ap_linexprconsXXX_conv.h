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
bool itvXXX_linexpr_set_itvZZZ_linexpr(itvXXX_linexpr_t a, itvZZZ_linexpr_t b, numinternal_t intern);
bool itvXXX_linexpr_array_set_itvZZZ_linexpr_array(itvXXX_linexpr_array_t a, itvZZZ_linexpr_array_t b, numinternal_t intern);
bool itvXXX_lincons_set_itvZZZ_lincons(itvXXX_lincons_t a, itvZZZ_lincons_t b, numinternal_t intern);
bool itvXXX_lincons_array_set_itvZZZ_lincons_array(itvXXX_lincons_array_t a, itvZZZ_lincons_array_t b, numinternal_t intern);

bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, numinternal_t intern);
bool eitvZZZ_set_eitvXXX(eitvZZZ_t a, eitvXXX_t b, numinternal_t intern);
bool itvZZZ_linexpr_set_itvXXX_linexpr(itvZZZ_linexpr_t a, itvXXX_linexpr_t b, numinternal_t intern);
bool itvZZZ_linexpr_array_set_itvXXX_linexpr_array(itvZZZ_linexpr_array_t a, itvXXX_linexpr_array_t b, numinternal_t intern);
bool itvZZZ_lincons_set_itvXXX_lincons(itvZZZ_lincons_t a, itvXXX_lincons_t b, numinternal_t intern);
bool itvZZZ_lincons_array_set_itvXXX_lincons_array(itvZZZ_lincons_array_t a, itvXXX_lincons_array_t b, numinternal_t intern);

ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
