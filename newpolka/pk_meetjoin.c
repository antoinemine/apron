/* ********************************************************************** */
/* pk_meetjoin.c: Meet and join operations */
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
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_test.h"
#include "pk_extract.h"
#include "pk_cherni.h"
#include "pk_meetjoin.h"
#include "ap_generic.h"
#include "itv_linearize.h"

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
bool poly_meet_matrix(bool meet,
		      bool lazy,
		      ap_manager_t* man,
		      pk_t* po,
		      pk_t* pa, pk_matrix_t* mat)
{
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  assert(mat->_sorted);

  man->result.flag_best = (pa->intdim==0);
  man->result.flag_exact = meet;

  /* lazy behaviour */
  if (lazy){
    poly_obtain_sorted_C(pk,pa);
    if (po != pa){
      po->C = pk_matrix_merge_sort(pk,pa->C,mat);
    }
    else {
       pk_matrix_merge_sort_with(pk,pa->C,mat);
       if (pa->F){ pk_matrix_free(pa->F); pa->F=NULL; }
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
      po->C = pk_matrix_append(pa->C,mat);
      po->F = pk_matrix_copy(pa->F);
      po->satC = satmat_copy_resize_cols(pa->satC,
					 bitindex_size(po->C->nbrows));
      po->nbline = pa->nbline;
      po->nbeq = pa->nbeq;
    }
    else {
      pk_matrix_append_with(pa->C,mat);
      satmat_resize_cols(pa->satC,
			 bitindex_size(pa->C->nbrows));
    }
    cherni_add_and_minimize(pk,meet,po,start);
    if (pk->exn) goto _poly_meet_matrix_exit0;
    po->status =
      meet
      ?
      ( pk_status_consgauss )
      :
      ( pk_status_gengauss )
      ;
    assert( poly_check_dual(pk,po,meet) );
  }
  return false;
 _poly_meet_matrix_exit0:
  po->status = 0;
  {
    char str[160];
    snprintf(str,sizeof(str),"conversion from %s of the (intermediate) result\n",
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
bool poly_meet_particularcases(bool meet, bool lazy,
			       ap_manager_t* man,
			       pk_t* po, pk_t* pa, pk_t* pb)
{
  assert(pa!=pb);

  pk_internal_t* pk = (pk_internal_t*)man->internal;
  man->result.flag_exact = true;
  if (meet){
    /* Meet */
    /* if one is bottom, return bottom */
    if ( (!pa->C && !pa->F) || (!pb->C && !pb->F) ){
      poly_set_bottom(pk,po);
      return true;
    }
  }
  else {
    /* Join */
    /* if one is bottom, return a copy of the other */
    if (!pa->C && !pa->F){
      if (!lazy){
	poly_chernikova_dual(man,pb,"of the second argument",false);
	pk->exn = AP_EXC_NONE;
      }
      poly_set(po,pb);
      return true;
    }
    if (!pb->C && !pb->F){
      if (!lazy){
	poly_chernikova_dual(man,pa,"of the first argument",false);
	pk->exn = AP_EXC_NONE;
      }
      poly_set(po,pa);
      return true;
    }
    /* if one want information about exactness, also test inclusion */
    if (pk->funopt->flag_exact_wanted){
      bool res1=false,res2=false;
      poly_dual(pa);
      poly_dual(pb);
      res1 = pk_is_leq(man,pa,pb);
      res2 = res1 ? false : pk_is_leq(man,pb,pa);
      poly_dual(pa);
      poly_dual(pb);
      if (res1){
	poly_set(po,pb);
        return true;
      }
      else if (res2){
        poly_set(po,pa);
        return true;
      }
    }
  }
  man->result.flag_exact = false;
  return false;
}

void poly_meet(bool meet,
	       bool lazy,
	       ap_manager_t* man,
	       pk_t* po, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  int pa_status;

  man->result.flag_best = true;

  if (pa==pb){
    if (!lazy) poly_chernikova_dual(man,pa,"of the first argument", meet);
    pk->exn = AP_EXC_NONE;
    poly_set(po,pa);
    man->result.flag_exact = true;
    return;
  }

  pa_status = pa->status;
  /* Set the dimensions of po */
  if (po!=pa){
    po->intdim = pa->intdim;
    po->realdim = pa->realdim;
    assert(!po->C && !po->F && !po->satC && !po->satF);
  }
  /* Particular cases */
  if (poly_meet_particularcases(meet,lazy,man,po,pa,pb))
    return;

  /* Get the constraint system of pa */
  poly_obtain_C_dual(man,pa,"of the first argument",meet);
  if (pk->exn){
    assert(!pa->C);
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    if (meet) poly_set(po,pb);
    else { poly_set_top(pk,po); poly_dual(po); }
    return;
  }
  /* Get the constraint system of pb */
  poly_obtain_C_dual(man,pb,"of the second argument",meet);
  if (pk->exn){
    assert(!pb->C);
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    if (meet) poly_set(po,pa);
    else { poly_set_top(pk,po); poly_dual(po); }
    return;
  }
  /* Particular cases again */
  if (poly_meet_particularcases(meet,lazy,man,po,pa,pb))
    return;

  /* lazy behaviour */
  if (lazy){
  _poly_meet_entry0:
    poly_obtain_sorted_C(pk,pa);
    poly_obtain_sorted_C(pk,pb);
    poly_meet_matrix(meet,lazy,man,po,pa,pb->C);
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
	  poly_chernikova_dual(man,pa,"of the first argument",meet);
	  if (pk->exn){
	    assert(pa->C);
	    pk->exn = AP_EXC_NONE;
	    start = 2;
	  }
	  poly_chernikova_dual(man,pb,"of the second argument",meet);
	  if (pk->exn){
	    assert(pb->C);
	    pk->exn = AP_EXC_NONE;
	    if (start==2){
	      goto _poly_meet_entry0;
	    }
	    else {
	      start = 1;
	    }
	  }
	  /* Particular cases */
	  if (poly_meet_particularcases(meet,lazy,man,po,pa,pb))
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
	pk_t* p = pa; pa=pb; pb=p;
      }
    }
    else {
      /* ensure minimization of pa */
      poly_chernikova_dual(man,pa,"of the first argument",meet);
      if (pk->exn){
	assert(pa->C);
	pk->exn = AP_EXC_NONE;
	goto _poly_meet_entry0;
      }
    }
    /* Now, pa is the start polyhedron */
    poly_obtain_satC(pa);
    poly_obtain_sorted_C(pk,pb);
    poly_meet_matrix(meet,lazy,man,po,pa,pb->C);
  }
  assert(poly_check_dual(pk,po,meet));
}

/* ====================================================================== */
/* I.3 Meet/Join array */
/* ====================================================================== */

static
pk_t* poly_meet_array(bool meet,
		      bool lazy,
		      ap_manager_t* man,
		      pk_t** po, size_t size)
{
  size_t intdim,realdim;
  pk_t* poly;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  man->result.flag_best = true;

  /* 1. Special cases */
  if (size==0){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    pk->funid, "empty array");
    man->result.flag_best = man->result.flag_exact = false;
    poly = pk_top(man,0,1);
    if (!meet) poly_dual(poly);
    return poly;
  }
  intdim = po[0]->intdim;
  realdim = po[0]->realdim;
  poly = poly_alloc(intdim,realdim);
  if (size==1){
    if (!lazy){
      poly_chernikova_dual(man,po[0],"of the single argument",meet);
      pk->exn = AP_EXC_NONE;
    }
    poly_set(poly,po[0]);
    return poly;
  }
  else if (size==2){
    poly_meet(meet,lazy,man,poly,po[0],po[1]);
    return poly;
  }
  /* 2. General case */
  else {
    pk_matrix_t* C;
    size_t nbrows;
    size_t i,j;

    man->result.flag_best = true;
    man->result.flag_exact = meet;

    /* Count the total number of constraints */
    nbrows = 0;
    i = 0;
    while (i<size){
      char str[80];
      snprintf(str,sizeof(str),"of the %lu argument",(unsigned long)i);
      if (lazy)
	poly_obtain_C_dual(man,po[i],str,meet);
      else
	poly_chernikova_dual(man,po[i],str,meet);
      if (pk->exn){
	pk->exn = AP_EXC_NONE;
	if (!po[i]->C){
	  man->result.flag_best = man->result.flag_exact = false;
	  poly_set_top(pk, poly);
	  if (!meet) poly_dual(poly);
	  return poly;
	}
      }
      if (!po[i]->C){
	/* one polyhedron is empty */
	if (meet){
	  /* We return with bottom */
	  poly_set_bottom(pk,poly);
	  return poly;
	}
	else {
	  /* We exchange po[i] and po[size-1] */
	  size--;
	  if (i<size){
	    pk_t* tmp = po[i]; po[i] = po[size]; po[size] = tmp;
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
	poly_set_bottom(pk,poly);
	poly_dual(poly);
      }
      else if (size==1){
	man->result.flag_exact = true;
	poly_set(poly,po[0]);
      }
      else if (size==2){
	poly_meet(meet,lazy,man,poly,po[0],po[1]);
      }
      return poly;
    }
    /* 2.1. lazy behaviour */
    if (lazy){
      C = pk_matrix_alloc(nbrows,pk->dec+intdim+realdim,true);
      C->nbrows = 0;
      C->_sorted = true;
      for (i=0; i<size; i++){
	if (po[i]->C){
	  poly_obtain_sorted_C(pk,po[i]);
	  pk_matrix_merge_sort_with(pk,C,po[i]->C);
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
      C = pk_matrix_alloc(nbrows, pk->dec+intdim+realdim,true);
      C->nbrows = 0;
      C->_sorted = true;
      for (i=0; i<size; i++){
	if (i!=j){
	  poly_obtain_sorted_C(pk,po[i]);
	  pk_matrix_merge_sort_with(pk,C,po[i]->C);
	}
      }
      pk_matrix_revappend_with(C,po[j]->C);
      C->_sorted = false;
      poly->C = C;
      poly->F = pk_matrix_copy(po[j]->F);
      poly_obtain_satC(po[j]);
      poly->satC = satmat_copy_resize_cols(po[j]->satC,
					   bitindex_size(C->nbrows));
      poly->nbeq = po[j]->nbeq;
      poly->nbline = po[j]->nbline;
      cherni_add_and_minimize(pk,meet,poly,po[j]->C->nbrows);
      if (pk->exn) goto _poly_meet_array_exit0;
      poly->status =
	meet ?
	( pk_status_consgauss ) :
	( pk_status_gengauss ) ;
    }
    assert(poly_check_dual(pk,poly,meet));
    return poly;
  _poly_meet_array_exit0:
    poly->status = 0;
    {
      char str[160];
      snprintf(str,sizeof(str),"conversion from %s of the (intermediate) result\n",
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

pk_t* pk_meet(ap_manager_t* man,
	      bool destructive, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET);
  pk_t* po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);
  poly_meet(true, pk->funopt->algorithm < 0,
	    man,po,pa,pb);
  assert(poly_check(pk,po));
  return po;
}

pk_t* pk_meet_array(ap_manager_t* man,
		    pk_t** po, size_t size)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET_ARRAY);
  pk_t* res = poly_meet_array(true, pk->funopt->algorithm < 0,
			      man,po,size);
  assert(poly_check(pk,res));
  return res;
}

/* ====================================================================== */
/* II.2 Meet with (array of) linear constraint(s) */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Factorized version */
void poly_meet_itv_lincons_array(bool lazy,
				 ap_manager_t* man,
				 pk_t* po, pk_t* pa,
				 itv_lincons_array_t* array)
{
  pk_matrix_t* mat;
  bool quasilinear;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  quasilinear = itv_lincons_array_is_quasilinear(array);

  /* Get the constraint systems */
  if (lazy && quasilinear){
    poly_obtain_C(man,pa,"of the argument");
  } else {
    poly_chernikova(man,pa,"of the argument");
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->C){
      man->result.flag_best = man->result.flag_exact = false;
      poly_set_top(pk,po);
      return;
    }
  }
  /* if pa is bottom, return bottom */
  if ( !pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    poly_set(po,pa);
    return;
  }

  /* quasilinearize if needed */
  if (!quasilinear){
    itv_t* env = pk_matrix_to_box(pk,pa->F);
    itv_quasilinearize_lincons_array(pk->itv,array,env,true);
    itv_array_free(env,pa->intdim+pa->realdim);
  }
  itv_linearize_lincons_array(pk->itv,array,true);
  itv_lincons_array_reduce_integer(pk->itv,array,po->intdim);
  bool exact = pk_matrix_set_itv_lincons_array(pk,&mat,array,po->intdim,po->realdim,true);
  pk_matrix_sort_rows(pk,mat);
  if (!lazy) poly_obtain_satC(pa);
  poly_meet_matrix(true,lazy,man,po,pa,mat);
  pk_matrix_free(mat);
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
  }
  else {
    man->result.flag_best = man->result.flag_exact = exact ? true : false;
  }
}

void* pk_meet_lincons_array(ap_manager_t* man, bool destructive, void* ppa, ap_lincons0_array_t* array)
{
  itv_lincons_array_t tcons;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY);
  pk_t* pa = (pk_t*) ppa;
  pk_t* po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);

  itv_lincons_array_init(&tcons,array->size);
  itv_lincons_array_set_ap_lincons0_array(pk->itv,&tcons,array);
  poly_meet_itv_lincons_array(pk->funopt->algorithm<0,
			      man,po,pa,&tcons);
  itv_lincons_array_clear(&tcons);
  assert(poly_check(pk,po));
  return (void*)po;
}

