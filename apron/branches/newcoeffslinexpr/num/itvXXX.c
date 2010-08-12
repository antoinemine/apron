/* ********************************************************************** */
/* itvXXX.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "itvXXX.h"
#include "eitvXXX.h"
#include "math.h"

static void make_itvXXX_float_const(int frac_bits, int exp_bits, int exp_bias,
				 itvXXX_float_const* cst)
{
  boundXXX_t b,c;
  boundXXX_init(b); boundXXX_init(c);
  itvXXX_init(cst->ulp); itvXXX_init(cst->min); itvXXX_init(cst->min_normal);
  itvXXX_init(cst->max); itvXXX_init(cst->max_exact);

  boundXXX_set_int(b,1);
  boundXXX_mul_2exp(b,b,-frac_bits);
  itvXXX_set_unit_bound(cst->ulp,b);

  boundXXX_set_int(b,1);
  boundXXX_mul_2exp(b,b,1-exp_bias-frac_bits);
  itvXXX_set_unit_bound(cst->min,b);

  boundXXX_set_int(b,1);
  boundXXX_mul_2exp(b,b,1-exp_bias);
  itvXXX_set_unit_bound(cst->min_normal,b);

  boundXXX_set_int(b,2);
  boundXXX_set_int(c,1);
  boundXXX_mul_2exp(c,c,-frac_bits);
  boundXXX_sub(b,b,c);
  boundXXX_mul_2exp(b,b,(1<<exp_bits)-2-exp_bias);
  itvXXX_set_unit_bound(cst->max,b);

  boundXXX_set_int(b,1);
  boundXXX_mul_2exp(b,b,frac_bits);
  itvXXX_set_unit_bound(cst->max_exact,b);

  boundXXX_clear(b); boundXXX_clear(c);
}
static void itvXXX_float_const_clear(itvXXX_float_const* cst)
{
  itvXXX_clear(cst->ulp); itvXXX_clear(cst->min); itvXXX_clear(cst->min_normal); itvXXX_clear(cst->max); itvXXX_clear(cst->max_exact);
}

/* ********************************************************************** */
/* Global Initialization */
/* ********************************************************************** */

void itvXXX_internal_init(itvXXX_internal_t intern)
{
  numinternal_init(intern->num);
  numXXX_init(intern->canonicalize_num);
  boundXXX_init(intern->muldiv_bound);
  boundXXX_init(intern->mul_bound);
  boundXXX_init(intern->sqrt_bound);
  boundXXX_init(intern->linear_bound);
  boundXXX_init(intern->linear_bound2);
  boundXXX_init(intern->linear_bound3);
  itvXXX_init(intern->mul_itv);
  itvXXX_init(intern->mul_itv2);
  itvXXX_init(intern->eval_itv);
  itvXXX_init(intern->eval_itv2);
  itvXXX_init(intern->eval_itv3);
  numXXX_init(intern->quasi_num);
  eitvXXX_init(intern->boxize_lincons_eitv);
  itvXXX_init(intern->boxize_lincons_eval);
  boundXXX_init(intern->boxize_lincons_bound);
  mpz_init(intern->reduce_lincons_gcd);
  mpz_init(intern->reduce_lincons_mpz);

  make_itvXXX_float_const(10,5,15,&intern->cst_half);         /* 16-bit */
  make_itvXXX_float_const(23,8,127,&intern->cst_single);      /* 32-bit */
  make_itvXXX_float_const(52,11,1023,&intern->cst_double);    /* 64-bit */
  make_itvXXX_float_const(63,15,16383,&intern->cst_extended); /* 80-bit, no hidden bit */
  make_itvXXX_float_const(112,15,16383,&intern->cst_quad);    /* 128-bit */
  itvXXX_init(intern->itvXXX_half);
  itvXXX_set_int2(intern->itvXXX_half,-1,1);
  itvXXX_mul_2exp(intern->itvXXX_half,intern->itvXXX_half,-1);
}
void itvXXX_internal_clear(itvXXX_internal_t intern)
{
  numinternal_clear(intern->num);
  numXXX_clear(intern->canonicalize_num);
  boundXXX_clear(intern->muldiv_bound);
  boundXXX_clear(intern->mul_bound);
  boundXXX_clear(intern->sqrt_bound);
  boundXXX_clear(intern->linear_bound);
  boundXXX_clear(intern->linear_bound2);
  boundXXX_clear(intern->linear_bound3);
  itvXXX_clear(intern->mul_itv);
  itvXXX_clear(intern->mul_itv2);
  itvXXX_clear(intern->eval_itv);
  itvXXX_clear(intern->eval_itv2);
  itvXXX_clear(intern->eval_itv3);
  numXXX_clear(intern->quasi_num);
  eitvXXX_clear(intern->boxize_lincons_eitv);
  itvXXX_clear(intern->boxize_lincons_eval);
  boundXXX_clear(intern->boxize_lincons_bound);
  mpz_clear(intern->reduce_lincons_gcd);
  mpz_clear(intern->reduce_lincons_mpz);
  itvXXX_float_const_clear(&intern->cst_half);
  itvXXX_float_const_clear(&intern->cst_single);
  itvXXX_float_const_clear(&intern->cst_double);
  itvXXX_float_const_clear(&intern->cst_extended);
  itvXXX_float_const_clear(&intern->cst_quad);
  itvXXX_clear(intern->itvXXX_half);
}

