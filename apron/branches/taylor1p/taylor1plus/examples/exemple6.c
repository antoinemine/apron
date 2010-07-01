#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/***************************************
  x = [-oo,+oo]
  y = [-oo,+oo]
  if (x >= 1)
    abs
  **************************************/


int main(void) {
    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit, ici les octogons */
    //ap_manager_t* manNS = oct_manager_alloc();
    /* true means strict constraints are supported */
    //ap_manager_t* manNS = pk_manager_alloc(true);
    ap_manager_t* manNS = box_manager_alloc();
    /* Le domaine abstrait T1+ est param�tr� par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables r��lles */
    ap_var_t name_of_dim[3] = {"x", "y", "z"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 3);

    /* Cr�er un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(3);
    //array[0] = t1p_create_pos(man);
    ap_interval_set_top(array[0]);
    ap_interval_set_top(array[1]);
    ap_interval_set_double(array[2], 0.0, 0.0);	/* array[2] = [-1,-1] */


    /* Abstraire le tableau array en un objet abstrait T1+ */
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);

    ap_interval_t *coeff = ap_interval_alloc();
    //ap_interval_set_int(coeff, 2, 2);
    //ap_abstract1_aff_build(man, &abstract1, "y", 0, coeff, false);

    /* Cr�er un tableau d'intervalles */
    ap_interval_t** box1 = ap_interval_array_alloc(2);
    ap_interval_set_double(box1[0], -0.5, 0.0);		/* box1[0] = [-1.0, 0.0] */
    ap_interval_set_double(box1[1], 0.0, 0.5);		/* box1[1] = [ 0.0, 0.5] */

    /* La commande _ns_meet_box_array permet de rajouter des contraintes intervalles *sur les epsilons*.
     * Une intersection syst�mtique est faite avec l'hypercube unit�.
     */
//    ap_abstract1_ns_meet_box_array(man, &abstract1, box1, 2);

    /* affiche l'objet abstrait, pour le moment n'affiche pas les contraintes sur les symboles */
    ap_abstract1_fprint(stdout, man, &abstract1);
    ap_box1_t gamma1 = ap_abstract1_to_box(man, &abstract1);
    ap_box1_fprint(stdout, &gamma1);
    ap_box1_clear(&gamma1);


    ap_linexpr1_t linexp = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp, (double)-1.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp, "x", (double)(1.0));
    ap_lincons1_array_t cons_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp, NULL);
    ap_lincons1_array_set(&cons_array, 0, &cons1);

    ap_abstract1_t abstract2 = ap_abstract1_meet_lincons_array(man, false, &abstract1, &cons_array);

    ap_abstract1_fprint(stdout, man, &abstract2);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abstract2);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* lib�rer le m�moire utilis�e */
    ap_interval_array_free(box1, 2);
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 3);

    /* lib�rer les manager */
    ap_manager_free(man);
}
