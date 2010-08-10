/* ********************************************************************** */
/* itvXXX_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITVXXX_CONV_H_
#define _ITVXXX_CONV_H_

#include "itvXXX_types.h"
#include "itvMPQ_types.h"
#include "itvD_types.h"
#include "itvMPFR_types.h"

#ifdef __cplusplus
extern "C" {
#endif

MACROZ

bool itvXXX_set_itvZZZ(itvXXX_t a, itvMPQ_t b, numinternal_t intern);
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
