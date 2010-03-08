/* ********************************************************************** */
/* itv.c: (unidimensional) intervals */
/* ********************************************************************** */

#include "itv.h"
#include "math.h"

/* The macro itv_name (defined in itv.h) expands name 
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

/* ********************************************************************** */
/* Global Initialization */
/* ********************************************************************** */

void itv_internal_init(itv_internal_t* intern)
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
void itv_internal_clear(itv_internal_t* intern)
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

itv_internal_t* itv_internal_alloc(void)
{
  itv_internal_t* intern = malloc(sizeof(itv_internal_t));
  itv_internal_init(intern);
  return intern;
}
void itv_internal_free(itv_internal_t* intern)
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
bool itv_canonicalize(itv_internal_t* intern,
		      itv_t a, bool integer)
{
  bool exc;

  if (integer){
    bound_floor(a->neginf,a->neginf);
    bound_floor(a->sup,a->sup);
  }
  if (bound_infty(a->neginf) || bound_infty(a->sup)) return false;

  /* Check that it is not bottom */
  exc = false;
  num_neg(intern->canonicalize_num,bound_numref(a->neginf));
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

void itv_mul_num(itv_t a, itv_t b, num_t c)
{
  bound_mul_num(a->sup,b->sup,c);
  bound_mul_num(a->neginf,b->neginf,c);
  if (num_sgn(c)<0){
    bound_swap(a->neginf,a->sup);
    bound_neg(a->sup,a->sup);
    bound_neg(a->neginf,a->neginf);
  }
}

void itv_mul_bound(itv_t a, itv_t b, bound_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  bound_mul(a->sup,b->sup,c);
  bound_mul(a->neginf,b->neginf,c);
  if (bound_sgn(c)<0){
    bound_swap(a->neginf,a->sup);
    bound_neg(a->sup,a->sup);
    bound_neg(a->neginf,a->neginf);
  }
}

void itv_div_num(itv_t a, itv_t b, num_t c)
{
  bound_div_num(a->sup,b->sup,c);
  bound_div_num(a->neginf,b->neginf,c);
  if (num_sgn(c)<0){
    bound_swap(a->neginf,a->sup);
    bound_neg(a->sup,a->sup);
    bound_neg(a->neginf,a->neginf);
  }
}
void itv_div_bound(itv_t a, itv_t b, bound_t c)
{
  assert (c!=a->neginf && c!=a->sup && c!=b->neginf && c!=b->sup);
  bound_div(a->sup,b->sup,c);
  bound_div(a->neginf,b->neginf,c);
  if (bound_sgn(c)<0){
    bound_swap(a->neginf,a->sup);
    bound_neg(a->sup,a->sup);
    bound_neg(a->neginf,a->neginf);
  }
}
void itv_sub(itv_t a, itv_t b, itv_t c)
{
  if (a!=c){
    bound_add(a->neginf,b->neginf,c->sup);
    bound_add(a->sup,b->sup,c->neginf);
  } else {
    bound_swap(a->neginf,a->sup);
    itv_add(a,b,c);
  }
}
void itv_neg(itv_t a, itv_t b)
{
  if (a!=b){
    bound_set(a->neginf,b->sup);
    bound_set(a->sup,b->neginf);
  } else {
    bound_swap(a->neginf,a->sup);
  }
}

bool itv_sqrt(itv_internal_t* intern, itv_t a, itv_t b)
{
  bool exact = true;
  if (itv_is_bottom(intern,b) || bound_sgn(b->sup)<0) {
    /* empty result */
    itv_set_bottom(a);
    return true;
  }
  /* lower bound */
  if (bound_sgn(b->neginf)>=0) {
    bound_set_int(a->neginf,0);
  }
  else {
    bound_neg(b->neginf,b->neginf);
    bound_sqrt(intern->sqrt_bound,a->neginf,b->neginf);
    exact = exact && bound_equal(intern->sqrt_bound,a->neginf);
    bound_neg(b->neginf,b->neginf);
    if (a!=b) bound_neg(a->neginf,a->neginf);
  }
  /* upper bound */
  bound_sqrt(a->sup,intern->sqrt_bound,b->sup);
  exact = exact && bound_equal(intern->sqrt_bound,a->sup);
  return exact;
}

void itv_abs(itv_t a, itv_t b)
{
  if (bound_sgn(b->neginf)<=0)
    /* positive interval */
    itv_set(a,b);
  else if (bound_sgn(b->sup)<=0)
    /* negative interval */
    itv_neg(a,b);
  else {
    bound_max(a->sup,b->neginf,b->sup);
    bound_set_int(a->neginf,0);
  }
}

void itv_mod(itv_internal_t* intern, itv_t a, itv_t b, itv_t c,
		     bool is_int)
{
  /* b-|c|*trunc(b/|c|) */
  itv_abs(intern->eval_itv, c);
  if (!bound_sgn(intern->eval_itv->neginf)) itv_set_top(a);
  else {
    itv_div(intern, intern->eval_itv2, b, intern->eval_itv);
    itv_trunc(intern->eval_itv2, intern->eval_itv2);
    itv_mul(intern, intern->eval_itv2, intern->eval_itv2, intern->eval_itv);
    if (is_int) bound_sub_uint(intern->eval_itv->sup,intern->eval_itv->sup,1);
    if (bound_sgn(b->sup)<0) {
      /* [-max|c|,0] */
      bound_set(intern->eval_itv->neginf, intern->eval_itv->sup);
      bound_set_int(intern->eval_itv->sup, 0);
    }
    else if (bound_sgn(b->neginf)>0)
      /* [-max|c|,max|c|] */
      bound_set(intern->eval_itv->neginf, intern->eval_itv->sup);
    else 
      /* [0,max|c|] */
      bound_set_int(intern->eval_itv->neginf, 0);
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
  assert(bound_sgn(b->neginf)<=0 && bound_sgn(c->neginf)<=0);
  bound_mul(a->neginf,b->neginf,c->neginf);
  bound_neg(a->neginf,a->neginf);
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
  bound_mul(a->neginf,b->neginf,c->neginf);
  bound_mul(a->sup,b->sup,c->sup);
  bound_neg(a->sup,a->sup);
  bound_swap(a->neginf,a->sup);
}
/* Assume that b is positive and c negative */
static
void itv_mulpn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->neginf)<=0 && bound_sgn(c->sup)<=0);
  bound_mul(intern->mul_bound,b->neginf,c->sup);
  bound_mul(a->neginf,b->sup,c->neginf);
  bound_neg(a->sup,intern->mul_bound);
}
/* Assume that interval c is positive */
static
void itv_mulp(itv_internal_t* intern,
	      itv_t a,
	      itv_t b,
	      itv_t c)
{
  assert(bound_sgn(c->neginf)<=0);

  if (bound_sgn(b->neginf)<=0){
    /* b is positive */
    itv_mulpp(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_mulpn(intern,a,c,b);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->sup */
    bound_mul(a->neginf,b->neginf,c->sup);
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

  if (bound_sgn(b->neginf)<=0){
    /* b is positive */
    itv_mulpn(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_mulnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one multiplies b by c->neginf */
    bound_mul(a->sup,b->sup,c->neginf);
    bound_mul(a->neginf,b->neginf,c->neginf);
    bound_swap(a->neginf,a->sup);
  }
}

void itv_mul(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  if (bound_sgn(c->neginf)<=0){
    /* c is positive, */
    itv_mulp(intern,a,b,c);
  }
  else if (bound_sgn(c->sup)<=0){
    /* c is negative */
    itv_muln(intern,a,b,c);
  }
  else if (bound_sgn(b->neginf)<=0){
    /* b is positive, */
    itv_mulp(intern,a,c,b);
  }
  else if (bound_sgn(c->sup)<=0){
    /* b is negative */
    itv_muln(intern,a,c,b);
  }
  else {
    /* divide c */
    bound_set(intern->mul_itv->neginf,c->neginf);
    bound_set_int(intern->mul_itv->sup,0);
    itv_muln(intern,intern->mul_itv2,b,intern->mul_itv);
 
    bound_set_int(intern->mul_itv->neginf,0);
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
  assert(bound_sgn(b->neginf)<=0 && bound_sgn(c->neginf)<0);
  bound_div(intern->mul_bound,b->sup,c->neginf);
  bound_div(a->neginf,b->neginf,c->sup);
  bound_neg(a->sup,intern->mul_bound);
}
/* Assume that both intervals are negative */
static
void itv_divnn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->sup)<=0 && bound_sgn(c->sup)<0);
  if (a!=b){
    bound_div(a->neginf,b->sup,c->neginf);
    bound_div(a->sup,b->neginf,c->sup);
    bound_neg(a->sup,a->sup);
  } else {
    bound_div(intern->mul_bound,b->sup,c->neginf);
    bound_div(a->sup,b->neginf,c->sup);
    bound_neg(a->sup,a->sup);
    bound_set(a->neginf,intern->mul_bound);
  }    
}
/* Assume that b is positive and c negative */
static
void itv_divpn(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->neginf)<=0 && bound_sgn(c->sup)<0);
  /* 
     bound_div(a->neginf,b->sup,c->sup);
     bound_neg(a->neginf,a->neginf)
     bound_div(a->sup,b->neginf,c->neginf);
  */
  bound_div(a->sup,b->sup,c->sup);
  bound_div(a->neginf,b->neginf,c->neginf);
  bound_neg(a->sup,a->sup);
  bound_swap(a->neginf,a->sup);
}
/* Assume that b is negative and c positive */
static
void itv_divnp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(b->sup)<=0 && bound_sgn(c->neginf)<0);
  bound_neg(b->neginf, b->neginf);
  bound_div(a->neginf,b->neginf,c->neginf);
  bound_div(a->sup,b->sup,c->sup);
  if (a!=b) bound_neg(b->neginf, b->neginf);
}

