/* ********************************************************************** */
/* example1.c: using APRON library  */
/* ********************************************************************** */

/* We illustrate here the use of the APRON interface, at level 1 */

#include "ap_global0.h"
#include "ap_global1.h"

#include "box.h"
#include "oct.h"
#include "pk.h"
#include "pkeq.h"
#include "ap_ppl.h"

void ex1(ap_manager_t* man)
{
  printf("******************************\n");
  printf("ex1\n");
  printf("Library %s, version %s\n",man->library,man->version);
  printf("******************************\n");

  ap_var_t name_of_dim[6] = {
    "x","y","z","u","w","v"
  };
  ap_environment_t* env = ap_environment_alloc(&name_of_dim[0],3,&name_of_dim[3],3);

  /* =================================================================== */
  /* Creation of polyhedra
     1/2x+2/3y=1, [1,2]<=z+2w<=4 */
  /* =================================================================== */

  /* 0. Create the array */
  ap_lincons1_array_t array = ap_lincons1_array_make(env,3);

  /* 1.a Creation of an equality constraint 1/2x+2/3y=1 */
  ap_linexpr1_t expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
    /* Now expr is memory-managed by cons */

  /* 1.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_FRAC,1,2,"x",
		       AP_COEFF_S_FRAC,2,3,"y",
		       AP_CST_S_INT,1,
		       AP_END);
  /* 1.c Put in the array */
  ap_lincons1_array_set(&array,0,&cons);
    /* Now cons is memory-managed by array */

  /* 2.a Creation of an inequality constraint [1,2]<=z+2w */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 2.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,1,"z",
		       AP_COEFF_S_DOUBLE,2.0,"w",
		       AP_CST_I_INT,-2,-1,
		       AP_END);
  /* 2.c Put in the array */
  ap_lincons1_array_set(&array,1,&cons);

  /* 2.a Creation of an inequality constraint */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 2.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,1,"z",
		       AP_COEFF_S_DOUBLE,2.0,"w",
		       AP_CST_I_INT,-2,-1,
		       AP_END);
  /* 2.c Put in the array */
  ap_lincons1_array_set(&array,1,&cons);

  /* 3.a Creation of an inequality constraint by duplication and
       modification z+2w<=4 */
  cons = ap_lincons1_copy(&cons);
  /* 3.b Fill the constraint (by negating the existing coefficients) */
  expr = ap_lincons1_linexpr1ref(&cons);
  {
    ap_coeff_t* pcoeff;
    ap_var_t var;
    size_t i;
    ap_linexpr1_ForeachLinterm1(&expr,i,var,pcoeff){
      ap_coeff_neg(pcoeff,pcoeff);
    }
  }
  ap_linexpr1_set_cst_scalar_int(&expr,4);
  /* 3.c Put in the array */
  ap_lincons1_array_set(&array,2,&cons);

  /* 4. Creation of an abstract value */
  ap_abstract1_t abs = ap_abstract1_of_lincons_array(man,env,&array);

  fprintf(stdout,"Abstract value:\n");
  ap_abstract1_fprint(stdout,man,&abs);

  /* deallocation */
  ap_lincons1_array_clear(&array);

  /* =================================================================== */
  /* Creation of polyhedra
     x+y+z+w+u=0, 0<= w <= 5, -5<=u<=0, x+y-z-w>=0, */
  /* =================================================================== */
  array = ap_lincons1_array_make(env,6);
  expr = ap_linexpr1_make(env,AP_LINEXPR_DENSE,0);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_linexpr1_set_list(&expr,
		       AP_COEFF_S_INT,1,"x",
		       AP_COEFF_S_INT,1,"y",
		       AP_COEFF_S_INT,1,"z",
		       AP_COEFF_S_INT,1,"w",
		       AP_COEFF_S_INT,1,"u",
		       AP_END);
  ap_lincons1_array_set(&array,0,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
  ap_linexpr1_set_list(&expr, AP_COEFF_S_INT,1,"w",AP_END);
  ap_lincons1_array_set(&array,1,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
  ap_linexpr1_set_list(&expr, AP_COEFF_S_INT,-1,"w",AP_CST_S_INT,5, AP_END);
  ap_lincons1_array_set(&array,2,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
  ap_linexpr1_set_list(&expr, AP_COEFF_S_INT,1,"u",AP_CST_S_INT,5, AP_END);
  ap_lincons1_array_set(&array,3,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
  ap_linexpr1_set_list(&expr, AP_COEFF_S_INT,-1,"u",AP_END);
  ap_lincons1_array_set(&array,4,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_DENSE,0);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_linexpr1_set_list(&expr,
		       AP_COEFF_S_INT,1,"x",
		       AP_COEFF_S_INT,1,"y",
		       AP_COEFF_S_INT,-1,"z",
		       AP_COEFF_S_INT,-1,"w",
		       AP_END);
  ap_lincons1_array_set(&array,5,&cons);

  ap_abstract1_t abs2 = ap_abstract1_of_lincons_array(man,env,&array);

  fprintf(stdout,"Abstract value 2:\n");
  ap_abstract1_fprint(stdout,man,&abs2);

  ap_lincons1_array_clear(&array);

  /* =================================================================== */
  /* Meet and Join */
  /* =================================================================== */

  ap_abstract1_t res = ap_abstract1_meet(man,false,&abs,&abs2);
  fprintf(stdout,"Meet:\n");
  ap_abstract1_fprint(stdout,man,&res);
  ap_abstract1_clear(man,&res);

  res = ap_abstract1_join(man,false,&abs,&abs2);
  fprintf(stdout,"Join:\n");
  ap_abstract1_fprint(stdout,man,&res);
  ap_abstract1_clear(man,&res);

  /* =================================================================== */
  /* Assignement */
  /* =================================================================== */

  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  ap_linexpr1_set_list(&expr,
		       AP_COEFF_S_INT,-3,"w",
		       AP_COEFF_S_DOUBLE,2.625,"z",
		       AP_CST_I_INT,0,3,
		       AP_END);
  fprintf(stdout,"Assignement (side-effect) in abstract value of u by expression:\n");
  ap_linexpr1_fprint(stdout,&expr);

  abs = ap_abstract1_assign_linexpr(man,true,&abs,"u",&expr,NULL);
  fprintf(stdout,"\n");
  ap_abstract1_fprint(stdout,man,&abs);
  ap_linexpr1_clear(&expr);

  /* deallocation */
  ap_abstract1_clear(man,&abs);
  ap_abstract1_clear(man,&abs2);

  ap_environment_free(env);
}




void ex2(ap_manager_t* man)
{
  printf("******************************\n");
  printf("ex2\n");
  printf("Library %s, version %s\n",man->library,man->version);
  printf("******************************\n");
  ap_var_t name_of_dim[6] = {
    "x","y","z","u","w","v"
  };
  ap_var_t tab[2] = {
    "x","y"
  };
  ap_environment_t* env = ap_environment_alloc(&name_of_dim[0],3,&name_of_dim[3],3);

  /* =================================================================== */
  /* Creation of polyhedra
     -39x + 40y >= 0
     -6x - 20y + 85 >= 0
     x >= 0  */
  /* =================================================================== */

  /* 0. Create the array */
  ap_lincons1_array_t array = ap_lincons1_array_make(env,3);

  /* 1.a Creation of constraint -39x + 40y >= 0 */
  ap_linexpr1_t expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* Now expr is memory-managed by cons */

  /* 1.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-39,"x",
		       AP_COEFF_S_INT,40,"y",
		       AP_CST_S_INT,0,
		       AP_END);
  /* 1.c Put in the array */
  ap_lincons1_array_set(&array,0,&cons);
    /* Now cons is memory-managed by array */

  /* 2.a Creation of an inequality constraint -6x - 20y + 85 >= 0 */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 2.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-6,"x",
		       AP_COEFF_S_INT,-20,"y",
		       AP_CST_S_INT,85,
		       AP_END);
  /* 2.c Put in the array */
  ap_lincons1_array_set(&array,1,&cons);

  /* 3.a Creation of an inequality constraint x >= 0 */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,1);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 3.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,1,"x",
		       AP_CST_S_INT,0,
		       AP_END);
  /* 3.c Put in the array */
  ap_lincons1_array_set(&array,2,&cons);

  ap_abstract1_t abs = ap_abstract1_of_lincons_array(man,env,&array);
  fprintf(stdout,"Abstract value:\n");
  ap_abstract1_fprint(stdout,man,&abs);
  ap_abstract1_t abs2 = ap_abstract1_fold(man,false,&abs,tab,2);
  fprintf(stdout,"Abstract value 2:\n");
  ap_abstract1_fprint(stdout,man,&abs2);

  ap_lincons1_array_clear(&array);
  ap_abstract1_clear(man,&abs);
  ap_abstract1_clear(man,&abs2);
  ap_environment_free(env);
}




