/*
 * Testing expression tree support in APRON.
 *
 * Antoine Mine' 2007
 */

/* Compile with:
   gcc test_texpr0.c -Wall -I$GMP_INSTALL/include -L$GMP_INSTALL/lib -I$MPFR_INSTALL/include -L$MPFR_INSTALL/lib -g  -DDEBUG -O0 -I. -L../box -lbox_debug -L. -lapron_debug -lmpfr -lgmp -lm
*/

#include <stdio.h>
#include <stdlib.h>

#include "ap_expr0.h"
#include "ap_abstract0.h"
#include "../box/box.h"

ap_manager_t* man;

#define V(i)   ap_texpr0_dim(i)
#define C(i)   ap_texpr0_cst_scalar_int(i)
#define I(a,b) ap_texpr0_cst_interval_double(a,b)
#define ADD(a,b) ap_texpr0_binop(AP_TEXPR_ADD,a,b,AP_RTYPE_REAL,AP_RDIR_RND)
#define SUB(a,b) ap_texpr0_binop(AP_TEXPR_SUB,a,b,AP_RTYPE_REAL,AP_RDIR_RND)
#define NEG(a)   ap_texpr0_unop(AP_TEXPR_NEG,a,AP_RTYPE_REAL,AP_RDIR_RND)
#define SQRT(a)  ap_texpr0_unop(AP_TEXPR_SQRT,a,AP_RTYPE_REAL,AP_RDIR_RND)
#define MUL(a,b) ap_texpr0_binop(AP_TEXPR_MUL,a,b,AP_RTYPE_REAL,AP_RDIR_RND)
#define DIV(a,b) ap_texpr0_binop(AP_TEXPR_DIV,a,b,AP_RTYPE_REAL,AP_RDIR_RND)
#define MOD(a,b) ap_texpr0_binop(AP_TEXPR_MOD,a,b,AP_RTYPE_REAL,AP_RDIR_RND)

#define ADDF(a,b) ap_texpr0_binop(AP_TEXPR_ADD,a,b,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define SUBF(a,b) ap_texpr0_binop(AP_TEXPR_SUB,a,b,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define NEGF(a)   ap_texpr0_unop(AP_TEXPR_NEG,a,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define SQRTF(a)  ap_texpr0_unop(AP_TEXPR_SQRT,a,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define MULF(a,b) ap_texpr0_binop(AP_TEXPR_MUL,a,b,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define DIVF(a,b) ap_texpr0_binop(AP_TEXPR_DIV,a,b,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define MODF(a,b) ap_texpr0_binop(AP_TEXPR_MOD,a,b,AP_RTYPE_SINGLE,AP_RDIR_RND)

#define ADDI(a,b) ap_texpr0_binop(AP_TEXPR_ADD,a,b,AP_RTYPE_INT,AP_RDIR_RND)
#define SUBI(a,b) ap_texpr0_binop(AP_TEXPR_SUB,a,b,AP_RTYPE_INT,AP_RDIR_RND)
#define NEGI(a)   ap_texpr0_unop(AP_TEXPR_NEG,a,AP_RTYPE_INT,AP_RDIR_RND)
#define SQRTI(a)  ap_texpr0_unop(AP_TEXPR_SQRT,a,AP_RTYPE_INT,AP_RDIR_RND)
#define MULI(a,b) ap_texpr0_binop(AP_TEXPR_MUL,a,b,AP_RTYPE_INT,AP_RDIR_RND)
#define DIVI(a,b) ap_texpr0_binop(AP_TEXPR_DIV,a,b,AP_RTYPE_INT,AP_RDIR_RND)
#define MODI(a,b) ap_texpr0_binop(AP_TEXPR_MOD,a,b,AP_RTYPE_INT,AP_RDIR_RND)

#define CASTF(a) ap_texpr0_unop(AP_TEXPR_CAST,a,AP_RTYPE_SINGLE,AP_RDIR_RND)
#define TRUNC(a) ap_texpr0_unop(AP_TEXPR_CAST,a,AP_RTYPE_INT,AP_RDIR_ZERO)
#define UP(a) ap_texpr0_unop(AP_TEXPR_CAST,a,AP_RTYPE_INT,AP_RDIR_UP)
#define DOWN(a) ap_texpr0_unop(AP_TEXPR_CAST,a,AP_RTYPE_INT,AP_RDIR_DOWN)
#define NEAREST(a) ap_texpr0_unop(AP_TEXPR_CAST,a,AP_RTYPE_INT,AP_RDIR_NEAREST)

