#include <time.h>
//#include <mcheck.h>
#include "ap_global1.h"
#include "box.h"
#include "oct.h"
#include "t1p.h"
#include "pk.h"
#include "ap_ppl.h"

#define	T1P			1
#define	LOOP			1
#define WIDENING_THRESHOLD	100
#define INITIAL_UNFOLD		0
#define THRESHOLD		20

static int widening;

void filter1(ap_manager_t* man, int *N, double *inf, double *sup, FILE* stream, double *cpu_time_used);

void filter1(ap_manager_t* man, int *N, double *inf, double *sup, FILE* stream, double *cpu_time_used) 
{

	int i = 0;
	int wide = 0;
	clock_t start_t1p, end_t1p;
	/* define env */
	ap_var_t name_of_dim[6] = {"E","E0","E1","S","S0","S1"};
	ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],6);

	ap_linexpr1_t zero = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t top = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t bot = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t E = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t E0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t S = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t S0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

	ap_linexpr1_set_cst_scalar_double(&zero, 0.0);
	ap_linexpr1_set_coeff_scalar_double(&E0, "E0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&E, "E", 1.);
	ap_linexpr1_set_coeff_scalar_double(&S0, "S0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&S, "S", 1.);
	/* itop = [-oo,+oo] */
	ap_interval_t* itop = ap_interval_alloc();
	ap_interval_set_top(itop);
	ap_linexpr1_set_cst_interval(&top, itop);
	/* ibot */
	ap_interval_t* ibot = ap_interval_alloc();
	ap_interval_set_bottom(ibot);
	ap_linexpr1_set_cst_interval(&bot, ibot);

	/* Transfer function */
	ap_linexpr1_t h = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)5);
