/* ********************************************************************** */
/* numMPQ.c */
/* ********************************************************************** */

#include "numMPQ.h"
#include "num_internal.h"

void numMPQ_add_uint(numMPQ_t a, numMPQ_t b, unsigned long int c)
{
  if (a==b) {
    mpq_t tmp;
    mpq_init(tmp);
    mpq_set_ui(tmp,c,1);
    mpq_add(a,b,tmp);
    mpq_clear(tmp);
  }
  else {
    mpq_set_ui(a,c,1);
    mpq_add(a,a,b);
  }
}

void numMPQ_sub_uint(numMPQ_t a, numMPQ_t b, unsigned long int c)
{
  if (a==b) {
    mpq_t tmp;
    mpq_init(tmp);
    mpq_set_ui(tmp,c,1);
    mpq_sub(a,b,tmp);
    mpq_clear(tmp);
  }
  else {
    mpq_set_ui(a,c,1);
    mpq_sub(a,b,a);
  }
}
void numMPQ_sqrt(numMPQ_t up, numMPQ_t down, numMPQ_t b)
{
  if (mpz_cmp_ui(mpq_denref(b),1)==0){
    numMPZ_sqrt(mpq_numref(up),mpq_numref(down),mpq_numref(b));
    mpz_set_ui(mpq_denref(up),1);
    mpz_set_ui(mpq_denref(down),1);
  }
  else {
    /* compute sqrt(p/q) as sqrt(p*q)/q */
    mpz_t tmp;
    int perfect;
    assert(mpq_sgn(b)>=0);
    mpz_init(tmp);
    mpz_mul(tmp,mpq_numref(b),mpq_denref(b));
    numMPZ_sqrt(mpq_numref(up),mpq_numref(down),tmp);
    mpz_set(mpq_denref(up),mpq_denref(b));
    mpz_set(mpq_denref(down),mpq_denref(b));
    numMPQ_canonicalize(up);
    numMPQ_canonicalize(down);
    mpz_clear(tmp);
  }
}

int numMPQ_hash(numMPQ_t a)
{
  double d = mpq_get_d(a);
  if (d <= -(double)(INT_MAX))
    return -INT_MAX;
  else if (d < (double)INT_MAX)
    return lrint(d);
  else
    return INT_MAX;
}

int numMPQ_snprint(char* s, size_t size, numMPQ_t a)
{
  int res;
  if (mpz_sizeinbase(mpq_numref(a),10) +
      mpz_sizeinbase(mpq_denref(a),10) +
      3 > size )
    res = snprintf(s,size, mpq_sgn(a)>0 ? "+BIG" : "-BIG");
  else {
    mpq_get_str(s,10,a);
    res = strlen(s);
  }
  return res;
}

size_t numMPQ_serialize(void* dst, numMPQ_t src)
{
  size_t count1 = 0;
  size_t count2 = 0;
  *((char*)dst) = mpq_sgn(src);
  mpz_export((char*)dst+9,&count1,1,1,1,0,mpq_numref(src));
  mpz_export((char*)dst+count1+9,&count2,1,1,1,0,mpq_denref(src));
  assert(((unsigned)count1)==count1);
  assert(((unsigned)count2)==count2);
  num_dump_word32((char*)dst+1,(unsigned)count1);
  num_dump_word32((char*)dst+5,(unsigned)count2);
  return count1+count2+9;
}

size_t numMPQ_deserialize(numMPQ_t dst, const void* src)
{
  size_t count1 = num_undump_word32((const char*)src+1);
  size_t count2 = num_undump_word32((const char*)src+5);
  mpz_import(mpq_numref(dst),count1,1,1,1,0,(const char*)src+9);
  mpz_import(mpq_denref(dst),count2,1,1,1,0,(const char*)src+count1+9);
  if (*(const char*)src<0)
    mpq_neg(dst,dst);
  return count1+count2+9;
}

size_t numMPQ_serialize_array(void* dst, numMPQ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_serialize((char*)dst+n,src[i]);
  return n;
}

size_t numMPQ_deserialize_array(numMPQ_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t numMPQ_serialized_size_array(numMPQ_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += numMPQ_serialized_size(src[i]);
  return n;
}
