#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"

int main(void) {

    /* Choisir un domaine abstrair dans APRON pour les symboles de bruit, ici les octogons */
    ap_manager_t* manNS = oct_manager_alloc();
    /* Le domaine abstrait T1+ est paramétré par manNS */
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[3] = {"x","y","z"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],3);

    /* Créer un tableau d'intervalles de dimension 3 */
    ap_interval_t** array = ap_interval_array_alloc(3);
    ap_interval_set_double(array[0], 1.0, 1.0);	/* array[0] = [1,1] */
    ap_interval_set_top(array[1]);		/* array[1] = T */
    ap_interval_set_bottom(array[2]);		/* array[2] = bottom */

    /* Abstraire le tableau array en un objet abstrait T1+ */
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);
    /* On obtient donc :
     * _________________ 
     * |x := 1		
     * |y := [-oo, +oo]
     * |z := [1, -1]
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
     * |y := [-oo, +oo]
     * |z := [1, -1]
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * ------------------------
     * Notez que si f est l'indice du dernier symbole utilisé dans l'analyse, 
     * et que l'indice souhaité est > f + 1, alors l'indice du nouveau symbole crée sera f + 1 
     * et non l'indice demandé, ainsi :
     * ap_abstract1_aff_build(man, &abstract1, "x", 25, coeff, false);
     * donne quand même :
     * _______________________
     * |x := 1 + [-1,-1]eps_0
     * |y := [-oo, +oo]
     * |z := [1, -1]
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * -----------------------
     */
    ap_interval_set_int(coeff, 2, 2);
    ap_abstract1_aff_build(man, &abstract1, "x", 1, coeff, false);
    /* On obtient :
     * _____________________________________
     * |x := 1 + [-1,-1]eps_0 + [2,2]eps_1
     * |y := [-oo, +oo]
     * |z := [1, -1]
     * |*** eps ***
     * |eps_0 <= 1
     * |eps_0 >= -1
     * |eps_1 <= 1
     * |eps_1 >= -1
     * --------------------------------------
     */

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
     * |y := [-oo, +oo]
     * |z := [1, -1]
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

    ap_interval_t** box2 = ap_interval_array_alloc(2);
    ap_interval_set_double(box2[0], -0.5, 0.5);
    ap_interval_set_double(box2[1], 0.0, 1.0);
    ap_abstract1_ns_meet_box_array(man, &abstract2, box2, 2);
    /*
     * abstract 2 :
     * ______________________________________
     * |x := 2 + [1,1]eps_0 + [1,1]eps_1
     * |y := [-oo,+oo]
     * |z := [1,-1]
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
    /* Remarquez que les contraintes sur les eps du mub sont le résultat de l'union dans le domaine abstrait des epsilons, ici les octogons.
     *_____________________________________
     * |x := 1.75 + 1.[1] + [u]0.75.[2]
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |array of constraints of size 8
     * |0: x0 + 1 >= 0
     * |1: -x0 + 0.5 >= 0
     * |2: -x0 + x1 + 0.5 >= 0
     * |3: x0 + x1 + 1 >= 0
     * |4: x1 >= 0
     * |5: -x0 - x1 + 1.5 >= 0
     * |6: x0 - x1 + 1.5 >= 0
     * |7: -x1 + 1 >= 0
     *-----------------------------------
     * en modifiant le domaine abstrait des eps avec celui des intervalles par exemple (box_manager_alloc), on trouve
     *_______________________________
     * |x := 1.75 + 1.[1] + [u]0.75.[2]
     * |y := [-oo,+oo]
     * |z := [1,-1]
     * |array of constraints of size 4
     * |0: x0 + 1 >= 0
     * |1: -x0 + 0.5 >= 0
     * |2: x1 >= 0
     * |3: -x1 + 1 >= 0
     *----------------------
     */

    ap_abstract1_fprint(stdout, man, &abstract3);

    /* libérer le mémoire utilisée */
    ap_interval_array_free(box1, 2);
    ap_interval_array_free(box2, 2);
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);
    ap_abstract1_clear(man, &abstract3);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 3);

    /* libérer les manager */
    ap_manager_free(man);
    ap_manager_free(manNS);
}