pk_t* pk_meet_tcons_array(ap_manager_t* man, bool destructive, pk_t* pa, ap_tcons0_array_t* array)
{
  return ap_generic_meet_intlinearize_tcons_array(man,destructive,pa,array,
						  AP_SCALAR_MPQ, AP_LINEXPR_LINEAR,
						  &pk_meet_lincons_array);
}

/* ********************************************************************** */
/* III. Join */
/* ********************************************************************** */

/* ====================================================================== */
/* III.1 Join of two or more polyhedra, functional and side-effect versions */
/* ====================================================================== */

pk_t* pk_join(ap_manager_t* man, bool destructive, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_JOIN);
  pk_t* po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);

  poly_dual(pa);
  if (pb!=pa) poly_dual(pb); /* We take care of possible alias */
  poly_meet(false,pk->funopt->algorithm<0,
	    man,po,pa,pb);
  poly_dual(pa);
  if (pb!=pa) poly_dual(pb); /* We take care of possible alias */
  if (po!=pa) poly_dual(po);
  return po;
}

static int poly_cmp(const void* a, const void* b)
{
  const pk_t* pa = *((const pk_t*const*)a);
  const pk_t* pb = *((const pk_t*const*)b);
  return (pa>pb ? 1 : (pa==pb ? 0 : -1));
}

