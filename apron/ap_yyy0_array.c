/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_yyy_array.c: array of ap_yyy_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy_array.h"
#include "itvD_conv.h"
#include "itvMPQ_conv.h"
#include "itvMPFR_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy_array_init(ap_yyy_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->ref = false;
  a->discr = discr;
  SWITCH(discr)
    a->yyy_array.XXX = itvXXX_yyy_array_alloc(size);
  ENDSWITCH
}
void ap_yyy_array_init_set(ap_yyy_array_t a,ap_yyy_array_t b)
{
  a->ref = false;
  a->discr = b->discr;
  SWITCH(a->discr)
    a->yyy_array.XXX = itvXXX_yyy_array_alloc_set(b->yyy_array.XXX);
  ENDSWITCH
}
void ap_yyy_array_minimize(ap_yyy_array_t a)
{
  SWITCH(a->discr)
    itvXXX_yyy_array_minimize(a->yyy_array.XXX);
  ENDSWITCH
}
void ap_yyy_array_clear(ap_yyy_array_t a)
{
  SWITCH(a->discr)
    itvXXX_yyy_array_clear(a->yyy_array.XXX);
  ENDSWITCH
}

void ap_yyy_array_fprint(FILE* stream, ap_yyy_array_t e, char** name_of_dim)
{
  SWITCH(e->discr)
    itvXXX_yyy_array_fprint(stream,e->yyy_array.XXX,name_of_dim);
  ENDSWITCH
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

MACROX
bool ap_yyy_array_set_itvXXX_yyy_array(ap_yyy_array_t a, itvXXX_yyy_array_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_yyy_array_set_itvXXX_yyy_array(a->yyy_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return itvMPQ_yyy_array_set_itvXXX_yyy_array(a->yyy_array.MPQ,b,intern);
  case AP_SCALAR_XXX:
    return itvXXX_yyy_array_set(a->yyy_array.XXX,b,intern);
  default:
    abort();
  }
}
bool itvXXX_yyy_array_set_ap_yyy_array(itvXXX_yyy_array_t a, ap_yyy_array_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvXXX_yyy_array_set_itvD_yyy_array(a,b->yyy_array.D,intern);
  case AP_SCALAR_MPQ:
    return itvXXX_yyy_array_set_itvMPQ_yyy_array(a,b->yyy_array.MPQ,intern);
  case AP_SCALAR_XXX:
    return itvXXX_yyy_array_set(a,b->yyy_array.XXX,intern);
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
    res = p->yyy_array.XXX->size;
  ENDSWITCH
  return res;
}

itvyyy_type_t ap_yyy_array_type(ap_yyy_array_t p)
{
  itvyyy_type_t res;
  SWITCH(p->discr)
    res = itvXXX_yyy_array_type(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy_array_is_linear(ap_yyy_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = itvXXX_yyy_array_is_linear(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy_array_is_quasilinear(ap_yyy_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = itvXXX_yyy_array_is_quasilinear(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}

void ap_yyy_array_ref_index(ap_yyy_t a, ap_yyy_array_t p, size_t i)
{
  SWITCH (p->discr)
    ap_yyy_ref_XXX(a,p->yyy_array.XXX->p[i]);
  ENDSWITCH
}

bool ap_yyy_array_get_index(ap_yyy_t a, ap_yyy_array_t p, size_t i, numinternal_t intern)
{
  ap_yyy_t ref;
  ap_yyy_array_ref_index(ref,p,i);
  return ap_yyy_set(a,ref,intern);
}
bool ap_yyy_array_set_index(ap_yyy_array_t p, size_t i, ap_yyy_t a, numinternal_t intern)
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
  if (res->discr!=expr->discr || (res==expr && expr->ref))
    abort();
  SWITCH(expr->discr)
    itvXXX_yyy_array_add_dimensions(res->yyy_array.XXX,
					expr->yyy_array.XXX,
					dimchange);
  ENDSWITCH
}
void ap_yyy_array_permute_dimensions(ap_yyy_array_t res,
				    ap_yyy_array_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr || (res==expr && expr->ref))
    abort();
  SWITCH(expr->discr)
    itvXXX_yyy_array_permute_dimensions(res->yyy_array.XXX,
					    expr->yyy_array.XXX,
					    perm);
  ENDSWITCH
}
