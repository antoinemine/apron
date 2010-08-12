/* ********************************************************************** */
/* ap_linexprconsXXX.c: conversion between linear expressions and constraints */
/* ********************************************************************** */

#include "boundXXX.h"
#include "itvXXX.h"
#include "eitvXXX.h"
#include "numbounditvXXX_conv.h"

#include "numitvboundD_conv.h"
#include "numitvboundMPQ_conv.h"
#include "numitvboundMPFR_conv.h"

#define _APXXX_MARK_

inline bool itvXXX_set_numD(itvXXX_t a, numD_t b, numinternal_t intern)
{
  return itvXXX_set_double(a,*b,intern);
}
inline bool itvXXX_set_numD2(itvXXX_t a, numD_t b, numD_t c, numinternal_t intern)
{
  return itvXXX_set_double2(a,*b,*c,intern);
}
inline bool itvXXX_set_numMPQ(itvXXX_t a, numMPQ_t b, numinternal_t intern)
{
  return itvXXX_set_mpq(a,b,intern);
}
inline bool itvXXX_set_numMPQ2(itvXXX_t a, numMPQ_t b, numMPQ_t c, numinternal_t intern)
{
  return itvXXX_set_mpq2(a,b,c,intern);
}
inline bool itvXXX_set_numMPFR(itvXXX_t a, numMPFR_t b, numinternal_t intern)
{
  return itvXXX_set_mpfr(a,b,intern);
}
inline bool itvXXX_set_numMPFR2(itvXXX_t a, numMPFR_t b, numMPFR_t c, numinternal_t intern)
{
  return itvXXX_set_mpfr2(a,b,c,intern);
}

MACRO_MAINZ
bool eitvXXX_set_numZZZ(eitvXXX_t a, numZZZ_t b, numinternal_t intern)
{
  a->eq = itvXXX_set_numZZZ(a->itv,b,intern); 
  return a->eq; 
}
bool eitvXXX_set_numZZZ2(eitvXXX_t a, numZZZ_t b, numZZZ_t c, numinternal_t intern)
{
  bool exact = itvXXX_set_numZZZ2(a->itv,b,c,intern);
  a->eq = exact && numZZZ_equal(b,c);
  return exact;
}

inline bool itvXXX_set_itvZZZ(itvXXX_t a, itvZZZ_t b, numinternal_t intern)
{
  bool exact = boundXXX_set_boundZZZ(a->neginf,b->neginf,intern);
  return boundXXX_set_boundZZZ(a->sup,b->sup,intern) && exact;
}
inline bool eitvXXX_set_eitvZZZ(eitvXXX_t a, eitvZZZ_t b, numinternal_t intern)
{
  bool exact = itvXXX_set_itvZZZ(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
bool itvXXX_linexpr_set_itvZZZ_linexpr(itvXXX_linexpr_t a, itvZZZ_linexpr_t b, numinternal_t intern)
{
  size_t i;
  bool res = eitvXXX_set_eitvZZZ(a->cst,b->cst,intern);
  itvXXX_linexpr_resize(a,b->size);
  for (i=0; i<a->size; i++){
    a->linterm[i]->dim = b->linterm[i]->dim;
    res = eitvXXX_set_eitvZZZ(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res;
  }
  return res;
}
bool itvXXX_linexpr_array_set_itvZZZ_linexpr_array(itvXXX_linexpr_array_t a, itvZZZ_linexpr_array_t b, numinternal_t intern)
{
  size_t i;
  bool res = true;
  itvXXX_linexpr_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = itvXXX_linexpr_set_itvZZZ_linexpr(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
bool itvXXX_lincons_set_itvZZZ_lincons(itvXXX_lincons_t a, itvZZZ_lincons_t b, numinternal_t intern)
{
  bool res;
  res = itvXXX_linexpr_set_itvZZZ_linexpr(a->linexpr,b->linexpr,intern);
  a->constyp = b->constyp;
  mpq_set(a->mpq,b->mpq);
  return res;
}

bool itvXXX_lincons_array_set_itvZZZ_lincons_array(itvXXX_lincons_array_t a, itvZZZ_lincons_array_t b, numinternal_t intern)
{
  size_t i;
  bool res = true;
  itvXXX_lincons_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = itvXXX_lincons_set_itvZZZ_lincons(a->p[i],b->p[i],intern) && res;
  }
  return res;
}

#if !defined(_ITVZZZ_MARK_) && !defined(_ITVD_MARK_) && !defined(_ITVMPQ_MARK_) && !defined(_ITVMPFR_MARK_)
inline bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, numinternal_t intern)
{
  bool exact = boundZZZ_set_boundXXX(a->neginf,b->neginf,intern);
  return boundZZZ_set_boundXXX(a->sup,b->sup,intern) && exact;
}
inline bool eitvZZZ_set_eitvXXX(eitvZZZ_t a, eitvXXX_t b, numinternal_t intern)
{
  bool exact = itvZZZ_set_itvXXX(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
bool itvZZZ_linexpr_set_itvXXX_linexpr(itvZZZ_linexpr_t a, itvXXX_linexpr_t b, numinternal_t intern)
{
  size_t i;
  bool res = eitvZZZ_set_eitvXXX(a->cst,b->cst,intern);
  itvZZZ_linexpr_resize(a,b->size);
  for (i=0; i<a->size; i++){
    a->linterm[i]->dim = b->linterm[i]->dim;
    res = eitvZZZ_set_eitvXXX(a->linterm[i]->eitv,b->linterm[i]->eitv,intern) || res;
  }
  return res;
}
bool itvZZZ_linexpr_array_set_itvXXX_linexpr_array(itvZZZ_linexpr_array_t a, itvXXX_linexpr_array_t b, numinternal_t intern)
{
  size_t i;
  bool res = true;
  itvZZZ_linexpr_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = itvZZZ_linexpr_set_itvXXX_linexpr(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
bool itvZZZ_lincons_set_itvXXX_lincons(itvZZZ_lincons_t a, itvXXX_lincons_t b, numinternal_t intern)
{
  bool res;
  res = itvZZZ_linexpr_set_itvXXX_linexpr(a->linexpr,b->linexpr,intern);
  a->constyp = b->constyp;
  mpq_set(a->mpq,b->mpq);
  return res;
}
bool itvZZZ_lincons_array_set_itvXXX_lincons_array(itvZZZ_lincons_array_t a, itvXXX_lincons_array_t b, numinternal_t intern)
{
  size_t i;
  bool res = true;
  itvZZZ_lincons_array_resize(a,b->size);
  for (i=0; i<b->size; i++){
    res = itvZZZ_lincons_set_itvXXX_lincons(a->p[i],b->p[i],intern) && res;
  }
  return res;
}
#endif

ENDMACRO

#undef _APXXX_MARK_
