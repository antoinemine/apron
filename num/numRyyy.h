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
static inline void numRyyy_mul_2(numRyyy_t a, numRyyy_t b)
{
  if (*b->d % 2 == 0){
    *a->n = *b->n;
    *a->d = *b->d / 2;
  }
  else {
    *a->n = *b->n * 2;
    *a->d = *b->d;
  }
}
void numRyyy_div(numRyyy_t a, numRyyy_t b, numRyyy_t c);
static inline void numRyyy_div_2(numRyyy_t a, numRyyy_t b)
{
  if (*b->n % 2 == 0){
    *a->n = *b->n / 2;
    *a->d = *b->d;
  }
  else {
    *a->n = *b->n;
    *a->d = *b->d * 2;
  }
}
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

bool lint_fits_numRyyy(long int a);
bool llint_fits_numRyyy(long long int a);
bool mpz_fits_numRyyy(mpz_t a);
bool lfrac_fits_numRyyy(long int i, long int j);
bool llfrac_fits_numRyyy(long long int i, long long int j);
bool mpq_fits_numRyyy(mpq_t a);
bool double_fits_numRyyy(double k);
bool ldouble_fits_numRyyy(long double k);
bool mpfr_fits_numRyyy(mpfr_t a, num_internal_t intern);
bool numD_fits_numRyyy(numD_t a);
bool numMPQ_fits_numRyyy(numMPQ_t a);
bool numMPFR_fits_numRyyy(numMPFR_t a, num_internal_t intern);

bool numRyyy_fits_lint(numRyyy_t a);
bool numRyyy_fits_llint(numRyyy_t a);
bool numRyyy_fits_mpz(numRyyy_t a);
bool numRyyy_fits_lfrac(numRyyy_t a);
bool numRyyy_fits_llfrac(numRyyy_t a);
bool numRyyy_fits_mpq(numRyyy_t a);
bool numRyyy_fits_float(numRyyy_t a);
bool numRyyy_fits_double(numRyyy_t a);
bool numRyyy_fits_ldouble(numRyyy_t a);
bool numRyyy_fits_mpfr(numRyyy_t a);
bool numRyyy_fits_numD(numRyyy_t a);
bool numRyyy_fits_numMPQ(numRyyy_t a);
bool numRyyy_fits_numMPFR(numRyyy_t a);

bool numRyyy_set_lint(numRyyy_t a, long int i, num_internal_t intern);
bool numRyyy_set_llint(numRyyy_t a, long long int i, num_internal_t intern);
bool numRyyy_set_mpz(numRyyy_t a, mpz_t b, num_internal_t intern);
bool numRyyy_set_lfrac(numRyyy_t a, long int i, long int j, num_internal_t intern);
bool numRyyy_set_llfrac(numRyyy_t a, long long int i, long long int j, num_internal_t intern);
bool numRyyy_set_mpq(numRyyy_t a, mpq_t b, num_internal_t intern);
bool numRyyy_set_double(numRyyy_t a, double k, num_internal_t intern);
bool numRyyy_set_ldouble(numRyyy_t a, long double k, num_internal_t intern);
bool numRyyy_set_mpfr(numRyyy_t a, mpfr_t b, num_internal_t intern);
bool numRyyy_set_numD(numRyyy_t a, numD_t b, num_internal_t intern);
bool numRyyy_set_numMPQ(numRyyy_t a, numMPQ_t b, num_internal_t intern);
bool numRyyy_set_numMPFR(numRyyy_t a, numMPFR_t b, num_internal_t intern);

bool lint_set_numRyyy(long int* a, numRyyy_t b, num_internal_t intern);
bool llint_set_numRyyy(long long int* a, numRyyy_t b, num_internal_t intern);
bool mpz_set_numRyyy(mpz_t a, numRyyy_t b, num_internal_t intern);
bool lfrac_set_numRyyy(long int* i, long int* j, numRyyy_t b, num_internal_t intern);
bool llfrac_set_numRyyy(long long int* i, long long int* j, numRyyy_t b, num_internal_t intern);
bool mpq_set_numRyyy(mpq_t a, numRyyy_t b, num_internal_t intern);
bool double_set_numRyyy(double* a, numRyyy_t b, num_internal_t intern);
bool ldouble_set_numRyyy(long double* a, numRyyy_t b, num_internal_t intern);
bool mpfr_set_numRyyy(mpfr_t a, numRyyy_t b, num_internal_t intern);
bool numD_set_numRyyy(numD_t a, numRyyy_t b, num_internal_t intern);
bool numMPQ_set_numRyyy(numMPQ_t a, numRyyy_t b, num_internal_t intern);
bool numMPFR_set_numRyyy(numMPFR_t a, numRyyy_t b, num_internal_t intern);

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
static inline void numintRyyy_mul_2(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_mul_2(a,b); }
static inline void numintRyyy_div(numintRyyy_t a, numintRyyy_t b, numintRyyy_t c)
	      { numIyyy_cdiv_q(a,b,c); }
static inline void numintRyyy_div_2(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_cdiv_2(a,b); }
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
static inline void numintRyyy_cdiv_2(numintRyyy_t a, numintRyyy_t b)
	      { numIyyy_cdiv_2(a,b); }
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

