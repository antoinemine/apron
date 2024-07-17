/* ********************************************************************** */
/* bound_def.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUND_DEF_H_
#define _BOUND_DEF_H_

#ifndef _BOUND_H_
#error "File bound_def.h should not be included directly, only via bound.h"
#endif

#include <stdio.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NUM_MAX) || defined(NUM_NUMRAT) || defined(NUM_NUMFLT)

typedef num_t bound_t;
#define bound_numref(a) a
#define _bound_inf(a)
#define BOUND_NUM

#else

typedef struct _bound_t {
  num_t num; /* always allocated, even if inf=1 */
  char inf;  /* 1 => +/-oo; the sign of num decides the sign of the oo
		0 => >-oo, <+oo */
} bound_t[1];
#define bound_numref(a) a->num
#define _bound_inf(a) a->inf = 0
#define BOUND_NUM_INF

#endif


/* ---------------------------------------------------------------------- */
static inline bool bound_infty(bound_t a)
#if defined(NUM_NUMFLT)
{ return numflt_infty(a); }
#elif defined(NUM_MAX)
{ return (*a>=NUM_MAX || *a<=-NUM_MAX); }
#elif defined(NUM_NUMRAT)
{ return numint_sgn(numrat_denref(a))==0; }
#else
{ return (bool)a->inf; }
#endif

/* ---------------------------------------------------------------------- */
static inline void bound_set_infty(bound_t a, int sgn)
#if defined(NUM_NUMFLT)
{ assert(sgn); numflt_set_infty(a,sgn); }
#elif defined(NUM_MAX)
{ assert(sgn); *a = sgn>0 ? NUM_MAX : -NUM_MAX; }
#elif defined(NUM_NUMRAT)
{
  assert(sgn);
  numint_set_int(numrat_numref(a),sgn>0 ? 1 : -1);
  numint_set_int(numrat_denref(a),0);
}
#else
{
  assert(sgn);
  num_set_int(a->num,sgn>0 ? 1 : -1);
  a->inf = 1;
}
#endif

/* ---------------------------------------------------------------------- */
static inline void bound_init_set_infty(bound_t a, int sgn)
{
  num_init(bound_numref(a));
  bound_set_infty(a,sgn);
}
static inline void bound_swap(bound_t a, bound_t b)
{
#if defined(BOUND_NUM_INF)
  int t = a->inf; a->inf = b->inf; b->inf = t;
#endif
  num_swap(bound_numref(a),bound_numref(b));
}

static inline int bound_sgn(bound_t a)
{ return num_sgn(bound_numref(a)); }

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

#if defined(BOUND_NUM)

static inline void bound_set(bound_t a, bound_t b)
{ num_set(a,b); }
static inline void bound_set_array(bound_t* a, bound_t* b, size_t size)
{ num_set_array(a,b,size); }

#elif defined(NUM_NATIVE)

static inline void bound_set(bound_t a, bound_t b)
{ *a = *b; }
static inline void bound_set_array(bound_t* a, bound_t* b, size_t size)
{ memcpy(a,b,size*sizeof(bound_t)); }

#else

static inline void bound_set(bound_t a, bound_t b)
{ num_set(a->num,b->num); a->inf = b->inf; }
static inline void bound_set_array(bound_t* a, bound_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) bound_set(a[i],b[i]);
}

#endif

static inline void bound_set_int(bound_t a, long int i)
{  num_set_int(bound_numref(a),i); _bound_inf(a); }

