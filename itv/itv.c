/* ********************************************************************** */
/* itv.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "itv.h"
#include "math.h"

/* The macro ITVFUN(itv_name) (defined in itv.h) expands name
   with itvNUM_SUFFIX_ */

static void make_float_const(int frac_bits, int exp_bits, int exp_bias,
			     float_const* cst)
{
  bound_t b,c;
  bound_init(b); bound_init(c);
  itv_init(cst->ulp); itv_init(cst->min); itv_init(cst->min_normal);
  itv_init(cst->max); itv_init(cst->max_exact);

  bound_set_int(b,1);
  bound_mul_2exp(b,b,-frac_bits);
  itv_set_unit_bound(cst->ulp,b);

  bound_set_int(b,1);
  bound_mul_2exp(b,b,1-exp_bias-frac_bits);
  itv_set_unit_bound(cst->min,b);

  bound_set_int(b,1);
  bound_mul_2exp(b,b,1-exp_bias);
  itv_set_unit_bound(cst->min_normal,b);

  bound_set_int(b,2);
  bound_set_int(c,1);
  bound_mul_2exp(c,c,-frac_bits);
  bound_sub(b,b,c);
  bound_mul_2exp(b,b,(1<<exp_bits)-2-exp_bias);
  itv_set_unit_bound(cst->max,b);

  bound_set_int(b,1);
  bound_mul_2exp(b,b,frac_bits);
  itv_set_unit_bound(cst->max_exact,b);

  bound_clear(b); bound_clear(c);
}
static void float_const_clear(float_const* cst)
{
  itv_clear(cst->ulp); itv_clear(cst->min); itv_clear(cst->min_normal); itv_clear(cst->max); itv_clear(cst->max_exact);
}
void ITVFUN(itv_internal_init)(itv_internal_t* intern)
{
  num_init(intern->canonicalize_num);
  bound_init(intern->muldiv_bound);
  bound_init(intern->mul_bound);
  bound_init(intern->sqrt_bound);
  bound_init(intern->linear_bound);
  bound_init(intern->linear_bound2);
  bound_init(intern->linear_bound3);
  itv_init(intern->mul_itv);
  itv_init(intern->mul_itv2);
  intern->ap_conversion_scalar = ap_scalar_alloc();
  bound_init(intern->ap_conversion_bound);
  itv_init(intern->eval_itv);
  itv_init(intern->eval_itv2);
  itv_init(intern->eval_itv3);
  num_init(intern->quasi_num);
  itv_init(intern->boxize_lincons_itv);
  itv_init(intern->boxize_lincons_eval);
  bound_init(intern->boxize_lincons_bound);
  mpz_init(intern->reduce_lincons_gcd);
  mpz_init(intern->reduce_lincons_mpz);

  make_float_const(10,5,15,&intern->cst_half);         /* 16-bit */
  make_float_const(23,8,127,&intern->cst_single);      /* 32-bit */
  make_float_const(52,11,1023,&intern->cst_double);    /* 64-bit */
  make_float_const(63,15,16383,&intern->cst_extended); /* 80-bit, no hidden bit */
  make_float_const(112,15,16383,&intern->cst_quad);    /* 128-bit */
  itv_init(intern->itv_half);
  itv_set_int2(intern->itv_half,-1,1);
  itv_mul_2exp(intern->itv_half,intern->itv_half,-1);
}
void ITVFUN(itv_internal_clear)(itv_internal_t* intern)
{
  num_clear(intern->canonicalize_num);
  bound_clear(intern->muldiv_bound);
  bound_clear(intern->mul_bound);
  bound_clear(intern->sqrt_bound);
  bound_clear(intern->linear_bound);
  bound_clear(intern->linear_bound2);
  bound_clear(intern->linear_bound3);
  itv_clear(intern->mul_itv);
  itv_clear(intern->mul_itv2);
  ap_scalar_free(intern->ap_conversion_scalar); intern->ap_conversion_scalar = NULL;
  bound_clear(intern->ap_conversion_bound);
  itv_clear(intern->eval_itv);
  itv_clear(intern->eval_itv2);
  itv_clear(intern->eval_itv3);
  num_clear(intern->quasi_num);
  itv_clear(intern->boxize_lincons_itv);
  itv_clear(intern->boxize_lincons_eval);
  bound_clear(intern->boxize_lincons_bound);
  mpz_clear(intern->reduce_lincons_gcd);
  mpz_clear(intern->reduce_lincons_mpz);
  float_const_clear(&intern->cst_half);
  float_const_clear(&intern->cst_single);
  float_const_clear(&intern->cst_double);
  float_const_clear(&intern->cst_extended);
  float_const_clear(&intern->cst_quad);
  itv_clear(intern->itv_half);
}

