/* ********************************************************************** */
/* itvXXX.c: (unidimensional) intervals */
/* ********************************************************************** */

#include <math.h>
#include <stdarg.h>
#include "itvXXX.h"
#include "num_internal.h"

/* ********************************************************************** */
/* Initialization and clearing */
/* ********************************************************************** */

void itvXXX_init_array(itvXXX_t* a, size_t size)
{
  size_t i;
  for (i=0; i<size; i++) itvXXX_init(a[i]);
}
itvXXX_t* itvXXX_array_alloc(size_t size)
{
  itvXXX_t* res = (itvXXX_t*)ap_malloc(size*sizeof(itvXXX_t));
  itvXXX_init_array(res,size);
  return res;
}
void itvXXX_array_free(itvXXX_t* a, size_t size)
{
  itvXXX_clear_array(a,size);
  free(a);
}

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool itvXXX_canonicalize(itvXXX_t a, bool integer)
{
  bool exc;

  if (integer){
    boundXXX_floor(a->neginf,a->neginf);
    boundXXX_floor(a->sup,a->sup);
  }
  if (boundXXX_infty(a->neginf) || boundXXX_infty(a->sup)) return false;

  /* Check that it is not bottom */
  exc = false;
  numXXX_ptr neginf = boundXXX_numref(a->neginf);
  numXXX_neg(neginf,neginf);
  if (numXXX_cmp(boundXXX_numref(a->sup),neginf) < 0)
    exc = true;
  numXXX_neg(neginf,neginf);
  return exc;
}

bool itvXXX_is_int(itvXXX_t a, num_internal_t intern)
{
  boundXXX_trunc(intern->XXX.muldiv_bound,a->sup);
  if (boundXXX_cmp(intern->XXX.muldiv_bound,a->sup)) return false;
  boundXXX_trunc(intern->XXX.muldiv_bound,a->neginf);
  return !boundXXX_cmp(intern->XXX.muldiv_bound,a->neginf);
}
bool itvXXX_is_point(itvXXX_t a)
{
  if (!boundXXX_infty(a->neginf) && !boundXXX_infty(a->sup)){
    numXXX_ptr b = boundXXX_numref(a->neginf);
    numXXX_neg(b,b);
    bool res = numXXX_equal(b,boundXXX_numref(a->sup));
    numXXX_neg(b,b);
    return res;
  }
  else
    return false;
}

/* Comparison:
   0: equality
   -1: i1 included in i2
   +1: i2 included in i1
   -2: i1->sup less than i2->sup
   +2: i1->sup greater than i2->sup
*/
int itvXXX_cmp(itvXXX_t a, itvXXX_t b)
{
  int sup = boundXXX_cmp(a->sup,b->sup);
  int neginf = boundXXX_cmp(a->neginf,b->neginf);
  if (neginf<0)
    return sup<=0 ? -1 : 2;
  else if (neginf==0)
    return sup<0 ? -1 : (sup==0 ? 0 : 1);
  else
    return sup<0 ? -2 : 1;
}
int itvXXX_cmp_zero(itvXXX_t a)
{
  int sup = boundXXX_sgn(a->sup);
  int neginf = boundXXX_sgn(a->neginf);
  if (neginf<0)
    return sup<=0 ? -1 : 2;
  else if (neginf==0)
    return sup<0 ? -1 : (sup==0 ? 0 : 1);
  else
    return sup<0 ? -2 : 1;
}

void itvXXX_range_rel(boundXXX_t a, itvXXX_t b, num_internal_t intern)
{
  boundXXX_add(a,b->sup,b->neginf);
  if (!boundXXX_infty(a)) {
    itvXXX_magnitude(intern->XXX.muldiv_bound,b);
    boundXXX_mul_2exp(intern->XXX.muldiv_bound,intern->XXX.muldiv_bound,-1);
    boundXXX_div(a,a,intern->XXX.muldiv_bound);
  }
}

/* ********************************************************************** */
/* Lattice operations */
/* ********************************************************************** */

