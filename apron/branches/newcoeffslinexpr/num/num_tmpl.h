/* ********************************************************************** */
/* num.h */
/* ********************************************************************** */

#ifndef _NUM_H_
#define _NUM_H_

/* Be aware: 
   we erase the (obsolete) GMP function mpq_set_num */
#undef mpq_set_num

#include "numConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NUM_NUMINT
#include "numint.h"
typedef numint_ptr num_ptr;
typedef numint_t num_t;
#elif NUM_NUMRAT
#include "numrat.h"
typedef numrat_ptr num_ptr;
typedef numrat_t num_t;
#elif NUM_NUMFLT
#include "numflt.h"
typedef numflt_ptr num_ptr;
typedef numflt_t num_t;
#else
#error "HERE"
#endif

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */
static inline void num_set(num_t a, num_t b);
static inline void num_set_array(num_t* a, num_t* b, size_t size);
static inline void num_set_int(num_t a, long int i);

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void num_init(num_t a);
static inline void num_init_array(num_t* a, size_t size);
static inline void num_init_set(num_t a, num_t b);
static inline void num_init_set_int(num_t a, long int i);

static inline void num_clear(num_t a);
static inline void num_clear_array(num_t* a, size_t size);

static inline void num_swap(num_t a, num_t b);

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

static inline void num_neg(num_t a, num_t b);
static inline void num_abs(num_t a, num_t b);
static inline void num_add(num_t a, num_t b, num_t c);
static inline void num_add_uint(num_t a, num_t b, unsigned long int c);
static inline void num_sub(num_t a, num_t b, num_t c);
static inline void num_sub_uint(num_t a, num_t b, unsigned long int c);
static inline void num_mul(num_t a, num_t b, num_t c);
static inline void num_mul_2(num_t a, num_t b);
static inline void num_div(num_t a, num_t b, num_t c);
static inline void num_div_2(num_t a, num_t b);
static inline void num_min(num_t a, num_t b, num_t c);
static inline void num_max(num_t a, num_t b, num_t c);

static inline void num_mul_2exp(num_t a, num_t b, int c);
  /* multiplies b by 2^c (c can be positive or negative) */

static inline void num_floor(num_t a, num_t b);
static inline void num_ceil(num_t a, num_t b);
static inline void num_trunc(num_t a, num_t b);
  /* Approximate to the nearest integer toward resp. -infty, +infty */

static inline void num_sqrt(num_t up, num_t down, num_t b);
  /* Compute both an upper bound and a lower bound */

/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

static inline int num_sgn(num_t a);
static inline int num_cmp(num_t a, num_t b);
static inline int num_cmp_int(num_t a, long int b);
static inline bool num_equal(num_t a, num_t b);
static inline bool num_integer(num_t a);
static inline int num_hash(num_t a);

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void num_print(num_t a);
static inline void num_fprint(FILE* stream, num_t a);
static inline int num_snprint(char* s, size_t size, num_t a);

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

/* Notes:
   - call _serialized_size to get the minimal size of the buffer to allocate
   - _serialize then returns the actual size of the serialized data (may be 
   less than _serialized_size)
   - deserialze assumes that the num destination has already been initialized
   - when deserializing an array, you must supply the number of elements in the
   array (it is not stored with the array by serialize)
   - serialization is not type-safe (it depends on the chosen numerical type
   and, for types using native integers, on whether the machine is 32 or 64 
   bits). You can only deserialize data if the num_serialize_id function 
   returns the same value as when the data was serialized. (This id is not
   stored systematically with each serialized number to save state, so, the
   checking is up to you.)
*/

static inline unsigned char num_serialize_id(void);

static inline size_t num_serialize(void* dst, num_t src);
static inline size_t num_deserialize(num_t dst, const void* src);
static inline size_t num_serialized_size(num_t a);

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

#ifdef __cplusplus
}
#endif

#endif