itvXXX_internal_ptr itvXXX_internal_alloc(void)
{
  itvXXX_internal_ptr intern = malloc(sizeof(itvXXX_internal_t));
  itvXXX_internal_init(intern);
  return intern;
}
void itvXXX_internal_free(itvXXX_internal_ptr intern)
{
  itvXXX_internal_clear(intern);
  free(intern);
}

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
  numXXX_neg(intern->canonicalize_num,boundXXX_numref(a->neginf));
  if (boundXXX_cmp_num(a->sup,intern->canonicalize_num) < 0)
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
    boundXXX_sqrt(intern->sqrt_bound,a->neginf,b->neginf);
    exact = exact && boundXXX_equal(intern->sqrt_bound,a->neginf);
    boundXXX_neg(b->neginf,b->neginf);
    if (a!=b) boundXXX_neg(a->neginf,a->neginf);
  }
  /* upper bound */
  boundXXX_sqrt(a->sup,intern->sqrt_bound,b->sup);
  exact = exact && boundXXX_equal(intern->sqrt_bound,a->sup);
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
  itvXXX_abs(intern->eval_itv, c);
  if (!boundXXX_sgn(intern->eval_itv->neginf)) itvXXX_set_top(a);
  else {
    itvXXX_div(intern, intern->eval_itv2, b, intern->eval_itv);
    itvXXX_trunc(intern->eval_itv2, intern->eval_itv2);
    itvXXX_mul(intern, intern->eval_itv2, intern->eval_itv2, intern->eval_itv);
    if (is_int) boundXXX_sub_uint(intern->eval_itv->sup,intern->eval_itv->sup,1);
    if (boundXXX_sgn(b->sup)<0) {
      /* [-max|c|,0] */
      boundXXX_set(intern->eval_itv->neginf, intern->eval_itv->sup);
      boundXXX_set_int(intern->eval_itv->sup, 0);
    }
    else if (boundXXX_sgn(b->neginf)>0)
      /* [-max|c|,max|c|] */
      boundXXX_set(intern->eval_itv->neginf, intern->eval_itv->sup);
    else
      /* [0,max|c|] */
      boundXXX_set_int(intern->eval_itv->neginf, 0);
    itvXXX_sub(a, b, intern->eval_itv2);
    itvXXX_meet(intern, a, a, intern->eval_itv);
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
  boundXXX_mul(intern->mul_bound,b->neginf,c->sup);
  boundXXX_mul(a->neginf,b->sup,c->neginf);
  boundXXX_neg(a->sup,intern->mul_bound);
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
    boundXXX_set(intern->mul_itv->neginf,c->neginf);
    boundXXX_set_int(intern->mul_itv->sup,0);
    itvXXX_muln(intern,intern->mul_itv2,b,intern->mul_itv);

    boundXXX_set_int(intern->mul_itv->neginf,0);
    boundXXX_set(intern->mul_itv->sup,c->sup);
    itvXXX_mulp(intern,a,b,intern->mul_itv);

    itvXXX_join(a,a,intern->mul_itv2);
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
  boundXXX_div(intern->mul_bound,b->sup,c->neginf);
  boundXXX_div(a->neginf,b->neginf,c->sup);
  boundXXX_neg(a->sup,intern->mul_bound);
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
    boundXXX_div(intern->mul_bound,b->sup,c->neginf);
    boundXXX_div(a->sup,b->neginf,c->sup);
    boundXXX_neg(a->sup,a->sup);
    boundXXX_set(a->neginf,intern->mul_bound);
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
