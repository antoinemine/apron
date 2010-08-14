/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_linexpr1.h: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_LINEXPR1_H_
#define _AP_LINEXPR1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap_linexpr0.h"
#include "ap_environment.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Linear expressions */
typedef struct ap_linexpr1_struct {
  ap_linexpr0_t linexpr0;
  ap_environment_t* env;
} ap_linexpr1_sruct;
typedef ap_linexpr1_struct ap_linexpr1_t[1];
typedef ap_linexpr1_struct* ap_linexpr1_ptr;


#ifdef __cplusplus
}
#endif

#endif
