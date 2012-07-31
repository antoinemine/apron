
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

void check(itv_t a)
{
  bound_neg(a->inf,a->inf);
  if (bound_cmp(a->inf,a->sup)>0) abort();
  bound_neg(a->inf,a->inf);
}

void testunopi(char* str, bool (*ptr)(itv_internal_t*,itv_t,itv_t),itv_t c, itv_t a)
{
  itv_t a2;
  itv_init_set(a2,a);
  printf("%s\n",str);
  ptr(intern,c,a);
  printf("z = unop x = "); itv_print(c); printf("\n"); 
  ptr(intern,a,a);
  printf("x = unop x = "); itv_print(a); printf("\n"); 
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);
  itv_clear(a2);
}
void testunop(char* str, void (*ptr)(itv_t,itv_t),itv_t c, itv_t a)
{
  itv_t a2;
  itv_init_set(a2,a);
  printf("%s\n",str);
  ptr(c,a);
  printf("z = unop x = "); itv_print(c); printf("\n"); check(c);
  ptr(a,a);
  printf("x = unop x = "); itv_print(a); printf("\n"); check(a);
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);
  itv_clear(a2);
}

void test_pow(itv_t a, int n)
{
  itv_t r,x;
  itv_init(r); itv_init(x);
  itv_set_int(x,n);
  itv_pow(intern,r,a,x);
  printf("x ^ %i = ", n); itv_print(r); printf("\n");
  itv_clear(r); itv_clear(x);
}

void test_inv_pow(itv_t a, int n)
{
  itv_t r,x,p;
  itv_init(r); itv_init(x); itv_init(p);
  itv_set_int(x,n);
  itv_set_int2(p,1,1);
  itv_inv_pow(intern,r,p,a,x);
  printf("x ^ 1/%i = ", n); itv_print(r);
  itv_set_int2(p,-1,-1);
  itv_inv_pow(intern,r,p,a,x);
  printf(" / "); itv_print(r);
  itv_set_int2(p,-1,1);
  itv_inv_pow(intern,r,p,a,x);
  printf(" / "); itv_print(r);
  printf("\n");
  itv_clear(r); itv_clear(x); itv_clear(p);
}

void testun(itv_t a)
{
  itv_t c,a2;
  bool exact;

  itv_init_set(a2,a);
  itv_init(c);
  printf("x = "); itv_print(a); printf("\n");
  testunop("neg",itv_neg,c,a);
  testunop("abs",itv_abs,c,a);
  testunopi("sqrt",itv_sqrt,c,a);
  testunop("ceil",itv_ceil,c,a);
  testunop("floor",itv_floor,c,a);
  testunop("trunc",itv_trunc,c,a);
  testunop("to_float",itv_to_float,c,a);
  testunop("to_double",itv_to_double,c,a);
  itv_mul_2exp(c,a,2); printf("x << 2 = "); itv_print(c); printf("\n");
  itv_mul_2exp(c,a,-2); printf("x >> 2 = "); itv_print(c); printf("\n");
  test_pow(a,0); test_pow(a,1); test_pow(a,2); test_pow(a,3); test_pow(a,10); test_pow(a,11);
  test_inv_pow(a,0); test_inv_pow(a,1); test_inv_pow(a,2); test_inv_pow(a,3); test_inv_pow(a,10); test_inv_pow(a,11);
  printf("\n");
  itv_clear(c);
  itv_clear(a2);
}

