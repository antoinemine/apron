/* ********************************************************************** */
/* eitvXXX.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "math.h"
#include "eitvXXX.h"
#include "num_internal.h"

/* ********************************************************************** */
/* Initialization and clearing */
/* ********************************************************************** */

void eitvXXX_init_array(eitvXXX_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) eitvXXX_init(a[i]);
}
eitvXXX_t* eitvXXX_array_alloc(size_t size)
{
  eitvXXX_t* res = (eitvXXX_t*)ap_malloc(size*sizeof(eitvXXX_t));
  eitvXXX_init_array(res,size);
  return res;
}
void eitvXXX_array_free(eitvXXX_t* a, size_t size)
{
  eitvXXX_clear_array(a,size);
  free(a);
}

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool eitvXXX_canonicalize(eitvXXX_t a, bool integer)
{
  bool exc;
  int cmp;

  a->eq = false;
  if (integer){
    boundXXX_floor(a->itv->neginf,a->itv->neginf);
    boundXXX_floor(a->itv->sup,a->itv->sup);
  }
  if (boundXXX_infty(a->itv->neginf) || boundXXX_infty(a->itv->sup))
    return false;

  /* Check that it is not bottom */
  numXXX_ptr neginf = boundXXX_numref(a->itv->neginf);
  numXXX_neg(neginf,neginf);
  cmp = numXXX_cmp(boundXXX_numref(a->itv->sup),neginf);
  numXXX_neg(neginf,neginf);
  if (cmp<0){
    a->eq = false;
    exc = true;
  }
  else {
    a->eq = (cmp==0);
    exc = false;
  }
  return exc;
}

bool eitvXXX_is_int(const eitvXXX_t a, num_internal_t intern)
{
  boundXXX_trunc(intern->XXX.muldiv_bound,a->itv->sup);
  if (boundXXX_cmp(intern->XXX.muldiv_bound,a->itv->sup)) return false;
  if (a->eq)
    return true;
  else {
    boundXXX_trunc(intern->XXX.muldiv_bound,a->itv->neginf);
    return !boundXXX_cmp(intern->XXX.muldiv_bound,a->itv->neginf);
  }
}

/* ********************************************************************** */
/* Lattice operations */
/* ********************************************************************** */

bool eitvXXX_meet(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c)
{
  if (b==c) {
    eitvXXX_set(a,b);
    return eitvXXX_is_bottom(a);
  }
  else {
    boundXXX_min(a->itv->sup,b->itv->sup,c->itv->sup);
    boundXXX_min(a->itv->neginf,b->itv->neginf,c->itv->neginf);
    return eitvXXX_canonicalize(a,false);
  }
}


/* ********************************************************************** */
/* Arithmetic operations */
/* ********************************************************************** */

/* We assume no aliasing between

   - an itv and a num or a bound,
*/

void eitvXXX_mul_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c)
{
  assert(!eitvXXX_is_bottom(b));
  if (NUMXXX_EXACT && b->eq){
    boundXXX_mul_num(a->itv->sup,b->itv->sup,c);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    itvXXX_mul_num(a->itv,b->itv,c);
    a->eq = (numXXX_sgn(c)==0);
  }
}
void eitvXXX_mul_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c)
{
  assert(!eitvXXX_is_bottom(b));
  if (NUMXXX_EXACT && b->eq){
    boundXXX_mul(a->itv->sup,b->itv->sup,c);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    itvXXX_mul_bound(a->itv,b->itv,c);
    a->eq = (boundXXX_sgn(c)==0);
  }
}