bool itvXXX_meet(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  boundXXX_min(a->sup,b->sup,c->sup);
  boundXXX_min(a->neginf,b->neginf,c->neginf);
  return itvXXX_canonicalize(a,false);
}
void itvXXX_join(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  boundXXX_max(a->sup,b->sup,c->sup);
  boundXXX_max(a->neginf,b->neginf,c->neginf);
}
void itvXXX_widening(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  boundXXX_widening(a->sup,b->sup,c->sup);
  boundXXX_widening(a->neginf,b->neginf,c->neginf);
}

/* ********************************************************************** */
/* Arithmetic operations */
/* ********************************************************************** */

/* We assume no aliasing between

   - an itv and a num or a bound,
*/

MACRO_FOREACH OP ("mul","div");
void itvXXX_OP_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  assert(!itvXXX_is_bottom(b));
  assert (c!=boundXXX_numref(a->neginf) && c!=boundXXX_numref(a->sup) &&
	  c!=boundXXX_numref(b->neginf) && c!=boundXXX_numref(b->sup));
  if (numXXX_sgn(c)>=0){
    boundXXX_OP_num(a->neginf,b->neginf,c);
    boundXXX_OP_num(a->sup,b->sup,c);
  }
  else {
    numXXX_neg(c,c);
    boundXXX_OP_num(a->neginf,b->neginf,c);
    boundXXX_OP_num(a->sup,b->sup,c);
    boundXXX_swap(a->neginf,a->sup);
    numXXX_neg(c,c);
  }
#ifndef NDEBUG
  boundXXX_neg(a->neginf,a->neginf);
  if (boundXXX_cmp(a->neginf,a->sup)>0)
    abort();
  else
    boundXXX_neg(a->neginf,a->neginf);
#endif
}
void itvXXX_OP_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  assert(!itvXXX_is_bottom(b));
  assert (c!=(a->neginf) && c!=(a->sup) &&
	  c!=(b->neginf) && c!=(b->sup));
  if (boundXXX_sgn(c)>=0){
    boundXXX_OP(a->neginf,b->neginf,c);
    boundXXX_OP(a->sup,b->sup,c);
  }
  else {
    boundXXX_neg(c,c);
    boundXXX_OP(a->neginf,b->neginf,c);
    boundXXX_OP(a->sup,b->sup,c);
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(c,c);
  }
#ifndef NDEBUG
  boundXXX_neg(a->neginf,a->neginf);
  if (boundXXX_cmp(a->neginf,a->sup)>0)
    abort();
  else
    boundXXX_neg(a->neginf,a->neginf);
#endif
}
ENDMACRO;

void itvXXX_sub(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  if (a!=c){
    boundXXX_add(a->neginf,b->neginf,c->sup);
    boundXXX_add(a->sup,b->sup,c->neginf);
  } else if (a!=b) { /* a=c */
    boundXXX_swap(c->sup,c->neginf);
    itvXXX_add(a,b,c);
  } else { /* a=b=c */
    boundXXX_add(a->sup,b->sup,c->neginf);
    boundXXX_set(a->neginf,a->sup);
  }
}
void itvXXX_neg(itvXXX_t a, itvXXX_t b)
{
  assert(!itvXXX_is_bottom(b));
  if (a!=b){
    boundXXX_set(a->neginf,b->sup);
    boundXXX_set(a->sup,b->neginf);
  } else {
    boundXXX_swap(a->neginf,a->sup);
  }
}

bool itvXXX_sqrt(itvXXX_t a, itvXXX_t b, num_internal_t intern)
{
  bool exact = true;
  if (itvXXX_is_bottom(b) || boundXXX_sgn(b->sup)<0) {
    /* empty result */
    itvXXX_set_bottom(a);
    return true;
  }
  /* lower bound */
  if (boundXXX_sgn(b->neginf)>=0) {
    boundXXX_set_int(a->neginf,0);
  }
  else {
    boundXXX_neg(b->neginf,b->neginf);
    boundXXX_sqrt(intern->XXX.sqrt_bound,a->neginf,b->neginf);
    exact = exact && boundXXX_equal(intern->XXX.sqrt_bound,a->neginf);
    boundXXX_neg(b->neginf,b->neginf);
    if (a!=b) boundXXX_neg(a->neginf,a->neginf);
  }
  /* upper bound */
  boundXXX_sqrt(a->sup,intern->XXX.sqrt_bound,b->sup);
  exact = exact && boundXXX_equal(intern->XXX.sqrt_bound,a->sup);
  return exact;
}

