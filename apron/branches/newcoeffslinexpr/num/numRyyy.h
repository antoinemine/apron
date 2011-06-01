/* ********************************************************************** */
/* numRyyy.h */
/* ********************************************************************** */

#ifndef _NUMRyyy_H_
#define _NUMRyyy_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "num_types.h"
#include "numIyyy.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

void numRyyy_canonicalize(numRyyy_t r);
static inline bool numRyyy_set_numintRyyy2(numRyyy_t a, numIyyy_t b, numIyyy_t c)
{
  numIyyy_set(numRyyy_numref(a),b);
  numIyyy_set(numRyyy_denref(a),c);
  numRyyy_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numRyyy_set(numRyyy_t a, numRyyy_t b)
{ *a = *b; }
static inline void numRyyy_set_array(numRyyy_t* a, numRyyy_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numRyyy_t)); }
static inline void numRyyy_set_int(numRyyy_t a, long int i)
{
  numIyyy_set_int(a->n,i);
  numIyyy_set_int(a->d,1);
}

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numRyyy_init(numRyyy_t a)
{
  numIyyy_init(a->n);
  numIyyy_init_set_int(a->d,1);
}
static inline void numRyyy_init_array(numRyyy_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numRyyy_init(a[i]);
}
static inline void numRyyy_init_set(numRyyy_t a, numRyyy_t b)
{ *a = *b; }
static inline void numRyyy_init_set_int(numRyyy_t a, long int i)
{ numIyyy_set_int(a->n,i); numIyyy_set_int(a->d,1); }

