#include <time.h>
#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/********************************
 var wb : int, count : int, len: int, C : int, res : real;
 begin

 assume C >= 0 and C <= 10;

 if (wb >= 2*count) then
  if (count + C >= len) then
     res = wb - 2*(len - C);
      endif;
      endif;
      end
********************************/

int main(void) {
    clock_t start, end;
    double time;
    double cpu_time_used;
    start = clock();
    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit, ici les octogons */
    //ap_manager_t* manNS = oct_manager_alloc();
    /* true means strict constraints are supported */
    ap_manager_t* manNS = pk_manager_alloc(true);
    //ap_manager_t* manNS = box_manager_alloc();
    /* Le domaine abstrait T1+ est paramétré par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);
    //ap_manager_t* man = ap_ppl_poly_manager_alloc(true);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[5] = {"C","len","wb","count","res"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 5);

    /* Créer un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(5);
    ap_interval_set_double(array[0], 0.0, 10.0);
    ap_interval_set_double(array[1], 0.0, INT_MAX);
    ap_interval_set_double(array[2], 0.0, INT_MAX);
    ap_interval_set_double(array[3], 0.0, INT_MAX);
    ap_interval_set_double(array[4], 0.0, INT_MAX);


    ap_abstract1_t abs = ap_abstract1_of_box(man, env, name_of_dim, array, 5);
    ap_abstract1_fprint(stdout, man, &abs);

    ap_linexpr1_t wbm2count = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&wbm2count, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&wbm2count, "wb", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&wbm2count, "count", (double)(-2.0));

    ap_linexpr1_t countpcml = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)3);
    ap_linexpr1_set_cst_scalar_double(&countpcml, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&countpcml, "count", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&countpcml, "C", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&countpcml, "len", (double)(-1.0));

    ap_linexpr1_t res = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)3);
    ap_linexpr1_set_cst_scalar_double(&res, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&res, "wb", (double)(1.0));
    ap_linexpr1_set_coeff_scalar_double(&res, "C", (double)(2.0));
    ap_linexpr1_set_coeff_scalar_double(&res, "len", (double)(-2.0));

    ap_lincons1_t cons_countpcml = ap_lincons1_make(AP_CONS_SUPEQ, &countpcml, NULL);
    ap_lincons1_t cons_wbm2count  = ap_lincons1_make(AP_CONS_SUPEQ, &wbm2count, NULL);

    ap_lincons1_array_t cons_if1 = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_if1, 0, &cons_wbm2count);

    ap_lincons1_array_t cons_if2 = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons_if2, 0, &cons_countpcml);

    ap_lincons1_array_t cons_if = ap_lincons1_array_make(env, 2);
    ap_lincons1_array_set(&cons_if, 0, &cons_countpcml);
    ap_lincons1_array_set(&cons_if, 1, &cons_wbm2count);

    ap_abstract1_t abs1, abs2, abs3, abs4, abs5, abs6, abs7, abs8, abs9;
    abs1 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons_if1);
    abs2 = ap_abstract1_meet_lincons_array(man, false, &abs1, &cons_if2);
    abs3 = ap_abstract1_assign_linexpr(man, false, &abs2, "res", &res, NULL);

    abs4 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons_if);
    abs5 = ap_abstract1_assign_linexpr(man, false, &abs4, "res", &res, NULL);

    ap_abstract1_fprint(stdout, man, &abs3);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abs3);
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
