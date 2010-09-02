#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"


int main(void) {

    ap_manager_t* manNS = oct_manager_alloc();
    ap_manager_t* man = t1p_manager_alloc(manNS);

    ap_var_t name_of_dim[2] = {"x","y"};
    ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],2);

    /* commencer par creéer les deux centres des formes [a,a] (ici, 1 et 0) */
    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], 1.0, 1.0);
    ap_interval_set_double(array[1], 0.0, 0.0);
    
    ap_abstract1_t abstract = ap_abstract1_of_box(man, env, name_of_dim, array, 2); 
    // A ce stade on a : x = 1 et y = 0, pour verifier :
    ap_abstract1_fprint(stdout, man, &abstract);

    /* créer chaque coeff */
    ap_interval_t* coeffu = ap_interval_alloc();
    ap_interval_set_double(coeffu, 3.0, 3.0);

    /* construire les formes affines, en indiquant : 
       - le manager
       - la valeur abstraite
       - la variable concernee
       - l'indice souhaite
       - le coeff du symbol
       - true si on veut un symbol union, false sinon
     */
    ap_abstract1_aff_build(man, &abstract, "x",  4, coeffu, false);

    /* afficher pour verifier */
    printf("\n");
    ap_abstract1_fprint(stdout, man, &abstract);

    /* un nouveau coeff */
    ap_interval_t* coeffy = ap_interval_alloc();
    ap_interval_set_double(coeffy, 2.0, 2.0);
    ap_abstract1_aff_build(man, &abstract, "y",  3, coeffy, false);
    /* afficher pour verifier */
    printf("\n");
    ap_abstract1_fprint(stdout, man, &abstract);

    /* partager eps 2 entre x et y avec le meme coeff par exemple */
    ap_abstract1_aff_build(man, &abstract, "x",  2, coeffy, false);
    /* afficher pour verifier */
    printf("\n");
    ap_abstract1_fprint(stdout, man, &abstract);

    printf("\n");
    ap_interval_free(coeffu);
    ap_interval_free(coeffy);
    ap_abstract1_clear(man, &abstract);

    ap_interval_array_free(array, 2);

    ap_manager_free(man);
    ap_manager_free(manNS);
}