itv_internal_t* ITVFUN(itv_internal_alloc)(void)
{
  itv_internal_t* intern = malloc(sizeof(itv_internal_t));
  itv_internal_init(intern);
  return intern;
}
void ITVFUN(itv_internal_free)(itv_internal_t* intern)
{
  itv_internal_clear(intern);
  free(intern);
}

/* ********************************************************************** */
/* Normalization and tests */
/* ********************************************************************** */

/* If integer is true, narrow the interval to integer bounds.
   In any case, return true if the interval is bottom
*/
bool ITVFUN(itv_canonicalize)(itv_internal_t* intern,
			      itv_t a, bool integer)
{
  bool exc;

  if (integer){
    bound_floor(a->inf,a->inf);
    bound_floor(a->sup,a->sup);
  }
  if (bound_infty(a->inf) || bound_infty(a->sup)) return false;

  /* Check that it is not bottom */
  exc = false;
  num_neg(intern->canonicalize_num,bound_numref(a->inf));
  if (bound_cmp_num(a->sup,intern->canonicalize_num) < 0)
    exc = true;
  return exc;
}

/* ********************************************************************** */
/* Arithmetic operations */
/* ********************************************************************** */

/* We assume no aliasing between

   - an itv and a num or a bound,
*/

void ITVFUN(itv_mul_num)(itv_t a, itv_t b, num_t c)
{
  if (num_sgn(c)>=0){
    bound_mul_num(a->sup,b->sup,c);
    bound_mul_num(a->inf,b->inf,c);
  } else {
    num_neg(c,c);
    bound_mul_num(a->sup,b->sup,c);
    bound_mul_num(a->inf,b->inf,c);
    bound_swap(a->inf,a->sup);
    num_neg(c,c);
  }
}

void ITVFUN(itv_mul_bound)(itv_t a, itv_t b, bound_t c)
{
  assert (c!=a->inf && c!=a->sup && c!=b->inf && c!=b->sup);
  if (bound_sgn(c)>=0){
    bound_mul(a->sup,b->sup,c);
    bound_mul(a->inf,b->inf,c);
  } else {
    bound_neg(c,c);
    bound_mul(a->sup,b->sup,c);
    bound_mul(a->inf,b->inf,c);
    bound_swap(a->inf,a->sup);
    bound_neg(c,c);
  }
}

void ITVFUN(itv_div_num)(itv_t a, itv_t b, num_t c)
{
  if (num_sgn(c)>=0){
    bound_div_num(a->sup,b->sup,c);
    bound_div_num(a->inf,b->inf,c);
  } else {
    num_neg(c,c);
    bound_div_num(a->sup,b->sup,c);
    bound_div_num(a->inf,b->inf,c);
    bound_swap(a->inf,a->sup);
    num_neg(c,c);
  }
}
void ITVFUN(itv_div_bound)(itv_t a, itv_t b, bound_t c)
{
  assert (c!=a->inf && c!=a->sup && c!=b->inf && c!=b->sup);
  if (bound_sgn(c)>=0){
    bound_div(a->sup,b->sup,c);
    bound_div(a->inf,b->inf,c);
  } else {
    bound_neg(c,c);
    bound_div(a->sup,b->sup,c);
    bound_div(a->inf,b->inf,c);
    bound_swap(a->inf,a->sup);
    bound_neg(c,c);
  }
}
void ITVFUN(itv_sub)(itv_t a, itv_t b, itv_t c)
{
  if (a!=c){
    bound_add(a->inf,b->inf,c->sup);
    bound_add(a->sup,b->sup,c->inf);
  } else if (a!=b) { /* a=c */
    bound_swap(c->sup,c->inf);
    itv_add(a,b,c);
  } else { /* a=b=c */
    bound_add(a->sup,b->sup,c->inf);
    bound_set(a->inf,a->sup);
  }
}
void ITVFUN(itv_neg)(itv_t a, itv_t b)
{
  if (a!=b){
    bound_set(a->inf,b->sup);
    bound_set(a->sup,b->inf);
  } else {
    bound_swap(a->inf,a->sup);
  }
}

