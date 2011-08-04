#include <stdio.h>
#include "ap_global0.h"
#include "ml2c_wrapper.h"

void test1(void) 
{
  ap_manager_t* man;
  ap_dimension_t dim = { 10, 0 };
  ap_abstract0_t* a;
  bool b;

  man = wrapper_manager_alloc();
 
  a = ap_abstract0_top(man, dim);
  b = ap_abstract0_is_top(man, a);
  printf("is_top: %s\n",(b ? "true" : "false"));
  a = ap_abstract0_bottom(man, dim);
  b = ap_abstract0_is_top(man, a);
  printf("is_top: %s\n",(b ? "true" : "false"));
  free(a);
}

int main (int argc, char* argv[]) {
  test1();
  return(0);
}
