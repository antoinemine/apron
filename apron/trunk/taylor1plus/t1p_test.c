/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <mcheck.h>
#include "ap_global1.h"

#include "num.h"
#include "itv.h"
#include "t1p_internal.h"
#include "t1p_fun.h"

#define NB	50
#define MAX_SIZE	1000000

struct timespec time1, time2;
double t;
clock_t start_m, end_m;
double t1;

#define TIME(action)	\
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);	\
action						\
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);	\
t = (time2.tv_nsec - time1.tv_nsec)*1e-6 + (time2.tv_sec - time1.tv_sec)*1e-3;

#define TIME1(action)	\
    start_m = clock();	\
action						\
end_m = clock();	\
t1 = ((end_m - start_m)) / (1.0*CLOCKS_PER_SEC); 


t1p_afexpr_t* afexpr_random_cst(t1p_internal_t* pr, unsigned int seed)
{
    itv_t itv;
    num_t num_dbl;
    double dbl;
    size_t tmp;
    t1p_afexpr_t* afexpr = t1p_afexpr_alloc(pr);
    dbl = (double)((rand() - rand())/ (1.0*rand()));
    num_set_double(num_dbl, dbl);
    itv_set_num(itv, num_dbl);
    t1p_afexpr_init_cst(pr, afexpr, itv);
    return afexpr;
}

t1p_afexpr_t* afexpr_random_nb(t1p_internal_t* pr, unsigned int seed, size_t Nb)
{
    itv_t itv;
    num_t num_dbl;
    double dbl;
    size_t tmp;
    t1p_afexpr_t* afexpr = t1p_afexpr_alloc(pr);
    dbl = (double)((rand() - rand())/ (1.0*rand()));
    num_set_double(num_dbl, dbl);
    itv_set_num(itv, num_dbl);
    t1p_afexpr_init_cst(pr, afexpr, itv);
    srand(seed);
    //	pr->index = 0;
    while (afexpr->val.aff->l<Nb) {
	dbl = (double)((rand() - rand())/ (1.0*rand()));
	num_set_double(num_dbl, dbl);
	itv_set_num(itv, num_dbl);
	t1p_aff_nsym_create(pr, afexpr->val.aff, itv, NSYM);
    }
    return afexpr;
}

void afexpr_sqrt(ap_manager_t* man)
{
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    t1p_afexpr_t *a;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    int i = 0;
    for(i=1; i<10; i+=2) {
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), i);
	t1p_afexpr_sqrt(pr, res, a);
	printf("============== sqrt =================\n");
	printf("a:\n");
	t1p_afexpr_fprint(pr, stdout, a);
	printf("\nfin a/\n res:\n");
	t1p_afexpr_fprint(pr, stdout, res);
	printf("\nfin res/\n");
	pr->index = 0;
	t1p_afexpr_clear(pr, a);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
    }
    t1p_afexpr_free(pr,res);
    t1p_afexpr_free(pr, a);
}
void afexpr_mul_cst(ap_manager_t* man)
{
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    t1p_afexpr_t *a,*b;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    int i = 0;

    for(i=1000; i<800000; i+=20000) {
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), i);
	b = afexpr_random_cst(pr, (unsigned int)rand());
	t1p_afexpr_mul(pr, res, a, b);
	pr->index = 0;
	t1p_afexpr_clear(pr, a);
	t1p_afexpr_clear(pr, b);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
    }
    t1p_afexpr_free(pr,res);
    t1p_afexpr_free(pr, a);
    t1p_afexpr_free(pr, b);
}

void afexpr_div(ap_manager_t* man)
{
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    t1p_afexpr_t *a,*b;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    size_t i = 0;
    size_t j = 0;
    for(i=1; i<5 ; i+=1) {
	j = rand()%i;
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), j);
	b = afexpr_random_nb(pr, (unsigned int)rand(), i-j);
	t1p_afexpr_div(pr, res, a, b);
	printf("============== div =================\n");
	printf("a:\n");
	t1p_afexpr_fprint(pr, stdout, a);
	printf("\nfin a/\n b:\n");
	t1p_afexpr_fprint(pr, stdout, b);
	printf("\nfin b/\n res:\n");
	t1p_afexpr_fprint(pr, stdout, res);
	printf("\nfin res/\n");
	t1p_afexpr_clear(pr, a);
	t1p_afexpr_clear(pr, b);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	//printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
	pr->index = 0;
    }
    t1p_afexpr_free(pr, a);
    t1p_afexpr_free(pr, b);
    t1p_afexpr_free(pr, res);
}

