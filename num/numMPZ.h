/* ********************************************************************** */
/* numMPZ.h */
/* ********************************************************************** */

#ifndef _NUMMPZ_H_
#define _NUMMPZ_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "num_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Require C99 compliant compiler */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numMPZ_set(numMPZ_t a, numMPZ_t b)
{ mpz_set(a,b); }

static inline void numMPZ_set_array(numMPZ_t* a, numMPZ_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_set(a[i],b[i]);
}
static inline void numMPZ_set_int(numMPZ_t a, long int i)
{ mpz_set_si(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numMPZ_init(numMPZ_t a)
{ mpz_init(a); }
static inline void numMPZ_init_array(numMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_init(a[i]);
}
static inline void numMPZ_init_set(numMPZ_t a, numMPZ_t b)
{ mpz_init_set(a,b); }
static inline void numMPZ_init_set_int(numMPZ_t a, long int i)
{ mpz_init_set_si(a,i); }

static inline void numMPZ_clear(numMPZ_t a)
{ mpz_clear(a); }
static inline void numMPZ_clear_array(numMPZ_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) mpz_clear(a[i]);
}
static inline void numMPZ_swap(numMPZ_t a, numMPZ_t b)
{ mpz_swap(a,b); }
/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numMPZ_neg(numMPZ_t a, numMPZ_t b)
{ mpz_neg(a,b); }
static inline void numMPZ_abs(numMPZ_t a, numMPZ_t b)
{ mpz_abs(a,b); }
static inline void numMPZ_add(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_add(a,b,c); }
static inline void numMPZ_add_uint(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_add_ui(a,b,c); }
static inline void numMPZ_sub(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_sub(a,b,c); }
static inline void numMPZ_sub_uint(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_sub_ui(a,b,c); }
static inline void numMPZ_mul(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_mul(a,b,c); }
static inline void numMPZ_mul_2(numMPZ_t a, numMPZ_t b)
{ mpz_mul_2exp(a,b,1); }
static inline void numMPZ_fdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_fdiv_q(a,b,c); }
static inline void numMPZ_cdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_cdiv_q(a,b,c); }
static inline void numMPZ_tdiv_q(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_tdiv_q(a,b,c); }
static inline void numMPZ_cdiv_qr(numMPZ_t a, numMPZ_t b, numMPZ_t c, numMPZ_t d)
{ mpz_cdiv_qr(a,b,c,d); }
static inline void numMPZ_cdiv_2(numMPZ_t a, numMPZ_t b)
{ mpz_cdiv_q_2exp(a,b,1); }
static inline void numMPZ_cdiv_q_2exp(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_cdiv_q_2exp(a,b,c); }
static inline void numMPZ_fdiv_q_2exp(numMPZ_t a, numMPZ_t b, unsigned long int c)
{ mpz_fdiv_q_2exp(a,b,c); }
static inline void numMPZ_min(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)<=0 ? b : c); }
static inline void numMPZ_max(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_set(a, mpz_cmp(b,c)>=0 ? b : c); }

void numMPZ_sqrt(numMPZ_t up, numMPZ_t down, numMPZ_t b);

static inline void numMPZ_mul_2exp(numMPZ_t a, numMPZ_t b, int c)
{
  if (c>=0) mpz_mul_2exp(a,b,c);
  else mpz_cdiv_q_2exp(a,b,-c);
}

static inline void numMPZ_trunc(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_floor(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_ceil(numMPZ_t a, numMPZ_t b)
{ numMPZ_set(a,b); }
static inline void numMPZ_div(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ numMPZ_cdiv_q(a,b,c); }
static inline void numMPZ_div_2(numMPZ_t a, numMPZ_t b)
{ numMPZ_cdiv_2(a,b); }

/* ====================================================================== */
/* Arithmetic Integer Operations */
/* ====================================================================== */

static inline void numMPZ_divexact(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_divexact(a,b,c); }
static inline void numMPZ_mod(numMPZ_t a, numMPZ_t b, numMPZ_t c)
{ mpz_mod(a,b,c); }
static inline void numMPZ_gcd(numMPZ_t a, numMPZ_t b,  numMPZ_t c)
{ mpz_gcd(a,b,c); }
static inline void numMPZ_lcm(numMPZ_t a, numMPZ_t b,  numMPZ_t c)
{ mpz_lcm(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numMPZ_sgn(numMPZ_t a)
{ return mpz_sgn(a); }
static inline int numMPZ_cmp(numMPZ_t a, numMPZ_t b)
{ return mpz_cmp(a,b); }
static inline int numMPZ_cmp_int(numMPZ_t a, long int b)
{ return mpz_cmp_si(a,b); }
static inline bool numMPZ_equal(numMPZ_t a, numMPZ_t b)
{ return mpz_cmp(a,b)==0; }
static inline bool numMPZ_integer(numMPZ_t a)
{ return true; }
static inline int numMPZ_hash(numMPZ_t a)
{ return mpz_get_si(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numMPZ_print(numMPZ_t a)
{ mpz_out_str(stdout,10,a); }
static inline void numMPZ_fprint(FILE* stream, numMPZ_t a)
{ mpz_out_str(stream,10,a); }
int numMPZ_snprint(char* s, size_t size, numMPZ_t a);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numMPZ_serialize_id(void)
{ return 0xf; }

size_t numMPZ_serialize(void* dst, numMPZ_t src);
size_t numMPZ_deserialize(numMPZ_t dst, const void* src);

/* not the exact size of serialized data, but a sound overapproximation */
static inline size_t numMPZ_serialized_size(numMPZ_t a)
{ return mpz_sizeinbase(a,2)/8+5+sizeof(mp_limb_t); }

size_t numMPZ_serialize_array(void* dst, numMPZ_t* src, size_t size);
size_t numMPZ_deserialize_array(numMPZ_t* dst, const void* src, size_t size);
size_t numMPZ_serialized_size_array(numMPZ_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* See num_conv.h.tmpl */

#ifdef __cplusplus
}
#endif

#endif
