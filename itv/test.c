#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "itv.h"

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
static bool num_fpu_init(void)
{
  __asm volatile ("mtfsfi 7,2");
  return test_fpu();
}

#elif defined(__linux) || defined (__APPLE__)
#include <fenv.h>
static bool num_fpu_init(void)
{
  if (!fesetround(FE_UPWARD)) return test_fpu();
  fprintf(stderr,"could not set fpu rounding mode: fesetround failed\n");
  return false;
}

#elif defined(__FreeBSD__) || defined(sun)
#include <ieeefp.h>
static bool num_fpu_init(void)
{
  fpsetround(FP_RP);
  return test_fpu();
}

#else
static bool num_fpu_init(void)
{
  fprintf(stderr,"could not set fpu rounding mode: platform not supported\n");
  return false;
}

#endif

void arith(itv_internal_t* intern,
	   itv_t a, itv_t b, itv_t c, bound_t bound)
{
  itv_t bb,cc;

  itv_init_set(bb,b);
  itv_init_set(cc,c);

  printf("********************\n");
  printf("b="); itv_print(b);
  printf(" c="); itv_print(c);
  printf(" bound="); bound_print(bound); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_swap(b,c);
  printf("itv_swap(b,c): "); printf("b="); itv_print(b); printf(" c="); itv_print(c); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_neg(a,b);
  printf("itv_neg(b)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_add(a,b,c);
  printf("itv_add(b,c)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_sub(a,b,c);
  printf("itv_sub(b,c)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_mul(intern,a,b,c);
  printf("itv_mul(b,c)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_add_bound(a,b,bound);
  printf("itv_add_bound(b,bound)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_sub_bound(a,b,bound);
  printf("itv_sub_bound(b,bound)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_mul_bound(a,b,bound);
  printf("itv_mul_bound(b,bound)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);
  itv_div_bound(a,b,bound);
  printf("itv_div_bound(b,bound)="); itv_print(a); printf("\n");

  itv_set(b,bb); itv_set(c,cc);

  itv_clear(bb);
  itv_clear(cc);
}



int main(int argc, char**argv)
{
  itv_t a,b,c;
  bound_t bound;
  itv_internal_t* intern;

  num_fpu_init();
  mpfr_set_default_prec(4046);

  intern = itv_internal_alloc();
  itv_init(a); itv_init(b); itv_init(c); bound_init(bound);

  /* Positive or negative intervals */
  bound_set_int(b->neginf,-3); bound_set_int(b->sup,5);
  bound_set_int(c->neginf,-1); bound_set_int(c->sup,5);
  bound_set_int(bound,4);
  arith(intern,a,b,c,bound);
  itv_neg(c,c);
  arith(intern,a,b,c,bound);
  itv_neg(b,b);
  arith(intern,a,b,c,bound);
  itv_neg(c,c);
  arith(intern,a,b,c,bound);
  /* general intervals */
  bound_set_int(b->neginf,3); bound_set_int(b->sup,5);
  bound_set_int(c->neginf,7); bound_set_int(c->sup,11);
  bound_set_int(bound,3);
  arith(intern,a,b,c,bound);
  bound_set_int(bound,-3);
  arith(intern,a,b,c,bound);

  /* aliases */
  bound_set_int(b->neginf,3); bound_set_int(b->sup,5);
  bound_set_int(c->neginf,7); bound_set_int(c->sup,11);
  bound_set_int(bound,3);
  arith(intern,b,b,b,bound);
  bound_set_int(bound,-3);
  arith(intern,b,b,b,bound);

  itv_clear(a);
  itv_clear(b);
  itv_clear(c);
  bound_clear(bound);
  itv_internal_free(intern);
}
