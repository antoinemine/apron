/* GENERATED, DO NOT MODIFY */
#line 1 "/Users/bjeannet/dev/newcoeffslinexpr/num/boundXXX.h"
/* ********************************************************************** */
/* boundTypes.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUNDTypes_H_
#define _BOUNDTypes_H_

#include "boundTypes.h"
#include "numTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_NUMFLT ("Types"=="D" || "Types"=="Dl" || "Types"=="MPFR")
#define NUM_NUMRAT ("Types"=="Rl" || "Types"=="Rll" || "Types"=="MPQ")

/* ---------------------------------------------------------------------- */
static inline bool boundTypes_infty(boundTypes_t a)
#if NUM_NUMFLT
{ return numTypes_infty(a); }
#elif NUM_NUMRAT
{ return numTypes_sgn(numTypes_denref(a))==0; }
#elif "Types"=="MPZ"
{ return (bool)a->inf; }
#else
{ return (*a>=NUMTypes_MAX || *a<=-NUMTypes_MAX); }
#endif

/* ---------------------------------------------------------------------- */
static inline void boundTypes_set_infty(boundTypes_t a, int sgn)
#if NUM_NUMFLT
{ assert(sgn); numTypes_set_infty(a,sgn); }
#elif NUM_NUMRAT
{
  assert(sgn);
  numTypes_set_int(numTypes_numref(a),sgn>0 ? 1 : -1);
  numTypes_set_int(numTypes_denref(a),0);
}
#elif "Types"=="MPZ"
{
  assert(sgn);
  numTypes_set_int(a->num,sgn>0 ? 1 : -1);
  a->inf = 1;
}
#else
{ assert(sgn); *a = sgn>0 ? NUMTypes_MAX : -NUMTypes_MAX; }
#endif

/* ---------------------------------------------------------------------- */
static inline void boundTypes_init_set_infty(boundTypes_t a, int sgn)
{
  numTypes_init(boundTypes_numref(a));
  boundTypes_set_infty(a,sgn);
}
static inline void boundTypes_swap(boundTypes_t a, boundTypes_t b)
{
#ifdef "Types"=="MPZ"
  int t = a->inf; a->inf = b->inf; b->inf = t;
#endif
  numTypes_swap(boundTypes_numref(a),boundTypes_numref(b));
}

static inline int boundTypes_sgn(boundTypes_t a)
{ return numTypes_sgn(boundTypes_numref(a)); }

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

#if "Types"=="MPZ"
static inline void boundTypes_set(boundTypes_t a, boundTypes_t b)
{ numTypes_set(a->num,b->num); a->inf = b->inf; }
static inline void boundTypes_set_array(boundTypes_t* a, boundTypes_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) boundTypes_set(a[i],b[i]);
}
#else
static inline void boundTypes_set(boundTypes_t a, boundTypes_t b)
{ numTypes_set(a,b); }
static inline void boundTypes_set_array(boundTypes_t* a, boundTypes_t* b, size_t size)
{ numTypes_set_array(a,b,size); }
#endif

static inline void boundTypes_set_int(boundTypes_t a, long int i)
{  numTypes_set_int(boundTypes_numref(a),i); _boundTypes_inf(a); }

static inline void boundTypes_set_num(boundTypes_t a, numTypes_t b)
{ numTypes_set(boundTypes_numref(a),b); _boundTypes_inf(a); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void boundTypes_init(boundTypes_t a)
{ numTypes_init(boundTypes_numref(a)); _boundTypes_inf(a); }
static inline void boundTypes_init_set_int(boundTypes_t a, long int i)
{ numTypes_init_set_int(boundTypes_numref(a),i); _boundTypes_inf(a); }
static inline void boundTypes_clear(boundTypes_t a)
{ numTypes_clear(boundTypes_numref(a)); }

#if "Types"=="MPZ"
static inline void boundTypes_init_array(boundTypes_t* a, size_t size)
{
  size_t i;
  for (i=0;i<size;i++) boundTypes_init(a[i]);
}
static inline void boundTypes_init_set(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)){
    boundTypes_init_set_infty(a,boundTypes_sgn(b));
  } else {
    numTypes_init_set(boundTypes_numref(a),boundTypes_numref(b));
    _boundTypes_inf(a);
  }
}
static inline void boundTypes_clear_array(boundTypes_t* a, size_t size)
{
  size_t i;
  for (i=0;i<size;i++) boundTypes_clear(a[i]);
}

