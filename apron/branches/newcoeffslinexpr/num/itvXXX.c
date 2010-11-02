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
bool itvXXX_canonicalize(itvXXX_t a, bool integer,itv_internal_t intern)
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
  if (numXXX_sgn(c)>=0){
    boundXXX_mul_num(a->sup,b->sup,c);
    boundXXX_mul_num(a->neginf,b->neginf,c);
  } else {
    numXXX_neg(c,c);
    boundXXX_mul_num(a->sup,b->sup,c);
    boundXXX_mul_num(a->neginf,b->neginf,c);
    boundXXX_swap(a->neginf,a->sup);
    numXXX_neg(c,c);
  }
}

void itvXXX_mul_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  if (boundXXX_sgn(c)>=0){
    boundXXX_mul(a->sup,b->sup,c);
    boundXXX_mul(a->neginf,b->neginf,c);
  } else {
    boundXXX_neg(c,c);
    boundXXX_mul(a->sup,b->sup,c);
    boundXXX_mul(a->neginf,b->neginf,c);
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(c,c);
  }
}

void itvXXX_div_num(itvXXX_t a, itvXXX_t b, numXXX_t c)
{
  if (numXXX_sgn(c)>=0){
    boundXXX_div_num(a->sup,b->sup,c);
    boundXXX_div_num(a->neginf,b->neginf,c);
  } else {
    numXXX_neg(c,c);
    boundXXX_div_num(a->sup,b->sup,c);
    boundXXX_div_num(a->neginf,b->neginf,c);
    boundXXX_swap(a->neginf,a->sup);
    numXXX_neg(c,c);
  }
}
void itvXXX_div_bound(itvXXX_t a, itvXXX_t b, boundXXX_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  if (boundXXX_sgn(c)>=0){
    boundXXX_div(a->sup,b->sup,c);
    boundXXX_div(a->neginf,b->neginf,c);
  } else {
    boundXXX_neg(c,c);
    boundXXX_div(a->sup,b->sup,c);
    boundXXX_div(a->neginf,b->neginf,c);
    boundXXX_swap(a->neginf,a->sup);
    boundXXX_neg(c,c);
  }
}
void itvXXX_sub(itvXXX_t a, itvXXX_t b, itvXXX_t c)
{
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
  if (a!=b){
    boundXXX_set(a->neginf,b->sup);
    boundXXX_set(a->sup,b->neginf);
  } else {
    boundXXX_swap(a->neginf,a->sup);
  }
}

bool itvXXX_sqrt(itvXXX_t a, itvXXX_t b, itv_internal_t intern)
{
  bool exact = true;
  if (itvXXX_is_bottom(b, intern) || boundXXX_sgn(b->sup)<0) {
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

void itvXXX_mod(itvXXX_t a, itvXXX_t b, itvXXX_t c,
		     bool is_int, itv_internal_t intern)
{
  /* b-|c|*trunc(b/|c|) */
  itvXXX_abs(intern->XXX->eval_itv, c);
  if (!boundXXX_sgn(intern->XXX->eval_itv->neginf)) itvXXX_set_top(a);
  else {
    itvXXX_div(intern->XXX->eval_itv2, b, intern->XXX->eval_itv, intern);
    itvXXX_trunc(intern->XXX->eval_itv2, intern->XXX->eval_itv2);
    itvXXX_mul(intern->XXX->eval_itv2, intern->XXX->eval_itv2, intern->XXX->eval_itv, intern);
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
    itvXXX_meet(a, a, intern->XXX->eval_itv, intern);
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
		  itv_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<=0);
  boundXXX_neg(intern->XXX->mul_bound,c->neginf);
  boundXXX_mul(a->neginf,b->neginf,intern->XXX->mul_bound);
  boundXXX_mul(a->sup,b->sup,c->sup);
}
/* Assume that both intervals are negative */
static
void itvXXX_mulnn(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 itv_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_neg(intern->XXX->mul_bound,c->sup);
  boundXXX_mul(intern->XXX->mul_bound,b->sup,intern->XXX->mul_bound);
  boundXXX_mul(a->sup,b->neginf,c->neginf);
  boundXXX_set(a->neginf,intern->XXX->mul_bound);
}
/* Assume that b is positive and c negative */
static
void itvXXX_mulpn(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 itv_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<=0);
  boundXXX_neg(intern->XXX->mul_bound,b->neginf);
  boundXXX_mul(a->neginf,b->sup,c->neginf);
  boundXXX_mul(a->sup,intern->XXX->mul_bound,c->sup);
}
/* Assume that interval c is positive */
static
void itvXXX_mulp(itvXXX_t a,
		 itvXXX_t b,
		 itvXXX_t c,
		 itv_internal_t intern)
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
		 itv_internal_t intern)
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

