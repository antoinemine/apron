/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include "ap_global1.h"

#include "num.h"
#include "itv.h"
#include "t1p_internal.h"
#include "t1p_fun.h"
#include "t1p_constructor.h"
#include "t1p_representation.h"


t1p_aff_t* afexpr_random_cst(t1p_internal_t* pr, unsigned int seed)
{
    itv_t itv;
    num_t num_dbl;
    double dbl;
    t1p_aff_t* afexpr = t1p_aff_alloc_init(pr);
    srand(seed);
    dbl = (double)((rand() - rand())/ (1.0*rand()));
    num_set_double(num_dbl, dbl);
    itv_set_num(itv, num_dbl);
    itv_set(afexpr->c, itv);
    itv_set(afexpr->itv, itv);
    return afexpr;
}

t1p_aff_t* afexpr_random_nb(t1p_internal_t* pr, unsigned int seed, size_t Nb)
{
    itv_t itv;
    itv_init(itv);
    num_t num_dbl;
    num_init(num_dbl);
    double dbl;
    size_t tmp;
    t1p_aff_t* afexpr = t1p_aff_alloc_init(pr);
    dbl = (double)((rand() - rand())/ (1.0*rand()));
    num_set_double(num_dbl, dbl);
    itv_set_num(itv, num_dbl);
    itv_set(afexpr->c, itv);
    itv_set(afexpr->itv, itv);
    srand(seed);
    while (afexpr->l<Nb) {
	dbl = (double)((rand() - rand())/ (1.0*rand()));
	num_set_double(num_dbl, dbl);
	itv_set_num(itv, num_dbl);
	if (afexpr->l<Nb - 3) t1p_aff_nsym_create(pr, afexpr, itv, IN);
	else t1p_aff_nsym_create(pr, afexpr, itv, UN);
    }
    itv_clear(itv);
    num_clear(num_dbl);
    return afexpr;
}

/* multiplication */
/* 
   - 0 x random = 0
   - bottom x random = bottom
   - top x random = top
   - exp1 x exp2 = exp3 (compared with Fluctuat)
 */
