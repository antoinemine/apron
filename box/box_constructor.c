/* ********************************************************************** */
/* box_constructor.c: constructors */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box_representation.h"
#include "ap_generic.h"

#include "itv_linexpr.h"
#include "itv_linearize.h"

/* ********************************************************************** */
/* 1. Basic constructors */
/* ********************************************************************** */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

/* Create a bottom (empty) value */
box_t* box_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return box_alloc(intdim,realdim);
}

/* Create a top (universe) value */
box_t* box_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  size_t i;
  box_t* a = box_alloc(intdim,realdim);
  box_init(a);
  for(i=0;i<a->intdim+a->realdim; i++){
    itv_set_top(a->p[i]);
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return a;
}

/* Abstract an hypercube defined by the array of intervals
   of size intdim+realdim */
box_t* box_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval)
{
  size_t i;
  bool exc;
  box_internal_t* intern = box_init_from_manager(man,AP_FUNID_OF_BOX);

  box_t* a = box_alloc(intdim,realdim);
  box_init(a);
  for(i=0;i<intdim+realdim; i++){
    itv_set_ap_interval(intern->itv,a->p[i],tinterval[i]);
    exc = itv_canonicalize(intern->itv,a->p[i],i<intdim);
    if (exc) { box_set_bottom(a); break; }
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return a;
}

/* ********************************************************************** */
/* 2. Accessors */
/* ********************************************************************** */

ap_dimension_t box_dimension(ap_manager_t* man, box_t* a)
{ 
  ap_dimension_t res;
  res.intdim = a->intdim;
  res.realdim = a->realdim;
  return res;
}

/* ********************************************************************** */
/* 3. Tests */
/* ********************************************************************** */

bool box_is_bottom(ap_manager_t* man, box_t* a)
{
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return a->p==NULL;
}

bool box_is_top(ap_manager_t* man, box_t* a)
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
    if (! itv_is_top(a->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

/* inclusion check */
bool box_is_leq(ap_manager_t* man, box_t* a, box_t* b)
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
    if (! itv_is_leq(a->p[i],b->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

/* equality check */
bool box_is_eq(ap_manager_t* man, box_t* a, box_t* b)
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
    if (! itv_is_eq(a->p[i],b->p[i])){
      res = false;
      break;
    }
  }
  return res;
}

bool box_is_dimension_unconstrained(ap_manager_t* man, box_t* a, ap_dim_t dim)
{
  return a->p && itv_is_top(a->p[dim]);
}

/* is the dimension included in the interval in the abstract value ? */
bool box_sat_interval(ap_manager_t* man, 
			 box_t* a,
			 ap_dim_t dim, ap_interval_t* interval)
{
  box_internal_t* intern = box_init_from_manager(man,AP_FUNID_SAT_INTERVAL);
  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (a->p==NULL)
    return true;

  ap_interval_set_itv(intern->itv, intern->sat_interval_interval, a->p[dim]);
  return ap_interval_is_leq(intern->sat_interval_interval,interval);
}

/* does the abstract value satisfy the linear constraint ? */
bool box_sat_lincons(ap_manager_t* man,
			box_t* a, ap_lincons0_t* cons)
{
  itv_lincons_t lincons;
  bool exact;
  tbool_t res;
  box_internal_t* intern = box_init_from_manager(man,AP_FUNID_SAT_LINCONS);
  
  man->result.flag_best = man->result.flag_exact = true;
  
  if (a->p==NULL)
    return true;

  itv_lincons_init(&lincons);
  itv_lincons_set_ap_lincons0(intern->itv,&lincons,cons);
  exact = itv_eval_ap_linexpr0(intern->itv,
			       lincons.linexpr.cst, cons->linexpr0, a->p);
  lincons.linexpr.equality = itv_is_point(intern->itv,lincons.linexpr.cst);
  itv_linexpr_reinit(&lincons.linexpr,0);
  res = itv_eval_cstlincons(intern->itv,&lincons);
  itv_lincons_clear(&lincons);
  
  man->result.flag_exact = exact;
  
  return res==tbool_true;
}

/* does the abstract value satisfy the tree constraint ? */
bool box_sat_tcons(ap_manager_t* man, 
		      box_t* a, ap_tcons0_t* cons)
{
  itv_lincons_t lincons;
  bool exact;
  tbool_t res;
  box_internal_t* intern = box_init_from_manager(man,AP_FUNID_SAT_TCONS);
  
  man->result.flag_best = man->result.flag_exact = true;
  
  if (a->p==NULL)
    return true;
  
  man->result.flag_best = man->result.flag_exact = false;
  itv_lincons_init(&lincons);
  itv_eval_ap_texpr0(intern->itv,
		     lincons.linexpr.cst, cons->texpr0, a->p);
  lincons.linexpr.equality = itv_is_point(intern->itv,lincons.linexpr.cst);
  lincons.constyp = cons->constyp;
  if (cons->scalar)
    num_set_ap_scalar(lincons.num,cons->scalar);
  else
    num_set_int(lincons.num,0);
  res = itv_eval_cstlincons(intern->itv,&lincons);
  if (res==tbool_top) res = tbool_false;
  itv_lincons_clear(&lincons);
  return res==tbool_true;
}

/* ********************************************************************** */
/* II.4 Extraction of properties */
/* ********************************************************************** */

ap_interval_t* box_bound_dimension(ap_manager_t* man,
				   box_t* a, ap_dim_t dim)
{
  bool exact;
  box_internal_t* intern = box_init_from_manager(man,AP_FUNID_BOUND_DIMENSION);
  ap_interval_t* interval = ap_interval_alloc();
  if (a->p==NULL){
    ap_interval_set_bottom(interval);
    exact = true;
  }
  else {
    exact = ap_interval_set_itv(intern->itv,interval,a->p[dim]);
    
  }
  man->result.flag_best = true;
  man->result.flag_exact = exact;
  return interval;
}

/* Returns the interval taken by a linear expression
   over the abstract value */
ap_interval_t* box_bound_linexpr(ap_manager_t* man,
				 box_t* a, ap_linexpr0_t* expr)
{
  bool exact;
  ap_interval_t* interval = ap_interval_alloc();
  box_internal_t* intern =  box_init_from_manager(man,AP_FUNID_BOUND_LINEXPR);

  if (a->p==NULL){
    ap_interval_set_bottom(interval);
    exact = true;
  }
  else {
    exact = itv_eval_ap_linexpr0(intern->itv,
				 intern->bound_linexpr_itv,expr,a->p);
				 
    ap_interval_set_itv(intern->itv, interval,intern->bound_linexpr_itv);
  }
  man->result.flag_best = true;
  man->result.flag_exact = exact;
  return interval;
}

/* Returns the interval taken by a tree expression
   over the abstract value */
ap_interval_t* box_bound_texpr(ap_manager_t* man,
			       box_t* a, ap_texpr0_t* expr)
{
  bool exact;
  ap_interval_t* interval;
  box_internal_t* intern =  box_init_from_manager(man,AP_FUNID_BOUND_TEXPR);

  interval = ap_interval_alloc();
  if (a->p==NULL){
    ap_interval_set_bottom(interval);
    exact = true;
  }
  else {
    itv_eval_ap_texpr0(intern->itv,
		       intern->bound_linexpr_itv,expr,a->p);
    ap_interval_set_itv(intern->itv, interval, intern->bound_linexpr_itv);
  }
  man->result.flag_best = true;
  man->result.flag_exact = false;
  return interval;
}

/* Converts an abstract value to a polyhedra
   (conjunction of linear constraints).
   The size of the returned array is stored in size. */
ap_lincons0_array_t box_to_lincons_array(ap_manager_t* man, box_t* a)
{
  size_t i;
  ap_lincons0_array_t array;
  box_internal_t* intern = (box_internal_t*)man->internal;

  size_t nbdims = a->intdim + a->realdim;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  if (a->p==NULL){
    array = ap_lincons0_array_make(1);
    array.p[0] = ap_lincons0_make_unsat();
  } 
  else if (nbdims==0){
    array = ap_lincons0_array_make(0);
  }
  else {
    size_t size;
    ap_linexpr0_t* expr;
    ap_scalar_t* scalar;

    size = 0;
    for (i=0;i<nbdims;i++){
      if (!bound_infty(a->p[i]->inf)) size++;
      bool point = itv_is_point(intern->itv,a->p[i]);
      if (!point && !bound_infty(a->p[i]->sup)) size++;
    }
    array = ap_lincons0_array_make(size);
    size = 0;
    for (i=0;i<nbdims;i++){      
      bool point = false;
      if (!bound_infty(a->p[i]->inf)){
	expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
	ap_coeff_set_scalar_int(&expr->p.linterm[0].coeff, 1);
	expr->p.linterm[0].dim = i;

	ap_coeff_reinit(&expr->cst,AP_COEFF_SCALAR,AP_SCALAR_DOUBLE);
	scalar = expr->cst.val.scalar;
	ap_scalar_set_bound(scalar,a->p[i]->inf);

	point = itv_is_point(intern->itv,a->p[i]);
	array.p[size].constyp = point ? AP_CONS_EQ : AP_CONS_SUPEQ;
	array.p[size].linexpr0 = expr;
	size++;
      }
      if (!point && !bound_infty(a->p[i]->sup)){
	expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
	ap_coeff_set_scalar_int(&expr->p.linterm[0].coeff, -1);
	expr->p.linterm[0].dim = i;

	ap_coeff_reinit(&expr->cst,AP_COEFF_SCALAR,AP_SCALAR_DOUBLE);
	ap_scalar_set_bound(expr->cst.val.scalar,a->p[i]->sup);

	array.p[size].constyp = AP_CONS_SUPEQ;
	array.p[size].linexpr0 = expr;
	size++;
      }
    }
  }
  return array;
}

ap_tcons0_array_t box_to_tcons_array(ap_manager_t* man, box_t* a)
{
  return ap_generic_to_tcons_array(man,a);
}

ap_generator0_array_t box_to_generator_array(ap_manager_t* man, box_t* a)
{
  size_t i,j,size;
  size_t nbcoeffs,nblines,nbrays,nbvertices,l,r,v;
  ap_generator0_array_t array;
  ap_linexpr0_t* vertex;
  ap_scalar_t scalar;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  
  size = a->intdim+a->realdim;
  if (a->p==NULL){
    array = ap_generator0_array_make(0);
    return array;
  }
  /* Count the number of generators */
  nbvertices = 1; /* we take into account the vertex */
  nbrays = 0;
  nblines = 0;
  nbcoeffs = 0;
  for (i=0;i<size;i++){
    itv_ptr itv = a->p[i];
    bool iinf = bound_infty(itv->inf);
    bool isup = bound_infty(itv->sup);
    if (iinf && isup){
      nblines++;
    }
    else {
      nbcoeffs++;
      if (iinf || isup){
	nbrays++;
      }
      else {
	nbvertices *= 2;
      }
    }
  }
  /* Preparation */
  array = ap_generator0_array_make(nblines+nbrays+nbvertices);
  ap_scalar_init(&scalar,AP_SCALAR_DOUBLE);
  ap_scalar_set_double(&scalar,0.0);
  /* Let's go now ! */  
  v = r = l = 0;
  /* Creates the vertices */
  vertex = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,nbcoeffs);
  for (i=0;i<size;i++){
    itv_ptr itv = a->p[i];
    bool iinf = bound_infty(itv->inf);
    bool isup = bound_infty(itv->sup);
    if (!iinf || !isup){
      ap_linexpr0_set_coeff_scalar(vertex,i,&scalar);
    }
  }
  array.p[nblines+nbrays + v] = ap_generator0_make(AP_GEN_VERTEX,vertex);
  v=1;
  for (i=0; i<size; i++){
    itv_ptr itv = a->p[i];
    bool iinf = bound_infty(itv->inf);
    bool isup = bound_infty(itv->sup);
    if (!iinf && !isup){
      /* Duplication, and in same time set inf and sup */
      for (j=0; j<v; j++){
	array.p[nblines+nbrays + v + j] = 
	  ap_generator0_copy(&array.p[nblines+nbrays + j]);
	ap_scalar_set_bound(&scalar,itv->inf);
	ap_scalar_neg(&scalar,&scalar);
	ap_linexpr0_set_coeff_scalar(array.p[nblines+nbrays + j].linexpr0,
				     i,&scalar);
	ap_scalar_set_bound(&scalar,itv->sup);
	ap_linexpr0_set_coeff_scalar(array.p[nblines+nbrays + v + j].linexpr0,
				     i,&scalar);
      }
      v *= 2;
    }
  }
  /* Create lines and rays */
  for (i=0; i<size; i++){
    ap_linexpr0_t* expr;
    itv_ptr itv = a->p[i];
    bool iinf = bound_infty(itv->inf);
    bool isup = bound_infty(itv->sup);
    if (iinf || isup){
      expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
      if (iinf && isup){
	ap_linexpr0_set_coeff_scalar_double(expr,i,1.0);
	array.p[l] = ap_generator0_make(AP_GEN_LINE,expr);
	l++;
      }
      else if (isup){
	ap_linexpr0_set_coeff_scalar_double(expr,i,1.0);
	array.p[nblines+r] = ap_generator0_make(AP_GEN_RAY,expr);
	r++;
	for (j=nblines+nbrays; j<nblines+nbrays+v; j++){
	  ap_scalar_set_bound(&scalar,itv->inf);
	  ap_scalar_neg(&scalar,&scalar);
	  ap_linexpr0_set_coeff_scalar(expr,i,&scalar);
	}	   
      }
      else if (iinf){
	ap_linexpr0_set_coeff_scalar_double(expr,i,-1.0);
	array.p[nblines+r] = ap_generator0_make(AP_GEN_RAY,expr);
	r++;
	for (j=nblines+nbrays; j<nblines+nbrays+v; j++){
	  ap_scalar_set_bound(&scalar,itv->sup);
	  ap_linexpr0_set_coeff_scalar(expr,i,&scalar);
	}	   
      }
    }
  }
  /* Clear things */
  ap_scalar_clear(&scalar);
  return array;
}

/* Converts an abstract value to an interval/hypercube.
   The size of the resulting array is box_dimension(man,a).  This
   function can be reimplemented by using box_bound_linexpr */
ap_interval_t** box_to_box(ap_manager_t* man, box_t* a)
{
  size_t i;
  ap_interval_t** interval;
  size_t nbdims;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  nbdims = a->intdim+a->realdim;
  interval = ap_interval_array_alloc(nbdims);
  for (i=0; i<nbdims; i++){
    if (a->p==NULL){
      ap_interval_set_bottom(interval[i]);
    } else {
      ap_interval_set_itv(intern->itv,interval[i],a->p[i]);
    }
  }
  return interval;
}
