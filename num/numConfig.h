#ifndef _NUMCONFIG_H_
#define _NUMCONFIG_H_

#include <string.h>
#include <float.h>
#include <stdint.h>
#include "stdio.h"
#include "gmp.h"
#include "mpfr.h"

#ifdef __cplusplus
#define HAS_BOOL
extern "C" {
#endif

#ifndef HAS_BOOL
#define HAS_BOOL
typedef char bool;
static const bool false = 0;
static const bool true  = 1;
#endif


/* prints attempts to convert NaN and infinities to non floating-point types */
#define DEBUG_SPECIAL fprintf(stderr,"invalid floating-point in %s\n",__func__)

typedef struct __numinternal_struct {
  mpz_t q;
  mpz_t r;
  mpq_t mpq;
  mpfr_t dbl;
  mpfr_t ldbl;
} __numinternal_struct;
typedef __numinternal_struct* numinternal_ptr;
typedef __numinternal_struct numinternal_t[1];

static inline void numinternal_init(numinternal_t intern)
{
  mpz_init(intern->q); mpz_init(intern->r);
  mpq_init(intern->mpq);
  mpfr_init2(intern->dbl,DBL_MANT_DIG);
  mpfr_init2(intern->ldbl,LDBL_MANT_DIG);
}
static inline void numinternal_clear(numinternal_t intern)
{
  mpz_clear(intern->q); mpz_clear(intern->r);
  mpq_clear(intern->mpq);
  mpfr_clear(intern->dbl); mpfr_clear(intern->ldbl);
}

static inline void num_store_words8(void* dst, const void* src, size_t t)
{
#ifdef WORDS_BIGENDIAN
  unsigned char *s = (unsigned char*)src+t-1, *d = (unsigned char*) dst;
  size_t i;
  for (i=0;i<t;i++,s--,d++) *d = *s;
#else
  memcpy(dst,src,t);
#endif
}

static inline void num_dump_word32(void* dst, unsigned src)
{
  *((unsigned char*)dst+0) = (src>>24) & 0xff;
  *((unsigned char*)dst+1) = (src>>16) & 0xff;
  *((unsigned char*)dst+2) = (src>>8) & 0xff;
  *((unsigned char*)dst+3) = src & 0xff;
}

static inline unsigned num_undump_word32(const void* src)
{
  return
    ((unsigned)(((const unsigned char*)src)[0])<<24)+
    ((unsigned)(((const unsigned char*)src)[1])<<16)+
    ((unsigned)(((const unsigned char*)src)[2])<<8)+
    ((unsigned)(((const unsigned char*)src)[3]));
}

#ifdef __cplusplus
}
#endif

#endif