void afexpr_mul1(ap_manager_t* man) 
{
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    t1p_afexpr_t *exp1,*exp2,*res,*res1,*res2;
    t1p_afexpr_t *A;
    t1p_afexpr_t *un;
    exp1 = t1p_afexpr_alloc(pr);
    //exp2 = t1p_afexpr_alloc(pr);
    A = t1p_afexpr_alloc(pr);
    res = t1p_afexpr_alloc(pr);
    res2 = t1p_afexpr_alloc(pr);
    res1 = t1p_afexpr_alloc(pr);
    un = t1p_afexpr_alloc(pr);
    t1p_afexpr_init_aff(pr,exp1);
    //t1p_afexpr_init_aff(pr,exp2);
    t1p_afexpr_init_aff(pr,A);
    t1p_afexpr_init_aff(pr,res);
    t1p_afexpr_init_aff(pr,res2);
    t1p_afexpr_init_aff(pr,res1);
    t1p_afexpr_init_aff(pr,un);
    num_t a,b,c,d,e,A0,A1,un_num;
    itv_t a_itv;
    itv_t b_itv;
    itv_t c_itv;
    itv_t d_itv;
    itv_t e_itv;
    itv_t A0_itv;
    itv_t A1_itv;
    itv_t un_itv;
    itv_t box;
    num_set_double(a, (double)0.9296875);
    //num_set_double(a, (double)1.1181092262268066e-1);
    num_set_double(b, (double)-0.0078125);
    //num_set_double(b, (double)-5.8460235595703125e-4);
    //num_set_double(c, (double)3);
    //num_set_double(c, (double)7.1525573730468750e-7);
    //num_set_double(d, (double)-2);
    //num_set_double(e, (double)1);

    num_set_int(A0,18);
    num_set_int(A1,2);
    num_set_int(un_num,1);
    itv_set_num(a_itv,a);
    itv_set_num(b_itv,b);
    itv_set_num(c_itv,c);
    itv_set_num(d_itv,d);
    itv_set_num(e_itv,e);
    itv_set_num(A0_itv,A0);
    itv_set_num(A1_itv,A1);
    itv_set_num(un_itv,un_num);
    itv_set(exp1->val.aff->c,a_itv);
    //	itv_set(exp2->val.aff->c,a_itv);
    t1p_aff_nsym_create(pr, exp1->val.aff, b_itv, NSYM);
    //	t1p_aff_nsym_create(pr, exp2->val.aff, d_itv, NSYM);
    //	exp2->val.aff->q->pnsym = exp1->val.aff->q->pnsym;
    //	t1p_aff_nsym_create(pr, exp1->val.aff, c_itv, NSYM);
    //	t1p_aff_nsym_create(pr, exp2->val.aff, e_itv, NSYM);
    //exp2->val.aff->end->pnsym = exp1->val.aff->end->pnsym;


    itv_set(A->val.aff->c,A0_itv);
    t1p_aff_nsym_create(pr, A->val.aff, A1_itv, NSYM);
    A->val.aff->q->pnsym = exp1->val.aff->q->pnsym;

    itv_set(un->val.aff->c,un_itv);

    exp2 = exp1;
    t1p_afexpr_mul(pr, res, exp1, exp2);
    printf("\n***** exp1 ******\n");
    t1p_afexpr_fprint(pr, stdout, exp1);
    printf("\n***** exp2 ******\n");
    t1p_afexpr_fprint(pr, stdout, exp2);
    printf("\n***** exp1*exp2 ******\n");
    //t1p_afexpr_reduce(pr, res);
    t1p_afexpr_fprint(pr, stdout, res);

    //	printf("\n***** A ******\n");
    //	t1p_afexpr_fprint(pr, stdout, A);
    //	printf("\n***** A*exp1*exp2 ******\n");
    //	t1p_afexpr_mul(pr, res1, A, res);
    //	t1p_afexpr_fprint(pr, stdout, res1);
    //	printf("\n");
    //	//t1p_afexpr_reduce(pr, res1);
    //	t1p_afexpr_fprint(pr, stdout, res1);
    //	//t1p_afexpr_boxize(pr, box, res1);
    //	//printf("\n");
    //	//itv_print(box);
    //	printf("\n***** 1 ******\n");
    //	t1p_afexpr_fprint(pr, stdout, un);
    //	printf("\n***** 1 - A*exp1*exp2 ******\n");
    //	t1p_afexpr_sub(pr, res2, un, res1);
    //	t1p_afexpr_fprint(pr, stdout, res2);
    //	printf("\n");
    //	t1p_afexpr_reduce(pr, res2);
    //	t1p_afexpr_fprint(pr, stdout, res2);
    //	printf("\n");

    t1p_afexpr_clear(pr, exp1);
    //	t1p_afexpr_clear(pr, exp2);
    t1p_afexpr_clear(pr, res);
    t1p_afexpr_clear(pr, res1);
    t1p_afexpr_clear(pr, res2);
    t1p_afexpr_clear(pr, un);
    t1p_afexpr_clear(pr, A);
}

