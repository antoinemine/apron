#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "eitvXXX.h"

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

void arith(num_internal_t intern,
	   eitvXXX_t a, eitvXXX_t b, eitvXXX_t c, boundXXX_t bound)
{
  eitvXXX_t bb,cc;

  eitvXXX_init_set(bb,b);
  eitvXXX_init_set(cc,c);

  printf("********************\n");
  printf("b="); eitvXXX_print(b);
  printf(" c="); eitvXXX_print(c);
  printf(" bound="); boundXXX_print(bound); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_swap(b,c);
  printf("eitvXXX_swap(b,c): "); printf("b="); eitvXXX_print(b); printf(" c="); eitvXXX_print(c); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_neg(a,b);
  printf("eitvXXX_neg(b)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_add(a,b,c);
  printf("eitvXXX_add(b,c)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_sub(a,b,c);
  printf("eitvXXX_sub(b,c)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_mul(a,b,c, intern);
  printf("eitvXXX_mul(b,c)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_add_bound(a,b,bound);
  printf("eitvXXX_add_bound(b,bound)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_sub_bound(a,b,bound);
  printf("eitvXXX_sub_bound(b,bound)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_mul_bound(a,b,bound);
  printf("eitvXXX_mul_bound(b,bound)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);
  eitvXXX_div_bound(a,b,bound);
  printf("eitvXXX_div_bound(b,bound)="); eitvXXX_print(a); printf("\n");

  eitvXXX_set(b,bb); eitvXXX_set(c,cc);

  eitvXXX_clear(bb);
  eitvXXX_clear(cc);
}



int main(int argc, char**argv)
{
  eitvXXX_t a,b,c;
  boundXXX_t bound;
  num_internal_ptr intern;

  numXXX_fpu_init();
  mpfr_set_default_prec(4046);

  intern = num_internal_alloc();
  eitvXXX_init(a); eitvXXX_init(b); eitvXXX_init(c); boundXXX_init(bound);

  /* Positive or negative intervals */
  boundXXX_set_int(b->neginf,-3); boundXXX_set_int(b->sup,5);
  boundXXX_set_int(c->neginf,-1); boundXXX_set_int(c->sup,5);
  boundXXX_set_int(bound,4);
  arith(intern,a,b,c,bound);
  eitvXXX_neg(c,c);
  arith(intern,a,b,c,bound);
  eitvXXX_neg(b,b);
  arith(intern,a,b,c,bound);
  eitvXXX_neg(c,c);
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

  eitvXXX_clear(a);
  eitvXXX_clear(b);
  eitvXXX_clear(c);
  boundXXX_clear(bound);
  num_internal_free(intern);
}
