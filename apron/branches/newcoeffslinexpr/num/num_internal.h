/* ********************************************************************** */
/* num_global.h: workspace and serialization */
/* ********************************************************************** */

#ifndef _NUM_INTERNAL_H_
#define _NUM_INTERNAL_H_

#include <stdio.h>
#include <string.h>
#include "num_types.h"

void num_internal_init(num_internal_t intern);
void num_internal_clear(num_internal_t intern);
num_internal_ptr num_internal_alloc(void);
void num_internal_free(num_internal_ptr intern);

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

#endif
