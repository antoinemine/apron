/* ********************************************************************** */
/* numIyyy.c */
/* ********************************************************************** */

#include "numIyyy.h"

#define _NUMIyyy_MARK_

void numIyyy_cdiv_qr(numIyyy_t q, numIyyy_t r, numIyyy_t a, numIyyy_t b)
{
  numIyyy_t qq,rr;
  *qq = *a / *b;
  *rr = *a % *b;
  if (numIyyy_sgn(a)*numIyyy_sgn(b)>0 && *rr) {
    *q = *qq + 1;
    *r = *rr - *b;
  }
  else {
    *q = *qq;
    *r = *rr;
  }
}
void numIyyy_sqrt(numIyyy_t up, numIyyy_t down, numIyyy_t b)
{
  double f = sqrt(*b);
  assert(*b>=0);
  if (*b<numIyyy_max_exact_double) {
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

numIyyy_native _numIyyy_gcd_aux(numIyyy_native a, numIyyy_native b)
{
  numIyyy_native t;
  numIyyy_abs(&a,&a);
  numIyyy_abs(&b,&b);
  if (a<b){
    numIyyy_native t=a; a=b; b=t;
  }
  /* a is supposed to be greater than b */
  while (b!=NUMIyyy_ZERO && a!=b) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}

size_t numIyyy_serialize_array(void* dst, numIyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numIyyy_deserialize_array(numIyyy_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numIyyy_serialized_size_array(numIyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numIyyy_serialized_size(src[i]);
  return n;
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

#if defined(_NUMIl_MARK_)

#elif defined(_NUMIll_MARK_)


#else
#error "HERE"
#endif

#undef _NUMIyyy_MARK_
