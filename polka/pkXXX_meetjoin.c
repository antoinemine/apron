/* ********************************************************************** */
/* pkXXX_meetjoin.c: Meet and join operations */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "ap_linconsXXX.h"
#include "ap_generic.h"

/* ********************************************************************** */
/* I. Meet/Join */
/* ********************************************************************** */

/* In all the following functions, for the structure poly_t:

   - If meet is true, standard meaning of everything
   - If meet is false,
   - matrices C and F, satC and satF have been exchanged,
   - nbeq and nbline have been exchanged
   - in status, nothing has changed
*/

/* ====================================================================== */
/* I.1 Meet/Join of a (prepared) polyhedron with a sorted matrix of
   constraints/generators */
/* ====================================================================== */

/* The polyhedron is supposed:
   - to have constraints,
   - if lazy is false, to be minimized, and to have satC
   - possible emptiness not detected,

   Matrix of constraints is supposed:
   - to be sorted
   - to be canonical too.

   Return true if exception
*/
bool pkXXX_meet_matrix(bool meet,
		      ap_manager_t* man,
		      pkXXX_t* po,
		      pkXXX_t* pa, matrixXXX_t* mat)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  assert(mat->_sorted);

  man->result.flag_best = (pa->dim.intd==0);
  man->result.flag_exact = meet;

  /* lazy behaviour */
  if (pk->option.op_lazy){
    pkXXX_obtain_sorted_C(pk,pa);
    if (po != pa){
      po->C = matrixXXX_merge_sort(pk,pa->C,mat);
    }
    else {
      matrixXXX_merge_sort_with(pk,pa->C,mat);
      if (pa->F){ matrixXXX_free(pa->F); pa->F=NULL; }
      if (pa->satC){ satmat_free(pa->satC); pa->satC=NULL; }
      if (pa->satF){ satmat_free(pa->satF); pa->satF=NULL; }
      pa->nbeq = pa->nbline = 0;
    }
    po->status = 0;
  }
  /* strict behaviour */
  else {
    size_t start = pa->C->nbrows;
    assert(pa->satC);
    if (po != pa){
      po->C = matrixXXX_append(pa->C,mat);
      po->F = matrixXXX_copy(pa->F);
      po->satC = satmat_copy_resize_cols(pa->satC,
					 bitindex_size(po->C->nbrows));
      po->nbline = pa->nbline;
      po->nbeq = pa->nbeq;
    }
    else {
      matrixXXX_append_with(pa->C,mat);
      satmat_resize_cols(pa->satC,
			 bitindex_size(pa->C->nbrows));
    }
    cherniXXX_add_and_minimize(pk,meet,po,start);
    if (pk->exn) goto _pkXXX_meet_matrixXXX_exit0;
    po->status =
      meet
      ?
      ( pk_status_consgauss )
      :
      ( pk_status_gengauss )
      ;
    assert( pkXXX_check_dual(pk,po,meet) );
  }
  return false;
 _pkXXX_meet_matrixXXX_exit0:
  po->status = 0;
  {
    char str[160];
    sprintf(str,"conversion from %s of the (intermediate) result\n",
	    (meet ? po->C : po->F) ? "constraints to generators" : "generators to constraints");
    ap_manager_raise_exception(man,pk->exn,pk->funid,str);
  }
  return true;
}

/* ====================================================================== */
/* I.2 Meet/Join of two polyhedra, functional and side-effect versions */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Factorized form */
/* ---------------------------------------------------------------------- */
static
bool pkXXX_meet_particularcases(bool meet,
				ap_manager_t* man,
				pkXXX_t* po, pkXXX_t* pa, pkXXX_t* pb)
{
  assert(pa!=pb);

  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  man->result.flag_exact = true;
  if (meet){
    /* Meet */
    /* if one is bottom, return bottom */
    if ( (!pa->C && !pa->F) || (!pb->C && !pb->F) ){
      pkXXX_set_bottom(pk,po);
      return true;
    }
  }
  else {
    /* Join */
    /* if one is bottom, return a copy of the other */
    if (!pa->C && !pa->F){
      if (!pk->option.op_lazy){
	pkXXX_chernikova_dual(man,pb,"of the second argument",false);
	pk->exn = AP_EXC_NONE;
      }
      pkXXX_set(po,pb);
      return true;
    }
    if (!pb->C && !pb->F){
      if (!pk->option.op_lazy){
	pkXXX_chernikova_dual(man,pa,"of the first argument",false);
	pk->exn = AP_EXC_NONE;
      }
      pkXXX_set(po,pa);
      return true;
    }
    /* if one want information about exactness, also test inclusion */
    if (pk->funopt->flag_exact_wanted){
      pkXXX_dual(pa);
      pkXXX_dual(pb);
      if (pkXXX_is_leq(man,pa,pb)){
	pkXXX_set(po,pb);
	goto _pkXXX_meet_particularcases_exit;
      }
      else if (pkXXX_is_leq(man,pb,pa)){
	pkXXX_set(po,pa);
      _pkXXX_meet_particularcases_exit:
	pkXXX_dual(pa);
	pkXXX_dual(pb);
	if (po!=pa) pkXXX_dual(po);
	return true;
      }
    }
  }
  man->result.flag_exact = false;
  return false;
}

