/* ************************************************************************* */
/* ap_yyy0_array.c: array of ap_yyy0_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy0_array.h"
#include "ap_linexprconsD_conv.h"
#include "ap_linexprconsMPQ_conv.h"
#include "ap_linexprconsMPFR_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy0_array_init(ap_yyy0_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->discr = discr;
  SWITCH(discr)
    a->yyy_array.XXX = ap_yyyXXX_array_alloc(size);
  ENDSWITCH
}
void ap_yyy0_array_init_set(ap_yyy0_array_t a,ap_yyy0_array_t b)
{
  a->discr = b->discr;
  SWITCH(a->discr)
    a->yyy_array.XXX = ap_yyyXXX_array_alloc_set(b->yyy_array.XXX);
  ENDSWITCH
}
MACRO_MAINZ
void ap_yyy0_array_init_set_yyyZZZ_array(ap_yyy0_array_t res, ap_yyyZZZ_array_t e)
 {
   ap_yyy0_array_init(res,AP_SCALAR_ZZZ,0);
   ap_yyyZZZ_array_set(res->yyy_array.ZZZ,e);
} 
ENDMACRO
void ap_yyy0_array_minimize(ap_yyy0_array_t a)
{
  SWITCH(a->discr)
    ap_yyyXXX_array_minimize(a->yyy_array.XXX);
  ENDSWITCH
}
void ap_yyy0_array_clear(ap_yyy0_array_t a)
{
  SWITCH(a->discr)
    ap_yyyXXX_array_clear(a->yyy_array.XXX);
  ENDSWITCH
}

void ap_yyy0_array_fprint(FILE* stream, ap_yyy0_array_t e, char** name_of_dim)
{
  SWITCH(e->discr)
    ap_yyyXXX_array_fprint(stream,e->yyy_array.XXX,name_of_dim);
  ENDSWITCH
}

/* ====================================================================== */
/* Access with possible conversions */
/* ====================================================================== */

size_t ap_yyy0_array_size(ap_yyy0_array_t p)
{
  size_t res;
  SWITCH(p->discr)
    res = p->yyy_array.XXX->size;
  ENDSWITCH
  return res;
}

ap_linexpr_type_t ap_yyy0_array_type(ap_yyy0_array_t p)
{
  ap_linexpr_type_t res;
  SWITCH(p->discr)
    res = ap_yyyXXX_array_type(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy0_array_is_linear(ap_yyy0_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_yyyXXX_array_is_linear(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy0_array_is_quasilinear(ap_yyy0_array_t p)
{
  bool res;
  SWITCH(p->discr)
    res = ap_yyyXXX_array_is_quasilinear(p->yyy_array.XXX);
  ENDSWITCH
  return res;
}

bool ap_yyy0_array_set(ap_yyy0_array_t res, ap_yyy0_array_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_yyy0_array_set_yyyXXX_array(res,e->yyy_array.XXX,intern);
  ENDSWITCH
}
void ap_yyy0_array_ref_index(ap_yyy0_t a, ap_yyy0_array_t p, size_t i)
{
  SWITCH (p->discr)
    ap_yyy0_cons_XXX(a,p->yyy_array.XXX->p[i]);
  ENDSWITCH
}
bool ap_yyy0_array_get_index(ap_yyy0_t a, ap_yyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_yyy0_t ref;
  ap_yyy0_array_ref_index(ref,p,i);
  return ap_yyy0_set(a,ref,intern);
}
bool ap_yyy0_array_set_index(ap_yyy0_array_t p, size_t i, ap_yyy0_t a, num_internal_t intern)
{
  ap_yyy0_t ref;
  ap_yyy0_array_ref_index(ref,p,i);
  return ap_yyy0_set(ref,a,intern);
}

MACRO_MAINZ
bool ap_yyy0_array_set_yyyZZZ_array(ap_yyy0_array_t a, ap_yyyZZZ_array_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_yyyD_array_set_yyyZZZ_array(a->yyy_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_yyyMPQ_array_set_yyyZZZ_array(a->yyy_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_yyyMPFR_array_set_yyyZZZ_array(a->yyy_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_yyyZZZ_array_set_yyy0_array(ap_yyyZZZ_array_t a, ap_yyy0_array_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_yyyZZZ_array_set_yyyD_array(a,b->yyy_array.D,intern);
  case AP_SCALAR_MPQ:
    return ap_yyyZZZ_array_set_yyyMPQ_array(a,b->yyy_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_yyyZZZ_array_set_yyyMPFR_array(a,b->yyy_array.MPFR,intern);
  default:
    abort();
  }
}
bool ap_yyy0_array_set_index_yyyZZZ(ap_yyy0_array_t p, size_t i, ap_yyyZZZ_t a, num_internal_t intern)
{
  ap_yyy0_t ref;
  ap_yyy0_array_ref_index(ref,p,i);
  return ap_yyy0_set_yyyZZZ(ref,a,intern);
}
bool ap_yyy0_array_get_index_yyyZZZ(ap_yyyZZZ_t a, ap_yyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_yyy0_t ref;
  ap_yyy0_array_ref_index(ref,p,i);
  return ap_yyyZZZ_set_yyy0(a,ref,intern);
}
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy0_array_add_dimensions(ap_yyy0_array_t res,
				ap_yyy0_array_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_yyyXXX_array_add_dimensions(res->yyy_array.XXX,
					expr->yyy_array.XXX,
					dimchange);
  ENDSWITCH
}
void ap_yyy0_array_permute_dimensions(ap_yyy0_array_t res,
				    ap_yyy0_array_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_yyyXXX_array_permute_dimensions(res->yyy_array.XXX,
					expr->yyy_array.XXX,
					perm);
  ENDSWITCH
}
