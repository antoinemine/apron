/* ********************************************************************** */
/* numDwww.c */
/* ********************************************************************** */

#include "numDwww.h"
#include "num_internal.h"

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

void numDwww_sqrt(numDwww_t up, numDwww_t down, const numDwww_t b)
{
  assert(*b>=0);
  *up = sqrtwww(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafterwww(*up,0);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

int numDwww_hash(const numDwww_t a)
{
 if (*a <= -(numDwww_native)(INT_MAX))
    return -INT_MAX;
  else if (*a < (numDwww_native)INT_MAX)
    return lrintwww(*a);
  else
    return INT_MAX;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t numDwww_serialize_array(void* dst, numDwww_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDwww_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numDwww_deserialize_array(numDwww_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDwww_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numDwww_serialized_size_array(numDwww_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDwww_serialized_size(src[i]);
  return n;
}
