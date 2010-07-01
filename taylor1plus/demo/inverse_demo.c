#include <time.h>
#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "ap_ppl.h"
#include "box.h"

#define T1P			1
#define LOOP			0
#define MAX_IT			1000
#define INITIAL_UNFOLD  	10
#define THRESHOLD		1
#define WIDENING_THRESHOLD	100

#define SDP		1
#define INF_A		16
#define SUP_A		20
#define EPSILON		0.00001

static int widening;

void sqrt_householder(ap_manager_t* man, double init, int *N, double *inf, double *sup, FILE* stream);

void sqrt_householder(ap_manager_t* man, double init, int *N, double *inf, double *sup, FILE* stream)
{
	int wide = 0;
	clock_t start, end;
	double cpu_time_used;
	ap_var_t name_of_dim[6] = {"A","hn","xn","xnp1","z","zz"};
	ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],6);

	ap_texpr1_t* one = ap_texpr1_cst_scalar_double(env, 1.0);
	ap_texpr1_t* deux = ap_texpr1_cst_scalar_double(env, 2.0);
	ap_texpr1_t* c3d8 = ap_texpr1_cst_scalar_double(env, 0.375);
	ap_texpr1_t* half = ap_texpr1_cst_scalar_double(env, 0.5);
	ap_texpr1_t* xn_init = ap_texpr1_cst_scalar_double(env, init);
	/* ibot */
	ap_linexpr1_t bot = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
	ap_interval_t* ibot = ap_interval_alloc();
	ap_interval_set_bottom(ibot);
	ap_linexpr1_set_cst_interval(&bot, ibot);
	/* xnp1 = xn - A*xn*xn */
	ap_texpr1_t* xnp1_scheme = ap_texpr1_binop(AP_TEXPR_SUB,
			ap_texpr1_binop(AP_TEXPR_MUL,
			deux,
			ap_texpr1_var(env, "xn"),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP),
			ap_texpr1_binop(AP_TEXPR_MUL, 
				ap_texpr1_binop(AP_TEXPR_MUL,
					ap_texpr1_var(env, "A"),
					ap_texpr1_var(env, "xn"),
					AP_RTYPE_DOUBLE,
					AP_RDIR_UP),
				ap_texpr1_var(env, "xn"),
				AP_RTYPE_DOUBLE,
				AP_RDIR_UP),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP);

	ap_texpr1_t* xnp1 = ap_texpr1_var(env, "xnp1");
	ap_texpr1_t* hn = ap_texpr1_var(env, "xnp1");
	/* z = A * 1/sqrt(A) = A * xn */
	ap_texpr1_t* z = ap_texpr1_binop(AP_TEXPR_DIV,
			one,
			ap_texpr1_var(env, "xn"),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP);

	ap_linexpr1_t epsilon = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
	ap_linexpr1_set_list(&epsilon,
			AP_COEFF_S_DOUBLE, 1.,"xnp1",
			AP_COEFF_S_DOUBLE, -1.,"xn",
			AP_END);
	/* [-oo, +oo] */
	ap_texpr1_t* top = ap_texpr1_cst_interval_top(env);

	ap_lincons1_array_t array_consA = ap_lincons1_array_make(env, 2);
	/* INF_A <= A */
	ap_linexpr1_t lexp1 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &lexp1, NULL);
	ap_lincons1_set_list(&cons1,
			AP_COEFF_S_DOUBLE, 1., "A",
			AP_CST_S_DOUBLE, (double)-INF_A,
			AP_END);
	ap_lincons1_array_set(&array_consA,0,&cons1);
	/* A <= SUP_A */
	ap_linexpr1_t lexp2 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ,&lexp2,NULL);
	ap_lincons1_set_list(&cons2,
			AP_COEFF_S_DOUBLE, -1., "A",
			AP_CST_S_DOUBLE, (double)SUP_A,
			AP_END);
	ap_lincons1_array_set(&array_consA,1,&cons2);

	/*=========*/
	/* ANALYSE */
	/*=========*/
	/* starting with top */
	ap_abstract1_t _8 = ap_abstract1_top(man, env);
	/* A = random */
	ap_abstract1_t _9 = ap_abstract1_assign_texpr(man, false, &_8, "A", top, NULL);
	/* assume A>=INF_A and A<=SUP_A */
	ap_abstract1_t _10=  ap_abstract1_meet_lincons_array(man, false, &_9, &array_consA);
	/* xn = init */
	ap_abstract1_t _11 = ap_abstract1_assign_texpr(man, false, &_10, "xn", xn_init, NULL);

	ap_abstract1_t _16 = ap_abstract1_assign_linexpr(man, false, &_11, "xnp1", &bot, NULL);
	ap_abstract1_t _17 = ap_abstract1_assign_linexpr(man, false, &_16, "hn", &bot, NULL);
	ap_abstract1_t _18 = ap_abstract1_assign_linexpr(man, false, &_17, "z", &bot, NULL);
	ap_abstract1_t _19 = ap_abstract1_assign_linexpr(man, false, &_18, "zz", &bot, NULL);

	ap_box1_t box;
	ap_abstract1_t _12, _13, _14, _15, pivot, pivot2, _k1, _k2, stop;
	/* initial unfolding */
	int i = 0;
	ap_abstract1_t array[THRESHOLD+INITIAL_UNFOLD+1];
	pivot = ap_abstract1_copy(man, &_11);
	array[i] = ap_abstract1_copy(man, &pivot);
	while (i<INITIAL_UNFOLD) 
	{
		fprintf(stream, "\n***** X_%d *****\n",i);
		ap_abstract1_fdump(stream, man, &pivot);
		//if (i>0) array[i] = ap_abstract1_copy(man, &pivot);
		_12 = ap_abstract1_assign_texpr(man, false, &pivot, "xnp1", xnp1_scheme, NULL);
		//_15 = ap_abstract1_assign_texpr(man, false, &_12, "z", z, NULL);
		_14 = ap_abstract1_assign_texpr(man, false, &_12, "xn", xnp1, NULL);
		ap_abstract1_clear(man, &pivot);
		pivot = ap_abstract1_copy(man, &_14);
		//array[i+1] = ap_abstract1_copy(man, &_15);
		ap_abstract1_clear(man, &_12);
		//ap_abstract1_clear(man, &_13);
		ap_abstract1_clear(man, &_14);
//		_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
		box = ap_abstract1_to_box(man, &pivot);
		ap_double_set_scalar(inf, box.p[2]->inf, GMP_RNDU);
		ap_double_set_scalar(sup, box.p[2]->sup, GMP_RNDU);
		printf("[%.5f, %.5f]\n",*inf, *sup);
		ap_box1_clear(&box);
		//ap_abstract1_clear(man, &_15);
		i++;
	}
	if (!LOOP) {
		//_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
		box = ap_abstract1_to_box(man, &pivot);
		ap_double_set_scalar(inf, box.p[2]->inf, GMP_RNDU);
		ap_double_set_scalar(sup, box.p[2]->sup, GMP_RNDU);
		ap_box1_clear(&box);
		//ap_abstract1_clear(man, &_15);
		*N = INITIAL_UNFOLD;
	}
	int j = 0;
	while (LOOP)
	{
		//fprintf(stream,"\n");
{
		_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
		box = ap_abstract1_to_box(man, &_15);
		ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
		ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
		ap_box1_clear(&box);
		printf("%.16e, %.16e\n",*inf, *sup);
		ap_abstract1_clear(man, &_15);
	}
		_k1 = ap_abstract1_copy(man, &pivot);
		i = 0;
		/* F^c (x_n) */
		while (i<THRESHOLD) 
		{
			_12 = ap_abstract1_assign_texpr(man, false, &pivot, "hn", hn, NULL);	
			_13 = ap_abstract1_assign_texpr(man, false, &_12, "xnp1", xnp1_scheme, NULL);	
			_14 = ap_abstract1_assign_texpr(man, false, &_13, "xn", xnp1, NULL);	
			ap_abstract1_clear(man, &pivot);
			pivot = ap_abstract1_copy(man, &_14);
			ap_abstract1_clear(man, &_12);
			ap_abstract1_clear(man, &_13);
			ap_abstract1_clear(man, &_14);
			i++;
		}

//			ap_abstract1_fdump(stream, man, &pivot);
		/* convergence test */
//		i=0;
//		pivot2 = ap_abstract1_copy(man, &_k2);
//		while (i<THRESHOLD) 
//		{
//			_12 = ap_abstract1_assign_texpr(man, false, &pivot2, "hn", hn, NULL);	
//			_13 = ap_abstract1_assign_texpr(man, false, &_12, "xnp1", xnp1_scheme, NULL);	
//			_14 = ap_abstract1_assign_texpr(man, false, &_13, "xn", xnp1, NULL);	
//			ap_abstract1_clear(man, &pivot2);
//			pivot2 = ap_abstract1_copy(man, &_14);
//			ap_abstract1_clear(man, &_12);
//			ap_abstract1_clear(man, &_13);
//			ap_abstract1_clear(man, &_14);
//			i++;
//		}
		if (ap_abstract1_is_leq(man, &pivot, &_k1) == tbool_true) {
			/* compute fixpoint of F */
			//ap_abstract1_fprint(stdout, man, &_k1);
			if (T1P) {
				i = INITIAL_UNFOLD;
				_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
				array[INITIAL_UNFOLD+THRESHOLD] = ap_abstract1_copy(man, &_15);
//				while (i<INITIAL_UNFOLD+THRESHOLD-1)
//				{
//					_12 = ap_abstract1_assign_texpr(man, false, &array[i], "hn", hn, NULL);
//					_13 = ap_abstract1_assign_texpr(man, false, &_12, "xnp1", xnp1_scheme, NULL);
//					_14 = ap_abstract1_assign_texpr(man, false, &_13, "xn", xnp1, NULL);
//				_15 = ap_abstract1_assign_texpr(man, false, &_15, "z", z, NULL);
//					array[i+1] = ap_abstract1_copy(man, &_15);
//					ap_abstract1_clear(man, &_12);
//					ap_abstract1_clear(man, &_13);
//					ap_abstract1_clear(man, &_14);
//					ap_abstract1_clear(man, &_15);
//					i++;
//				}
				ap_abstract1_t fix = ap_abstract1_join_array(man, array, INITIAL_UNFOLD+THRESHOLD+1);
		i=0;
//		while (i<THRESHOLD) 
//		{
//			_12 = ap_abstract1_assign_texpr(man, false, &fix, "hn", hn, NULL);	
//			_13 = ap_abstract1_assign_texpr(man, false, &_12, "xnp1", xnp1_scheme, NULL);	
//			_14 = ap_abstract1_assign_texpr(man, false, &_13, "xn", xnp1, NULL);	
//			ap_abstract1_clear(man, &pivot);
//			pivot = ap_abstract1_copy(man, &_14);
//			ap_abstract1_clear(man, &_12);
//			ap_abstract1_clear(man, &_13);
//			ap_abstract1_clear(man, &_14);
//			i++;
//		}
				//_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
				fprintf(stream,"\n**** fix ****\n");
				ap_abstract1_fdump(stream, man, &fix);
				box = ap_abstract1_to_box(man, &fix);
				ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
				ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
				ap_box1_clear(&box);
				printf("T1P: fixpoint of F: [%.4f, %.4f]\n",*inf, *sup);
				for(i=0;i<INITIAL_UNFOLD+THRESHOLD+1;i++) {ap_abstract1_clear(man, &array[i]);}
				ap_abstract1_clear(man, &fix);
				//ap_abstract1_clear(man, &_15);
			}
			//_15 = ap_abstract1_assign_texpr(man, false, &pivot, "z", z, NULL);
			//	fprintf(stream,"\n**** fixpoint ****\n");
			//ap_abstract1_fdump(stream, man, &_15);
			//ap_abstract1_fprint(stdout, man, &_15);
			box = ap_abstract1_to_box(man, &_15);
			ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
			ap_box1_clear(&box);
			*N = j;
			// fonctionnel
			//ap_abstract1_t _16 = ap_abstract1_assign_texpr(man, false, &_15, "zz", zz, NULL);
			ap_abstract1_clear(man, &_15);

			//ap_abstract1_clear(man, &pivot2);
			//ap_abstract1_clear(man, &_k2);
			ap_abstract1_clear(man, &_k1);			
			break;
		} else {
			if ((j*THRESHOLD + widening==MAX_IT) && T1P) {
				printf("\nT\n");
				//box = ap_abstract1_to_box(man, &pivot);
				//ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
				//ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
				//ap_box1_clear(&box);
				*N = j;
				//ap_abstract1_clear(man, &pivot);
				break;
			}
			if (j*THRESHOLD >= WIDENING_THRESHOLD && WIDENING_THRESHOLD>0) wide = 1;
			if (wide) {
				_k2 = ap_abstract1_widening(man, &_k1, &pivot);
				widening++;
			} else {_k2 = ap_abstract1_join(man, false, &_k1, &pivot);j++;}
			fprintf(stream, "\n***** X_%d *****\n",j-1);
			ap_abstract1_fdump(stream, man, &_k1);
			fprintf(stream, "\n***** F(X_%d) *****\n",j-1);
			ap_abstract1_fdump(stream, man, &pivot);
			fprintf(stream, "\n***** X_%d U F(X_%d) *****\n",j-1,j-1);
			ap_abstract1_fdump(stream, man, &_k2);
		}
//		if (ap_abstract1_is_leq(man, &_k2, &_k1) == tbool_true) {
//			printf("iso eq\n"); 
//			if (T1P) {
//				i = INITIAL_UNFOLD;
//				array[i] = ap_abstract1_copy(man, &_k1);
//				while (i<INITIAL_UNFOLD+THRESHOLD-1)
//				{
//					_12 = ap_abstract1_assign_texpr(man, false, &array[i], "hn", hn, NULL);
//					_13 = ap_abstract1_assign_texpr(man, false, &_12, "xnp1", xnp1_scheme, NULL);
//					_14 = ap_abstract1_assign_texpr(man, false, &_13, "xn", xnp1, NULL);
//					array[i+1] = ap_abstract1_copy(man, &_14);
//					ap_abstract1_clear(man, &_12);
//					ap_abstract1_clear(man, &_13);
//					ap_abstract1_clear(man, &_14);
//					i++;
//				}
//				ap_abstract1_t fix = ap_abstract1_join_array(man, array, INITIAL_UNFOLD+THRESHOLD);
//				_15 = ap_abstract1_assign_texpr(man, false, &fix, "z", z, NULL);
//				fprintf(stream,"\n**** fix ****\n");
//				ap_abstract1_fdump(stream, man, &_15);
//				box = ap_abstract1_to_box(man, &_15);
//				ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
//				ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
//				ap_box1_clear(&box);
//				printf("T1P: fixpoint of F: [%.2f, %.2f]\n",*inf, *sup);
//				for(i=0;i<INITIAL_UNFOLD+THRESHOLD;i++) {ap_abstract1_clear(man, &array[i]);}
//				ap_abstract1_clear(man, &fix);
//				ap_abstract1_clear(man, &_15);
//			}
//			_15 = ap_abstract1_assign_texpr(man, false, &_k1, "z", z, NULL);
//				fprintf(stream,"\n**** fixpoint ****\n");
//			ap_abstract1_fdump(stream, man, &_15);
//			box = ap_abstract1_to_box(man, &_15);
//			ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
//			ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
//			ap_box1_clear(&box);
//			*N = j;
//			ap_abstract1_clear(man, &_15);
//			ap_abstract1_clear(man, &_k1);			
//			break;
//		}
		//ap_abstract1_clear(man, &pivot2);
		/* end of convergence test */
			if (0) {
			/* critere boucle */
			stop = ap_abstract1_assign_linexpr(man, false, &_k2, "zz", &epsilon, NULL);
			box = ap_abstract1_to_box(man, &stop);
			ap_double_set_scalar(inf, box.p[5]->inf, GMP_RNDU);
			ap_double_set_scalar(sup, box.p[5]->sup, GMP_RNDU);
			//printf("[%.20g,%.20g]\n",*inf, *sup);
			ap_box1_clear(&box);
			if (*sup <= EPSILON && -1*EPSILON <= *inf) {
				printf("critère d'arrêt de la boucle vérifié\n");
				_15 = ap_abstract1_assign_texpr(man, false, &stop, "z", z, NULL);
				box = ap_abstract1_to_box(man, &_15);
				ap_double_set_scalar(inf, box.p[4]->inf, GMP_RNDU);
				ap_double_set_scalar(sup, box.p[4]->sup, GMP_RNDU);
				ap_box1_clear(&box);
				*N = j;
				ap_abstract1_clear(man, &_15);
				ap_abstract1_clear(man, &stop);
				ap_abstract1_clear(man, &_k2);
				ap_abstract1_clear(man, &_k1);			
				goto L1;
			}
			ap_abstract1_clear(man, &stop);
			/* fin critere boucle */
			}
		ap_abstract1_clear(man, &pivot);
		pivot = ap_abstract1_copy(man, &_k2);
		ap_abstract1_clear(man, &_k2);
		ap_abstract1_clear(man, &_k1);
	}
	
