/* ********************************************************************** */
/* numMPZ.c */
/* ********************************************************************** */

#include "numMPZ.h"
#include "num_internal.h"

void numMPZ_sqrt(numMPZ_t up, numMPZ_t down, numMPZ_t b)
{
  int perfect;
  assert(mpz_sgn(b)>=0);
  perfect = mpz_perfect_square_p(b);
  mpz_sqrt(down,b);
  if (perfect) mpz_set(up,down);
  else mpz_add_ui(up,down,1);
}

int numMPZ_snprint(char* s, size_t size, numMPZ_t a)
{
  int res;
  if (mpz_sizeinbase(a,10)+2>size)
    res = snprintf(s,size, mpz_sgn(a)>0 ? "+BIG" : "-BIG");
  else {
    mpz_get_str(s,10,a);
    res = strlen(s);
  }
  return res;
}

size_t numMPZ_serialize(void* dst, numMPZ_t src)
{
  size_t count = 0;
  *((char*)dst) = mpz_sgn(src);
  mpz_export((char*)dst+5,&count,1,1,1,0,src);
  assert(((unsigned)count)==count);
  num_dump_word32((char*)dst+1,(unsigned)count);
  return count+5;
}

size_t numMPZ_deserialize(numMPZ_t dst, const void* src)
{
  size_t count = num_undump_word32((const char*)src+1);
  mpz_import(dst,count,1,1,1,0,(const char*)src+5);
  if (*(const char*)src<0)
    mpz_neg(dst,dst);
  return count+5;
}

size_t numMPZ_serialize_array(void* dst, numMPZ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numMPZ_deserialize_array(numMPZ_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numMPZ_serialized_size_array(numMPZ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPZ_serialized_size(src[i]);
  return n;
}
