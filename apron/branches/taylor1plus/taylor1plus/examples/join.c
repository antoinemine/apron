#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"


int main(void) {

    ap_manager_t* manNS = oct_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    ap_var_t name_of_dim[3] = {"x","y","z"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],3);

    ap_interval_t** array = ap_interval_array_alloc(3);
    ap_interval_set_double(array[0], 1.0, 1.0);
    ap_interval_set_top(array[1]);
    ap_interval_set_bottom(array[2]);
    ap_abstract1_t abstract1 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);
    ap_interval_t *coeff = ap_interval_alloc();
    ap_interval_set_int(coeff, -1, -1);
    ap_abstract1_aff_build(man, &abstract1, "x", 0, coeff, false);
    ap_interval_set_int(coeff, 2, 2);
    ap_abstract1_aff_build(man, &abstract1, "x", 1, coeff, false);

    ap_abstract1_fprint(stdout, man, &abstract1);

    ap_interval_set_double(array[0], 2.0, 2.0);
    ap_abstract1_t abstract2 = ap_abstract1_of_box(man, env, name_of_dim, array, 3);
    ap_interval_set_int(coeff, 1, 1);
    ap_abstract1_aff_build(man, &abstract2, "x", 0, coeff, false);
    ap_abstract1_aff_build(man, &abstract2, "x", 1, coeff, false);

    ap_abstract1_fprint(stdout, man, &abstract2);

    ap_abstract1_t abstract3 = ap_abstract1_join(man, false, &abstract1, &abstract2);

    ap_abstract1_fprint(stdout, man, &abstract3);

    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract2);
    ap_abstract1_clear(man, &abstract3);

    ap_interval_free(coeff);
    ap_interval_array_free(array, 3);

    ap_manager_free(man);
    ap_manager_free(manNS);
}
