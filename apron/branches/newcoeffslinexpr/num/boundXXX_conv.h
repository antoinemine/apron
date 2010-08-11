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

#define _NUMXXX_MARK_

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
#if defined(_NUMZZZ_MARK_) 
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
ENDMACRO

#if !defined(_BOUNDD_CONV_H_) && !defined(_BOUNDMPQ_CONV_H_) && !defined(_BOUNDMPFR_CONV_H_)
static inline bool numXXX_fits_numD(numXXX_t a)
{ return numXXX_fits_double(a); }
static inline bool numXXX_fits_numMPQ(numXXX_t a)
{ return numXXX_fits_mpq(a); }
static inline bool numXXX_fits_numMPFR(numXXX_t a)
{ return numXXX_fits_mpfr(a); }
static inline bool numD_set_numXXX(numD_t a, numXXX_t b, numinternal_t intern)
{ return double_set_numXXX(a,b,intern); }
static inline bool numMPQ_set_numXXX(numMPQ_t a, numXXX_t b, numinternal_t intern)
{ return mpq_set_numXXX(a,b,intern); }
static inline bool numMPFR_set_numXXX(numMPFR_t a, numXXX_t b, numinternal_t intern)
{ return mpfr_set_numXXX(a,b,intern); }

MACROZ
#if !defined(_NUMZZZ_MARK_) 
static inline bool boundZZZ_set_boundXXX(boundZZZ_t a, boundXXX_t b, numinternal_t intern)
{
  if (boundXXX_infty(b)){
    boundZZZ_set_infty(a,boundXXX_sgn(b));
    return true;
  }
  else {
    return boundZZZ_set_numXXX(a,boundXXX_numref(b),intern);
  }
}
#endif
ENDMACRO

#endif

#undef _NUMXXX_MARK_

#ifdef __cplusplus
}
#endif

#endif
