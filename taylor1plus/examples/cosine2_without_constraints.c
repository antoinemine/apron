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
    ap_manager_t* manNS = box_manager_alloc();
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

    /* construire la contrainte (x \leq 45) dans cons45_array[0] */
    ap_linexpr1_t linexp45 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp45, (double)45.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp45, "x", (double)(-1.0));
    ap_lincons1_t cons45 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp45, NULL);
    ap_lincons1_array_t cons45_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&cons45_array, 0, &cons45);

    /* construire la contrainte (45 \leq x \leq 90) dans cons1_array[0] */
    ap_linexpr1_t linexp1 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp1, (double)90.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp1, "x", (double)(-1.0));
    ap_linexpr1_t linexp1_ = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&linexp1_, (double)-45.0);
    ap_linexpr1_set_coeff_scalar_double(&linexp1_, "x", (double)(1.0));
    ap_lincons1_t cons1 = ap_lincons1_make(AP_CONS_SUPEQ, &linexp1, NULL);
    ap_lincons1_t cons1_ = ap_lincons1_make(AP_CONS_SUPEQ, &linexp1_, NULL);
    ap_lincons1_array_t cons1_array = ap_lincons1_array_make(env, 2);
    ap_lincons1_array_set(&cons1_array, 0, &cons1);
    ap_lincons1_array_set(&cons1_array, 1, &cons1_);
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

    /* construire y0 = 1 - 0.006508738196*x */
    ap_texpr1_t* y0 = ap_texpr1_binop(AP_TEXPR_SUB,
	    ap_texpr1_cst_scalar_double(env, 1.0),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_cst_scalar_double(env, -0.006508738196),
		ap_texpr1_var(env, "x"),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);

    /* construire l'expression arth y1 = 1 + x*x*(-0.00017644492 +x*0.000000588757)*/
    ap_texpr1_t* y1 = ap_texpr1_binop(AP_TEXPR_ADD,
	    ap_texpr1_cst_scalar_double(env, 1.0),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_binop(AP_TEXPR_ADD,
		    ap_texpr1_cst_scalar_double(env, -0.00017644492),
		    ap_texpr1_binop(AP_TEXPR_MUL,
			ap_texpr1_var(env, "x"),
			ap_texpr1_cst_scalar_double(env,0.000000588757),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP),
		    AP_RTYPE_DOUBLE,
		    AP_RDIR_UP),
		ap_texpr1_binop(AP_TEXPR_MUL,
		    ap_texpr1_var(env, "x"),
		    ap_texpr1_var(env, "x"),
		    AP_RTYPE_DOUBLE,
		    AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);
//		ap_texpr1_var(env, "x"),
//		ap_texpr1_binop(AP_TEXPR_MUL,
//		    ap_texpr1_var(env, "x"),
//		    ap_texpr1_binop(AP_TEXPR_ADD,
//			ap_texpr1_cst_scalar_double(env, -0.00017644492),
//			ap_texpr1_binop(AP_TEXPR_MUL,
//			    ap_texpr1_var(env, "x"),
//			    ap_texpr1_cst_scalar_double(env,0.000000588757),
//			    AP_RTYPE_DOUBLE,
//			    AP_RDIR_UP),
//			AP_RTYPE_DOUBLE,
//			AP_RDIR_UP),
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

    ap_abstract1_t* if_then_else = (ap_abstract1_t*)malloc(3*sizeof(ap_abstract1_t));
    /******** IF *********/
    /* appliquer le test x \leq 45 */
    //ap_abstract1_t abs1_ = ap_abstract1_meet_lincons_array(man, false, &abs, &cons45_array);
    /* evaluer [[y <- y0]] */
    //if_then_else[0] = ap_abstract1_assign_texpr(man, false, &abs1_, "y", y0, NULL);
    if_then_else[0] = ap_abstract1_assign_texpr(man, false, &abs, "y", y0, NULL);

    /*********** ELSE IF ************/
    /* appliquer le test 45 \leq x \leq 90 */
    //ap_abstract1_t abs2_ = ap_abstract1_meet_lincons_array(man, false, &abs, &cons1_array);
    /* evaluer [[y <- y1]] */
    //if_then_else[1]  = ap_abstract1_assign_texpr(man, false, &abs2_, "y", y1, NULL);
    if_then_else[1]  = ap_abstract1_assign_texpr(man, false, &abs, "y", y1, NULL);
    /********** ELSE ************/
    /* appliquer le test x \geq 90 */
    //ap_abstract1_t abs3_ = ap_abstract1_meet_lincons_array(man, false, &abs, &cons2_array);
    /* evaluer [[y <- y2]] */
    //if_then_else[2] = ap_abstract1_assign_texpr(man, false, &abs3_, "y", y2, NULL);
    if_then_else[2] = ap_abstract1_assign_texpr(man, false, &abs, "y", y2, NULL);

    /********* JOIN **************/
    ap_abstract1_t res = ap_abstract1_join_array(man, if_then_else, 3);
    /* pour afficher le resultat */
    ap_abstract1_fprint(stdout, man, &res);
    /* pour afficher la concretisation intervalle de abs_res */
    ap_box1_t gamma = ap_abstract1_to_box(man, &res);
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer la mémoire utilisée*/
    ap_abstract1_clear(man, &abs);
    //ap_abstract1_clear(man, &abs1_);
    //ap_abstract1_clear(man, &abs2_);
    //ap_abstract1_clear(man, &abs3_);
    ap_abstract1_clear(man, &res);
    ap_abstract1_clear(man, &if_then_else[0]);
    ap_abstract1_clear(man, &if_then_else[1]);
    ap_abstract1_clear(man, &if_then_else[2]);
    free(if_then_else);

    ap_texpr1_free(y0);
    ap_texpr1_free(y1);
    ap_texpr1_free(y2);

    ap_lincons1_array_clear(&cons45_array);
    ap_lincons1_array_clear(&cons1_array);
    ap_lincons1_array_clear(&cons2_array);

    ap_interval_array_free(array, 2);
    ap_manager_free(man);
    ap_manager_free(manNS);
}

