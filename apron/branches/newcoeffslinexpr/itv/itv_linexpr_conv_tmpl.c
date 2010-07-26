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
