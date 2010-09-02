#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/******************************
  int x, y;
  if ( x >= -1 and x <= 1 ) then
        y := x - 1;
	else
	      y := x;
	      endif;
	      if( x == 0 ) then
	        assert(y != 0);
	      endif;

  *****************************/

int main(void) {

    /* choisir le domaine pour les symboles de bruit.
       - box_manager_alloc() pour les intervalles
       - oct_manager_alloc() pour les octogones
       - pk_manager_alloc(true) pour les polyhedres version newpolka, (true means strict constraints are supported)
       - ap_ppl_poly_manager_alloc(true) pour les polyhedre version ppl
     */
    FILE* file = fopen("interval_polyhedra2_debug.txt", "w");
    ap_manager_t* manNS = box_manager_alloc();
    /* Vous pouvez choisir l'un des domaine précédent pour avoir les résus dans les zones/poly ... */
    ap_manager_t* man = t1p_manager_alloc(manNS);
    //ap_manager_t* man = box_manager_alloc();
    //ap_manager_t* man = t1p_manager_alloc();
    /* par exemple pour les octogones */
    //ap_manager_t* man = pk_manager_alloc(true);

    /* Construite un environnement de 2 variables réélles */
    ap_var_t name_of_dim[2] = {"x", "y"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 2);

    /* Construite l'equivalent du x = DBETWEEN(0,180) et y = top */
    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], (double)(INT_MIN), (double)(INT_MAX));
    ap_interval_set_double(array[1], (double)(INT_MIN), (double)(INT_MAX));
//    ap_interval_set_double(array[0], (double)(-150.0), (double)(100.0));
//    ap_interval_set_double(array[1], (double)(-150.0), (double)(100.0));
    //ap_interval_set_top(array[0]);
    //ap_interval_set_top(array[1]);

    /* abstraire le tableau d'intervalles array en un objet T1+ */
    ap_abstract1_t abs = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
    /* pour afficher l'objet abstrait */
    ap_abstract1_fprint(file, man, &abs);

    /* construire la contrainte (x \leq 1) and (x \geq -1) */
    ap_linexpr1_t linexp1 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp1, (double)1.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp1, "x", (double)(1.0));
    ap_linexpr1_t linexp1_ = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp1_, (double)1.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp1_, "x", (double)(-1.0));
    ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp1, NULL);
    ap_lincons1_t cons1_ = ap_lincons1_make(AP_CONS_SUPEQ, &linexp1_, NULL);
    ap_lincons1_array_t cons1_array = ap_lincons1_array_make(env, 2);
    ap_lincons1_array_set(&cons1_array, 0, &cons1);
    ap_lincons1_array_set(&cons1_array, 1, &cons1_);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(file, &cons1_array);

    /* construire la contrainte (x \geq -1) */
    ap_linexpr1_t linexp2 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp2, (double)-1.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp2, "x", (double)(-1.0));
    ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp2, NULL);
    ap_lincons1_array_t cons2_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons2_array, 0, &cons2);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(file, &cons2_array);

    /* construire la contrainte (x \geq 1) */
    ap_linexpr1_t linexp3 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp3, (double)-1.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp3, "x", (double)(1.0));
    ap_lincons1_t cons3 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp3, NULL);
    ap_lincons1_array_t cons3_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons3_array, 0, &cons3);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(file, &cons3_array);

    /* construire la contrainte (x == 0) */
    ap_linexpr1_t linexp4 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp4, (double)0.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp4, "x", (double)(1.0));
    ap_lincons1_t cons4 = ap_lincons1_make(AP_CONS_EQ, &linexp4, NULL);
    ap_lincons1_array_t cons4_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons4_array, 0, &cons4);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(file, &cons4_array);

    ap_box1_t gamma;
    /******** IF *********/
    ap_abstract1_t abs_if = ap_abstract1_meet_lincons_array(man, false, &abs, &cons1_array);
    fprintf(file,"###### if (x <= 1 and x>= -1) ######\n");
    ap_abstract1_fprint(file, man, &abs_if);
    ap_abstract1_t abs_if_y = ap_abstract1_assign_linexpr(man, false, &abs_if, "y", &linexp3, NULL);
    fprintf(file,"###### if (x <= 1 and x>= -1) y = x - 1 ######\n");
    ap_abstract1_fprint(file, man, &abs_if_y);

    /*********** ELSE ************/
    ap_abstract1_t abs_else1 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons2_array);
    fprintf(file,"###### if (x <= -1) ######\n");
    ap_abstract1_fprint(file, man, &abs_else1);
    ap_abstract1_t abs_else_y1 = ap_abstract1_assign_linexpr(man, false, &abs_else1, "y", &linexp4, NULL);
    fprintf(file,"###### if (x <= -1) y = x ######\n");
    ap_abstract1_fprint(file, man, &abs_else_y1);

    ap_abstract1_t abs_else2 = ap_abstract1_meet_lincons_array(man, false, &abs, &cons3_array);
    fprintf(file,"###### if (x >= 1) ######\n");
    ap_abstract1_fprint(file, man, &abs_else2);
    ap_abstract1_t abs_else_y2 = ap_abstract1_assign_linexpr(man, false, &abs_else2, "y", &linexp4, NULL);
    fprintf(file,"###### if (x >= 1) y = x ######\n");
    ap_abstract1_fprint(file, man, &abs_else_y2);
    
    ap_abstract1_t abs_else_y = ap_abstract1_join(man, false, &abs_else_y1, &abs_else_y2);
    fprintf(file,"###### if (x >= 1) y = x JOIN if (x <= -1) y = x  ######\n");
    ap_abstract1_fprint(file, man, &abs_else_y);


    /********* JOIN **************/
    ap_abstract1_fprint(stdout, man, &abs_if_y);
    ap_abstract1_fprint(stdout, man, &abs_else_y);
    ap_abstract1_t abs1 = ap_abstract1_join(man, false, &abs_if_y, &abs_else_y);
    fprintf(file,"###### if (x <= 1 and x >= -1) y = x-1 JOIN else  y = x  ######\n");
    ap_abstract1_fprint(file, man, &abs1);
    ap_abstract1_fprint(stdout, man, &abs1);

    ap_abstract1_t abs2 = ap_abstract1_meet_lincons_array(man, false, &abs1, &cons4_array);
    /* pour afficher le resultat */
    fprintf(file,"###### assert y != 0 #####\n");
    ap_abstract1_fprint(file, man, &abs2);
    ap_abstract1_fprint(stdout, man, &abs2);
    /* pour afficher la concretisation intervalle de abs_res */
    gamma = ap_abstract1_to_box(man, &abs2);
    ap_box1_fprint(file, &gamma);
    ap_box1_clear(&gamma);

    /* libérer la mémoire utilisée*/
    ap_abstract1_clear(man, &abs);
    ap_abstract1_clear(man, &abs1);
    ap_abstract1_clear(man, &abs_if);
    ap_abstract1_clear(man, &abs_if_y);
    ap_abstract1_clear(man, &abs_else1);
    ap_abstract1_clear(man, &abs_else2);
    ap_abstract1_clear(man, &abs_else_y);
    ap_abstract1_clear(man, &abs_else_y1);
    ap_abstract1_clear(man, &abs_else_y2);

    ap_lincons1_array_clear(&cons1_array);
    ap_lincons1_array_clear(&cons2_array);
    ap_lincons1_array_clear(&cons3_array);
    ap_lincons1_array_clear(&cons4_array);

    ap_interval_array_free(array, 2);
    ap_manager_free(man);
    ap_manager_free(manNS);
    fclose(file);
}