bool ITVFUN(itv_sqrt)(itv_internal_t* intern, itv_t a, itv_t b)
{
  bool exact = true;
  if (itv_is_bottom(intern,b) || bound_sgn(b->sup)<0) {
    /* empty result */
    itv_set_bottom(a);
    return true;
  }
  /* lower bound */
  if (bound_sgn(b->inf)>=0) {
    bound_set_int(a->inf,0);
  }
  else {
    bound_neg(b->inf,b->inf);
    bound_sqrt(intern->sqrt_bound,a->inf,b->inf);
    exact = exact && bound_equal(intern->sqrt_bound,a->inf);
    bound_neg(b->inf,b->inf);
    if (a!=b) bound_neg(a->inf,a->inf);
  }
  /* upper bound */
  bound_sqrt(a->sup,intern->sqrt_bound,b->sup);
  exact = exact && bound_equal(intern->sqrt_bound,a->sup);
  return exact;
}

void ITVFUN(itv_abs)(itv_t a, itv_t b)
{
  if (bound_sgn(b->inf)<=0)
    /* positive interval */
    itv_set(a,b);
  else if (bound_sgn(b->sup)<=0)
    /* negative interval */
    itv_neg(a,b);
  else {
    bound_max(a->sup,b->inf,b->sup);
    bound_set_int(a->inf,0);
  }
}

void ITVFUN(itv_mod)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c,
		     bool is_int)
{
  /* b-|c|*trunc(b/|c|) */
  itv_abs(intern->eval_itv, c);
  if (!bound_sgn(intern->eval_itv->inf)) itv_set_top(a);
  else {
    itv_div(intern, intern->eval_itv2, b, intern->eval_itv);
    itv_trunc(intern->eval_itv2, intern->eval_itv2);
    itv_mul(intern, intern->eval_itv2, intern->eval_itv2, intern->eval_itv);
    if (is_int) bound_sub_uint(intern->eval_itv->sup,intern->eval_itv->sup,1);
    if (bound_sgn(b->sup)<0) {
      /* [-max|c|,0] */
      bound_set(intern->eval_itv->inf, intern->eval_itv->sup);
      bound_set_int(intern->eval_itv->sup, 0);
    }
    else if (bound_sgn(b->inf)>0)
      /* [-max|c|,max|c|] */
      bound_set(intern->eval_itv->inf, intern->eval_itv->sup);
    else
      /* [0,max|c|] */
      bound_set_int(intern->eval_itv->inf, 0);
    itv_sub(a, b, intern->eval_itv2);
    itv_meet(intern, a, a, intern->eval_itv);
  }
}


