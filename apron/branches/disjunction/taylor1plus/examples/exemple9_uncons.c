#include <time.h>
#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/*******************************
  Y = 0;
  while rand {
    X dans [-128, 128];
    D dans [0, 16];
    S = Y;
    R = X  - S;
    Y = X;
    if R  <= -0.5*D { Y = S  - D  } 
    else
      if 0.5*D  <= R { Y = S  + D  }
 }

  ******************************/

int main(void) {
    clock_t start, end;
    double time;
    double cpu_time_used;
    start = clock();
    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit, ici les octogons */
    //ap_manager_t* manNS = oct_manager_alloc();
    /* true means strict constraints are supported */
    //ap_manager_t* manNS = pk_manager_alloc(true);
    ap_manager_t* manNS = box_manager_alloc();
    /* Le domaine abstrait T1+ est paramétré par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);
    //ap_manager_t* man = ap_ppl_poly_manager_alloc(true);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[5] = {"d","r","s","x","y"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 5);

    /* Créer un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(5);
    ap_interval_set_double(array[0], 0.0, 16.0);	/* array[0] = [1,1] */
    ap_interval_set_double(array[3], -128.0, 128.0);	/* array[1] = [2,2] */
    ap_interval_set_top(array[1]);
    ap_interval_set_top(array[2]);
    ap_interval_set_double(array[4], 0.0, 0.0);


    ap_abstract1_t abs = ap_abstract1_of_box(man, env, name_of_dim, array, 5);
    //ap_abstract1_fprint(stdout, man, &abs);

    ap_linexpr1_t x = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&x, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&x, "x", (double)(1.0));

    ap_linexpr1_t y = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&y, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&y, "y", (double)(1.0));

    ap_linexpr1_t xms = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&xms, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&xms, "x", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&xms, "s", (double)(-1.0));

    ap_linexpr1_t mdmr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&mdmr, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&mdmr, "r", (double)(-1.0));
    ap_linexpr1_set_coeff_scalar_double(&mdmr, "d", (double)(-0.5));

    ap_linexpr1_t rmd = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&rmd, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&rmd, "r", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&rmd, "d", (double)(-0.5));

    ap_linexpr1_t rpd = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&rpd, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&rpd, "r", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&rpd, "d", (double)(0.5));

    ap_linexpr1_t smd = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&smd, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&smd, "s", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&smd, "d", (double)(-0.5));

    ap_linexpr1_t spd = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&spd, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&spd, "s", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&spd, "d", (double)(0.5));


    ap_lincons1_t cons_MDMR = ap_lincons1_make(AP_CONS_SUPEQ, &mdmr, NULL);
    ap_lincons1_t cons_RMD  = ap_lincons1_make(AP_CONS_SUPEQ, &rmd, NULL);
    ap_lincons1_t cons_RPD  = ap_lincons1_make(AP_CONS_SUPEQ, &rpd, NULL);

    ap_lincons1_array_t cons_elseif = ap_lincons1_array_make(env, 2);
    ap_lincons1_array_set(&cons_elseif, 0, &cons_RPD);
    ap_lincons1_array_set(&cons_elseif, 1, &cons_RMD);

    ap_lincons1_array_t cons_if = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_if, 0, &cons_MDMR);


    ap_abstract1_t abs1, abs2, abs3, abs4, abs5, abs6, abs7, abs8, abs9;
	abs1 = ap_abstract1_assign_linexpr(man, false, &abs, "s", &y, NULL);
	//ap_abstract1_fprint(stdout, man, &abs1);
	abs2 = ap_abstract1_assign_linexpr(man, false, &abs1, "r", &xms, NULL);
	//ap_abstract1_fprint(stdout, man, &abs2);
	abs3 = ap_abstract1_assign_linexpr(man, false, &abs2, "y", &x, NULL);
	//ap_abstract1_fprint(stdout, man, &abs3);
	//abs4 = ap_abstract1_meet_lincons_array(man, false, &abs3, &cons_if);
	//ap_abstract1_fprint(stdout, man, &abs4);
	abs5 = ap_abstract1_assign_linexpr(man, false, &abs3, "y", &smd, NULL);
	//ap_abstract1_fprint(stdout, man, &abs5);
	//abs6 = ap_abstract1_meet_lincons_array(man, false, &abs3, &cons_elseif);
	//ap_abstract1_fprint(stdout, man, &abs6);
	abs7 = ap_abstract1_assign_linexpr(man, false, &abs3, "y", &spd, NULL);
	//ap_abstract1_fprint(stdout, man, &abs7);
	abs8 = ap_abstract1_join(man, false, &abs7, &abs5);
	ap_abstract1_clear(man, &abs);
	abs = ap_abstract1_copy(man, &abs8);
	ap_abstract1_clear(man, &abs1);
	ap_abstract1_clear(man, &abs2);
	ap_abstract1_clear(man, &abs3);
	//ap_abstract1_clear(man, &abs4);
	ap_abstract1_clear(man, &abs5);
	//ap_abstract1_clear(man, &abs6);
	ap_abstract1_clear(man, &abs7);
	ap_abstract1_clear(man, &abs8);
    size_t i = 0;
    while (i<100) {
	abs1 = ap_abstract1_assign_linexpr(man, false, &abs, "s", &y, NULL);
	//ap_abstract1_fprint(stdout, man, &abs1);
	abs2 = ap_abstract1_assign_linexpr(man, false, &abs1, "r", &xms, NULL);
	//ap_abstract1_fprint(stdout, man, &abs2);
	abs3 = ap_abstract1_assign_linexpr(man, false, &abs2, "y", &x, NULL);
	//ap_abstract1_fprint(stdout, man, &abs3);
	//abs4 = ap_abstract1_meet_lincons_array(man, false, &abs3, &cons_if);
	//ap_abstract1_fprint(stdout, man, &abs4);
	abs5 = ap_abstract1_assign_linexpr(man, false, &abs3, "y", &smd, NULL);
	//ap_abstract1_fprint(stdout, man, &abs5);
	//abs6 = ap_abstract1_meet_lincons_array(man, false, &abs3, &cons_elseif);
	//ap_abstract1_fprint(stdout, man, &abs6);
	abs7 = ap_abstract1_assign_linexpr(man, false, &abs3, "y", &spd, NULL);
	//ap_abstract1_fprint(stdout, man, &abs7);
	abs8 = ap_abstract1_join(man, false, &abs7, &abs5);
	abs9 = ap_abstract1_join(man, false, &abs, &abs8);

	ap_abstract1_clear(man, &abs);
	abs = ap_abstract1_copy(man, &abs9);
	ap_abstract1_clear(man, &abs1);
	ap_abstract1_clear(man, &abs2);
	ap_abstract1_clear(man, &abs3);
	//ap_abstract1_clear(man, &abs4);
	ap_abstract1_clear(man, &abs5);
	//ap_abstract1_clear(man, &abs6);
	ap_abstract1_clear(man, &abs7);
	ap_abstract1_clear(man, &abs8);
	ap_abstract1_clear(man, &abs9);
	i++;
    }

    ap_abstract1_fprint(stdout, man, &abs);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abs);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer le mémoire utilisée */
    ap_abstract1_clear(man, &abs);

    ap_interval_array_free(array, 5);

    /* libérer les manager */
    ap_manager_free(man);
    ap_manager_free(manNS);
    end = clock();
    time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("CpuTime: %.2f\n",time);
}