pk_t* pk_join_array(ap_manager_t* man, pk_t** po, size_t size)
{
  pk_t** tpoly;
  pk_t* poly;
  size_t i;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_JOIN_ARRAY);

  if (size==0){
    ap_manager_raise_exception(man,
			    AP_EXC_INVALID_ARGUMENT,
			    AP_FUNID_JOIN_ARRAY, "empty array");
    man->result.flag_best = man->result.flag_exact = false;
    poly = pk_top(man,0,1);
    return poly;
  }
  else if (size==1){
    man->result.flag_best = man->result.flag_exact = true;
    poly = pk_copy(man,po[0]);
    return poly;
  }
  /* We have to take care of possible aliases in the array of polyhedra */
  tpoly = malloc(size*sizeof(pk_t*));
  memcpy(tpoly, po, size*sizeof(pk_t*));
  qsort(tpoly,size,sizeof(pk_t*),poly_cmp);

  /* remove doublons */
  for(i=0;i<size-1;i++){
    if (tpoly[i]==tpoly[i+1]){
      if (i<size-2){
	memmove(&tpoly[i+1],&tpoly[i+2],(size-i-2)*sizeof(pk_t*));
      }
      size--;
    }
  }
  /* dual */
  for (i=0;i<size;i++)
    poly_dual(tpoly[i]);

  poly = poly_meet_array(false,pk->funopt->algorithm<0,
			 man,(pk_t**)tpoly,size);
  for(i=0;i<size;i++){
    poly_dual(tpoly[i]);
  }
  free(tpoly);
  poly_dual(poly);
  assert(poly_check(pk,poly));
  return poly;
}