void test_mul_non_constrained(t1p_internal_t* pr, t1p_t* env)
{
    printf("### aff x aff ###\n");
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);
    itv_t coeff;
    itv_init(coeff);
    num_t num;
    num_init(num);
    env->paf[0] = t1p_aff_alloc_init(pr);
    long int seed48 = time(NULL);
    env->paf[1] = afexpr_random_nb(pr, (unsigned int) (seed48 % UINT_MAX), 10);
    /* 0 x random = 0 */
    env->paf[2] = t1p_aff_mul(pr, env->paf[0], env->paf[1], env);
    if (t1p_aff_is_zero(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* bottom x random = bottom */
    env->paf[2] = t1p_aff_mul(pr, pr->bot, env->paf[1], env);
    if (t1p_aff_is_bottom(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* top x random = top */
    env->paf[2] = t1p_aff_mul(pr, pr->top, env->paf[1], env);
    if (t1p_aff_is_top(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* exp1 = -2.5 + .5\eps1 + \eps2 ; [-4,-1]
       exp2 = exp1 x exp1
       (fluctuat) exp2 = 6.875 - 2.5\eps1 -5\eps2 + 1.625\eps_f ; [1,16]
       (t1+) exp2 = 6.25 - 2.5\eps1 -5\eps2 + [0,2.25]	; [1,16]
       = 7.375  - 2.5\eps1 -5\eps2 + 1.125\eps_f	; [1,16]
     */
    t1p_aff_check_free(pr, env->paf[1]);
    env->paf[1] = t1p_aff_alloc_init(pr);
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[1]->c, coeff);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 0);
    num_set_double(num, 1);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 1);
    itv_set_int2(env->paf[1]->itv, -4, -1);
    env->paf[2] = t1p_aff_mul(pr, env->paf[1], env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, 7.375);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, -5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, 1.125);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact->itv, 1, 16);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact)) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* exp1 = -2.5 + .5\eps1 + \eps2 ; [-4,-1]
       exp0 = 1.5 + eps1 - 0.5\eps3 ; [0,3]
       exp2 = exp1 x exp0
       (fluctuat, t1+) exp2 = -3.5 - 1.75\eps1 + 1.5\eps2 + 1.25\eps3 + 2\eps_f ; [-10,0]
     */
    t1p_aff_check_free(pr, env->paf[0]);
    env->paf[0] = t1p_aff_alloc_init(pr);
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, 1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, -0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 2);
    itv_set_int2(env->paf[0]->itv, 0, 3);
    env->paf[2] = t1p_aff_mul(pr, env->paf[0], env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact1 = t1p_aff_alloc_init(pr);
    num_set_double(num, -3.5);
    itv_set_num(coeff, num);
    itv_set(aff_exact1->c, coeff);
    num_set_double(num, -1.75);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 0);
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 1);
    num_set_double(num, 1.25);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 2);
    num_set_double(num, 2);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact1->itv, -10, 0);

    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact1)) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* exp1 = -2.5 + .5\eps1 + \eps2 ; [-4,-1]
       lambda = [1,1]
       exp2 = lambda x exp1
       exp2 = exp1
     */
    itv_t lambda; itv_init(lambda);
    itv_set_int(lambda, 1);
    env->paf[2] = t1p_aff_mul_itv(pr, env->paf[1], lambda);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], env->paf[1])) printf("o");
    else printf("n");

    /* exp1 = 2 + eps0 - eps1 + 1.5eps3	; [-1.5,5.5]
       exp2 = -1 + eps0 + 0.5eps2 + 0.75eps3 + eps4 ; [1.25,5.25]
       exp3 = exp1*exp2 
       = (fluctuat) -0.9375 + eps0 + eps1 + eps2 + 2eps4 + 10.3125epsf; [-16.25,12.375]
     */
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);

    env->paf[0] = t1p_aff_alloc_init(pr);
    num_set_double(num, 2);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, 1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, -1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 1);
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 3);
    num_set_double(num, 1.5);
    bound_set_num(env->paf[0]->itv->inf,num);
    num_set_double(num, 5.5);
    bound_set_num(env->paf[0]->itv->sup,num);

    env->paf[1] = t1p_aff_alloc_init(pr);
    num_set_double(num, -1);
    itv_set_num(coeff, num);
    itv_set(env->paf[1]->c, coeff);
    num_set_double(num, 1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 0);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 2);
    num_set_double(num, 0.75);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 3);
    num_set_double(num, 1);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 4);
    num_set_double(num, 4.25);
    bound_set_num(env->paf[1]->itv->inf,num);
    num_set_double(num, 2.25);
    bound_set_num(env->paf[1]->itv->sup,num);
    env->paf[2] = t1p_aff_mul(pr, env->paf[0], env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_check_free(pr, aff_exact);
    aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, -0.9375);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, 1.);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, 1.);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, 1.);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 2);
    num_set_double(num, 2);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 4);
    num_set_double(num, 10.3125);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    num_set_double(num, 16.25);
    bound_set_num(aff_exact->itv->inf,num);
    num_set_double(num, 12.375);
    bound_set_num(aff_exact->itv->sup,num);

    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact)) printf("o");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    itv_clear(lambda);
    t1p_aff_free(pr, aff_exact);
    t1p_aff_free(pr, aff_exact1);
    printf("\n");
}

