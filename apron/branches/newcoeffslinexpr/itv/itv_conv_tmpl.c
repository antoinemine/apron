/* ********************************************************************** */
/* itv_conv.c: conversion between itvs */
/* ********************************************************************** */

#include "itv_conv.h"
#include "bound_conv.h"

bool itv_set_itvMPQ(itv_t a, itvMPQ_t b, numinternal_t intern)
{
  bool exact = bound_set_boundMPQ(a->neginf,b->neginf,intern);
  return bound_set_boundMPQ(a->sup,b->sup,intern) && exact;
}
bool itv_set_itvD(itv_t a, itvD_t b, numinternal_t intern)
{
  bool exact = bound_set_boundD(a->neginf,b->neginf,intern);
  return bound_set_boundD(a->sup,b->sup,intern) && exact;
}
bool itv_set_itvMPFR(itv_t a, itvMPFR_t b, numinternal_t intern)
{
  bool exact = bound_set_boundMPFR(a->neginf,b->neginf,intern);
  return bound_set_boundMPFR(a->sup,b->sup,intern) && exact;
}
#if !NUM_NUMMPQ
bool itvMPQ_set_itv(itvMPQ_t a, itv_t b, numinternal_t intern)
{
  bool exact = boundMPQ_set_bound(a->neginf,b->neginf,intern);
  return boundMPQ_set_bound(a->sup,b->sup,intern) && exact;
}
#endif
#if !NUM_NUMD
bool itvD_set_itv(itvD_t a, itv_t b, numinternal_t intern)
{
  bool exact = boundD_set_bound(a->neginf,b->neginf,intern);
  return boundD_set_bound(a->sup,b->sup,intern) && exact;
}
#endif
#if !NUM_NUMMPFR
bool itvMPFR_set_itv(itvMPFR_t a, itv_t b, numinternal_t intern)
{
  bool exact = boundMPFR_set_bound(a->neginf,b->neginf,intern);
  return boundMPFR_set_bound(a->sup,b->sup,intern) && exact;
}
#endif
