/* ********************************************************************** */
/* numintMPZ_def.h */
/* ********************************************************************** */

#ifndef _NUMINTMPZ_DEF_H_
#define _NUMINTMPZ_DEF_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "numConfig.h"
#include "numintMPZ.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Require C99 compliant compiler */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void numintMPZ_set(numintMPZ_t a, numintMPZ_t b)
{ mpz_set(a,b); }
static inline void numintMPZ_set_array(numintMPZ_t* a, numintMPZ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_set(a[i],b[i]);
}
static inline void numintMPZ_set_int(numintMPZ_t a, long int i)
{ mpz_set_si(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintMPZ_init(numintMPZ_t a)
{ mpz_init(a); }
static inline void numintMPZ_init_array(numintMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_init(a[i]);
}
static inline void numintMPZ_init_set(numintMPZ_t a, numintMPZ_t b)
{ mpz_init_set(a,b); }
static inline void numintMPZ_init_set_int(numintMPZ_t a, long int i)
{ mpz_init_set_si(a,i); }

static inline void numintMPZ_clear(numintMPZ_t a)
{ mpz_clear(a); }
static inline void numintMPZ_clear_array(numintMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_clear(a[i]);
}
static inline void numintMPZ_swap(numintMPZ_t a, numintMPZ_t b)
{ mpz_swap(a,b); }
/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintMPZ_neg(numintMPZ_t a, numintMPZ_t b)
{ mpz_neg(a,b); }
static inline void numintMPZ_abs(numintMPZ_t a, numintMPZ_t b)
{ mpz_abs(a,b); }
static inline void numintMPZ_add(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_add(a,b,c); }
static inline void numintMPZ_add_uint(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_add_ui(a,b,c); }
static inline void numintMPZ_sub(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_sub(a,b,c); }
static inline void numintMPZ_sub_uint(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_sub_ui(a,b,c); }
static inline void numintMPZ_mul(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_mul(a,b,c); }
static inline void numintMPZ_mul_2(numintMPZ_t a, numintMPZ_t b)
{ mpz_mul_2exp(a,b,1); }
static inline void numintMPZ_fdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_fdiv_q(a,b,c); }
static inline void numintMPZ_cdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_cdiv_q(a,b,c); }
static inline void numintMPZ_tdiv_q(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_tdiv_q(a,b,c); }
static inline void numintMPZ_cdiv_qr(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c, numintMPZ_t d)
{ mpz_cdiv_qr(a,b,c,d); }
static inline void numintMPZ_cdiv_2(numintMPZ_t a, numintMPZ_t b)
{ mpz_cdiv_q_2exp(a,b,1); }
static inline void numintMPZ_cdiv_q_2exp(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_cdiv_q_2exp(a,b,c); }
static inline void numintMPZ_fdiv_q_2exp(numintMPZ_t a, numintMPZ_t b, unsigned long int c)
{ mpz_fdiv_q_2exp(a,b,c); }
static inline void numintMPZ_min(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)<=0 ? b : c); }
static inline void numintMPZ_max(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)>=0 ? b : c); }

static inline void numintMPZ_sqrt(numintMPZ_t up, numintMPZ_t down, numintMPZ_t b)
{
  int perfect;
  assert(mpz_sgn(b)>=0);
  perfect = mpz_perfect_square_p(b);
  mpz_sqrt(down,b);
  if (perfect) mpz_set(up,down);
  else mpz_add_ui(up,down,1);
}

static inline void numintMPZ_mul_2exp(numintMPZ_t a, numintMPZ_t b, int c)
{
  if (c>=0) mpz_mul_2exp(a,b,c);
  else mpz_cdiv_q_2exp(a,b,-c);
}


/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numintMPZ_divexact(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_divexact(a,b,c); }
static inline void numintMPZ_mod(numintMPZ_t a, numintMPZ_t b, numintMPZ_t c)
{ mpz_mod(a,b,c); }
static inline void numintMPZ_gcd(numintMPZ_t a, numintMPZ_t b,  numintMPZ_t c)
{ mpz_gcd(a,b,c); }
static inline void numintMPZ_lcm(numintMPZ_t a, numintMPZ_t b,  numintMPZ_t c)
{ mpz_lcm(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintMPZ_sgn(numintMPZ_t a)
{ return mpz_sgn(a); }
static inline int numintMPZ_cmp(numintMPZ_t a, numintMPZ_t b)
{ return mpz_cmp(a,b); }
static inline int numintMPZ_cmp_int(numintMPZ_t a, long int b)
{ return mpz_cmp_si(a,b); }
static inline bool numintMPZ_equal(numintMPZ_t a, numintMPZ_t b)
{ return mpz_cmp(a,b)==0; }
static inline bool numintMPZ_integer(numintMPZ_t a)
{ return true; }
static inline int numintMPZ_hash(numintMPZ_t a)
{ return mpz_get_si(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintMPZ_print(numintMPZ_t a)
{ mpz_out_str(stdout,10,a); }
static inline void numintMPZ_fprint(FILE* stream, numintMPZ_t a)
{ mpz_out_str(stream,10,a); }
static inline int numintMPZ_snprint(char* s, size_t size, numintMPZ_t a)
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

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintMPZ_serialize_id(void)
{ return 0xf; }

static inline size_t numintMPZ_serialize(void* dst, numintMPZ_t src)
{
  size_t count = 0;
  *((char*)dst) = mpz_sgn(src);
  mpz_export((char*)dst+5,&count,1,1,1,0,src);
  assert(((unsigned)count)==count);
  num_dump_word32((char*)dst+1,(unsigned)count);
  return count+5;
}

static inline size_t numintMPZ_deserialize(numintMPZ_t dst, const void* src)
{
  size_t count = num_undump_word32((const char*)src+1);
  mpz_import(dst,count,1,1,1,0,(const char*)src+5);
  if (*(const char*)src<0)
    mpz_neg(dst,dst);
  return count+5;
}

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numintMPZ_serialized_size(numintMPZ_t a)
{ return mpz_sizeinbase(a,2)/8+5+sizeof(mp_limb_t); }

#ifdef __cplusplus
}
#endif

#endif