void afexpr_mul(ap_manager_t* man)
{
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    t1p_afexpr_t *a,*b;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    size_t i = 0;
    size_t j = 0;
    ap_funopt_t option = ap_manager_get_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
    option.algorithm = INT_MAX;
    ap_manager_set_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY, &option);
    for(i=1; i<5 ; i+=1) {
	j = rand()%i;
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), j);
	b = afexpr_random_nb(pr, (unsigned int)rand(), i-j);
	printf("============== mul =================\n");
	printf("a:\n");
	t1p_afexpr_fprint(pr, stdout, a);
	printf("\nfin a/\n b:\n");
	t1p_afexpr_fprint(pr, stdout, b);
	printf("\nfin b/\n res:\n");
	t1p_afexpr_mul(pr, res, a, b);
	t1p_afexpr_fprint(pr, stdout, res);
	printf("\nfin res/\n");
	fflush(stdout);
	t1p_afexpr_clear(pr, a);
	t1p_afexpr_clear(pr, b);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	//printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
	//pr->index = 0;
    }
    t1p_afexpr_free(pr, a);
    t1p_afexpr_free(pr, b);
    t1p_afexpr_free(pr, res);
}
void afexpr_linexp(ap_manager_t* man)
{
    size_t i = 0;
    t1p_internal_t* pr = man->internal;
    t1p_afexpr_t *x0,*x1,*x2,*x3,*x4;
    t1p_afexpr_t *c0,*c1,*c2,*c3,*c4;
    t1p_afexpr_t tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    for(i=500; i<100000 ; i+=2000) {
	start = clock();
	c0 = afexpr_random_cst(pr, (unsigned int)rand());
	c1 = afexpr_random_cst(pr, (unsigned int)rand());
	c2 = afexpr_random_cst(pr, (unsigned int)rand());
	c3 = afexpr_random_cst(pr, (unsigned int)rand());
	c4 = afexpr_random_cst(pr, (unsigned int)rand());
	x0 = afexpr_random_nb(pr, (unsigned int)rand(), i);
	x1 = afexpr_random_nb(pr, (unsigned int)rand(), i);
	x2 = afexpr_random_nb(pr, (unsigned int)rand(), i);
	x3 = afexpr_random_nb(pr, (unsigned int)rand(), i);
	x4 = afexpr_random_nb(pr, (unsigned int)rand(), i);
	t1p_afexpr_mul(pr,&tmp0,c0,x0);
	t1p_afexpr_clear(pr, c0);
	t1p_afexpr_clear(pr, x0);
	t1p_afexpr_mul(pr,&tmp1,c1,x1);
	t1p_afexpr_clear(pr, c1);
	t1p_afexpr_clear(pr, x1);
	t1p_afexpr_add(pr,&tmp2,&tmp0,&tmp1);
	t1p_afexpr_clear(pr, &tmp0);
	t1p_afexpr_clear(pr, &tmp1);
	t1p_afexpr_mul(pr,&tmp3,c2,x2);
	t1p_afexpr_clear(pr, c2);
	t1p_afexpr_clear(pr, x2);
	t1p_afexpr_add(pr,&tmp4,&tmp3,&tmp2);
	t1p_afexpr_clear(pr, &tmp2);
	t1p_afexpr_clear(pr, &tmp3);
	t1p_afexpr_mul(pr,&tmp5,c3,x3);
	t1p_afexpr_clear(pr, c3);
	t1p_afexpr_clear(pr, x3);
	t1p_afexpr_add(pr,&tmp6,&tmp4,&tmp5);
	t1p_afexpr_clear(pr, &tmp4);
	t1p_afexpr_clear(pr, &tmp5);
	t1p_afexpr_mul(pr,&tmp7,c4,x4);
	t1p_afexpr_clear(pr, c4);
	t1p_afexpr_clear(pr, x4);
	t1p_afexpr_add(pr,res,&tmp7,&tmp6);
	t1p_afexpr_clear(pr, &tmp6);
	t1p_afexpr_clear(pr, &tmp7);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
    }
    t1p_afexpr_free(pr, c0);
    t1p_afexpr_free(pr, c1);
    t1p_afexpr_free(pr, c2);
    t1p_afexpr_free(pr, c3);
    t1p_afexpr_free(pr, c4);
    t1p_afexpr_free(pr, x0);
    t1p_afexpr_free(pr, x1);
    t1p_afexpr_free(pr, x2);
    t1p_afexpr_free(pr, x3);
    t1p_afexpr_free(pr, x4);
}
void afexpr_sub(ap_manager_t* man)
{
    t1p_internal_t* pr = man->internal;
    size_t Nb1, Nb2; 
    unsigned int seed1, seed2;
    t1p_afexpr_t *a,*b;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    size_t i = 0;
    size_t j = 0;
    for(i=1; i<10 ; i+=2) {
	j = rand()%i;
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), j);
	b = afexpr_random_nb(pr, (unsigned int)rand(), i-j);
	t1p_afexpr_sub(pr, res, a, b);
	pr->index = 0;
	printf("============== sub =================\n");
	printf("a:\n");
	t1p_afexpr_fprint(pr, stdout, a);
	printf("\nfin a/\n b:\n");
	t1p_afexpr_fprint(pr, stdout, b);
	printf("\nfin b/\n res:\n");
	t1p_afexpr_fprint(pr, stdout, res);
	printf("\nfin res/\n");
	t1p_afexpr_clear(pr, a);
	t1p_afexpr_clear(pr, b);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	//printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
    }
    t1p_afexpr_free(pr, a);
    t1p_afexpr_free(pr, b);
    t1p_afexpr_free(pr, res);
}
void afexpr_add(ap_manager_t* man)
{
    t1p_internal_t* pr = man->internal;
    size_t Nb1, Nb2; 
    unsigned int seed1, seed2;
    t1p_afexpr_t *a,*b;
    clock_t start, end;
    double cpu_time_used;
    t1p_afexpr_t* res = t1p_afexpr_alloc(pr);
    size_t i = 0;
    size_t j = 0;
    for(i=1000; i<800000 ; i+=20000) {
	j = rand()%i;
	start = clock();
	a = afexpr_random_nb(pr, (unsigned int)rand(), j);
	b = afexpr_random_nb(pr, (unsigned int)rand(), i-j);
	t1p_afexpr_add(pr, res, a, b);
	pr->index = 0;
	t1p_afexpr_clear(pr, a);
	t1p_afexpr_clear(pr, b);
	end = clock();
	cpu_time_used = ((double) (end - start)) / (1.0*CLOCKS_PER_SEC);
	printf("%d	%.3g\n", i, cpu_time_used);
	t1p_afexpr_clear(pr, res);
    }
    t1p_afexpr_free(pr, a);
    t1p_afexpr_free(pr, b);
    t1p_afexpr_free(pr, res);
}

