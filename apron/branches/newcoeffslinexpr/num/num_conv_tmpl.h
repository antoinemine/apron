/* ********************************************************************** */
/* num_conv.h: conversion between nums */
/* ********************************************************************** */

#ifndef _NUM_CONV_H_
#define _NUM_CONV_H_

#include "numConfig.h"
#include "numIl.h"
#include "numIll.h"
#include "numMPZ.h"
#include "numRl.h"
#include "numRll.h"
#include "numMPQ.h"
#include "numD.h"
#include "numDl.h"
#include "numMPFR.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool num_set_numIl(num_t a, numIl_t b, numinternal_t internal);
static inline bool num_set_numIll(num_t a, numIll_t b, numinternal_t internal);
static inline bool num_set_numMPZ(num_t a, numMPZ_t b, numinternal_t internal);
static inline bool num_set_numRl(num_t a, numRl_t b, numinternal_t internal);
static inline bool num_set_numRll(num_t a, numRll_t b, numinternal_t internal);
static inline bool num_set_numD(num_t a, numD_t b, numinternal_t internal);
static inline bool num_set_numDl(num_t a, numDl_t b, numinternal_t internal);
static inline bool num_set_numMPFR(num_t a, numMPFR_t b, numinternal_t internal);

/* ********************************************************************** */
/* Inline definitions */
/* ********************************************************************** */

static inline bool num_set_numIl(num_t a, numIl_t b, numinternal_t internal)
{ return num_set_lint(a,*b,internal); }
static inline bool num_set_numIll(num_t a, numIll_t b, numinternal_t internal)
{ return num_set_llint(a,*b,internal); }
static inline bool num_set_numMPZ(num_t a, numMPZ_t b, numinternal_t internal)
{ return num_set_mpz(a,b,internal); }
static inline bool num_set_numRl(num_t a, numRl_t b, numinternal_t internal)
{ return num_set_lfrac(a,*b->n,*b->d,internal); }
static inline bool num_set_numRll(num_t a, numRll_t b, numinternal_t internal)
{ return num_set_llfrac(a,*b->n,*b->d,internal); }
static inline bool num_set_numD(num_t a, numD_t b, numinternal_t internal)
{ return num_set_double(a,*b,internal); }
static inline bool num_set_numDl(num_t a, numDl_t b, numinternal_t internal)
{ return num_set_ldouble(a,*b,internal); }
static inline bool num_set_numMPFR(num_t a, numMPFR_t b, numinternal_t internal)
{ return num_set_mpfr(a,b,internal); }

#ifdef __cplusplus
}
#endif

#endif
