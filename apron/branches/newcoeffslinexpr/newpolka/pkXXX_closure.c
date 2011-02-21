/* ********************************************************************** */
/* pkXXX_closure.c: topological closure */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"

/* Either po is just allocated with the right dimensions, or po==pa */

pkXXX_t* pkXXX_closure(ap_manager_t* man, bool destructive, pkXXX_t* pa)
{
  matrixXXX_t* C;
  bool change,positivity;
  size_t i;
  pkXXX_t* po;

  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_CLOSURE);
  man->result.flag_best = man->result.flag_exact = true;
  if (!pk->strict){
    return destructive ? pa : pkXXX_copy(man,pa);
  }
  if (pk->funopt->algorithm<0)
    pkXXX_obtain_C(man,pa,"of the argument");
  else
    pkXXX_chernikova(man,pa,"of the argument");

  if (!pa->C && !pa->F){
    return destructive ? pa : pkXXX_copy(man,pa);
  }
  po = destructive ? pa : pkXXX_alloc(pa->dim);
  if (pk->exn){
    pkXXX_set_top(pk,po);
    man->result.flag_best = man->result.flag_exact = false;
    return po;
  }
  if (!destructive){
    po->C = matrixXXX_copy(pa->C);
  }

  C = po->C;
  change = false;
  positivity = false;
  for (i=0;i<C->nbrows; i++){
    if (numintXXX_sgn(C->p[i][polka_eps])<0){
      if (vectorXXX_is_positivity_constraint(pk,C->p[i],
					     C->nbcolumns)){
	/* we keep the positivity constraint epsilon<=1 */
	positivity = true;
      }
      else {
	change = true;
	numintXXX_set_int(C->p[i][polka_eps],0);
      }
    }
  }
  assert(change || positivity);
  if (change){
    if (!positivity){
      numintXXX_t* q;
      size_t nbrows;
      /* we should add it */
      nbrows = C->nbrows;
      matrixXXX_resize_rows_lazy(C,C->nbrows+1);
      q = C->p[nbrows];
      numintXXX_set_int(q[0],1);
      numintXXX_set_int(q[polka_cst],1);
      numintXXX_set_int(q[polka_eps],-1);
      for (i=3; i<C->nbcolumns; i++) numintXXX_set_int(q[i],0);
    }
    C->_sorted = false;
    if (destructive){
      if (po->F) matrixXXX_free(po->F);
      if (po->satC) satmat_free(po->satC);
      if (po->satF) satmat_free(po->satF);
      po->F = NULL;
      po->satC = NULL;
      po->satF = NULL;
      po->status = pk_status_conseps;
      po->nbeq = 0;
      po->nbline = 0;
    }
  }
  assert(pkXXX_check(pk,po));
  return po;
}
