#include <time.h>
#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "ap_ppl.h"
#include "box.h"

#define T1P		1

#define SDP		1

void multi(ap_manager_t* man, double *inf, double *sup, FILE* stream);

void multi(ap_manager_t* man, double *inf, double *sup, FILE* stream)
{
	clock_t start, end;
	double cpu_time_used;
	ap_var_t name_of_dim[5] = {"eps0","x","y","z","t"};
	ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],5);

	/* top */
	ap_texpr1_t* top = ap_texpr1_cst_interval_top(env);

	/* y = x + eps0 */
	ap_linexpr1_t y = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)2);
	ap_linexpr1_set_coeff_scalar_double(&y, "x", 1.);
	ap_linexpr1_set_coeff_scalar_double(&y, "eps0", 1.);

	/* z = x*y */
	ap_texpr1_t* z = ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_var(env, "x"),
		ap_texpr1_var(env, "y"),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP);

	/* t = z - 2x -y */
	ap_linexpr1_t t = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)3);
	ap_linexpr1_set_coeff_scalar_double(&t, "x", -2.);
	ap_linexpr1_set_coeff_scalar_double(&t, "y", -1.);
	ap_linexpr1_set_coeff_scalar_double(&t, "z", 1.);

	ap_linexpr1_fprint(stdout, &y);
	printf("\n");
	ap_texpr1_fprint(stdout, z);
	printf("\n");
	ap_linexpr1_fprint(stdout, &t);
	printf("\n");

	ap_lincons1_array_t array_consEps0 = ap_lincons1_array_make(env, 2);
	/* INF_A <= A */
	ap_linexpr1_t lexp1 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &lexp1, NULL);
	ap_lincons1_set_list(&cons1,
			AP_COEFF_S_DOUBLE, 1., "eps0",
			AP_CST_S_DOUBLE, (double)0.0,
			AP_END);
	ap_lincons1_array_set(&array_consEps0,0,&cons1);
	/* A <= SUP_A */
	ap_linexpr1_t lexp2 = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ,&lexp2,NULL);
	ap_lincons1_set_list(&cons2,
			AP_COEFF_S_DOUBLE, -1., "eps0",
			AP_CST_S_DOUBLE, (double)2,
			AP_END);
	ap_lincons1_array_set(&array_consEps0,1,&cons2);

	ap_lincons1_array_t array_consX = ap_lincons1_array_make(env, 2);
	/* INF_A <= A */
	ap_linexpr1_t lexp1y = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons1y = ap_lincons1_make(AP_CONS_SUPEQ, &lexp1y, NULL);
	ap_lincons1_set_list(&cons1y,
			AP_COEFF_S_DOUBLE, 1., "x",
			AP_CST_S_DOUBLE, (double)0.0,
			AP_END);
	ap_lincons1_array_set(&array_consX,0,&cons1y);
	/* A <= SUP_A */
	ap_linexpr1_t lexp2y = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
	ap_lincons1_t cons2y = ap_lincons1_make(AP_CONS_SUPEQ,&lexp2y,NULL);
	ap_lincons1_set_list(&cons2y,
			AP_COEFF_S_DOUBLE, -1., "x",
			AP_CST_S_DOUBLE, (double)2,
			AP_END);
	ap_lincons1_array_set(&array_consX,1,&cons2y);

	/*=========*/
	/* ANALYSE */
	/*=========*/
	/* starting with top */
	ap_abstract1_t _8 = ap_abstract1_top(man, env);

	ap_abstract1_t _9 = ap_abstract1_assign_texpr(man, false, &_8, "x", top, NULL);
	ap_abstract1_t _10 =  ap_abstract1_meet_lincons_array(man, false, &_9, &array_consX);

	ap_abstract1_t _11 = ap_abstract1_assign_texpr(man, false, &_10, "eps0", top, NULL);
	ap_abstract1_t _12 =  ap_abstract1_meet_lincons_array(man, false, &_11, &array_consEps0);

	ap_abstract1_t _13 = ap_abstract1_assign_linexpr(man, false, &_12, "y", &y, NULL);
	ap_abstract1_t _14 = ap_abstract1_assign_texpr(man, false, &_13, "z", z, NULL);
	ap_abstract1_t _15 = ap_abstract1_assign_linexpr(man, false, &_14, "t", &t, NULL);

	ap_abstract1_fprint(stdout, man, &_15);

	ap_box1_t box;
	box = ap_abstract1_to_box(man, &_15);
	ap_double_set_scalar(inf, box.p[1]->inf, GMP_RNDU);
        ap_double_set_scalar(sup, box.p[1]->sup, GMP_RNDU);
//	ap_box1_fprint(stdout, &box);
	printf("t in [%f,%f]\n",*inf, *sup);
	ap_double_set_scalar(inf, box.p[2]->inf, GMP_RNDU);
        ap_double_set_scalar(sup, box.p[2]->sup, GMP_RNDU);
//	ap_box1_fprint(stdout, &box);
	printf("x in [%f,%f]\n",*inf, *sup);

	ap_var_t pack[5] = {"eps0","y","z"};
	ap_abstract1_t _16 = ap_abstract1_forget_array(man, false, &_15, pack, 3, false);
	ap_abstract1_fprint(stdout, man, &_16);

	box = ap_abstract1_to_box(man, &_16);
	ap_double_set_scalar(inf, box.p[1]->inf, GMP_RNDU);
        ap_double_set_scalar(sup, box.p[1]->sup, GMP_RNDU);
	printf("t in [%f,%f]\n",*inf, *sup);

	ap_box1_clear(&box);

	/* freeing memory used */
	ap_lincons1_array_clear(&array_consEps0);
	ap_lincons1_array_clear(&array_consX);
	ap_linexpr1_clear(&y);
	ap_texpr1_free(z);
	ap_linexpr1_clear(&t);
	ap_texpr1_free(top);
	ap_abstract1_clear(man, &_8);
	ap_abstract1_clear(man, &_9);
	ap_abstract1_clear(man, &_10);
	ap_abstract1_clear(man, &_11);
	ap_abstract1_clear(man, &_12);
	ap_abstract1_clear(man, &_13);
	ap_abstract1_clear(man, &_14);
	ap_abstract1_clear(man, &_15);
	ap_abstract1_clear(man, &_16);
	ap_environment_free (env);
}

int main(void)
{
	ap_manager_t* man;
	clock_t start, end;
	double cpu_time;
	FILE* stream = fopen("/tmp/logg", "w+");
	double inf, sup;

	man = t1p_manager_alloc();
	start = clock();
	if (T1P && SDP) {
		ap_funopt_t option = ap_manager_get_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY);
		option.algorithm = INT_MAX;
		ap_manager_set_funopt(man, AP_FUNID_ASSIGN_TEXPR_ARRAY, &option);
	}
	multi(man, &inf, &sup, stream);
	end = clock();
	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
//	printf("it:%d\nwidenings:%d\ntime:%.2g\noutput:[%.2f, %.2f]\n", N, widening, cpu_time, inf, sup);
	ap_manager_free(man);
	fclose(stream);	
	return 0;
}

