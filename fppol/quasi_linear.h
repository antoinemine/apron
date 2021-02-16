/*
 * quasi_linear.h
 *
 * Functions for converting the interval linear form into the quasi-linearisation form.
 *
 * APRON Library / FPPol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
  with an exception allowing the redistribution of statically linked
  executables.

  Please read the COPYING file packaged in the distribution.
*/

#ifndef _QUASI_LINEAR_H_
#define _QUASI_LINEAR_H_

#ifdef __cplusplus
extern "C" {
#endif

numdbl_t *quasi_linearisation(int dim, numdbl_t *coeffs, numdbl_t *bounds, int *sat);
int normalise_steep_inequality(int dim, numdbl_t *con, numdbl_t *bnds);
int normalise_quasiInteger_inequality(int dim, numdbl_t *con, numdbl_t *bnds);
int normalise_integer_inequality(int dim, numdbl_t *c);
numdbl_t *normalise_integer_inequality_withConstant(int dim, numdbl_t *c,bool *sat);
#ifdef __cplusplus
}
#endif

#endif