static inline void bound_set_num(bound_t a, num_t b)
{ num_set(bound_numref(a),b); _bound_inf(a); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void bound_init(bound_t a)
{ num_init(bound_numref(a)); _bound_inf(a); }
static inline void bound_init_set_int(bound_t a, long int i)
{ num_init_set_int(bound_numref(a),i); _bound_inf(a); }
static inline void bound_clear(bound_t a)
{ num_clear(bound_numref(a)); }

#if defined(BOUND_NUM)

static inline void bound_init_array(bound_t* a, size_t size)
{ num_init_array(a,size); }
static inline void bound_init_set(bound_t a, bound_t b)
{ num_init_set(a,b); }
static inline void bound_clear_array(bound_t* a, size_t size)
{ num_clear_array(a,size); }

#else

static inline void bound_init_array(bound_t* a, size_t size)
{
  size_t i;
  for (i=0;i<size;i++) bound_init(a[i]);
}
static inline void bound_init_set(bound_t a, bound_t b)
{
  if (bound_infty(b)){
    bound_init_set_infty(a,bound_sgn(b));
  } else {
    num_init_set(bound_numref(a),bound_numref(b));
    _bound_inf(a);
  }
}
static inline void bound_clear_array(bound_t* a, size_t size)
{
#if !defined(NUM_NATIVE)
  size_t i;
  for (i=0;i<size;i++) bound_clear(a[i]);
#endif
}

#endif

/* ====================================================================== */
/* Arithmetic Operations */
/* ====================================================================== */

/* +oo + -oo  \
   -oo + +oo  | undefined
   +oo - +oo  |
   -oo - -oo  /

   +oo + x = +oo - x = x - -oo = +oo
   -oo + x = -oo - x = x - +oo = -oo

   0 * +oo = +oo * 0 = 0 * -oo = -oo * 0 = 0
   x * +oo = +oo * x =  sign(x) * oo  if x!=0
   x * -oo = -oo * x = -sign(x) * oo  if x!=0

   0 / x = x / +oo = x / -oo = 0
   x / 0 = sign(x) * oo     if x!=0
   +oo / x =  sign(x) * oo  if x!=0,+oo,-oo
   -oo / x = -sign(x) * oo  if x!=0,+oo,-oo

*/

#if defined (NUM_MAX) && defined(NUM_NUMFLT)
static inline void bound_neg(bound_t a, bound_t b)
{ num_neg(a,b); }
static inline void bound_abs(bound_t a, bound_t b)
{ num_abs(a,b); }
static inline void bound_add(bound_t a, bound_t b, bound_t c)
{ num_add(a,b,c); }
static inline void bound_add_uint(bound_t a, bound_t b, unsigned long int c)
{ num_add_uint(a,b,c); }
static inline void bound_add_num(bound_t a, bound_t b, num_t c)
{ num_add(a,b,c); }
static inline void bound_sub(bound_t a, bound_t b, bound_t c)
{ num_sub(a,b,c); }
static inline void bound_sub_uint(bound_t a, bound_t b, unsigned long int c)
{ num_sub_uint(a,b,c); }
static inline void bound_sub_num(bound_t a, bound_t b, num_t c)
{ num_sub(a,b,c); }
static inline void bound_mul(bound_t a, bound_t b, bound_t c)
{ if (!bound_sgn(b) || !bound_sgn(c)) num_set_int(a,0); else num_mul(a,b,c); }
static inline void bound_mul_num(bound_t a, bound_t b, num_t c)
{ if (!bound_sgn(b) || !num_sgn(c)) num_set_int(a,0); else num_mul(a,b,c); }
static inline void bound_mul_2(bound_t a, bound_t b)
{ num_mul_2(a,b); }
static inline void bound_div(bound_t a, bound_t b, bound_t c)
{
  if (!bound_sgn(b) || bound_infty(c)) bound_set_int(a,0);
  else if (!bound_sgn(c)) bound_set_infty(a,bound_sgn(b));
  else num_div(a,b,c);
}
static inline void bound_div_num(bound_t a, bound_t b, num_t c)
{
  if (!bound_sgn(b)) bound_set_int(a,0);
  else if (!num_sgn(c)) bound_set_infty(a,bound_sgn(b));
  else num_div(a,b,c);
}
static inline void bound_div_2(bound_t a, bound_t b)
{ num_div_2(a,b); }

#else

static inline void bound_neg(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,-bound_sgn(b));
  else { num_neg(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
static inline void bound_abs(bound_t a, bound_t b)
{ num_abs(bound_numref(a),bound_numref(b)); }
static inline void bound_add(bound_t a, bound_t b, bound_t c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else if (bound_infty(c)) bound_set_infty(a,bound_sgn(c));
  else { num_add(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}
static inline void bound_add_uint(bound_t a, bound_t b, unsigned long int c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_add_uint(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}
static inline void bound_add_num(bound_t a, bound_t b, num_t c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_add(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}
static inline void bound_sub(bound_t a, bound_t b, bound_t c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else if (bound_infty(c)) bound_set_infty(a,-bound_sgn(c));
  else { num_sub(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}
static inline void bound_sub_uint(bound_t a, bound_t b, unsigned long int c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_sub_uint(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}
static inline void bound_sub_num(bound_t a, bound_t b, num_t c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_sub(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}

static inline void bound_mul(bound_t a, bound_t b, bound_t c)
{
  if (!bound_sgn(b) || !bound_sgn(c)) bound_set_int(a,0);
  else if (bound_infty(b) || bound_infty(c)) bound_set_infty(a,bound_sgn(b)*bound_sgn(c));
  else { num_mul(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}
static inline void bound_mul_num(bound_t a, bound_t b, num_t c)
{
  if (!bound_sgn(b) || !num_sgn(c)) bound_set_int(a,0);
  else if (bound_infty(b)) bound_set_infty(a,bound_sgn(b)*num_sgn(c));
  else { num_mul(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}
static inline void bound_mul_2(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_mul_2(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
static inline void bound_div(bound_t a, bound_t b, bound_t c)
{
  if (!bound_sgn(b) || bound_infty(c)) bound_set_int(a,0);
  else if (!bound_sgn(c)) bound_set_infty(a,bound_sgn(b));
  else if (bound_infty(b))  bound_set_infty(a,bound_sgn(b)*bound_sgn(c));
  else { num_div(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}
static inline void bound_div_num(bound_t a, bound_t b, num_t c)
{
  if (!bound_sgn(b)) bound_set_int(a,0);
  else if (!num_sgn(c)) bound_set_infty(a,bound_sgn(b));
  else if (bound_infty(b))  bound_set_infty(a,bound_sgn(b)*num_sgn(c));
  else { num_div(bound_numref(a),bound_numref(b),c); _bound_inf(a); }
}
static inline void bound_div_2(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_div_2(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
#endif


#if defined(NUM_MAX) || defined(NUM_NUMFLT)

static inline void bound_min(bound_t a, bound_t b, bound_t c)
{ num_min(a,b,c); }
static inline void bound_max(bound_t a, bound_t b, bound_t c)
{ num_max(a,b,c); }

#else

static inline void bound_min(bound_t a, bound_t b, bound_t c)
{
  if (bound_infty(b)) if (bound_sgn(b)>0) bound_set(a,c); else bound_set(a,b);
  else if (bound_infty(c)) if (bound_sgn(c)>0) bound_set(a,b); else bound_set(a,c);
  else { num_min(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}
static inline void bound_max(bound_t a, bound_t b, bound_t c)
{
  if (bound_infty(b)) if (bound_sgn(b)>0) bound_set(a,b); else bound_set(a,c);
  else if (bound_infty(c)) if (bound_sgn(c)>0) bound_set(a,c); else bound_set(a,b);
  else { num_max(bound_numref(a),bound_numref(b),bound_numref(c)); _bound_inf(a); }
}

#endif

static inline void bound_floor(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_floor(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
static inline void bound_ceil(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_ceil(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
static inline void bound_trunc(bound_t a, bound_t b)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_trunc(bound_numref(a),bound_numref(b)); _bound_inf(a); }
}
static inline void bound_sqrt(bound_t up, bound_t down, bound_t b)
{
  if (bound_infty(b)) {
    bound_set_infty(up,1);
    bound_set_infty(down,1);
  }
  else {
    num_sqrt(bound_numref(up),bound_numref(down),bound_numref(b));
    _bound_inf(up);
    _bound_inf(down);
  }
}
static inline void bound_pow(bound_t up, bound_t down, bound_t b, unsigned long n)
{
  if (bound_infty(b)) {
    bound_set_infty(up, 1);
    bound_set_infty(down, 1);
  }
  else if (num_pow(bound_numref(up), bound_numref(down), bound_numref(b), n)) {
    bound_set_infty(up,1);
    if (n & 1) bound_set_infty(down,-1);
    else bound_set_int(down, 0);
  }
  else {
    _bound_inf(up);
    _bound_inf(down);
  }
}
static inline void bound_root(bound_t up, bound_t down, bound_t b, unsigned long n)
{
  if (bound_infty(b)) {
    bound_set_infty(up, 1);
    bound_set_infty(down, 1);
  }
  else {
    num_root(bound_numref(up), bound_numref(down), bound_numref(b), n);
    _bound_inf(up);
    _bound_inf(down);
  }  
}
static inline void bound_to_float(bound_t a, bound_t b)
{
  if (bound_infty(b))
    bound_set_infty(a,bound_sgn(b));
  else {
    double d;
    double_set_num(&d,bound_numref(b));
    d = (float)d;
    if (d == 1./0.) {
      bound_set_infty(a,1);
    }
    else if (d == -1./0.) {
      num_set_double(bound_numref(a),-FLT_MAX);
      _bound_inf(a);
    }
    else {
      num_set_double(bound_numref(a),d);
      _bound_inf(a);
    }
  }
}
static inline void bound_to_double(bound_t a, bound_t b)
{
  if (bound_infty(b))
    bound_set_infty(a,bound_sgn(b));
  else {
    double d;
    double_set_num(&d,bound_numref(b));
    if (d == 1./0.) {
      bound_set_infty(a,1);
    }
    else if (d == -1./0.) {
      num_set_double(bound_numref(a),-DBL_MAX);
      _bound_inf(a);
    }
    else {
      num_set_double(bound_numref(a),d);
      _bound_inf(a);
    }
  }
}

static inline void bound_next_float(bound_t a, bound_t b)
{
  if (bound_infty(b)) {
    /* +oo overapproximates nextfloat(-oo) and nextfloat(+oo) */
    bound_set_infty(a,1);
  }
  else {
    double d;
    double_set_num(&d,bound_numref(b));
    d = nextafterf(d,d+1);
    if (d == 1./0.) {
      bound_set_infty(a,1);
    }
    else {
      num_set_double(bound_numref(a),d);
      _bound_inf(a);
    }
  }
}

static inline void bound_next_double(bound_t a, bound_t b)
{
  if (bound_infty(b)) {
    bound_set_infty(a,1);
  }
  else {
    double d;
    double_set_num(&d,bound_numref(b));
    d = nextafter(d,d+1);
    if (d == 1./0.) {
      bound_set_infty(a,1);
    }
    else {
      num_set_double(bound_numref(a),d);
      _bound_inf(a);
    }
  }
}

static inline void bound_mul_2exp(bound_t a, bound_t b, int c)
{
  if (bound_infty(b)) bound_set_infty(a,bound_sgn(b));
  else { num_mul_2exp(bound_numref(a),bound_numref(b),c); }
}


/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

#if defined(NUM_MAX) || defined(NUM_NUMFLT)

static inline int bound_cmp(bound_t a, bound_t b)
{ return num_cmp(a,b); }
static inline int bound_cmp_int(bound_t a, long int b)
{ return num_cmp_int(a,b); }
static inline int bound_cmp_num(bound_t a, num_t b)
{ return num_cmp(a,b); }
static inline bool bound_equal(bound_t a, bound_t b)
{ return num_equal(a,b); }
#else

static inline int bound_cmp(bound_t a, bound_t b)
{
  if (bound_infty(a)){
    if (bound_infty(b)) return (bound_sgn(a)-bound_sgn(b))/2;
    else return bound_sgn(a);
  } else {
    if (bound_infty(b)) return -bound_sgn(b);
    else return num_cmp(bound_numref(a),bound_numref(b));
  }
}
static inline int bound_cmp_int(bound_t a, long int b)
{
  if (bound_infty(a)) return bound_sgn(a);
  else return num_cmp_int(bound_numref(a),b);
}
static inline int bound_cmp_num(bound_t a, num_t b)
{
  if (bound_infty(a)) return bound_sgn(a);
  else return num_cmp(bound_numref(a),b);
}
static inline bool bound_equal(bound_t a, bound_t b)
{
  if (bound_infty(a)){
    return bound_infty(b) && bound_sgn(a)==bound_sgn(b);
  } else {
    if (bound_infty(b)) return false;
    else return num_equal(bound_numref(a),bound_numref(b));
  }
}
#endif

static inline int bound_hash(bound_t a)
{ if (bound_infty(a))
    return bound_sgn(a)>0 ? INT_MAX : INT_MIN;
  else {
    long int hash;
    int_set_num(&hash,bound_numref(a));
    return hash;
  }
}

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void bound_fprint(FILE* stream, bound_t a)
{
  if (bound_infty(a)) fprintf(stream,"%coo",bound_sgn(a)>0 ? '+' : '-');
  else num_fprint(stream,bound_numref(a));
}
static inline void bound_print(bound_t a)
{
  bound_fprint(stdout,a);
}
static inline int bound_snprint(char* s, size_t size, bound_t a)
{
  if (bound_infty(a)) return snprintf(s,size,"%coo",bound_sgn(a)>0 ? '+' : '-');
  else return num_snprint(s,size,bound_numref(a));
}


/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

/* Convert an ap_scalar_t into a bound_t */
static inline
bool bound_set_ap_scalar(bound_t a, ap_scalar_t* b)
{
  switch (b->discr){
  case AP_SCALAR_MPQ:
    if (mpz_sgn(mpq_denref(b->val.mpq))==0){
      bound_set_infty(a,mpz_sgn(mpq_numref(b->val.mpq)));
      return true;
    }
    else {
      _bound_inf(a);
      return num_set_mpq(bound_numref(a),b->val.mpq);
    }
    break;
  case AP_SCALAR_DOUBLE:
    if (isinf(b->val.dbl)) {
      if (b->val.dbl>0) bound_set_infty(a,1);
      else bound_set_infty(a,-1);
      return true;
    }
    else {
      _bound_inf(a);
      return num_set_double(bound_numref(a),b->val.dbl);
    }
    break;
  case AP_SCALAR_MPFR:
    if (mpfr_inf_p(b->val.mpfr)) {
      if (mpfr_sgn(b->val.mpfr)>0) bound_set_infty(a,1);
      else bound_set_infty(a,-1);
      return true;
    }
    else {
      _bound_inf(a);
      return num_set_mpfr(bound_numref(a),b->val.mpfr);
    }
    break;
  default:
    abort();
    return false;
  }
}
/* Convert a bound_t into an ap_scalar_t */
static inline bool ap_scalar_set_bound(ap_scalar_t* a, bound_t b)
{
  ap_scalar_reinit(a,NUM_AP_SCALAR);
  if (bound_infty(b)) { ap_scalar_set_infty(a,bound_sgn(b)); return true; }
  else {
    switch (NUM_AP_SCALAR) {
    case AP_SCALAR_DOUBLE: return double_set_num(&a->val.dbl,bound_numref(b));
    case AP_SCALAR_MPQ:    return mpq_set_num(a->val.mpq,bound_numref(b));
    case AP_SCALAR_MPFR:   return mpfr_set_num(a->val.mpfr,bound_numref(b));
    default:               abort(); return false;
    }
  }
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t bound_serialize(void* dst, bound_t src)
{
#if defined(BOUND_NUM)
  return num_serialize(dst,src);
#else
  *(char*)dst = src->inf;
  return num_serialize((char*)dst+1,bound_numref(src))+1;
#endif
}

static inline size_t bound_deserialize(bound_t dst, const void* src)
{
#if defined(BOUND_NUM)
  return num_deserialize(dst,src);
#else
  dst->inf = *(const char*)src;
  return num_deserialize(bound_numref(dst),(const char*)src+1)+1;
#endif
}

static inline size_t bound_serialized_size(bound_t a)
{
#if defined(BOUND_NUM)
  return num_serialized_size(a);
#else
  return num_serialized_size(bound_numref(a))+1;
#endif
}

static inline size_t bound_serialize_array(void* dst, bound_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += bound_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t bound_deserialize_array(bound_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += bound_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t bound_serialized_size_array(bound_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += bound_serialized_size(src[i]);
  return n;
}

static inline bool bound_integer(bound_t a)
{ return !bound_infty(a) && num_integer(bound_numref(a)); }

#ifdef __cplusplus
}
#endif

#endif
