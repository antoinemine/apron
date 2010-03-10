#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ap_manager.h"
#include "num.h"
#include "bound.h"
#include "itv.h"

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

  ap_fpu_init();
  mpfr_set_default_prec(4046);

  intern = itv_internal_alloc();
  itv_init(a); itv_init(b); itv_init(c); bound_init(bound);

  /* Positive or negative intervals */
  bound_set_int(b->inf,-3); bound_set_int(b->sup,5);
  bound_set_int(c->inf,-1); bound_set_int(c->sup,5);
  bound_set_int(bound,4);
  arith(intern,a,b,c,bound);
  itv_neg(c,c);
  arith(intern,a,b,c,bound);
  itv_neg(b,b);
  arith(intern,a,b,c,bound);
  itv_neg(c,c);
  arith(intern,a,b,c,bound);
  /* general intervals */
  bound_set_int(b->inf,3); bound_set_int(b->sup,5);
  bound_set_int(c->inf,7); bound_set_int(c->sup,11);
  bound_set_int(bound,3);
  arith(intern,a,b,c,bound);
  bound_set_int(bound,-3);
  arith(intern,a,b,c,bound);

  /* aliases */
  bound_set_int(b->inf,3); bound_set_int(b->sup,5);
  bound_set_int(c->inf,7); bound_set_int(c->sup,11);
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