#else

static inline void boundTypes_init_array(boundTypes_t* a, size_t size)
{ numTypes_init_array(a,size); }
static inline void boundTypes_init_set(boundTypes_t a, boundTypes_t b)
{ numTypes_init_set(a,b); }
static inline void boundTypes_clear_array(boundTypes_t* a, size_t size)
#if "Types"=="MPQ" || "Types"="MPFR"
{ numTypes_clear_array(a,size); }
#else
{}
#endif
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
static inline void boundTypes_neg(boundTypes_t a, boundTypes_t b)
{ numTypes_neg(a,b); }
static inline void boundTypes_abs(boundTypes_t a, boundTypes_t b)
{ numTypes_abs(a,b); }
static inline void boundTypes_add(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{ numTypes_add(a,b,c); }
static inline void boundTypes_add_uint(boundTypes_t a, boundTypes_t b, unsigned long int c)
{ numTypes_add_uint(a,b,c); }
static inline void boundTypes_add_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{ numTypes_add(a,b,c); }
static inline void boundTypes_sub(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{ numTypes_sub(a,b,c); }
static inline void boundTypes_sub_uint(boundTypes_t a, boundTypes_t b, unsigned long int c)
{ numTypes_sub_uint(a,b,c); }
static inline void boundTypes_sub_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{ numTypes_sub(a,b,c); }
static inline void boundTypes_mul(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{ if (!boundTypes_sgn(b) || !boundTypes_sgn(c)) numTypes_set_int(a,0); else numTypes_mul(a,b,c); }
static inline void boundTypes_mul_numTypes(boundTypes_t a, boundTypes_t b, numTypes_t c)
{ if (!boundTypes_sgn(b) || !numTypes_sgn(c)) numTypes_set_int(a,0); else numTypes_mul(a,b,c); }
static inline void boundTypes_mul_2(boundTypes_t a, boundTypes_t b)
{ numTypes_mul_2(a,b); }
static inline void boundTypes_div(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (!boundTypes_sgn(b) || boundTypes_infty(c)) boundTypes_set_int(a,0);
  else if (!boundTypes_sgn(c)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else numTypes_div(a,b,c);
}
static inline void boundTypes_div_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{
  if (!boundTypes_sgn(b)) boundTypes_set_int(a,0);
  else if (!numTypes_sgn(c)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else numTypes_div(a,b,c);
}
static inline void boundTypes_div_2(boundTypes_t a, boundTypes_t b)
{ numTypes_div_2(a,b); }

#else

static inline void boundTypes_neg(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,-boundTypes_sgn(b));
  else { numTypes_neg(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
static inline void boundTypes_abs(boundTypes_t a, boundTypes_t b)
{ numTypes_abs(boundTypes_numref(a),boundTypes_numref(b)); }
static inline void boundTypes_add(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else if (boundTypes_infty(c)) boundTypes_set_infty(a,boundTypes_sgn(c));
  else { numTypes_add(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}
static inline void boundTypes_add_uint(boundTypes_t a, boundTypes_t b, unsigned long int c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_add_uint(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}
static inline void boundTypes_add_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_add(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}
static inline void boundTypes_sub(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else if (boundTypes_infty(c)) boundTypes_set_infty(a,-boundTypes_sgn(c));
  else { numTypes_sub(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}
static inline void boundTypes_sub_uint(boundTypes_t a, boundTypes_t b, unsigned long int c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_sub_uint(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}
static inline void boundTypes_sub_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_sub(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}

static inline void boundTypes_mul(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (!boundTypes_sgn(b) || !boundTypes_sgn(c)) boundTypes_set_int(a,0);
  else if (boundTypes_infty(b) || boundTypes_infty(c)) boundTypes_set_infty(a,boundTypes_sgn(b)*boundTypes_sgn(c));
  else { numTypes_mul(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}
static inline void boundTypes_mul_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{
  if (!boundTypes_sgn(b) || !numTypes_sgn(c)) boundTypes_set_int(a,0);
  else if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b)*numTypes_sgn(c));
  else { numTypes_mul(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}
static inline void boundTypes_mul_2(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_mul_2(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
static inline void boundTypes_div(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (!boundTypes_sgn(b) || boundTypes_infty(c)) boundTypes_set_int(a,0);
  else if (!boundTypes_sgn(c)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else if (boundTypes_infty(b))  boundTypes_set_infty(a,boundTypes_sgn(b)*boundTypes_sgn(c));
  else { numTypes_div(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}
static inline void boundTypes_div_num(boundTypes_t a, boundTypes_t b, numTypes_t c)
{
  if (!boundTypes_sgn(b)) boundTypes_set_int(a,0);
  else if (!numTypes_sgn(c)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else if (boundTypes_infty(b))  boundTypes_set_infty(a,boundTypes_sgn(b)*numTypes_sgn(c));
  else { numTypes_div(boundTypes_numref(a),boundTypes_numref(b),c); _boundTypes_inf(a); }
}
static inline void boundTypes_div_2(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_div_2(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
#endif


#if NUM_NUMFLT || "Types"=="Il" || "Types"=="Ill"

static inline void boundTypes_min(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{ numTypes_min(a,b,c); }
static inline void boundTypes_max(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{ numTypes_max(a,b,c); }

#else

static inline void boundTypes_min(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (boundTypes_infty(b)) if (boundTypes_sgn(b)>0) boundTypes_set(a,c); else boundTypes_set(a,b);
  else if (boundTypes_infty(c)) if (boundTypes_sgn(c)>0) boundTypes_set(a,b); else boundTypes_set(a,c);
  else { numTypes_min(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}
static inline void boundTypes_max(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (boundTypes_infty(b)) if (boundTypes_sgn(b)>0) boundTypes_set(a,b); else boundTypes_set(a,c);
  else if (boundTypes_infty(c)) if (boundTypes_sgn(c)>0) boundTypes_set(a,c); else boundTypes_set(a,b);
  else { numTypes_max(boundTypes_numref(a),boundTypes_numref(b),boundTypes_numref(c)); _boundTypes_inf(a); }
}

#endif

static inline void boundTypes_floor(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_floor(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
static inline void boundTypes_ceil(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_ceil(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
static inline void boundTypes_trunc(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_trunc(boundTypes_numref(a),boundTypes_numref(b)); _boundTypes_inf(a); }
}
static inline void boundTypes_sqrt(boundTypes_t up, boundTypes_t down, boundTypes_t b)
{
  if (boundTypes_infty(b)) {
    boundTypes_set_infty(up,1);
    boundTypes_set_infty(down,1);
  }
  else {
    numTypes_sqrt(boundTypes_numref(up),boundTypes_numref(down),boundTypes_numref(b));
    _boundTypes_inf(up);
    _boundTypes_inf(down);
  }
}
static inline void boundTypes_to_float(boundTypes_t a, boundTypes_t b, numinternal_t intern)
{
  if (boundTypes_infty(b) || !numTypes_fits_float(boundTypes_numref(b)))
    boundTypes_set_infty(a,boundTypes_sgn(b));
  else {
    double d;
    double_set_numTypes(&d,boundTypes_numref(b),intern);
    numTypes_set_double(boundTypes_numref(a),(double)((float)d),intern);
    _boundTypes_inf(a);
  }
}
static inline void boundTypes_to_double(boundTypes_t a, boundTypes_t b, numinternal_t intern)
{
  if (boundTypes_infty(b) || !numTypes_fits_double(boundTypes_numref(b)))
    boundTypes_set_infty(a,boundTypes_sgn(b));
  else {
    double d;
    double_set_numTypes(&d,boundTypes_numref(b),intern);
    numTypes_set_double(boundTypes_numref(a),d,intern);
    _boundTypes_inf(a);
  }
}

static inline void boundTypes_mul_2exp(boundTypes_t a, boundTypes_t b, int c)
{
  if (boundTypes_infty(b)) boundTypes_set_infty(a,boundTypes_sgn(b));
  else { numTypes_mul_2exp(boundTypes_numref(a),boundTypes_numref(b),c); }
}


/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

#if NUM_NUMFLT || "Types"=="Il" || "Types"=="Ill"

static inline int boundTypes_cmp(boundTypes_t a, boundTypes_t b)
{ return numTypes_cmp(a,b); }
static inline int boundTypes_cmp_int(boundTypes_t a, long int b)
{ return numTypes_cmp_int(a,b); }
static inline int boundTypes_cmp_num(boundTypes_t a, numTypes_t b)
{ return numTypes_cmp(a,b); }
static inline bool boundTypes_equal(boundTypes_t a, boundTypes_t b)
{ return numTypes_equal(a,b); }

#else

static inline int boundTypes_cmp(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(a)){
    if (boundTypes_infty(b)) return (boundTypes_sgn(a)-boundTypes_sgn(b))/2;
    else return boundTypes_sgn(a);
  } else {
    if (boundTypes_infty(b)) return -boundTypes_sgn(b);
    else return numTypes_cmp(boundTypes_numref(a),boundTypes_numref(b));
  }
}
static inline int boundTypes_cmp_int(boundTypes_t a, long int b)
{
  if (boundTypes_infty(a)) return boundTypes_sgn(a);
  else return numTypes_cmp_int(boundTypes_numref(a),b);
}
static inline int boundTypes_cmp_num(boundTypes_t a, numTypes_t b)
{
  if (boundTypes_infty(a)) return boundTypes_sgn(a);
  else return numTypes_cmp(boundTypes_numref(a),b);
}
static inline bool boundTypes_equal(boundTypes_t a, boundTypes_t b)
{
  if (boundTypes_infty(a)){
    return boundTypes_infty(b) && boundTypes_sgn(a)==boundTypes_sgn(b);
  } else {
    if (boundTypes_infty(b)) return false;
    else return numTypes_equal(boundTypes_numref(a),boundTypes_numref(b));
  }
}
#endif

static inline int boundTypes_hash(boundTypes_t a)
{ 
  if (boundTypes_infty(a))
    return boundTypes_sgn(a)>0 ? INT_MAX : -INT_MAX;
  else {
    return numTypes_hash(boundTypes_numref(a));
  }
}

static inline void boundTypes_widening(boundTypes_t a, boundTypes_t b, boundTypes_t c)
{
  if (boundTypes_cmp(b,c)<0)
    boundTypes_set_infty(a,1);
  else
    boundTypes_set(a,b);
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

static inline bool boundTypes_set_lint(boundTypes_t a, long int b, numinternal_t intern)
{
  if (lint_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_lint(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_llint(boundTypes_t a, long long int b, numinternal_t intern)
{
  if (llint_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_llint(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_mpz(boundTypes_t a, mpz_t b, numinternal_t intern)
{
  if (mpz_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_mpz(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_lfrac(boundTypes_t a, long int i, long int j, numinternal_t intern)
{
  if (lfrac_fits_numTypes(i,j)){
    _boundTypes_inf(a); 
    return numTypes_set_lfrac(boundTypes_numref(a),i,j,intern); 
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_llfrac(boundTypes_t a, long long int i, long long int j, numinternal_t intern)
{
  if (llfrac_fits_numTypes(i,j)){
    _boundTypes_inf(a); 
    return numTypes_set_lfrac(boundTypes_numref(a),i,j,intern); 
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_mpq(boundTypes_t a, mpq_t b, numinternal_t intern)
{
  if (mpq_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_mpq(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_double(boundTypes_t a, double b, numinternal_t intern)
{
  if (double_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_double(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_ldouble(boundTypes_t a, long double b, numinternal_t intern)
{
  if (ldouble_fits_numTypes(b)){
    _boundTypes_inf(a);
    return numTypes_set_ldouble(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_mpfr(boundTypes_t a, mpfr_t b, numinternal_t intern)
{
  if (mpfr_fits_numTypes(b,intern)){
    _boundTypes_inf(a);
    return numTypes_set_mpfr(boundTypes_numref(a),b,intern);
  }
  else {
    boundTypes_set_infty(a,1);
    return false;
  }
}
static inline bool boundTypes_set_numIl(boundTypes_t a, numIl_t b, numinternal_t intern)
{ return boundTypes_set_lint(a,*b,intern); }
static inline bool boundTypes_set_numIll(boundTypes_t a, numIll_t b, numinternal_t intern)
{ return boundTypes_set_llint(a,*b,intern); }
static inline bool boundTypes_set_numMPZ(boundTypes_t a, numMPZ_t b, numinternal_t intern)
{ return boundTypes_set_mpz(a,b,intern); }
static inline bool boundTypes_set_numRl(boundTypes_t a, numRl_t b, numinternal_t intern)
{ return boundTypes_set_lfrac(a,*b->n,*b->d,intern); }
static inline bool boundTypes_set_numRll(boundTypes_t a, numRll_t b, numinternal_t intern)
{ return boundTypes_set_llfrac(a,*b->n,*b->d,intern); }
static inline bool boundTypes_set_numMPQ(boundTypes_t a, numMPQ_t b, numinternal_t intern)
{ return boundTypes_set_mpq(a,b,intern); }
static inline bool boundTypes_set_numD(boundTypes_t a, numD_t b, numinternal_t intern)
{ return boundTypes_set_double(a,*b,intern); }
static inline bool boundTypes_set_numDl(boundTypes_t a, numDl_t b, numinternal_t intern)
{ return boundTypes_set_ldouble(a,*b,intern); }
static inline bool boundTypes_set_numMPFR(boundTypes_t a, numMPFR_t b, numinternal_t intern)
{ return boundTypes_set_mpfr(a,b,intern); }

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void boundTypes_fprint(FILE* stream, boundTypes_t a)
{
  if (boundTypes_infty(a)) fprintf(stream,"%coo",boundTypes_sgn(a)>0 ? '+' : '-');
  else numTypes_fprint(stream,boundTypes_numref(a));
}
static inline void boundTypes_print(boundTypes_t a)
{
  boundTypes_fprint(stdout,a);
}
static inline int boundTypes_snprint(char* s, size_t size, boundTypes_t a)
{
  if (boundTypes_infty(a)) return snprintf(s,size,"%coo",boundTypes_sgn(a)>0 ? '+' : '-');
  else return numTypes_snprint(s,size,boundTypes_numref(a));
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t boundTypes_serialize(void* dst, boundTypes_t src)
{
#if "Types"=="MPZ"
  *(char*)dst = src->inf;
  return numTypes_serialize((char*)dst+1,boundTypes_numref(src))+1;
#else
  return numTypes_serialize(dst,src);
#endif
}

static inline size_t boundTypes_deserialize(boundTypes_t dst, const void* src)
{
#if "Types"=="MPZ"
  dst->inf = *(const char*)src;
  return numTypes_deserialize(boundTypes_numref(dst),(const char*)src+1)+1;
#else
  return numTypes_deserialize(dst,src);
#endif
}

static inline size_t boundTypes_serialized_size(boundTypes_t a)
{
#if "Types"=="MPZ"
  return numTypes_serialized_size(boundTypes_numref(a))+1;
#else
  return numTypes_serialized_size(a);
#endif
}

static inline size_t boundTypes_serialize_array(void* dst, boundTypes_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundTypes_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t boundTypes_deserialize_array(boundTypes_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundTypes_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t boundTypes_serialized_size_array(boundTypes_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundTypes_serialized_size(src[i]);
  return n;
}

#ifdef __cplusplus
}
#endif

#endif
