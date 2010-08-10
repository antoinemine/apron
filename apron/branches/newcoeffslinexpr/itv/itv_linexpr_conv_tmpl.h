/* ********************************************************************** */
/* itv_linexpr_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITV_LINEXPR_CONV_H_
#define _ITV_LINEXPR_CONV_H_

#include "itv_linexpr.h"
#include "itvMPQ_linexpr.h"
#include "itvD_linexpr.h"
#include "itvMPFR_linexpr.h"

#ifdef __cplusplus
extern "C" {
#endif

bool itv_linexpr_set_itvMPQ_linexpr(itv_linexpr_t a, itvMPQ_linexpr_t b, numinternal_t intern);
bool itv_linexpr_set_itvD_linexpr(itv_linexpr_t a, itvD_linexpr_t b, numinternal_t intern);
bool itv_linexpr_set_itvMPFR_linexpr(itv_linexpr_t a, itvMPFR_linexpr_t b, numinternal_t intern);
bool itvMPQ_linexpr_set_itv_linexpr(itvMPQ_linexpr_t a, itv_linexpr_t b, numinternal_t intern);
bool itvD_linexpr_set_itv_linexpr(itvD_linexpr_t a, itv_linexpr_t b, numinternal_t intern);
bool itvMPFR_linexpr_set_itv_linexpr(itvMPFR_linexpr_t a, itv_linexpr_t b, numinternal_t intern);

bool itv_linexpr_array_set_itvMPQ_linexpr_array(itv_linexpr_array_t a, itvMPQ_linexpr_array_t b, numinternal_t intern);
bool itv_linexpr_array_set_itvD_linexpr_array(itv_linexpr_array_t a, itvD_linexpr_array_t b, numinternal_t intern);
bool itv_linexpr_array_set_itvMPFR_linexpr_array(itv_linexpr_array_t a, itvMPFR_linexpr_array_t b, numinternal_t intern);
bool itvMPQ_linexpr_array_set_itv_linexpr_array(itvMPQ_linexpr_array_t a, itv_linexpr_array_t b, numinternal_t intern);
bool itvD_linexpr_array_set_itv_linexpr_array(itvD_linexpr_array_t a, itv_linexpr_array_t b, numinternal_t intern);
bool itvMPFR_linexpr_array_set_itv_linexpr_array(itvMPFR_linexpr_array_t a, itv_linexpr_array_t b, numinternal_t intern);

#ifdef __cplusplus
}
#endif

#endif
