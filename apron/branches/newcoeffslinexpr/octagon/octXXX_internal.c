/*
 * octXXX_internal.c
 *
 * Access to internal structures and algorithms.
 * Use with care.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "octXXX_fun.h"
#include "octXXX_internal.h"
#include "num_conv.h"
#include "eitvXXX.h"

/* ********************************************************************** */
/* I. Manager */
/* ********************************************************************** */

/* called by each function to setup and get manager-local data */
octXXX_internal_t*
octXXX_init_from_manager(ap_manager_t* man, ap_funid_t id, size_t size)
{
  octXXX_internal_t* pr = (octXXX_internal_t*) man->internal;
  pr->funid = id;
  pr->funopt = man->option.funopt+id;
  man->result.flag_exact = man->result.flag_best = true;
  pr->conv = false;
  if (pr->tmp_size<size) {
    boundXXX_clear_array(pr->tmp,pr->tmp_size);
    pr->tmp = (boundXXX_t*)realloc(pr->tmp,sizeof(boundXXX_t)*size);
    assert(pr->tmp);
    pr->tmp_size = size;
    boundXXX_init_array(pr->tmp,pr->tmp_size);
    pr->tmp2 = realloc(pr->tmp2,sizeof(long)*size);
    assert(pr->tmp2);
  }
  return pr;
}

/* ********************************************************************** */
/* III. Numbers */
/* ********************************************************************** */

/* ============================================================ */
/* III.2 Conversions from user types */
/* ============================================================ */

/* sound conversion from a coeff to an upper boundXXX_t
   optional multiplication by 2
   pr->conv is set if the conversion is not exact
 */

void upper_boundXXX_of_coeff(octXXX_internal_t* pr,
			     boundXXX_t r, ap_coeff_t coeff,
			     bool mul2)
{
  bool exact;
  MACRO_SWITCH(coeff->discr) ZZZ {
    exact = boundXXX_set_boundZZZ(r,coeff->eitv.ZZZ->itv->sup,pr->num);
  }
  ENDMACRO;
  if (mul2) {
    boundXXX_mul_2exp(r,r,1);
    exact = false;
  }
  pr->conv = !exact;
}

/* sound conversion from a coeff to [neginf,sup]/eitvXXX_t
   optional multiplication by 2
   pr->conv is set if the conversion is not exact
   returns true if the interval is empty
*/
bool boundsXXX_of_coeff(octXXX_internal_t* pr,
			boundXXX_t neginf, boundXXX_t sup,
			ap_coeff_t c,
			bool mul2)
{
  bool exact,bottom;
  MACRO_SWITCH(c->discr) ZZZ {
    exact = boundXXX_set_boundZZZ(neginf,c->eitv.ZZZ->itv->neginf,pr->num);
    exact = boundXXX_set_boundZZZ(sup,c->eitv.ZZZ->itv->sup,pr->num) && exact;
  }
  ENDMACRO;
  if (mul2){
    boundXXX_mul_2exp(neginf,neginf,1);
    boundXXX_mul_2exp(sup,sup,1);
    exact = false;
  }
  pr->conv = !exact;
  /* Tests if empty */
  if (boundXXX_infty(neginf) || boundXXX_infty(sup)){
    bottom = false;
  } else {
    numXXX_ptr num_neginf = boundXXX_numref(neginf);
    numXXX_neg(num_neginf,num_neginf);
    bottom = numXXX_cmp(boundXXX_numref(sup),num_neginf) < 0;
    numXXX_neg(num_neginf,num_neginf);
  }
  return bottom;
}

bool eitvXXX_of_coeff(octXXX_internal_t* pr,
				    eitvXXX_t res,
				    ap_coeff_t c,
				    bool mul2)
{
  bool exact = eitvXXX_set_ap_coeff(res,c,pr->num);
  if (mul2){
    eitvXXX_mul_2exp(res,res,1);
    exact = false;
  }
  pr->conv = !exact;
  return eitvXXX_canonicalize(res,false);
}

