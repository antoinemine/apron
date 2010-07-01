#include <time.h>
//#include <mcheck.h>
#include "ap_global1.h"
#include "box.h"
#include "oct.h"
#include "t1p.h"
#include "pk.h"
#include "ap_ppl.h"

#define T1P                     1
#define MAX_IT			1000
#define LOOP                    1
#define WIDENING_THRESHOLD      100
#define INITIAL_UNFOLD          0
#define THRESHOLD               16

static int widening;

void filter1(ap_manager_t* man, int *N, double *inf, double *sup, FILE* stream, double *cpu_time_used);

void filter1(ap_manager_t* man, int *N, double *inf, double *sup, FILE* stream, double *cpu_time_used) 
{

	int i = 0;
	int wide = 0;
	clock_t start_t1p, end_t1p;
	/* define env */
	ap_var_t name_of_dim[24] = {"E","E0","E1","S","S0","S1","Ep","Ep0","Ep1","Sp","Sp0","Sp1","Eq","Eq0","Eq1","Sq","Sq0","Sq1","En","En0","En1","Sn","Sn0","Sn1"};
	ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],24);

	ap_linexpr1_t zero = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t top = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t bot = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_linexpr1_t E = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t E0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t S = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t S0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

	ap_linexpr1_t Ep = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Ep0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sp = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sp0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

	ap_linexpr1_t Eq = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Eq0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sq = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sq0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

	ap_linexpr1_t En = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t En0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sn = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_linexpr1_t Sn0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

	ap_linexpr1_set_cst_scalar_double(&zero, 0.0);
	ap_linexpr1_set_coeff_scalar_double(&E0, "E0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&E, "E", 1.);
	ap_linexpr1_set_coeff_scalar_double(&S0, "S0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&S, "S", 1.);

	ap_linexpr1_set_coeff_scalar_double(&Ep0, "Ep0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Ep, "Ep", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sp0, "Sp0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sp, "Sp", 1.);

	ap_linexpr1_set_coeff_scalar_double(&Eq0, "Eq0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Eq, "Eq", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sq0, "Sq0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sq, "Sq", 1.);

	ap_linexpr1_set_coeff_scalar_double(&En0, "En0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&En, "En", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sn0, "Sn0", 1.);
	ap_linexpr1_set_coeff_scalar_double(&Sn, "Sn", 1.);

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
	ap_linexpr1_set_list(&h,
			AP_COEFF_S_DOUBLE, 0.81578,"E",
			AP_COEFF_S_DOUBLE, -0.815294,"E0",
			AP_COEFF_S_DOUBLE, 0.389763,"E1",
			AP_COEFF_S_DOUBLE, 1.077138,"S0",
			AP_COEFF_S_DOUBLE, -0.467388,"S1",
			AP_END);
	/*
	ap_linexpr1_set_list(&h,
			AP_COEFF_I_FRAC, 40789, 50000, 40789, 50000, "E",
			AP_COEFF_I_FRAC, -407647, 50000, -407647, 50000, "E0",
			AP_COEFF_I_FRAC, 389763, 1000000, 389763, 1000000, "E1",
			AP_COEFF_I_FRAC, 538569, 500000, 538569, 500000, "S0",
			AP_COEFF_I_FRAC, -116847,250000, -116847, 250000, "S1",
			AP_END);
	*/
	ap_linexpr1_t hp = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)5);
	ap_linexpr1_set_list(&hp,
			AP_COEFF_S_DOUBLE, 0.964223,"Ep",
			AP_COEFF_S_DOUBLE, - 0.036313,"Ep0",
			AP_COEFF_S_DOUBLE, 0.890149,"Ep1",
			AP_COEFF_S_DOUBLE, -0.171734,"Sp0",
			AP_COEFF_S_DOUBLE, -0.646325,"Sp1",
			AP_END);
	/*
	ap_linexpr1_set_list(&hp,
			AP_COEFF_I_FRAC, 964223,1000000,964223,1000000, "Ep",
			AP_COEFF_I_FRAC, - 36313,1000000, -36313,1000000, "Ep0",
			AP_COEFF_I_FRAC, 890149,1000000, 890149,1000000, "Ep1",
			AP_COEFF_I_FRAC, -85867,500000, -85867,500000, "Sp0",
			AP_COEFF_I_FRAC, -25853,40000, -25853,40000,"Sp1",
			AP_END);
	*/
	ap_linexpr1_t hq = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)5);
	ap_linexpr1_set_list(&hq,
			AP_COEFF_S_DOUBLE, 0.733165,"Eq",
			AP_COEFF_S_DOUBLE, - 0.409877,"Eq0",
			AP_COEFF_S_DOUBLE, 0.679167,"Eq1",
			AP_COEFF_S_DOUBLE, 0.666969,"Sq0",
			AP_COEFF_S_DOUBLE, -0.669425,"Sq1",
			AP_END);
	/*
	ap_linexpr1_set_list(&hq,
			AP_COEFF_I_FRAC, 146633,200000, 146633,200000,"Eq",
			AP_COEFF_I_FRAC, -409877,1000000, -409877,1000000, "Eq0",
			AP_COEFF_I_FRAC, 679167,1000000, 679167,1000000, "Eq1",
			AP_COEFF_I_FRAC, 666969,1000000, 666969,1000000, "Sq0",
			AP_COEFF_I_FRAC, -26777,40000,-26777,40000,"Sq1",
			AP_END);
	*/

	ap_linexpr1_t hn = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)5);
	ap_linexpr1_set_list(&hn,
			AP_COEFF_S_DOUBLE, 0.846686,"En",
			AP_COEFF_S_DOUBLE, 0.099170,"En0",
			AP_COEFF_S_DOUBLE, 0.307511,"En1",
			AP_COEFF_S_DOUBLE, 0.052887,"Sn0",
			AP_COEFF_S_DOUBLE, -0.306254,"Sn1",
			AP_END);
	/*
	ap_linexpr1_set_list(&hn,
			AP_COEFF_I_FRAC, 423343,500000,423343,500000,"En",
			AP_COEFF_I_FRAC, 9917,100000, 9917,100000, "En0",
			AP_COEFF_I_FRAC, 307511,1000000,307511,1000000,"En1",
			AP_COEFF_I_FRAC, 52887,1000000, 52887,1000000, "Sn0",
			AP_COEFF_I_FRAC, -153127,500000,-153127,500000,"Sn1",
			AP_END);
	*/

	ap_lincons1_array_t array_consE = ap_lincons1_array_make(env, 2);
	ap_lincons1_array_t array_consE0 = ap_lincons1_array_make(env, 2);
	/* -10 <= E0 */
	ap_linexpr1_t onemE0p = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE0p,NULL);
	ap_lincons1_set_list(&cons1,
			AP_COEFF_S_DOUBLE, 1., "E0",
			AP_CST_S_DOUBLE, -10.0,
			AP_END);
	ap_lincons1_array_set(&array_consE0,0,&cons1);
	/* -10 <= -E0 */
	ap_linexpr1_t onemE0 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE0,NULL);
	ap_lincons1_set_list(&cons2,
			AP_COEFF_S_DOUBLE, -1., "E0",
			AP_CST_S_DOUBLE, -10.0,
			AP_END);
	ap_lincons1_array_set(&array_consE0,1,&cons2);

	/* -10 <= E */
	//ap_linexpr1_set_coeff_scalar_double(&E, "E", 1.);
	ap_linexpr1_t onemEp = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons3 = ap_lincons1_make(AP_CONS_SUPEQ,&onemEp,NULL);
	ap_lincons1_set_list(&cons3,
			AP_COEFF_S_DOUBLE, 1., "E",
			AP_CST_S_DOUBLE, -10.0,
			AP_END);
	ap_lincons1_array_set(&array_consE,0,&cons3);
	/* -10 <= -E */
	ap_linexpr1_t onemE = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons4 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE,NULL);
	ap_lincons1_set_list(&cons4,
			AP_COEFF_S_DOUBLE, -1., "E",
			AP_CST_S_DOUBLE, -10.0,
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

	ap_abstract1_t _8 = ap_abstract1_assign_linexpr(man, false, &_7, "Ep0", &zero, NULL); 
	ap_abstract1_t _9 = ap_abstract1_assign_linexpr(man, false, &_8, "Ep", &zero, NULL); 
	ap_abstract1_t _10 = ap_abstract1_assign_linexpr(man, false, &_9, "Sp0", &zero, NULL); 
	ap_abstract1_t _11 = ap_abstract1_assign_linexpr(man, false, &_10, "Sp", &zero, NULL);

	ap_abstract1_t _12 = ap_abstract1_assign_linexpr(man, false, &_11, "Eq0", &zero, NULL); 
	ap_abstract1_t _13 = ap_abstract1_assign_linexpr(man, false, &_12, "Eq", &zero, NULL); 
	ap_abstract1_t _14 = ap_abstract1_assign_linexpr(man, false, &_13, "Sq0", &zero, NULL); 
	ap_abstract1_t _15 = ap_abstract1_assign_linexpr(man, false, &_14, "Sq", &zero, NULL); 

	ap_abstract1_t _16 = ap_abstract1_assign_linexpr(man, false, &_15, "En0", &zero, NULL); 
	ap_abstract1_t _17 = ap_abstract1_assign_linexpr(man, false, &_16, "En", &zero, NULL); 
	ap_abstract1_t _18 = ap_abstract1_assign_linexpr(man, false, &_17, "Sn0", &zero, NULL); 
	ap_abstract1_t _19 = ap_abstract1_assign_linexpr(man, false, &_18, "Sn", &zero, NULL); 

	ap_abstract1_t _20 = ap_abstract1_assign_linexpr(man, false, &_19, "E1", &bot, NULL); 
	ap_abstract1_t _21 = ap_abstract1_assign_linexpr(man, false, &_20, "S1", &bot, NULL); 
	ap_abstract1_t _22 = ap_abstract1_assign_linexpr(man, false, &_21, "Ep1", &bot, NULL); 
	ap_abstract1_t _23 = ap_abstract1_assign_linexpr(man, false, &_22, "Sp1", &bot, NULL); 
	ap_abstract1_t _24 = ap_abstract1_assign_linexpr(man, false, &_23, "Eq1", &bot, NULL); 
	ap_abstract1_t _25 = ap_abstract1_assign_linexpr(man, false, &_24, "Sq1", &bot, NULL); 
	ap_abstract1_t _26 = ap_abstract1_assign_linexpr(man, false, &_25, "En1", &bot, NULL); 
	ap_abstract1_t _27 = ap_abstract1_assign_linexpr(man, false, &_26, "Sn1", &bot, NULL); 

	ap_box1_t box;
	ap_abstract1_t _35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,pivot,_k1,_k2, pivot2, fix;
	i = 0;
	pivot = ap_abstract1_copy(man, &_27);
	ap_abstract1_t array[THRESHOLD+INITIAL_UNFOLD];
	if (T1P) array[i] = ap_abstract1_copy(man, &pivot);
	while (i<INITIAL_UNFOLD) {
		if (i>0 && T1P) array[i] = ap_abstract1_copy(man, &pivot);
		_35 = ap_abstract1_assign_linexpr(man, false, &pivot, "E1", &E0, NULL);	
		_36 = ap_abstract1_assign_linexpr(man, false, &_35, "E0", &E, NULL);		
		_37 = ap_abstract1_assign_linexpr(man, false, &_36, "E", &top, NULL);		
		_38 = ap_abstract1_meet_lincons_array(man, false, &_37, &array_consE);
		_39 = ap_abstract1_assign_linexpr(man, false, &_38, "S1", &S0, NULL);		
		_40 = ap_abstract1_assign_linexpr(man, false, &_39, "S0", &S, NULL);	
		_41 = ap_abstract1_assign_linexpr(man, false, &_40, "S", &h, NULL);	

		_42 = ap_abstract1_assign_linexpr(man, false, &_41, "Ep1", &Ep0, NULL);	
		_43 = ap_abstract1_assign_linexpr(man, false, &_42, "Ep0", &Ep, NULL);		
		_44 = ap_abstract1_assign_linexpr(man, false, &_43, "Ep", &S, NULL);		
		_45 = ap_abstract1_assign_linexpr(man, false, &_44, "Sp1", &Sp0, NULL);		
		_46 = ap_abstract1_assign_linexpr(man, false, &_45, "Sp0", &Sp, NULL);	
		_47 = ap_abstract1_assign_linexpr(man, false, &_46, "Sp", &hp, NULL);	

		_48 = ap_abstract1_assign_linexpr(man, false, &_47, "Eq1", &Eq0, NULL);	
		_49 = ap_abstract1_assign_linexpr(man, false, &_48, "Eq0", &Eq, NULL);		
		_50 = ap_abstract1_assign_linexpr(man, false, &_49, "Eq", &Sp, NULL);		
		_51 = ap_abstract1_assign_linexpr(man, false, &_50, "Sq1", &Sq0, NULL);		
		_52 = ap_abstract1_assign_linexpr(man, false, &_51, "Sq0", &Sq, NULL);	
		_53 = ap_abstract1_assign_linexpr(man, false, &_52, "Sq", &hq, NULL);	

		_54 = ap_abstract1_assign_linexpr(man, false, &_53, "En1", &En0, NULL);	
		_55 = ap_abstract1_assign_linexpr(man, false, &_54, "En0", &En, NULL);		
		_56 = ap_abstract1_assign_linexpr(man, false, &_55, "En", &Sq, NULL);		
		_57 = ap_abstract1_assign_linexpr(man, false, &_56, "Sn1", &Sn0, NULL);		
		_58 = ap_abstract1_assign_linexpr(man, false, &_57, "Sn0", &Sn, NULL);	
		_59 = ap_abstract1_assign_linexpr(man, false, &_58, "Sn", &hn, NULL);	

		ap_abstract1_clear(man, &pivot);
		pivot = ap_abstract1_copy(man, &_59);
		ap_abstract1_clear(man, &_35);
		ap_abstract1_clear(man, &_36);
		ap_abstract1_clear(man, &_37);
		ap_abstract1_clear(man, &_38);
		ap_abstract1_clear(man, &_39);
		ap_abstract1_clear(man, &_40);
		ap_abstract1_clear(man, &_41);
		ap_abstract1_clear(man, &_42);
		ap_abstract1_clear(man, &_43);
		ap_abstract1_clear(man, &_44);
		ap_abstract1_clear(man, &_45);
		ap_abstract1_clear(man, &_46);
		ap_abstract1_clear(man, &_47);
		ap_abstract1_clear(man, &_48);
		ap_abstract1_clear(man, &_49);
		ap_abstract1_clear(man, &_50);
		ap_abstract1_clear(man, &_51);
		ap_abstract1_clear(man, &_52);
		ap_abstract1_clear(man, &_53);
		ap_abstract1_clear(man, &_54);
		ap_abstract1_clear(man, &_55);
		ap_abstract1_clear(man, &_56);
		ap_abstract1_clear(man, &_57);
		ap_abstract1_clear(man, &_58);
		ap_abstract1_clear(man, &_59);
		i++;
	}
	if (!LOOP) {
                box = ap_abstract1_to_box(man, &pivot);
                ap_double_set_scalar(inf, box.p[15]->inf, GMP_RNDU);
                ap_double_set_scalar(sup, box.p[15]->sup, GMP_RNDU);
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
			_35 = ap_abstract1_assign_linexpr(man, false, &pivot, "E1", &E0, NULL);
			_36 = ap_abstract1_assign_linexpr(man, false, &_35, "E0", &E, NULL);	
			_37 = ap_abstract1_assign_linexpr(man, false, &_36, "E", &top, NULL);
			_38 = ap_abstract1_meet_lincons_array(man, false, &_37, &array_consE);
			_39 = ap_abstract1_assign_linexpr(man, false, &_38, "S1", &S0, NULL);
			_40 = ap_abstract1_assign_linexpr(man, false, &_39, "S0", &S, NULL);
			_41 = ap_abstract1_assign_linexpr(man, false, &_40, "S", &h, NULL);

			_42 = ap_abstract1_assign_linexpr(man, false, &_41, "Ep1", &Ep0, NULL);	
			_43 = ap_abstract1_assign_linexpr(man, false, &_42, "Ep0", &Ep, NULL);		
			_44 = ap_abstract1_assign_linexpr(man, false, &_43, "Ep", &S, NULL);		
			_45 = ap_abstract1_assign_linexpr(man, false, &_44, "Sp1", &Sp0, NULL);		
			_46 = ap_abstract1_assign_linexpr(man, false, &_45, "Sp0", &Sp, NULL);	
			_47 = ap_abstract1_assign_linexpr(man, false, &_46, "Sp", &hp, NULL);	

			_48 = ap_abstract1_assign_linexpr(man, false, &_47, "Eq1", &Eq0, NULL);	
			_49 = ap_abstract1_assign_linexpr(man, false, &_48, "Eq0", &Eq, NULL);		
			_50 = ap_abstract1_assign_linexpr(man, false, &_49, "Eq", &Sp, NULL);		
			_51 = ap_abstract1_assign_linexpr(man, false, &_50, "Sq1", &Sq0, NULL);		
			_52 = ap_abstract1_assign_linexpr(man, false, &_51, "Sq0", &Sq, NULL);	
			_53 = ap_abstract1_assign_linexpr(man, false, &_52, "Sq", &hq, NULL);	

			_54 = ap_abstract1_assign_linexpr(man, false, &_53, "En1", &En0, NULL);	
			_55 = ap_abstract1_assign_linexpr(man, false, &_54, "En0", &En, NULL);		
			_56 = ap_abstract1_assign_linexpr(man, false, &_55, "En", &Sq, NULL);		
			_57 = ap_abstract1_assign_linexpr(man, false, &_56, "Sn1", &Sn0, NULL);		
			_58 = ap_abstract1_assign_linexpr(man, false, &_57, "Sn0", &Sn, NULL);	
			_59 = ap_abstract1_assign_linexpr(man, false, &_58, "Sn", &hn, NULL);	

			ap_abstract1_clear(man, &pivot);
			pivot = ap_abstract1_copy(man, &_59);

			ap_abstract1_clear(man, &_35);
			ap_abstract1_clear(man, &_36);
			ap_abstract1_clear(man, &_37);
			ap_abstract1_clear(man, &_38);
			ap_abstract1_clear(man, &_39);
			ap_abstract1_clear(man, &_40);
			ap_abstract1_clear(man, &_41);
			ap_abstract1_clear(man, &_42);
			ap_abstract1_clear(man, &_43);
			ap_abstract1_clear(man, &_44);
			ap_abstract1_clear(man, &_45);
			ap_abstract1_clear(man, &_46);
			ap_abstract1_clear(man, &_47);
			ap_abstract1_clear(man, &_48);
			ap_abstract1_clear(man, &_49);
			ap_abstract1_clear(man, &_50);
			ap_abstract1_clear(man, &_51);
			ap_abstract1_clear(man, &_52);
			ap_abstract1_clear(man, &_53);
			ap_abstract1_clear(man, &_54);
			ap_abstract1_clear(man, &_55);
			ap_abstract1_clear(man, &_56);
			ap_abstract1_clear(man, &_57);
			ap_abstract1_clear(man, &_58);
			ap_abstract1_clear(man, &_59);
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
			_35 = ap_abstract1_assign_linexpr(man, false, &pivot2, "E1", &E0, NULL);
			_36 = ap_abstract1_assign_linexpr(man, false, &_35, "E0", &E, NULL);	
			_37 = ap_abstract1_assign_linexpr(man, false, &_36, "E", &top, NULL);
			_38 = ap_abstract1_meet_lincons_array(man, false, &_37, &array_consE);
			_39 = ap_abstract1_assign_linexpr(man, false, &_38, "S1", &S0, NULL);
			_40 = ap_abstract1_assign_linexpr(man, false, &_39, "S0", &S, NULL);
			_41 = ap_abstract1_assign_linexpr(man, false, &_40, "S", &h, NULL);

			_42 = ap_abstract1_assign_linexpr(man, false, &_41, "Ep1", &Ep0, NULL);	
			_43 = ap_abstract1_assign_linexpr(man, false, &_42, "Ep0", &Ep, NULL);		
			_44 = ap_abstract1_assign_linexpr(man, false, &_43, "Ep", &S, NULL);		
			_45 = ap_abstract1_assign_linexpr(man, false, &_44, "Sp1", &Sp0, NULL);		
			_46 = ap_abstract1_assign_linexpr(man, false, &_45, "Sp0", &Sp, NULL);	
			_47 = ap_abstract1_assign_linexpr(man, false, &_46, "Sp", &hp, NULL);	

			_48 = ap_abstract1_assign_linexpr(man, false, &_47, "Eq1", &Eq0, NULL);	
			_49 = ap_abstract1_assign_linexpr(man, false, &_48, "Eq0", &Eq, NULL);		
			_50 = ap_abstract1_assign_linexpr(man, false, &_49, "Eq", &Sp, NULL);		
			_51 = ap_abstract1_assign_linexpr(man, false, &_50, "Sq1", &Sq0, NULL);		
			_52 = ap_abstract1_assign_linexpr(man, false, &_51, "Sq0", &Sq, NULL);	
			_53 = ap_abstract1_assign_linexpr(man, false, &_52, "Sq", &hq, NULL);	

			_54 = ap_abstract1_assign_linexpr(man, false, &_53, "En1", &En0, NULL);	
			_55 = ap_abstract1_assign_linexpr(man, false, &_54, "En0", &En, NULL);		
			_56 = ap_abstract1_assign_linexpr(man, false, &_55, "En", &Sq, NULL);		
			_57 = ap_abstract1_assign_linexpr(man, false, &_56, "Sn1", &Sn0, NULL);		
			_58 = ap_abstract1_assign_linexpr(man, false, &_57, "Sn0", &Sn, NULL);	
			_59 = ap_abstract1_assign_linexpr(man, false, &_58, "Sn", &hn, NULL);	

			ap_abstract1_clear(man, &pivot2);
			pivot2 = ap_abstract1_copy(man, &_59);

			ap_abstract1_clear(man, &_35);
			ap_abstract1_clear(man, &_36);
			ap_abstract1_clear(man, &_37);
			ap_abstract1_clear(man, &_38);
			ap_abstract1_clear(man, &_39);
			ap_abstract1_clear(man, &_40);
			ap_abstract1_clear(man, &_41);
			ap_abstract1_clear(man, &_42);
			ap_abstract1_clear(man, &_43);
			ap_abstract1_clear(man, &_44);
			ap_abstract1_clear(man, &_45);
			ap_abstract1_clear(man, &_46);
			ap_abstract1_clear(man, &_47);
			ap_abstract1_clear(man, &_48);
			ap_abstract1_clear(man, &_49);
			ap_abstract1_clear(man, &_50);
			ap_abstract1_clear(man, &_51);
			ap_abstract1_clear(man, &_52);
			ap_abstract1_clear(man, &_53);
			ap_abstract1_clear(man, &_54);
			ap_abstract1_clear(man, &_55);
			ap_abstract1_clear(man, &_56);
			ap_abstract1_clear(man, &_57);
			ap_abstract1_clear(man, &_58);
			ap_abstract1_clear(man, &_59);
			i++;
		}

		/* F^c(_k2) <= ?? _k2 */

		if (ap_abstract1_is_leq(man, &pivot2, &_k2) == tbool_true) {
			box = ap_abstract1_to_box(man, &_k2);
			ap_double_set_scalar(inf, box.p[15]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[15]->sup, GMP_RNDU);
			ap_box1_clear(&box);
                	*N = j;

			/* Special T1P */
			if (T1P) {
				i = INITIAL_UNFOLD;
				array[i] = ap_abstract1_copy(man, &_k2);
				while (i<INITIAL_UNFOLD+THRESHOLD-1) 
				{
					_35 = ap_abstract1_assign_linexpr(man, false, &array[i], "E1", &E0, NULL);
					_36 = ap_abstract1_assign_linexpr(man, false, &_35, "E0", &E, NULL);	
					_37 = ap_abstract1_assign_linexpr(man, false, &_36, "E", &top, NULL);
					_38 = ap_abstract1_meet_lincons_array(man, false, &_37, &array_consE);
					_39 = ap_abstract1_assign_linexpr(man, false, &_38, "S1", &S0, NULL);
					_40 = ap_abstract1_assign_linexpr(man, false, &_39, "S0", &S, NULL);
					_41 = ap_abstract1_assign_linexpr(man, false, &_40, "S", &h, NULL);

					_42 = ap_abstract1_assign_linexpr(man, false, &_41, "Ep1", &Ep0, NULL);	
					_43 = ap_abstract1_assign_linexpr(man, false, &_42, "Ep0", &Ep, NULL);		
					_44 = ap_abstract1_assign_linexpr(man, false, &_43, "Ep", &S, NULL);		
					_45 = ap_abstract1_assign_linexpr(man, false, &_44, "Sp1", &Sp0, NULL);		
					_46 = ap_abstract1_assign_linexpr(man, false, &_45, "Sp0", &Sp, NULL);	
					_47 = ap_abstract1_assign_linexpr(man, false, &_46, "Sp", &hp, NULL);	

					_48 = ap_abstract1_assign_linexpr(man, false, &_47, "Eq1", &Eq0, NULL);	
					_49 = ap_abstract1_assign_linexpr(man, false, &_48, "Eq0", &Eq, NULL);		
					_50 = ap_abstract1_assign_linexpr(man, false, &_49, "Eq", &Sp, NULL);		
					_51 = ap_abstract1_assign_linexpr(man, false, &_50, "Sq1", &Sq0, NULL);		
					_52 = ap_abstract1_assign_linexpr(man, false, &_51, "Sq0", &Sq, NULL);	
					_53 = ap_abstract1_assign_linexpr(man, false, &_52, "Sq", &hq, NULL);	

					_54 = ap_abstract1_assign_linexpr(man, false, &_53, "En1", &En0, NULL);	
					_55 = ap_abstract1_assign_linexpr(man, false, &_54, "En0", &En, NULL);		
					_56 = ap_abstract1_assign_linexpr(man, false, &_55, "En", &Sq, NULL);		
					_57 = ap_abstract1_assign_linexpr(man, false, &_56, "Sn1", &Sn0, NULL);		
					_58 = ap_abstract1_assign_linexpr(man, false, &_57, "Sn0", &Sn, NULL);	
					_59 = ap_abstract1_assign_linexpr(man, false, &_58, "Sn", &hn, NULL);	

					array[i+1] = ap_abstract1_copy(man, &_59);

					ap_abstract1_clear(man, &_35);
					ap_abstract1_clear(man, &_36);
					ap_abstract1_clear(man, &_37);
					ap_abstract1_clear(man, &_38);
					ap_abstract1_clear(man, &_39);
					ap_abstract1_clear(man, &_40);
					ap_abstract1_clear(man, &_41);
					ap_abstract1_clear(man, &_42);
					ap_abstract1_clear(man, &_43);
					ap_abstract1_clear(man, &_44);
					ap_abstract1_clear(man, &_45);
					ap_abstract1_clear(man, &_46);
					ap_abstract1_clear(man, &_47);
					ap_abstract1_clear(man, &_48);
					ap_abstract1_clear(man, &_49);
					ap_abstract1_clear(man, &_50);
					ap_abstract1_clear(man, &_51);
					ap_abstract1_clear(man, &_52);
					ap_abstract1_clear(man, &_53);
					ap_abstract1_clear(man, &_54);
					ap_abstract1_clear(man, &_55);
					ap_abstract1_clear(man, &_56);
					ap_abstract1_clear(man, &_57);
					ap_abstract1_clear(man, &_58);
					ap_abstract1_clear(man, &_59);
					i++;
				}
				fix = ap_abstract1_join_array(man, array, INITIAL_UNFOLD+THRESHOLD);

				box = ap_abstract1_to_box(man, &fix);
				ap_double_set_scalar(inf, box.p[15]->inf, GMP_RNDU);
				ap_double_set_scalar(sup, box.p[15]->sup, GMP_RNDU);
				ap_box1_clear(&box);
				printf("T1P: fixpoint of F: [%.2f, %.2f]\n",*inf, *sup);
				for (k=0;k<INITIAL_UNFOLD+THRESHOLD;k++) ap_abstract1_clear(man, &array[k]);
				ap_abstract1_clear(man, &fix);
			}
			box = ap_abstract1_to_box(man, &_k2);
			ap_double_set_scalar(inf, box.p[15]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[15]->sup, GMP_RNDU);
			ap_box1_clear(&box);
			ap_abstract1_clear(man, &pivot2);
			ap_abstract1_clear(man, &_k1);
			ap_abstract1_clear(man, &_k2);
			/* END SEPCIAL T1P */
			break;
		}
		ap_abstract1_clear(man, &pivot2);

		if (j==MAX_IT && T1P) {
			printf("\nT\n");
			box = ap_abstract1_to_box(man, &_k2);
			ap_double_set_scalar(inf, box.p[15]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[15]->sup, GMP_RNDU);
			ap_box1_clear(&box);
			*N = j;
			ap_abstract1_clear(man, &_k1);
			ap_abstract1_clear(man, &_k2);
			break;
		}
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
	ap_linexpr1_clear(&hp);
	ap_linexpr1_clear(&hq);
	ap_linexpr1_clear(&hn);

	ap_linexpr1_clear(&S);
	ap_linexpr1_clear(&S0);

	ap_linexpr1_clear(&Ep);
	ap_linexpr1_clear(&Ep0);
	ap_linexpr1_clear(&Sp);
	ap_linexpr1_clear(&Sp0);

	ap_linexpr1_clear(&Eq);
	ap_linexpr1_clear(&Eq0);
	ap_linexpr1_clear(&Sq);
	ap_linexpr1_clear(&Sq0);

	ap_linexpr1_clear(&En);
	ap_linexpr1_clear(&En0);
	ap_linexpr1_clear(&Sn);
	ap_linexpr1_clear(&Sn0);

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
	ap_abstract1_clear(man, &_10);	
	ap_abstract1_clear(man, &_11);
	ap_abstract1_clear(man, &_12);
	ap_abstract1_clear(man, &_13);
	ap_abstract1_clear(man, &_14);
	ap_abstract1_clear(man, &_15);
	ap_abstract1_clear(man, &_16);
	ap_abstract1_clear(man, &_17);
	ap_abstract1_clear(man, &_18);
	ap_abstract1_clear(man, &_19);
	ap_abstract1_clear(man, &_20);
	ap_abstract1_clear(man, &_21);
	ap_abstract1_clear(man, &_22);
	ap_abstract1_clear(man, &_23);
	ap_abstract1_clear(man, &_24);
	ap_abstract1_clear(man, &_25);
	ap_abstract1_clear(man, &_26);
	ap_abstract1_clear(man, &_27);
	ap_environment_free (env);
}

int main(void)
{
	ap_manager_t* man;
	clock_t start, end;
	double time;
	double cpu_time_used;
	double inf, sup;
	int N,l;
	FILE* stream = fopen("/tmp/logg", "w+");
	widening = 0;
//	mtrace();
	man = t1p_manager_alloc();
	start = clock();
	for (l=0;l<1;l++) {widening=0;filter1(man, &N, &inf, &sup, stream, &cpu_time_used);}
	end = clock();
	time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("it:%d\nwidenings:%d\ntime:%.2f\noutput:[%.2f, %.2f]\n", N, widening, time, inf, sup);
	ap_manager_free(man);
	fclose(stream);
	return 0;
}

