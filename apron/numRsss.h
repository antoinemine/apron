/* ********************************************************************** */
/* numRsss.h */
/* ********************************************************************** */

#ifndef _NUMRsss_H_
#define _NUMRsss_H_

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "num_types.h"
#include "numIsss.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Rational operations */
/* ====================================================================== */

void numRsss_canonicalize(numRsss_t r);
static inline bool numRsss_set_numintRsss2(numRsss_t a, numIsss_t b, numIsss_t c)
{
  numIsss_set(numRsss_numref(a),b);
  numIsss_set(numRsss_denref(a),c);
  numRsss_canonicalize(a);
  return true;
}

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void numRsss_set(numRsss_t a, numRsss_t b)
{ *a = *b; }
static inline void numRsss_set_array(numRsss_t* a, numRsss_t* b, size_t size)
{ memcpy(a,b,size*sizeof(numRsss_t)); }
static inline void numRsss_set_int(numRsss_t a, long int i)
{
  numIsss_set_int(a->n,i);
  numIsss_set_int(a->d,1);
}

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numRsss_init(numRsss_t a)
{
  numIsss_init(a->n);
  numIsss_init_set_int(a->d,1);
}
static inline void numRsss_init_array(numRsss_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) numRsss_init(a[i]);
}
static inline void numRsss_init_set(numRsss_t a, numRsss_t b)
{ *a = *b; }
static inline void numRsss_init_set_int(numRsss_t a, long int i)
{ numIsss_set_int(a->n,i); numIsss_set_int(a->d,1); }

static inline void numRsss_clear(numRsss_t a)
{}
static inline void numRsss_clear_array(numRsss_t* a, size_t size)
{}
static inline void numRsss_swap(numRsss_t a, numRsss_t b)
{ numRsss_t t; *t=*a;*a=*b;*b=*t; }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numRsss_neg(numRsss_t a, numRsss_t b)
{ numIsss_neg(a->n,b->n); numIsss_set(a->d,b->d); }
void numRsss_inv(numRsss_t a, numRsss_t b);
static inline void numRsss_abs(numRsss_t a, numRsss_t b)
{ numIsss_abs(a->n,b->n); numIsss_set(a->d,b->d); }
void numRsss_add(numRsss_t a, numRsss_t b, numRsss_t c);
static inline void numRsss_add_uint(numRsss_t a, numRsss_t b, unsigned long int c)
{
  *a->n = *b->n + (numIsss_native)c * (*b->d);
  *a->d = *b->d;
  numRsss_canonicalize(a);
}
void numRsss_sub(numRsss_t a, numRsss_t b, numRsss_t c);
static inline void numRsss_sub_uint(numRsss_t a, numRsss_t b, unsigned long int c)
{
  *a->n = *b->n - (numIsss_native)c * (*b->d);
  *a->d = *b->d;
  numRsss_canonicalize(a);
}
static inline void numRsss_mul(numRsss_t a, numRsss_t b, numRsss_t c)
{
  *a->n = *b->n * *c->n;
  *a->d = *b->d * *c->d;
  numRsss_canonicalize(a);
}
void numRsss_div(numRsss_t a, numRsss_t b, numRsss_t c);
static inline void numRsss_floor(numRsss_t a, numRsss_t b)
{
  numIsss_fdiv_q(a->n, b->n, b->d);
  numIsss_set_int(a->d,1);
}
static inline void numRsss_ceil(numRsss_t a, numRsss_t b)
{
  numIsss_cdiv_q(a->n, b->n, b->d);
  numIsss_set_int(a->d,1);
}
static inline void numRsss_trunc(numRsss_t a, numRsss_t b)
{
  numIsss_tdiv_q(a->n, b->n, b->d);
  numIsss_set_int(a->d,1);
}
void numRsss_sqrt(numRsss_t up, numRsss_t down, numRsss_t b);
void numRsss_mul_2exp(numRsss_t a, numRsss_t b, int c);

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numRsss_sgn(numRsss_t a)
{ return numIsss_sgn(a->n); }
int numRsss_cmp(numRsss_t a, numRsss_t b);
static inline int numRsss_cmp_int(numRsss_t a, long int b)
{
  numIsss_t aa,bb;
  *aa = *a->n;
  *bb = *a->d * b;
  return numIsss_cmp(aa,bb);
}
static inline bool numRsss_equal(numRsss_t a, numRsss_t b)
{ return numIsss_equal(a->n,b->n) && numIsss_equal(a->d,b->d); }