/* Assume that interval c is positive */
static
void itv_divp(itv_internal_t* intern,
	       itv_t a,
	       itv_t b,
	       itv_t c)
{
  assert(bound_sgn(c->neginf)<0);

  if (bound_sgn(b->neginf)<=0){
    /* b is positive */
    itv_divpp(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_divnp(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one divides b by c->neginf */
    bound_div(a->sup,b->sup,c->neginf);
    bound_neg(a->sup,a->sup);
    bound_div(a->neginf,b->neginf,c->neginf);
    bound_neg(a->neginf,a->neginf);
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

  if (bound_sgn(b->neginf)<=0){
    /* b is positive */
    itv_divpn(intern,a,b,c);
  }
  else if (bound_sgn(b->sup)<=0){
    /* b is negative */
    itv_divnn(intern,a,b,c);
  }
  else {
    /* 0 is in the middle of b: one cross-divide b by c->sup */
    /*
      bound_div(a->neginf,b->sup,c->sup);
      bound_div(a->sup,b->neginf,c->sup);
    */
    bound_div(a->neginf,b->neginf,c->sup);
    bound_div(a->sup,b->sup,c->sup);
    bound_swap(a->neginf,a->sup);
    bound_neg(a->neginf,a->neginf);
    bound_neg(a->sup,a->sup);
  }
}

void itv_div(itv_internal_t* intern, itv_t a, itv_t b, itv_t c)
{
  if (bound_sgn(c->neginf)<0){
    /* c is positive */
    itv_divp(intern,a,b,c);
  }
  else if (bound_sgn(c->sup)<0){
    /* c is negative */
    itv_divn(intern,a,b,c);
  }
  else if (bound_sgn(b->neginf)==0 && bound_sgn(b->sup)==0){
    /* b is [0,0] */
    itv_set(a,b);
  }
  else {
    itv_set_top(a);
  }
}

/* ********************************************************************** */
/* Printing */
/* ********************************************************************** */

void itv_fprint(FILE* stream, itv_t a)
{
  num_t num;

  fprintf(stream,"[");
  if (bound_infty(a->neginf))
    fprintf(stream,"-oo");
  else {
    num_init(num);
    num_neg(num,bound_numref(a->neginf));
    num_fprint(stream,num);
    num_clear(num);
  }
  fprintf(stream,",");
  bound_fprint(stream,a->sup);
  fprintf(stream,"]");
}
void itv_print(itv_t itv){ 
  itv_fprint(stdout,itv); 
}
int itv_snprint(char* s, size_t size, itv_t a)
{
  num_t num;
  int count = 0;

  count += snprintf(s+count,size-count,"[");
  if (bound_infty(a->neginf))
    count += snprintf(s+count,size-count,"-oo");
  else {
    num_init(num);
    num_neg(num,bound_numref(a->neginf));
    count += num_snprint(s+count,size-count,num);
    num_clear(num);
  }
  count += snprintf(s+count,size-count,",");
  bound_snprint(s+count,size-count,a->sup);
  count += snprintf(s+count,size-count,"]");
  return count;
}
