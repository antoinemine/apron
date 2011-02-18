/* ********************************************************************** */
/* numRyyy.c */
/* ********************************************************************** */

#include "numRyyy.h"

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

void numRyyy_canonicalize(numRyyy_t r)
{
  if (r->d){
    numIyyy_t pgcd;
    numIyyy_gcd(pgcd,r->n,r->d);
    if (*pgcd==0 || (*pgcd==-1 && (*r->d==NUMIyyy_MIN || *r->n==NUMIyyy_MIN))) {
      fprintf(stderr,"overflow in numRyyy_canonicalize\n");
      return;
    }
    *r->n /= *pgcd;
    *r->d /= *pgcd;
  }
  else {
    numIyyy_set_int(r->n,1);
  }
}

void numRyyy_inv(numRyyy_t a, numRyyy_t b)
{
  if (a!=b)
    *a = *b;
  numIyyy_swap(a->n,a->d);
  if (*a->d<0){
    numIyyy_neg(a->n,a->n);
    numIyyy_neg(a->d,a->d);
  }
}
void numRyyy_add(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d + *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIyyy_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) + (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRyyy_canonicalize(a);
}
void numRyyy_sub(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d - *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIyyy_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) - (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRyyy_canonicalize(a);
}
void numRyyy_div(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  numIyyy_t d;
  *d = *b->d * *c->n;
  if (*d<0) {
    *a->n = - *b->n * *c->d;
    *a->d = - *d;
  }
  else {
    *a->n = *b->n * *c->d;
    *a->d = *d;
  }
  numRyyy_canonicalize(a);
}
void numRyyy_sqrt(numRyyy_t up, numRyyy_t down, numRyyy_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  numIyyy_t pq;
  assert(*b->n>=0);
  numIyyy_mul(pq, numRyyy_numref(b), numRyyy_denref(b));
  numIyyy_sqrt(numRyyy_numref(up), numRyyy_numref(down), pq);
  numIyyy_set(numRyyy_denref(up),numRyyy_denref(b));
  numIyyy_set(numRyyy_denref(down),numRyyy_denref(b));
  numRyyy_canonicalize(up);
  numRyyy_canonicalize(down);
}
void numRyyy_mul_2exp(numRyyy_t a, numRyyy_t b, int c)
{
  if (*b->n==0){
    numRyyy_set_int(a,0);
  }
  else {
    if (c>=0) {
      *a->d = *b->d;
      while (c>0 && (*a->d % 2==0)){
	*a->d = *a->d / 2;
	c--;
      }
      *a->n = *b->n << c;
    }
    else {
      c = -c;
      *a->n = *b->n;
      while (c>0 && (*a->n % 2==0)){
	*a->n = *a->n/2;
	c--;
      }
      *a->d = *b->d << c;
    }
  }
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

int numRyyy_cmp(numRyyy_t a, numRyyy_t b)
{
  numIyyy_t aa,bb;
#if 0
  *aa = *a->n * *b->d;
  *bb = *a->d * *b->n;
#else /* more cost but less overflows */
  numIyyy_t d;
  numIyyy_lcm(d,a->d,b->d);
  *aa = *a->n * (*d / *a->d);
  *bb = (*d / *b->d) * *b->n;
#endif
  return numIyyy_cmp(aa,bb);
}

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

void numRyyy_fprint(FILE* stream, numRyyy_t a)
{
  if (*a->n==0)
    fprintf(stream,"0");
  else if (*a->d==1)
    numIyyy_fprint(stream,a->n);
  else {
    numIyyy_fprint(stream,a->n);
    fprintf(stream,"/");
    numIyyy_fprint(stream,a->d);
  }
}
int numRyyy_snprint(char* s, size_t size, numRyyy_t a)
{
  int res;
  if (*a->n==0)
    res = snprintf(s,size, "0");
  else if (*a->d==1)
    res = numIyyy_snprint(s,size,a->n);
  else {
    res = numIyyy_snprint(s,size,a->n);
    res += snprintf(s+res,size-res,"/");
    res += numIyyy_snprint(s+res,size-res,a->n);
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t numRyyy_serialize_array(void* dst, numRyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRyyy_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numRyyy_deserialize_array(numRyyy_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRyyy_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numRyyy_serialized_size_array(numRyyy_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRyyy_serialized_size(src[i]);
  return n;
}
