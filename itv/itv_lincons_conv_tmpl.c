/* ********************************************************************** */
/* itv_lincons_conv.h: conversion between itvs */
/* ********************************************************************** */

#include "itv_lincons_conv.h"

#define linconsarray_set_XXXlinconsarray(XXX) \
  bool itv_lincons_array_set_itv##XXX##_lincons_array(itv_lincons_array_t a, itv##XXX##_lincons_array_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = true;							\
    itv_lincons_array_resize(a,b->size);				\
    for (i=0; i<b->size; i++){						\
      res = itv_lincons_set_itv##XXX##_lincons(a->p[i],b->p[i],intern) && res; \
    }									\
    return res;								\
  }

#define XXXlinconsarray_set_linconsarray(XXX) \
  bool itv##XXX##_lincons_array_set_itv_lincons_array(itv##XXX##_lincons_array_t a, itv_lincons_array_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = true;							\
    itv##XXX##_lincons_array_resize(a,b->size);				\
    for (i=0; i<b->size; i++){						\
      res = itv##XXX##_lincons_set_itv_lincons(a->p[i],b->p[i],intern) && res; \
    }									\
    return res;								\
  }

linconsarray_set_XXXlinconsarray(D)
#if !NUM_NUMD
XXXlinconsarray_set_linconsarray(D)
#endif

linconsarray_set_XXXlinconsarray(MPQ)
#if !NUM_NUMMPQ
XXXlinconsarray_set_linconsarray(MPQ)
#endif

linconsarray_set_XXXlinconsarray(MPFR)
#if !NUM_NUMMPFR
XXXlinconsarray_set_linconsarray(MPFR)
#endif
