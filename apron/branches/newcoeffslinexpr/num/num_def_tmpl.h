/* ********************************************************************** */
/* num_def.h */
/* ********************************************************************** */

#ifndef _NUM_DEF_H_
#define _NUM_DEF_H_

#include "numConfig.h"
#include "num.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* NUMINT */
/* ********************************************************************** */

#if NUM_NUMINT
#include "numint_def.h"

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
{ numint_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
{ numint_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
{ numint_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
{ numint_init(a); }
static inline void num_init_array(num_t* a, size_t size)
{ numint_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
{ numint_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
{ numint_init_set_int(a,i); }

static inline void num_clear(num_t a)
{ numint_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
{ numint_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
{ numint_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
{ numint_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
{ numint_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
{ numint_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
{ numint_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
{ numint_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
{ numint_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
{ numint_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
{ numint_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
{ numint_cdiv_q(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
{ numint_cdiv_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
{ numint_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
{ numint_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
{ numint_set(a,b); }
static inline void num_ceil(num_t a, num_t b)
{ numint_set(a,b); }
static inline void num_trunc(num_t a, num_t b)
{ numint_set(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
{ numint_sqrt(up,down,b); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
{ numint_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
{ return numint_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
{ return numint_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
{ return numint_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
{ return numint_equal(a,b); }
static inline bool num_integer(num_t a)
{ return true; }
static inline int num_hash(num_t a)
{ return numint_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
{ numint_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
{ numint_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
{ return numint_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numint_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numint_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numint_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numint_serialized_size(a); }

/* ********************************************************************** */
/* NUMRAT */
/* ********************************************************************** */

#elif NUM_NUMRAT
#include "numrat_def.h"


/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
{ numrat_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
{ numrat_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
{ numrat_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
{ numrat_init(a); }
static inline void num_init_array(num_t* a, size_t size)
{ numrat_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
{ numrat_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
{ numrat_init_set_int(a,i); }

static inline void num_clear(num_t a)
{ numrat_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
{ numrat_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
{ numrat_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
{ numrat_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
{ numrat_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
{ numrat_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
{ numrat_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
{ numrat_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
{ numrat_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
{ numrat_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
{ numrat_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
{ numrat_div(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
{ numrat_div_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
{ numrat_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
{ numrat_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
{ numrat_floor(a,b); }
static inline void num_ceil(num_t a, num_t b)
{ numrat_ceil(a,b); }
static inline void num_trunc(num_t a, num_t b)
{ numrat_trunc(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
{ numrat_sqrt(up,down,b); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
{ numrat_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
{ return numrat_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
{ return numrat_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
{ return numrat_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
{ return numrat_equal(a,b); }
static inline bool num_integer(num_t a)
{ return numrat_integer(a); }
static inline int num_hash(num_t a)
{ return numrat_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
{ numrat_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
{ numrat_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
{ return numrat_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numrat_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numrat_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numrat_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numrat_serialized_size(a); }

/* ********************************************************************** */
/* NUMINT */
/* ********************************************************************** */

#elif NUM_NUMFLT
#include "numflt_def.h"


/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

static inline void num_set(num_t a, num_t b)
{ numflt_set(a,b); }
static inline void num_set_array(num_t* a, num_t* b, size_t size)
{ numflt_set_array(a,b,size); }
static inline void num_set_int(num_t a, long int i)
{ numflt_set_int(a,i); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a)
{ numflt_init(a); }
static inline void num_init_array(num_t* a, size_t size)
{ numflt_init_array(a,size); }
static inline void num_init_set(num_t a, num_t b)
{ numflt_init_set(a,b); }
static inline void num_init_set_int(num_t a, long int i)
{ numflt_init_set_int(a,i); }

static inline void num_clear(num_t a)
{ numflt_clear(a); }
static inline void num_clear_array(num_t* a, size_t size)
{ numflt_clear_array(a,size); }

static inline void num_swap(num_t a, num_t b)
{ numflt_swap(a,b); }

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b)
{ numflt_neg(a,b); }
static inline void num_abs(num_t a, num_t b)
{ numflt_abs(a,b); }
static inline void num_add(num_t a, num_t b, num_t c)
{ numflt_add(a,b,c); }
static inline void num_add_uint(num_t a, num_t b, unsigned long int c)
{ numflt_add_uint(a,b,c); }
static inline void num_sub(num_t a, num_t b, num_t c)
{ numflt_sub(a,b,c); }
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c)
{ numflt_sub_uint(a,b,c); }
static inline void num_mul(num_t a, num_t b, num_t c)
{ numflt_mul(a,b,c); }
static inline void num_mul_2(num_t a, num_t b)
{ numflt_mul_2(a,b); }
static inline void num_div(num_t a, num_t b, num_t c)
{ numflt_div(a,b,c); }
static inline void num_div_2(num_t a, num_t b)
{ numflt_div_2(a,b); }
static inline void num_min(num_t a, num_t b, num_t c)
{ numflt_min(a,b,c); }
static inline void num_max(num_t a, num_t b, num_t c)
{ numflt_max(a,b,c); }
static inline void num_floor(num_t a, num_t b)
{ numflt_floor(a,b); }
static inline void num_ceil(num_t a, num_t b)
{ numflt_ceil(a,b); }
static inline void num_trunc(num_t a, num_t b)
{ numflt_trunc(a,b); }
static inline void num_sqrt(num_t up, num_t down, num_t b)
{ numflt_sqrt(up,down,b); }
static inline void num_mul_2exp(num_t a, num_t b, int c)
{ numflt_mul_2exp(a,b,c); }

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a)
{ return numflt_sgn(a); }
static inline int num_cmp(num_t a, num_t b)
{ return numflt_cmp(a,b); }
static inline int num_cmp_int(num_t a, long int b)
{ return numflt_cmp_int(a,b); }
static inline bool num_equal(num_t a, num_t b)
{ return numflt_equal(a,b); }
static inline bool num_integer(num_t a)
{ return numflt_integer(a); }
static inline int num_hash(num_t a)
{ return numflt_hash(a); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a)
{ numflt_print(a); }
static inline void num_fprint(FILE* stream, num_t a)
{ numflt_fprint(stream, a); }
static inline int num_snprint(char* s, size_t size, num_t a)
{ return numflt_snprint(s,size,a); }

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline unsigned char num_serialize_id(void)
{ return numflt_serialize_id(); }

static inline size_t num_serialize(void* dst, num_t src)
{ return numflt_serialize(dst,src); }

static inline size_t num_deserialize(num_t dst, const void* src)
{ return numflt_deserialize(dst,src); }

static inline size_t num_serialized_size(num_t a)
{ return numflt_serialized_size(a); }

static inline size_t num_serialize_array(void* dst, num_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += num_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t num_deserialize_array(num_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += num_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t num_serialized_size_array(num_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += num_serialized_size(src[i]);
  return n;
}

#else
#error "HERE"
#endif

#ifdef __cplusplus
}
#endif

#endif