void test_mul_constrained(t1p_internal_t* pr, t1p_t* env)
{
    printf("### aff x aff (constrained) ###\n");
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);
    itv_t coeff;
    itv_init(coeff);
    num_t num;
    num_init(num);

    env->paf[0] = t1p_aff_alloc_init(pr);
    num_set_double(num, 1.094658404232323301);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, -0.48202531945707516314);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, 0.93829687951457496631);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 1);
    num_set_double(num, -0.33333333333333331483);
    bound_set_num(env->paf[0]->itv->inf,num);
    num_set_double(num, 2.5149806032039729864);
    bound_set_num(env->paf[0]->itv->sup,num);
    /*
       num_set_double(num, 1);
       itv_set_num(coeff, num);
       itv_set(env->paf[0]->c, coeff);
       num_set_double(num, -0.5);
       itv_set_num(coeff, num);
       t1p_aff_build(pr, env->paf[0], coeff, 0);
       num_set_double(num, 1);
       itv_set_num(coeff, num);
       t1p_aff_build(pr, env->paf[0], coeff, 1);
       num_set_double(num, 0.0);
       bound_set_num(env->paf[0]->itv->inf,num);
       num_set_double(num, 2.5);
       bound_set_num(env->paf[0]->itv->sup,num);
     */

    t1p_aff_t* aff = t1p_aff_alloc_init(pr);
    itv_set_int(coeff, -1);
    t1p_aff_build(pr, aff, coeff, 0);
    ap_linexpr0_t* linexpr0 = t1p_ap_linexpr0_set_aff(pr, aff, env);
    ap_lincons0_t lincons0;
    lincons0.constyp = AP_CONS_SUPEQ;
    lincons0.linexpr0 = linexpr0;
    lincons0.scalar = NULL;
    ap_lincons0_array_t eps_lincons_array;
    eps_lincons_array.size = 1;
    eps_lincons_array.p = &lincons0;
    ap_abstract0_meet_lincons_array(pr->manNS, true, env->abs, &eps_lincons_array);
    t1p_update_nsymcons_gamma(pr, env);

    env->paf[2] = t1p_aff_mul(pr, env->paf[0], env->paf[0], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact1 = t1p_aff_alloc_init(pr);
    num_set_double(num, 1.8355754144819342866);
    itv_set_num(coeff, num);
    itv_set(aff_exact1->c, coeff);
    num_set_double(num, -1.2876545425906109621);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 0);
    num_set_double(num, 2.5065119827447759349);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 1);
    num_set_double(num, 0.69538549467490184952);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, (-1 + pr->dim));
    num_set_double(num, -0.11111111111111109107);
    bound_set_num(aff_exact1->itv->inf,num);
    num_set_double(num, 6.3251274344922201465);
    bound_set_num(aff_exact1->itv->sup,num);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact1)) printf("o");
    if (t1p_aff_is_tild_eq(pr, env->paf[2], aff_exact1)) printf("O");
    else printf("n");

    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[2]);

    env->paf[0] = t1p_aff_alloc_init(pr);
    num_set_double(num, 2.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, 20);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, 2.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 1);
    num_set_double(num, 20);
    bound_set_num(env->paf[0]->itv->inf,num);
    num_set_double(num, 5);
    bound_set_num(env->paf[0]->itv->sup,num);

    env->paf[1] = t1p_aff_alloc_init(pr);
    num_set_double(num, -1.25);
    itv_set_num(coeff, num);
    itv_set(env->paf[1]->c, coeff);
    num_set_double(num, 10);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 0);
    num_set_double(num, 1.25);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 2);
    num_set_double(num, 12.5);
    bound_set_num(env->paf[1]->itv->inf,num);
    num_set_double(num, 0);
    bound_set_num(env->paf[1]->itv->sup,num);

    env->paf[2] = t1p_aff_mul(pr, env->paf[0], env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, -28.125);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, -200);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, -15.625);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, -9.375);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 2);
    num_set_double(num, 53.125);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    num_set_double(num, 62.5);
    bound_set_num(aff_exact->itv->inf,num);
    num_set_double(num, 250);
    bound_set_num(aff_exact->itv->sup,num);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact)) printf("o");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    //itv_clear(lambda);
    t1p_aff_free(pr, aff_exact);
    t1p_aff_free(pr, aff);
    ap_lincons0_clear(&lincons0);
    t1p_aff_free(pr, aff_exact1);
    printf("\n");
}


/* division */
/* 
   - random / 0 = top
   - random / 1 = random
   - exp1 / exp2 = exp3 (compared with Fluctuat)
   - exp1 / [1, +oo] = exp4 (compared with Fluctuat)
   - exp1 / [-oo, -1] = exp5 (compared with Fluctuat)
 */