void print_info(ap_texpr0_t* a)
{
  int i, m = ap_texpr0_max_dim(a);
  ap_dim_t* dd = ap_texpr0_dimlist(a);
  ap_texpr0_fprint(stdout,a,NULL);
  /* sizes, hashes */
  printf("\ndepth=%i, size=%i, max_dim=%i, hash=%i, dims=[",
	 (int)ap_texpr0_depth(a),(int)ap_texpr0_size(a),m, (int)ap_texpr0_hash(a));
  /* dims */
  for (i=0;dd[i]!=AP_DIM_MAX;i++) printf("%s%i",i?",":"",dd[i]);
  printf("]\n");
  for (i=0;dd[i]!=AP_DIM_MAX;i++) assert(ap_texpr0_has_dim(a,dd[i]));
  assert(!i || dd[i-1]==m);
  assert(!i || !ap_texpr0_has_dim(a,dd[i-1]+1));
  /* classification */
  printf("%scst, %slinear, %spolynomial, %spolyfrac, %sscalar\n\n",
	 ap_texpr0_is_interval_cst(a)?"":"non-",
	 ap_texpr0_is_interval_linear(a)?"":"non-",
	 ap_texpr0_is_interval_polynomial(a)?"":"non-",
	 ap_texpr0_is_interval_polyfrac(a)?"":"non-",
	 ap_texpr0_is_scalar(a)?"":"non-");
  free(dd);
}

ap_interval_t** random_point(ap_interval_t** org, int dim, int p)
{
  ap_interval_t** res = ap_interval_array_alloc(dim);
  int i;

  for (i=0;i<dim;i++) {
    switch (p&1) {
    case 0:
      ap_interval_set_scalar(res[i],org[i]->inf,org[i]->inf);
      break;
    default:
      ap_interval_set_scalar(res[i],org[i]->sup,org[i]->sup);
      break;
    }
    p>>=1;
  }
  
  return res;
}

void testlinearize_discr(ap_texpr0_t* a, ap_interval_t**inter, int intdim, 
			 ap_scalar_discr_t discr)
{  
  bool exact = 2;
  ap_linexpr0_t *l, *q;
  ap_interval_t *itv, *itvl, *itvq;
  ap_abstract0_t *abs;
  int i,j, dim = ap_texpr0_max_dim(a)+1;

  abs = ap_abstract0_of_box(man, dim<intdim ? dim : intdim, 
			    dim>intdim ? dim-intdim : 0, inter);

  l = ap_intlinearize_texpr0(man, abs, a, &exact, discr, false);
  q = ap_intlinearize_texpr0(man, abs, a, &exact, discr, true);
  itv = ap_eval_texpr0(man, abs, a, discr, &exact);
  itvl = ap_eval_linexpr0(man, abs, l, discr, &exact);
  itvq = ap_eval_linexpr0(man, abs, q, discr, &exact);

  printf("  lin:  "); ap_linexpr0_fprint(stdout, l, NULL); printf("\n");
  printf("  qlin: "); ap_linexpr0_fprint(stdout, q, NULL); printf("\n");
  printf("  itv:      "); ap_interval_fprint(stdout, itv); printf("\n");
  printf("  lin-itv:  "); ap_interval_fprint(stdout, itvl); printf("\n");
  printf("  qlin-itv: "); ap_interval_fprint(stdout, itvq); printf("\n");

  for (j=0;j<1<<dim;j++) {
    ap_interval_t** pt = random_point(inter,dim,j);
    ap_abstract0_t* ab = ap_abstract0_of_box(man, dim<intdim ? dim : intdim, 
					     dim>intdim ? dim-intdim : 0, pt);
    ap_interval_t* pe = ap_eval_texpr0(man, ab, a, discr, &exact);
    ap_interval_t* pl = ap_eval_linexpr0(man, ab, l, discr, &exact);
    ap_interval_t* pq = ap_eval_linexpr0(man, ab, q, discr, &exact);
    printf("  eval ");
    for (i=0;i<dim;i++) {
      printf("%sx%i=",i?" ":"",i);
      ap_interval_print(pt[i]);
    }
    printf(" expr="); ap_interval_print(pe);
    printf(" lin=");  ap_interval_print(pl);
    printf(" qlin="); ap_interval_print(pq);
    if (ap_interval_equal(pe,pl)) {
      if (ap_interval_equal(pe,pq)) printf(" **");
      else printf(" *");
    }
    printf("\n");
    assert(ap_interval_is_leq(pe,itv));
    assert(ap_interval_is_leq(pl,itvl));
    assert(ap_interval_is_leq(pq,itvq));
    assert(ap_interval_is_leq(pe,pl));
    assert(ap_interval_is_leq(pe,pq));
    ap_interval_free(pe);
    ap_interval_free(pl);
    ap_interval_free(pq);
    ap_abstract0_free(man, ab);
    ap_interval_array_free(pt, dim);
  }

  ap_interval_free(itvl);
  ap_interval_free(itvq);
  ap_interval_free(itv);
  ap_linexpr0_free(l);
  ap_linexpr0_free(q);
  ap_abstract0_free(man, abs);
}

void testlinearize(ap_texpr0_t* a, ap_interval_t**inter, int intdim)
{
  int i, dim = ap_texpr0_max_dim(a)+1;
  ap_texpr0_fprint(stdout,a,NULL); printf("\n");
  for (i=0;i<dim;i++) {
    printf("%sx%i=",i?" ":"",i);
    ap_interval_print(inter[i]);
  }
  printf("\n");
  printf("mpq:\n");
  testlinearize_discr(a,inter,intdim,AP_SCALAR_MPQ);
  printf("double:\n");
  testlinearize_discr(a,inter,intdim,AP_SCALAR_DOUBLE);
  printf("mpfr:\n");
  testlinearize_discr(a,inter,intdim,AP_SCALAR_MPFR);
  printf("\n");
  ap_texpr0_free(a);
}

