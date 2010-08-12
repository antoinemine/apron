/* ********************************************************************** */
/* numbounditvXXX_conv.c: conversion between itvs */
/* ********************************************************************** */

#include "numbounditvXXX_conv.h"
#include "itvXXX.h"

#include "itvD.h"
#include "itvMPQ.h"
#include "itvMPFR.h"

#define _NUMXXX_MARK_

MACRO_MAINZ
inline bool itvXXX_set_itvZZZ(itvXXX_t a, itvZZZ_t b, num_internal_t intern)
{
  bool exact = boundXXX_set_boundZZZ(a->neginf,b->neginf,intern);
  return boundXXX_set_boundZZZ(a->sup,b->sup,intern) && exact;
}
inline bool eitvXXX_set_eitvZZZ(eitvXXX_t a, eitvZZZ_t b, num_internal_t intern)
{
  bool exact = itvXXX_set_itvZZZ(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
ENDMACRO

#if !defined(_NUMZZZ_MARK_) && !defined(_NUMBOUNDITVD_CONV_H_) && !defined(_NUMBOUNDITVMPQ_CONV_H_) && !defined(_NUMBOUNDITVMPFR_CONV_H_)
MACRO_MAINZ
inline bool itvZZZ_set_itvXXX(itvZZZ_t a, itvXXX_t b, num_internal_t intern)
{
  bool exact = boundZZZ_set_boundXXX(a->neginf,b->neginf,intern);
  return boundZZZ_set_boundXXX(a->sup,b->sup,intern) && exact;
}
inline bool eitvZZZ_set_eitvXXX(eitvZZZ_t a, eitvXXX_t b, num_internal_t intern)
{
  bool exact = itvZZZ_set_itvXXX(a->itv,b->itv,intern);
  a->eq = exact && b->eq;
  return exact;
}
ENDMACRO
#endif

#undef _NUMXXX_MARK_