void boundsXXX_of_lingen0(octXXX_internal_t* pr, boundXXX_t* dst,
			  ap_linexpr0_t e, size_t dim)
{
  size_t i;
  ap_dim_t d;
  ap_coeff_t coeff;

  for (i=0;i<dim;i++) {
    boundXXX_set_int(dst[2*i],0);
    boundXXX_set_int(dst[2*i+1],0);
  }
  ap_linexpr0_ForeachLinterm(e,i,d,coeff){
    arg_assert(d<dim,return;);
    boundsXXX_of_coeff(pr,dst[2*d],dst[2*d+1],coeff,false);
  }
}

/* ============================================================ */
/* III.3 Conversions to user types */
/* ============================================================ */

/* upper bound => coeff, with optional division by 2
   pr->conv is set if the conversion is not exact
*/
void coeff_of_upper_boundXXX(octXXX_internal_t* pr,
					   ap_coeff_t coeff,
					   boundXXX_t m,
					   bool div2)
{
  bool exact;
  assert(!boundXXX_infty(m));
  MACRO_SWITCH(coeff->discr) ZZZ {
    eitvZZZ_ptr eitv = coeff->eitv.ZZZ;
    exact = boundZZZ_set_boundXXX(eitv->itv->sup,m,pr->num);
    if (div2){
      boundZZZ_mul_2exp(eitv->itv->sup,eitv->itv->sup,-1);
      exact = false;
    }
    boundZZZ_neg(eitv->itv->neginf,eitv->itv->sup);
    eitv->eq = true;
  }
  ENDMACRO;
  pr->conv = !exact;
}


/* makes an interval from [neginf,sup]/eitvXXX, with sound approximations
   pr->conv is set if the conversion is not exact
   note: may output an empty interval
*/
void coeff_of_eitvXXX(octXXX_internal_t* pr,
				    ap_coeff_t res,
				    eitvXXX_t eitv,
				    bool div2)
{
  bool exact;
  if (div2){
    eitvXXX_mul_2exp(pr->eitv,eitv,-1);
    ap_coeff_set_eitvXXX(res,pr->eitv,pr->num);
    exact = false;
  } else {
    exact = ap_coeff_set_eitvXXX(res,eitv,pr->num);
  }
  pr->conv = !exact;
}
void coeff_of_boundsXXX(octXXX_internal_t* pr,
			ap_coeff_t coeff,
			boundXXX_t neginf, boundXXX_t sup,
			bool div2)
{
  boundXXX_ptr pneginf,psup;
  bool exact = true;
  if (div2){
    boundXXX_mul_2exp(pr->eitv->itv->neginf,neginf,-1);
    boundXXX_mul_2exp(pr->eitv->itv->sup,sup,-1);
    pneginf = pr->eitv->itv->neginf;
    psup = pr->eitv->itv->sup;
    exact = false;
  } else {
    pneginf = neginf;
    psup = sup;
  }
  MACRO_SWITCH(coeff->discr) ZZZ {
    eitvZZZ_ptr eitv = coeff->eitv.ZZZ;
    exact = boundZZZ_set_boundXXX(eitv->itv->neginf,pneginf,pr->num) && exact;
    exact = boundZZZ_set_boundXXX(eitv->itv->sup,psup,pr->num) && exact;
    eitv->eq = itvZZZ_is_point(eitv->itv);
  }
  ENDMACRO;
  pr->conv = !exact;
}

/* ============================================================ */
/* III.4 Bound manipulations */
/* ============================================================ */

/* [-r_inf,r_sup] = [-a_inf,a_sup] * [-b_inf,b_sup]
   where 0 * oo = oo * 0 = 0
 */
