/* ********************************************************************** */
/* itvXXX_types.c: workspace */
/* ********************************************************************** */

#include "itvXXX_types.h"
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
