/* ********************************************************************** */
/* itv_linexpr_conv.c: conversion between itv_linexpr */
/* ********************************************************************** */

#include "itv_linexpr_conv.h"
#include "eitv_conv.h"

#define itvlinexpr_set_itvXXXlinexpr(XXX)				\
  bool itv_linexpr_set_itv##XXX##_linexpr(itv_linexpr_t a, itv##XXX##_linexpr_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = eitv_set_eitv##XXX(a->cst,b->cst,intern);		\
    itv_linexpr_resize(a,b->size);					\
    for (i=0; i<a->size; i++){						\
      a->linterm[i]->dim = b->linterm[i]->dim;				\
      res = eitv_set_eitv##XXX(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res; \
    }									\
    return res;								\
  }

#define itvXXXlinexpr_set_itvlinexpr(XXX)				\
  bool itv##XXX##_linexpr_set_itv_linexpr(itv##XXX##_linexpr_t a, itv_linexpr_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = eitv##XXX##_set_eitv(a->cst,b->cst,intern);		\
    itv##XXX##_linexpr_resize(a,b->size);				\
    for (i=0; i<a->size; i++){						\
      a->linterm[i]->dim = b->linterm[i]->dim;				\
      res = eitv##XXX##_set_eitv(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res; \
    }									\
    return res;								\
  }

#define linexprarray_set_XXXlinexprarray(XXX) \
  bool itv_linexpr_array_set_itv##XXX##_linexpr_array(itv_linexpr_array_t a, itv##XXX##_linexpr_array_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = true;							\
    itv_linexpr_array_resize(a,b->size);				\
    for (i=0; i<b->size; i++){						\
      res = itv_linexpr_set_itv##XXX##_linexpr(a->p[i],b->p[i],intern) && res; \
    }									\
    return res;								\
  }

#define XXXlinexprarray_set_linexprarray(XXX) \
  bool itv##XXX##_linexpr_array_set_itv_linexpr_array(itv##XXX##_linexpr_array_t a, itv_linexpr_array_t b, numinternal_t intern) \
  {									\
    size_t i;								\
    bool res = true;							\
    itv##XXX##_linexpr_array_resize(a,b->size);				\
    for (i=0; i<b->size; i++){						\
      res = itv##XXX##_linexpr_set_itv_linexpr(a->p[i],b->p[i],intern) && res; \
    }									\
    return res;								\
  }

itvlinexpr_set_itvXXXlinexpr(MPQ)
#if !NUM_NUMMPQ
itvXXXlinexpr_set_itvlinexpr(MPQ)
#endif

itvlinexpr_set_itvXXXlinexpr(D)
#if !NUM_NUMD
itvXXXlinexpr_set_itvlinexpr(D)
#endif

itvlinexpr_set_itvXXXlinexpr(MPFR)
#if !NUM_NUMMPFR
itvXXXlinexpr_set_itvlinexpr(MPFR)
#endif

linexprarray_set_XXXlinexprarray(D)
#if !NUM_NUMD
XXXlinexprarray_set_linexprarray(D)
#endif

linexprarray_set_XXXlinexprarray(MPQ)
#if !NUM_NUMMPQ
XXXlinexprarray_set_linexprarray(MPQ)
#endif

linexprarray_set_XXXlinexprarray(MPFR)
#if !NUM_NUMMPFR
XXXlinexprarray_set_linexprarray(MPFR)
#endif
