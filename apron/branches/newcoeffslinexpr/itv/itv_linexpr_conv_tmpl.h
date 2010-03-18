/* ********************************************************************** */
/* itv_linexpr_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITV_LINEXPR_CONV_H_
#define _ITV_LINEXPR_CONV_H_

#include "eitv_conv.h"
#include "itv_linexpr.h"
#include "itvMPQ_linexpr.h"
#include "itvD_linexpr.h"
#include "itvMPFR_linexpr.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool itv_linexpr_set_itvMPQ_linexpr(itv_linexpr_t a, itvMPQ_linexpr_t b, numinternal_t intern);
static inline bool itv_linexpr_set_itvD(itv_linexpr_t a, itvD_linexpr_t b, numinternal_t intern);
static inline bool itv_linexpr_set_itvMPFR(itv_linexpr_t a, itvMPFR_linexpr_t b, numinternal_t intern);
static inline bool itvMPQ_set_itv_linexpr(itvMPQ_linexpr_t a, itv_linexpr_t b, numinternal_t intern);
static inline bool itvD_set_itv_linexpr(itvD_linexpr_t a, itv_linexpr_t b, numinternal_t intern);
static inline bool itvMPFR_set_itv_linexpr(itvMPFR_linexpr_t a, itv_linexpr_t b, numinternal_t intern);

#define itvlinexpr_set_itvXXXlinexpr(XXX) \
  static inline bool itv_linexpr_set_itv##XXX##_linexpr(itv_linexpr_t a, itv##XXX##_linexpr_t b, numinternal_t intern) \
  {									\
    bool res = eitv_set_eitv##XXX(a->cst,b->cst);			\
    itv_linexpr_reinit(a,b->size);					\
    for (i=0; i<a->size; i++){						\
      a->linterm[i]->dim = b->linterm[i]->dim;				\
      res = eitv_set_eitv##XXX(a->linterm[i]->eitv,b->linterm[i]->eitv) || res; \
    }									\
    return res;								\
  }

#define itvXXXlinexpr_set_itvlinexpr(XXX) \
  static inline bool itv##XXX##_linexpr_set_itv_linexpr(itv##XXX##_linexpr_t a, itv_linexpr_t b, numinternal_t intern) \
  {									\
    bool res = eitv##XXX##_set_eitv(a->cst,b->cst);			\
    itv_linexpr_reinit(a,b->size);					\
    for (i=0; i<a->size; i++){						\
      a->linterm[i]->dim = b->linterm[i]->dim;				\
      res = eitv##XXX_set_eitv(a->linterm[i]->eitv,b->linterm[i]->eitv) || res; \
    }									\
    return res;								\
  }

#if !(NUM_NUMMPQ && (defined(_ITVD_LINEXPR_CONV_H_) || defined(_ITVMPFR_LINEXPR_CONV_H_)))
itvlinexpr_set_itvXXXlinexpr(MPQ)
#endif
#if !NUM_NUMMPQ && !(defined(_ITVD_LINEXPR_CONV_H_) || defined(_ITVMPFR_LINEXPR_CONV_H_))
itvXXXlinexpr_set_itvlinexpr(MPQ)
#endif

#if !(NUM_NUMD && (defined(_ITVMPQ_LINEXPR_CONV_H_) || defined(_ITVMPFR_LINEXPR_CONV_H_)))
itvlinexpr_set_itvXXXlinexpr(D)
#endif
#if !NUM_NUMD && !(defined(_ITVMPQ_LINEXPR_CONV_H_) || defined(_ITVMPFR_LINEXPR_CONV_H_))
itvXXXlinexpr_set_itvlinexpr(D)
#endif

#if !(NUM_NUMMPFR && (defined(_ITVD_LINEXPR_CONV_H_) || defined(_ITVMPQ_LINEXPR_CONV_H_)))
itvlinexpr_set_itvXXXlinexpr(MPFR)
#endif
#if !NUM_NUMMPFR && !(defined(_ITVD_LINEXPR_CONV_H_) || defined(_ITVMPQ_LINEXPR_CONV_H_))
itvXXXlinexpr_set_itvlinexpr(MPFR)
#endif

#ifdef __cplusplus
}
#endif

#endif
