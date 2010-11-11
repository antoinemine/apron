#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "boundXXX.h"
#include "numXXX_conv.h"

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

/* ********************************************************************** */
/* Program */
/* ********************************************************************** */
void numXXX(numXXX_t a, numXXX_t b, numXXX_t c,
	 unsigned long int u, long int l,
	 unsigned long long int u2, long long int l2,
	 mpz_t mpz,
	 mpq_t mpq,
	 double d,
	 long double ld,
	 mpfr_t mpfr)
{
  num_internal_t intern;
  numXXX_t bb,cc;
  unsigned long int uu;
  long int ll;
  mpz_t mpzz;
  mpq_t mpqq;
  mpfr_t mpfrr;
  double dd;

  num_internal_init(intern);

  numXXX_init_set(bb,b);
  numXXX_init_set(cc,c);
  ll = l;
  uu = u;
  mpz_init_set(mpzz,mpz);
  mpq_init(mpqq); mpq_set(mpqq,mpq);
  mpfr_init(mpfrr); mpfr_set(mpfrr,mpfr,GMP_RNDU);

  printf("********************\n");
  printf("b="); numXXX_print(b);
  printf(" c="); numXXX_print(c);
  printf("\nu=%lu l=%ld\n",u,l);
  printf("\nu2=%llu l2=%lld\n",u2,l2);
  printf("mpz="); mpz_out_str(stdout,10,mpz);
  printf(" mpq="); mpq_out_str(stdout,10,mpq);
  printf(" mpfr="); mpfr_out_str(stdout,10,10,mpfr,GMP_RNDU);
  printf(" d=%.13e\n",d);
  printf(" d=%.13Le\n",ld);

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_swap(b,c);
  printf("numXXX_swap(b,c): "); printf("b="); numXXX_print(b); printf(" c="); numXXX_print(c); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_neg(a,b);
  printf("numXXX_neg(b)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_add(a,b,c);
  printf("numXXX_add(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_add_uint(a,b,u);
  printf("numXXX_add_uint(b,u)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_sub(a,b,c);
  printf("numXXX_sub(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_sub_uint(a,b,u);
  printf("numXXX_sub_uint(b,u)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_mul(a,b,c);
  printf("numXXX_mul(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_mul_2(a,b);
  printf("numXXX_mul_2(b)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_div(a,b,c);
  printf("numXXX_div(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_div_2(a,b);
  printf("numXXX_div_2(b)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_min(a,b,c);
  printf("numXXX_min(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_max(a,b,c);
  printf("numXXX_max(b,c)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_floor(a,b);
  printf("numXXX_floor(b)="); numXXX_print(a); printf("\n");

  numXXX_set(b,bb); numXXX_set(c,cc);
  numXXX_ceil(a,b);
  printf("numXXX_ceil(b)="); numXXX_print(a); printf("\n");

  printf("numXXX_sgn(b)=%d\n",numXXX_sgn(b));
  printf("numXXX_cmp(b,c)=%d\n",numXXX_cmp(b,c));
  printf("numXXX_cmp_int(b,l)=%d\n",numXXX_cmp_int(b,l));
  printf("numXXX_equal(b,c)=%d\n",numXXX_equal(b,c));

  if (u<=LONG_MAX) {
    numXXX_set_lfrac(a,l,u,intern);
    printf("numXXX_set_lfrac(l,u)=");numXXX_print(a); printf("\n");
  }

  if (u2<=LLONG_MAX) {
    numXXX_set_llfrac(a,l2,u2,intern);
    printf("numXXX_set_llfrac(l2,u2)=");numXXX_print(a); printf("\n");
  }

  printf("mpz_fits_numXXX(mpz)=%d\n",mpz_fits_numXXX(mpz));
  if (mpz_fits_numXXX(mpz)) {
    numXXX_set_mpz(a,mpz,intern);
    printf("numXXX_set_mpz(mpz)=");numXXX_print(a); printf("\n");
  }

  printf("mpq_fits_numXXX(mpq)=%d\n",mpq_fits_numXXX(mpq));
  if (mpq_fits_numXXX(mpq)) {
    numXXX_set_mpq(a,mpq,intern);
    printf("numXXX_set_mpq(mpq)=");numXXX_print(a); printf("\n");
  }

  printf("double_fits_numXXX(d)=%d\n",double_fits_numXXX(d));
  if (double_fits_numXXX(d)) {
    numXXX_set_double(a,d,intern);
    printf("numXXX_set_double(d)=");numXXX_print(a); printf("\n");
  }

  printf("ldouble_fits_numXXX(ld)=%d\n",ldouble_fits_numXXX(d));
  if (ldouble_fits_numXXX(ld)) {
    numXXX_set_ldouble(a,ld,intern);
    printf("numXXX_set_ldouble(ld)=");numXXX_print(a); printf("\n");
  }

  printf("mpfr_fits_numXXX(mpfr)=%d\n",mpfr_fits_numXXX(mpfr,intern));
  if (mpfr_fits_numXXX(mpfr,intern)) {
    numXXX_set_mpfr(a,mpfr,intern);
    printf("numXXX_set_mpfr(mpfr)=");numXXX_print(a); printf("\n");
  }

  {
    long int i;
    lint_set_numXXX(&i,b,intern);
    printf("lint_set_numXXX(b,intern)=%ld\n",i);
  }

  mpz_set_numXXX(mpz,b,intern);
  printf("mpz_set_numXXX(b)="); mpz_out_str(stdout,10,mpz); printf("\n"); mpz_set(mpz,mpzz);

  mpq_set_numXXX(mpq,b,intern);
  printf("mpq_set_numXXX(b)="); mpq_out_str(stdout,10,mpq); printf("\n"); mpq_set(mpq,mpqq);

  printf("numXXX_fits_double(b)=%d\n",numXXX_fits_double(b));
  if (numXXX_fits_double(b)) {
    double k;
    double_set_numXXX(&k,b,intern);
    printf("double(b)=%.13e\n",k);
  }
  numXXX_set(b,bb); numXXX_set(c,cc);

  printf("numXXX_fits_mpfr(b)=%d\n",numXXX_fits_mpfr(b));
  if (numXXX_fits_mpfr(b)) {
    mpfr_set_numXXX(mpfr,b,intern);
    printf("mpfr_set_numXXX(b)="); mpfr_out_str(stdout,10,10,mpfr,GMP_RNDU); printf("\n"); mpfr_set(mpfr,mpfrr,GMP_RNDU);
  }

  numXXX_clear(bb);
  numXXX_clear(cc);
  mpz_clear(mpzz);
  mpq_clear(mpqq);
  mpfr_clear(mpfrr);

  num_internal_clear(intern);
}

void boundXXX(boundXXX_t a, boundXXX_t b, boundXXX_t c,
	   unsigned long int u, long int l,
	   mpz_t mpz,
	   mpq_t mpq,
	   double d,
	   mpfr_t mpfr)
{
  num_internal_t intern;

  boundXXX_t bb,cc;
  unsigned long int uu;
  long int ll;
  double dd;

  num_internal_init(intern);

  boundXXX_init_set(bb,b);
  boundXXX_init_set(cc,c);
  ll = l;
  uu = u;

  printf("********************\n");
  printf("b="); boundXXX_print(b);
  printf(" c="); boundXXX_print(c);
  printf("\nu=%lu l=%ld\n",u,l);
  printf(" d=%.13e\n",d);

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  if (!boundXXX_infty(b)) {
    boundXXX_set_num(a,boundXXX_numref(b));
    printf("boundXXX_set_numXXX(b): "); printf("b="); boundXXX_print(b); printf(" a="); boundXXX_print(a); printf("\n");
  }
  boundXXX_set_infty(a,1);
  printf("boundXXX_set_infty(1)="); boundXXX_print(a); printf("\n");
  boundXXX_set_infty(a,-2);
  printf("boundXXX_set_infty(-2)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_swap(b,c);
  printf("boundXXX_swap(b,c): "); printf("b="); boundXXX_print(b); printf(" c="); boundXXX_print(c); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_neg(a,b);
  printf("boundXXX_neg(b)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_add(a,b,c);
  printf("boundXXX_add(b,c)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_add_uint(a,b,u);
  printf("boundXXX_add_uint(b,u)="); boundXXX_print(a); printf("\n");

  if (!boundXXX_infty(c)) {
    boundXXX_set(b,bb); boundXXX_set(c,cc);
    boundXXX_add_num(a,b,boundXXX_numref(c));
    printf("boundXXX_add_numXXX(b,boundXXX_numref(c)="); boundXXX_print(a); printf("\n");
  }

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_sub(a,b,c);
  printf("boundXXX_sub(b,c)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_sub_uint(a,b,u);
  printf("boundXXX_sub_uint(b,u)="); boundXXX_print(a); printf("\n");

  if (!boundXXX_infty(c)) {
    boundXXX_set(b,bb); boundXXX_set(c,cc);
    boundXXX_sub_num(a,b,boundXXX_numref(c));
    printf("boundXXX_sub_numXXX(b,boundXXX_numref(c)="); boundXXX_print(a); printf("\n");
  }

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_mul(a,b,c);
  printf("boundXXX_mul(b,c)="); boundXXX_print(a); printf("\n");

  if (!boundXXX_infty(c)) {
    boundXXX_set(b,bb); boundXXX_set(c,cc);
    boundXXX_mul_num(a,b,boundXXX_numref(c));
    printf("boundXXX_mul_numXXX(b,boundXXX_numref(c)="); boundXXX_print(a); printf("\n");
  }

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_mul_2(a,b);
  printf("boundXXX_mul_2(b)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_div(a,b,c);
  printf("boundXXX_div(b,c)="); boundXXX_print(a); printf("\n");

  if (!boundXXX_infty(c)) {
    boundXXX_set(b,bb); boundXXX_set(c,cc);
    boundXXX_div_num(a,b,boundXXX_numref(c));
    printf("boundXXX_div_numXXX(b,boundXXX_numref(c)="); boundXXX_print(a); printf("\n");
  }

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_div_2(a,b);
  printf("boundXXX_div_2(b)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_min(a,b,c);
  printf("boundXXX_min(b,c)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_max(a,b,c);
  printf("boundXXX_max(b,c)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_floor(a,b);
  printf("boundXXX_floor(b)="); boundXXX_print(a); printf("\n");

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_ceil(a,b);
  printf("boundXXX_ceil(b)="); boundXXX_print(a); printf("\n");

  printf("boundXXX_sgn(b)=%d\n",boundXXX_sgn(b));
  printf("boundXXX_cmp(b,c)=%d\n",boundXXX_cmp(b,c));
  printf("boundXXX_cmp_int(b,l)=%d\n",boundXXX_cmp_int(b,l));
  if (!boundXXX_infty(c)) {
    printf("boundXXX_cmp_num(b,boundXXX_numref(c))=%d\n",boundXXX_cmp_num(b,boundXXX_numref(c)));
  }
  printf("boundXXX_equal(b,c)=%d\n",boundXXX_equal(b,c));

  boundXXX_set(b,bb); boundXXX_set(c,cc);
  boundXXX_clear(bb);
  boundXXX_clear(cc);

  num_internal_clear(intern);
}




int main(int argc, char**argv)
{
  num_internal_t intern;
  unsigned long int u;
  long int l;
  unsigned long long int u2;
  long long int l2;
  mpz_t mpz;
  mpq_t mpq,mpq2;
  mpfr_t mpfr;
  double d;
  long double ld;

  numXXX_fpu_init();

  num_internal_init(intern);
  mpz_init(mpz); mpq_init(mpq); mpq_init(mpq2); mpfr_init(mpfr);

  /* Extreme cases */
  u = ULONG_MAX;
  l = LONG_MIN+1;
  u2 = ULLONG_MAX;
  l2 = LLONG_MIN+1;
  mpz_set_ui(mpz,u);
  mpq_set_z(mpq,mpz);
  mpz_mul(mpz,mpz,mpz);
  mpq_inv(mpq,mpq);
  d = (double)1.0/(double)0.0;
  ld = (long double)1.0/(long double)0.0;
  mpfr_set_d(mpfr,d,GMP_RNDU);

  {
    numXXX_t a,b,c;

    numXXX_init(a);
    numXXX_init_set_int(b,1000);
    numXXX_init_set_int(c,1500);
    numXXX(a,b,c,u,l,u2,l2,mpz,mpq,d,ld,mpfr);
    numXXX(a,c,b,u,l,u2,l2,mpz,mpq,d,ld,mpfr);

    numXXX_clear(a); numXXX_clear(b); numXXX_clear(c);
  }

  {
    boundXXX_t a,b,c;

    boundXXX_init(a);
    boundXXX_init_set_int(b,1000);
    boundXXX_init_set_int(c,1500);
    boundXXX(a,b,c,u,l,mpz,mpq,d,mpfr);
    boundXXX(a,c,b,u,l,mpz,mpq,d,mpfr);

    boundXXX_set_infty(c,1);
    boundXXX(a,b,c,u,l,mpz,mpq,d,mpfr);
    boundXXX(a,c,b,u,l,mpz,mpq,d,mpfr);

    boundXXX_clear(a); boundXXX_clear(b); boundXXX_clear(c);
  }

  /* Standard cases */
  u = 1000000;
  l = -300000;
  mpz_set_si(mpz,l);
  mpq_set_z(mpq,mpz);
  mpz_set_ui(mpz,u);
  mpq_set_z(mpq2,mpz);
  mpq_div(mpq,mpq,mpq2);
  mpz_set_si(mpz,l);
  mpfr_set_d(mpfr,4.25,GMP_RNDU);
  d = 3.5;
  {
    numXXX_t a,b,c;

    numXXX_init(a); numXXX_init(b); numXXX_init(c);
    numXXX_set_lfrac(b,3002,3,intern);
    numXXX_set_int(c,-1500);
    numXXX(a,b,c,u,l,u2,l2,mpz,mpq,d,ld,mpfr);
    numXXX(a,c,b,u,l,u2,l2,mpz,mpq,d,ld,mpfr);
    numXXX(b,b,b,u,l,u2,l2,mpz,mpq,d,ld,mpfr);

    numXXX_clear(a); numXXX_clear(b); numXXX_clear(c);
  }
  {
    boundXXX_t a,b,c;

    boundXXX_init(a); boundXXX_init(b); boundXXX_init(c);
    boundXXX_set_int(b,3002);
    boundXXX_set_int(c,3);
    boundXXX_div(b,b,c);
    boundXXX_set_int(c,-1500);
    boundXXX(a,b,c,u,l,mpz,mpq,d,mpfr);
    boundXXX(a,c,b,u,l,mpz,mpq,d,mpfr);
    boundXXX(b,b,b,u,l,mpz,mpq,d,mpfr);

    boundXXX_clear(a); boundXXX_clear(b); boundXXX_clear(c);
  }

  mpz_clear(mpz);
  mpq_clear(mpq2);
  mpq_clear(mpq);
  mpfr_clear(mpfr);
  num_internal_clear(intern);
}
