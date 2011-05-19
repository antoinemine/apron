/* ********************************************************************** */
/* boundXXX.h: numbers used for bounds */
/* ********************************************************************** */

#ifndef _BOUNDXXX_H_
#define _BOUNDXXX_H_

#include "num_types.h"
#include "numXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _NUMXXX_MARK_NUMXXX_
  /* Undefined at the end */

#define NUM_NUMFLT (defined(_NUMD_MARK_NUMD_) || defined(_NUMDl_MARK_NUMDl_) || defined(_NUMMPFR_MARK_NUMMPFR_))
#define NUM_NUMRAT (defined(_NUMRl_MARK_NUMRl_) || defined(_NUMRll_MARK_NUMRll_) || defined(_NUMMPQ_MARK_NUMMPQ_))

/* ---------------------------------------------------------------------- */
static inline bool boundXXX_infty(boundXXX_t a)
#if NUM_NUMFLT
{ return numXXX_infty(a); }
#elif NUM_NUMRAT
{ return numintXXX_sgn(numXXX_denref(a))==0; }
#elif defined(_NUMMPZ_MARK_NUMMPZ_)
{ return (bool)a->inf; }
#else
{ return (*a>=NUMXXX_MAX || *a<=-NUMXXX_MAX); }
#endif

/* ---------------------------------------------------------------------- */
static inline void boundXXX_set_infty(boundXXX_t a, int sgn)
#if NUM_NUMFLT
{ assert(sgn); numXXX_set_infty(a,sgn); }
#elif NUM_NUMRAT
{
  assert(sgn);
  numintXXX_set_int(numXXX_numref(a),sgn>0 ? 1 : -1);
  numintXXX_set_int(numXXX_denref(a),0);
}
#elif defined(_NUMMPZ_MARK_NUMMPZ_)
{
  assert(sgn);
  numXXX_set_int(a->num,sgn>0 ? 1 : -1);
  a->inf = 1;
}
#else
{ assert(sgn); *a = sgn>0 ? NUMXXX_MAX : -NUMXXX_MAX; }
#endif

/* ---------------------------------------------------------------------- */
static inline void boundXXX_init_set_infty(boundXXX_t a, int sgn)
{
  numXXX_init(boundXXX_numref(a));
  boundXXX_set_infty(a,sgn);
}
static inline void boundXXX_swap(boundXXX_t a, boundXXX_t b)
{
#if defined(_NUMMPZ_MARK_NUMMPZ_)
  int t = a->inf; a->inf = b->inf; b->inf = t;
#endif
  numXXX_swap(boundXXX_numref(a),boundXXX_numref(b));
}

static inline int boundXXX_sgn(boundXXX_t a)
{ return numXXX_sgn(boundXXX_numref(a)); }

/* ====================================================================== */
/* Assignement */
/* ====================================================================== */

#if defined(_NUMMPZ_MARK_NUMMPZ_)
static inline void boundXXX_set(boundXXX_t a, boundXXX_t b)
{ numXXX_set(a->num,b->num); a->inf = b->inf; }
static inline void boundXXX_set_array(boundXXX_t* a, boundXXX_t* b, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) boundXXX_set(a[i],b[i]);
}
#else
static inline void boundXXX_set(boundXXX_t a, boundXXX_t b)
{ numXXX_set(a,b); }
static inline void boundXXX_set_array(boundXXX_t* a, boundXXX_t* b, size_t size)
{ numXXX_set_array(a,b,size); }
#endif

static inline void boundXXX_set_int(boundXXX_t a, long int i)
{  numXXX_set_int(boundXXX_numref(a),i); _boundXXX_inf(a); }

static inline void boundXXX_set_num(boundXXX_t a, numXXX_t b)
{ numXXX_set(boundXXX_numref(a),b); _boundXXX_inf(a); }

/* ====================================================================== */
/* Constructors and Destructors */
/* ====================================================================== */

static inline void boundXXX_init(boundXXX_t a)
{ numXXX_init(boundXXX_numref(a)); _boundXXX_inf(a); }
static inline void boundXXX_init_set_int(boundXXX_t a, long int i)
{ numXXX_init_set_int(boundXXX_numref(a),i); _boundXXX_inf(a); }
static inline void boundXXX_init_set_num(boundXXX_t a, numXXX_t b)
{ numXXX_init_set(boundXXX_numref(a),b); _boundXXX_inf(a); }
static inline void boundXXX_clear(boundXXX_t a)
{ numXXX_clear(boundXXX_numref(a)); }