static inline void numRyyy_clear(numRyyy_t a)
{}
static inline void numRyyy_clear_array(numRyyy_t* a, size_t size)
{}
static inline void numRyyy_swap(numRyyy_t a, numRyyy_t b)
{ numRyyy_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numRyyy_neg(numRyyy_t a, numRyyy_t b)
{ numIyyy_neg(a->n,b->n); numIyyy_set(a->d,b->d); }
void numRyyy_inv(numRyyy_t a, numRyyy_t b);
static inline void numRyyy_abs(numRyyy_t a, numRyyy_t b)
{ numIyyy_abs(a->n,b->n); numIyyy_set(a->d,b->d); }
void numRyyy_add(numRyyy_t a, numRyyy_t b, numRyyy_t c);
static inline void numRyyy_add_uint(numRyyy_t a, numRyyy_t b, unsigned long int c)
{
  *a->n = *b->n + (numIyyy_native)c * (*b->d);
  *a->d = *b->d;
  numRyyy_canonicalize(a);
}
void numRyyy_sub(numRyyy_t a, numRyyy_t b, numRyyy_t c);
static inline void numRyyy_sub_uint(numRyyy_t a, numRyyy_t b, unsigned long int c)
{
  *a->n = *b->n - (numIyyy_native)c * (*b->d);
  *a->d = *b->d;
  numRyyy_canonicalize(a);
}
static inline void numRyyy_mul(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{
  *a->n = *b->n * *c->n;
  *a->d = *b->d * *c->d;
  numRyyy_canonicalize(a);
}
void numRyyy_div(numRyyy_t a, numRyyy_t b, numRyyy_t c);
static inline void numRyyy_floor(numRyyy_t a, numRyyy_t b)
{
  numIyyy_fdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
static inline void numRyyy_ceil(numRyyy_t a, numRyyy_t b)
{
  numIyyy_cdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
static inline void numRyyy_trunc(numRyyy_t a, numRyyy_t b)
{
  numIyyy_tdiv_q(a->n, b->n, b->d);
  numIyyy_set_int(a->d,1);
}
void numRyyy_sqrt(numRyyy_t up, numRyyy_t down, numRyyy_t b);
void numRyyy_mul_2exp(numRyyy_t a, numRyyy_t b, int c);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numRyyy_sgn(numRyyy_t a)
{ return numIyyy_sgn(a->n); }
int numRyyy_cmp(numRyyy_t a, numRyyy_t b);
static inline int numRyyy_cmp_int(numRyyy_t a, long int b)
{
  numIyyy_t aa,bb;
  *aa = *a->n;
  *bb = *a->d * b;
  return numIyyy_cmp(aa,bb);
}
static inline bool numRyyy_equal(numRyyy_t a, numRyyy_t b)
{ return numIyyy_equal(a->n,b->n) && numIyyy_equal(a->d,b->d); }

static inline void numRyyy_min(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{ numRyyy_set(a, numRyyy_cmp(b,c)<=0 ? b : c); }
static inline void numRyyy_max(numRyyy_t a, numRyyy_t b, numRyyy_t c)
{ numRyyy_set(a, numRyyy_cmp(b,c)>=0 ? b : c); }

static inline bool numRyyy_integer(numRyyy_t a)
{ return (*a->d==1); }
static inline int numRyyy_hash(numRyyy_t a)
{ return (int)((*a->n)/(*a->d)); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

void numRyyy_fprint(FILE* stream, numRyyy_t a);
static inline void numRyyy_print(numRyyy_t a)
{
  numRyyy_fprint(stdout,a);
}
int numRyyy_snprint(char* s, size_t size, numRyyy_t a);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numRyyy_serialize_id(void)
{ return 0x10 + sizeof(numIyyy_t)/4; }

static inline size_t numRyyy_serialize(void* dst, numRyyy_t src)
{
  size_t x = numIyyy_serialize(dst,numRyyy_numref(src));
  return x + numIyyy_serialize((char*)dst+x,numRyyy_denref(src));
}

static inline size_t numRyyy_deserialize(numRyyy_t dst, const void* src)
{
  size_t x = numIyyy_deserialize(numRyyy_numref(dst),src);
  return x + numIyyy_deserialize(numRyyy_denref(dst),(const char*)src+x);
}

static inline size_t numRyyy_serialized_size(numRyyy_t a)
{
  return numIyyy_serialized_size(numRyyy_numref(a)) +
	 numIyyy_serialized_size(numRyyy_denref(a));
}

size_t numRyyy_serialize_array(void* dst, numRyyy_t* src, size_t size);
size_t numRyyy_deserialize_array(numRyyy_t* dst, const void* src, size_t size);
size_t numRyyy_serialized_size_array(numRyyy_t* src, size_t size);

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* see num_conv.h.tmpl */

/* ********************************************************************** */
/* Underlying integer */
/* ********************************************************************** */

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numintRyyy_set(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_set(a,b); }
static inline void numintRyyy_set_array(numintRyyy_t* a, numintRyyy_t* b, size_t size)
	      { numIyyy_set_array(a,b,size); }
static inline void numintRyyy_set_int(numintRyyy_t a, long int i)
	      { numIyyy_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintRyyy_init(numintRyyy_t a)
	      { numIyyy_init(a); }
static inline void numintRyyy_init_array(numintRyyy_t* a, size_t size)
	      { numIyyy_init_array(a,size); }
static inline void numintRyyy_init_set(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_init_set(a,b); }
static inline void numintRyyy_init_set_int(numintRyyy_t a, long int i)
	      { numIyyy_init_set_int(a,i); }

static inline void numintRyyy_clear(numintRyyy_t a)
	      { numIyyy_clear(a); }
static inline void numintRyyy_clear_array(numintRyyy_t* a, size_t size)
	      { numIyyy_clear_array(a,size); }

static inline void numintRyyy_swap(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintRyyy_neg(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_neg(a,b); }
static inline void numintRyyy_abs(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_abs(a,b); }
static inline void numintRyyy_add(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_add(a,b,c); }
static inline void numintRyyy_add_uint(numintRyyy_t a, numintRyyy_t b, unsigned long int c)
	      { numIyyy_add_uint(a,b,c); }
static inline void numintRyyy_sub(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_sub(a,b,c); }
static inline void numintRyyy_sub_uint(numintRyyy_t a, numintRyyy_t b, unsigned long int c)
	      { numIyyy_sub_uint(a,b,c); }
static inline void numintRyyy_mul(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_mul(a,b,c); }
static inline void numintRyyy_div(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_cdiv_q(a,b,c); }
static inline void numintRyyy_min(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_min(a,b,c); }
static inline void numintRyyy_max(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_max(a,b,c); }
static inline void numintRyyy_mul_2exp(numintRyyy_t a, numintRyyy_t b, int c)
	      { numIyyy_mul_2exp(a,b,c); }

static inline void numintRyyy_floor(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_set(a,b); }
static inline void numintRyyy_ceil(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_set(a,b); }
static inline void numintRyyy_trunc(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_set(a,b); }
static inline void numintRyyy_sqrt(numintRyyy_t up, numintRyyy_t down, numintRyyy_t b)
	      { numIyyy_sqrt(up,down,b); }

static inline void numintRyyy_divexact(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_divexact(a,b,c); }
static inline void numintRyyy_mod(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_mod(a,b,c); }
static inline void numintRyyy_gcd(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_gcd(a,b,c); }
static inline void numintRyyy_lcm(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_lcm(a,b,c); }
static inline void numintRyyy_fdiv_q(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_fdiv_q(a,b,c); }
static inline void numintRyyy_cdiv_q(numintRyyy_t q, numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_cdiv_q(q,a,b); }
static inline void numintRyyy_tdiv_q(numintRyyy_t q, numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_tdiv_q(q,a,b); }
static inline void numintRyyy_cdiv_qr(numintRyyy_t q, numintRyyy_t r, numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_cdiv_qr(q,r,a,b); }
static inline void numintRyyy_cdiv_q_2exp(numintRyyy_t a, numintRyyy_t b, unsigned long int c)
	      { numIyyy_cdiv_q_2exp(a,b,c); }
static inline void numintRyyy_fdiv_q_2exp(numintRyyy_t a, numintRyyy_t b, unsigned long int c)
	      { numIyyy_fdiv_q_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintRyyy_sgn(numintRyyy_t a)
	 { return numIyyy_sgn(a); }
static inline int numintRyyy_cmp(numintRyyy_t a, numintRyyy_t b)
      { return numIyyy_cmp(a,b); }
static inline int numintRyyy_cmp_int(numintRyyy_t a, long int b)
      { return numIyyy_cmp_int(a,b); }
static inline bool numintRyyy_equal(numintRyyy_t a, numintRyyy_t b)
      { return numIyyy_equal(a,b); }
static inline bool numintRyyy_integer(numintRyyy_t a)
      { return true; }
static inline int numintRyyy_hash(numintRyyy_t a)
      { return numIyyy_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintRyyy_print(numintRyyy_t a)
	      { numIyyy_print(a); }
static inline void numintRyyy_fprint(FILE* stream, numintRyyy_t a)
	      { numIyyy_fprint(stream, a); }
static inline int numintRyyy_snprint(char* s, size_t size, numintRyyy_t a)
      { return numIyyy_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintRyyy_serialize_id(void)
{ return numIyyy_serialize_id(); }

static inline size_t numintRyyy_serialize(void* dst, numintRyyy_t src)
{ return numIyyy_serialize(dst,src); }

static inline size_t numintRyyy_deserialize(numintRyyy_t dst, const void* src)
{ return numIyyy_deserialize(dst,src); }

static inline size_t numintRyyy_serialized_size(numintRyyy_t src)
{ return numIyyy_serialized_size(src); }

static inline size_t numintRyyy_serialize_array(void* dst, numintRyyy_t* src, size_t size)
{ return numIyyy_serialize_array(dst,src,size); }

static inline size_t numintRyyy_deserialize_array(numintRyyy_t* dst, const void* src, size_t size)
{ return numIyyy_deserialize_array(dst,src,size); }

static inline size_t numintRyyy_serialized_size_array(numintRyyy_t* src, size_t size)
{ return numIyyy_serialized_size_array(src,size); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

MACRO_FOREACH XXX @AllNum
#define numintRyyy_fits_numXXX(a,b) numIyyy_fits_numXXX(a,b)
#define numXXX_fits_numintRyyy(a,b) numXXX_fits_numIyyy(a,b)
#define numXXX_set_numintRyyy(a,b,c) numXXX_set_numIyyy(a,b,c)
#define numintRyyy_set_numXXX(a,b,c) numIyyy_set_numXXX(a,b,c)
ENDMACRO

#define lint_fits_numintRyyy(a,b) lint_fits_numIyyy(a,b)
#define mpz_fits_numintRyyy(a,b) mpz_fits_numIyyy(a,b)
#define lfrac_fits_numintRyyy(a,b,c) lfrac_fits_numIyyy(a,b,c)
#define mpq_fits_numintRyyy(a,b) mpq_fits_numIyyy(a,b)
#define double_fits_numintRyyy(a,b) double_fits_numIyyy(a,b)
#define mpfr_fits_numintRyyy(a,b) mpfr_fits_numIyyy(a,b)

#define numintRyyy_set_lint(a,b,c) numIyyy_set_lint(a,b,c)
#define numintRyyy_set_mpz(a,b,c) numIyyy_set_mpz(a,b,c)
#define numintRyyy_set_lfrac(a,b,c,d) numIyyy_set_lfrac(a,b,c,d)
#define numintRyyy_set_mpq(a,b,c) numIyyy_set_mpq(a,b,c)
#define numintRyyy_set_double(a,b,c) numIyyy_set_double(a,b,c)
#define numintRyyy_set_mpfr(a,b,c) numIyyy_set_mpfr(a,b,c)

#ifdef __cplusplus
}
#endif

#endif
