#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <caml/callback.h>
#include "ap_global0.h"
#include "ap_global1.h"
#include "ml2c_wrapper.h"

void test1(void)
{
  ap_manager_t* man;
  ap_dimension_t dim = { 10, 0 };
  ap_abstract0_t* a;
  bool b;

  man = wrapper_manager_alloc();

  printf("******************************\n");
  printf("test1\n");
  printf("Library %s, version %s\n",man->library,man->version);
  printf("******************************\n");

  a = ap_abstract0_top(man, dim);
  b = ap_abstract0_is_top(man, a);
  printf("is_top: %s\n",(b ? "true" : "false"));
  ap_abstract0_free(man, a);
  a = ap_abstract0_bottom(man, dim);
  b = ap_abstract0_is_top(man, a);
  printf("is_top: %s\n",(b ? "true" : "false"));
  ap_abstract0_free(man, a);
  ap_manager_free(man);
  printf("done\n\n");
}

void test2(void) {
  ap_var_t name_of_dim[6] = {
    "x","y","z","u","w","v"
    };
  ap_environment_t* env;
  ap_manager_t* man;
  int i;
  ap_var_operations = &ap_var_operations_default;
  /*name_of_dim = (ap_var_t*) malloc(6*sizeof(ap_var_t));
  for(i = 0; i < 6; i++)
    name_of_dim[i] = (ap_var_t) malloc(3*sizeof(char));
  strcpy(name_of_dim[0],"x");
  strcpy(name_of_dim[1],"y");
  strcpy(name_of_dim[2],"z");
  strcpy(name_of_dim[3],"u");
  strcpy(name_of_dim[4],"w");
  strcpy(name_of_dim[5],"v");*/

  man = wrapper_manager_alloc();
  env = ap_environment_alloc(&name_of_dim[0],3,&name_of_dim[3],3);
  printf("******************************\n");
  printf("test2\n");
  printf("Library %s, version %s\n",man->library,man->version);
  printf("******************************\n");

  /* 1.a Creation of an equality constraint 1/2x+2/3y=1 */
  ap_lincons1_t cons1;
  ap_lincons1_init(cons1, AP_SCALAR_MPQ, 10, env);
  printf("ok\n");
  bool res;
  ap_lincons1_set_list(man->num, cons1, &res,
		       AP_COEFF_LINT,1L,&name_of_dim[0],
		       AP_COEFF_LINT,2L,&name_of_dim[1],
		       AP_COEFF_LINT,-1L,AP_DIM_MAX,
		       AP_END);
  printf("ok\n");
  ap_lincons1_fprint(stdout,cons1);
}

void test3(void) {
  char* name_of_dim[6] = {
    "x","y","z","u","w","v"
    };
  ap_environment_t* env;
  ap_var_operations = &ap_var_operations_default;
  env = ap_environment_alloc(&name_of_dim[0], 3, &name_of_dim[3], 3);
}

int main (int argc, char* argv[]) {
  caml_startup(argv);
  test1();
  test2();
  test3();
  return(0);
}
