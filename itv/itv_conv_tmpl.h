/* ********************************************************************** */
/* itv_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITV_CONV_H_
#define _ITV_CONV_H_

#include "itv.h"
#include "itvMPQ.h"
#include "itvD.h"
#include "itvMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

bool itv_set_itvMPQ(itv_t a, itvMPQ_t b, numinternal_t intern);
bool itv_set_itvD(itv_t a, itvD_t b, numinternal_t intern);
bool itv_set_itvMPFR(itv_t a, itvMPFR_t b, numinternal_t intern);
bool itvMPQ_set_itv(itvMPQ_t a, itv_t b, numinternal_t intern);
bool itvD_set_itv(itvD_t a, itv_t b, numinternal_t intern);
bool itvMPFR_set_itv(itvMPFR_t a, itv_t b, numinternal_t intern);

#ifdef __cplusplus
}
#endif

#endif