L1:
	/* freeing memory used */
	ap_lincons1_array_clear(&array_consA);
	ap_texpr1_free(z);
	ap_texpr1_free(xn_init);
	ap_texpr1_free(xnp1);
	ap_texpr1_free(xnp1_scheme);
	ap_texpr1_free(hn);
	ap_texpr1_free(top);
	ap_linexpr1_clear(&bot);
	ap_linexpr1_clear(&epsilon);
	ap_abstract1_clear(man, &pivot);
	ap_abstract1_clear(man, &_8);
	ap_abstract1_clear(man, &_9);
	ap_abstract1_clear(man, &_10);
	ap_abstract1_clear(man, &_11);
	ap_abstract1_clear(man, &_16);
	ap_abstract1_clear(man, &_17);
	ap_abstract1_clear(man, &_18);
	ap_abstract1_clear(man, &_19);
	ap_environment_free (env);
}

int main(void)
{
	ap_manager_t* man;
	clock_t start, end;
	double cpu_time;
	FILE* stream = fopen("/tmp/logg", "w+");
	double init = 0.0625;
	double inf, sup;
	int N,l;

	widening = 0;
	/* init manager */
	man = t1p_manager_alloc();
	start = clock();
	if (T1P && SDP) {
		ap_funopt_t option = ap_manager_get_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
		option.algorithm = INT_MAX;
		ap_manager_set_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY, &option);
	}
	for (l=0;l<1;l++) {widening=0;sqrt_householder(man, init, &N, &inf, &sup, stream);}
	end = clock();
	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("it:%d\nwidenings:%d\ntime:%.2g\noutput:[%.10f, %.10f]\n", N, widening, cpu_time, inf, sup);
	ap_manager_free(man);
	fclose(stream);	
	return 0;
}