void itvXXX_abs(itvXXX_t a, itvXXX_t b)
{
  assert(!itvXXX_is_bottom(b));
  if (boundXXX_sgn(b->neginf)<=0)
    /* positive interval */
    itvXXX_set(a,b);
  else if (boundXXX_sgn(b->sup)<=0)
    /* negative interval */
    itvXXX_neg(a,b);
  else {
    boundXXX_max(a->sup,b->neginf,b->sup);
    boundXXX_set_int(a->neginf,0);
  }
}

void itvXXX_mod(itvXXX_t a, itvXXX_t b, itvXXX_t c,
		bool is_int, num_internal_t intern)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  /* b-|c|*trunc(b/|c|) */
  itvXXX_abs(intern->XXX.eval_itv, c);
  if (!boundXXX_sgn(intern->XXX.eval_itv->neginf)) itvXXX_set_top(a);
  else {
    itvXXX_div(intern->XXX.eval_itv2, b, intern->XXX.eval_itv, intern);
    itvXXX_trunc(intern->XXX.eval_itv2, intern->XXX.eval_itv2);
    itvXXX_mul(intern->XXX.eval_itv2, intern->XXX.eval_itv2, intern->XXX.eval_itv, intern);
    if (is_int) boundXXX_sub_uint(intern->XXX.eval_itv->sup,intern->XXX.eval_itv->sup,1);
    if (boundXXX_sgn(b->sup)<0) {
      /* [-max|c|,0] */
      boundXXX_set(intern->XXX.eval_itv->neginf, intern->XXX.eval_itv->sup);
      boundXXX_set_int(intern->XXX.eval_itv->sup, 0);
    }
    else if (boundXXX_sgn(b->neginf)>0)
      /* [-max|c|,max|c|] */
      boundXXX_set(intern->XXX.eval_itv->neginf, intern->XXX.eval_itv->sup);
    else
      /* [0,max|c|] */
      boundXXX_set_int(intern->XXX.eval_itv->neginf, 0);
    itvXXX_sub(a, b, intern->XXX.eval_itv2);
    itvXXX_meet(a, a, intern->XXX.eval_itv);
  }
}


/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itvXXX_mulpp(itvXXX_t a,
		  itvXXX_t b,
		  itvXXX_t c,
		  num_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<=0);
  boundXXX_neg(intern->XXX.mul_bound,c->neginf);
  boundXXX_mul(a->neginf,b->neginf,intern->XXX.mul_bound);
  boundXXX_mul(a->sup,b->sup,c->sup);
}
/* Assume that both intervals are negative */
static
void itvXXX_mulnn(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 num_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_neg(intern->XXX.mul_bound,c->sup);
  boundXXX_mul(intern->XXX.mul_bound,b->sup,intern->XXX.mul_bound);
  boundXXX_mul(a->sup,b->neginf,c->neginf);
  boundXXX_set(a->neginf,intern->XXX.mul_bound);
}
/* Assume that b is positive and c negative */
static
void itvXXX_mulpn(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 num_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_neg(intern->XXX.mul_bound,b->neginf);
  boundXXX_mul(a->neginf,b->sup,c->neginf);
  boundXXX_mul(a->sup,intern->XXX.mul_bound,c->sup);
}
/* Assume that interval c is positive */
static
void itvXXX_mulp(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 num_internal_t intern)
{
  assert(boundXXX_sgn(c->neginf)<=0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_mulpp(a,b,c, intern);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_mulpn(a,c,b, intern);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->sup */
    boundXXX_mul(a->neginf,b->neginf,c->sup);
    boundXXX_mul(a->sup,b->sup,c->sup);
  }
}
/* Assume that interval c is negative */
static
void itvXXX_muln(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 num_internal_t intern)
{
  assert(boundXXX_sgn(c->sup)<=0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_mulpn(a,b,c, intern);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_mulnn(a,b,c, intern);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->neginf */
    boundXXX_mul(a->sup,b->sup,c->neginf);
    boundXXX_mul(a->neginf,b->neginf,c->neginf);
    boundXXX_swap(a->neginf,a->sup);
  }
}

