/* ********************************************************************** */
/* numIsss.c */
/* ********************************************************************** */

#include "numIsss.h"

#define _NUMIsss_MARK_

void numIsss_cdiv_qr(numIsss_t q, numIsss_t r, numIsss_t a, numIsss_t b)
{
  numIsss_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numIsss_sgn(a)*numIsss_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}
void numIsss_sqrt(numIsss_t up, numIsss_t down, numIsss_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numIsss_max_exact_double) {
    /* ceil(sqrt(b)) can be exactly represented as a double */
    *up = ceil(f);
    *down = floor(f);
  }
  else {
    /* (unlikely) case where ulp(sqrt(b)) might  be greater than 1 */
    *up = ceil(nextafter(f,+1/0.));
    *down = floor(nextafter(f,0.));
  }
}

numIsss_native _numIsss_gcd_aux(numIsss_native a, numIsss_native b)
{
  numIsss_native t;
  numIsss_abs(&a,&a);
  numIsss_abs(&b,&b);
  if (a<b){
    numIsss_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMIsss_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}

size_t numIsss_serialize_array(void* dst, numIsss_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIsss_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numIsss_deserialize_array(numIsss_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIsss_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numIsss_serialized_size_array(numIsss_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIsss_serialized_size(src[i]);
  return n;
}

#undef _NUMIsss_MARK_
