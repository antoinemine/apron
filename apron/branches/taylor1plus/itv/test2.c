
/* Some more itv testing.
   Compile with

   gcc test2.c itv.c itv_linexpr.c -std=c99 -I../num -I../apron  -L../apron -lapron_debug -lmpfr -lgmp -lm -DNUM_MPQ

   (replacing NUM_MPQ with your choice of NUM_)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ap_manager.h"
#include "num.h"
#include "bound.h"
#include "itv.h"

itv_internal_t* intern;

void testun(itv_t a)
{
  itv_t b;
  bool exact;
  itv_init(b);
  printf("x = "); itv_print(a); printf("\n");
  itv_neg(b,a); printf("-x = "); itv_print(b); printf("\n");
  itv_abs(b,a); printf("abs(x) = "); itv_print(b); printf("\n");
  exact = itv_sqrt(intern,b,a); printf("sqrt(x) = "); itv_print(b); printf(" (%i)\n",exact);
  itv_ceil(b,a); printf("ceil(x) = "); itv_print(b); printf("\n");
  itv_floor(b,a); printf("floor(x) = "); itv_print(b); printf("\n");
  itv_trunc(b,a); printf("trunc(x) = "); itv_print(b); printf("\n");
  itv_to_int(b,a); printf("int(x) = "); itv_print(b); printf("\n");
  itv_to_float(b,a); printf("float(x) = "); itv_print(b); printf("\n");
  itv_to_double(b,a); printf("double(x) = "); itv_print(b); printf("\n");
  itv_mul_2exp(b,a,2); printf("x << 2 = "); itv_print(b); printf("\n");
  itv_mul_2exp(b,a,-2); printf("x >> 2 = "); itv_print(b); printf("\n");
  printf("\n");
  itv_clear(b);
}

void testbin(itv_t a, itv_t b)
{
  itv_t c;
  itv_init(c);
  printf("x = "); itv_print(a); printf("\n");
  printf("y = "); itv_print(b); printf("\n");
  itv_add(c,a,b);
  printf("x + y = "); itv_print(c); printf("\n");
  itv_sub(c,a,b);
  printf("x - y = "); itv_print(c); printf("\n");
  itv_mul(intern,c,a,b);
  printf("x * y = "); itv_print(c); printf("\n");
  itv_div(intern,c,a,b);
  printf("x / y = "); itv_print(c); printf("\n");
  itv_clear(c);
  printf("\n");
}

void testmod(itv_t a, itv_t b)
{
  itv_t c;
  itv_init(c);
  printf("x = "); itv_print(a); printf("\n");
  printf("y = "); itv_print(b); printf("\n");
  itv_mod(intern,c,a,b,1);
  printf("x mod_i y = "); itv_print(c); printf("\n");
  itv_mod(intern,c,a,b,0);
  printf("x mod_f y = "); itv_print(c); printf("\n");
  printf("\n");
  itv_clear(c);
}

void set_double(itv_t a, double inf, double sup)
{
  num_t n;
  num_init(n);
  printf("[%f,%f]\n",inf,sup);
  if (sup==1./0.) bound_set_infty(a->sup,1);
  else { num_set_double(n,sup); bound_set_num(a->sup,n); }
  if (inf==-1./0.) bound_set_infty(a->inf,1);
  else { num_set_double(n,-inf); bound_set_num(a->inf,n); }
  num_clear(n);
}

void set_frac(itv_t a, int ninf, int dinf, int nsup, int dsup)
{
  num_t n;
  num_init(n);
  printf("[%i/%i,%i/%i]\n",ninf,dinf,nsup,dsup);
  num_set_int2(n,nsup,dsup);  bound_set_num(a->sup,n);
  num_set_int2(n,-ninf,dinf); bound_set_num(a->inf,n);
  num_clear(n);
}

int main()
{
  itv_t a,b;

  ap_fpu_init();
  mpfr_set_default_prec(4046);

  intern = itv_internal_alloc();
  itv_init(a); itv_init(b);

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

  itv_clear(a); itv_clear(b);
  itv_internal_free(intern);
  return 0;
}