void pkXXX_meetjoin(bool meet,
		    ap_manager_t* man,
		    pkXXX_t* po, pkXXX_t* pa, pkXXX_t* pb)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  int pa_status;

  man->result.flag_best = true;

  if (pa==pb){
    if (!pk->option.op_lazy) pkXXX_chernikova_dual(man,pa,"of the first argument", meet);
    pk->exn = AP_EXC_NONE;
    pkXXX_set(po,pa);
    man->result.flag_exact = true;
    return;
  }

  pa_status = pa->status;
  /* Set the dimensions of po */
  if (po!=pa){
    po->dim.intd = pa->dim.intd;
    po->dim.reald = pa->dim.reald;
    assert(!po->C && !po->F && !po->satC && !po->satF);
  }
  /* Particular cases */
  if (pkXXX_meet_particularcases(meet,man,po,pa,pb))
    return;

  /* Get the constraint system of pa */
  pkXXX_obtain_C_dual(man,pa,"of the first argument",meet);
  if (pk->exn){
    assert(!pa->C);
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    if (meet) pkXXX_set(po,pb);
    else { pkXXX_set_top(pk,po); pkXXX_dual(po); }
    return;
  }
  /* Get the constraint system of pb */
  pkXXX_obtain_C_dual(man,pb,"of the second argument",meet);
  if (pk->exn){
    assert(!pb->C);
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    if (meet) pkXXX_set(po,pa);
    else { pkXXX_set_top(pk,po); pkXXX_dual(po); }
    return;
  }
  /* Particular cases again */
  if (pkXXX_meet_particularcases(meet,man,po,pa,pb))
    return;

  /* lazy behaviour */
  if (pk->option.op_lazy){
  _pkXXX_meet_entry0:
    pkXXX_obtain_sorted_C(pk,pa);
    pkXXX_obtain_sorted_C(pk,pb);
    pkXXX_meet_matrix(meet,man,po,pa,pb->C);
  }
  /* strict behaviour */
  else {
    if (po != pa){
      int start = 0;

      /* choice of the starting polyhedron */
      if (pa->F && !pb->F) /* only pa is minimized */
	start=1;
      else if (pb->F && !pa->F) /* only pb is minimized */
	start=2;
      else { /* either both or none are minimized */
	if (!pa->F && !pb->F){
	  /* ensure minimization */
	  pkXXX_chernikova_dual(man,pa,"of the first argument",meet);
	  if (pk->exn){
	    assert(pa->C);
	    pk->exn = AP_EXC_NONE;
	    start = 2;
	  }
	  pkXXX_chernikova_dual(man,pb,"of the second argument",meet);
	  if (pk->exn){
	    assert(pb->C);
	    pk->exn = AP_EXC_NONE;
	    if (start==2){
	      goto _pkXXX_meet_entry0;
	    }
	    else {
	      start = 1;
	    }
	  }
	  /* Particular cases */
	  if (pkXXX_meet_particularcases(meet,man,po,pa,pb))
	    return;
	}
	/* Perform the choice */
	if (pa->F && pb->F){
	  if ( pa->nbeq > pb->nbeq
	       || (pa->nbeq == pb->nbeq &&
		   (pa->nbline < pb->nbline || pa->C->nbrows >= pb->C->nbrows) ) )
	    start = 1;
	  else
	    start = 2;
	}
      }
      if (start==2){
	pkXXX_t* p = pa; pa=pb; pb=p;
      }
    }
    else {
      /* ensure minimization of pa */
      pkXXX_chernikova_dual(man,pa,"of the first argument",meet);
      if (pk->exn){
	assert(pa->C);
	pk->exn = AP_EXC_NONE;
	goto _pkXXX_meet_entry0;
      }
    }
    /* Now, pa is the start polyhedron */
    pkXXX_obtain_satC(pa);
    pkXXX_obtain_sorted_C(pk,pb);
    pkXXX_meet_matrix(meet,man,po,pa,pb->C);
  }
  assert(pkXXX_check_dual(pk,po,meet));
}

