/* ********************************************************************** */
/* bound_conv.h: conversion between bounds */
/* ********************************************************************** */

#ifndef _BOUND_CONV_H_
#define _BOUND_CONV_H_

#include "bound_def.h"
#include "boundMPQ_def.h"
#include "boundD_def.h"
#include "boundMPFR_def.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool bound_set_boundMPQ(bound_t a, boundMPQ_t b, numinternal_t intern);
static inline bool bound_set_boundD(bound_t a, boundD_t b, numinternal_t intern);
static inline bool bound_set_boundMPFR(bound_t a, boundMPFR_t b, numinternal_t intern);
static inline bool boundMPQ_set_bound(boundMPQ_t a, bound_t b, numinternal_t intern);
static inline bool boundD_set_bound(boundD_t a, bound_t b, numinternal_t intern);
static inline bool boundMPFR_set_bound(boundMPFR_t a, bound_t b, numinternal_t intern);

/* ********************************************************************** */
/* Inline definitions */
/* ********************************************************************** */

static inline bool bound_set_boundMPQ(bound_t a, boundMPQ_t b, numinternal_t intern)
{
  if (boundMPQ_infty(b)){
    bound_set_infty(a,1);
    return true;
  }
  else
    return bound_set_mpq(a,b,intern);
}
static inline bool bound_set_boundD(bound_t a, boundD_t b, numinternal_t intern)
{
  if (boundD_infty(b)){
    bound_set_infty(a,1);
    return true;
  }
  else
    return bound_set_double(a,*b,intern);
}
static inline bool bound_set_boundMPFR(bound_t a, boundMPFR_t b, numinternal_t intern)
{
  if (boundMPFR_infty(b)){
    bound_set_infty(a,1);
    return true;
  }
  else
    return bound_set_mpfr(a,b,intern);
}
#if !NUM_NUMMPQ
static inline bool boundMPQ_set_bound(boundMPQ_t a, bound_t b, numinternal_t intern)
{
   if (bound_infty(b)){
    boundMPQ_set_infty(a,1);
    return true;
  }
  else
    return boundMPQ_set_num(a,bound_numref(b),intern);
}
#endif
#if !NUM_NUMD
static inline bool boundD_set_bound(boundD_t a, bound_t b, numinternal_t intern)
{
   if (bound_infty(b)){
    boundD_set_infty(a,1);
    return true;
  }
  else
    return boundD_set_num(a,bound_numref(b),intern);
}
#endif
#if !NUM_NUMMPFR
static inline bool boundMPFR_set_bound(boundMPFR_t a, bound_t b, numinternal_t intern)
{
   if (bound_infty(b)){
    boundMPFR_set_infty(a,1);
    return true;
  }
  else
    return boundMPFR_set_num(a,bound_numref(b),intern);
}
#endif

#ifdef __cplusplus
}
#endif

#endif
