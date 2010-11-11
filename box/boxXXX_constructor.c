/* ********************************************************************** */
/* boxXXX_constructor.c: constructors */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boxXXX.h"
#include "numXXX_conv.h"
#include "boxXXX_internal.h"
#include "boxXXX_representation.h"
#include "ap_generic.h"

#include "ap_linexprXXX.h"

/* ********************************************************************** */
/* 1. Basic constructors */
/* ********************************************************************** */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

/* Create a bottom (empty) value */
boxXXX_t* boxXXX_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return boxXXX_alloc(intdim,realdim);
}

/* Create a top (universe) value */
boxXXX_t* boxXXX_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  size_t i;
  boxXXX_t* a = boxXXX_alloc(intdim,realdim);
  boxXXX_init(a);
  for(i=0;i<a->intdim+a->realdim; i++){
    eitvXXX_set_top(a->p[i]);
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return a;
}

/* Abstract an hypercube defined by the array of intervals
   of size intdim+realdim */
boxXXX_t* boxXXX_of_box(ap_manager_t* man,
			ap_dimension_t dim,
			ap_box0_t box)
{
  size_t i;
  bool exc;
  ap_coeff_t coeffref;
  boxXXX_internal_t* intern = boxXXX_init_from_manager(man,AP_FUNID_OF_BOX);

  man->result.flag_best = true;
  man->result.flag_exact = true;
  boxXXX_t* a = boxXXX_alloc(dim.intdim,dim.realdim);
  if (dim.intdim+dim.realdim!=0){
    boxXXX_init(a);
    man->result.flag_exact = 
      ap_eitvXXX_array_set_box0(a->p,box,intern->num);
    for(i=0;i<dim.intdim+dim.realdim; i++){
      exc = eitvXXX_canonicalize(a->p[i],i<dim.intdim,intern->num);
      if (exc) { boxXXX_set_bottom(a); break; }
    }
  }
  return a;
}

/* ********************************************************************** */
/* 2. Accessors */
/* ********************************************************************** */

ap_dimension_t boxXXX_dimension(ap_manager_t* man, boxXXX_t* a)
{ 
  ap_dimension_t res;
  res.intdim = a->intdim;
  res.realdim = a->realdim;
  return res;
}

/* ********************************************************************** */
/* 3. Tests */
/* ********************************************************************** */

bool boxXXX_is_bottom(ap_manager_t* man, boxXXX_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return a->p==NULL;
}