void ex3(ap_manager_t* man)
{
  printf("******************************\n");
  printf("ex3\n");
  printf("Library %s, version %s\n",man->library,man->version);
  printf("******************************\n");
  ap_var_t name_of_dim[6] = {
    "x","y","z","u","w","v"
  };
  ap_var_t tab[2] = {
    "x","y"
  };
  ap_environment_t* env = ap_environment_alloc(&name_of_dim[0],6,NULL,0);
  /* =================================================================== */
  /* Creation of polyhedra
     -39x + 40y = 0
     -3x - 10z + 43 = 0
     -6x - 20y + 86 >= 0
  */
  /* =================================================================== */

  /* 0. Create the array */
  ap_lincons1_array_t array = ap_lincons1_array_make(env,1);

  /* 1.a Creation of constraint -39x + 40y = 0 */
  ap_linexpr1_t expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
    /* Now expr is memory-managed by cons */

  /* 1.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-39,"x",
		       AP_COEFF_S_INT,40,"y",
		       AP_CST_S_INT,0,
		       AP_END);
  /* 1.c Put in the array */
  ap_lincons1_array_set(&array,0,&cons);
    /* Now cons is memory-managed by array */

  ap_abstract1_t abs = ap_abstract1_of_lincons_array(man,env,&array);
  fprintf(stdout,"Abstract value:\n");
  ap_abstract1_fprint(stdout,man,&abs);
  ap_generator1_array_t garray = ap_abstract1_to_generator_array(man,&abs);
  ap_generator1_array_fprint(stdout,&garray);
  ap_generator1_array_clear(&garray);

  /* 2.a Creation of an equality constraint -3x - 10z + 43 = 0 */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 2.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-3,"x",
		       AP_COEFF_S_INT,-10,"z",
		       AP_CST_S_INT,43,
		       AP_END);
  /* 2.c Put in the array */
  ap_lincons1_array_set(&array,0,&cons);

  fprintf(stdout,"Adding constraints:\n");
  ap_lincons1_array_fprint(stdout,&array);
  abs = ap_abstract1_meet_lincons_array(man,true,&abs,&array);
  fprintf(stdout,"Abstract value:\n");
  ap_abstract1_fprint(stdout,man,&abs);

  /* 3.a Creation of an inequality constraint -6x - 20y + 86 >= 0 */
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr,NULL);
    /* The old cons is not lost, because it is stored in the array.
       It would be an error to clear it (same for expr). */
  /* 2.b Fill the constraint */
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-6,"x",
		       AP_COEFF_S_INT,-20,"y",
		       AP_CST_S_INT,86,
		       AP_END);
  /* 2.c Put in the array */
  ap_lincons1_array_set(&array,0,&cons);
  fprintf(stdout,"Adding constraints:\n");
  ap_lincons1_array_fprint(stdout,&array);
  abs = ap_abstract1_meet_lincons_array(man,true,&abs,&array);
  fprintf(stdout,"Abstract value:\n");
  ap_abstract1_fprint(stdout,man,&abs);

  ap_lincons1_array_clear(&array);


  /* =================================================================== */
  /* Creation of polyhedra
     -39x + 40u = 0
     u = y
     v = x+z
     -6v - 14z + 86 = 0
  */
  /* =================================================================== */

  /* 0. Create the array */
  array = ap_lincons1_array_make(env,4);

  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-39,"x",
		       AP_COEFF_S_INT,40,"u",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&array,0,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-1,"u",
		       AP_COEFF_S_INT,1,"y",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&array,1,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-1,"v",
		       AP_COEFF_S_INT,1,"x",
		       AP_COEFF_S_INT,1,"y",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&array,1,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-1,"v",
		       AP_COEFF_S_INT,1,"x",
		       AP_COEFF_S_INT,1,"z",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&array,2,&cons);
  expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_EQ,&expr,NULL);
  ap_lincons1_set_list(&cons,
		       AP_COEFF_S_INT,-6,"v",
		       AP_COEFF_S_INT,-14,"z",
		       AP_CST_S_INT,86,
		       AP_END);
  ap_lincons1_array_set(&array,3,&cons);

  ap_abstract1_t abs2 = ap_abstract1_of_lincons_array(man,env,&array);
  fprintf(stdout,"Abstract value 2:\n");
  ap_abstract1_fprint(stdout,man,&abs2);
  ap_lincons1_array_clear(&array);

  ap_abstract1_canonicalize(man,&abs2);
  fprintf(stdout,"Abstract value 2:\n");
  ap_abstract1_fprint(stdout,man,&abs2);

  ap_abstract1_t abs3 = ap_abstract1_join(man,false,&abs,&abs2);
  fprintf(stdout,"Abstract value 3 (join of 1 and 2):\n");
  ap_abstract1_fprint(stdout,man,&abs3);

  ap_abstract1_t abs4 = ap_abstract1_meet(man,false,&abs,&abs2);
  fprintf(stdout,"Abstract value 3 (meet of 1 and 2):\n");
  ap_abstract1_fprint(stdout,man,&abs4);


  ap_abstract1_clear(man,&abs);
  ap_abstract1_clear(man,&abs2);
  ap_abstract1_clear(man,&abs3);
  ap_abstract1_clear(man,&abs4);

  ap_environment_free(env);
}

int main()
{
  ap_manager_t* man;

  man = box_manager_alloc();
  ex1(man);
  ex2(man);
  ap_manager_free(man);

  man = oct_manager_alloc();
  ex1(man);
  ex2(man);
  ap_manager_free(man);

  man = pk_manager_alloc(true);
  ex1(man);
  ex2(man);
  ex3(man);
  ap_manager_free(man);

  man = ap_ppl_poly_manager_alloc(true);
  ex1(man);
  ex2(man);
  ex3(man);
  ap_manager_free(man);

  man = pkeq_manager_alloc();
  ex2(man);
  ex3(man);
  ap_manager_free(man);

  man = ap_ppl_grid_manager_alloc();
  ex2(man);
  ex3(man);
  ap_manager_free(man);
}