void test_div_non_constrained(t1p_internal_t* pr, t1p_t* env)
{
    printf("### aff / aff ###\n");
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);
    itv_t coeff, lambda ;
    itv_init(coeff);
    itv_init(lambda);
    num_t num;
    num_init(num);
    env->paf[0] = t1p_aff_alloc_init(pr);
    long int seed48 = time(NULL);
    env->paf[1] = afexpr_random_nb(pr, (unsigned int) (seed48 % UINT_MAX), 10);
    /* random / top = top */
    env->paf[2] = t1p_aff_div(pr, env->paf[1], pr->top, env);
    if (t1p_aff_is_top(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* random / 0 = top */
    env->paf[2] = t1p_aff_div(pr, env->paf[1], env->paf[0], env);
    if (t1p_aff_is_top(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* randim / 1 = random */
    t1p_aff_t * dummy = t1p_aff_alloc_init(pr);
    itv_set_int(dummy->c, 1);
    itv_set_int(dummy->itv, 1);
    env->paf[2] = t1p_aff_div(pr, env->paf[1], dummy, env);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[1], env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* exp1 = -2.5 + .5\eps1 + \eps2 ; [-4,-1]
       exp2 = exp1 / [1, +oo]
       (fluctuat) exp2 = -1.25 + 2.75\eps_U ; [-4,0]
       (t1+) exp2 = -1.25 + 0.25\eps1 + 0.5\eps2 + 2\eps_f ; [-4,0]
     */
    itv_set_top(dummy->c);
    bound_set_int(dummy->c->inf, -1);
    itv_set(dummy->itv, dummy->c);
    t1p_aff_check_free(pr, env->paf[1]);
    env->paf[1] = t1p_aff_alloc_init(pr);
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[1]->c, coeff);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 0);
    num_set_double(num, 1);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 1);
    itv_set_int2(env->paf[1]->itv, -4, -1);
    env->paf[2] = t1p_aff_div(pr, env->paf[1], dummy, env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, -1.25);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, 0.25);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, 2);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact->itv, -4, 0);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact)) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    /* exp1 = -2.5 + .5\eps1 + \eps2 ; [-4,-1]
       exp0 = 1.5 + eps1 - 0.5\eps3 ; [0,3]
       exp2 = exp0 / exp1
       (fluctuat, t1+) exp2 = -0.91 - 0.7\eps1 - 0.24\eps2 + 0.29\eps3 + 0.86\eps_f ; [-3,0]
     */
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, 1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, -0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 2);
    itv_set_int2(env->paf[0]->itv, 0, 3);
    env->paf[2] = t1p_aff_div(pr, env->paf[0], env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact1 = t1p_aff_alloc_init(pr);
    num_set_double(num, -0.91);
    itv_set_num(coeff, num);
    itv_set(aff_exact1->c, coeff);
    num_set_double(num, -0.7);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 0);
    num_set_double(num, -0.24);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 1);
    num_set_double(num, 0.29);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 2);
    num_set_double(num, 0.86);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact1->itv, -3, 0);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact1)) printf("o");
    else if (t1p_aff_is_tild_eq(pr, env->paf[2], aff_exact1)) printf("O");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    itv_clear(lambda);
    t1p_aff_free(pr, dummy);
    t1p_aff_free(pr, aff_exact);
    t1p_aff_free(pr, aff_exact1);
    printf("\n");
}
/* sqrt */
/*
   - sqrt([-1,0]) = bottom
   - sqrt(bot) = bot
   - sqrt(top) = top
   - exp2 = sqrt(exp0) (compared with fluctuat)
 */
