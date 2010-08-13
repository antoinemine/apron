/* ********************************************************************** */
/* numbounditvXXX_conv.h: conversion between nums and bounds */
/* ********************************************************************** */

#ifndef _NUMBOUNDITVXXX_CONV_H_
#define _NUMBOUNDITVXXX_CONV_H_

#include "num_types.h"
#include "bound_types.h"
#include "itv_types.h"

#include "numXXX.h"
#include "boundXXX.h"
#include "numD.h"
#include "boundD.h"
#include "numMPQ.h"
#include "boundMPQ.h"
#include "numMPFR.h"
#include "boundMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _NUMXXX_MARK_

static inline bool numD_fits_numXXX(numD_t a);
static inline bool numMPQ_fits_numXXX(numMPQ_t a);
static inline bool numMPFR_fits_numXXX(numMPFR_t a, num_internal_t intern);

#if !defined(_NUMMPFR_MARK_) 
static inline bool numXXX_fits_numD(numXXX_t a);
static inline bool numXXX_fits_numMPQ(numXXX_t a);
static inline bool numXXX_fits_numMPFR(numXXX_t a);
#endif

MACRO_MAINZ
static inline bool numXXX_set_numZZZ(numXXX_t a, numZZZ_t b, num_internal_t intern);
static inline bool boundXXX_set_boundZZZ(boundXXX_t a, boundZZZ_t b, num_internal_t intern);
bool itvXXX_set_itvZZZ(itvXXX_t a, itvZZZ_t b, num_internal_t intern);
bool eitvXXX_set_eitvZZZ(eitvXXX_t a, eitvZZZ_t b, num_internal_t intern);

static inline bool numZZZ_set_numXXX(numZZZ_t a, numXXX_t b, num_internal_t intern);
static inline bool boundZZZ_set_boundXXX(boundZZZ_t a, boundXXX_t b, num_internal_t intern);
bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, num_internal_t intern);
bool eitvZZZ_set_eitvXXX(eitvZZZ_t a, eitvXXX_t b, num_internal_t intern);
ENDMACRO

/* ********************************************************************** */
/* Definitions */
/* ********************************************************************** */

static inline bool numD_fits_numXXX(numD_t a)
{ return double_fits_numXXX(*a); }
static inline bool numMPQ_fits_numXXX(numMPQ_t a)
{ return mpq_fits_numXXX(a); }
static inline bool numMPFR_fits_numXXX(numMPFR_t a, num_internal_t intern)
{ return mpfr_fits_numXXX(a,intern); }
static inline bool numXXX_set_numD(numXXX_t a, numD_t b, num_internal_t intern)
{ return numXXX_set_double(a,*b,intern); }
static inline bool numXXX_set_numMPQ(numXXX_t a, numMPQ_t b, num_internal_t intern)
{ return numXXX_set_mpq(a,b,intern); }
static inline bool numXXX_set_numMPFR(numXXX_t a, numMPFR_t b, num_internal_t intern)
{ return numXXX_set_mpfr(a,b,intern); }
MACRO_MAINZ
static inline bool boundXXX_set_boundZZZ(boundXXX_t a, boundZZZ_t b, num_internal_t intern)
#if defined(_NUMZZZ_MARK_)
{ boundXXX_set(a,b); return true; }
#else
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

#if !defined(_NUMZZZ_MARK_) && !defined(_NUMBOUNDITVD_CONV_H_) && !defined(_NUMBOUNDITVMPQ_CONV_H_) && !defined(_NUMBOUNDITVMPFR_CONV_H_)
static inline bool numXXX_fits_numD(numXXX_t a)
{ return numXXX_fits_double(a); }
static inline bool numXXX_fits_numMPQ(numXXX_t a)
{ return numXXX_fits_mpq(a); }
static inline bool numXXX_fits_numMPFR(numXXX_t a)
{ return numXXX_fits_mpfr(a); }
static inline bool numD_set_numXXX(numD_t a, numXXX_t b, num_internal_t intern)
{ return double_set_numXXX(a,b,intern); }
static inline bool numMPQ_set_numXXX(numMPQ_t a, numXXX_t b, num_internal_t intern)
{ return mpq_set_numXXX(a,b,intern); }
static inline bool numMPFR_set_numXXX(numMPFR_t a, numXXX_t b, num_internal_t intern)
{ return mpfr_set_numXXX(a,b,intern); }
MACRO_MAINZ
static inline bool boundZZZ_set_boundXXX(boundZZZ_t a, boundXXX_t b, num_internal_t intern)
{
  if (boundXXX_infty(b)){
    boundZZZ_set_infty(a,boundXXX_sgn(b));
    return true;
  }
  else {
    return boundZZZ_set_numXXX(a,boundXXX_numref(b),intern);
  }
}
ENDMACRO

#endif

#undef _NUMXXX_MARK_

#ifdef __cplusplus
}
#endif

#endif