#if defined(_NUMMPZ_MARK_NUMMPZ_)
static inline void boundXXX_init_array(boundXXX_t* a, size_t size)
{
  size_t i;
  for (i=0;i<size;i++) boundXXX_init(a[i]);
}
static inline void boundXXX_init_set(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)){
    boundXXX_init_set_infty(a,boundXXX_sgn(b));
  } else {
    numXXX_init_set(boundXXX_numref(a),boundXXX_numref(b));
    _boundXXX_inf(a);
  }
}
static inline void boundXXX_clear_array(boundXXX_t* a, size_t size)
{
  size_t i;
  for (i=0;i<size;i++) boundXXX_clear(a[i]);
}

#else

static inline void boundXXX_init_array(boundXXX_t* a, size_t size)
{ numXXX_init_array(a,size); }
static inline void boundXXX_init_set(boundXXX_t a, boundXXX_t b)
{ numXXX_init_set(a,b); }
static inline void boundXXX_clear_array(boundXXX_t* a, size_t size)
#if defined(_NUMMPQ_MARK_NUMMPQ_) || defined(_NUMMPFR_MARK_NUMMPFR_)
{ numXXX_clear_array(a,size); }
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
static inline void boundXXX_neg(boundXXX_t a, boundXXX_t b)
{ numXXX_neg(a,b); }
static inline void boundXXX_abs(boundXXX_t a, boundXXX_t b)
{ numXXX_abs(a,b); }
static inline void boundXXX_add(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{ numXXX_add(a,b,c); }
static inline void boundXXX_add_uint(boundXXX_t a, boundXXX_t b, unsigned long int c)
{ numXXX_add_uint(a,b,c); }
static inline void boundXXX_add_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{ numXXX_add(a,b,c); }
static inline void boundXXX_sub(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{ numXXX_sub(a,b,c); }
static inline void boundXXX_sub_uint(boundXXX_t a, boundXXX_t b, unsigned long int c)
{ numXXX_sub_uint(a,b,c); }
static inline void boundXXX_sub_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{ numXXX_sub(a,b,c); }
static inline void boundXXX_mul(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{ if (!boundXXX_sgn(b) || !boundXXX_sgn(c)) numXXX_set_int(a,0); else numXXX_mul(a,b,c); }
static inline void boundXXX_mul_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{ if (!boundXXX_sgn(b) || !numXXX_sgn(c)) numXXX_set_int(a,0); else numXXX_mul(a,b,c); }
static inline void boundXXX_mul_2(boundXXX_t a, boundXXX_t b)
{ numXXX_mul_2(a,b); }
static inline void boundXXX_div(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (!boundXXX_sgn(b) || boundXXX_infty(c)) boundXXX_set_int(a,0);
  else if (!boundXXX_sgn(c)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else numXXX_div(a,b,c);
}
static inline void boundXXX_div_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{
  if (!boundXXX_sgn(b)) boundXXX_set_int(a,0);
  else if (!numXXX_sgn(c)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else numXXX_div(a,b,c);
}
static inline void boundXXX_div_2(boundXXX_t a, boundXXX_t b)
{ numXXX_div_2(a,b); }

#else

static inline void boundXXX_neg(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,-boundXXX_sgn(b));
  else { numXXX_neg(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
static inline void boundXXX_abs(boundXXX_t a, boundXXX_t b)
{ numXXX_abs(boundXXX_numref(a),boundXXX_numref(b)); }
static inline void boundXXX_add(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else if (boundXXX_infty(c)) boundXXX_set_infty(a,boundXXX_sgn(c));
  else { numXXX_add(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}
static inline void boundXXX_add_uint(boundXXX_t a, boundXXX_t b, unsigned long int c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_add_uint(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}
static inline void boundXXX_add_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_add(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}
static inline void boundXXX_sub(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else if (boundXXX_infty(c)) boundXXX_set_infty(a,-boundXXX_sgn(c));
  else { numXXX_sub(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}
static inline void boundXXX_sub_uint(boundXXX_t a, boundXXX_t b, unsigned long int c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_sub_uint(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}
static inline void boundXXX_sub_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_sub(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}

static inline void boundXXX_mul(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (!boundXXX_sgn(b) || !boundXXX_sgn(c)) boundXXX_set_int(a,0);
  else if (boundXXX_infty(b) || boundXXX_infty(c)) boundXXX_set_infty(a,boundXXX_sgn(b)*boundXXX_sgn(c));
  else { numXXX_mul(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}
static inline void boundXXX_mul_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{
  if (!boundXXX_sgn(b) || !numXXX_sgn(c)) boundXXX_set_int(a,0);
  else if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b)*numXXX_sgn(c));
  else { numXXX_mul(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}
static inline void boundXXX_mul_2(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_mul_2(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
static inline void boundXXX_div(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (!boundXXX_sgn(b) || boundXXX_infty(c)) boundXXX_set_int(a,0);
  else if (!boundXXX_sgn(c)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else if (boundXXX_infty(b))  boundXXX_set_infty(a,boundXXX_sgn(b)*boundXXX_sgn(c));
  else { numXXX_div(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}
static inline void boundXXX_div_num(boundXXX_t a, boundXXX_t b, numXXX_t c)
{
  if (!boundXXX_sgn(b)) boundXXX_set_int(a,0);
  else if (!numXXX_sgn(c)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else if (boundXXX_infty(b))  boundXXX_set_infty(a,boundXXX_sgn(b)*numXXX_sgn(c));
  else { numXXX_div(boundXXX_numref(a),boundXXX_numref(b),c); _boundXXX_inf(a); }
}
static inline void boundXXX_div_2(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_div_2(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
#endif


#if NUM_NUMFLT || defined(_NUMIl_MARK_NUMIl_) || defined(_NUMIll_MARK_NUMIll_)

static inline void boundXXX_min(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{ numXXX_min(a,b,c); }
static inline void boundXXX_max(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{ numXXX_max(a,b,c); }

#else

static inline void boundXXX_min(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (boundXXX_infty(b)) if (boundXXX_sgn(b)>0) boundXXX_set(a,c); else boundXXX_set(a,b);
  else if (boundXXX_infty(c)) if (boundXXX_sgn(c)>0) boundXXX_set(a,b); else boundXXX_set(a,c);
  else { numXXX_min(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}
static inline void boundXXX_max(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (boundXXX_infty(b)) if (boundXXX_sgn(b)>0) boundXXX_set(a,b); else boundXXX_set(a,c);
  else if (boundXXX_infty(c)) if (boundXXX_sgn(c)>0) boundXXX_set(a,c); else boundXXX_set(a,b);
  else { numXXX_max(boundXXX_numref(a),boundXXX_numref(b),boundXXX_numref(c)); _boundXXX_inf(a); }
}

#endif

static inline void boundXXX_floor(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_floor(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
static inline void boundXXX_ceil(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_ceil(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
static inline void boundXXX_trunc(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_trunc(boundXXX_numref(a),boundXXX_numref(b)); _boundXXX_inf(a); }
}
static inline void boundXXX_sqrt(boundXXX_t up, boundXXX_t down, boundXXX_t b)
{
  if (boundXXX_infty(b)) {
    boundXXX_set_infty(up,1);
    boundXXX_set_infty(down,1);
  }
  else {
    numXXX_sqrt(boundXXX_numref(up),boundXXX_numref(down),boundXXX_numref(b));
    _boundXXX_inf(up);
    _boundXXX_inf(down);
  }
}

void boundXXX_to_float(boundXXX_t a, boundXXX_t b, num_internal_t intern);
void boundXXX_to_double(boundXXX_t a, boundXXX_t b, num_internal_t intern);

static inline void boundXXX_mul_2exp(boundXXX_t a, boundXXX_t b, int c)
{
  if (boundXXX_infty(b)) boundXXX_set_infty(a,boundXXX_sgn(b));
  else { numXXX_mul_2exp(boundXXX_numref(a),boundXXX_numref(b),c); }
}


/* ====================================================================== */
/* Arithmetic Tests */
/* ====================================================================== */

#if NUM_NUMFLT || defined(_NUMIl_MARK_NUMIl_) || defined(_NUMIll_MARK_NUMIll_)

static inline int boundXXX_cmp(boundXXX_t a, boundXXX_t b)
{ return numXXX_cmp(a,b); }
static inline int boundXXX_cmp_int(boundXXX_t a, long int b)
{ return numXXX_cmp_int(a,b); }
static inline int boundXXX_cmp_num(boundXXX_t a, numXXX_t b)
{ return numXXX_cmp(a,b); }
static inline bool boundXXX_equal(boundXXX_t a, boundXXX_t b)
{ return numXXX_equal(a,b); }

#else

static inline int boundXXX_cmp(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(a)){
    if (boundXXX_infty(b)) return (boundXXX_sgn(a)-boundXXX_sgn(b))/2;
    else return boundXXX_sgn(a);
  } else {
    if (boundXXX_infty(b)) return -boundXXX_sgn(b);
    else return numXXX_cmp(boundXXX_numref(a),boundXXX_numref(b));
  }
}
static inline int boundXXX_cmp_int(boundXXX_t a, long int b)
{
  if (boundXXX_infty(a)) return boundXXX_sgn(a);
  else return numXXX_cmp_int(boundXXX_numref(a),b);
}
static inline int boundXXX_cmp_num(boundXXX_t a, numXXX_t b)
{
  if (boundXXX_infty(a)) return boundXXX_sgn(a);
  else return numXXX_cmp(boundXXX_numref(a),b);
}
static inline bool boundXXX_equal(boundXXX_t a, boundXXX_t b)
{
  if (boundXXX_infty(a)){
    return boundXXX_infty(b) && boundXXX_sgn(a)==boundXXX_sgn(b);
  } else {
    if (boundXXX_infty(b)) return false;
    else return numXXX_equal(boundXXX_numref(a),boundXXX_numref(b));
  }
}
#endif

static inline int boundXXX_hash(boundXXX_t a)
{
  if (boundXXX_infty(a))
    return boundXXX_sgn(a)>0 ? INT_MAX : -INT_MAX;
  else {
    return numXXX_hash(boundXXX_numref(a));
  }
}

static inline void boundXXX_widening(boundXXX_t a, boundXXX_t b, boundXXX_t c)
{
  if (boundXXX_cmp(b,c)<0)
    boundXXX_set_infty(a,1);
  else
    boundXXX_set(a,b);
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool boundXXX_set_lint(boundXXX_t a, long int b, num_internal_t intern);
bool boundXXX_set_llint(boundXXX_t a, long long int b, num_internal_t intern);
bool boundXXX_set_mpz(boundXXX_t a, mpz_t b, num_internal_t intern);
bool boundXXX_set_lfrac(boundXXX_t a, long int i, long int j, num_internal_t intern);
bool boundXXX_set_llfrac(boundXXX_t a, long long int i, long long int j, num_internal_t intern);
bool boundXXX_set_mpq(boundXXX_t a, mpq_t b, num_internal_t intern);
bool boundXXX_set_double(boundXXX_t a, double b, num_internal_t intern);
bool boundXXX_set_ldouble(boundXXX_t a, long double b, num_internal_t intern);
bool boundXXX_set_mpfr(boundXXX_t a, mpfr_t b, num_internal_t intern);

MACRO_FOREACH ZZZ @AllNum
bool boundXXX_set_numZZZ(boundXXX_t a, numZZZ_t b, num_internal_t intern);
ENDMACRO

MACRO_FOREACH ZZZ @MainNum
bool boundXXX_set_boundZZZ(boundXXX_t a, boundZZZ_t b, num_internal_t intern);
bool boundZZZ_set_boundXXX(boundZZZ_t a, boundXXX_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* Printing */
/* ====================================================================== */

static inline void boundXXX_fprint(FILE* stream, boundXXX_t a)
{
  if (boundXXX_infty(a)) fprintf(stream,"%coo",boundXXX_sgn(a)>0 ? '+' : '-');
  else numXXX_fprint(stream,boundXXX_numref(a));
}
static inline void boundXXX_print(boundXXX_t a)
{
  boundXXX_fprint(stdout,a);
}
static inline int boundXXX_snprint(char* s, size_t size, boundXXX_t a)
{
  if (boundXXX_infty(a)) return snprintf(s,size,"%coo",boundXXX_sgn(a)>0 ? '+' : '-');
  else return numXXX_snprint(s,size,boundXXX_numref(a));
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

static inline size_t boundXXX_serialize(void* dst, boundXXX_t src)
{
#if defined(_NUMMPZ_MARK_NUMMPZ_)
  *(char*)dst = src->inf;
  return numXXX_serialize((char*)dst+1,boundXXX_numref(src))+1;
#else
  return numXXX_serialize(dst,src);
#endif
}

static inline size_t boundXXX_deserialize(boundXXX_t dst, const void* src)
{
#if defined(_NUMMPZ_MARK_NUMMPZ_)
  dst->inf = *(const char*)src;
  return numXXX_deserialize(boundXXX_numref(dst),(const char*)src+1)+1;
#else
  return numXXX_deserialize(dst,src);
#endif
}

static inline size_t boundXXX_serialized_size(boundXXX_t a)
{
#if defined(_NUMMPZ_MARK_NUMMPZ_)
  return numXXX_serialized_size(boundXXX_numref(a))+1;
#else
  return numXXX_serialized_size(a);
#endif
}

static inline size_t boundXXX_serialize_array(void* dst, boundXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundXXX_serialize((char*)dst+n,src[i]);
  return n;
}

static inline size_t boundXXX_deserialize_array(boundXXX_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundXXX_deserialize(dst[i],(const char*)src+n);
  return n;
}

static inline size_t boundXXX_serialized_size_array(boundXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += boundXXX_serialized_size(src[i]);
  return n;
}

#ifdef __cplusplus
}
#endif

#undef _NUMXXX_MARK_NUMXXX_

#endif
