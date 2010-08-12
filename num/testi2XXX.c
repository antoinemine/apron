/* Some more itv testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "itvXXX.h"
#include "itvD.h"
#include "itvMPQ.h"
#include "itvMPFR.h"
#include "itvXXX_conv.h"
#include "itvD_conv.h"
#include "itvMPQ_conv.h"
#include "itvMPFR_conv.h"

itvXXX_internal_t intern;

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

void testconv(itvXXX_t x)
{
  itvXXX_t y;
  itvD_t d;
  itvMPQ_t mpq;
  itvMPFR_t mpfr;
  bool exact;

  itvXXX_init(y);
  itvD_init(d);
  itvMPQ_init(mpq);
  itvMPFR_init(mpfr);

  printf("x = "); itvXXX_print(x); printf("\n");

  exact = itvD_set_itvXXX(d,x,intern->num);
  printf("itvD_set_itvXXX(d,x); d = "); itvD_print(d); printf(" exact = %d\n",exact);
  exact = itvXXX_set_itvD(y,d,intern->num);
  printf("itvXXX_set_itvD(y,d); y = "); itvXXX_print(y); printf(" exact = %d\n",exact);

  exact = itvMPQ_set_itvXXX(mpq,x,intern->num);
  printf("itvMPQ_set_itvXXX(mpq,x); mpq = "); itvMPQ_print(mpq); printf(" exact = %d\n",exact);
  exact = itvXXX_set_itvMPQ(y,mpq,intern->num);
  printf("itvXXX_set_itvMPQ(y,mpq); y = "); itvXXX_print(y); printf(" exact = %d\n",exact);

  exact = itvMPFR_set_itvXXX(mpfr,x,intern->num);
  printf("itvMPFR_set_itvXXX(mpfr,x); mpfr = "); itvMPFR_print(mpfr); printf(" exact = %d\n",exact);
  exact = itvXXX_set_itvMPFR(y,mpfr,intern->num);
  printf("itvXXX_set_itvMPFR(y,mpfr); y = "); itvXXX_print(y); printf(" exact = %d\n",exact);

  itvXXX_clear(y);
  itvD_clear(d);
  itvMPQ_clear(mpq);
  itvMPFR_clear(mpfr);
}

void testun(itvXXX_t a)
{
  itvXXX_t b;
  bool exact;
  itvXXX_init(b);
  printf("x = "); itvXXX_print(a); printf("\n");
  itvXXX_neg(b,a); printf("-x = "); itvXXX_print(b); printf("\n");
  itvXXX_abs(b,a); printf("abs(x) = "); itvXXX_print(b); printf("\n");
  exact = itvXXX_sqrt(intern,b,a); printf("sqrt(x) = "); itvXXX_print(b); printf(" (%i)\n",exact);
  itvXXX_ceil(b,a); printf("ceil(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_floor(b,a); printf("floor(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_trunc(b,a); printf("trunc(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_to_int(b,a); printf("int(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_to_float(intern,b,a); printf("float(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_to_double(intern,b,a); printf("double(x) = "); itvXXX_print(b); printf("\n");
  itvXXX_mul_2exp(b,a,2); printf("x << 2 = "); itvXXX_print(b); printf("\n");
  itvXXX_mul_2exp(b,a,-2); printf("x >> 2 = "); itvXXX_print(b); printf("\n");
  printf("\n");
  testconv(a);
  itvXXX_clear(b);
}

void testbin(itvXXX_t a, itvXXX_t b)
{
  itvXXX_t c;
  itvXXX_init(c);
  printf("x = "); itvXXX_print(a); printf("\n");
  printf("y = "); itvXXX_print(b); printf("\n");
  itvXXX_add(c,a,b);
  printf("x + y = "); itvXXX_print(c); printf("\n");
  itvXXX_sub(c,a,b);
  printf("x - y = "); itvXXX_print(c); printf("\n");
  itvXXX_mul(intern,c,a,b);
  printf("x * y = "); itvXXX_print(c); printf("\n");
  itvXXX_div(intern,c,a,b);
  printf("x / y = "); itvXXX_print(c); printf("\n");
  itvXXX_clear(c);
  printf("\n");
}

void testmod(itvXXX_t a, itvXXX_t b)
{
  itvXXX_t c;
  itvXXX_init(c);
  printf("x = "); itvXXX_print(a); printf("\n");
  printf("y = "); itvXXX_print(b); printf("\n");
  itvXXX_mod(intern,c,a,b,1);
  printf("x mod_i y = "); itvXXX_print(c); printf("\n");
  itvXXX_mod(intern,c,a,b,0);
  printf("x mod_f y = "); itvXXX_print(c); printf("\n");
  printf("\n");
  itvXXX_clear(c);
}

void set_double(itvXXX_t a, double inf, double sup)
{
  numXXX_t n;
  numXXX_init(n);
  printf("[%f,%f]\n",inf,sup);
  if (sup==1./0.) boundXXX_set_infty(a->sup,1);
  else { numXXX_set_double(n,sup,intern->num); boundXXX_set_num(a->sup,n); }
  if (inf==-1./0.) boundXXX_set_infty(a->neginf,1);
  else { numXXX_set_double(n,-inf,intern->num); boundXXX_set_num(a->neginf,n); }
  numXXX_clear(n);
}

void set_frac(itvXXX_t a, int ninf, int dinf, int nsup, int dsup)
{
  numXXX_t n;
  numXXX_init(n);
  printf("[%i/%i,%i/%i]\n",ninf,dinf,nsup,dsup);
  numXXX_set_lfrac(n,nsup,dsup,intern->num);  boundXXX_set_num(a->sup,n);
  numXXX_set_lfrac(n,-ninf,dinf,intern->num); boundXXX_set_num(a->neginf,n);
  numXXX_clear(n);
}

int main(void)
{
  itvXXX_t a,b;

  numXXX_fpu_init();
  mpfr_set_default_prec(4046);

  itvXXX_internal_init(intern);
  itvXXX_init(a); itvXXX_init(b);

  set_double(a,2,9); testun(a);
  set_double(a,2.25,9.5); testun(a);
  set_double(a,-9.5,-2.25); testun(a);
  set_double(a,-5.5,7.25); testun(a);
  set_double(a,15,1./0.); testun(a);
  set_double(a,-1./0.,6); testun(a);
  set_double(a,-1./0.,1./0.); testun(a);
  set_frac(a,-1,10,1,100); testun(a);
  set_double(a,1,-2); testun(a);

  set_double(a,1,5); set_double(b,10,10); testmod(a,b);
  set_double(a,15,18); set_double(b,10,10); testmod(a,b);
  set_double(a,2,28); set_double(b,10,10); testmod(a,b);
  set_double(a,-15,28); set_double(b,10,10); testmod(a,b);
  set_double(a,-15,-2); set_double(b,10,10); testmod(a,b);
  set_double(a,-15,-10); set_double(b,10,10); testmod(a,b);
  set_double(a,1,5); set_double(b,12,24); testmod(a,b);
  set_double(a,-4,6); set_double(b,12,24); testmod(a,b);
  set_double(a,0,10); set_double(b,6,8); testmod(a,b);
  set_double(a,0,7); set_double(b,6,8); testmod(a,b);
  set_double(a,2,10); set_double(b,6,8); testmod(a,b);
  set_double(a,1.5,10.5); set_double(b,12.3,14.5); testmod(a,b);

  set_double(a,1,2); set_double(b,3,4); testbin(a,b);
  set_double(a,2,6); set_double(b,-2,-1); testbin(a,b);
  set_double(a,-2,-1); set_double(b,2,6); testbin(a,b);
  set_double(a,-4,-1); set_double(b,-2,-1); testbin(a,b);
  set_double(a,-2,3); set_double(b,0,2); testbin(a,b);
  set_double(a,-2,-1); set_double(b,-2,-1); testbin(a,b);
  set_double(a,-2,-1); set_double(b,-2,0); testbin(a,b);
  set_double(a,-2,3); set_double(b,-1,1); testbin(a,b);


  itvXXX_clear(a); itvXXX_clear(b);
  itvXXX_internal_clear(intern);
  return 0;
}
