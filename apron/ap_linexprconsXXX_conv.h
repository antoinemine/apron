/* ********************************************************************** */
/* ap_linexprconsXXX_conv.h: conversion between linear expressions and constraints */
/* ********************************************************************** */

#ifndef _AP_LINEXPRCONSXXX_CONV_H_
#define _AP_LINEXPRCONSXXX_CONV_H_

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

MACRO_MAINZ
bool ap_linexprXXX_set_linexprZZZ(ap_linexprXXX_t a, ap_linexprZZZ_t b, num_internal_t intern);
bool ap_linexprXXX_array_set_linexprZZZ_array(ap_linexprXXX_array_t a, ap_linexprZZZ_array_t b, num_internal_t intern);
bool ap_linconsXXX_set_linconsZZZ(ap_linconsXXX_t a, ap_linconsZZZ_t b, num_internal_t intern);
bool ap_linconsXXX_array_set_linconsZZZ_array(ap_linconsXXX_array_t a, ap_linconsZZZ_array_t b, num_internal_t intern);

bool ap_linexprZZZ_set_linexprXXX(ap_linexprZZZ_t a, ap_linexprXXX_t b, num_internal_t intern);
bool ap_linexprZZZ_array_set_linexprXXX_array(ap_linexprZZZ_array_t a, ap_linexprXXX_array_t b, num_internal_t intern);
bool ap_linconsZZZ_set_linconsXXX(ap_linconsZZZ_t a, ap_linconsXXX_t b, num_internal_t intern);
bool ap_linconsZZZ_array_set_linconsXXX_array(ap_linconsZZZ_array_t a, ap_linconsXXX_array_t b, num_internal_t intern);
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