bool boxXXX_is_top(ap_manager_t* man, boxXXX_t* a)
{
  size_t i;
  bool res;
  size_t nbdims = a->intdim + a->realdim;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (a->p==NULL)
    return false;

  res = true;
  for (i=0;i<nbdims;i++){
    if (! eitvXXX_is_top(a->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

/* inclusion check */
bool boxXXX_is_leq(ap_manager_t* man, boxXXX_t* a, boxXXX_t* b)
{
  size_t i;
  bool res;
  size_t nbdims;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a->intdim + a->realdim;
  if (a->p==NULL)
    return true;
  else if (b->p==NULL)
    return false;

  res = true;
  for (i=0;i<nbdims;i++){
    if (! eitvXXX_is_leq(a->p[i],b->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

/* equality check */
bool boxXXX_is_eq(ap_manager_t* man, boxXXX_t* a, boxXXX_t* b)
{
  size_t i;
  bool res;
  size_t nbdims;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a->intdim + a->realdim;
  if (a->p==NULL)
    return b->p==NULL;
  else if (b->p==NULL)
    return false;

  res = true;
  for (i=0;i<nbdims;i++){
    if (! eitvXXX_is_eq(a->p[i],b->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

bool boxXXX_is_dimension_unconstrained(ap_manager_t* man, boxXXX_t* a, ap_dim_t dim)
{
  return a->p && eitvXXX_is_top(a->p[dim]);
}

/* is the dimension included in the interval in the abstract value ? */
bool boxXXX_sat_interval(ap_manager_t* man, 
			 boxXXX_t* a,
			 ap_dim_t dim, ap_coeff_t interval)
{
  boxXXX_internal_t* intern = boxXXX_init_from_manager(man,AP_FUNID_SAT_INTERVAL);
  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (a->p==NULL)
    return true;

  eitvXXX_set_ap_coeff(intern->sat_interval_eitv, interval, intern->num);
  return eitvXXX_is_leq(a->p[dim],intern->sat_interval_eitv);
}

/* does the abstract value satisfy the linear constraint ? */
bool boxXXX_sat_lincons(ap_manager_t* man,
			boxXXX_t* a, ap_lincons0_t cons)
{
  ap_linexpr0_t linexpr0;
  ap_linconsXXX_t lincons;
  bool exact;
  tbool_t res;
  boxXXX_internal_t* intern = boxXXX_init_from_manager(man,AP_FUNID_SAT_LINCONS);
  
  man->result.flag_best = man->result.flag_exact = true;
  
  if (a->p==NULL)
    return true;

  ap_lincons0_linexpr0ref(linexpr0,cons);
  exact = eitvXXX_eval_ap_linexpr0(intern->sat_lincons_itv,
				   linexpr0,a->p,intern->num);
  ap_linconsXXX_init(lincons,0);
  eitvXXX_set(lincons->linexpr->cst,intern->sat_lincons_itv);
  lincons->constyp = ap_lincons0_get_constyp(cons);
  ap_lincons0_get_mpq(lincons->mpq,cons);

  res = ap_linconsXXX_evalcst(lincons,intern->num);
  ap_linconsXXX_clear(lincons);
  man->result.flag_exact = exact;
  return res==tbool_true;
}

/* does the abstract value satisfy the tree constraint ? */
bool boxXXX_sat_tcons(ap_manager_t* man, 
		      boxXXX_t* a, ap_tcons0_t* cons)
{
  ap_linconsXXX_t lincons;
  bool exact;
  tbool_t res;
  boxXXX_internal_t* intern = boxXXX_init_from_manager(man,AP_FUNID_SAT_TCONS);
  
  man->result.flag_best = man->result.flag_exact = true;
  
  if (a->p==NULL)
    return true;
  
  man->result.flag_best = man->result.flag_exact = false;

  eitvXXX_eval_ap_texpr0(intern->sat_lincons_itv,
			 cons->texpr0,a->p,intern->num);
  ap_linconsXXX_init(lincons,0);
  eitvXXX_set(lincons->linexpr->cst,intern->sat_lincons_itv);
  lincons->constyp = cons->constyp;
  mpq_set(lincons->mpq, cons->mpq);
  res = ap_linconsXXX_evalcst(lincons,intern->num);
  ap_linconsXXX_clear(lincons);
  return res==tbool_true;
}

/* ********************************************************************** */
/* II.4 Extraction of properties */
/* ********************************************************************** */

void boxXXX_bound_dimension(ap_manager_t* man,
			    ap_coeff_t interval, boxXXX_t* a, ap_dim_t dim)
{
  bool exact;
  boxXXX_internal_t* intern = boxXXX_init_from_manager(man,AP_FUNID_BOUND_DIMENSION);
  if (a->p==NULL){
    ap_coeff_set_bottom(interval);
    exact = true;
  }
  else {
    exact = ap_coeff_set_eitvXXX(interval,a->p[dim],intern->num);
  }
  man->result.flag_best = true;
  man->result.flag_exact = exact;
}

/* Returns the interval taken by a linear expression
   over the abstract value */
void boxXXX_bound_linexpr(ap_manager_t* man,
			  ap_coeff_t interval, boxXXX_t* a, ap_linexpr0_t expr)
{
  bool exact;
  boxXXX_internal_t* intern =  boxXXX_init_from_manager(man,AP_FUNID_BOUND_LINEXPR);

  if (a->p==NULL){
    ap_coeff_set_bottom(interval);
    exact = true;
  }
  else {
    exact = eitvXXX_eval_ap_linexpr0(intern->bound_linexpr_itv,
				     expr,a->p,intern->num);
    ap_coeff_set_eitvXXX(interval,intern->bound_linexpr_itv,intern->num);
  }
  man->result.flag_best = true;
  man->result.flag_exact = exact;
}

/* Returns the interval taken by a tree expression
   over the abstract value */
void boxXXX_bound_texpr(ap_manager_t* man,
			ap_coeff_t interval, boxXXX_t* a, ap_texpr0_t* expr)
{
  bool exact;
  boxXXX_internal_t* intern =  boxXXX_init_from_manager(man,AP_FUNID_BOUND_TEXPR);

  if (a->p==NULL){
    ap_coeff_set_bottom(interval);
    exact = true;
  }
  else {
    eitvXXX_eval_ap_texpr0(intern->bound_linexpr_itv,expr,a->p, intern->num);
    ap_coeff_set_eitvXXX(interval,intern->bound_linexpr_itv, intern->num);
  }
  man->result.flag_best = true;
  man->result.flag_exact = false;
}

/* Converts an abstract value to a polyhedra
   (conjunction of linear constraints). */

void boxXXX_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t array, boxXXX_t* a)
{
  bool exact,exact2;
  size_t i,j;
  ap_lincons0_t lincons0ref;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;
  size_t nbdims = a->intdim + a->realdim;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (a->p==NULL){
    ap_lincons0_array_resize(array,1);
    ap_lincons0_array_ref_index(lincons0ref,array,0);
    ap_lincons0_set_bool(lincons0ref,false);
  }
  else if (nbdims==0){
    ap_lincons0_array_resize(array,0);
  }
  else {
    size_t size;

    size = 0;
    for (i=0;i<nbdims;i++){
      if (!boundXXX_infty(a->p[i]->itv->neginf)) size++;
      if (!boundXXX_infty(a->p[i]->itv->sup)) size++;
    }
    SWITCHZ(array->discr)
      {
	eitvZZZ_t eitv;
	ap_linconsZZZ_ptr lincons;
	ap_linconsZZZ_array_ptr tab = array->lincons_array.ZZZ;

	ap_linconsZZZ_array_resize(tab,size);
	eitvZZZ_init(eitv);
	exact = true;
	j = 0;
	for (i=0;i<nbdims;i++){
	  exact = eitvZZZ_set_eitvXXX(eitv,a->p[i],intern->num) && exact;

	  if (eitv->eq){
	    lincons = tab->p[j];
	    ap_linconsZZZ_resize(lincons,1);
	    lincons->linexpr->linterm[0]->dim = i;
	    eitvZZZ_set_int(lincons->linexpr->linterm[0]->eitv,1);
	    eitvZZZ_neg(lincons->linexpr->cst,eitv);
	    lincons->constyp = AP_CONS_EQ;
	    j++;
	  }
	  else {
	    if (!boundZZZ_infty(eitv->itv->neginf)){
	      lincons = tab->p[j];
	      ap_linconsZZZ_resize(lincons,1);
	      lincons->linexpr->linterm[0]->dim = i;
	      eitvZZZ_set_int(lincons->linexpr->linterm[0]->eitv,1);
	      eitvZZZ_set_num(lincons->linexpr->cst,
			      boundZZZ_numref(eitv->itv->neginf));   
	      lincons->constyp = AP_CONS_SUPEQ;
	      j++;
	    }
	    if (!boundZZZ_infty(eitv->itv->sup)){
	      lincons = tab->p[j];
	      ap_linconsZZZ_resize(lincons,1);
	      lincons->linexpr->linterm[0]->dim = i;
	      eitvZZZ_set_int(lincons->linexpr->linterm[0]->eitv,-1);
	      eitvZZZ_set_num(lincons->linexpr->cst,
				      boundZZZ_numref(eitv->itv->sup));    
	      lincons->constyp = AP_CONS_SUPEQ;
	      j++;
	    }
	  }
	}
	if (j<size) ap_linconsZZZ_array_resize(tab,j);
      }
    ENDSWITCH;
  }
}

ap_tcons0_array_t boxXXX_to_tcons_array(ap_manager_t* man, boxXXX_t* a)
{
  return ap_generic_to_tcons_array(man,a);
}

void boxXXX_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t array, boxXXX_t* a)
{
  bool exact;
  size_t i,j,size;
  size_t nbcoeffs,nblines,nbrays,nbvertices,l,r,v;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  
  size = a->intdim+a->realdim;
  if (a->p==NULL){
    ap_lingen0_array_resize(array,0);
    return;
  }
  SWITCHZ(array->discr)
    {
      eitvZZZ_t eitv,coeff;
      ap_lingenZZZ_t vertex;
      ap_lingenZZZ_array_ptr tab = array->lingen_array.ZZZ;

      eitvZZZ_init(eitv);
      eitvZZZ_init(coeff);
      ap_lingenZZZ_init(vertex,0);
      vertex->gentyp = AP_GEN_VERTEX;

      exact = true;
      /* Count the number of generators */
      nbvertices = 1; /* we take into account the vertex */
      nbrays = 0;
      nblines = 0;
      for (i=0;i<size;i++){
	exact = eitvZZZ_set_eitvXXX(eitv,a->p[i],intern->num) && exact;
	bool iinf = boundZZZ_infty(eitv->itv->neginf);
	bool isup = boundZZZ_infty(eitv->itv->sup);
	if (iinf && isup){
	  nblines++;
	}
	else {
	  if (iinf || isup){
	    nbrays++;
	  }
	  else {
	    if (!eitv->eq){
	      nbvertices *= 2;
	    }
	    ap_linexprZZZ_set_eitv0(vertex->linexpr,i,coeff);
	  }
	}
      }
      
      /* Preparation */
      ap_lingenZZZ_array_resize(tab,nblines+nbrays+nbvertices);
      eitvZZZ_set_int(coeff,0);
      for (i=0; i<nblines+nbrays+nbvertices;i++){
	ap_linexprZZZ_set_cst(tab->p[i]->linexpr,coeff);
      }
      /* Let's go now ! */  
      v = r = l = 0;
      /* Creates the vertices */
      ap_lingenZZZ_set(tab->p[nblines+nbrays + v], vertex);
      v=1;
      for (i=0; i<size; i++){
	eitvZZZ_set_eitvXXX(eitv,a->p[i],intern->num);
	bool iinf = boundZZZ_infty(eitv->itv->neginf);
	bool isup = boundZZZ_infty(eitv->itv->sup);
	if (iinf || isup){
	  /* line or ray */
	  eitvZZZ_set_int(coeff, (iinf && !isup) ? -1 : 1);
	  j = (iinf && isup) ? l : (nbrays + r);
	  ap_linexprZZZ_resize(tab->p[j]->linexpr,1);
	  ap_linexprZZZ_set_eitv0(tab->p[j]->linexpr,i,coeff);
	  if (iinf && isup){ /* line */
	    tab->p[j]->gentyp =
	      i<a->intdim ? AP_GEN_LINEMOD : AP_GEN_LINE;
	    l++;
	  }
	  else { /* ray */
	    tab->p[j]->gentyp = 
	      i<a->intdim ? AP_GEN_RAYMOD : AP_GEN_RAY;
	    r++;
	  }
	}
	else { /* vertex */
	  if (!eitv->eq){
	    /* Duplication, and in same time set inf and sup */
	    for (j=0; j<v; j++){
	      ap_lingenZZZ_set(tab->p[nblines+nbrays + v + j],
			       tab->p[nblines+nbrays + j]);
	      eitvZZZ_set_num(coeff,boundZZZ_numref(eitv->itv->neginf));
	      eitvZZZ_neg(coeff,coeff);
	      ap_linexprZZZ_set_eitv0(tab->p[nblines+nbrays + j]->linexpr,
				    i,coeff);
	      eitvZZZ_set_num(coeff,boundZZZ_numref(eitv->itv->sup));
	      ap_linexprZZZ_set_eitv0(tab->p[nblines+nbrays + v + j]->linexpr,
				    i,coeff);
	    }
	    v *= 2;
	  }
	  else {
	    for (j=0; j<v; j++){
	      ap_linexprZZZ_set_eitv0(tab->p[nblines+nbrays + v + j]->linexpr,
				      i,eitv);
	    }
	  }
	}
      }
      eitvZZZ_clear(eitv);
      eitvZZZ_clear(coeff);
      ap_lingenZZZ_clear(vertex);
      man->result.flag_exact = exact;
    }
  ENDSWITCH;
}

/* Converts an abstract value to an interval/hypercube.
   The size of the resulting array is boxXXX_dimension(man,a).  This
   function can be reimplemented by using boxXXX_bound_linexpr */
void boxXXX_to_box(ap_manager_t* man, ap_box0_t res, boxXXX_t* a)
{
  size_t i;
  size_t nbdims;
  boxXXX_internal_t* intern = (boxXXX_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a->intdim+a->realdim;
  ap_box0_resize(res,nbdims);
  if (nbdims>0){
    man->result.flag_exact = 
      ap_box0_set_eitvXXX_array(res,a->p,nbdims,intern->num);
  }
}
