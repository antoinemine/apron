/* ********************************************************************** */
/* numDyyy.c */
/* ********************************************************************** */

#include "numDyyy.h"
#include "num_internal.h"

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

void numDyyy_sqrt(numDyyy_t up, numDyyy_t down, numDyyy_t b)
{
  assert(*b>=0);
  *up = sqrtyyy(*b);
  assert(*up**up>=*b); /* assumes round towards +oo! */
  if (*up**up==*b) *down = *up;
  else *down = nextafteryyy(*up,0);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

int numDyyy_hash(numDyyy_t a)
{
 if (*a <= -(numDyyy_native)(INT_MAX))
    return -INT_MAX;
  else if (*a < (numDyyy_native)INT_MAX)
    return lrintyyy(*a);
  else
    return INT_MAX;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t numDyyy_serialize_array(void* dst, numDyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numDyyy_deserialize_array(numDyyy_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numDyyy_serialized_size_array(numDyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numDyyy_serialized_size(src[i]);
  return n;
}
