#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "box.h"

int main(void) {

    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit, ici les octogons */
    ap_manager_t* manNS = box_manager_alloc();
    /* Le domaine abstrait T1+ est paramétré par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[2] = {"x", "y"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],2);

    /* Créer un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], 1.0, 1.0);	/* array[0] = [1,1] */
    ap_interval_set_double(array[1], 0.0, 0.0);	/* array[1] = [0,0] */

    /* Abstraire le tableau array en un objet abstrait T1+ */
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
    /* On obtient donc :
     * _________________ 
     * |x := 1		
     * |y := 0
     * |*** eps ***
     * |bottom
     * -----------------
     */

    ap_interval_t *coeff = ap_interval_alloc();
    ap_interval_set_int(coeff, -1, -1);
    /* la fonction _aff_build permet de construire des formes affines, en précisant:
     * - le manager
     * - l'objet abstrait T1+
     * - la variable concernée
     * - l'indice du symbole de bruit (s'il n'existe pas encore, le créer)
     * - la deviation partielle relative au symbole à rajouter
     * - si True, le nouveau symbole est un symbole union
     */
    ap_abstract1_aff_build(man, &abstract1, "x", 0, coeff, false);
    /* On a donc pour l'objet abstract1 :
     * ________________________
     * |x := 1 + [-1,-1]eps_0	
     * |y := 0
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * ------------------------
     */
    ap_interval_set_int(coeff, 2, 2);
    ap_abstract1_aff_build(man, &abstract1, "x", 1, coeff, false);
    /* On obtient :
     * _____________________________________
     * |x := 1 + [-1,-1]eps_0 + [2,2]eps_1
     * |y := 0
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * |eps_1 <= 1
     * |eps_1 >= -1
     * --------------------------------------
     */
    ap_interval_set_int(coeff, 1, 1);
    ap_abstract1_aff_build(man, &abstract1, "y", 0, coeff, false);
    ap_interval_set_int(coeff, -1, -1);
    ap_abstract1_aff_build(man, &abstract1, "y", 1, coeff, false);

    /* Créer un tableau d'intervalles */
    ap_interval_t** box1 = ap_interval_array_alloc(2);
    ap_interval_set_double(box1[0], -1.0, 0.0);		/* box1[0] = [-1.0, 0.0] */
    ap_interval_set_double(box1[1], 0.0, 0.5);		/* box1[1] = [ 0.0, 0.5] */

    /* La commande _ns_meet_box_array permet de rajouter des contraintes intervalles *sur les epsilons*.
     * Une intersection systémtique est faite avec l'hypercube unité.
     * A ne pas confondre avec ap_abstract1_of_box();
     * qui elle créer un objet abstrait T1+ à partir d'un tableau d'intervalles.
     * TODO: le nom est a changer pour toute ambiguïté.
     */
    ap_abstract1_ns_meet_box_array(man, &abstract1, box1, 2);
    /* On obtient :
     * _____________________________________
     * |x := 1 + [-1,-1]eps_0 + [2,2]eps_1
     * |y := 0 + [1,1]eps_0 + [-1,-1]eps_1
     * |*** eps ***
     * |eps_0 <= 0
     * |eps_0 >= -1
     * |eps_1 <= 0.5
     * |eps_1 >= 0
     * --------------------------------------
     */

    /* affiche l'objet abstrait, pour le moment n'affiche pas les contraintes sur les symboles */
    ap_abstract1_fprint(stdout, man, &abstract1);

    ap_interval_set_double(array[0], 2.0, 2.0);		/* array[0] = [2,2] */
    ap_abstract1_t abstract2 = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
    /*
     * abstract 2 :
     * _________________
     * |x := 2
     * |y := 0
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
     * |y := 0
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
     * |y := 0
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * |eps_1 <= 1
     * |eps_1 >= -1
     * ---------------------------------------
     */
    ap_interval_set_int(coeff, 1, 1);
    ap_abstract1_aff_build(man, &abstract2, "y", 0, coeff, false);
    ap_interval_set_int(coeff, -1, -1);
    ap_abstract1_aff_build(man, &abstract2, "y", 1, coeff, false);

    ap_interval_t** box2 = ap_interval_array_alloc(2);
    ap_interval_set_double(box2[0], -0.5, 0.5);
    ap_interval_set_double(box2[1], 0.0, 1.0);
    ap_abstract1_ns_meet_box_array(man, &abstract2, box2, 2);
    /*
     * abstract 2 :
     * ______________________________________
     * |x := 2 + [1,1]eps_0 + [1,1]eps_1
     * |y := 0 + [1,1]eps_0 + [-1,-1]eps_1
     * |*** eps ***
     * |eps_0 <= 0.5
     * |eps_0 >= -0.5
     * |eps_1 <= 1
     * |eps_1 >= 0
     * ---------------------------------------
     */

    ap_abstract1_fprint(stdout, man, &abstract2);

    /* Calculer le mub de abstract1 et de abstract2 
     * le "false" pour dire que le calcul n'ecrase pas abstract1.
     */
    ap_abstract1_t abstract3 = ap_abstract1_join(man, false, &abstract1, &abstract2);
    ap_abstract1_fprint(stdout, man, &abstract3);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abstract3);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer le mémoire utilisée */
    ap_interval_array_free(box1, 2);
    ap_interval_array_free(box2, 2);
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);
    ap_abstract1_clear(man, &abstract3);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 2);

    /* libérer les manager */
    ap_manager_free(man);
    ap_manager_free(manNS);
}
