#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

int main(void) {

    /* choisir le domaine pour les symboles de bruit.
       - box_manager_alloc() pour les intervalles
       - oct_manager_alloc() pour les octogones
       - pk_manager_alloc(true) pour les polyhedres version newpolka, (true means strict constraints are supported)
       - ap_ppl_poly_manager_alloc(true) pour les polyhedre version ppl
     */
    ap_manager_t* manNS = ap_ppl_poly_manager_alloc(true);
    /* Vous pouvez choisir l'un des domaine précédent pour avoir les résus dans les zones/poly ... */
    ap_manager_t* man = t1p_manager_alloc(manNS);
    /* par exemple pour les octogones */
    //ap_manager_t* man = oct_manager_alloc();

    /* Construite un environnement de 2 variables réélles */
    ap_var_t name_of_dim[2] = {"x", "y"};
    ap_environment_t* env = ap_environment_alloc(NULL, 0, &name_of_dim[0], 2);

    /* Construite l'equivalent du x = DBETWEEN(0,180) et y = top */
    ap_interval_t** array = ap_interval_array_alloc(2);
    ap_interval_set_double(array[0], 0.0, 180.0);
    ap_interval_set_top(array[1]);

    /* abstraire le tableau d'intervalles array en un objet T1+ */
    ap_abstract1_t abs = ap_abstract1_of_box(man, env, name_of_dim, array, 2);
    /* pour afficher l'objet abstrait */
    ap_abstract1_fprint(stdout, man, &abs);

    /* construire la contrainte (x \leq 90) dans cons1_array[0] */
    ap_linexpr1_t linexp1 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp1, (double)90.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp1, "x", (double)(-1.0));
    ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp1, NULL);
    ap_lincons1_array_t cons1_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons1_array, 0, &cons1);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(stdout, &cons1_array);

    /* construire la contrainte (x \geq 90) dans cons2_array[0] */
    ap_linexpr1_t linexp2 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp2, (double)-90.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp2, "x", (double)(1.0));
    ap_lincons1_t cons2 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp2, NULL);
    ap_lincons1_array_t cons2_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons2_array, 0, &cons2);
    /* pour afficher la contrainte */
    //ap_lincons1_array_fprint(stdout, &cons2_array);

    /* construire l'expression arth y1 = 1 + x*x*(-0.00017644492 +x*0.000000588757)*/
    ap_texpr1_t* y1 = ap_texpr1_binop(AP_TEXPR_ADD,
	    ap_texpr1_cst_scalar_double(env, 1.0),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_var(env, "x"),
		ap_texpr1_binop(AP_TEXPR_MUL,
		    ap_texpr1_var(env, "x"),
		    ap_texpr1_binop(AP_TEXPR_ADD,
			ap_texpr1_cst_scalar_double(env, -0.00017644492),
			ap_texpr1_binop(AP_TEXPR_MUL,
			    ap_texpr1_var(env, "x"),
			    ap_texpr1_cst_scalar_double(env,0.000000588757),
			    AP_RTYPE_DOUBLE,
			    AP_RDIR_UP),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,
		    AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);
//		ap_texpr1_binop(AP_TEXPR_ADD,
//		    ap_texpr1_cst_scalar_double(env, -0.00017644492),
//		    ap_texpr1_binop(AP_TEXPR_MUL,
//			ap_texpr1_var(env, "x"),
//			ap_texpr1_cst_scalar_double(env,0.000000588757),
//			AP_RTYPE_DOUBLE,
//			AP_RDIR_UP),
//		    AP_RTYPE_DOUBLE,
//		    AP_RDIR_UP),
//		ap_texpr1_binop(AP_TEXPR_MUL,
//		    ap_texpr1_var(env, "x"),
//		    ap_texpr1_var(env, "x"),
//		    AP_RTYPE_DOUBLE,
//		    AP_RDIR_UP),
//		AP_RTYPE_DOUBLE,
//		AP_RDIR_UP),
//	    AP_RTYPE_DOUBLE,
//	    AP_RDIR_UP);
    /* pour afficher l'expression y1 */
    //ap_texpr1_fprint(stdout, y1);

    /* construire l'expression arth y2 = 1.283184584 +x*(-0.0062929908 + x*(-0.00014148386 +x*0.000000588757))*/
    ap_texpr1_t* y2 = ap_texpr1_binop(AP_TEXPR_ADD,
	    ap_texpr1_cst_scalar_double(env, 1.283184584),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_var(env, "x"),
		ap_texpr1_binop(AP_TEXPR_ADD,
		    ap_texpr1_cst_scalar_double(env, -0.0062929908),
		    ap_texpr1_binop(AP_TEXPR_MUL,
			ap_texpr1_var(env, "x"),
			ap_texpr1_binop(AP_TEXPR_ADD,
			    ap_texpr1_cst_scalar_double(env, -0.00014148386),
			    ap_texpr1_binop(AP_TEXPR_MUL,
				ap_texpr1_var(env, "x"),
				ap_texpr1_cst_scalar_double(env, 0.000000588757),
				AP_RTYPE_DOUBLE,
				AP_RDIR_UP),
			    AP_RTYPE_DOUBLE,
			    AP_RDIR_UP),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,
		    AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP);
    /* pour afficher l'expression y2 */
    //ap_texpr1_fprint(stdout, y2);

    /******** IF *********/
    /* appliquer le test x \leq 90 */
    ap_abstract1_t abs_if = ap_abstract1_meet_lincons_array(man, false, &abs, &cons1_array);
    /* evaluer [[y <- y1]] dans abs_if */
    ap_abstract1_t abs_if_y = ap_abstract1_assign_texpr(man, false, &abs_if, "y", y1, NULL);
    //ap_abstract1_t abs_if_y = ap_abstract1_assign_texpr(man, false, &abs, "y", y1, NULL);

    /*********** ELSE ************/
    /* appliquer le test x \geq 90 */
    ap_abstract1_t abs_else = ap_abstract1_meet_lincons_array(man, false, &abs, &cons2_array);
    /* evaluer [[y <- y2]] dans abs_else */
    ap_abstract1_t abs_else_y = ap_abstract1_assign_texpr(man, false, &abs_else, "y", y2, NULL);
    //ap_abstract1_t abs_else_y = ap_abstract1_assign_texpr(man, false, &abs, "y", y2, NULL);

    /********* JOIN **************/
    ap_abstract1_t abs_res = ap_abstract1_join(man, false, &abs_if_y, &abs_else_y);
    /* pour afficher le resultat */
    ap_abstract1_fprint(stdout, man, &abs_res);
    /* pour afficher la concretisation intervalle de abs_res */
    ap_box1_t gamma = ap_abstract1_to_box(man, &abs_res);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer la mémoire utilisée*/
    ap_abstract1_clear(man, &abs);
    ap_abstract1_clear(man, &abs_if);
    ap_abstract1_clear(man, &abs_if_y);
    ap_abstract1_clear(man, &abs_else);
    ap_abstract1_clear(man, &abs_else_y);
    ap_abstract1_clear(man, &abs_res);

    ap_texpr1_free(y1);
    ap_texpr1_free(y2);

    ap_lincons1_array_clear(&cons1_array);
    ap_lincons1_array_clear(&cons2_array);

    ap_interval_array_free(array, 2);
    ap_manager_free(man);
    ap_manager_free(manNS);
}