void test_sqrt_non_constrained(t1p_internal_t* pr, t1p_t* env)
{
    printf("### sqrt(aff) ###\n");
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);
    itv_t coeff, lambda ;
    itv_init(coeff);
    itv_init(lambda);
    num_t num;
    num_init(num);
    env->paf[0] = t1p_aff_alloc_init(pr);
    /* sqrt(0) = 0 */
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[0], env);
    if (t1p_aff_is_zero(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* exp1 < 0; sqrt(exp1) = bottom */
    env->paf[1] = t1p_aff_alloc_init(pr);
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[1]->c, coeff);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 0);
    num_set_double(num, 1);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[1], coeff, 1);
    itv_set_int2(env->paf[1]->itv, -4, -1);
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[1], env);
    t1p_aff_reduce(pr, env->paf[2]);
    if (t1p_aff_is_bottom(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* exp0 = -exp1 = 2.5 - .5\eps1 - \eps2 ; [1,4]
       exp2 = sqrt(exp0)
       (fluctuat, t1+) exp2 = 1.527 -0.158\eps1 -0.316\eps2 + 0.053\eps_f ; [1,2]
     */
    t1p_aff_check_free(pr, env->paf[0]);
    env->paf[0] = t1p_aff_neg(pr, env->paf[1]);
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[0], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, 1.5277402395547232);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, -0.1581138830084189);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, -0.316227766016837);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, 0.053398590529466938);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact->itv, 1, 2);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact)) printf("o");
    else if (t1p_aff_is_tild_eq(pr, env->paf[2], aff_exact)) printf("O");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);
    /* exp0 = 1.5 + 1\eps1 - 0.5\eps2 ; [0,3]
       exp2 = sqrt(exp0)
       (fluctuat, t1+) exp2 = 0.918 + 0.40\eps1 -0.20\eps2 + 0.30\eps_f ; [0,1.73...]
     */
    t1p_aff_check_free(pr, env->paf[0]);
    env->paf[0] = t1p_aff_alloc_init(pr);
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    itv_set(env->paf[0]->c, coeff);
    num_set_double(num, 1.0);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 0);
    num_set_double(num, -0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, env->paf[0], coeff, 2);
    itv_set_int2(env->paf[0]->itv, 0, 3);
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[0], env);
    t1p_aff_reduce(pr, env->paf[2]);

    t1p_aff_t *aff_exact1 = t1p_aff_alloc_init(pr);
    num_set_double(num, 0.91855865354369);
    itv_set_num(coeff, num);
    itv_set(aff_exact1->c, coeff);
    num_set_double(num, 0.408248290463863);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 0);
    num_set_double(num, -0.2041241452319315);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, 1);
    num_set_double(num, 0.306186217847897);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact1, coeff, (-1 + pr->dim));
    num_set_double(num, 1.7320508075688);
    itv_set_num(aff_exact1->itv, num);
    bound_set_int(aff_exact1->itv->inf, 0);
    if (t1p_aff_is_perfectly_eq(pr, env->paf[2], aff_exact1)) printf("o");
    else if (t1p_aff_is_tild_eq(pr, env->paf[2], aff_exact1)) printf("O");
    else printf("n");
    /* sqrt(bottom) = bottom */
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[2]);
    env->paf[0] = pr->bot;
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[0], env);
    if (t1p_aff_is_bottom(pr, env->paf[2])) printf("o");
    else printf("n");
    /* sqrt(top) = [0,+oo] + caveat */
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[2]);
    env->paf[0] = pr->top;
    env->paf[2] = t1p_aff_sqrt(pr, env->paf[0], env);
    itv_t o_oo; itv_init(o_oo);
    itv_set_top(o_oo);
    bound_set_int(o_oo->inf,0);
    if (itv_is_leq(o_oo, env->paf[2]->itv)) printf("o");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    itv_clear(lambda);
    itv_clear(o_oo);
    t1p_aff_free(pr, aff_exact);
    t1p_aff_free(pr, aff_exact1);
    printf("\n");
}

