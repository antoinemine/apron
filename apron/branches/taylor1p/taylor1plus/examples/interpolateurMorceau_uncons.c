#include <time.h>

#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/************************************
  void main() {
    float x, y;
      x = FBETWEEN(-1,1);
        if (x>=0)
	    y = 10*x;
	      else 
	          y = 20*x;
		  }
  *************************************/

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
    /* Le domaine abstrait T1+ est param�tr� par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables r��lles */
    ap_var_t name_of_dim[2] = {"x", "y"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 2);

    /* Cr�er un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], -1.0, 1.0);	/* array[0] = [1,1] */
    ap_interval_set_double(array[1], 0.0, 0.0);	/* array[1] = [2,2] */


    ap_abstract1_t abs = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
//    ap_abstract1_fprint(stdout, man, &abs);

    ap_linexpr1_t x = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&x, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&x, "x", (double)(1.0));

    ap_linexpr1_t ymun = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&ymun, (double)-1.0);
    ap_linexpr1_set_coeff_scalar_double(&ymun, "y", (double)(1.0));

    ap_linexpr1_t mx = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&mx, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&mx, "x", (double)(-1.0));

    ap_linexpr1_t dixx = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&dixx, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&dixx, "x", (double)(10.0));

    ap_linexpr1_t vinx = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&vinx, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&vinx, "x", (double)(20.0));


    ap_lincons1_t cons_xpos = ap_lincons1_make(AP_CONS_SUPEQ, &x, NULL);
    ap_lincons1_t cons_xneg = ap_lincons1_make(AP_CONS_SUPEQ, &mx, NULL);
    ap_lincons1_t cons_ymun = ap_lincons1_make(AP_CONS_EQ, &ymun, NULL);

    ap_lincons1_array_t cons_if = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_if, 0, &cons_xpos);

    ap_lincons1_array_t cons_else = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_else, 0, &cons_xneg);

    ap_lincons1_array_t cons_assert = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_assert, 0, &cons_ymun);


    ap_abstract1_t abs1, abs2, abs3, abs4, abs5, abs6;
    //abs1 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons_if);
    abs2 = ap_abstract1_assign_linexpr(man, false, &abs, "y", &dixx, NULL);
    //abs3 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons_else);
    abs4 = ap_abstract1_assign_linexpr(man, false, &abs, "y", &vinx, NULL);

    abs5 = ap_abstract1_join(man, false, &abs2, &abs4);

    //abs6 = ap_abstract1_meet_lincons_array(man, false, &abs5, &cons_assert);

    ap_abstract1_fprint(stdout, man, &abs5);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abs5);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* lib�rer le m�moire utilis�e */
    ap_abstract1_clear(man, &abs);
    ap_abstract1_clear(man, &abs1);
    ap_abstract1_clear(man, &abs2);
    ap_abstract1_clear(man, &abs3);
    ap_abstract1_clear(man, &abs4);
    ap_abstract1_clear(man, &abs5);
    ap_abstract1_clear(man, &abs6);

    ap_interval_array_free(array, 2);

    /* lib�rer les manager */
    ap_manager_free(man);
    ap_manager_free(manNS);
     end = clock();
         time = ((double) (end - start)) / CLOCKS_PER_SEC;
	     printf("CpuTime: %.2f\n",time);
}