void itvXXX_mul(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
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
    boundXXX_set(intern->XXX->mul_itv->neginf,c->neginf);
    boundXXX_set_int(intern->XXX->mul_itv->sup,0);
    itvXXX_muln(intern->XXX->mul_itv2,b,intern->XXX->mul_itv, intern);

    boundXXX_set_int(intern->XXX->mul_itv->neginf,0);
    boundXXX_set(intern->XXX->mul_itv->sup,c->sup);
    itvXXX_mulp(a,b,intern->XXX->mul_itv, intern);

    itvXXX_join(a,a,intern->XXX->mul_itv2);
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itvXXX_divpp(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_neg(intern->XXX->mul_bound,c->neginf);
  boundXXX_div(a->neginf,b->neginf,c->sup);
  boundXXX_div(a->sup,b->sup,intern->XXX->mul_bound);
}
/* Assume that both intervals are negative */
static
void itvXXX_divnn(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->sup)<0);
  boundXXX_neg(intern->XXX->mul_bound,b->neginf);
  boundXXX_div(a->neginf,b->sup,c->neginf);
  boundXXX_div(a->sup,intern->XXX->mul_bound,c->sup);
}
/* Assume that b is positive and c negative */
static
void itvXXX_divpn(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
  assert(boundXXX_sgn(b->neginf)<=0 && boundXXX_sgn(c->sup)<0);
  boundXXX_neg(intern->XXX->mul_bound,b->sup);
  boundXXX_div(intern->XXX->mul_bound,intern->XXX->mul_bound,c->sup);
  boundXXX_div(a->sup,b->neginf,c->neginf);
  boundXXX_set(a->neginf,intern->XXX->mul_bound);
}
/* Assume that b is negative and c positive */
static
void itvXXX_divnp(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
  assert(boundXXX_sgn(b->sup)<=0 && boundXXX_sgn(c->neginf)<0);
  boundXXX_neg(intern->XXX->mul_bound, b->neginf);
  boundXXX_div(a->neginf,intern->XXX->mul_bound,c->neginf);
  boundXXX_div(a->sup,b->sup,c->sup);
}

/* Assume that interval c is positive */
static
void itvXXX_divp(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
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
    boundXXX_neg(intern->XXX->mul_bound,c->neginf);
    boundXXX_div(a->neginf,b->neginf,intern->XXX->mul_bound);
    boundXXX_div(a->sup,b->sup,intern->XXX->mul_bound);
  }
}
/* Assume that interval c is negative */
static
void itvXXX_divn(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
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
    boundXXX_neg(intern->XXX->mul_bound,c->sup);
    boundXXX_div(a->neginf,b->sup,intern->XXX->mul_bound);
    boundXXX_div(a->sup,b->neginf,intern->XXX->mul_bound);
  }
}

void itvXXX_div(itvXXX_t a, itvXXX_t b, itvXXX_t c, itv_internal_t intern)
{
  if (boundXXX_sgn(c->neginf)<0){
    /* c is positive */
    itvXXX_divp(a,b,c, intern);
  }
  else if (boundXXX_sgn(c->sup)<0){
    /* c is negative */
    itvXXX_divn(a,b,c, intern);
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