/*
	ap_linexpr1_set_list(&h,
			AP_COEFF_S_DOUBLE, 0.7,"E",
			AP_COEFF_S_DOUBLE, -1.3,"E0",
			AP_COEFF_S_DOUBLE, 1.1,"E1",
			AP_COEFF_S_DOUBLE, 1.4,"S0",
			AP_COEFF_S_DOUBLE, -0.7,"S1",
			AP_END);
*/
	ap_linexpr1_set_list(&h,
			AP_COEFF_I_FRAC, 7, 10, 7,10,"E",
			AP_COEFF_I_FRAC, -13, 10, -13, 10,"E0",
			AP_COEFF_I_FRAC, 11, 10, 11, 10,"E1",
			AP_COEFF_I_FRAC, 14, 10, 14, 10,"S0",
			AP_COEFF_I_FRAC, -7, 10, -7, 10,"S1",
			AP_END);

	ap_lincons1_array_t array_consE = ap_lincons1_array_make(env, 2);
	ap_lincons1_array_t array_consE0 = ap_lincons1_array_make(env, 2);
	/* 0 <= E0 */
	//ap_linexpr1_t E0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_set_coeff_scalar_double(&E0, "E0", 1.);
	ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ,&E0,NULL);
	ap_lincons1_array_set(&array_consE0,0,&cons1);
	/* E0 <= 1 */
	ap_linexpr1_t onemE0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE0,NULL);
	ap_lincons1_set_list(&cons2,
			AP_COEFF_S_DOUBLE, -1., "E0",
			AP_CST_S_DOUBLE, 1.0,
			AP_END);
	ap_lincons1_array_set(&array_consE0,1,&cons2);

	/* 0 <= E */
	//ap_linexpr1_t E = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_set_coeff_scalar_double(&E, "E", 1.);
	ap_lincons1_t cons3 = ap_lincons1_make(AP_CONS_SUPEQ,&E,NULL);
	ap_lincons1_array_set(&array_consE,0,&cons3);
	/* E <= 1 */
	ap_linexpr1_t onemE = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons4 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE,NULL);
	ap_lincons1_set_list(&cons4,
			AP_COEFF_S_DOUBLE, -1., "E",
			AP_CST_S_DOUBLE, 1.0,
			AP_END);
	ap_lincons1_array_set(&array_consE,1,&cons4);

	/***********/
	/* ANALYSE */
	/***********/
	ap_abstract1_t _1 = ap_abstract1_top (man, env);
	ap_abstract1_t _2 = ap_abstract1_assign_linexpr(man, false, &_1, "S", &zero, NULL);
	ap_abstract1_t _3 = ap_abstract1_assign_linexpr(man, false, &_2, "S0", &zero, NULL);
	ap_abstract1_t _4 = ap_abstract1_assign_linexpr(man, false, &_3, "E0", &top, NULL);
	ap_abstract1_t _5 = ap_abstract1_meet_lincons_array(man, false, &_4, &array_consE0);
	ap_abstract1_t _6 = ap_abstract1_assign_linexpr(man, false, &_5, "E", &top, NULL);
	ap_abstract1_t _7 = ap_abstract1_meet_lincons_array(man, false, &_6, &array_consE);
	ap_abstract1_t _8 = ap_abstract1_assign_linexpr(man, false, &_7, "E1", &bot, NULL); 
	ap_abstract1_t _9 = ap_abstract1_assign_linexpr(man, false, &_8, "S1", &bot, NULL); 
	
	ap_box1_t box;
	ap_abstract1_t _15,_16,_17,_18,_19,_20,_21,pivot,_k1,_k2, pivot2, fix;
	pivot = ap_abstract1_copy(man, &_9);
	i = 0;
	while (i<INITIAL_UNFOLD) {
		_15 = ap_abstract1_assign_linexpr(man, false, &pivot, "E1", &E0, NULL);	
		_16 = ap_abstract1_assign_linexpr(man, false, &_15, "E0", &E, NULL);		
		_17 = ap_abstract1_assign_linexpr(man, false, &_16, "E", &top, NULL);		
		_18 = ap_abstract1_meet_lincons_array(man, false, &_17, &array_consE);
		_19 = ap_abstract1_assign_linexpr(man, false, &_18, "S1", &S0, NULL);		
		_20 = ap_abstract1_assign_linexpr(man, false, &_19, "S0", &S, NULL);	
		_21 = ap_abstract1_assign_linexpr(man, false, &_20, "S", &h, NULL);	
		ap_abstract1_clear(man, &pivot);
		pivot = ap_abstract1_copy(man, &_21);
		ap_abstract1_clear(man, &_15);
		ap_abstract1_clear(man, &_16);
		ap_abstract1_clear(man, &_17);
		ap_abstract1_clear(man, &_18);
		ap_abstract1_clear(man, &_19);
		ap_abstract1_clear(man, &_20);
		ap_abstract1_clear(man, &_21);
		i++;
	}
	if (!LOOP) {
		box = ap_abstract1_to_box(man, &pivot);
		ap_double_set_scalar(inf, box.p[3]->inf, GMP_RNDU);
		ap_double_set_scalar(sup, box.p[3]->sup, GMP_RNDU);
		ap_box1_clear(&box);
		*N = INITIAL_UNFOLD;
	}
	/**********************/
	/* It THRESHOLD times */
	/**********************/
	int k = 0;
	int j = 0;
	while (LOOP) {
		_k1 = ap_abstract1_copy(man, &pivot);		
		i = 0;
		while (i<THRESHOLD)
		{
			ap_abstract1_fdump(stream, man, &pivot);
			_15 = ap_abstract1_assign_linexpr(man, false, &pivot, "E1", &E0, NULL);
			_16 = ap_abstract1_assign_linexpr(man, false, &_15, "E0", &E, NULL);	
			_17 = ap_abstract1_assign_linexpr(man, false, &_16, "E", &top, NULL);
			_18 = ap_abstract1_meet_lincons_array(man, false, &_17, &array_consE);
			_19 = ap_abstract1_assign_linexpr(man, false, &_18, "S1", &S0, NULL);
			_20 = ap_abstract1_assign_linexpr(man, false, &_19, "S0", &S, NULL);
			_21 = ap_abstract1_assign_linexpr(man, false, &_20, "S", &h, NULL);
			ap_abstract1_clear(man, &pivot);
			pivot = ap_abstract1_copy(man, &_21);
			ap_abstract1_clear(man, &_15);
			ap_abstract1_clear(man, &_16);
			ap_abstract1_clear(man, &_17);
			ap_abstract1_clear(man, &_18);
			ap_abstract1_clear(man, &_19);
			ap_abstract1_clear(man, &_20);
			ap_abstract1_clear(man, &_21);
			i++;
		}
		//if (j%WIDENING_THRESHOLD==0 && WIDENING_THRESHOLD>0) wide = 1;
		if (wide) {_k2 = ap_abstract1_widening(man, &_k1, &pivot);widening++;}
		else {_k2 = ap_abstract1_join(man, false, &_k1, &pivot);j++;}
		if (j*THRESHOLD>=WIDENING_THRESHOLD && WIDENING_THRESHOLD>0) wide = 1;
		/* F^c(_k2) */
		i = 0;
		pivot2 = ap_abstract1_copy(man, &_k2);
		while (i<THRESHOLD)
		{
			_15 = ap_abstract1_assign_linexpr(man, false, &pivot2, "E1", &E0, NULL);
			_16 = ap_abstract1_assign_linexpr(man, false, &_15, "E0", &E, NULL);	
			_17 = ap_abstract1_assign_linexpr(man, false, &_16, "E", &top, NULL);
			_18 = ap_abstract1_meet_lincons_array(man, false, &_17, &array_consE);
			_19 = ap_abstract1_assign_linexpr(man, false, &_18, "S1", &S0, NULL);
			_20 = ap_abstract1_assign_linexpr(man, false, &_19, "S0", &S, NULL);
			_21 = ap_abstract1_assign_linexpr(man, false, &_20, "S", &h, NULL);
			ap_abstract1_clear(man, &pivot2);
			pivot2 = ap_abstract1_copy(man, &_21);
			ap_abstract1_clear(man, &_15);
			ap_abstract1_clear(man, &_16);
			ap_abstract1_clear(man, &_17);
			ap_abstract1_clear(man, &_18);
			ap_abstract1_clear(man, &_19);
			ap_abstract1_clear(man, &_20);
			ap_abstract1_clear(man, &_21);
			i++;
		}
		/* F^c(_k2) <= ?? _k2 */
		if (ap_abstract1_is_leq(man, &pivot2, &_k2) == tbool_true) {
			/* Special T1P: Computing fixpoint of F */
			if (T1P) {
				ap_abstract1_t array[THRESHOLD];
				array[0] = ap_abstract1_copy(man, &_k2);
				i = 0;
				while (i<(THRESHOLD-1)) 
				{
					_15 = ap_abstract1_assign_linexpr(man, false, &array[i], "E1", &E0, NULL);
					_16 = ap_abstract1_assign_linexpr(man, false, &_15, "E0", &E, NULL);	
					_17 = ap_abstract1_assign_linexpr(man, false, &_16, "E", &top, NULL);
					_18 = ap_abstract1_meet_lincons_array(man, false, &_17, &array_consE);
					_19 = ap_abstract1_assign_linexpr(man, false, &_18, "S1", &S0, NULL);
					_20 = ap_abstract1_assign_linexpr(man, false, &_19, "S0", &S, NULL);
					_21 = ap_abstract1_assign_linexpr(man, false, &_20, "S", &h, NULL);
					array[i+1] = ap_abstract1_copy(man, &_21);
					ap_abstract1_clear(man, &_15);
					ap_abstract1_clear(man, &_16);
					ap_abstract1_clear(man, &_17);
					ap_abstract1_clear(man, &_18);
					ap_abstract1_clear(man, &_19);
					ap_abstract1_clear(man, &_20);
					ap_abstract1_clear(man, &_21);
					i++;
				}
				fix = ap_abstract1_join_array(man, array, THRESHOLD);
				box = ap_abstract1_to_box(man, &fix);
				ap_double_set_scalar(inf, box.p[3]->inf, GMP_RNDU);
				ap_double_set_scalar(sup, box.p[3]->sup, GMP_RNDU);
				ap_box1_clear(&box);
				printf("T1P: fixpoint of F: [%.2f, %.2f]\n",*inf, *sup);
				for (k=0;k<THRESHOLD;k++) ap_abstract1_clear(man, &array[k]);
				ap_abstract1_clear(man, &fix);
			} 
			/* END SEPCIAL T1P */
			box = ap_abstract1_to_box(man, &_k2);
			ap_double_set_scalar(inf, box.p[3]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[3]->sup, GMP_RNDU);
			ap_box1_clear(&box);
			*N = j;
			ap_abstract1_clear(man, &pivot2);
			ap_abstract1_clear(man, &_k1);
			ap_abstract1_clear(man, &_k2);
			break;
		}
		ap_abstract1_clear(man, &pivot2);

		ap_abstract1_clear(man, &pivot);
		pivot = ap_abstract1_copy(man, &_k2);
		ap_abstract1_clear(man, &_k1);
		ap_abstract1_clear(man, &_k2);
	}
	/* freeing memory used */
	ap_lincons1_array_clear(&array_consE);
	ap_lincons1_array_clear(&array_consE0);

	ap_linexpr1_clear(&zero);
	ap_linexpr1_clear(&top);
	ap_linexpr1_clear(&bot);
	ap_linexpr1_clear(&h);
	ap_linexpr1_clear(&S);
	ap_linexpr1_clear(&S0);

	ap_abstract1_clear(man, &pivot);
	ap_abstract1_clear(man, &_1);
	ap_abstract1_clear(man, &_2);
	ap_abstract1_clear(man, &_3);
	ap_abstract1_clear(man, &_4);
	ap_abstract1_clear(man, &_5);
	ap_abstract1_clear(man, &_6);
	ap_abstract1_clear(man, &_7);
	ap_abstract1_clear(man, &_8);
	ap_abstract1_clear(man, &_9);
	ap_environment_free (env);
}

int main(void)
{
	ap_manager_t* man;
	clock_t start, end;
	double time;
	double cpu_time_used;
	double inf, sup;
	int N;
	int l;
	FILE* stream = fopen("/tmp/logg", "w+");
	widening = 0;
	man = t1p_manager_alloc();
	start = clock();
	for (l=0;l<1;l++) {widening=0;filter1(man, &N, &inf, &sup, stream, &cpu_time_used);}
	end = clock();
	time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("it:%dx%d\nwidenings:%d\ntime:%.2g\nF^%d fixpoint :[%.2f, %.2f]\n", N, THRESHOLD, widening, time, THRESHOLD, inf, sup);
	ap_manager_free(man);
	fclose(stream);
	return 0;
}