/* ====================================================================== */
/* Multiplication */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itv_mulpp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->inf)<=0 && bound_sgn(c->inf)<=0);
  bound_neg(intern->mul_bound,c->inf);
  bound_mul(a->inf,b->inf,intern->mul_bound);
  bound_mul(a->sup,b->sup,c->sup);
}
/* Assume that both intervals are negative */
static
void itv_mulnn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->sup)<=0 && bound_sgn(c->sup)<=0);
  bound_neg(intern->mul_bound,c->sup);
  bound_mul(intern->mul_bound,b->sup,intern->mul_bound);
  bound_mul(a->sup,b->inf,c->inf);
  bound_set(a->inf,intern->mul_bound);
}
/* Assume that b is positive and c negative */
static
void itv_mulpn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->inf)<=0 && bound_sgn(c->sup)<=0);
  bound_neg(intern->mul_bound,b->inf);
  bound_mul(a->inf,b->sup,c->inf);
  bound_mul(a->sup,intern->mul_bound,c->sup);
}
/* Assume that interval c is positive */
static
void itv_mulp(itv_internal_t* intern,
	      itv_t a,
	      itv_t b,
	      itv_t c)
{
  assert(bound_sgn(c->inf)<=0);

  if (bound_sgn(b->inf)<=0){
    /* b is positive */
    itv_mulpp(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_mulpn(intern,a,c,b);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->sup */
    bound_mul(a->inf,b->inf,c->sup);
    bound_mul(a->sup,b->sup,c->sup);
  }
}
/* Assume that interval c is negative */
static
void itv_muln(itv_internal_t* intern,
	      itv_t a,
	      itv_t b,
	      itv_t c)
{
  assert(bound_sgn(c->sup)<=0);

  if (bound_sgn(b->inf)<=0){
    /* b is positive */
    itv_mulpn(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_mulnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->inf */
    bound_mul(a->sup,b->sup,c->inf);
    bound_mul(a->inf,b->inf,c->inf);
    bound_swap(a->inf,a->sup);
  }
}

void ITVFUN(itv_mul)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  if (bound_sgn(c->inf)<=0){
    /* c is positive, */
    itv_mulp(intern,a,b,c);
  }
  else if (bound_sgn(c->sup)<=0){
    /* c is negative */
    itv_muln(intern,a,b,c);
  }
  else if (bound_sgn(b->inf)<=0){
    /* b is positive, */
    itv_mulp(intern,a,c,b);
  }
  else if (bound_sgn(c->sup)<=0){
    /* b is negative */
    itv_muln(intern,a,c,b);
  }
  else {
    /* divide c */
    bound_set(intern->mul_itv->inf,c->inf);
    bound_set_int(intern->mul_itv->sup,0);
    itv_muln(intern,intern->mul_itv2,b,intern->mul_itv);

    bound_set_int(intern->mul_itv->inf,0);
    bound_set(intern->mul_itv->sup,c->sup);
    itv_mulp(intern,a,b,intern->mul_itv);

    itv_join(a,a,intern->mul_itv2);
  }
}

/* ====================================================================== */
/* Division */
/* ====================================================================== */

/* Assume that both intervals are positive */
static
void itv_divpp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->inf)<=0 && bound_sgn(c->inf)<0);
  bound_neg(intern->mul_bound,c->inf);
  bound_div(a->inf,b->inf,c->sup);
  bound_div(a->sup,b->sup,intern->mul_bound);
}
/* Assume that both intervals are negative */
static
void itv_divnn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->sup)<=0 && bound_sgn(c->sup)<0);
  bound_neg(intern->mul_bound,b->inf);
  bound_div(a->inf,b->sup,c->inf);
  bound_div(a->sup,intern->mul_bound,c->sup);
}
/* Assume that b is positive and c negative */
static
void itv_divpn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->inf)<=0 && bound_sgn(c->sup)<0);
  bound_neg(intern->mul_bound,b->sup);
  bound_div(intern->mul_bound,intern->mul_bound,c->sup);
  bound_div(a->sup,b->inf,c->inf);
  bound_set(a->inf,intern->mul_bound);
}
/* Assume that b is negative and c positive */
static
void itv_divnp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->sup)<=0 && bound_sgn(c->inf)<0);
  bound_neg(intern->mul_bound, b->inf);
  bound_div(a->inf,intern->mul_bound,c->inf);
  bound_div(a->sup,b->sup,c->sup);
}

