#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"


int main(void) {

    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit */
    ap_manager_t* manNS = oct_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[3] = {"x","y","z"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],3);

    /* Créer un tableau d'intervalles de dimension 3 */
    ap_interval_t** array = ap_interval_array_alloc(3);
    ap_interval_set_double(array[0], 1.0, 1.0);		/* array[0] = [1,1] */
    ap_interval_set_top(array[1]);			/* array[1] = T */
    ap_interval_set_bottom(array[2]);			/* array[2] = bottom */

    /* Abstraire le tableau array en un objet abstrait T1+ */
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);
     /* _________________ 
     * |x := 1          
     * |y := [-oo, +oo]
     * |z := [1, -1]
     * |*** eps ***
     * |bottom
     * -----------------
     */

    ap_interval_t *coeff = ap_interval_alloc();
    ap_interval_set_int(coeff, -1, -1);
    ap_abstract1_aff_build(man, &abstract1, "x", 0, coeff, false);
    ap_interval_set_int(coeff, 2, 2);
    ap_abstract1_aff_build(man, &abstract1, "x", 1, coeff, false);

    /* Créer un tableau de contraintes linéaires */
    ap_lincons0_array_t array_cons1 = ap_lincons0_array_make(4);	// cleared when freeing the abstract object
    /* Construire l'expression linéaire : eps0 + 1 
     * l'expression n'est pas dense et contient un seul monome
     * sa constante est un double : 1.0
     * le coeff du seul monome est 1.0
     * l'indice du monome est 0
     */
    ap_linexpr0_t* linexp1 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_cst_scalar_double(linexp1, 1.0);
    ap_linexpr0_set_coeff_scalar_double(linexp1, (ap_dim_t)0, (double)1.0);	/* dim = 0 means eps0 */

    /* Construire l'expression linéaire : -eps0 */
    ap_linexpr0_t* linexp2 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_coeff_scalar_double(linexp2, (ap_dim_t)0, (double)-1.0);
    /* -eps1 + 0.5 */
    ap_linexpr0_t* linexp3 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_cst_scalar_double(linexp3, 0.5);
    ap_linexpr0_set_coeff_scalar_double(linexp3, (ap_dim_t)1, (double)-1.0);	/* dim = 1 means eps1 */
    /* eps1 */
    ap_linexpr0_t* linexp4 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_coeff_scalar_double(linexp4, (ap_dim_t)1, (double)1.0);

    /* Construire la contrainte eps0 + 1 >= 0
     * Les contraintes sont toujours par rapport à 0 (toujours à droite)
     */
    array_cons1.p[0] = ap_lincons0_make(AP_CONS_SUPEQ, linexp1, NULL);
    /* -eps >= 0*/
    array_cons1.p[1] = ap_lincons0_make(AP_CONS_SUPEQ, linexp2, NULL);
    /* -eps1 + 0.5 >= 0 */
    array_cons1.p[2] = ap_lincons0_make(AP_CONS_SUPEQ, linexp3, NULL);
    /* eps1 >= 0 */
    array_cons1.p[3] = ap_lincons0_make(AP_CONS_SUPEQ, linexp4, NULL);

    /* La commande _set_lincons permet de rajouter un tableau de contraintes linéaires *sur les epsilons* */
    ap_abstract1_ns_meet_lincons_array(man, &abstract1, &array_cons1);
    /*
     *_________________________________
     * |x := 1 + -1.[0] + 2.[1]
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |*** eps ****
     * |0: x0 + 1 >= 0
     * |1: -x0 >= 0
     * |2: -x1 + 0.5 >= 0
     * |3: x1 >= 0
     *---------------------------------
     */

    ap_abstract1_fprint(stdout, man, &abstract1);

    ap_interval_set_double(array[0], 2.0, 2.0);
    ap_abstract1_t abstract2 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);
    /*
     * abstract 2 :
     * _________________
     * |x := 2
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |*** eps ***
     * |bottom
     * -----------------
     */
    ap_interval_set_int(coeff, 1, 1);
    ap_abstract1_aff_build(man, &abstract2, "x", 0, coeff, false);
    /*
     * abstract 2 :
     * ______________________
     * |x := 2 + [1,1]eps_0
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * ----------------------
     */

    ap_abstract1_aff_build(man, &abstract2, "x", 1, coeff, false);
    /*
     * abstract 2 :
     * ______________________________________
     * |x := 2 + [1,1]eps_0 + [1,1]eps_1
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * |eps_1 <= 1
     * |eps_1 >= -1
     * ---------------------------------------
     */

    ap_lincons0_array_t array_cons2 = ap_lincons0_array_make(4);	/* cleared when freeing the abstract object */
    /* eps0 + 0.5 */
    ap_linexpr0_t* linexp12 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)0.5);
    ap_linexpr0_set_cst_scalar_double(linexp12, 1.0);
    ap_linexpr0_set_coeff_scalar_double(linexp12, (ap_dim_t)0, (double)1.0);	/* dim = 0 means eps0 */
    /* -eps0 + 0.5 */
    ap_linexpr0_t* linexp22 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_cst_scalar_double(linexp22, 0.5);
    ap_linexpr0_set_coeff_scalar_double(linexp22, (ap_dim_t)0, (double)-1.0);
    /* -eps1 + 1 */
    ap_linexpr0_t* linexp32 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_cst_scalar_double(linexp32, 1.0);
    ap_linexpr0_set_coeff_scalar_double(linexp32, (ap_dim_t)1, (double)-1.0);	/* dim = 1 means eps1 */
    /* eps1 */
    ap_linexpr0_t* linexp42 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_coeff_scalar_double(linexp42, (ap_dim_t)1, (double)1.0);

    /* 0 <= eps0 + 0.5 */
    array_cons2.p[0] = ap_lincons0_make(AP_CONS_SUPEQ, linexp12, NULL);
    /* 0 <= -eps0 + 0.5 */
    array_cons2.p[1] = ap_lincons0_make(AP_CONS_SUPEQ, linexp22, NULL);
    /* 0 <= -eps1 + 1 */
    array_cons2.p[2] = ap_lincons0_make(AP_CONS_SUPEQ, linexp32, NULL);
    /* 0 <= eps1 */
    array_cons2.p[3] = ap_lincons0_make(AP_CONS_SUPEQ, linexp42, NULL);
    ap_abstract1_ns_meet_lincons_array(man, &abstract2, &array_cons2);

    ap_abstract1_fprint(stdout, man, &abstract2);

    /* Calculer le mub de abstract1 et de abstract2 
     * le "false" pour dire que le calcul n'ecrase pas abstract1.
     */
    ap_abstract1_t abstract3 = ap_abstract1_join(man, false, &abstract1, &abstract2);
    /*
     *__________________________________
     *|x := 1.75 + 1.[1] + [u]0.75.[2]
     *|y := [-oo,+oo]
     *|z := [1,-1]
     *|array of constraints of size 8
     *|0: x0 + 1 >= 0
     *|1: -x0 + 0.5 >= 0
     *|2: -x0 + x1 + 0.5 >= 0
     *|3: x0 + x1 + 1 >= 0
     *|4: x1 >= 0
     *|5: -x0 - x1 + 1.5 >= 0
     *|6: x0 - x1 + 1.5 >= 0
     *|7: -x1 + 1 >= 0
     *-------------------------------
     */

    ap_abstract1_fprint(stdout, man, &abstract3);

    /* libérer la mémoire */
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);
    ap_abstract1_clear(man, &abstract3);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 3);

    ap_manager_free(man);
    ap_manager_free(manNS);
}
