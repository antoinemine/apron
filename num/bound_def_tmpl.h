/* ********************************************************************** */
/* bound_def.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUND_DEF_H_
#define _BOUND_DEF_H_

#include "numConfig.h"
#include "bound.h"
#include "num_def.h"
#include "num_conv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------- */
static inline bool bound_infty(bound_t a)
#if NUM_NUMFLT
{ return numflt_infty(a); }
#elif NUM_NUMRAT
{ return numint_sgn(numrat_denref(a))==0; }
#elif NUM_NUMMPZ
{ return (bool)a->inf; }
#else
{ return (*a>=NUM_MAX || *a<=-NUM_MAX); }
#endif

/* ---------------------------------------------------------------------- */
static inline void bound_set_infty(bound_t a, int sgn)
#if NUM_NUMFLT
{ assert(sgn); numflt_set_infty(a,sgn); }
#elif NUM_NUMRAT
{
  assert(sgn);
  numint_set_int(numrat_numref(a),sgn>0 ? 1 : -1);
  numint_set_int(numrat_denref(a),0);
}
#elif NUM_NUMMPZ
{
  assert(sgn);
  num_set_int(a->num,sgn>0 ? 1 : -1);
  a->inf = 1;
}
#else
{ assert(sgn); *a = sgn>0 ? NUM_MAX : -NUM_MAX; }
#endif

/* ---------------------------------------------------------------------- */
static inline void bound_init_set_infty(bound_t a, int sgn)
{
  num_init(bound_numref(a));
  bound_set_infty(a,sgn);
}
static inline void bound_swap(bound_t a, bound_t b)
{
#ifdef BOUND_INF
  int t = a->inf; a->inf = b->inf; b->inf = t;
#endif
  num_swap(bound_numref(a),bound_numref(b));
}

static inline int bound_sgn(bound_t a)
{ return num_sgn(bound_numref(a)); }

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

#ifndef BOUND_INF

static inline void bound_set(bound_t a, bound_t b)
{ num_set(a,b); }
static inline void bound_set_array(bound_t* a, bound_t* b, size_t size)
{ num_set_array(a,b,size); }

#elif NUM_NATIVE

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

#ifndef BOUND_INF

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
#if !NUM_NATIVE
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

#if NUM_NUMFLT
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


