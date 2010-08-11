/* ********************************************************************** */
/* boundXXX_conv.h: conversion between nums and bounds */
/* ********************************************************************** */

#ifndef _BOUNDXXX_CONV_H_
#define _BOUNDXXX_CONV_H_

#include "boundXXX.h"
#include "boundD.h"
#include "boundMPQ.h"
#include "boundMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _NUMXXX_MARK_X_

static inline bool numD_fits_numXXX(numD_t a)
{ return double_fits_numXXX(*a); }
static inline bool numMPQ_fits_numXXX(numMPQ_t a)
{ return mpq_fits_numXXX(a); }
static inline bool numMPFR_fits_numXXX(numMPFR_t a, numinternal_t intern)
{ return mpfr_fits_numXXX(a,intern); }
static inline bool numXXX_set_numD(numXXX_t a, numD_t b, numinternal_t intern)
{ return numXXX_set_double(a,*b,intern); }
static inline bool numXXX_set_numMPQ(numXXX_t a, numMPQ_t b, numinternal_t intern)
{ return numXXX_set_mpq(a,b,intern); }
static inline bool numXXX_set_numMPFR(numXXX_t a, numMPFR_t b, numinternal_t intern)
{ return numXXX_set_mpfr(a,b,intern); }

MACROZ
#define _NUMZZZ_MARK_Z_
#if defined(_NUMXXX_MARK_X_) && defined(_NUMXXX_MARK_Z_)
static inline bool boundXXX_set_boundZZZ(boundXXX_t a, boundZZZ_t b, numinternal_t intern)
{ boundXXX_set(a,b); return true; }
#else
static inline bool boundXXX_set_boundZZZ(boundXXX_t a, boundZZZ_t b, numinternal_t intern)
{
  if (boundZZZ_infty(b)){
    boundXXX_set_infty(a,boundZZZ_sgn(b));
    return true;
  }
  else {
    return boundXXX_set_numZZZ(a,boundZZZ_numref(b),intern);
  }
}
#endif
#undef _NUMZZZ_MARK_Z_
ENDMACRO

#undef _NUMXXX_MARK_X_

#ifdef __cplusplus
}
#endif

#endif