void afexpr_join(ap_manager_t* man)
{
    t1p_internal_t* pr = man->internal;
    t1p_afexpr_t* res = NULL;
    t1p_afexpr_t *a, *b;
    tbool_t tbool = tbool_true;
    b = afexpr_random_nb(pr, (unsigned int)rand(), 5);
    /* T U b */
    res = t1p_afexpr_join(pr, pr->top, b, &tbool);
    if (res != pr->top) printf("false\n");
    //t1p_afexpr_check_free(pr, res);
    /* bot U b */
    res = t1p_afexpr_join(pr, pr->bot, b, &tbool);
    if (res != b) printf("false\n");
    //t1p_afexpr_check_free(pr, res);
    /* a U b */
    pr->index = 0;
    a = afexpr_random_nb(pr, (unsigned int)rand(), 3);
    pr->index = 0;
    b = afexpr_random_nb(pr, (unsigned int)rand(), 3);
    res = t1p_afexpr_join(pr, a, b, &tbool);
    t1p_afexpr_fprint(pr, stdout, a);
    printf("\n");
    t1p_afexpr_fprint(pr, stdout, b);
    printf("\n");
    if (res) t1p_afexpr_fprint(pr, stdout, res);
    printf("\n");
    //t1p_afexpr_check_free(pr, a);
    //t1p_afexpr_check_free(pr, b);
}

int main(void)
{
    //	mtrace();
    long int seed48 = time(NULL);
    unsigned int seed = (unsigned int) (seed48 % UINT_MAX);
    srand(seed);
    ap_manager_t* t1p = t1p_manager_alloc();

    afexpr_mul1(t1p);

    /* free managers */
    ap_manager_free(t1p);
    return 0;
}

