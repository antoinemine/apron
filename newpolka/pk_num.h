
/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _PK_CONFIG_H_
#define _PK_CONFIG_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "numMPQ.h"

#ifdef __cplusplus
extern "C" {
#endif

/* size in words */
static inline size_t numintMPQ_size(numintMPQ_t a)
{ return mpz_size(a); }
static inline size_t numintMPQ_size2(numintMPQ_t a)
{ return mpz_sizeinbase(a,2); }

#ifdef __cplusplus
}
#endif

#endif

