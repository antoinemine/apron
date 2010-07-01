#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"


int main(void) {

    ap_manager_t* manNS = oct_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    ap_var_t name_of_dim[2] = {"x","y"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],2);

    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], -1.0, 1.0);
    ap_interval_set_double(array[1], 0.0, 1.0);

    ap_abstract1_t abstract = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
    ap_abstract1_fprint(stdout, man, &abstract);

    ap_abstract1_clear(man, &abstract);

    ap_interval_array_free(array, 2);

    ap_manager_free(man);
    ap_manager_free(manNS);
}