static inline bool lint_fits_numintRyyy(long int a)
{ return lint_fits_numIyyy(a); }
static inline bool llint_fits_numintRyyy(long long int a)
{ return llint_fits_numIyyy(a); }
static inline bool mpz_fits_numintRyyy(mpz_t a)
{ return mpz_fits_numIyyy(a); }
static inline bool lfrac_fits_numintRyyy(long int i, long int j)
{ return lfrac_fits_numIyyy(i,j); }
static inline bool llfrac_fits_numintRyyy(long long int i, long long int j)
{ return llfrac_fits_numIyyy(i,j); }
static inline bool mpq_fits_numintRyyy(mpq_t a)
{ return mpq_fits_numIyyy(a); }
static inline bool double_fits_numintRyyy(double a)
{ return double_fits_numIyyy(a); }
static inline bool ldouble_fits_numintRyyy(long double a)
{ return ldouble_fits_numIyyy(a); }
static inline bool mpfr_fits_numintRyyy(mpfr_t a, num_internal_t intern)
{ return mpfr_fits_numIyyy(a,intern); }

static inline bool numintRyyy_fits_lint(numintRyyy_t a)
{ return numIyyy_fits_lint(a); }
static inline bool numintRyyy_fits_llint(numintRyyy_t a)
{ return numIyyy_fits_llint(a); }
static inline bool numintRyyy_fits_mpz(numintRyyy_t a)
{ return numIyyy_fits_mpz(a); }
static inline bool numintRyyy_fits_lfrac(numintRyyy_t a)
{ return numIyyy_fits_lfrac(a); }
static inline bool numintRyyy_fits_llfrac(numintRyyy_t a)
{ return numIyyy_fits_llfrac(a); }
static inline bool numintRyyy_fits_mpq(numintRyyy_t a)
{ return numIyyy_fits_mpq(a); }
static inline bool numintRyyy_fits_double(numintRyyy_t a)
{ return numIyyy_fits_double(a); }
static inline bool numintRyyy_fits_ldouble(numintRyyy_t a)
{ return numIyyy_fits_ldouble(a); }
static inline bool numintRyyy_fits_mpfr(numintRyyy_t a)
{ return numIyyy_fits_mpfr(a); }

static inline bool numintRyyy_set_lint(numintRyyy_t a, long int b, num_internal_t intern)
{ return numIyyy_set_lint(a,b,intern); }
static inline bool numintRyyy_set_llint(numintRyyy_t a, long long int b, num_internal_t intern)
{ return numIyyy_set_llint(a,b,intern); }
static inline bool numintRyyy_set_mpz(numintRyyy_t a, mpz_t b, num_internal_t intern)
{ return numIyyy_set_mpz(a,b,intern); }
static inline bool numintRyyy_set_lfrac(numintRyyy_t a, long int i, long int j, num_internal_t intern)
{ return numIyyy_set_lfrac(a,i,j,intern); }
static inline bool numintRyyy_set_llfrac(numintRyyy_t a, long long int i, long long int j, num_internal_t intern)
{ return numIyyy_set_llfrac(a,i,j,intern); }
static inline bool numintRyyy_set_mpq(numintRyyy_t a, mpq_t b, num_internal_t intern)
{ return numIyyy_set_mpq(a,b,intern); }
static inline bool numintRyyy_set_double(numintRyyy_t a, double b, num_internal_t intern)
{ return numIyyy_set_double(a,b,intern); }
static inline bool numintRyyy_set_ldouble(numintRyyy_t a, long double b, num_internal_t intern)
{ return numIyyy_set_ldouble(a,b,intern); }
static inline bool numintRyyy_set_mpfr(numintRyyy_t a, mpfr_t b, num_internal_t intern)
{ return numIyyy_set_mpfr(a,b,intern); }

static inline bool lint_set_numintRyyy(long int* a, numintRyyy_t b, num_internal_t intern)
{ return lint_set_numIyyy(a,b,intern); }
static inline bool llint_set_numintRyyy(long long int* a, numintRyyy_t b, num_internal_t intern)
{ return llint_set_numIyyy(a,b,intern); }
static inline bool mpz_set_numintRyyy(mpz_t a, numintRyyy_t b, num_internal_t intern)
{ return mpz_set_numIyyy(a,b,intern); }
static inline bool lfrac_set_numintRyyy(long int* i, long int* j, numintRyyy_t b, num_internal_t intern)
{ return lfrac_set_numIyyy(i,j,b,intern); }
static inline bool llfrac_set_numintRyyy(long long int* i, long long int* j, numintRyyy_t b, num_internal_t intern)
{ return llfrac_set_numIyyy(i,j,b,intern); }
static inline bool mpq_set_numintRyyy(mpq_t a, numintRyyy_t b, num_internal_t intern)
{ return mpq_set_numIyyy(a,b,intern); }
static inline bool double_set_numintRyyy(double* a, numintRyyy_t b, num_internal_t intern)
{ return double_set_numIyyy(a,b,intern); }
static inline bool ldouble_set_numintRyyy(long double* a, numintRyyy_t b, num_internal_t intern)
{ return ldouble_set_numIyyy(a,b,intern); }
static inline bool mpfr_set_numintRyyy(mpfr_t a, numintRyyy_t b, num_internal_t intern)
{ return mpfr_set_numIyyy(a,b,intern); }

#ifdef __cplusplus
}
#endif

#endif
