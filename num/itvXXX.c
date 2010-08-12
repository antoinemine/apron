/* ********************************************************************** */
/* itvXXX.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "itvXXX.h"
#include "eitvXXX.h"
#include "math.h"

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool itvXXX_canonicalize(itv_internal_t intern,
		      itvXXX_t a, bool integer)
{
  bool exc;

  if (integer){
    boundXXX_floor(a->neginf,a->neginf);
    boundXXX_floor(a->sup,a->sup);
  }
  if (boundXXX_infty(a->neginf) || boundXXX_infty(a->sup)) return false;

  /* Check that it is not bottom */
  exc = false;
  numXXX_neg(intern->XXX->canonicalize_num,boundXXX_numref(a->neginf));
  if (boundXXX_cmp_num(a->sup,intern->XXX->canonicalize_num) < 0)
    exc = true;
  return exc;
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

/* ********************************************************************** */
/* Arithmetic operations */
/* ********************************************************************** */

/* We assume no aliasing between

   - an itv and a num or a bound,
*/

void itvXXX_mul_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  boundXXX_mul_num(a->sup,b->sup,c);
  boundXXX_mul_num(a->neginf,b->neginf,c);
  if (numXXX_sgn(c)<0){
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_neg(a->neginf,a->neginf);
  }
}

void itvXXX_mul_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  boundXXX_mul(a->sup,b->sup,c);
  boundXXX_mul(a->neginf,b->neginf,c);
  if (boundXXX_sgn(c)<0){
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_neg(a->neginf,a->neginf);
  }
}

void itvXXX_div_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  boundXXX_div_num(a->sup,b->sup,c);
  boundXXX_div_num(a->neginf,b->neginf,c);
  if (numXXX_sgn(c)<0){
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_neg(a->neginf,a->neginf);
  }
}
void itvXXX_div_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  boundXXX_div(a->sup,b->sup,c);
  boundXXX_div(a->neginf,b->neginf,c);
  if (boundXXX_sgn(c)<0){
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_neg(a->neginf,a->neginf);
  }
}
void itvXXX_sub(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  if (a!=c){
    boundXXX_add(a->neginf,b->neginf,c->sup);
    boundXXX_add(a->sup,b->sup,c->neginf);
  } else {
    boundXXX_swap(a->neginf,a->sup);
    itvXXX_add(a,b,c);
  }
}
void itvXXX_neg(itvXXX_t a, itvXXX_t b)
{
  if (a!=b){
    boundXXX_set(a->neginf,b->sup);
    boundXXX_set(a->sup,b->neginf);
  } else {
    boundXXX_swap(a->neginf,a->sup);
  }
}

bool itvXXX_sqrt(itv_internal_t intern, itvXXX_t a, itvXXX_t b)
{
  bool exact = true;
  if (itvXXX_is_bottom(intern,b) || boundXXX_sgn(b->sup)<0) {
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
    boundXXX_sqrt(intern->XXX->sqrt_bound,a->neginf,b->neginf);
    exact = exact && boundXXX_equal(intern->XXX->sqrt_bound,a->neginf);
    boundXXX_neg(b->neginf,b->neginf);
    if (a!=b) boundXXX_neg(a->neginf,a->neginf);
  }
  /* upper bound */
  boundXXX_sqrt(a->sup,intern->XXX->sqrt_bound,b->sup);
  exact = exact && boundXXX_equal(intern->XXX->sqrt_bound,a->sup);
  return exact;
}

