/* ********************************************************************** */
/* box_meetjoin.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box_representation.h"
#include "box_constructor.h"
#include "box_meetjoin.h"

#include "itv_linexpr.h"
#include "itv_linearize.h"

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

box_t* box_meet(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2)
{
  size_t i;
  bool exc;
  size_t nbdims;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = true;
  res = destructive ? a1 : box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL || a2->p==NULL){
    box_set_bottom(res);
    return res;
  }
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a1->realdim;
  for (i=0; i<nbdims; i++){
    exc = itv_meet(intern->itv,res->p[i],a1->p[i],a2->p[i]);
    if (exc){
      box_set_bottom(res);
      break;
    }
  }
  return res;
}

box_t* box_join(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2)
{
  size_t i;
  size_t nbdims;
  box_t* res;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a1 : box_alloc(a1->intdim,a1->realdim);
  if (a1->p==NULL){
    if (a2->p!=NULL){
      man->result.flag_exact = true;
      box_set(res,a2);
    }
    return res;
  }
  else if (a2->p==NULL){
    man->result.flag_exact = true;
    if (!destructive) box_set(res,a1);
    return res;
  }
  man->result.flag_exact = false;
  if (!destructive){
    box_init(res);
  }
  nbdims = a1->intdim + a2->realdim;
  for (i=0; i<nbdims; i++){
    itv_join(res->p[i],a1->p[i],a2->p[i]);
  }
  return res;
}

box_t* box_meet_array(ap_manager_t* man, box_t** tab, size_t size)
{
  size_t i;
  box_t* res;
  res = box_copy(man,tab[0]);
  for (i=1;i<size;i++){
    box_meet(man,true,res,tab[i]);
    if (res->p==NULL) break;
  }
  man->result.flag_best = true;
  man->result.flag_exact = true;
  return res;
}
box_t* box_join_array(ap_manager_t* man, box_t** tab, size_t size)
{
  size_t i;
  box_t* res;
  res = box_copy(man,tab[0]);
  for (i=1;i<size;i++){
    box_join(man,true,res,tab[i]);
  }
  man->result.flag_best = true;
  man->result.flag_exact = false;
  return res;
}

/* ============================================================ */
/* Add_ray_array */
/* ============================================================ */


/* Generalized time elapse operator */
void box_add_ray(box_internal_t* intern,
		 box_t* a,
		 ap_generator0_t* gen)
{
  size_t i;
  int sgn;
  ap_coeff_t* coeff;
  ap_dim_t dim;
  ap_linexpr0_t* expr;

  assert(gen->gentyp != AP_GEN_VERTEX);
  if (a->p==NULL){
    box_set_bottom(a);
    return;
  }
  expr = gen->linexpr0;
  ap_linexpr0_ForeachLinterm(expr,i,dim,coeff){
    assert(coeff->discr==AP_COEFF_SCALAR);
    ap_scalar_t* scalar = coeff->val.scalar;
    sgn = ap_scalar_sgn(scalar);
    if (sgn!=0){
      if (sgn>0 || gen->gentyp==AP_GEN_LINE){
	bound_set_infty(a->p[dim]->sup,1);
      }
      if (sgn<0 || gen->gentyp==AP_GEN_LINE){
	bound_set_infty(a->p[dim]->inf,1);
      }
    }
  }
}

box_t* box_add_ray_array(ap_manager_t* man,
			 bool destructive,
			 box_t* a,
			 ap_generator0_array_t* array)
{
  size_t i;
  box_t* res;
  box_internal_t* intern = (box_internal_t*)man->internal;

  man->result.flag_best = true;
  man->result.flag_exact = false;
  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_exact = true;
    return res;
  }
  for (i=0;i<array->size; i++){
    box_add_ray(intern,res,&array->p[i]);
  }
  return res;
}


/* ============================================================ */
/* Meet_lincons */
/* ============================================================ */

box_t* box_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_lincons0_array_t* array)
{
  bool empty = false;
  box_t* res;
  size_t kmax;
  itv_lincons_array_t tlincons;
  box_internal_t* intern = (box_internal_t*)man->internal;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = array->size==1;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_LINCONS_ARRAY].algorithm;
    if (kmax<1) kmax=2;
    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern->itv,&tlincons,array);
    tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
    if (tb==tbool_false){
      goto _box_meet_lincons_array_bottom;
    }
    itv_boxize_lincons_array(intern->itv,
			     res->p,NULL,
			     &tlincons,res->p,a->intdim,kmax,false,&empty);
    if (empty){
    _box_meet_lincons_array_bottom:
      box_set_bottom(res);
    }
    itv_lincons_array_clear(&tlincons);
  }
  return res;
}


box_t* box_meet_tcons_array(ap_manager_t* man,
			    bool destructive,
			    box_t* a,
			    ap_tcons0_array_t* array)
{
  bool empty = false;
  box_t* res;
  int kmax;
  box_internal_t* intern = (box_internal_t*)man->internal;
  itv_lincons_array_t tlincons;

  res = destructive ? a : box_copy(man,a);
  if (a->p==NULL){
    man->result.flag_best = true;
    man->result.flag_exact = true;
  }
  else {
    man->result.flag_best = false;
    man->result.flag_exact = false;
    kmax = man->option.funopt[AP_FUNID_MEET_TCONS_ARRAY].algorithm;

    if (kmax < 100) {
      /* algo in -oo - 99: legacy implementation */

      if (kmax<1) kmax=2;
      itv_lincons_array_init(&tlincons,array->size);
      itv_intlinearize_ap_tcons0_array(intern->itv,&tlincons,
                                       array,res->p,res->intdim);
      tbool_t tb = itv_lincons_array_reduce_integer(intern->itv,&tlincons,a->intdim);
      if (tb==tbool_false){
        goto _box_meet_tcons_array_bottom;
      }
      itv_boxize_lincons_array(intern->itv,
                               res->p,NULL,
                               &tlincons,res->p,a->intdim,kmax,false,&empty);
      if (empty){
      _box_meet_tcons_array_bottom:
        box_set_bottom(res);
      }
      itv_lincons_array_clear(&tlincons);
    }
    else {
      /* algo in 100 - +oo: experimental implementation based on HC4 */

      kmax -= 100;
      if (kmax < 1) kmax = 2; /* default, for algorithm = 100 */
      if (itv_meet_ap_tcons0_array(intern->itv,array,res->p,res->intdim,kmax)) {
        box_set_bottom(res);
      }
    }
  }
  return res;
}