void testbinopi(char* str, void (*ptr)(itv_internal_t*,itv_t,itv_t,itv_t),itv_t c, itv_t a, itv_t b)
{
  itv_t a2,b2;
  itv_init_set(a2,a);
  itv_init_set(b2,b);

  printf("%s\n",str);
  ptr(intern,c,a,b);
  printf("z = x binop y = "); itv_print(c); printf("\n"); check(c);
  ptr(intern,a,a,b);
  printf("x = x binop y = "); itv_print(a); printf("\n"); check(a);
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);
  ptr(intern,b,a,b);
  printf("y = x binop y = "); itv_print(b); printf("\n"); check(b);
  if (! itv_is_eq(b,c)) abort();
  itv_set(b,b2);
  ptr(intern,c,a,a);
  printf("z = x binop x = "); itv_print(c); printf("\n"); check(c);
  ptr(intern,a,a,a);
  printf("x = x binop x = "); itv_print(a); printf("\n"); check(a);
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);

  itv_clear(a2);
  itv_clear(b2);
}
void testbinop(char* str, void (*ptr)(itv_t,itv_t,itv_t),itv_t c, itv_t a, itv_t b)
{
  itv_t a2,b2;
  itv_init_set(a2,a);
  itv_init_set(b2,b);

  printf("%s\n",str);
  ptr(c,a,b);
  printf("z = x binop y = "); itv_print(c); printf("\n"); check(c);
  ptr(a,a,b);
  printf("x = x binop y = "); itv_print(a); printf("\n"); check(a);
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);
  ptr(b,a,b);
  printf("y = x binop y = "); itv_print(b); printf("\n"); check(b);
  if (! itv_is_eq(b,c)) abort();
  itv_set(b,b2);
  ptr(c,a,a);
  printf("z = x binop x = "); itv_print(c); printf("\n"); check(c);
  ptr(a,a,a);
  printf("x = x binop x = "); itv_print(a); printf("\n"); check(a);
  if (! itv_is_eq(a,c)) abort();
  itv_set(a,a2);

  itv_clear(a2);
  itv_clear(b2);
}

void testbin(itv_t a, itv_t b)
{
  itv_t c;
  itv_init(c);
  printf("x = "); itv_print(a); printf("\n");
  printf("y = "); itv_print(b); printf("\n");
  testbinop("add",itv_add,c,a,b);
  testbinop("sub",itv_sub,c,a,b);
  testbinopi("mul",itv_mul,c,a,b);
  testbinopi("div",itv_div,c,a,b);
  printf("\n");
  
  itv_clear(c);
}

void testmod(itv_t a, itv_t b)
{
  itv_t c,a2,b2;
  itv_init_set(a2,a);
  itv_init_set(b2,b);
  itv_init(c);
  printf("x = "); itv_print(a); printf("\n");
  printf("y = "); itv_print(b); printf("\n");
  itv_mod(intern,c,a,b,1);
  printf("z = x mod_i y = "); itv_print(c); printf("\n");
  itv_mod(intern,a,a,b,1);
  printf("x = x mod_i y = "); itv_print(a); printf("\n");
  itv_set(a,a2);
  itv_mod(intern,b,a,b,1);
  printf("y = x mod_i y = "); itv_print(b); printf("\n");
  itv_set(b,b2);
  itv_mod(intern,c,a,a,1);
  printf("z = x mod_i x = "); itv_print(c); printf("\n");
  itv_mod(intern,a,a,a,1);
  printf("x = x mod_i x = "); itv_print(a); printf("\n");
  itv_set(a,a2);

  itv_mod(intern,c,a,b,0);
  printf("z = x mod y = "); itv_print(c); printf("\n");
  itv_mod(intern,a,a,b,0);
  printf("x = x mod y = "); itv_print(a); printf("\n");
  itv_set(a,a2);
  itv_mod(intern,b,a,b,0);
  printf("y = x mod y = "); itv_print(b); printf("\n");
  itv_set(b,b2);
  itv_mod(intern,c,a,a,0);
  printf("z = x mod x = "); itv_print(c); printf("\n");
  itv_mod(intern,a,a,a,0);
  printf("x = x mod x = "); itv_print(a); printf("\n");
  itv_set(a,a2);
  itv_clear(c); itv_clear(a2); itv_clear(b2);
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

  set_double(a,-0.375,-0.375); set_double(b,-0.625,-0.625); testbin(a,b); 
  set_double(a,-0.375,-0.375); set_double(b,0.625,0.625); testbin(a,b); 
  set_double(a,0.375,0.375); set_double(b,-0.625,-0.625); testbin(a,b); 
  set_double(a,0.375,0.375); set_double(b,0.625,0.625); testbin(a,b);

  set_frac(a,-1,3,-1,3); set_frac(b,-2,3,-2,3); testbin(a,b); 
  set_frac(a,-1,3,-1,3); set_frac(b,2,3,2,3); testbin(a,b); 
  set_frac(a,1,3,1,3); set_frac(b,-2,3,-2,3); testbin(a,b); 
  set_frac(a,1,3,1,3); set_frac(b,2,3,2,3); testbin(a,b); 




  itv_clear(a); itv_clear(b);
  itv_internal_free(intern);
  return 0;
}
