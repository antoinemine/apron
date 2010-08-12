#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "itvXXX.h"

/* ********************************************************************** */
/* FPU init */
/* ********************************************************************** */

/* simple run-time test that fpu behaves correctly */
static bool test_fpu(void)
{
  int i;
  long double d = 1., dd;
  /* find the minimal long double, as the fixpoint of x -> x/2 with rounding
     towards +oo;
     the max iteration value should be enough for 128-bit floating-point */
  for (i=0;i<5000000;i++) {
    dd = d;
    d /= 2;
    if (d==dd || d==0.) break;
  }
  /* fails if flush to 0 */
  if (d!=dd) { fprintf(stderr,"test_fpu failed test #1 after %i iterations\n",i); return false; }
  /* fails if long double rounding is not towards +oo */
  if (d*0.25!=dd) { fprintf(stderr,"test_fpu failed test #2\n"); return false; }
  /* fails if double rounding is not towards +oo */
  if ((double)d<dd) { fprintf(stderr,"test_fpu failed test #3\n"); return false; }
  /* fails if float rounding is not towards +oo */
  if ((float)d<dd) { fprintf(stderr,"test_fpu failed test #4\n"); return false; }
  return true;
}

#if defined(__ppc__)
static bool numXXX_fpu_init(void)
{
  __asm volatile ("mtfsfi 7,2");
  return test_fpu();
}

#elif defined(__linux) || defined (__APPLE__)
#include <fenv.h>
static bool numXXX_fpu_init(void)
{
  if (!fesetround(FE_UPWARD)) return test_fpu();
  fprintf(stderr,"could not set fpu rounding mode: fesetround failed\n");
  return false;
}

#elif defined(__FreeBSD__) || defined(sun)
#include <ieeefp.h>
static bool numXXX_fpu_init(void)
{
  fpsetround(FP_RP);
  return test_fpu();
}

#else
static bool numXXX_fpu_init(void)
{
  fprintf(stderr,"could not set fpu rounding mode: platform not supported\n");
  return false;
}

#endif

void arith(itvXXX_internal_t intern,
	   itvXXX_t a, itvXXX_t b, itvXXX_t c, boundXXX_t bound)
{
  itvXXX_t bb,cc;

  itvXXX_init_set(bb,b);
  itvXXX_init_set(cc,c);

  printf("********************\n");
  printf("b="); itvXXX_print(b);
  printf(" c="); itvXXX_print(c);
  printf(" bound="); boundXXX_print(bound); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_swap(b,c);
  printf("itvXXX_swap(b,c): "); printf("b="); itvXXX_print(b); printf(" c="); itvXXX_print(c); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_neg(a,b);
  printf("itvXXX_neg(b)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_add(a,b,c);
  printf("itvXXX_add(b,c)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_sub(a,b,c);
  printf("itvXXX_sub(b,c)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_mul(intern,a,b,c);
  printf("itvXXX_mul(b,c)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_add_bound(a,b,bound);
  printf("itvXXX_add_bound(b,bound)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_sub_bound(a,b,bound);
  printf("itvXXX_sub_bound(b,bound)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_mul_bound(a,b,bound);
  printf("itvXXX_mul_bound(b,bound)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);
  itvXXX_div_bound(a,b,bound);
  printf("itvXXX_div_bound(b,bound)="); itvXXX_print(a); printf("\n");

  itvXXX_set(b,bb); itvXXX_set(c,cc);

  itvXXX_clear(bb);
  itvXXX_clear(cc);
}



int main(int argc, char**argv)
{
  itvXXX_t a,b,c;
  boundXXX_t bound;
  itvXXX_internal_t intern;

  numXXX_fpu_init();
  mpfr_set_default_prec(4046);

  intern = itvXXX_internal_alloc();
  itvXXX_init(a); itvXXX_init(b); itvXXX_init(c); boundXXX_init(bound);

  /* Positive or negative intervals */
  boundXXX_set_int(b->neginf,-3); boundXXX_set_int(b->sup,5);
  boundXXX_set_int(c->neginf,-1); boundXXX_set_int(c->sup,5);
  boundXXX_set_int(bound,4);
  arith(intern,a,b,c,bound);
  itvXXX_neg(c,c);
  arith(intern,a,b,c,bound);
  itvXXX_neg(b,b);
  arith(intern,a,b,c,bound);
  itvXXX_neg(c,c);
  arith(intern,a,b,c,bound);
  /* general intervals */
  boundXXX_set_int(b->neginf,3); boundXXX_set_int(b->sup,5);
  boundXXX_set_int(c->neginf,7); boundXXX_set_int(c->sup,11);
  boundXXX_set_int(bound,3);
  arith(intern,a,b,c,bound);
  boundXXX_set_int(bound,-3);
  arith(intern,a,b,c,bound);

  /* aliases */
  boundXXX_set_int(b->neginf,3); boundXXX_set_int(b->sup,5);
  boundXXX_set_int(c->neginf,7); boundXXX_set_int(c->sup,11);
  boundXXX_set_int(bound,3);
  arith(intern,b,b,b,bound);
  boundXXX_set_int(bound,-3);
  arith(intern,b,b,b,bound);

  itvXXX_clear(a);
  itvXXX_clear(b);
  itvXXX_clear(c);
  boundXXX_clear(bound);
  itvXXX_internal_free(intern);
}