void itvXXX_abs(itvXXX_t a, itvXXX_t b)
{
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

void itvXXX_mod(itv_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c,
		     bool is_int)
{
  /* b-|c|*trunc(b/|c|) */
  itvXXX_abs(intern->XXX->eval_itv, c);
  if (!boundXXX_sgn(intern->XXX->eval_itv->neginf)) itvXXX_set_top(a);
  else {
    itvXXX_div(intern, intern->XXX->eval_itv2, b, intern->XXX->eval_itv);
    itvXXX_trunc(intern->XXX->eval_itv2, intern->XXX->eval_itv2);
    itvXXX_mul(intern, intern->XXX->eval_itv2, intern->XXX->eval_itv2, intern->XXX->eval_itv);
    if (is_int) boundXXX_sub_uint(intern->XXX->eval_itv->sup,intern->XXX->eval_itv->sup,1);
    if (boundXXX_sgn(b->sup)<0) {
      /* [-max|c|,0] */
      boundXXX_set(intern->XXX->eval_itv->neginf, intern->XXX->eval_itv->sup);
      boundXXX_set_int(intern->XXX->eval_itv->sup, 0);
    }
    else if (boundXXX_sgn(b->neginf)>0)
      /* [-max|c|,max|c|] */
      boundXXX_set(intern->XXX->eval_itv->neginf, intern->XXX->eval_itv->sup);
    else
      /* [0,max|c|] */
      boundXXX_set_int(intern->XXX->eval_itv->neginf, 0);
    itvXXX_sub(a, b, intern->XXX->eval_itv2);
    itvXXX_meet(intern, a, a, intern->XXX->eval_itv);
  }
}


/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itvXXX_mulpp(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<=0);
  boundXXX_mul(a->neginf,b->neginf,c->neginf);
  boundXXX_neg(a->neginf,a->neginf);
  boundXXX_mul(a->sup,b->sup,c->sup);
}
/* Assume that both intervals are negative */
static
void itvXXX_mulnn(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_mul(a->neginf,b->neginf,c->neginf);
  boundXXX_mul(a->sup,b->sup,c->sup);
  boundXXX_neg(a->sup,a->sup);
  boundXXX_swap(a->neginf,a->sup);
}
/* Assume that b is positive and c negative */
static
void itvXXX_mulpn(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_mul(intern->XXX->mul_bound,b->neginf,c->sup);
  boundXXX_mul(a->neginf,b->sup,c->neginf);
  boundXXX_neg(a->sup,intern->XXX->mul_bound);
}
/* Assume that interval c is positive */
static
void itvXXX_mulp(itv_internal_t intern,
	      itvXXX_t a,
	      itvXXX_t b,
	      itvXXX_t c)
{
  assert(boundXXX_sgn(c->neginf)<=0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_mulpp(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_mulpn(intern,a,c,b);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->sup */
    boundXXX_mul(a->neginf,b->neginf,c->sup);
    boundXXX_mul(a->sup,b->sup,c->sup);
  }
}
/* Assume that interval c is negative */
static
void itvXXX_muln(itv_internal_t intern,
	      itvXXX_t a,
	      itvXXX_t b,
	      itvXXX_t c)
{
  assert(boundXXX_sgn(c->sup)<=0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_mulpn(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_mulnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->neginf */
    boundXXX_mul(a->sup,b->sup,c->neginf);
    boundXXX_mul(a->neginf,b->neginf,c->neginf);
    boundXXX_swap(a->neginf,a->sup);
  }
}

void itvXXX_mul(itv_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  if (boundXXX_sgn(c->neginf)<=0){
    /* c is positive, */
    itvXXX_mulp(intern,a,b,c);
  }
  else if (boundXXX_sgn(c->sup)<=0){
    /* c is negative */
    itvXXX_muln(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive, */
    itvXXX_mulp(intern,a,c,b);
  }
  else if (boundXXX_sgn(c->sup)<=0){
    /* b is negative */
    itvXXX_muln(intern,a,c,b);
  }
  else {
    /* divide c */
    boundXXX_set(intern->XXX->mul_itv->neginf,c->neginf);
    boundXXX_set_int(intern->XXX->mul_itv->sup,0);
    itvXXX_muln(intern,intern->XXX->mul_itv2,b,intern->XXX->mul_itv);

    boundXXX_set_int(intern->XXX->mul_itv->neginf,0);
    boundXXX_set(intern->XXX->mul_itv->sup,c->sup);
    itvXXX_mulp(intern,a,b,intern->XXX->mul_itv);

    itvXXX_join(a,a,intern->XXX->mul_itv2);
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itvXXX_divpp(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_div(intern->XXX->mul_bound,b->sup,c->neginf);
  boundXXX_div(a->neginf,b->neginf,c->sup);
  boundXXX_neg(a->sup,intern->XXX->mul_bound);
}
/* Assume that both intervals are negative */
static
void itvXXX_divnn(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<0);
  if (a!=b){
    boundXXX_div(a->neginf,b->sup,c->neginf);
    boundXXX_div(a->sup,b->neginf,c->sup);
    boundXXX_neg(a->sup,a->sup);
  } else {
    boundXXX_div(intern->XXX->mul_bound,b->sup,c->neginf);
    boundXXX_div(a->sup,b->neginf,c->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_set(a->neginf,intern->XXX->mul_bound);
  }
}
/* Assume that b is positive and c negative */
static
void itvXXX_divpn(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<0);
  /*
     boundXXX_div(a->neginf,b->sup,c->sup);
     boundXXX_neg(a->neginf,a->neginf)
     boundXXX_div(a->sup,b->neginf,c->neginf);
  */
  boundXXX_div(a->sup,b->sup,c->sup);
  boundXXX_div(a->neginf,b->neginf,c->neginf);
  boundXXX_neg(a->sup,a->sup);
  boundXXX_swap(a->neginf,a->sup);
}
/* Assume that b is negative and c positive */
static
void itvXXX_divnp(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_neg(b->neginf, b->neginf);
  boundXXX_div(a->neginf,b->neginf,c->neginf);
  boundXXX_div(a->sup,b->sup,c->sup);
  if (a!=b) boundXXX_neg(b->neginf, b->neginf);
}

/* Assume that interval c is positive */
static
void itvXXX_divp(itv_internal_t intern,
	       itvXXX_t a,
	       itvXXX_t b,
	       itvXXX_t c)
{
  assert(boundXXX_sgn(c->neginf)<0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_divpp(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_divnp(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one divides b by c->neginf */
    boundXXX_div(a->sup,b->sup,c->neginf);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_div(a->neginf,b->neginf,c->neginf);
    boundXXX_neg(a->neginf,a->neginf);
  }
}
/* Assume that interval c is negative */
static
void itvXXX_divn(itv_internal_t intern,
	      itvXXX_t a,
	      itvXXX_t b,
	      itvXXX_t c)
{
  assert(boundXXX_sgn(c->sup)<0);

  if (boundXXX_sgn(b->neginf)<=0){
    /* b is positive */
    itvXXX_divpn(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->sup)<=0){
    /* b is negative */
    itvXXX_divnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one cross-divide b by c->sup */
    /*
      boundXXX_div(a->neginf,b->sup,c->sup);
      boundXXX_div(a->sup,b->neginf,c->sup);
    */
    boundXXX_div(a->neginf,b->neginf,c->sup);
    boundXXX_div(a->sup,b->sup,c->sup);
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(a->neginf,a->neginf);
    boundXXX_neg(a->sup,a->sup);
  }
}

void itvXXX_div(itv_internal_t intern, itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
  if (boundXXX_sgn(c->neginf)<0){
    /* c is positive */
    itvXXX_divp(intern,a,b,c);
  }
  else if (boundXXX_sgn(c->sup)<0){
    /* c is negative */
    itvXXX_divn(intern,a,b,c);
  }
  else if (boundXXX_sgn(b->neginf)==0 && boundXXX_sgn(b->sup)==0){
    /* b is [0,0] */
    itvXXX_set(a,b);
  }
  else {
    itvXXX_set_top(a);
  }
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */

void itvXXX_fprint(FILE* stream, itvXXX_t a)
{
  numXXX_t num;

  fprintf(stream,"[");
  if (boundXXX_infty(a->neginf))
    fprintf(stream,"-oo");
  else {
    numXXX_init(num);
    numXXX_neg(num,boundXXX_numref(a->neginf));
    numXXX_fprint(stream,num);
    numXXX_clear(num);
  }
  fprintf(stream,",");
  boundXXX_fprint(stream,a->sup);
  fprintf(stream,"]");
}
void itvXXX_print(itvXXX_t itv){
  itvXXX_fprint(stdout,itv);
}
int itvXXX_snprint(char* s, size_t size, itvXXX_t a)
{
  numXXX_t num;
  int count = 0;

  count += snprintf(s+count,size-count,"[");
  if (boundXXX_infty(a->neginf))
    count += snprintf(s+count,size-count,"-oo");
  else {
    numXXX_init(num);
    numXXX_neg(num,boundXXX_numref(a->neginf));
    count += numXXX_snprint(s+count,size-count,num);
    numXXX_clear(num);
  }
  count += snprintf(s+count,size-count,",");
  boundXXX_snprint(s+count,size-count,a->sup);
  count += snprintf(s+count,size-count,"]");
  return count;
}
