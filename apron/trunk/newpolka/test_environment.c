/* ********************************************************************** */
/* test_environment.c: testing of environment */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license 
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#include "ap_expr0.h"
#include "ap_environment.h"

int main(int argc, char**argv)
{
  size_t i;
  environment_t* e1;
  environment_t* e2;
  environment_t* e3;
  environment_t* e4;
  environment_t* e5;
  dim_t* perm1;
  dim_t* perm2;

  var_t name_of_intdim1[3] = {
    "a30","a10","a20"
  };
  var_t name_of_realdim1[3] = {
    "a11","a31","a21"
  };
  var_t name_of_intdim2[4] = {
    "a2","a0","a22","a24"
  };
  var_t name_of_realdim2[4] = {
    "a27","a23","a33","a37"
  };
  var_t name_of_intdim5[4] = {
    "a2","a10","a22","a20"
  };
  var_t name_of_realdim5[4] = {
    "a27","a31","a11","a37"
  };
  e1 = environment_make_empty();
  environment_ref(e1);
  environment_fdump(stdout,e1);
  environment_deref(e1);
  e1 = environment_make(name_of_intdim1,3,
			name_of_realdim1,3);
  environment_ref(e1);
  environment_fdump(stdout,e1);
  e2 = environment_make(name_of_intdim2,4,
			name_of_realdim2,4);
  environment_ref(e2);
  environment_fdump(stdout,e2);
  e5 = environment_make(name_of_intdim5,4,
			name_of_realdim5,4);
  environment_ref(e5);
  environment_fdump(stdout,e5);
  
  e3 = environment_add(e1,name_of_intdim2,4,
			name_of_realdim2,4);
  environment_ref(e3);
  environment_fdump(stdout,e3);
  e4 = environment_add(e2,name_of_intdim1,3,
		       name_of_realdim1,3);
  environment_ref(e4);
  environment_fdump(stdout,e4);
  for(i=0; i<e4->intdim+e4->realdim; i++){
    var_t var = environment_var_of_dim(e4,i);
    dim_t dim = environment_dim_of_var(e4,var);
    printf("i=%d, var=%s, dim=%d\n",i,var,dim);
  }
  environment_deref(e4);
  environment_deref(e3);

  e4 = environment_remove(e5,name_of_intdim2,4,
			name_of_realdim2,4);
  environment_ref(e4);
  environment_fdump(stdout,e4);
  environment_deref(e4);


  e3 = environment_lce(e1,e2,&perm1,&perm2);
  environment_ref(e3);
  environment_fdump(stdout,e3);
  permutation_fprint(stdout,perm1,e3->intdim+e3->realdim);
  permutation_fprint(stdout,perm2,e3->intdim+e3->realdim);
  free(perm1);
  free(perm2);

  e4 = environment_lce(e3,e2,&perm1,&perm2);
  assert(e3==e4);
  environment_ref(e4);
  environment_fdump(stdout,e4);
  assert(perm1==NULL);
  permutation_fprint(stdout,perm2,e3->intdim+e3->realdim);
  free(perm2);
  environment_deref(e3);
  environment_deref(e4);

  e3 = environment_lce(e1,e5,&perm1,&perm2);
  environment_ref(e3);
  environment_fdump(stdout,e3);
  permutation_fprint(stdout,perm1,e3->intdim+e3->realdim);
  permutation_fprint(stdout,perm2,e3->intdim+e3->realdim);
  free(perm1);
  free(perm2);
  environment_deref(e3);



  environment_deref(e1);
  environment_deref(e2);
  environment_deref(e5);
}
