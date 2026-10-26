/*
 * oct_tight_closure_test.c
 *
 * Hand-written tests for the tight (integer) closure.
 */

#include <stdio.h>
#include <stdlib.h>

#include "oct.h"
#include "oct_fun.h"
#include "oct_internal.h"

ap_manager_t* mo;
oct_internal_t* pr;
int error_ = 0;

void print_oct(const char* msg, oct_t* o)
{
  fprintf(stderr,"%s = ",msg);
  oct_fprint(stderr,mo,o,NULL);
  oct_fdump(stderr,mo,o);
  fprintf(stderr,"\n");
}

/* sx*x_a + sy*x_b <= c   (a != b, sx,sy = +1 ou -1) */
static void set_binary(oct_t* o, size_t a, int sx, size_t b, int sy, long c)
{
  size_t j = (sx>0) ? 2*a : 2*a+1;     /* V_j  =  sx*x_a */
  size_t i = (sy>0) ? 2*b+1 : 2*b;     /* -V_i =  sy*x_b */
  bound_set_int(o->m[matpos2(i,j)], c);
}

static void set_binary_double(oct_t* o, size_t a, int sx, size_t b, int sy, double c){
  size_t j = (sx>0) ? 2*a : 2*a+1;     /* V_j  =  sx*x_a */
  size_t i = (sy>0) ? 2*b+1 : 2*b;     /* -V_i =  sy*x_b */
  num_t n;
  num_init(n);
  num_set_double(n,c);
  bound_set_num(o->m[matpos2(i,j)], n);
  num_clear(n);
}
/* s*x_k <= c */
static void set_unary(oct_t* o, size_t k, int s, long c)
{
  if (s>0) (o->m[matpos2(2*k+1,2*k)], 2*c);
  else bound_set_int(o->m[matpos2(2*k,2*k+1)], 2*c);
}

void test_tight_closure(bool tight)
{
  size_t dim = 3;
  size_t intdim = 2;         /* x = x0, y = x1 */
  oct_t* o = oct_alloc_internal(pr,dim,intdim);   /* intdim = dim  */
  bound_t expected;

  oct_set_tight_closure(mo,tight);
  printf("\ntight closure %s\n",tight ? "enabled" : "disabled");
  o->m = hmat_alloc_top(pr,dim);
  set_binary(o, 0,+1, 1,+1, 3);                /* x + y <= 3 */
  set_binary(o, 0,+1, 1,-1, 0);                /* x - y <= 0 */
  

  print_oct("before",o);
  oct_close(pr,o);
  print_oct("after",o);
  
  /* R : x <= 3/2 => m[1][0] = 3 ; Z : x <= 1 => m[1][0] = 2*1 */
  bound_init(expected);
  bound_set_int(expected,tight ? 2 : 3);
  if (!o->closed) { printf("unexpected empty\n"); error_++; }
  else if (bound_cmp(o->closed[matpos2(1,0)],expected))
    { printf("tight closure FAILED\n"); error_++; }
  else printf("tight closure OK\n");
  bound_clear(expected);
  oct_free(mo,o);
}

/* checks that cell (i,j) of the closed matrix of o equals w */
static void check_cell(const char* name, oct_t* o, size_t i, size_t j, long w)
{
  bound_t expected;
  bound_init(expected);
  bound_set_int(expected,w);
  if (!o->closed) { printf("%s: unexpected empty FAILED\n",name); error_++; }
  else if (bound_cmp(o->closed[matpos2(i,j)],expected)) {
    printf("%s FAILED (expected %ld, got ",name,w);
    bound_print(o->closed[matpos2(i,j)]);
    printf(")\n");
    error_++;
  }
  else printf("%s OK\n",name);
  bound_clear(expected);
}

/* checks that o is (or is not) empty after closure */
static void check_empty(const char* name, oct_t* o, bool empty)
{
  bool is_empty = !o->closed && !o->m;
  if (is_empty!=empty) {
    printf("%s FAILED (expected %s)\n",name,empty ? "empty" : "non-empty");
    error_++;
  }
  else printf("%s OK\n",name);
}

/* negative bound: -x - y <= -3, -x + y <= 0  =>  -2x <= -3, i.e. x >= 3/2
   R : m[0][1] = -3 ; Z : x >= 2 => m[0][1] = 2*floor(-3/2) = -4 */
void test_tight_negative(bool tight)
{
  size_t dim = 2;
  oct_t* o = oct_alloc_internal(pr,dim,dim);
  oct_set_tight_closure(mo,tight);
  o->m = hmat_alloc_top(pr,dim);
  set_binary(o, 0,-1, 1,-1, -3);               /* -x - y <= -3 */
  set_binary(o, 0,-1, 1,+1, 0);                /* -x + y <= 0 */
  oct_close(pr,o);
  check_cell(tight ? "negative bound, tight" : "negative bound, not tight",
	     o,0,1,tight ? -4 : -3);
  oct_free(mo,o);
}

/* x + y <= 1, x - y <= 0, -x - y <= -1, -x + y <= 0  =>  x = y = 1/2
   non-empty on R, empty on Z */
void test_tight_empty(bool tight)
{
  size_t dim = 2;
  oct_t* o = oct_alloc_internal(pr,dim,dim);
  oct_set_tight_closure(mo,tight);
  o->m = hmat_alloc_top(pr,dim);
  set_binary(o, 0,+1, 1,+1,1);                /* x + y <= 1 */
  set_binary(o, 0, +1, 1,-1, 0);                /* x - y <= 0 */
  set_binary(o, 0,-1, 1,-1, -1);               /* -x - y <= -1 */
  set_binary(o, 0,-1, 1,+1, 0);                /* -x + y <= 0 */
  oct_close(pr,o);
  check_empty(tight ? "Z-emptiness, tight" : "Z-emptiness, not tight",
	      o,tight);
  print_oct("oct:",o);
  oct_free(mo,o);
}


int main(void)
{
  mo = oct_manager_alloc();
  if (!mo) return 1;
  pr = oct_init_from_manager(mo,0,0);
  printf("nums: %s\n",NUM_NAME);

  test_tight_closure(false);
  test_tight_closure(true);
  test_tight_negative(false);
  test_tight_negative(true);
  test_tight_empty(false);
  test_tight_empty(true);

  ap_manager_free(mo);
  if (error_) printf("\n%i error(s)!\n",error_);
  else printf("\nall tests passed\n");
  return error_ ? 1 : 0;
}
