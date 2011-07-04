#ifndef _NUM_CONFIG_H_
#define _NUM_CONFIG_H_

#include "ap_config.h"

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

#include <string.h>

/* prints attempts to convert NaN and infinities to non floating-point types */
#define DEBUG_SPECIAL fprintf(stderr,"invalid floating-point in %s\n",__func__);


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
