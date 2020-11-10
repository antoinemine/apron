/*
 * numitv.h
 *
 * Functions for outward interval arithmetic with floating point arithmetic
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */


/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef __NUMITV_H
#define __NUMITV_H

/* dependencies */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   numdbl_t inf;
   numdbl_t sup;
} numitv_t;

numitv_t numitv_neg(numitv_t a);
numitv_t numitv_add(numitv_t a,numitv_t b);
numitv_t numitv_sub(numitv_t a,numitv_t b);
numitv_t numitv_mul(numitv_t a,numitv_t b);
numitv_t numitv_div(numitv_t a,numitv_t b);

bool numitv_is_leq(numitv_t a,numitv_t b);
numitv_t numitv_singleton(numdbl_t a);
void numitv_print(numitv_t a);
numdbl_t numitv_width(numitv_t a);

numitv_t numitv_add_number(numdbl_t a,numdbl_t b);
numitv_t numitv_sub_number(numdbl_t a,numdbl_t b);
numitv_t numitv_mul_number(numdbl_t a,numdbl_t b);
numitv_t numitv_div_number(numdbl_t a,numdbl_t b);

numitv_t numitv_lexp_range(int dim, numdbl_t *lexp_coeff, numdbl_t *bounds);


numdbl_t  __attribute__ ((noinline)) num_add_downward(numdbl_t a, numdbl_t b);
numdbl_t  __attribute__ ((noinline)) num_sub_downward(numdbl_t a, numdbl_t b);
numdbl_t  __attribute__ ((noinline)) num_mul_downward(numdbl_t a, numdbl_t b);
numdbl_t  __attribute__ ((noinline)) num_div_downward(numdbl_t a, numdbl_t b);

#ifdef __cplusplus
}
#endif

#endif /* __NUMITV_H */