/* ====================================================================== */
/* I.3 Meet/Join array */
/* ====================================================================== */

static
pkXXX_t* pkXXX_meetjoin_array(bool meet,
			      ap_manager_t* man,
			      pkXXX_t** po, size_t size)
{
  ap_dimension_t dim;
  pkXXX_t* poly;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  man->result.flag_best = true;

  /* 1. Special cases */
  if (size==0){
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       pk->funid, "empty array");
    man->result.flag_best = man->result.flag_exact = false;
    poly = pkXXX_top(man,ap_dimension_make(0,1));
    if (!meet) pkXXX_dual(poly);
    return poly;
  }
  dim = po[0]->dim;
  poly = pkXXX_alloc(dim);
  if (size==1){
    if (!pk->option.op_lazy){
      pkXXX_chernikova_dual(man,po[0],"of the single argument",meet);
      pk->exn = AP_EXC_NONE;
    }
    pkXXX_set(poly,po[0]);
    return poly;
  }
  else if (size==2){
    pkXXX_meetjoin(meet,man,poly,po[0],po[1]);
    return poly;
  }
  /* 2. General case */
  else {
    matrixXXX_t* C;
    size_t nbrows;
    size_t i,j;

    man->result.flag_best = true;
    man->result.flag_exact = meet;

    /* Count the total number of constraints */
    nbrows = 0;
    i = 0;
    while (i<size){
      char str[80];
      sprintf(str,"of the %lu argument",(unsigned long)i);
      if (pk->option.op_lazy)
	pkXXX_obtain_C_dual(man,po[i],str,meet);
      else
	pkXXX_chernikova_dual(man,po[i],str,meet);
      if (pk->exn){
	pk->exn = AP_EXC_NONE;
	if (!po[i]->C){
	  man->result.flag_best = man->result.flag_exact = false;
	  pkXXX_set_top(pk, poly);
	  if (!meet) pkXXX_dual(poly);
	  return poly;
	}
      }
      if (!po[i]->C){
	/* one polyhedron is empty */
	if (meet){
	  /* We return with bottom */
	  pkXXX_set_bottom(pk,poly);
	  return poly;
	}
	else {
	  /* We exchange po[i] and po[size-1] */
	  size--;
	  if (i<size){
	    pkXXX_t* tmp = po[i]; po[i] = po[size]; po[size] = tmp;
	  }
	}
      }
      else {
	nbrows += po[i]->C->nbrows;
	i++;
      }
    }
    /* if size has been decreased */
    if (size<=2){
      assert(!meet);
      if (size==0){
	man->result.flag_exact = true;
	pkXXX_set_bottom(pk,poly);
	pkXXX_dual(poly);
      }
      else if (size==1){
	man->result.flag_exact = true;
	pkXXX_set(poly,po[0]);
      }
      else if (size==2){
	pkXXX_meetjoin(meet,man,poly,po[0],po[1]);
      }
      return poly;
    }
    /* 2.1. lazy behaviour */
    if (pk->option.op_lazy){
      C = matrixXXX_alloc(nbrows,pk->dec+dim.intd+dim.reald,true);
      C->nbrows = 0;
      C->_sorted = true;
      for (i=0; i<size; i++){
	if (po[i]->C){
	  pkXXX_obtain_sorted_C(pk,po[i]);
	  matrixXXX_merge_sort_with(pk,C,po[i]->C);
	}
      }
      poly->C = C;
      poly->status = 0;
    }
    /* 2.2 strict hehaviour */
    else {
      /* Minimizing and selecting the start polyhedron */
      j = 0; /* The selected start polyhedron */
      for (i=1; i<size; i++){
	assert(po[i]->C && po[i]->F);
	if (po[i]->nbeq > po[j]->nbeq ||
	    (po[i]->nbeq == po[j]->nbeq &&
	     (po[i]->nbline < po[j]->nbline ||
	      po[i]->C->nbrows > po[j]->C->nbrows)))
	  j=i;
      }
      /* Add the other polyehdra to the polyhedra of index j */
      C = matrixXXX_alloc(nbrows, pk->dec+dim.intd+dim.reald,true);
      C->nbrows = 0;
      C->_sorted = true;
      for (i=0; i<size; i++){
	if (i!=j){
	  pkXXX_obtain_sorted_C(pk,po[i]);
	  matrixXXX_merge_sort_with(pk,C,po[i]->C);
	}
      }
      matrixXXX_revappend_with(C,po[j]->C);
      C->_sorted = false;
      poly->C = C;
      poly->F = matrixXXX_copy(po[j]->F);
      pkXXX_obtain_satC(po[j]);
      poly->satC = satmat_copy_resize_cols(po[j]->satC,
					   bitindex_size(C->nbrows));
      poly->nbeq = po[j]->nbeq;
      poly->nbline = po[j]->nbline;
      cherniXXX_add_and_minimize(pk,meet,poly,po[j]->C->nbrows);
      if (pk->exn) goto _pkXXX_meetjoin_exit0;
      poly->status =
	meet ?
	( pk_status_consgauss ) :
	( pk_status_gengauss ) ;
    }
    assert(pkXXX_check_dual(pk,poly,meet));
    return poly;
  _pkXXX_meetjoin_exit0:
    poly->status = 0;
    {
      char str[160];
      sprintf(str,"conversion from %s of the (intermediate) result\n",
	      (meet ? poly->C : poly->F) ? "constraints to generators" : "generators to constraints");
      ap_manager_raise_exception(man,pk->exn,pk->funid,str);
    }
    pk->exn = AP_EXC_NONE;
    return poly;
  }
}

