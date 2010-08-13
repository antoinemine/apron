/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_yyy_array.c: array of ap_yyy_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy_array.h"
#include "ap_linexprconsD_conv.h"
#include "ap_linexprconsMPQ_conv.h"
#include "ap_linexprconsMPFR_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy_array_init(ap_yyy_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->discr = discr;
  SWITCH(discr)
    a->zzz_array.XXX = ap_zzzXXX_array_alloc(size);
  ENDSWITCH
}
void ap_yyy_array_init_set(ap_yyy_array_t a,ap_yyy_array_t b)
{
  a->discr = b->discr;
  SWITCH(a->discr)
    a->zzz_array.XXX = ap_zzzXXX_array_alloc_set(b->zzz_array.XXX);
  ENDSWITCH
}
void ap_yyy_array_minimize(ap_yyy_array_t a)
{
  SWITCH(a->discr)
    ap_zzzXXX_array_minimize(a->zzz_array.XXX);
  ENDSWITCH
}
void ap_yyy_array_clear(ap_yyy_array_t a)
{
  SWITCH(a->discr)
    ap_zzzXXX_array_clear(a->zzz_array.XXX);
  ENDSWITCH
}

void ap_yyy_array_fprint(FILE* stream, ap_yyy_array_t e, char** name_of_dim)
{
  SWITCH(e->discr)
    ap_zzzXXX_array_fprint(stream,e->zzz_array.XXX,name_of_dim);
  ENDSWITCH
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

MACRO_MAINZ
bool ap_yyy_array_set_zzzZZZ_array(ap_yyy_array_t a, ap_zzzZZZ_array_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_zzzD_array_set_zzzZZZ_array(a->zzz_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_zzzMPQ_array_set_zzzZZZ_array(a->zzz_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_zzzMPFR_array_set_zzzZZZ_array(a->zzz_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_zzzZZZ_array_set_yyy_array(ap_zzzZZZ_array_t a, ap_yyy_array_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_zzzZZZ_array_set_zzzD_array(a,b->zzz_array.D,intern);
  case AP_SCALAR_MPQ:
    return ap_zzzZZZ_array_set_zzzMPQ_array(a,b->zzz_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_zzzZZZ_array_set_zzzMPFR_array(a,b->zzz_array.MPFR,intern);
  default:
    abort();
  }
}
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

size_t ap_yyy_array_size(ap_yyy_array_t p)
{
  size_t res;
  SWITCH(p->discr)
    res = p->zzz_array.XXX->size;
  ENDSWITCH
  return res;
}

ap_linexpr_type_t ap_yyy_array_type(ap_yyy_array_t p)
{
  ap_linexpr_type_t res;
  SWITCH(p->discr)
    res = ap_zzzXXX_array_type(p->zzz_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy_array_is_linear(ap_yyy_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_zzzXXX_array_is_linear(p->zzz_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy_array_is_quasilinear(ap_yyy_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_zzzXXX_array_is_quasilinear(p->zzz_array.XXX);
  ENDSWITCH
  return res;
}

void ap_yyy_array_ref_index(ap_yyy_t a, ap_yyy_array_t p, size_t i)
{
  SWITCH (p->discr)
    ap_yyy_cons_XXX(a,p->zzz_array.XXX->p[i]);
  ENDSWITCH
}

bool ap_yyy_array_get_index(ap_yyy_t a, ap_yyy_array_t p, size_t i, num_internal_t intern)
{
  ap_yyy_t ref;
  ap_yyy_array_ref_index(ref,p,i);
  return ap_yyy_set(a,ref,intern);
}
bool ap_yyy_array_set_index(ap_yyy_array_t p, size_t i, ap_yyy_t a, num_internal_t intern)
{
  ap_yyy_t ref;
  ap_yyy_array_ref_index(ref,p,i);
  return ap_yyy_set(ref,a,intern);
}

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy_array_add_dimensions(ap_yyy_array_t res,
				ap_yyy_array_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_zzzXXX_array_add_dimensions(res->zzz_array.XXX,
					expr->zzz_array.XXX,
					dimchange);
  ENDSWITCH
}
void ap_yyy_array_permute_dimensions(ap_yyy_array_t res,
				    ap_yyy_array_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_zzzXXX_array_permute_dimensions(res->zzz_array.XXX,
					expr->zzz_array.XXX,
					perm);
  ENDSWITCH
}