static inline void numRsss_min(numRsss_t a, numRsss_t b, numRsss_t c)
{ numRsss_set(a, numRsss_cmp(b,c)<=0 ? b : c); }
static inline void numRsss_max(numRsss_t a, numRsss_t b, numRsss_t c)
{ numRsss_set(a, numRsss_cmp(b,c)>=0 ? b : c); }

static inline bool numRsss_integer(numRsss_t a)
{ return (*a->d==1); }
static inline int numRsss_hash(numRsss_t a)
{ return (int)((*a->n)/(*a->d)); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

void numRsss_fprint(FILE* stream, numRsss_t a);
static inline void numRsss_print(numRsss_t a)
{
  numRsss_fprint(stdout,a);
}
int numRsss_snprint(char* s, int size, numRsss_t a);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numRsss_serialize_id(void)
{ return 0x10 + sizeof(numIsss_t)/4; }

static inline size_t numRsss_serialize(void* dst, numRsss_t src)
{
  size_t x = numIsss_serialize(dst,numRsss_numref(src));
  return x + numIsss_serialize((char*)dst+x,numRsss_denref(src));
}

static inline size_t numRsss_deserialize(numRsss_t dst, const void* src)
{
  size_t x = numIsss_deserialize(numRsss_numref(dst),src);
  return x + numIsss_deserialize(numRsss_denref(dst),(const char*)src+x);
}

static inline size_t numRsss_serialized_size(numRsss_t a)
{
  return numIsss_serialized_size(numRsss_numref(a)) +
	 numIsss_serialized_size(numRsss_denref(a));
}

size_t numRsss_serialize_array(void* dst, numRsss_t* src, size_t size);
size_t numRsss_deserialize_array(numRsss_t* dst, const void* src, size_t size);
size_t numRsss_serialized_size_array(numRsss_t* src, size_t size);

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

static inline void numintRsss_set(numintRsss_t a, numintRsss_t b)
	      { numIsss_set(a,b); }
static inline void numintRsss_set_array(numintRsss_t* a, numintRsss_t* b, size_t size)
	      { numIsss_set_array(a,b,size); }
static inline void numintRsss_set_int(numintRsss_t a, long int i)
	      { numIsss_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void numintRsss_init(numintRsss_t a)
	      { numIsss_init(a); }
static inline void numintRsss_init_array(numintRsss_t* a, size_t size)
	      { numIsss_init_array(a,size); }
static inline void numintRsss_init_set(numintRsss_t a, numintRsss_t b)
	      { numIsss_init_set(a,b); }
static inline void numintRsss_init_set_int(numintRsss_t a, long int i)
	      { numIsss_init_set_int(a,i); }

static inline void numintRsss_clear(numintRsss_t a)
	      { numIsss_clear(a); }
static inline void numintRsss_clear_array(numintRsss_t* a, size_t size)
	      { numIsss_clear_array(a,size); }

static inline void numintRsss_swap(numintRsss_t a, numintRsss_t b)
	      { numIsss_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void numintRsss_neg(numintRsss_t a, numintRsss_t b)
	      { numIsss_neg(a,b); }
static inline void numintRsss_abs(numintRsss_t a, numintRsss_t b)
	      { numIsss_abs(a,b); }
static inline void numintRsss_add(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_add(a,b,c); }
static inline void numintRsss_add_uint(numintRsss_t a, numintRsss_t b, unsigned long int c)
	      { numIsss_add_uint(a,b,c); }
static inline void numintRsss_sub(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_sub(a,b,c); }
static inline void numintRsss_sub_uint(numintRsss_t a, numintRsss_t b, unsigned long int c)
	      { numIsss_sub_uint(a,b,c); }
static inline void numintRsss_mul(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_mul(a,b,c); }
static inline void numintRsss_div(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_cdiv_q(a,b,c); }
static inline void numintRsss_min(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_min(a,b,c); }
static inline void numintRsss_max(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_max(a,b,c); }
static inline void numintRsss_mul_2exp(numintRsss_t a, numintRsss_t b, int c)
	      { numIsss_mul_2exp(a,b,c); }

static inline void numintRsss_floor(numintRsss_t a, numintRsss_t b)
	      { numIsss_set(a,b); }
static inline void numintRsss_ceil(numintRsss_t a, numintRsss_t b)
	      { numIsss_set(a,b); }
static inline void numintRsss_trunc(numintRsss_t a, numintRsss_t b)
	      { numIsss_set(a,b); }
static inline void numintRsss_sqrt(numintRsss_t up, numintRsss_t down, numintRsss_t b)
	      { numIsss_sqrt(up,down,b); }

static inline void numintRsss_divexact(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_divexact(a,b,c); }
static inline void numintRsss_mod(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_mod(a,b,c); }
static inline void numintRsss_gcd(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_gcd(a,b,c); }
static inline void numintRsss_lcm(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_lcm(a,b,c); }
static inline void numintRsss_fdiv_q(numintRsss_t a, numintRsss_t b, numintRsss_t c)
	      { numIsss_fdiv_q(a,b,c); }
static inline void numintRsss_cdiv_q(numintRsss_t q, numintRsss_t a, numintRsss_t b)
	      { numIsss_cdiv_q(q,a,b); }
static inline void numintRsss_tdiv_q(numintRsss_t q, numintRsss_t a, numintRsss_t b)
	      { numIsss_tdiv_q(q,a,b); }
static inline void numintRsss_cdiv_qr(numintRsss_t q, numintRsss_t r, numintRsss_t a, numintRsss_t b)
	      { numIsss_cdiv_qr(q,r,a,b); }
static inline void numintRsss_cdiv_q_2exp(numintRsss_t a, numintRsss_t b, unsigned long int c)
	      { numIsss_cdiv_q_2exp(a,b,c); }
static inline void numintRsss_fdiv_q_2exp(numintRsss_t a, numintRsss_t b, unsigned long int c)
	      { numIsss_fdiv_q_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int numintRsss_sgn(numintRsss_t a)
	 { return numIsss_sgn(a); }
static inline int numintRsss_cmp(numintRsss_t a, numintRsss_t b)
      { return numIsss_cmp(a,b); }
static inline int numintRsss_cmp_int(numintRsss_t a, long int b)
      { return numIsss_cmp_int(a,b); }
static inline bool numintRsss_equal(numintRsss_t a, numintRsss_t b)
      { return numIsss_equal(a,b); }
static inline bool numintRsss_integer(numintRsss_t a)
      { return true; }
static inline int numintRsss_hash(numintRsss_t a)
      { return numIsss_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void numintRsss_print(numintRsss_t a)
	      { numIsss_print(a); }
static inline void numintRsss_fprint(FILE* stream, numintRsss_t a)
	      { numIsss_fprint(stream, a); }
static inline int numintRsss_snprint(char* s, size_t size, numintRsss_t a)
      { return numIsss_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char numintRsss_serialize_id(void)
{ return numIsss_serialize_id(); }

static inline size_t numintRsss_serialize(void* dst, numintRsss_t src)
{ return numIsss_serialize(dst,src); }

static inline size_t numintRsss_deserialize(numintRsss_t dst, const void* src)
{ return numIsss_deserialize(dst,src); }

static inline size_t numintRsss_serialized_size(numintRsss_t src)
{ return numIsss_serialized_size(src); }

static inline size_t numintRsss_serialize_array(void* dst, numintRsss_t* src, size_t size)
{ return numIsss_serialize_array(dst,src,size); }

static inline size_t numintRsss_deserialize_array(numintRsss_t* dst, const void* src, size_t size)
{ return numIsss_deserialize_array(dst,src,size); }

static inline size_t numintRsss_serialized_size_array(numintRsss_t* src, size_t size)
{ return numIsss_serialized_size_array(src,size); }

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

MACRO_FOREACH XXX @AllNum
#define numintRsss_fits_numXXX(a,b) numIsss_fits_numXXX(a,b)
#define numXXX_fits_numintRsss(a,b) numXXX_fits_numIsss(a,b)
#define numXXX_set_numintRsss(a,b,c) numXXX_set_numIsss(a,b,c)
#define numintRsss_set_numXXX(a,b,c) numIsss_set_numXXX(a,b,c)
ENDMACRO

#define lint_fits_numintRsss(a,b) lint_fits_numIsss(a,b)
#define mpz_fits_numintRsss(a,b) mpz_fits_numIsss(a,b)
#define lfrac_fits_numintRsss(a,b,c) lfrac_fits_numIsss(a,b,c)
#define mpq_fits_numintRsss(a,b) mpq_fits_numIsss(a,b)
#define double_fits_numintRsss(a,b) double_fits_numIsss(a,b)
#define mpfr_fits_numintRsss(a,b) mpfr_fits_numIsss(a,b)

#define numintRsss_set_lint(a,b,c) numIsss_set_lint(a,b,c)
#define numintRsss_set_mpz(a,b,c) numIsss_set_mpz(a,b,c)
#define numintRsss_set_lfrac(a,b,c,d) numIsss_set_lfrac(a,b,c,d)
#define numintRsss_set_mpq(a,b,c) numIsss_set_mpq(a,b,c)
#define numintRsss_set_double(a,b,c) numIsss_set_double(a,b,c)
#define numintRsss_set_mpfr(a,b,c) numIsss_set_mpfr(a,b,c)

#ifdef __cplusplus
}
#endif

#endif
