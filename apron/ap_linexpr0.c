/* ************************************************************************* */
/* ap_linexpr0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linexpr0.h"

#include <stdarg.h>

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr0_t* ap_linexpr0_alloc(ap_scalar_discr_t discr, size_t size)
{
  ap_linexpr0_t* e = (ap_linexpr0_t*)malloc(sizeof(ap_linexpr0_t));
  e->discr = discr;
  switch (discr){
  case AP_SCALAR_D:
    itvD_linexpr_init(e->linexpr.D,size);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_init(e->linexpr.MPQ,size);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_init(e->linexpr.MPFR,size);
    break;
  default:
    abort();
  }
  return e;
}

void ap_linexpr0_minimize(ap_linexpr0_t* e)
{
  switch (e->discr){
  case AP_SCALAR_D:
    itvD_linexpr_minimize(e->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_minimize(e->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_minimize(e->linexpr.MPFR);
    break;
  default:
    abort();
  }
}

ap_linexpr0_t* ap_linexpr0_copy(ap_linexpr0_t* a)
{
  ap_linexpr0_t* e = (ap_linexpr0_t*)malloc(sizeof(ap_linexpr0_t));
  e->discr = a->discr;
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_linexpr_init_set(e->linexpr.D,a->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_init_set(e->linexpr.MPQ,a->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_init_set(e->linexpr.MPFR,a->linexpr.MPFR);
    break;
  default:
    abort();
  }
  return e;
}

static void ap_linexpr0_clear(ap_linexpr0_t* e)
{
  switch (e->discr){
  case AP_SCALAR_D:
    itvD_linexpr_clear(e->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_clear(e->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_clear(e->linexpr.MPFR);
    break;
  default:
    abort();
  }
}

void ap_linexpr0_free(ap_linexpr0_t* e)
{
  ap_linexpr0_clear(e);
  free(e);
}

void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t* e, char** name_of_dim)
{
  switch (e->discr){
  case AP_SCALAR_D:
    itvD_linexpr_fprint(stream,e->linexpr.D,name_of_dim);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_fprint(stream,e->linexpr.MPQ,name_of_dim);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_fprint(stream,e->linexpr.MPFR,name_of_dim);
    break;
  default:
    abort();
  }
}
void ap_linexpr0_array_fprint(FILE* stream, ap_linexpr0_array_t* tab, char** name_of_dim)
{
  switch (tab->discr){
  case AP_SCALAR_D:
    itvD_linexpr_array_fprint(stream,tab->p.D,name_of_dim);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_array_fprint(stream,tab->p.MPQ,name_of_dim);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_array_fprint(stream,tab->p.MPFR,name_of_dim);
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

ap_coeff_t ap_linexpr0_cstref(ap_linexpr0_t* expr, ap_dim_t dim)
{
  ap_coeff_t coeff;
  coeff->discr = expr->discr;
  switch (e->discr){
  case AP_SCALAR_D:
    coeff->eitv.D = expr->linexpr.D->cst;
    break;
  case AP_SCALAR_MPQ:
    coeff->eitv.MPQ = expr->linexpr.MPQ->cst;
    break;
  case AP_SCALAR_MPFR:
    coeff->eitv.MPFR = expr->linexpr.MPFR->cst;
    break;
  default:
    abort();
  }
}