/* ********************************************************************** */
/* II. Meet */
/* ********************************************************************** */

/* ********************************************************************** */
/* II.1 Meet of two or more polyhedra */
/* ********************************************************************** */

pkXXX_t* pkXXX_meet(ap_manager_t* man,
		    bool destructive, pkXXX_t* pa, pkXXX_t* pb)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET);
  pkXXX_t* po = destructive ? pa : pkXXX_alloc(pa->dim);
  pkXXX_meetjoin(true, man,po,pa,pb);
  assert(pkXXX_check(pk,po));
  return po;
}

pkXXX_t* pkXXX_meet_array(ap_manager_t* man,
			  pkXXX_t** po, size_t size)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET_ARRAY);
 pkXXX_t* res = pkXXX_meetjoin_array(true,man,po,size);
  assert(pkXXX_check(pk,res));
  return res;
}

/* ====================================================================== */
/* II.2 Meet with (array of) linear constraint(s) */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Factorized version */
pkXXX_t* pkXXX_meet_lincons_array_linear(ap_manager_t* man,
					 bool destructive,
					 pkXXX_t* pa,
					 ap_lincons0_array_t array)
{
  matrixXXX_t* mat;
  bool quasilinear;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY);

  pkXXX_t* po = destructive ? pa : pkXXX_alloc(pa->dim);
  /* Get the constraint systems */
  if (pk->option.op_lazy){
    pkXXX_obtain_C(man,pa,"of the argument");
  } else {
    pkXXX_chernikova(man,pa,"of the argument");
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->C){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return po;
    }
  }
  /* if pa is bottom, return bottom */
  if ( !pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    pkXXX_set(po,pa);
    return po;
  }

  if (array->discr!=AP_SCALAR_MPQ) abort();
  bool exact = matrixXXX_set_linconsMPQ_array(pk,&mat,array->lincons_array.MPQ,po->dim,true);
  matrixXXX_sort_rows(pk,mat);
  if (!pk->option.op_lazy) pkXXX_obtain_satC(pa);
  pkXXX_meet_matrix(true,man,po,pa,mat);
  matrixXXX_free(mat);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
  }
  else {
    man->result.flag_best = man->result.flag_exact = exact ? true : false;
  }
  assert(pkXXX_check(pk,po));
  return po;
}

pkXXX_t* pkXXX_meet_lincons_array(ap_manager_t* man, bool destructive, pkXXX_t* pa, ap_lincons0_array_t array)
{
  return
    ap_generic_meet_quasilinearize_lincons_array(
	man,destructive,pa,array,
	AP_SCALAR_MPQ, AP_LINEXPR_LINEAR,
	AP_LINEXPR_INTLINEAR,1,
	false,false,
	(void*(*)(ap_manager_t*,bool,void*,ap_lincons0_array_t))pkXXX_meet_lincons_array_linear
    );
}

pkXXX_t* pkXXX_meet_tcons_array(ap_manager_t* man, bool destructive, pkXXX_t* pa, ap_tcons0_array_t* array)
{
  return
    ap_generic_meet_intlinearize_tcons_array(
	man,destructive,pa,array,
	AP_SCALAR_MPQ, AP_LINEXPR_LINEAR,
	AP_LINEXPR_INTLINEAR,1,
	false,false,
	(void*(*)(ap_manager_t*,bool,void*,ap_lincons0_array_t))pkXXX_meet_lincons_array_linear
    );
}