void boundsXXX_mul(boundXXX_t r_inf, boundXXX_t r_sup,
				 boundXXX_t a_inf, boundXXX_t a_sup,
				 boundXXX_t b_inf, boundXXX_t b_sup,
				 boundXXX_t tmp[8])
{
  boundXXX_mul(tmp[0],a_sup,b_sup);
  boundXXX_neg(tmp[4],a_sup); boundXXX_mul(tmp[4],tmp[4],b_sup);

  boundXXX_mul(tmp[1],a_inf,b_inf);
  boundXXX_neg(tmp[5],a_inf);  boundXXX_mul(tmp[5],tmp[5],b_inf);

  boundXXX_mul(tmp[6],a_sup,b_inf);
  boundXXX_neg(tmp[2],a_sup);  boundXXX_mul(tmp[2],tmp[2],b_inf);

  boundXXX_mul(tmp[7],a_inf,b_sup);
  boundXXX_neg(tmp[3],a_inf);  boundXXX_mul(tmp[3],tmp[3],b_sup);

  boundXXX_max(r_sup,tmp[0],tmp[1]);
  boundXXX_max(r_sup,r_sup,tmp[2]);
  boundXXX_max(r_sup,r_sup,tmp[3]);

  boundXXX_max(r_inf,tmp[4],tmp[5]);
  boundXXX_max(r_inf,r_inf,tmp[6]);
  boundXXX_max(r_inf,r_inf,tmp[7]);
}

/* ============================================================ */
/* III.5 Conversion to constraints */
/* ============================================================ */

/* constraint at line i, column j, with upper bound m */
MACRO_FOREACH ZZZ @MainNum
void linconsZZZ_of_boundXXX(octXXX_internal_t* pr,
			    ap_linconsZZZ_t cons,
			    size_t i, size_t j,
			    boundXXX_t m)
{
  ap_coeff_t coeff;
  eitvZZZ_ptr eitv;
  ap_linconsZZZ_set_zero(cons);
  ap_coeff_cons_eitvZZZ(coeff,cons->linexpr->cst);
  coeff_of_upper_boundXXX(pr,coeff,m,(i==j) || (i==(j^1)));
  if (i==j) {
    /* zeroary constraint */
  }
  else if (i==(j^1)) {
    /* unary constraint */
    eitv = ap_linconsZZZ_eitvref0(cons,i/2,true);
    eitvZZZ_set_int(eitv, i&1 ? -1 : 1);
  }
  else {
    /* binary constraint */
    eitv = ap_linconsZZZ_eitvref0(cons,i/2,true);
    eitvZZZ_set_int(eitv, i&1 ? -1 : 1);
    eitv = ap_linconsZZZ_eitvref0(cons,j/2,true);
    eitvZZZ_set_int(eitv, j&1 ? 1 : -1);
  }
  cons->constyp = AP_CONS_SUPEQ;
}
ENDMACRO
void lincons0_of_boundXXX(octXXX_internal_t* pr,
			  ap_lincons0_t cons,
			  size_t i, size_t j,
			  boundXXX_t m)
{
  ap_coeff_t coeff;
  ap_lincons0_set_zero(cons);
  ap_lincons0_cstref(coeff,cons);
  coeff_of_upper_boundXXX(pr,coeff,m,(i==j) || (i==(j^1)));
  if (i==j) {
    /* zeroary constraint */
  }
  else if (i==(j^1)) {
    /* unary constraint */
    ap_lincons0_coeffref(coeff,cons,i/2);
    ap_coeff_set_int(coeff, i&1 ? -1 : 1);
  }
  else {
    /* binary constraint */
    ap_lincons0_coeffref(coeff,cons,i/2);
    ap_coeff_set_int(coeff, i&1 ? -1 : 1);
    ap_lincons0_coeffref(coeff,cons,j/2);
    ap_coeff_set_int(coeff, j&1 ? 1 : -1);
  }
  ap_lincons0_set_constyp(cons,AP_CONS_SUPEQ);
}