void itvXXX_mul(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  if (boundXXX_sgn(c->neginf)<=0){
    /* c is positive, */
    itvXXX_mulp(a,b,c, intern);
  }
  else if (boundXXX_sgn(c->sup)<=0){
    /* c is negative */
    itvXXX_muln(a,b,c, intern);
  }
  else if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive, */
    itvXXX_mulp(a,c,b, intern);
  }
  else if (boundXXX_sgn(c->sup)<=0){
    /* b is negative */
    itvXXX_muln(a,c,b, intern);
  }
  else {
    /* divide c */
    boundXXX_set(intern->XXX.mul_itv->neginf,c->neginf);
    boundXXX_set_int(intern->XXX.mul_itv->sup,0);
    itvXXX_muln(intern->XXX.mul_itv2,b,intern->XXX.mul_itv, intern);

    boundXXX_set_int(intern->XXX.mul_itv->neginf,0);
    boundXXX_set(intern->XXX.mul_itv->sup,c->sup);
    itvXXX_mulp(a,b,intern->XXX.mul_itv, intern);

    itvXXX_join(a,a,intern->XXX.mul_itv2);
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itvXXX_divpp(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_neg(intern->XXX.mul_bound,c->neginf);
  boundXXX_div(a->neginf,b->neginf,c->sup);
  boundXXX_div(a->sup,b->sup,intern->XXX.mul_bound);
}
/* Assume that both intervals are negative */
static
void itvXXX_divnn(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<0);
  boundXXX_neg(intern->XXX.mul_bound,b->neginf);
  boundXXX_div(a->neginf,b->sup,c->neginf);
  boundXXX_div(a->sup,intern->XXX.mul_bound,c->sup);
}
/* Assume that b is positive and c negative */
static
void itvXXX_divpn(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<0);
  boundXXX_neg(intern->XXX.mul_bound,b->sup);
  boundXXX_div(intern->XXX.mul_bound,intern->XXX.mul_bound,c->sup);
  boundXXX_div(a->sup,b->neginf,c->neginf);
  boundXXX_set(a->neginf,intern->XXX.mul_bound);
}
/* Assume that b is negative and c positive */
static
void itvXXX_divnp(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_neg(intern->XXX.mul_bound, b->neginf);
  boundXXX_div(a->neginf,intern->XXX.mul_bound,c->neginf);
  boundXXX_div(a->sup,b->sup,c->sup);
}

/* Assume that interval c is positive */
static
void itvXXX_divp(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(c->neginf)<0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_divpp(a,b,c, intern);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_divnp(a,b,c, intern);
  }
  else {
    /* 0 is in the middle of b: one divides b by c->neginf */
    boundXXX_neg(intern->XXX.mul_bound,c->neginf);
    boundXXX_div(a->neginf,b->neginf,intern->XXX.mul_bound);
    boundXXX_div(a->sup,b->sup,intern->XXX.mul_bound);
  }
}
/* Assume that interval c is negative */
static
void itvXXX_divn(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(boundXXX_sgn(c->sup)<0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_divpn(a,b,c, intern);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_divnn(a,b,c, intern);
  }
  else {
    /* 0 is in the middle of b: one cross-divide b by c->sup */
    boundXXX_neg(intern->XXX.mul_bound,c->sup);
    boundXXX_div(a->neginf,b->neginf,intern->XXX.mul_bound);
    boundXXX_div(a->sup,b->sup,intern->XXX.mul_bound);
    boundXXX_swap(a->sup,a->neginf);
  }
}

