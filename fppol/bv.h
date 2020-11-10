/*
 * bv.h
 *
 * Functions for Fixed-size Bit Vectors for Kohler's Rule
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

#ifndef _BV_H_
#define _BV_H_

#include "fpp.h"
#include "fpp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  unsigned vsize; /* size of each vector */
  unsigned vchars; /* number of chars for each vector */
  unsigned nvecs;   /* number of vectors  */
  char *bits;       /* memory for storage */
}bit_vectors_t;

typedef struct {
  unsigned posI; /* index of postive constraint */
  unsigned negI; /* index of negative constraint */
}fm_pair_t;

void bv_free(bit_vectors_t *bv);
bit_vectors_t* bv_copy(bit_vectors_t *bv);
char* bv_or(bit_vectors_t *bv, char *v1,char *v2);
void bv_set(bit_vectors_t *bv, unsigned k,char *v);
bool bv_subset(bit_vectors_t *bv, char *v1,char *v2);
fpp_t *bv_redundancy_removal_firstKs_byKohler(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv);
void fpp_fdump_withBV(FILE* stream, fpp_t* a,bit_vectors_t *bv);

int bv_andTest(bit_vectors_t *bv, char *v1,char *v2);
fpp_t *bv_redundancy_removal_firstKs_byEnvelope(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv,char *nenv1bv,char *nenv2bv);
fm_pair_t *bv_redundancy_removal_byKohler_lazy(fpp_internal_t* pr,unsigned k,unsigned zeroI,unsigned nelimv, fm_pair_t *fmp, bit_vectors_t *bv);
void bv_fprint(FILE* stream, bit_vectors_t *bv);
unsigned bv_1bits(bit_vectors_t *bv, char *v);
void charprint(char c);
#ifdef __cplusplus
}
#endif

#endif
