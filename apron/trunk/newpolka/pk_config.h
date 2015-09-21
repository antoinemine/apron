
/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_CONFIG_H_
#define _PK_CONFIG_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "num.h"
#include "numint.h"
#include "numrat.h"

#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"

#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_expr0.h"
#include "ap_manager.h"

#ifdef __cplusplus
#define HAS_BOOL
extern "C" {
#endif

#ifndef HAS_BOOL
#define HAS_BOOL
typedef char bool;
static const bool false = 0;
static const bool true  = 1;
#endif

/* Extension to the num package */
/* size in words */
static inline size_t numint_size(numint_t a)
#if defined (NUMINT_NATIVE)
{ return 1; }
#elif defined(NUMINT_MPZ)
{ return mpz_size(a); }
#else
#error "Here"
#endif

/* size in bits */
#if defined (NUMINT_NATIVE)

#if defined(NUMINT_LONGINT) || defined(NUMINT_LONGLONGINT)
#if defined(__GNUC__)
static inline size_t numint_size2(numint_t a)
{
  numint_t x;
  if (a==0) return 0;
  numint_abs(x,a);
  return (size_t)(
		  sizeof(numint_t)*8 - 
#if defined(NUMINT_LONGINT)
		  __builtin_clzl((unsigned long int)(*x))
#else
		  __builtin_clzll((unsigned long long int)(*x))
#endif
		  );
}
#else
static inline size_t numint_size2(numint_t a)
{ 
  numint_t x;
  size_t nbzero;
  static size_t table[256] = {
    8,
    7,
    6,6,
    5,5,5,5,
    4,4,4,4,4,4,4,4,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  }    
  if (a==0) return 0;
  numint_abs(x,a);
  /* Iterate over leading zero octets */
#if defined(NUMINT_LONGINT)
  unsigned long int mask = (0xffUL << (sizeof(numint_t)-1)*8);
#else
  unsigned long long int mask = (0xffULL << (sizeof(numint_t)-1)*8);
#endif
  nbzero = 0;
  while ( (*x & *mask)==0 ){
    nbzero += 8;
    *mask = *mask >> 256;
  }
  *x = (*x & *mask) >> ((sizeof(numint_t)-1)*8 - nbzero);
  nbzero += table[*x];
  return (sizeof(numint_t)*8 - nbzero);
}
#endif
#else
#error "Here"
#endif

#elif defined(NUMINT_MPZ)
static inline size_t numint_size2(numint_t a)
{ return mpz_sizeinbase(a,2); }
#else
#error "Here"
#endif


/* Do not change ! */
static const size_t polka_cst = 1;
static const size_t polka_eps = 2;

#ifdef __cplusplus
}
#endif

#endif