/* addition / subtraction */
// TODO tester que rand1 + rand2 = rand2 + rand1
void test_pm_non_constrained(t1p_internal_t* pr, t1p_t* env)
{
    printf("### aff +/- aff ###\n");
    t1p_aff_check_free(pr, env->paf[0]);
    t1p_aff_check_free(pr, env->paf[1]);
    t1p_aff_check_free(pr, env->paf[2]);
    itv_t coeff;
    itv_init(coeff);
    num_t num;
    num_init(num);
    long int seed48 = time(NULL);
    env->paf[0] = afexpr_random_nb(pr, (unsigned int) (seed48 % UINT_MAX), 10);
    /* random - random  = 0 */
    env->paf[2] = t1p_aff_sub(pr, env->paf[0], env->paf[0], env);
    if (t1p_aff_is_zero(pr, env->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, env->paf[2]);

    env->paf[1] = t1p_aff_neg(pr, env->paf[0]);
    env->paf[2] = t1p_aff_add(pr, env->paf[0], env->paf[1], env);
    if (t1p_aff_is_zero(pr, env->paf[2])) printf("o");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    //itv_clear(lambda);
    //t1p_aff_free(pr, aff_exact);
    //t1p_aff_free(pr, aff_exact1);
    printf("\n");
}

/* some texpr */
void test_eval_texpr_non_constrained(t1p_internal_t* pr, t1p_t* abs)
{
    printf("### eval texp ###\n");
    t1p_aff_check_free(pr, abs->paf[0]);
    //    t1p_aff_check_free(pr, abs->paf[1]);
    t1p_aff_check_free(pr, abs->paf[2]);
    itv_t coeff, lambda ;
    itv_init(coeff);
    itv_init(lambda);
    num_t num;
    num_init(num);
    /* x = 0
       y = sqrt(x)*sqrt(x) 
       y = 0
     */
    abs->paf[0] = t1p_aff_alloc_init(pr);
    abs->paf[0]->pby++;
    ap_texpr0_t* texp = ap_texpr0_binop(AP_TEXPR_MUL,
	    ap_texpr0_unop(AP_TEXPR_SQRT,ap_texpr0_dim(0),AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    ap_texpr0_unop(AP_TEXPR_SQRT,ap_texpr0_dim(0),AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);
    abs->paf[2] = t1p_aff_eval_ap_texpr0(pr, texp, abs);
    if (t1p_aff_is_zero(pr, abs->paf[2])) printf("o");
    else printf("n");
    t1p_aff_check_free(pr, abs->paf[2]);
    t1p_aff_check_free(pr, abs->paf[0]);
    /* x = bottom
       y = sqrt(x)*sqrt(x) 
       y = bottom
     */
    abs->paf[0] = pr->bot;
    abs->paf[0]->pby++;
    abs->paf[2] = t1p_aff_eval_ap_texpr0(pr, texp, abs);
    //t1p_aff_fprint(pr, stdout, abs->paf[2]);
    if (t1p_aff_is_bottom(pr, abs->paf[2])) printf("o");
    else printf("n");

    /* rump polynome
       x = 77617
       y = 33096
       z = 1335y^6/4 + x*x(11x^2y^2 - y^6 - 121y^4 - 2) + 11y^8/2 + x/2y
       z = a + b + c + d
     */
    t1p_aff_check_free(pr, abs->paf[0]);
    t1p_aff_check_free(pr, abs->paf[1]);
    t1p_aff_check_free(pr, abs->paf[2]);

    abs->paf[0] = t1p_aff_alloc_init(pr);
    abs->paf[0]->pby++;
    abs->paf[1] = t1p_aff_alloc_init(pr);
    abs->paf[1]->pby++;

    itv_set_int(abs->paf[0]->c, 77617);
    itv_set_int(abs->paf[0]->itv, 77617);
    itv_set_int(abs->paf[1]->c, 33096);
    itv_set_int(abs->paf[1]->itv, 33096);

    ap_texpr0_t* a = ap_texpr0_binop(AP_TEXPR_DIV,
	    ap_texpr0_binop(AP_TEXPR_MUL,
		ap_texpr0_cst_scalar_int(1335),
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_binop(AP_TEXPR_MUL,
			ap_texpr0_binop(AP_TEXPR_MUL,
			    ap_texpr0_binop(AP_TEXPR_MUL,
				ap_texpr0_binop(AP_TEXPR_MUL,
				    ap_texpr0_dim(1),
				    ap_texpr0_dim(1),
				    AP_RTYPE_DOUBLE,AP_RDIR_UP),
				ap_texpr0_dim(1),
				AP_RTYPE_DOUBLE,AP_RDIR_UP),
			    ap_texpr0_dim(1),
			    AP_RTYPE_DOUBLE,AP_RDIR_UP),
			ap_texpr0_dim(1),
			AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    ap_texpr0_dim(1),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    ap_texpr0_cst_scalar_int(4),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    ap_texpr0_t* bb = ap_texpr0_binop(AP_TEXPR_SUB,
	    ap_texpr0_binop(AP_TEXPR_SUB,
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_cst_scalar_int(11),
		    ap_texpr0_binop(AP_TEXPR_MUL,
			ap_texpr0_dim(0),
			ap_texpr0_binop(AP_TEXPR_MUL,
			    ap_texpr0_dim(0),
			    ap_texpr0_binop(AP_TEXPR_MUL,
				ap_texpr0_dim(1),
				ap_texpr0_dim(1),
				AP_RTYPE_DOUBLE,AP_RDIR_UP),
			    AP_RTYPE_DOUBLE,AP_RDIR_UP),
			AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_dim(1),
		    ap_texpr0_binop(AP_TEXPR_MUL,
			ap_texpr0_dim(1),
			ap_texpr0_binop(AP_TEXPR_MUL,
			    ap_texpr0_dim(1),
			    ap_texpr0_binop(AP_TEXPR_MUL,
				ap_texpr0_dim(1),
				ap_texpr0_binop(AP_TEXPR_MUL,
				    ap_texpr0_dim(1),
				    ap_texpr0_dim(1),
				    AP_RTYPE_DOUBLE,AP_RDIR_UP),
				AP_RTYPE_DOUBLE,AP_RDIR_UP),
			    AP_RTYPE_DOUBLE,AP_RDIR_UP),
			AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
		ap_texpr0_binop(AP_TEXPR_ADD,
			ap_texpr0_binop(AP_TEXPR_MUL,
			    ap_texpr0_cst_scalar_int(121),
			    ap_texpr0_binop(AP_TEXPR_MUL,
				ap_texpr0_dim(1),
				ap_texpr0_binop(AP_TEXPR_MUL,
				    ap_texpr0_dim(1),
				    ap_texpr0_binop(AP_TEXPR_MUL,
					ap_texpr0_dim(1),
					ap_texpr0_dim(1),
					AP_RTYPE_DOUBLE,AP_RDIR_UP),
				    AP_RTYPE_DOUBLE,AP_RDIR_UP),
				AP_RTYPE_DOUBLE,AP_RDIR_UP),
			    AP_RTYPE_DOUBLE,AP_RDIR_UP),
			ap_texpr0_cst_scalar_int(2),
			AP_RTYPE_DOUBLE,AP_RDIR_UP),
		AP_RTYPE_DOUBLE,AP_RDIR_UP);

    ap_texpr0_t* b = ap_texpr0_binop(AP_TEXPR_MUL,
	    ap_texpr0_dim(0),
	    ap_texpr0_binop(AP_TEXPR_MUL,
		ap_texpr0_dim(0),
		bb,
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    ap_texpr0_t* c = ap_texpr0_binop(AP_TEXPR_DIV,
	    ap_texpr0_binop(AP_TEXPR_MUL,
		ap_texpr0_cst_scalar_int(11),
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_binop(AP_TEXPR_MUL,
			ap_texpr0_binop(AP_TEXPR_MUL,
			    ap_texpr0_binop(AP_TEXPR_MUL,
				ap_texpr0_binop(AP_TEXPR_MUL,
				    ap_texpr0_binop(AP_TEXPR_MUL,
					ap_texpr0_binop(AP_TEXPR_MUL,
					    ap_texpr0_dim(1),
					    ap_texpr0_dim(1),
					    AP_RTYPE_DOUBLE,AP_RDIR_UP),
					ap_texpr0_dim(1),
					AP_RTYPE_DOUBLE,AP_RDIR_UP),
				    ap_texpr0_dim(1),
				    AP_RTYPE_DOUBLE,AP_RDIR_UP),
				ap_texpr0_dim(1),
				AP_RTYPE_DOUBLE,AP_RDIR_UP),
			    ap_texpr0_dim(1),
			    AP_RTYPE_DOUBLE,AP_RDIR_UP),
			ap_texpr0_dim(1),
			AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    ap_texpr0_dim(1),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		    ap_texpr0_cst_scalar_int(2),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    ap_texpr0_t* d = ap_texpr0_binop(AP_TEXPR_DIV,
	    ap_texpr0_dim(0),
	    ap_texpr0_binop(AP_TEXPR_MUL,
		ap_texpr0_cst_scalar_int(2),
		ap_texpr0_dim(1),
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    ap_texpr0_t* rump = ap_texpr0_binop(AP_TEXPR_ADD,
	    ap_texpr0_binop(AP_TEXPR_ADD,
		a, b,
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    ap_texpr0_binop(AP_TEXPR_ADD,
		c, d,
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    abs->paf[2] = t1p_aff_eval_ap_texpr0(pr, rump, abs);
    /* x = -2.5 + 0.5*eps1 + eps2;
       z = 2x - x - x;
     */
    t1p_aff_check_free(pr, abs->paf[1]);
    t1p_aff_check_free(pr, abs->paf[2]);

    abs->paf[1] = t1p_aff_alloc_init(pr);
    abs->paf[1]->pby++;
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    itv_set(abs->paf[1]->c, coeff);
    num_set_double(num, 0.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, abs->paf[1], coeff, 0);
    num_set_double(num, 1);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, abs->paf[1], coeff, 1);
    itv_set_int2(abs->paf[1]->itv, -4, -1);

    ap_texpr0_free(texp);
    texp = ap_texpr0_binop(AP_TEXPR_SUB,
	    ap_texpr0_binop(AP_TEXPR_SUB,
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_cst_scalar_int(2),
		    ap_texpr0_dim(1),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		ap_texpr0_dim(1),
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    ap_texpr0_dim(1),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);
    abs->paf[2] = t1p_aff_eval_ap_texpr0(pr, texp, abs);
    if (t1p_aff_is_zero(pr, abs->paf[2])) printf("o");
    else printf("n");

    ap_texpr0_free(texp);
    texp = ap_texpr0_binop(AP_TEXPR_MUL,
	    ap_texpr0_dim(0),
	    ap_texpr0_binop(AP_TEXPR_SUB,
		ap_texpr0_binop(AP_TEXPR_MUL,
		    ap_texpr0_cst_scalar_int(2),
		    ap_texpr0_dim(1),
		    AP_RTYPE_DOUBLE,AP_RDIR_UP),
		ap_texpr0_dim(1),
		AP_RTYPE_DOUBLE,AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,AP_RDIR_UP);

    t1p_aff_check_free(pr, abs->paf[0]);
    abs->paf[0] = t1p_aff_alloc_init(pr);
    abs->paf[0]->pby++;
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    itv_set(abs->paf[0]->c, coeff);
    num_set_double(num, 1.5);
    itv_set_num(coeff, num);
    t1p_aff_nsym_create(pr, abs->paf[0], coeff, UN);
    itv_set_int2(abs->paf[0]->itv, -4, -1);

    t1p_aff_check_free(pr, abs->paf[2]);
    abs->paf[2] = t1p_aff_eval_ap_texpr0(pr, texp, abs);
    t1p_aff_reduce(pr, abs->paf[2]);

    t1p_aff_t *aff_exact = t1p_aff_alloc_init(pr);
    num_set_double(num, 6.25);
    itv_set_num(coeff, num);
    itv_set(aff_exact->c, coeff);
    num_set_double(num, -1.25);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 0);
    num_set_double(num, -2.5);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, 1);
    num_set_double(num, -3.75);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-2 + pr->dim));
    num_set_double(num, 2.25);
    itv_set_num(coeff, num);
    t1p_aff_build(pr, aff_exact, coeff, (-1 + pr->dim));
    itv_set_int2(aff_exact->itv, 1, 16);
    if (t1p_aff_is_perfectly_eq(pr, abs->paf[2], aff_exact)) printf("o");
    else printf("n");

    num_clear(num);
    itv_clear(coeff);
    itv_clear(lambda);
    ap_texpr0_free(texp);
    ap_texpr0_free(rump);
    //t1p_aff_free(pr, dummy);
    t1p_aff_free(pr, aff_exact);
    //t1p_aff_free(pr, aff_exact1);
    printf("\n");
}

int main(void)
{
    //	mtrace();
    //long int seed48 = time(NULL);
    //unsigned int seed = (unsigned int) (seed48 % UINT_MAX);
    //srand(seed);
    ap_manager_t* man = t1p_manager_alloc();
    t1p_internal_t * pr = t1p_init_from_manager(man, AP_FUNID_UNKNOWN);
    t1p_t* abs = t1p_top(man, 0, 3);

    test_pm_non_constrained(pr, abs);
    test_mul_non_constrained(pr, abs);
    test_div_non_constrained(pr, abs);
    test_sqrt_non_constrained(pr, abs);
    test_eval_texpr_non_constrained(pr, abs);

    test_mul_constrained(pr, abs);

    /* free managers */
    t1p_free(man, abs);
    ap_manager_free(man);
    return 0;
}

