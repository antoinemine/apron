#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "num.h"
#include "ap_manager.h"
#include "bound.h"

void num(num_t a, num_t b, num_t c,
	 unsigned long int u, long int l,
	 mpz_t mpz,
	 mpq_t mpq,
	 double d,
	 mpfr_t mpfr)
{
  num_t bb,cc;
  unsigned long int uu;
  long int ll;
  mpz_t mpzz;
  mpq_t mpqq;
  mpfr_t mpfrr;
  double dd;

  num_init_set(bb,b);
  num_init_set(cc,c);
  ll = l;
  uu = u;
  mpz_init_set(mpzz,mpz);
  mpq_init(mpqq); mpq_set(mpqq,mpq);
  mpfr_init(mpfrr); mpfr_set(mpfrr,mpfr,GMP_RNDU);

  printf("********************\n");
  printf("b="); num_print(b);
  printf(" c="); num_print(c);
  printf("\nu=%lu l=%ld\n",u,l);
  printf("mpz="); mpz_out_str(stdout,10,mpz);
  printf(" mpq="); mpq_out_str(stdout,10,mpq);
  printf(" mpfr="); mpfr_out_str(stdout,10,10,mpfr,GMP_RNDU);
  printf(" d=%.13e\n",d);

  num_set(b,bb); num_set(c,cc);
  num_swap(b,c);
  printf("num_swap(b,c): "); printf("b="); num_print(b); printf(" c="); num_print(c); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_neg(a,b);
  printf("num_neg(b)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_add(a,b,c);
  printf("num_add(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_add_uint(a,b,u);
  printf("num_add_uint(b,u)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_sub(a,b,c);
  printf("num_sub(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_sub_uint(a,b,u);
  printf("num_sub_uint(b,u)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_mul(a,b,c);
  printf("num_mul(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_mul_2(a,b);
  printf("num_mul_2(b)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_div(a,b,c);
  printf("num_div(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_div_2(a,b);
  printf("num_div_2(b)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_min(a,b,c);
  printf("num_min(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_max(a,b,c);
  printf("num_max(b,c)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_floor(a,b);
  printf("num_floor(b)="); num_print(a); printf("\n");

  num_set(b,bb); num_set(c,cc);
  num_ceil(a,b);
  printf("num_ceil(b)="); num_print(a); printf("\n");

  printf("num_sgn(b)=%d\n",num_sgn(b));
  printf("num_cmp(b,c)=%d\n",num_cmp(b,c));
  printf("num_cmp_int(b,l)=%d\n",num_cmp_int(b,l));
  printf("num_equal(b,c)=%d\n",num_equal(b,c));

  if (u<=LONG_MAX) {
    num_set_int2(a,l,u);
    printf("num_set_int2(l,u)=");num_print(a); printf("\n");
  }

  printf("mpz_fits_num(mpz)=%d\n",mpz_fits_num(mpz));
  if (mpz_fits_num(mpz)) {
    num_set_mpz(a,mpz);
    printf("num_set_mpz(mpz)=");num_print(a); printf("\n");
  }

  printf("mpq_fits_num(mpq)=%d\n",mpq_fits_num(mpq));
  if (mpq_fits_num(mpq)) {
    num_set_mpq(a,mpq);
    printf("num_set_mpq(mpq)=");num_print(a); printf("\n");
  }

  printf("double_fits_num(d)=%d\n",double_fits_num(d));
  if (double_fits_num(d)) {
    num_set_double(a,d);
    printf("num_set_double(d)=");num_print(a); printf("\n");
  }

  printf("mpfr_fits_num(mpfr)=%d\n",mpfr_fits_num(mpfr));
  if (mpfr_fits_num(mpfr)) {
    num_set_mpfr(a,mpfr);
    printf("num_set_mpfr(mpfr)=");num_print(a); printf("\n");
  }

  {
    long int i;
    int_set_num(&i,b);
    printf("int_set_num(b)=%ld\n",i);
  }

  mpz_set_num(mpz,b);
  printf("mpz_set_num(b)="); mpz_out_str(stdout,10,mpz); printf("\n"); mpz_set(mpz,mpzz);

  mpq_set_num(mpq,b);
  printf("mpq_set_num(b)="); mpq_out_str(stdout,10,mpq); printf("\n"); mpq_set(mpq,mpqq);

  printf("num_fits_double(b)=%d\n",num_fits_double(b));
  if (num_fits_double(b)) {
    double k;
    double_set_num(&k,b);
    printf("double(b)=%.13e\n",k);
  }
  num_set(b,bb); num_set(c,cc);

  printf("num_fits_mpfr(b)=%d\n",num_fits_mpfr(b));
  if (num_fits_mpfr(b)) {
    mpfr_set_num(mpfr,b); 
    printf("mpfr_set_num(b)="); mpfr_out_str(stdout,10,10,mpfr,GMP_RNDU); printf("\n"); mpfr_set(mpfr,mpfrr,GMP_RNDU);
  }

  num_clear(bb);
  num_clear(cc);
  mpz_clear(mpzz);
  mpq_clear(mpqq);
  mpfr_clear(mpfrr);
}

void bound(bound_t a, bound_t b, bound_t c,
	   unsigned long int u, long int l,
	   mpz_t mpz,
	   mpq_t mpq,
	   double d,
	   mpfr_t mpfr)
{
  bound_t bb,cc;
  unsigned long int uu;
  long int ll;
  double dd;

  bound_init_set(bb,b);
  bound_init_set(cc,c);
  ll = l;
  uu = u;

  printf("********************\n");
  printf("b="); bound_print(b);
  printf(" c="); bound_print(c);
  printf("\nu=%lu l=%ld\n",u,l);
  printf(" d=%.13e\n",d);

  bound_set(b,bb); bound_set(c,cc);
  if (!bound_infty(b)) {
    bound_set_num(a,bound_numref(b));
    printf("bound_set_num(b): "); printf("b="); bound_print(b); printf(" a="); bound_print(a); printf("\n");
  }
  bound_set_infty(a,1);
  printf("bound_set_infty(1)="); bound_print(a); printf("\n");
  bound_set_infty(a,-2);
  printf("bound_set_infty(-2)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_swap(b,c);
  printf("bound_swap(b,c): "); printf("b="); bound_print(b); printf(" c="); bound_print(c); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_neg(a,b);
  printf("bound_neg(b)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_add(a,b,c);
  printf("bound_add(b,c)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_add_uint(a,b,u);
  printf("bound_add_uint(b,u)="); bound_print(a); printf("\n");

  if (!bound_infty(c)) {
    bound_set(b,bb); bound_set(c,cc);
    bound_add_num(a,b,bound_numref(c));
    printf("bound_add_num(b,bound_numref(c)="); bound_print(a); printf("\n");
  }

  bound_set(b,bb); bound_set(c,cc);
  bound_sub(a,b,c);
  printf("bound_sub(b,c)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_sub_uint(a,b,u);
  printf("bound_sub_uint(b,u)="); bound_print(a); printf("\n");

  if (!bound_infty(c)) {
    bound_set(b,bb); bound_set(c,cc);
    bound_sub_num(a,b,bound_numref(c));
    printf("bound_sub_num(b,bound_numref(c)="); bound_print(a); printf("\n");
  }

  bound_set(b,bb); bound_set(c,cc);
  bound_mul(a,b,c);
  printf("bound_mul(b,c)="); bound_print(a); printf("\n");

  if (!bound_infty(c)) {
    bound_set(b,bb); bound_set(c,cc);
    bound_mul_num(a,b,bound_numref(c));
    printf("bound_mul_num(b,bound_numref(c)="); bound_print(a); printf("\n");
  }

  bound_set(b,bb); bound_set(c,cc);
  bound_mul_2(a,b);
  printf("bound_mul_2(b)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_div(a,b,c);
  printf("bound_div(b,c)="); bound_print(a); printf("\n");

  if (!bound_infty(c)) {
    bound_set(b,bb); bound_set(c,cc);
    bound_div_num(a,b,bound_numref(c));
    printf("bound_div_num(b,bound_numref(c)="); bound_print(a); printf("\n");
  }

  bound_set(b,bb); bound_set(c,cc);
  bound_div_2(a,b);
  printf("bound_div_2(b)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_min(a,b,c);
  printf("bound_min(b,c)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_max(a,b,c);
  printf("bound_max(b,c)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_floor(a,b);
  printf("bound_floor(b)="); bound_print(a); printf("\n");

  bound_set(b,bb); bound_set(c,cc);
  bound_ceil(a,b);
  printf("bound_ceil(b)="); bound_print(a); printf("\n");

  printf("bound_sgn(b)=%d\n",bound_sgn(b));
  printf("bound_cmp(b,c)=%d\n",bound_cmp(b,c));
  printf("bound_cmp_int(b,l)=%d\n",bound_cmp_int(b,l));
  if (!bound_infty(c)) {
    printf("bound_cmp_num(b,bound_numref(c))=%d\n",bound_cmp_num(b,bound_numref(c)));
  }
  printf("bound_equal(b,c)=%d\n",bound_equal(b,c));

  bound_set(b,bb); bound_set(c,cc);
  bound_clear(bb);
  bound_clear(cc);
}




int main(int argc, char**argv)
{
  unsigned long int u; 
  long int l;
  mpz_t mpz;
  mpq_t mpq,mpq2;
  mpfr_t mpfr;
  double d;

  ap_fpu_init();

  mpz_init(mpz); mpq_init(mpq); mpq_init(mpq2); mpfr_init(mpfr); 

  /* Extreme cases */
  u = ULONG_MAX;
  l = LONG_MIN+1;
  mpz_set_ui(mpz,u);
  mpq_set_z(mpq,mpz);
  mpz_mul(mpz,mpz,mpz);
  mpq_inv(mpq,mpq);
  d = (double)1.0/(double)0.0;
  mpfr_set_d(mpfr,d,GMP_RNDU);

  {
    num_t a,b,c;

    num_init(a);
    num_init_set_int(b,1000);
    num_init_set_int(c,1500);
    num(a,b,c,u,l,mpz,mpq,d,mpfr);
    num(a,c,b,u,l,mpz,mpq,d,mpfr);

    num_clear(a); num_clear(b); num_clear(c);
  }

  {
    bound_t a,b,c;

    bound_init(a);
    bound_init_set_int(b,1000);
    bound_init_set_int(c,1500);
    bound(a,b,c,u,l,mpz,mpq,d,mpfr);
    bound(a,c,b,u,l,mpz,mpq,d,mpfr);

    bound_set_infty(c,1);
    bound(a,b,c,u,l,mpz,mpq,d,mpfr);
    bound(a,c,b,u,l,mpz,mpq,d,mpfr);

    bound_clear(a); bound_clear(b); bound_clear(c);
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
    num_t a,b,c;

    num_init(a); num_init(b); num_init(c);
    num_set_int2(b,3002,3);
    num_set_int(c,-1500);
    num(a,b,c,u,l,mpz,mpq,d,mpfr);
    num(a,c,b,u,l,mpz,mpq,d,mpfr);
    num(b,b,b,u,l,mpz,mpq,d,mpfr);

    num_clear(a); num_clear(b); num_clear(c);
  }
  {
    bound_t a,b,c;

    bound_init(a); bound_init(b); bound_init(c);
    bound_set_int(b,3002);
    bound_set_int(c,3);
    bound_div(b,b,c);
    bound_set_int(c,-1500);
    bound(a,b,c,u,l,mpz,mpq,d,mpfr);
    bound(a,c,b,u,l,mpz,mpq,d,mpfr);
    bound(b,b,b,u,l,mpz,mpq,d,mpfr);

    bound_clear(a); bound_clear(b); bound_clear(c);
  }
  
  mpz_clear(mpz);
  mpq_clear(mpq2);
  mpq_clear(mpq);
  mpfr_clear(mpfr);
}
