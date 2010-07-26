/* ********************************************************************** */
/* eitv_conv.h: conversion between eitvs */
/* ********************************************************************** */

#ifndef _EITV_CONV_H_
#define _EITV_CONV_H_

#include "itv_conv.h"
#include "eitv.h"
#include "eitvMPQ.h"
#include "eitvD.h"
#include "eitvMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool eitv_set_eitvMPQ(eitv_t a, eitvMPQ_t b, numinternal_t intern);
static inline bool eitv_set_eitvD(eitv_t a, eitvD_t b, numinternal_t intern);
static inline bool eitv_set_eitvMPFR(eitv_t a, eitvMPFR_t b, numinternal_t intern);
static inline bool eitvMPQ_set_eitv(eitvMPQ_t a, eitv_t b, numinternal_t intern);
static inline bool eitvD_set_eitv(eitvD_t a, eitv_t b, numinternal_t intern);
static inline bool eitvMPFR_set_eitv(eitvMPFR_t a, eitv_t b, numinternal_t intern);

#if !(defined(_EITVD_CONV_H_) && (NUM_NUMMPQ || NUM_NUMMPFR))
static inline bool eitv_set_eitvD(eitv_t a, eitvD_t b, numinternal_t intern)
{
  bool exact = itv_set_itvD(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#if !NUM_NUMD
static inline bool eitvD_set_eitv(eitvD_t a, eitv_t b, numinternal_t intern)
{
  bool exact = itvD_set_itv(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#endif
#endif

#if !(defined(_EITVMPQ_CONV_H_) && (NUM_NUMD || NUM_NUMMPFR))
static inline bool eitv_set_eitvMPQ(eitv_t a, eitvMPQ_t b, numinternal_t intern)
{
  bool exact = itv_set_itvMPQ(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#if !NUM_NUMMPQ
static inline bool eitvMPQ_set_eitv(eitvMPQ_t a, eitv_t b, numinternal_t intern)
{
  bool exact = itvMPQ_set_itv(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#endif
#endif

#if !(defined(_EITVMPFR_CONV_H_) && (NUM_NUMD || NUM_NUMMPQ))
static inline bool eitv_set_eitvMPFR(eitv_t a, eitvMPFR_t b, numinternal_t intern)
{
  bool exact = itv_set_itvMPFR(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#if !NUM_NUMMPFR
static inline bool eitvMPFR_set_eitv(eitvMPFR_t a, eitv_t b, numinternal_t intern)
{
  bool exact = itvMPFR_set_itv(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