void eitvXXX_div_num(eitvXXX_t a, const eitvXXX_t b, const numXXX_t c)
{
  assert(!eitvXXX_is_bottom(b));

  if (numXXX_sgn(c)==0){
    if (eitvXXX_is_zero(b)){
      eitvXXX_set_int(a,0);
    }
    else {
      itvXXX_div_zero(a->itv,b->itv);
      a->eq = false;
    }
  }
  else {
    if (NUMXXX_DIVEXACT && b->eq){
      boundXXX_div_num(a->itv->sup,b->itv->sup,c);
      boundXXX_neg(a->itv->neginf,a->itv->sup);
      a->eq = true;
    }
    else {
      itvXXX_div_num(a->itv,b->itv,c);
      a->eq = false;
    }
  }
}
void eitvXXX_div_bound(eitvXXX_t a, const eitvXXX_t b, const boundXXX_t c)
{
  assert(!eitvXXX_is_bottom(b));

  if (boundXXX_sgn(c)==0){
    if (eitvXXX_is_zero(b)){
      eitvXXX_set_int(a,0);
    }
    else {
      itvXXX_div_zero(a->itv,b->itv);
      a->eq = false;
    }
  }
  else {
    if (NUMXXX_DIVEXACT && b->eq){
      boundXXX_div(a->itv->sup,b->itv->sup,c);
      boundXXX_neg(a->itv->neginf,a->itv->sup);
      a->eq = true;
    }
    else {
      itvXXX_div_bound(a->itv,b->itv,c);
      a->eq = boundXXX_infty(c);
    }
  }
}
void eitvXXX_sub(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c)
{
  assert(!eitvXXX_is_bottom(b) && !eitvXXX_is_bottom(c));
  if (NUMXXX_EXACT && b->eq && c->eq){
    numXXX_sub(boundXXX_numref(a->itv->sup),
	       boundXXX_numref(b->itv->sup),
	       boundXXX_numref(c->itv->sup));
    _boundXXX_set_finite(a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else if (a!=c && c->eq){
    eitvXXX_sub_num(a,b,boundXXX_numref(c->itv->sup));
  }
  else {
    itvXXX_sub(a->itv,b->itv,c->itv);
    a->eq = false;
  }
}
void eitvXXX_abs(eitvXXX_t a, const eitvXXX_t b)
{
  assert(!eitvXXX_is_bottom(b));
  if (boundXXX_sgn(b->itv->neginf)<=0)
    /* positive interval */
    eitvXXX_set(a,b);
  else if (boundXXX_sgn(b->itv->sup)<=0)
    /* negative interval */
    eitvXXX_neg(a,b);
  else {
    boundXXX_max(a->itv->sup,b->itv->neginf,b->itv->sup);
    boundXXX_set_int(a->itv->neginf,0);
    a->eq = (boundXXX_sgn(a->itv->sup)==0);
  }
}
void eitvXXX_mul_2exp(eitvXXX_t a, const eitvXXX_t b, int c)
{
  assert(!eitvXXX_is_bottom(b));
  boundXXX_mul_2exp(a->itv->sup,b->itv->sup,c);
  if (NUMXXX_EXACT && b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_mul_2exp(a->itv->neginf,b->itv->neginf,c);
    a->eq = false;
  }
}



/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

void eitvXXX_mul(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c, num_internal_t intern)
{
  assert(!eitvXXX_is_bottom(b) && !eitvXXX_is_bottom(c));
  if (NUMXXX_EXACT && b->eq && c->eq){
    numXXX_mul(boundXXX_numref(a->itv->sup),
	       boundXXX_numref(b->itv->sup),
	       boundXXX_numref(c->itv->sup));
    _boundXXX_set_finite(a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else if (c->eq){
    numXXX_set(intern->XXX.muldiv_num,boundXXX_numref(c->itv->sup));
    eitvXXX_mul_num(a,b,intern->XXX.muldiv_num);
  }
  else if (b->eq){
    numXXX_set(intern->XXX.muldiv_num,boundXXX_numref(b->itv->sup));
    eitvXXX_mul_num(a,c,intern->XXX.muldiv_num);
  }
  else {
    itvXXX_mul(a->itv,b->itv,c->itv, intern);
    a->eq = false;
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

void eitvXXX_div(eitvXXX_t a, const eitvXXX_t b, const eitvXXX_t c, num_internal_t intern)
{
  assert(!eitvXXX_is_bottom(b) && !eitvXXX_is_bottom(c));
  if (NUMXXX_DIVEXACT && b->eq && c->eq &&
      numXXX_sgn(boundXXX_numref(c->itv->sup))){
    numXXX_div(boundXXX_numref(a->itv->sup),
	       boundXXX_numref(b->itv->sup),
	       boundXXX_numref(c->itv->sup));
    _boundXXX_set_finite(a->itv->sup);
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else if (c->eq){
    numXXX_set(intern->XXX.muldiv_num,boundXXX_numref(c->itv->sup));
    eitvXXX_div_num(a,b,intern->XXX.muldiv_num);
  }
  else {
    itvXXX_div(a->itv,b->itv,c->itv, intern);
    a->eq = itvXXX_is_point(a->itv);
  }
}

/* ********************************************************************** */
/* Casts */
/* ********************************************************************** */

void eitvXXX_ceil(eitvXXX_t a, const eitvXXX_t b)
{
  assert(!eitvXXX_is_bottom(b));
  boundXXX_ceil(a->itv->sup,b->itv->sup);
  if (b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_floor(a->itv->neginf,b->itv->neginf);
    a->eq = itvXXX_is_point(a->itv);
  }
}

void eitvXXX_floor(eitvXXX_t a, const eitvXXX_t b)
{
  assert(!eitvXXX_is_bottom(b));
  boundXXX_floor(a->itv->sup,b->itv->sup);
  if (b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_ceil(a->itv->neginf,b->itv->neginf);
    a->eq = itvXXX_is_point(a->itv);
  }
}

void eitvXXX_trunc(eitvXXX_t a, const eitvXXX_t b)
{
  assert(!eitvXXX_is_bottom(b));
  boundXXX_trunc(a->itv->sup,b->itv->sup);
  if (b->eq){
    boundXXX_neg(a->itv->neginf,a->itv->sup);
    a->eq = true;
  }
  else {
    boundXXX_trunc(a->itv->neginf,b->itv->neginf);
    a->eq = itvXXX_is_point(a->itv);
  }
}

void eitvXXX_to_int(eitvXXX_t a, const eitvXXX_t b)
{
  assert(!eitvXXX_is_bottom(b));
  itvXXX_to_int(a->itv,b->itv);
  a->eq = itvXXX_is_point(a->itv);
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t eitvXXX_serialize(void* dst, const eitvXXX_t src)
{
  *((char*)dst) = src->eq ? 1 : 0;
  return 1 + itvXXX_serialize((char*)dst+1,src->itv);
}

size_t eitvXXX_deserialize(eitvXXX_t dst, const void* src)
{
  dst->eq = (*((const char*)src) == 1);
  return 1 + itvXXX_deserialize(dst->itv,(const char*)src+1);
}
size_t eitvXXX_serialized_size(const eitvXXX_t a)
{
  return 1 + itvXXX_serialized_size(a->itv);
}

size_t eitvXXX_serialize_array(void* dst, eitvXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += eitvXXX_serialize((char*)dst+n,src[i]);
  return n;
}

size_t eitvXXX_deserialize_array(eitvXXX_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += eitvXXX_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t eitvXXX_serialized_size_array(eitvXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += eitvXXX_serialized_size(src[i]);
  return n;
}

void eitvXXX_fprint(FILE* stream, const eitvXXX_t a)
{
  itvXXX_fprint(stream,a->itv);
 }

void eitvXXX_print(const eitvXXX_t a)
{ eitvXXX_fprint(stdout, a); fflush(stdout); }

int eitvXXX_snprint(char* s, int size, const eitvXXX_t a)
{
  return itvXXX_snprint(s,size,a->itv);
}