/* ====================================================================== */
/* III.2 Join with array of rays */
/* ====================================================================== */

/* ---------------------------------------------------------------------- */
/* Factorized version */
static
void poly_add_ray_array(bool lazy,
			ap_manager_t* man,
			pk_t* po, pk_t* pa, ap_generator0_array_t* array)
{
  bool exact;
  pk_matrix_t* mat;

  pk_internal_t* pk = (pk_internal_t*)man->internal;

  man->result.flag_best = man->result.flag_exact = true;

  /* Get the generator systems */
  if (lazy){
    poly_obtain_F(man,pa,"of the argument");
  } else {
    poly_chernikova(man,pa,"of the argument");
  }
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    if (!pa->F){
      man->result.flag_best = man->result.flag_exact = false;
      poly_set_top(pk,po);
      return;
    }
  }
  /* if pa is bottom, return bottom */
  if ( !pa->C && !pa->F){
    poly_set(po,pa);
    return;
  }
  exact = pk_matrix_set_ap_generator0_array(pk,&mat,array,pa->intdim,pa->realdim);
  pk_matrix_sort_rows(pk,mat);

  if (!lazy) poly_obtain_satF(pa);
  poly_dual(po);
  if (po!=pa) poly_dual(pa);
  poly_meet_matrix(false,lazy,man,po,pa,mat);
  poly_dual(po);
  if (po!=pa) poly_dual(pa);
  pk_matrix_free(mat);
  man->result.flag_exact = exact;
}

pk_t* pk_add_ray_array(ap_manager_t* man, bool destructive, pk_t* pa, ap_generator0_array_t* array)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY);
  pk_t* po = destructive ? pa : poly_alloc(pa->intdim,pa->realdim);
  poly_add_ray_array(pk->funopt->algorithm<0,
		     man,po,pa,array);
  assert(poly_check(pk,po));
  return po;
}
