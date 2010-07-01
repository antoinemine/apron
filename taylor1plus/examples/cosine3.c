#include <time.h>
#include "ap_global1.h"
#include "t1p.h"
#include "oct.h"
#include "pk.h"
#include "box.h"
#include "ap_ppl.h"

/************************************
  var x : real, y : real;
  begin
  x = random;
  assume x>=0 and x<=180;
  if (x <= 45) then
  y = 1 - 0.006508738196*x;
  else 
  if (x <= 90) then
  y = 1 + x*x*(-0.00017644492 +x*0.000000588757);
  else 
  if (x<=135) then
  y = 1.283184584 +x*(-0.0062929908 + x*(-0.00014148386 +x*0.000000588757));
  else
  y = 0.17157287528 - 0.006508738196*x;
  endif;
  endif;
  endif;
  end
  **********************************/

int main(void) {
    clock_t start, end;
    double time;
    double cpu_time_used;
    start = clock();

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
    //ap_manager_t* man = box_manager_alloc();

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

    /* -x + 45 */
    ap_linexpr1_t x45 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&x45, (double)45.0);
    ap_linexpr1_set_coeff_scalar_double(&x45, "x", (double)(-1.0));
    ap_lincons1_t consif45 = ap_lincons1_make(AP_CONS_SUPEQ, &x45, NULL);

    /* x - 45 */
    ap_linexpr1_t xm45 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&xm45, (double)-45.0);
    ap_linexpr1_set_coeff_scalar_double(&xm45, "x", (double)(1.0));
    ap_lincons1_t conselse45 = ap_lincons1_make(AP_CONS_SUPEQ, &xm45, NULL);

    /* -x + 90 */
    ap_linexpr1_t x90 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&x90, (double)90.0);
    ap_linexpr1_set_coeff_scalar_double(&x90, "x", (double)(-1.0));
    ap_lincons1_t consif90 = ap_lincons1_make(AP_CONS_SUPEQ, &x90, NULL);

    /* x - 90 */
    ap_linexpr1_t xm90 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&xm90, (double)-90.0);
    ap_linexpr1_set_coeff_scalar_double(&xm90, "x", (double)(1.0));
    ap_lincons1_t conselse90 = ap_lincons1_make(AP_CONS_SUPEQ, &xm90, NULL);

    /* -x + 135 */
    ap_linexpr1_t x135 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&x135, (double)135.0);
    ap_linexpr1_set_coeff_scalar_double(&x135, "x", (double)(-1.0));
    ap_lincons1_t consif135 = ap_lincons1_make(AP_CONS_SUPEQ, &x135, NULL);

    /* x - 135 */
    ap_linexpr1_t xm135 = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr1_set_cst_scalar_double(&xm135, (double)-135.0);
    ap_linexpr1_set_coeff_scalar_double(&xm135, "x", (double)(1.0));
    ap_lincons1_t conselse135 = ap_lincons1_make(AP_CONS_SUPEQ, &xm135, NULL);

    ap_lincons1_array_t consif45_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&consif45_array, 0, &consif45);
    ap_lincons1_array_t conselse45_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&conselse45_array, 0, &conselse45);
    ap_lincons1_array_t consif90_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&consif90_array, 0, &consif90);
    ap_lincons1_array_t conselse90_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&conselse90_array, 0, &conselse90);
    ap_lincons1_array_t consif135_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&consif135_array, 0, &consif135);
    ap_lincons1_array_t conselse135_array = ap_lincons1_array_make(env, 1);
    ap_lincons1_array_set(&conselse135_array, 0, &conselse135);