/* Assume that interval c is positive */
static
void itv_divp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(c->inf)<0);

  if (bound_sgn(b->inf)<=0){
    /* b is positive */
    itv_divpp(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_divnp(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one divides b by c->inf */
    bound_neg(intern->mul_bound,c->inf);
    bound_div(a->inf,b->inf,intern->mul_bound);
    bound_div(a->sup,b->sup,intern->mul_bound);
  }
}
/* Assume that interval c is negative */
static
void itv_divn(itv_internal_t* intern,
	      itv_t a,
	      itv_t b,
	      itv_t c)
{
  assert(bound_sgn(c->sup)<0);

  if (bound_sgn(b->inf)<=0){
    /* b is positive */
    itv_divpn(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_divnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one cross-divide b by c->sup */
    if (a!=b) {
      bound_neg(intern->mul_bound,c->sup);
      bound_div(a->inf,b->sup,intern->mul_bound);
      bound_div(a->sup,b->inf,intern->mul_bound);
    }
    else {
      bound_neg(intern->mul_bound,c->sup);
      bound_div(intern->sqrt_bound,b->sup,intern->mul_bound);
      bound_div(a->sup,b->inf,intern->mul_bound);
      bound_set(a->inf,intern->sqrt_bound);
    }
  }
}

void ITVFUN(itv_div)(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  if (bound_sgn(c->inf)<0){
    /* c is positive */
    itv_divp(intern,a,b,c);
  }
  else if (bound_sgn(c->sup)<0){
    /* c is negative */
    itv_divn(intern,a,b,c);
  }
  else if (bound_sgn(b->inf)==0 && bound_sgn(b->sup)==0){
    /* b is [0,0] */
    itv_set(a,b);
  }
  else {
    itv_set_top(a);
  }
}


/* ********************************************************************** */
/* Power */
/* ********************************************************************** */

void ITVFUN(itv_pow)(itv_internal_t* intern, itv_t a, itv_t b, itv_t n)
{
  long x;
  if (itv_is_bottom(intern, b) || itv_is_bottom(intern, n)) {
    itv_set_bottom(a);
    return;
  }
  /* ensures that the exponent is a singleton */
  bound_neg(intern->mul_bound, n->inf);
  if (bound_infty(n->sup) || bound_cmp(intern->mul_bound, n->sup)) {
    itv_set_top(a);
    return;
  }
  /* ensures that the exponent is a positive integer, stores it in x */
  int_set_num(&x, bound_numref(n->sup));
  bound_set_int(intern->mul_bound, x);
  if (bound_cmp(intern->mul_bound, n->sup) || x < 0) {
    itv_set_top(a);
    return;
  }
  if (x & 1) itv_set(intern->mul_itv, b);
  else itv_abs(intern->mul_itv, b);
  bound_neg(intern->mul_itv->inf, intern->mul_itv->inf);
  bound_pow(a->sup, intern->mul_bound, intern->mul_itv->sup, x);
  bound_pow(intern->mul_bound, a->inf, intern->mul_itv->inf, x);
  bound_neg(a->inf, a->inf);
}

/* inverse of pow, uses the sign of orga to determine the sign of a */
void ITVFUN(itv_inv_pow)(itv_internal_t* intern, itv_t a, itv_t orga, itv_t b, itv_t n)
{
  long x;
  assert(orga != a);
  if (itv_is_bottom(intern, b) || itv_is_bottom(intern, orga) || itv_is_bottom(intern, n)) {
    itv_set_bottom(a);
    return;
  }
  /* ensures that the exponent is a singleton */
  bound_neg(intern->mul_bound, n->inf);
  if (bound_infty(n->sup) || bound_cmp(intern->mul_bound, n->sup)) {
    itv_set_top(a);
    return;
  }
  /* ensures that the exponent is a positive integer, stores it in x */
  int_set_num(&x, bound_numref(n->sup));
  bound_set_int(intern->mul_bound, x);
  if (bound_cmp(intern->mul_bound, n->sup) || x < 0) {
    itv_set_top(a);
    return;
  }
  if (!x) {
    /* special case: 0-th root is undefined */
    itv_set_top(a);
    return;
  }
  if ((x & 1) || (bound_sgn(b->inf) <= 0)) {
    /* keep bound sign */
    bound_set(intern->mul_itv->sup, b->sup);
    bound_neg(intern->mul_itv->inf, b->inf);
  }
  else {
    /* intersect with [0;+oo] */
    if (bound_sgn(b->sup) < 0) {
      itv_set_bottom(a);
      return;
    }
    bound_set(intern->mul_itv->sup, b->sup);
    bound_set_int(intern->mul_itv->inf, 0);
  }
  bound_root(a->sup, intern->mul_bound, intern->mul_itv->sup, x);
  bound_root(intern->mul_bound, a->inf, intern->mul_itv->inf, x);
  bound_neg(a->inf, a->inf);
  if (!(x & 1)) {
    /* fix the sign of a depending on the sign of orga */
    if ((bound_sgn(orga->sup) <= 0)) {
      /* orga is negative -> the result is negative */
      bound_swap(a->inf,a->sup);
    }
    else if (bound_sgn(orga->inf) >= 0) {
      /* orga has unknown sign -> result has unknown sign */
      bound_set(a->inf, a->sup);
    }
  }
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */

void ITVFUN(itv_fprint)(FILE* stream, itv_t a)
{
  num_t num;

  fprintf(stream,"[");
  if (bound_infty(a->inf))
    fprintf(stream,"-oo");
  else {
    num_init(num);
    num_neg(num,bound_numref(a->inf));
    num_fprint(stream,num);
    num_clear(num);
  }
  fprintf(stream,",");
  bound_fprint(stream,a->sup);
  fprintf(stream,"]");
}
void ITVFUN(itv_print)(itv_t a)
{
  ITVFUN(itv_fprint)(stdout,a);
}
int ITVFUN(itv_snprint)(char* s, size_t size, itv_t a)
{
  num_t num;
  int count = 0;

  count += snprintf(s+count,size-count,"[");
  if (bound_infty(a->inf))
    count += snprintf(s+count,size-count,"-oo");
  else {
    num_init(num);
    num_neg(num,bound_numref(a->inf));
    count += num_snprint(s+count,size-count,num);
    num_clear(num);
  }
  count += snprintf(s+count,size-count,",");
  bound_snprint(s+count,size-count,a->sup);
  count += snprintf(s+count,size-count,"]");
  return count;
}

/* ********************************************************************** */
/* Conversions */
/* ********************************************************************** */

bool ITVFUN(itv_set_ap_scalar)(itv_internal_t* intern,
			   itv_t a, ap_scalar_t* b)
{
  assert (ap_scalar_infty(b)==0);
  bool exact = bound_set_ap_scalar(a->sup,b);
  if (exact){
    bound_neg(a->inf,a->sup);
    return true;
  }
  else {
    ap_scalar_neg(intern->ap_conversion_scalar, b);
    bound_set_ap_scalar(a->inf,intern->ap_conversion_scalar);
    return false;
  }
}
bool ITVFUN(itv_set_ap_interval)(itv_internal_t* intern,
			     itv_t a, ap_interval_t* b)
{
  ap_scalar_neg(intern->ap_conversion_scalar, b->inf);
  bool b1 = bound_set_ap_scalar(a->inf,intern->ap_conversion_scalar);
  bool b2 = bound_set_ap_scalar(a->sup,b->sup);
  return b1 && b2;
}
bool ITVFUN(itv_set_ap_coeff)(itv_internal_t* intern,
			  itv_t itv, ap_coeff_t* coeff)
{
  switch(coeff->discr){
  case AP_COEFF_SCALAR:
    return itv_set_ap_scalar(intern,itv,coeff->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    return itv_set_ap_interval(intern, itv, coeff->val.interval);
    break;
  default:
    abort();
  }
}

bool ITVFUN(ap_interval_set_itv)(itv_internal_t* intern,
				   ap_interval_t* a, itv_t b)
{
  (void)intern;
  bool b1 = ap_scalar_set_bound(a->inf,b->inf);
  ap_scalar_neg(a->inf,a->inf);
  bool b2 = ap_scalar_set_bound(a->sup,b->sup);
  return b1 && b2;
}
bool ITVFUN(ap_coeff_set_itv)(itv_internal_t* intern,
				ap_coeff_t* a, itv_t b)
{
  bool exact;

  if (itv_is_point(intern,b)){
    exact = ap_scalar_set_bound(intern->ap_conversion_scalar, b->sup);
    if (exact){
      ap_coeff_set_scalar(a, intern->ap_conversion_scalar);
      return true;
    }
    else {
      ap_coeff_reinit(a,AP_COEFF_INTERVAL,
		      intern->ap_conversion_scalar->discr);
      ap_scalar_set(a->val.interval->sup, intern->ap_conversion_scalar);
      bound_neg(intern->ap_conversion_bound, b->inf); /* we now it is finite */
      ap_scalar_set_bound(a->val.interval->inf, intern->ap_conversion_bound);
      return false;
    }
  }
  else {
    ap_coeff_reinit(a,AP_COEFF_INTERVAL,NUM_AP_SCALAR);
    return ap_interval_set_itv(intern,a->val.interval,b);
  }
}
bool ITVFUN(itv_array_set_ap_interval_array)(itv_internal_t* intern,
					 itv_t** ptitv,
					 ap_interval_t** array,
					 size_t size)
{
  bool exact, res;
  itv_t* titv;
  size_t i;

  titv = itv_array_alloc(size);
  res = true;
  for (i=0; i<size; i++){
    exact = itv_set_ap_interval(intern,titv[i],array[i]);
    res = res && exact;
  }
  *ptitv = titv;
  return res;
}
