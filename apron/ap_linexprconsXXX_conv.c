/* ********************************************************************** */
/* ap_linexprconsXXX_conv.c: conversion between linear expressions and constraints */
/* ********************************************************************** */

#include "ap_linexprconsXXX_conv.h"

#include "boundXXX.h"
#include "itvXXX.h"
#include "eitvXXX.h"

#include "numbounditvXXX_conv.h"
#include "numbounditvD_conv.h"
#include "numbounditvMPQ_conv.h"
#include "numbounditvMPFR_conv.h"


#define _APXXX_MARK_

MACRO_MAINZ
bool ap_linexprXXX_set_linexprZZZ(ap_linexprXXX_t a, ap_linexprZZZ_t b, num_internal_t intern)
{
  size_t i;
  bool res = eitvXXX_set_eitvZZZ(a->cst,b->cst,intern);
  ap_linexprXXX_resize(a,b->size);
  for (i=0; i<a->size; i++){
    a->linterm[i]->dim = b->linterm[i]->dim;
    res = eitvXXX_set_eitvZZZ(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res;
  }
  return res;
}
bool ap_linexprXXX_array_set_linexprZZZ_array(ap_linexprXXX_array_t a, ap_linexprZZZ_array_t b, num_internal_t intern)
{
  size_t i;
  bool res = true;
  ap_linexprXXX_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = ap_linexprXXX_set_linexprZZZ(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
bool ap_linconsXXX_set_linconsZZZ(ap_linconsXXX_t a, ap_linconsZZZ_t b, num_internal_t intern)
{
  bool res;
  res = ap_linexprXXX_set_linexprZZZ(a->linexpr,b->linexpr,intern);
  a->constyp = b->constyp;
  mpq_set(a->mpq,b->mpq);
  return res;
}
bool ap_linconsXXX_array_set_linconsZZZ_array(ap_linconsXXX_array_t a, ap_linconsZZZ_array_t b, num_internal_t intern)
{
  size_t i;
  bool res = true;
  ap_linconsXXX_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = ap_linconsXXX_set_linconsZZZ(a->p[i],b->p[i],intern) && res;
  }
  return res;
}

#if !defined(_APZZZ_MARK_) && !defined(_APD_MARK_) && !defined(_APMPQ_MARK_) && !defined(_APMPFR_MARK_)
bool ap_linexprZZZ_set_linexprXXX(ap_linexprZZZ_t a, ap_linexprXXX_t b, num_internal_t intern)
{
  size_t i;
  bool res = eitvZZZ_set_eitvXXX(a->cst,b->cst,intern);
  ap_linexprZZZ_resize(a,b->size);
  for (i=0; i<a->size; i++){
    a->linterm[i]->dim = b->linterm[i]->dim;
    res = eitvZZZ_set_eitvXXX(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res;
  }
  return res;
}
bool ap_linexprZZZ_array_set_linexprXXX_array(ap_linexprZZZ_array_t a, ap_linexprXXX_array_t b, num_internal_t intern)
{
  size_t i;
  bool res = true;
  ap_linexprZZZ_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = ap_linexprZZZ_set_linexprXXX(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
bool ap_linconsZZZ_set_linconsXXX(ap_linconsZZZ_t a, ap_linconsXXX_t b, num_internal_t intern)
{
  bool res;
  res = ap_linexprZZZ_set_linexprXXX(a->linexpr,b->linexpr,intern);
  a->constyp = b->constyp;
  mpq_set(a->mpq,b->mpq);
  return res;
}
bool ap_linconsZZZ_array_set_linconsXXX_array(ap_linconsZZZ_array_t a, ap_linconsXXX_array_t b, num_internal_t intern)
{
  size_t i;
  bool res = true;
  ap_linconsZZZ_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = ap_linconsZZZ_set_linconsXXX(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
#endif

ENDMACRO

#undef _APXXX_MARK_