//    ap_lincons1_array_fprint(stdout, &cons3_array);

    /* construire y0 = 1 - 0.006508738196*x */
    // en mettant comme constante -0.006508738196 les octogones sont vides et Taylor1+ donne qq chose, zarb !!
    ap_texpr1_t* y0 = ap_texpr1_binop(AP_TEXPR_SUB,
	    ap_texpr1_cst_scalar_double(env, 1.0),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_cst_scalar_double(env, (double)(0.006508738196)),
		ap_texpr1_var(env, "x"),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);
    //ap_texpr1_fprint(stdout, y0);


    /* construire l'expression arth y1 = 1 + x*x*(-0.00017644492 +x*0.000000588757)*/
    //ap_texpr1_t* cons = ap_texpr1_cst_scalar_double(env, (double)(1.0));
    //ap_texpr1_t* cons = ap_texpr1_cst_scalar_double(env, (double)(0.000000588757));
    //ap_texpr1_t* cons = ap_texpr1_cst_scalar_int(env, 23);
    //ap_texpr1_fprint(stdout, cons);
    //ap_texpr1_t* bloc0 = ap_texpr1_var(env, "x");
    ap_scalar_t* scalar = ap_scalar_alloc_set_double(0.000000588757);
    ap_texpr1_t* cons = ap_texpr1_cst_scalar(env, scalar);
    ap_texpr1_t* cons3 = ap_texpr1_cst_scalar(env, scalar);
    ap_scalar_t* scalar1 = ap_scalar_alloc_set_double(0.00017644492);
    ap_texpr1_t* cons1 = ap_texpr1_cst_scalar(env, scalar1);

    ap_texpr1_t* toto1 = ap_texpr1_binop(AP_TEXPR_MUL, cons, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* toto2 = ap_texpr1_binop(AP_TEXPR_MUL, toto1, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* toto3 = ap_texpr1_binop(AP_TEXPR_MUL, toto2, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* toto4 = ap_texpr1_binop(AP_TEXPR_MUL, cons1, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* toto5 = ap_texpr1_binop(AP_TEXPR_MUL, toto4, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* toto6 = ap_texpr1_binop(AP_TEXPR_SUB, toto3, toto5, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* y1    = ap_texpr1_binop(AP_TEXPR_ADD, ap_texpr1_cst_scalar_double(env, (double)(1.0)), toto6, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    
    //ap_texpr1_t* y1 = ap_texpr1_binop(AP_TEXPR_ADD, ap_texpr1_cst_scalar_double(env, 1.0), ap_texpr1_binop(AP_TEXPR_MUL, ap_texpr1_binop(AP_TEXPR_SUB, ap_texpr1_binop(AP_TEXPR_MUL, ap_texpr1_var(env, "x"), ap_texpr1_cst_scalar_double(env, (double)(0.000000588757)), AP_RTYPE_DOUBLE, AP_RDIR_UP), ap_texpr1_cst_scalar_double(env, (double)(0.00017644492)), AP_RTYPE_DOUBLE, AP_RDIR_UP), ap_texpr1_binop(AP_TEXPR_MUL, ap_texpr1_var(env, "x"), ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP), AP_RTYPE_DOUBLE, AP_RDIR_UP), AP_RTYPE_DOUBLE, AP_RDIR_UP);


//    ap_texpr1_t* y1 = ap_texpr1_binop(AP_TEXPR_ADD,
//	    ap_texpr1_cst_scalar_double(env, 1.0),
//	    ap_texpr1_binop(AP_TEXPR_MUL,
//		ap_texpr1_binop(AP_TEXPR_SUB,
//		    ap_texpr1_binop(AP_TEXPR_MUL,
//			ap_texpr1_var(env, "x"),
//			ap_texpr1_cst_scalar_double(env, 0.000000588757),
//			AP_RTYPE_DOUBLE,
//			AP_RDIR_UP),
//		    ap_texpr1_cst_scalar_double(env, (double)(0.00017644492)),
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

    /* construire l'expression arth y2 = 1.283184584 +x*(-0.0062929908 + x*(-0.00014148386 +x*0.000000588757))*/
   // ap_texpr1_t* 1_  = ap_texpr1_var(env, "x");
   // ap_texpr1_t* 2_ = ap_texpr1_binop(AP_TEXPR_MUL, 1_, cons, AP_RTYPE_DOUBLE, AP_RDIR_UP);

    ap_scalar_t* s1 = ap_scalar_alloc_set_double(0.00014148386);
    ap_scalar_t* s2 = ap_scalar_alloc_set_double(0.0062929908);
    ap_scalar_t* s3 = ap_scalar_alloc_set_double(1.283184584);
    ap_texpr1_t* e1 = ap_texpr1_cst_scalar(env, s1);
    ap_texpr1_t* e2 = ap_texpr1_cst_scalar(env, s2);
    ap_texpr1_t* e3 = ap_texpr1_cst_scalar(env, s3);
    ap_texpr1_t* titi = ap_texpr1_binop(AP_TEXPR_MUL, cons3, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi1= ap_texpr1_binop(AP_TEXPR_MUL, titi,  ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi2= ap_texpr1_binop(AP_TEXPR_MUL, titi1,  ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi3= ap_texpr1_binop(AP_TEXPR_MUL, e1, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi4= ap_texpr1_binop(AP_TEXPR_MUL, titi3, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi5= ap_texpr1_binop(AP_TEXPR_MUL, e2, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi6= ap_texpr1_binop(AP_TEXPR_SUB, titi2, titi4, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi7= ap_texpr1_binop(AP_TEXPR_SUB, titi6, titi5, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* y2= ap_texpr1_binop(AP_TEXPR_ADD, e3, titi7, AP_RTYPE_DOUBLE, AP_RDIR_UP);


    /*
    ap_texpr1_t* titi = ap_texpr1_binop(AP_TEXPR_MUL, cons3, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi1 = ap_texpr1_binop(AP_TEXPR_SUB, titi, e1, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi2 = ap_texpr1_binop(AP_TEXPR_MUL, titi1, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi3 = ap_texpr1_binop(AP_TEXPR_SUB, titi2, e2, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* titi4 = ap_texpr1_binop(AP_TEXPR_MUL, titi3, ap_texpr1_var(env, "x"), AP_RTYPE_DOUBLE, AP_RDIR_UP);
    ap_texpr1_t* y2    = ap_texpr1_binop(AP_TEXPR_ADD, titi4, e3, AP_RTYPE_DOUBLE, AP_RDIR_UP);
    */

    /*
    ap_texpr1_t* y2 = ap_texpr1_binop(AP_TEXPR_ADD,
	    ap_texpr1_cst_scalar_double(env, (double)(1.283184584)),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_var(env, "x"),
		ap_texpr1_binop(AP_TEXPR_SUB,
		    ap_texpr1_binop(AP_TEXPR_MUL,
			ap_texpr1_var(env, "x"),
			ap_texpr1_binop(AP_TEXPR_SUB,
			    ap_texpr1_binop(AP_TEXPR_MUL,
				ap_texpr1_var(env, "x"),
				ap_texpr1_cst_scalar_double(env, (double)(0.000000588757)),
				AP_RTYPE_DOUBLE,
				AP_RDIR_UP),
			    ap_texpr1_cst_scalar_double(env, (double)(0.00014148386)),
			    AP_RTYPE_DOUBLE,
			    AP_RDIR_UP),
			AP_RTYPE_DOUBLE,
			AP_RDIR_UP),
		    ap_texpr1_cst_scalar_double(env, (double)(0.0062929908)),
		    AP_RTYPE_DOUBLE,
		    AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP);
		*/
    /* pour afficher l'expression y2 */
    //ap_texpr1_fprint(stdout, y2);

    /* construire y = 0.17157287528 - 0.006508738196*x */
    ap_texpr1_t* y3 = ap_texpr1_binop(AP_TEXPR_SUB,
	    ap_texpr1_cst_scalar_double(env, 0.17157287528),
	    ap_texpr1_binop(AP_TEXPR_MUL,
		ap_texpr1_cst_scalar_double(env, (double)(0.006508738196)),
		ap_texpr1_var(env, "x"),
		AP_RTYPE_DOUBLE,
		AP_RDIR_UP),
	    AP_RTYPE_DOUBLE,
	    AP_RDIR_UP);
    //ap_texpr1_fprint(stdout, y3);

    ap_abstract1_t abs1, abs2, abs3, abs4, abs5, abs6, abs7, abs8, abs9, abs10, abs11, abs12, res;
    /* if (x <= 45) */
    fprintf(stdout, "######## if (x <= 45) ########\n");
    abs1 = ap_abstract1_meet_lincons_array(man, false, &abs, &consif45_array);
    ap_abstract1_fprint(stdout, man, &abs1);
    /* assign */
    fprintf(stdout, "######## if (x <= 45) y = 1 - 0.006508738196*x ########\n");
    abs2 = ap_abstract1_assign_texpr(man, false, &abs1, "y", y0, NULL);
    ap_abstract1_fprint(stdout, man, &abs2);
    /* else [x >= 45] */
    fprintf(stdout, "######## else du if (x <= 45) ########\n");
    abs3 = ap_abstract1_meet_lincons_array(man, false, &abs, &conselse45_array);
    ap_abstract1_fprint(stdout, man, &abs3);
    /* if (x <= 90) */
    fprintf(stdout, "######## if (x <= 90) ########\n");
    abs4 = ap_abstract1_meet_lincons_array(man, false, &abs3, &consif90_array);
    ap_abstract1_fprint(stdout, man, &abs4);
    /* assign */
    //ap_abstract1_fprint(stdout, man, &abs4);
    fprintf(stdout, "######## if (x <= 90) y = 1 - 0.00017644492*x*x + 0.000000588757*x*x*x; ########\n");
    abs5 = ap_abstract1_assign_texpr(man, false, &abs4, "y", y1, NULL);
    ap_abstract1_fprint(stdout, man, &abs5);
    /* else [x >= 90] */
    fprintf(stdout, "######## else du if (x <= 90) ########\n");
    abs6 = ap_abstract1_meet_lincons_array(man, false, &abs3, &conselse90_array);
    ap_abstract1_fprint(stdout, man, &abs6);
    /* if (x<=135) */
    fprintf(stdout, "######## if (x <= 135) ########\n");
    abs7 = ap_abstract1_meet_lincons_array(man, false, &abs6, &consif135_array);
    ap_abstract1_fprint(stdout, man, &abs7);
    /* assign */
    fprintf(stdout, "######## if (x <= 135) y = 1.283184584 - 0.0062929908*x - 0.00014148386*x*x +0.000000588757*x*x*x; ########\n");
    abs8 = ap_abstract1_assign_texpr(man, false, &abs7, "y", y2, NULL);
    ap_abstract1_fprint(stdout, man, &abs8);
    /* else [x >= 135] */
    fprintf(stdout, "######## else if (x <= 135) ########\n");
    abs9 = ap_abstract1_meet_lincons_array(man, false, &abs6, &conselse135_array);
    ap_abstract1_fprint(stdout, man, &abs9);
    /* assign */
    fprintf(stdout, "######## else if (x <= 135) y = 0.17157287528 - 0.006508738196*x; ########\n");
    abs10 = ap_abstract1_assign_texpr(man, false, &abs9, "y", y3, NULL);
    ap_abstract1_fprint(stdout, man, &abs10);
    /* join if ((x<=135)) else */
    fprintf(stdout, "######## if (x <= 135) JOIN son else ########\n");
    abs11 = ap_abstract1_join(man, false, &abs8, &abs10);
    ap_abstract1_fprint(stdout, man, &abs11);
    /* join if ((x<=90)) else */
    fprintf(stdout, "######## JOIN avec le else du if (x <= 90) ########\n");
    abs12 = ap_abstract1_join(man, false, &abs5, &abs11);
    ap_abstract1_fprint(stdout, man, &abs12);
    /* join if ((x<=45)) else */
    fprintf(stdout, "######## JOIN avec le else du if (x <= 45) ########\n");
    res = ap_abstract1_join(man, false, &abs2, &abs12);
    ap_abstract1_fprint(stdout, man, &res);

    /* pour afficher le resultat */
 //   ap_abstract1_fprint(stdout, man, &res);
    /* pour afficher la concretisation intervalle de abs_res */
    ap_box1_t gamma = ap_abstract1_to_box(man, &res);
    double inf = 0.0; double sup = 0.0;
    size_t i = 0;
    for (i=0; i<env->realdim; i++) {
	ap_double_set_scalar(&inf, gamma.p[i]->inf, GMP_RNDU);
	ap_double_set_scalar(&sup, gamma.p[i]->sup, GMP_RNDU);
	printf("%s : [%f,%f]\n", (char*)name_of_dim[i], inf, sup);
    }
    ap_box1_fprint(stdout, &gamma);
    ap_box1_clear(&gamma);

    /* libérer la mémoire utilisée*/
    ap_abstract1_clear(man, &abs);

    /*
    ap_texpr1_free(y0);
    ap_texpr1_free(y1);
    ap_texpr1_free(y2);
    */

    ap_interval_array_free(array, 2);
    ap_manager_free(man);
    ap_manager_free(manNS);
    end = clock();
    time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("CpuTime: %.2f\n",time);
}