void itvXXX_div_zero(itvXXX_t a, itvXXX_t b)
{
  if (boundXXX_sgn(b->neginf)>0){
    boundXXX_set_infty(a->neginf,+1);
  }
  else {
    boundXXX_set_int(a->neginf,0);
  }
  if (boundXXX_sgn(b->sup)>0){
    boundXXX_set_infty(a->sup,+1);
  }
  else {
    boundXXX_set_int(a->sup,0);
  }
}

void itvXXX_div(itvXXX_t a, itvXXX_t b, itvXXX_t c, num_internal_t intern)
{
  assert(!itvXXX_is_bottom(b) && !itvXXX_is_bottom(c));
  int scneginf = boundXXX_sgn(c->neginf);
  if (scneginf<0){
    /* c is positive */
    itvXXX_divp(a,b,c, intern);
  }
  else if (scneginf==0){
    /* c is [0,xxx] */
    itvXXX_div_zero(a,b);
  }
  else {
    int scsup = boundXXX_sgn(c->sup);
    if (scsup<0){
      /* c is negative */
      itvXXX_divn(a,b,c, intern);
    }
    else if (scsup==0){
      /* c is [xxx,O] with xxx < 0 */
      itvXXX_div_zero(a,b);
      boundXXX_swap(a->neginf,a->sup);
    }
    else if (itvXXX_is_zero(b)){
      /* b is [0,0] */
      itvXXX_set(a,b);
    }
    else {
      itvXXX_set_top(a);
    }
  }
#ifndef NDEBUG
  boundXXX_neg(a->neginf,a->neginf);
  if (boundXXX_cmp(a->neginf,a->sup)>0)
    abort();
  else
    boundXXX_neg(a->neginf,a->neginf);
#endif
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */

void itvXXX_fprint(FILE* stream, itvXXX_t a)
{
  if (itvXXX_is_point(a)){
    boundXXX_fprint(stream,a->sup);
  }
  else {
    fprintf(stream,"[");
    boundXXX_neg(a->neginf,a->neginf);
    boundXXX_fprint(stream,a->neginf);
    boundXXX_neg(a->neginf,a->neginf);
    fprintf(stream,",");
    boundXXX_fprint(stream,a->sup);
    fprintf(stream,"]");
  }
}
void itvXXX_print(itvXXX_t itv){
  itvXXX_fprint(stdout,itv); fflush(stdout);
}
int itvXXX_snprint(char* s, int size, itvXXX_t a)
{
  int res;

  if (itvXXX_is_point(a)){
    res = boundXXX_snprint(s,size,a->sup);
  }
  else {
    res = ap_snprintf(s,size,"[");
    boundXXX_neg(a->neginf,a->neginf);
    res += boundXXX_snprint(s+res,size-res,a->neginf);
    boundXXX_neg(a->neginf,a->neginf);
    res += ap_snprintf(s+res,size,",");
    res += boundXXX_snprint(s+res,size,a->sup);
    res += ap_snprintf(s+res,size,"]");
  }
  return res;
}

/* ====================================================================== */
/* Serialization */
/* ====================================================================== */

size_t itvXXX_serialize(void* dst, itvXXX_t src)
{
  size_t res = boundXXX_serialize(dst,src->neginf);
  res += boundXXX_serialize((char*)dst+res,src->sup);
  return res;
}
size_t itvXXX_deserialize(itvXXX_t dst, const void* src)
{
  size_t res = boundXXX_deserialize(dst->neginf,src);
  res += boundXXX_deserialize(dst->sup,(const char*)src+res);
  return res;
}
size_t itvXXX_serialized_size(itvXXX_t a)
{
  return boundXXX_serialized_size(a->neginf) + boundXXX_serialized_size(a->sup);
}

size_t itvXXX_serialize_array(void* dst, itvXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += itvXXX_serialize((char*)dst+n,src[i]);
  return n;
}

size_t itvXXX_deserialize_array(itvXXX_t* dst, const void* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += itvXXX_deserialize(dst[i],(const char*)src+n);
  return n;
}

size_t itvXXX_serialized_size_array(itvXXX_t* src, size_t size)
{
  size_t i,n=0;
  for (i=0;i<size;i++)
    n += itvXXX_serialized_size(src[i]);
  return n;
}
