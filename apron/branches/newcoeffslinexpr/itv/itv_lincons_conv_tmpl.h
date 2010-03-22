/* ********************************************************************** */
/* itv_lincons_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITV_LINCONS_CONV_H_
#define _ITV_LINCONS_CONV_H_

#include "num_conv.h"
#include "eitv_conv.h"
#include "itv_linexpr_conv.h"
#include "itv_lincons.h"
#include "itvMPQ_lincons.h"
#include "itvD_lincons.h"
#include "itvMPFR_lincons.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool itv_lincons_set_itvMPQ_lincons(itv_lincons_t a, itvMPQ_lincons_t b, numinternal_t intern);
static inline bool itv_lincons_set_itvD(itv_lincons_t a, itvD_lincons_t b, numinternal_t intern);
static inline bool itv_lincons_set_itvMPFR(itv_lincons_t a, itvMPFR_lincons_t b, numinternal_t intern);
static inline bool itvMPQ_set_itv_lincons(itvMPQ_lincons_t a, itv_lincons_t b, numinternal_t intern);
static inline bool itvD_set_itv_lincons(itvD_lincons_t a, itv_lincons_t b, numinternal_t intern);
static inline bool itvMPFR_set_itv_lincons(itvMPFR_lincons_t a, itv_lincons_t b, numinternal_t intern);

#define itvlincons_set_itvXXXlincons(XXX) \
  static inline bool itv_lincons_set_itv##XXX##_lincons(itv_lincons_t a, itv##XXX##_lincons_t b, numinternal_t intern) \
  {									\
    bool res;								\
    res = itv_linexpr_set_itv##XXX##_linexpr(a->linexpr,b->linexpr,intern); \
    a->constyp = b->constyp;						\
    res = num_set_num##XXX(a->num,b->num) && res;			\
    return res;								\
  }

#define itvXXXlincons_set_itvlincons(XXX) \
  static inline bool itv##XXX##_lincons_set_itv_lincons(itv##XXX##_lincons_t a, itv_lincons_t b, numinternal_t intern) \
  {									\
    bool res;								\
    res = itv##XXX##_linexpr_set_itv_linexpr(a->linexpr,b->linexpr,intern); \
    a->constyp = b->constyp;						\
    res = num_set_num##XXX(a->num,b->num) && res;			\
    return res;								\
  }
  
#if !(NUM_NUMMPQ && (defined(_ITVD_LINCONS_CONV_H_) || defined(_ITVMPFR_LINCONS_CONV_H_)))
itvlincons_set_itvXXXlincons(MPQ)
#endif
#if !NUM_NUMMPQ && !(defined(_ITVD_LINCONS_CONV_H_) || defined(_ITVMPFR_LINCONS_CONV_H_))
itvXXXlincons_set_itvlincons(MPQ)
#endif

#if !(NUM_NUMD && (defined(_ITVMPQ_LINCONS_CONV_H_) || defined(_ITVMPFR_LINCONS_CONV_H_)))
itvlincons_set_itvXXXlincons(D)
#endif
#if !NUM_NUMD && !(defined(_ITVMPQ_LINCONS_CONV_H_) || defined(_ITVMPFR_LINCONS_CONV_H_))
itvXXXlincons_set_itvlincons(D)
#endif

#if !(NUM_NUMMPFR && (defined(_ITVD_LINCONS_CONV_H_) || defined(_ITVMPQ_LINCONS_CONV_H_)))
itvlincons_set_itvXXXlincons(MPFR)
#endif
#if !NUM_NUMMPFR && !(defined(_ITVD_LINCONS_CONV_H_) || defined(_ITVMPQ_LINCONS_CONV_H_))
itvXXXlincons_set_itvlincons(MPFR)
#endif

#ifdef __cplusplus
}
#endif

#endif