#if NUM_NUMFLT || (NUM_NUMINT && NUM_NATIVE)

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
static inline void bound_to_float(bound_t a, bound_t b, numinternal_t intern)
{
  if (bound_infty(b) || !num_fits_float(bound_numref(b)))
    bound_set_infty(a,bound_sgn(b));
  else {
    double d;
    double_set_num(&d,bound_numref(b),intern);
    num_set_double(bound_numref(a),(double)((float)d),intern);
    _bound_inf(a);
  }
}
static inline void bound_to_double(bound_t a, bound_t b, numinternal_t intern)
{
  if (bound_infty(b) || !num_fits_double(bound_numref(b)))
    bound_set_infty(a,bound_sgn(b));
  else {
    double d;
    double_set_num(&d,bound_numref(b),intern);
    num_set_double(bound_numref(a),d,intern);
    _bound_inf(a);
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

#if NUM_NUMFLT || (NUM_NUMINT && NUM_NATIVE)

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
    return bound_sgn(a)>0 ? INT_MAX : -INT_MAX;
  else {
    return num_hash(bound_numref(a));
  }
}

static inline void bound_widening(bound_t a, bound_t b, bound_t c)
{
  if (bound_cmp(b,c)<0)
    bound_set_infty(a,1);
  else
    bound_set(a,b);
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool bound_set_lint(bound_t a, long int b, numinternal_t intern)
{
  if (lint_fits_num(b)){
    _bound_inf(a);
    return num_set_lint(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_llint(bound_t a, long long int b, numinternal_t intern)
{
  if (llint_fits_num(b)){
    _bound_inf(a);
    return num_set_llint(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_mpz(bound_t a, mpz_t b, numinternal_t intern)
{
  if (mpz_fits_num(b)){
    _bound_inf(a);
    return num_set_mpz(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_lfrac(bound_t a, long int i, long int j, numinternal_t intern)
{
  if (lfrac_fits_num(i,j)){
    _bound_inf(a); 
    return num_set_lfrac(bound_numref(a),i,j,intern); 
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_llfrac(bound_t a, long long int i, long long int j, numinternal_t intern)
{
  if (llfrac_fits_num(i,j)){
    _bound_inf(a); 
    return num_set_lfrac(bound_numref(a),i,j,intern); 
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_mpq(bound_t a, mpq_t b, numinternal_t intern)
{
  if (mpq_fits_num(b)){
    _bound_inf(a);
    return num_set_mpq(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_double(bound_t a, double b, numinternal_t intern)
{
  if (double_fits_num(b)){
    _bound_inf(a);
    return num_set_double(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_ldouble(bound_t a, long double b, numinternal_t intern)
{
  if (ldouble_fits_num(b)){
    _bound_inf(a);
    return num_set_ldouble(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
static inline bool bound_set_mpfr(bound_t a, mpfr_t b, numinternal_t intern)
{
  if (mpfr_fits_num(b,intern)){
    _bound_inf(a);
    return num_set_mpfr(bound_numref(a),b,intern);
  }
  else {
    bound_set_infty(a,1);
    return false;
  }
}
#if !NUM_NUMIL
static inline bool bound_set_numIl(bound_t a, numIl_t b, numinternal_t intern)
{ return bound_set_lint(a,*b,intern); }
#endif
#if !NUM_NUMILL
static inline bool bound_set_numIll(bound_t a, numIll_t b, numinternal_t intern)
{ return bound_set_llint(a,*b,intern); }
#endif
#if !NUM_NUMMPZ
static inline bool bound_set_numMPZ(bound_t a, numMPZ_t b, numinternal_t intern)
{ return bound_set_mpz(a,b,intern); }
#endif
#if !NUM_NUMRL
static inline bool bound_set_numRl(bound_t a, numRl_t b, numinternal_t intern)
{ return bound_set_lfrac(a,*b->n,*b->d,intern); }
#endif
#if !NUM_NUMRLL
static inline bool bound_set_numRll(bound_t a, numRll_t b, numinternal_t intern)
{ return bound_set_llfrac(a,*b->n,*b->d,intern); }
#endif
#if !NUM_NUMMPQ
static inline bool bound_set_numMPQ(bound_t a, numMPQ_t b, numinternal_t intern)
{ return bound_set_mpq(a,b,intern); }
#endif
#if !NUM_NUMD
static inline bool bound_set_numD(bound_t a, numD_t b, numinternal_t intern)
{ return bound_set_double(a,*b,intern); }
#endif
#if !NUM_NUMDL
static inline bool bound_set_numDl(bound_t a, numDl_t b, numinternal_t intern)
{ return bound_set_ldouble(a,*b,intern); }
#endif
#if !NUM_NUMMPFR
static inline bool bound_set_numMPFR(bound_t a, numMPFR_t b, numinternal_t intern)
{ return bound_set_mpfr(a,b,intern); }
#endif

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
/* Serialization */
/* ====================================================================== */

static inline size_t bound_serialize(void* dst, bound_t src)
{
#ifndef BOUND_INF
  return num_serialize(dst,src);
#else
  *(char*)dst = src->inf;
  return num_serialize((char*)dst+1,bound_numref(src))+1;
#endif
}

static inline size_t bound_deserialize(bound_t dst, const void* src)
{
#ifndef BOUND_INF
  return num_deserialize(dst,src);
#else
  dst->inf = *(const char*)src;
  return num_deserialize(bound_numref(dst),(const char*)src+1)+1;
#endif
}

static inline size_t bound_serialized_size(bound_t a)
{
#ifndef BOUND_INF
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

#ifdef __cplusplus
}
#endif

#endif
