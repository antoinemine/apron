#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"


int main(void) {

    ap_manager_t* manNS = oct_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    ap_var_t name_of_dim[3] = {"x","y","z"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],3);

    ap_interval_t** array = ap_interval_array_alloc(3);
    ap_interval_set_double(array[0], -1.0, 1.0);
    ap_interval_set_double(array[1], 0.0, 1.0);
    ap_interval_set_bottom(array[2]);

    ap_abstract1_t abstract = ap_abstract1_of_box(man, env, name_of_dim, array, 3);

    ap_abstract1_fprint(stdout, man, &abstract);

    /* x + y */
    ap_linexpr1_t linexp = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)2);
    ap_linexpr1_set_cst_scalar_double(&linexp, 0.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp, "x", 1.);
    ap_linexpr1_set_coeff_scalar_double(&linexp, "y", 1.);

    /* z = x + y */
    ap_abstract1_t abstract1 = ap_abstract1_assign_linexpr(man, false, &abstract, "z", &linexp, NULL);

    ap_abstract1_fprint(stdout, man, &abstract1);

    ap_linexpr1_clear(&linexp);
    ap_abstract1_clear(man, &abstract1);
    ap_abstract1_clear(man, &abstract);

    ap_interval_array_free(array, 3);

    ap_manager_free(man);
    ap_manager_free(manNS);
}
