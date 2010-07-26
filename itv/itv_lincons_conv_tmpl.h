/* ********************************************************************** */
/* itv_lincons_conv.h: conversion between itvs */
/* ********************************************************************** */

#ifndef _ITV_LINCONS_CONV_H_
#define _ITV_LINCONS_CONV_H_

#include "itv_linexpr_conv.h"
#include "itv_lincons.h"
#include "itvMPQ_lincons.h"
#include "itvD_lincons.h"
#include "itvMPFR_lincons.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool itv_lincons_set_itvMPQ_lincons(itv_lincons_t a, itvMPQ_lincons_t b, numinternal_t intern);
static inline bool itv_lincons_set_itvD_lincons(itv_lincons_t a, itvD_lincons_t b, numinternal_t intern);
static inline bool itv_lincons_set_itvMPFR_lincons(itv_lincons_t a, itvMPFR_lincons_t b, numinternal_t intern);
static inline bool itvMPQ_lincons_set_itv_lincons(itvMPQ_lincons_t a, itv_lincons_t b, numinternal_t intern);
static inline bool itvD_lincons_set_itv_lincons(itvD_lincons_t a, itv_lincons_t b, numinternal_t intern);
static inline bool itvMPFR_lincons_set_itv_lincons(itvMPFR_lincons_t a, itv_lincons_t b, numinternal_t intern);

bool itv_lincons_array_set_itvMPQ_lincons_array(itv_lincons_array_t a, itvMPQ_lincons_array_t b, numinternal_t intern);
bool itv_lincons_array_set_itvD_lincon_arrays(itv_lincons_array_t a, itvD_lincons_array_t b, numinternal_t intern);
bool itv_lincons_array_set_itvMPFR_lincons_array(itv_lincons_array_t a, itvMPFR_lincons_array_t b, numinternal_t intern);
bool itvMPQ_lincons_array_set_itv_lincons_array(itvMPQ_lincons_array_t a, itv_lincons_array_t b, numinternal_t intern);
bool itvD_lincons_array_set_itv_lincons_array(itvD_lincons_array_t a, itv_lincons_array_t b, numinternal_t intern);
bool itvMPFR_lincons_array_set_itv_lincons_array(itvMPFR_lincons_array_t a, itv_lincons_array_t b, numinternal_t intern);

#undef itvlincons_set_itvXXXlincons
#define itvlincons_set_itvXXXlincons(XXX) \
  static inline bool itv_lincons_set_itv##XXX##_lincons(itv_lincons_t a, itv##XXX##_lincons_t b, numinternal_t intern) \
  {									\
    bool res;								\
    res = itv_linexpr_set_itv##XXX##_linexpr(a->linexpr,b->linexpr,intern); \
    a->constyp = b->constyp;						\
    mpq_set(a->mpq,b->mpq);						\
    return res;								\
  }

#undef itvXXXlincons_set_itvlincons
#define itvXXXlincons_set_itvlincons(XXX) \
  static inline bool itv##XXX##_lincons_set_itv_lincons(itv##XXX##_lincons_t a, itv_lincons_t b, numinternal_t intern) \
  {									\
    bool res;								\
    res = itv##XXX##_linexpr_set_itv_linexpr(a->linexpr,b->linexpr,intern); \
    a->constyp = b->constyp;						\
    mpq_set(a->mpq,b->mpq);						\
    return res;								\
  }

#if !(defined(_ITVD_LINCONS_CONV_H_) && (NUM_NUMMPQ || NUM_NUMMPFR))
itvlincons_set_itvXXXlincons(D)
#if !NUM_NUMD
itvXXXlincons_set_itvlincons(D)
#endif
#endif

#if !(defined(_ITVMPQ_LINCONS_CONV_H_) && (NUM_NUMD || NUM_NUMMPFR))
itvlincons_set_itvXXXlincons(MPQ)
#if !NUM_NUMMPQ
itvXXXlincons_set_itvlincons(MPQ)
#endif
#endif

#if !(defined(_ITVMPFR_LINCONS_CONV_H_) && (NUM_NUMD || NUM_NUMMPQ))
itvlincons_set_itvXXXlincons(MPFR)
#if !NUM_NUMMPFR
itvXXXlincons_set_itvlincons(MPFR)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
