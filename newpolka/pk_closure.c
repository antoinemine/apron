/* ********************************************************************** */
/* pk_closure.c: topological closure */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_representation.h"
#include "pk_constructor.h"

/* Either po is just allocated with the right dimensions, or po==pa */

pk_t* pk_closure(ap_manager_t* man, bool destructive, pk_t* pa)
{
  pk_matrix_t* C;
  bool change,positivity;
  size_t i;
  pk_t* po;

  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_CLOSURE);
  man->result.flag_best = man->result.flag_exact = true;
  if (!pk->strict){
    return destructive ? pa : pk_copy(man,pa);
  }
  if (pk->funopt->algorithm<0)
    poly_obtain_C(man,pa,"of the argument");
  else
    poly_chernikova(man,pa,"of the argument");
  
  if (!pa->C && !pa->F){
    return destructive ? pa : pk_copy(man,pa);
  }
  po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);
  if (pk->exn){
    poly_set_top(pk,po);
    man->result.flag_best = man->result.flag_exact = false;
    return po;
  }
  if (!destructive){
    po->C = pk_matrix_copy(pa->C);
  }
  
  C = po->C;
  change = false;
  positivity = false;
  for (i=0;i<C->nbrows; i++){
    if (numint_sgn(C->p[i][polka_eps])<0){
      if (vector_is_positivity_constraint(pk,C->p[i],
					  C->nbcolumns)){
	/* we keep the positivity constraint epsilon<=1 */
	positivity = true;
      }
      else {
	change = true;
	numint_set_int(C->p[i][polka_eps],0);
      }
    }
  }
  assert(change || positivity);
  if (change){
    if (!positivity){
      numint_t* q;
      size_t nbrows;
      /* we should add it */
      nbrows = C->nbrows;
      pk_matrix_resize_rows_lazy(C,C->nbrows+1);
      q = C->p[nbrows];
      numint_set_int(q[0],1);
      numint_set_int(q[polka_cst],1);
      numint_set_int(q[polka_eps],-1);
      for (i=3; i<C->nbcolumns; i++) numint_set_int(q[i],0);
    }
    C->_sorted = false;
    if (destructive){
      if (po->F) pk_matrix_free(po->F);
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
  assert(poly_check(pk,po));
  return po;
}