/* ********************************************************************** */
/* III. Join */
/* ********************************************************************** */

/* ====================================================================== */
/* III.1 Join of two or more polyhedra, functional and side-effect versions */
/* ====================================================================== */

pkXXX_t* pkXXX_join(ap_manager_t* man, bool destructive, pkXXX_t* pa, pkXXX_t* pb)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_JOIN);
  pkXXX_t* po = destructive ? pa : pkXXX_alloc(pa->dim);

  pkXXX_dual(pa);
  if (pb!=pa) pkXXX_dual(pb); /* We take care of possible alias */
  pkXXX_meetjoin(false,man,po,pa,pb);
  pkXXX_dual(pa);
  if (pb!=pa) pkXXX_dual(pb); /* We take care of possible alias */
  if (po!=pa) pkXXX_dual(po);
  return po;
}

static int pkXXX_cmp(const void* a, const
void* b)
{
  pkXXX_t* pa = *((pkXXX_t*const*)a);
  pkXXX_t* pb = *((pkXXX_t*const*)b);
  return (pa>pb ? 1 : (pa==pb ? 0 : -1));
}

pkXXX_t* pkXXX_join_array(ap_manager_t* man, pkXXX_t** po, size_t size)
{
  pkXXX_t** tpoly;
  pkXXX_t* poly;
  size_t i;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_JOIN_ARRAY);

  if (size==0){
    ap_manager_raise_exception(man,
			       AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_JOIN_ARRAY, "empty array");
    man->result.flag_best = man->result.flag_exact = false;
    poly = pkXXX_top(man,ap_dimension_make(0,1));
    return poly;
  }
  else if (size==1){
    man->result.flag_best = man->result.flag_exact = true;
    poly = pkXXX_copy(man,po[0]);
    return poly;
  }
  /* We have to take care of possible aliases in the array of polyhedra */
  tpoly = malloc(size*sizeof(pkXXX_t*));
  memcpy(tpoly, po, size*sizeof(pkXXX_t*));
  qsort(tpoly,size,sizeof(pkXXX_t*),pkXXX_cmp);

  /* remove doublons */
  for(i=0;i<size-1;i++){
    if (tpoly[i]==tpoly[i+1]){
      if (i<size-2){
	memmove(&tpoly[i+1],&tpoly[i+2],(size-i-2)*sizeof(pkXXX_t*));
      }
      size--;
    }
  }
  /* dual */
  for (i=0;i<size;i++)
    pkXXX_dual(tpoly[i]);

  poly = pkXXX_meetjoin_array(false,man,(pkXXX_t**)tpoly,size);
  for(i=0;i<size;i++){
    pkXXX_dual(tpoly[i]);
  }
  free(tpoly);
  pkXXX_dual(poly);
  assert(pkXXX_check(pk,poly));
  return poly;
}

/* ====================================================================== */
/* III.2 Join with array of rays */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Factorized version */
static
void poly_add_ray_array(ap_manager_t* man,
			pkXXX_t* po, pkXXX_t* pa, ap_lingen0_array_t array)
{
  bool exact;
  matrixXXX_t* mat;

  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  man->result.flag_best = man->result.flag_exact = true;

  /* Get the generator systems */
  if (pk->option.op_lazy){
    pkXXX_obtain_F(man,pa,"of the argument");
  } else {
    pkXXX_chernikova(man,pa,"of the argument");
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      pkXXX_set_top(pk,po);
      return;
    }
  }
  /* if pa is bottom, return bottom */
  if ( !pa->C && !pa->F){
    pkXXX_set(po,pa);
    return;
  }
  exact = matrixXXX_set_ap_lingen0_array(pk,&mat,array,pa->dim);
  matrixXXX_sort_rows(pk,mat);

  if (!pk->option.op_lazy) pkXXX_obtain_satF(pa);
  pkXXX_dual(po);
  if (po!=pa) pkXXX_dual(pa);
  pkXXX_meet_matrix(false,man,po,pa,mat);
  pkXXX_dual(po);
  if (po!=pa) pkXXX_dual(pa);
  matrixXXX_free(mat);
  man->result.flag_exact = exact;
}

pkXXX_t* pkXXX_add_ray_array(ap_manager_t* man, bool destructive, pkXXX_t* pa, ap_lingen0_array_t array)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY);
  pkXXX_t* po = destructive ? pa : pkXXX_alloc(pa->dim);
  poly_add_ray_array(man,po,pa,array);
  assert(pkXXX_check(pk,po));
  return po;
}
