/* ************************************************************************* */
/* ap_linyyy0_array.c: array of ap_linyyy0_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linyyy0_array.h"
#include "ap_linexprconsgenD_conv.h"
#include "ap_linexprconsgenMPQ_conv.h"
#include "ap_linexprconsgenMPFR_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_array_init(ap_linyyy0_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->discr = discr;
  SWITCH(discr)
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc(size);
  ENDSWITCH
}
void ap_linyyy0_array_init_set(ap_linyyy0_array_t a,ap_linyyy0_array_t b)
{
  a->discr = b->discr;
  SWITCH(a->discr)
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc_set(b->linyyy_array.XXX);
  ENDSWITCH
}
MACRO_MAINZ
void ap_linyyy0_array_init_set_linyyyZZZ_array(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e)
 {
   ap_linyyy0_array_init(res,AP_SCALAR_ZZZ,0);
   ap_linyyyZZZ_array_set(res->linyyy_array.ZZZ,e);
} 
ENDMACRO
void ap_linyyy0_array_resize(ap_linyyy0_array_t a, size_t size)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_resize(a->linyyy_array.XXX,size);
  ENDSWITCH
}
void ap_linyyy0_array_minimize(ap_linyyy0_array_t a)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_minimize(a->linyyy_array.XXX);
  ENDSWITCH
}
void ap_linyyy0_array_clear(ap_linyyy0_array_t a)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_clear(a->linyyy_array.XXX);
  ENDSWITCH
}

void ap_linyyy0_array_fprint(FILE* stream, ap_linyyy0_array_t e, char** name_of_dim)
{
  SWITCH(e->discr)
    ap_linyyyXXX_array_fprint(stream,e->linyyy_array.XXX,name_of_dim);
  ENDSWITCH
}

/* ====================================================================== */
/* Access with possible conversions */
/* ====================================================================== */

size_t ap_linyyy0_array_size(ap_linyyy0_array_t p)
{
  size_t res;
  SWITCH(p->discr)
    res = p->linyyy_array.XXX->size;
  ENDSWITCH
  return res;
}

#if !defined(_AP_lingen0_ARRAY_H_)
ap_linexpr_type_t ap_linyyy0_array_type(ap_linyyy0_array_t p)
{
  ap_linexpr_type_t res;
  SWITCH(p->discr)
    res = ap_linyyyXXX_array_type(p->linyyy_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_linyyy0_array_is_quasilinear(ap_linyyy0_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_linyyyXXX_array_is_quasilinear(p->linyyy_array.XXX);
  ENDSWITCH
  return res;
}
#endif
bool ap_linyyy0_array_is_linear(ap_linyyy0_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_linyyyXXX_array_is_linear(p->linyyy_array.XXX);
  ENDSWITCH
  return res;
}

bool ap_linyyy0_array_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_linyyy0_array_set_linyyyXXX_array(res,e->linyyy_array.XXX,intern);
  ENDSWITCH
}
void ap_linyyy0_array_ref_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i)
{
  SWITCH (p->discr)
    ap_linyyy0_cons_XXX(a,p->linyyy_array.XXX->p[i]);
  ENDSWITCH
}
bool ap_linyyy0_array_get_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set(a,ref,intern);
}
bool ap_linyyy0_array_set_index(ap_linyyy0_array_t p, size_t i, ap_linyyy0_t a, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set(ref,a,intern);
}

MACRO_MAINZ
bool ap_linyyy0_array_set_linyyyZZZ_array(ap_linyyy0_array_t a, ap_linyyyZZZ_array_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_linyyyD_array_set_linyyyZZZ_array(a->linyyy_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyMPQ_array_set_linyyyZZZ_array(a->linyyy_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyMPFR_array_set_linyyyZZZ_array(a->linyyy_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linyyyZZZ_array_set_linyyy0_array(ap_linyyyZZZ_array_t a, ap_linyyy0_array_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_linyyyZZZ_array_set_linyyyD_array(a,b->linyyy_array.D,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyZZZ_array_set_linyyyMPQ_array(a,b->linyyy_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyZZZ_array_set_linyyyMPFR_array(a,b->linyyy_array.MPFR,intern);
  default:
    abort();
  }
}
bool ap_linyyy0_array_set_index_linyyyZZZ(ap_linyyy0_array_t p, size_t i, ap_linyyyZZZ_t a, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set_linyyyZZZ(ref,a,intern);
}
bool ap_linyyy0_array_get_index_linyyyZZZ(ap_linyyyZZZ_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyyZZZ_set_linyyy0(a,ref,intern);
}
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy0_array_add_dimensions(ap_linyyy0_array_t res,
				ap_linyyy0_array_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_linyyyXXX_array_add_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					dimchange);
  ENDSWITCH
}
void ap_linyyy0_array_permute_dimensions(ap_linyyy0_array_t res,
				    ap_linyyy0_array_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_linyyyXXX_array_permute_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					perm);
  ENDSWITCH
}
