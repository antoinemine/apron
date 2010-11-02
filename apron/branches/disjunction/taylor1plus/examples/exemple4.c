#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"

int main(void) {

    ap_manager_t* manNS = box_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    /* Construite un environnement de trois variables réélles */
    ap_var_t name_of_dim[3] = {"x", "y", "z"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 3);

    /* Créer un tableau d'intervalles de dimension 2 */
    ap_interval_t** array = ap_interval_array_alloc(3);
    ap_interval_set_double(array[0], 10.0, 10.0);
    ap_interval_set_double(array[1], 10.0, 10.0);
    ap_interval_set_double(array[2], 0.0, 0.0);


    /* Abstraire le tableau array en un objet abstrait T1+ */
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);

    ap_interval_t *coeff = ap_interval_alloc();
    ap_interval_set_int(coeff, 5, 5);
    ap_abstract1_aff_build(man, &abstract1, "x", 0, coeff, false);
    ap_interval_set_int(coeff, 3, 3);
    ap_abstract1_aff_build(man, &abstract1, "x", 1, coeff, false);

    ap_interval_set_int(coeff, -2, -2);
    ap_abstract1_aff_build(man, &abstract1, "y", 0, coeff, false);
    ap_interval_set_int(coeff, 1, 1);
    ap_abstract1_aff_build(man, &abstract1, "y", 2, coeff, false);

    /* Créer un tableau d'intervalles */
    ap_interval_t** box1 = ap_interval_array_alloc(3);
    ap_interval_set_double(box1[0], -1.0, 1.0);		/* box1[0] = [-1.0, 0.0] */
    ap_interval_set_double(box1[1], -1.0, 1.0);		/* box1[1] = [ 0.0, 0.5] */
    ap_interval_set_double(box1[2], -1.0, 1.0);		/* box1[1] = [ 0.0, 0.5] */
    ap_abstract1_ns_meet_box_array(man, &abstract1, box1, 3);

    /* affiche l'objet abstrait, pour le moment n'affiche pas les contraintes sur les symboles */
    ap_abstract1_fprint(stdout, man, &abstract1);
    ap_box1_t gamma1 = ap_abstract1_to_box(man, &abstract1);
    ap_box1_fprint(stdout, &gamma1);
    ap_box1_clear(&gamma1);

    ap_texpr1_t* z = ap_texpr1_binop(AP_TEXPR_MUL,
	    ap_texpr1_var(env, "x"),
	    ap_texpr1_var(env, "y"),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);

    //ap_texpr1_fprint(stdout, z);

    ap_abstract1_t abstract2 = ap_abstract1_assign_texpr(man, false, &abstract1, "z", z, NULL);

    ap_abstract1_fprint(stdout, man, &abstract2);
    ap_box1_t gamma = ap_abstract1_to_box(man, &abstract2);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer le mémoire utilisée */
    ap_interval_array_free(box1, 2);
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);

    ap_texpr1_free(z);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 3);

    /* libérer les manager */
    ap_manager_free(man);
}
