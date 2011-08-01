/* ********************************************************************** */
/* numRsss.c */
/* ********************************************************************** */

#include "numRsss.h"

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

void numRsss_canonicalize(numRsss_t r)
{
  if (r->d){
    numIsss_t pgcd;
    numIsss_gcd(pgcd,r->n,r->d);
    if (*pgcd==0 || (*pgcd==-1 && (*r->d==NUMIsss_MIN || *r->n==NUMIsss_MIN))) {
      fprintf(stderr,"overflow in numRsss_canonicalize\n");
      return;
    }
    *r->n /= *pgcd;
    *r->d /= *pgcd;
  }
  else {
    numIsss_set_int(r->n,1);
  }
}

void numRsss_inv(numRsss_t a, numRsss_t b)
{
  if (a!=b)
    *a = *b;
  numIsss_swap(a->n,a->d);
  if (*a->d<0){
    numIsss_neg(a->n,a->n);
    numIsss_neg(a->d,a->d);
  }
}
void numRsss_add(numRsss_t a, numRsss_t b, numRsss_t c)
{
  numIsss_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d + *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIsss_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) + (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRsss_canonicalize(a);
}
void numRsss_sub(numRsss_t a, numRsss_t b, numRsss_t c)
{
  numIsss_t d;
#if 0
  *d = *b->d * *c->d;
  *a->n = *b->n * *c->d - *b->d * *c->n;
  *a->d = *d;
#else /* more cost but less overflows */
  numIsss_lcm(d,b->d,c->d);
  *a->n = *b->n * (*d / *b->d) - (*d / *c->d) * *c->n;
  *a->d = *d;
#endif
  numRsss_canonicalize(a);
}
void numRsss_div(numRsss_t a, numRsss_t b, numRsss_t c)
{
  numIsss_t d;
  *d = *b->d * *c->n;
  if (*d<0) {
    *a->n = - *b->n * *c->d;
    *a->d = - *d;
  }
  else {
    *a->n = *b->n * *c->d;
    *a->d = *d;
  }
  numRsss_canonicalize(a);
}
void numRsss_sqrt(numRsss_t up, numRsss_t down, numRsss_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  numIsss_t pq;
  assert(*b->n>=0);
  numIsss_mul(pq, numRsss_numref(b), numRsss_denref(b));
  numIsss_sqrt(numRsss_numref(up), numRsss_numref(down), pq);
  numIsss_set(numRsss_denref(up),numRsss_denref(b));
  numIsss_set(numRsss_denref(down),numRsss_denref(b));
  numRsss_canonicalize(up);
  numRsss_canonicalize(down);
}
void numRsss_mul_2exp(numRsss_t a, numRsss_t b, int c)
{
  if (*b->n==0){
    numRsss_set_int(a,0);
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

int numRsss_cmp(numRsss_t a, numRsss_t b)
{
  numIsss_t aa,bb;
#if 0
  *aa = *a->n * *b->d;
  *bb = *a->d * *b->n;
#else /* more cost but less overflows */
  numIsss_t d;
  numIsss_lcm(d,a->d,b->d);
  *aa = *a->n * (*d / *a->d);
  *bb = (*d / *b->d) * *b->n;
#endif
  return numIsss_cmp(aa,bb);
}

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

void numRsss_fprint(FILE* stream, numRsss_t a)
{
  if (*a->n==0)
    fprintf(stream,"0");
  else if (*a->d==1)
    numIsss_fprint(stream,a->n);
  else {
    numIsss_fprint(stream,a->n);
    fprintf(stream,"/");
    numIsss_fprint(stream,a->d);
  }
}
int numRsss_snprint(char* s, size_t size, numRsss_t a)
{
  int res;
  if (*a->n==0)
    res = snprintf(s,size, "0");
  else if (*a->d==1)
    res = numIsss_snprint(s,size,a->n);
  else {
    res = numIsss_snprint(s,size,a->n);
    if ((unsigned int)res<size) res += snprintf(s+res,size-res,"/");
    if ((unsigned int)res<size) res += numIsss_snprint(s+res,size-res,a->n);
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t numRsss_serialize_array(void* dst, numRsss_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRsss_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numRsss_deserialize_array(numRsss_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRsss_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numRsss_serialized_size_array(numRsss_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numRsss_serialized_size(src[i]);
  return n;
}
