/* ********************************************************************** */
/* numratMPQ_def.h */
/* ********************************************************************** */

#ifndef _NUMRATMPQ_DEF_H_
#define _NUMRATMPQ_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "numConfig.h"
#include "numratMPQ.h"
#include "numintMPZ_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

static inline void numratMPQ_canonicalize(numratMPQ_t a)
{ mpq_canonicalize(a); }

static inline bool numratMPQ_set_numintMPZ2(numratMPQ_t a, numintMPZ_t b, numintMPZ_t c)
{
  numintMPZ_set(numratMPQ_numref(a),b);
  numintMPZ_set(numratMPQ_denref(a),c);
  numratMPQ_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numratMPQ_set(numratMPQ_t a, numratMPQ_t b)
{ mpq_set(a,b); }
static inline void numratMPQ_set_array(numratMPQ_t* a, numratMPQ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_set(a[i],b[i]);
}
static inline void numratMPQ_set_int(numratMPQ_t a, long int i)
{ mpq_set_si(a,i,1); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numratMPQ_init(numratMPQ_t a)
{ mpq_init(a); }
static inline void numratMPQ_init_array(numratMPQ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_init(a[i]);
}
static inline void numratMPQ_init_set(numratMPQ_t a, numratMPQ_t b)
{ mpq_init(a); mpq_set(a,b); }
static inline void numratMPQ_init_set_int(numratMPQ_t a, long int i)
{ mpq_init(a); mpq_set_si(a,i,1); }

static inline void numratMPQ_clear(numratMPQ_t a)
{ mpq_clear(a); }
static inline void numratMPQ_clear_array(numratMPQ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpq_clear(a[i]);
}
static inline void numratMPQ_swap(numratMPQ_t a, numratMPQ_t b)
{ mpq_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numratMPQ_neg(numratMPQ_t a, numratMPQ_t b)
{ mpq_neg(a,b); }
static inline void numratMPQ_inv(numratMPQ_t a, numratMPQ_t b)
{ mpq_inv(a,b); }
static inline void numratMPQ_abs(numratMPQ_t a, numratMPQ_t b)
{ mpq_abs(a,b); }
static inline void numratMPQ_add(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_add(a,b,c); }
static inline void numratMPQ_add_uint(numratMPQ_t a, numratMPQ_t b, unsigned long int c)
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
static inline void numratMPQ_sub(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_sub(a,b,c); }
static inline void numratMPQ_sub_uint(numratMPQ_t a, numratMPQ_t b, unsigned long int c)
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
static inline void numratMPQ_mul(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_mul(a,b,c); }
static inline void numratMPQ_mul_2(numratMPQ_t a, numratMPQ_t b)
{ mpq_mul_2exp(a,b,1); }
static inline void numratMPQ_div(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_div(a,b,c); }
static inline void numratMPQ_div_2(numratMPQ_t a, numratMPQ_t b)
{ mpq_div_2exp(a,b,1); }
static inline void numratMPQ_min(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_set(a, mpq_cmp(b,c)<=0 ? b : c); }
static inline void numratMPQ_max(numratMPQ_t a, numratMPQ_t b, numratMPQ_t c)
{ mpq_set(a, mpq_cmp(b,c)>=0 ? b : c); }
static inline void numratMPQ_floor(numratMPQ_t a, numratMPQ_t b)
{
  numintMPZ_fdiv_q(numratMPQ_numref(a),numratMPQ_numref(b),numratMPQ_denref(b));
  numintMPZ_set_int(numratMPQ_denref(a),1);
}
static inline void numratMPQ_ceil(numratMPQ_t a, numratMPQ_t b)
{
  numintMPZ_cdiv_q(numratMPQ_numref(a),numratMPQ_numref(b),numratMPQ_denref(b));
  numintMPZ_set_int(numratMPQ_denref(a),1);
}
static inline void numratMPQ_trunc(numratMPQ_t a, numratMPQ_t b)
{
  numintMPZ_tdiv_q(numratMPQ_numref(a),numratMPQ_numref(b),numratMPQ_denref(b));
  numintMPZ_set_int(numratMPQ_denref(a),1);
}
static inline void numratMPQ_sqrt(numratMPQ_t up, numratMPQ_t down, numratMPQ_t b)
{
  /* compute sqrt(p/q) as sqrt(p*q)/q */
  mpz_t tmp;
  int perfect;
  assert(mpq_sgn(b)>=0);
  mpz_init(tmp);
  mpz_mul(tmp,mpq_numref(b),mpq_denref(b));
  perfect = mpz_perfect_square_p(tmp);
  mpz_sqrt(mpq_numref(down),tmp);
  if (perfect) mpz_set(mpq_numref(up),mpq_numref(down));
  else mpz_add_ui(mpq_numref(up),mpq_numref(down),1);
  mpz_set(mpq_denref(up),mpq_denref(b));
  mpz_set(mpq_denref(down),mpq_denref(b));
  numratMPQ_canonicalize(up);
  numratMPQ_canonicalize(down);
  mpz_clear(tmp);
}

static inline void numratMPQ_mul_2exp(numratMPQ_t a, numratMPQ_t b, int c)
{
  if (c>=0) mpq_mul_2exp(a,b,c);
  else mpq_div_2exp(a,b,-c);
}

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numratMPQ_sgn(numratMPQ_t a)
{ return mpq_sgn(a); }
static inline int numratMPQ_cmp(numratMPQ_t a, numratMPQ_t b)
{ return mpq_cmp(a,b); }
static inline int numratMPQ_cmp_int(numratMPQ_t a, long int b)
{ return mpq_cmp_si(a,b,1); }
static inline bool numratMPQ_equal(numratMPQ_t a, numratMPQ_t b)
{ return mpq_equal(a,b); }
static inline bool numratMPQ_integer(numratMPQ_t a)
{ return (mpz_cmp_ui(mpq_denref(a),1)==0); }
static inline int numratMPQ_hash(numratMPQ_t a)
{
  double d = mpq_get_d(a);
  if (d <= -(double)(INT_MAX))
    return -INT_MAX;
  else if (d < (double)INT_MAX)
    return lrint(d);
  else
    return INT_MAX;
}
/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numratMPQ_print(numratMPQ_t a)
{ mpq_out_str(stdout,10,a); }
static inline void numratMPQ_fprint(FILE* stream, numratMPQ_t a)
{ mpq_out_str(stream,10,a); }
static inline int numratMPQ_snprint(char* s, size_t size, numratMPQ_t a)
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

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numratMPQ_serialize_id(void)
{ return 0x1f; }

static inline size_t numratMPQ_serialize(void* dst, numratMPQ_t src)
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

static inline size_t numratMPQ_deserialize(numratMPQ_t dst, const void* src)
{
  size_t count1 = num_undump_word32((const char*)src+1);
  size_t count2 = num_undump_word32((const char*)src+5);
  mpz_import(mpq_numref(dst),count1,1,1,1,0,(const char*)src+9);
  mpz_import(mpq_denref(dst),count2,1,1,1,0,(const char*)src+count1+9);
  if (*(const char*)src<0)
    mpq_neg(dst,dst);
  return count1+count2+9;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numratMPQ_serialized_size(numratMPQ_t a)
{
  return
    (mpz_sizeinbase(mpq_numref(a),2)+mpz_sizeinbase(mpq_denref(a),2))/8+
    9+2*sizeof(mp_limb_t);
}

#ifdef __cplusplus
}
#endif

#endif