void test_op()
{
  /* creation, copy, comparison */
  ap_texpr0_t* x = ADD(V(0),MUL(NEG(ADD(V(4),I(1,2.5))),C(3)));
  ap_texpr0_t* y = ap_texpr0_copy(x);
  print_info(y);
  assert(ap_texpr0_equal(x,y));
  ap_texpr0_free(x);
  ap_texpr0_free(y);

  x = ADDF(V(1),MUL(NEG(ADD(V(4),I(1,1))),C(3)));
  y = ADD(V(1),MUL(NEG(ADD(V(4),I(1,1))),V(3)));
  print_info(x);
  print_info(y);
  assert(!ap_texpr0_equal(x,y));
  ap_texpr0_free(x);   
  ap_texpr0_free(y);   

  x = DIV(V(1),MUL(NEG(ADD(V(4),I(1,1))),C(3)));
  print_info(x);
  ap_texpr0_free(x);   

  /* from linexpr0 */
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,12);
  ap_linexpr0_fprint(stdout,l,NULL); printf("\n");
  x = ap_texpr0_from_linexpr0(l);
  print_info(x);
  ap_texpr0_free(x);
  ap_linexpr0_set_list(l,AP_CST_S_INT,12,AP_COEFF_I_INT,1,2,42,AP_COEFF_I_INT,-1,1,3,AP_END);
  ap_linexpr0_fprint(stdout,l,NULL); printf("\n");
  x = ap_texpr0_from_linexpr0(l);
  print_info(x);
  ap_texpr0_free(x);
  ap_linexpr0_free(l);

  /* substitutions */
  x = DIV(V(1),MUL(SQRT(ADD(V(4),I(1,1))),C(3)));
  y = ADD(C(1),V(1));
  print_info(x);
  ap_texpr0_substitute_with(x, 1, y);
  ap_texpr0_substitute_with(x, 1, y);
  print_info(x);
  ap_texpr0_free(x);   
  ap_texpr0_free(y);

  /* dimchanges */
  ap_dim_t dt[] = { 0,1,2,2,4 };
  ap_dimchange_t d = { dt,3,1 };
  x = DIV(MUL(V(0),V(1)),MUL(NEG(ADD(V(2),C(3))),V(3)));
  print_info(x);
  ap_dimchange_fprint(stdout, &d);
  y = ap_texpr0_add_dimensions(x, &d);
  print_info(y);
  ap_texpr0_free(x);
  ap_dimchange_add_invert(&d);
  ap_dimchange_fprint(stdout, &d);
  x = ap_texpr0_remove_dimensions(y, &d);
  print_info(x);
  ap_texpr0_free(x);
  ap_texpr0_free(y);
}



void test_lin()
{
  int i;
  double it[10][2] =
    { {2,4}, {-4,-3}, {0,2}, {-2,2}, {0.5,1.75} };

  ap_interval_t** itv = ap_interval_array_alloc(sizeof(it)/sizeof(it[0]));
  for (i=0;i<sizeof(it)/sizeof(it[0]);i++)
    ap_interval_set_double(itv[i],it[i][0],it[i][1]);

  man = box_manager_alloc();

  testlinearize(ADD(V(0),ADD(MUL(C(2),V(1)),C(3))),itv,4);
  testlinearize(ADD(V(1),SUB(C(3),MUL(C(2),V(1)))),itv,4);
  testlinearize(MUL(ADD(I(0,1),C(2)),NEG(MUL(V(0),C(3)))),itv,4);
  testlinearize(MUL(V(0),V(1)),itv,4);
  testlinearize(MUL(V(1),V(0)),itv,4);
  testlinearize(MUL(SQRT(MUL(C(3),ADD(C(1),V(0)))),V(1)),itv,4);
  testlinearize(DIV(ADD(MUL(V(0),I(0,1)),I(1,2)),ADD(MUL(V(2),I(1,3)),I(1,2))),itv,4);
  testlinearize(MULI(I(2,5),V(0)),itv,0);
  testlinearize(MULI(I(2,5),V(0)),itv,4);
  testlinearize(MULF(I(2,5),V(0)),itv,4);
  testlinearize(MULF(I(0.1,0.2),V(0)),itv,4);
  testlinearize(SUB(MULF(V(1),I(2,3)),ADD(V(1),C(3))),itv,4);
  testlinearize(MOD(ADD(V(0),I(1,2)),I(4,5)),itv,4);

  ap_manager_free(man);
  ap_interval_array_free(itv,sizeof(it)/sizeof(it[0]));
}

int main()
{
  ap_fpu_init();
  mpfr_set_default_prec(4046);
  test_op();
  test_lin();
  return 0;
}
