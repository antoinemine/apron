/*
 * octXXX_predicate.c
 *
 * Predicates on octagons, extraction functions
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "octXXX.h"
#include "octXXX_internal.h"
#include "ap_generic.h"



/* ============================================================ */
/* Tests */
/* ============================================================ */

bool octXXX_is_bottom(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_IS_BOTTOM,0);
  if (pr->option.comparison_test_closure) octXXX_cache_closure(pr,a);
  if (a->closed) {
    /* definitively non empty on Q */
    if (numXXX_incomplete || a->intdim) { flag_incomplete; }
    return false;
  }
  else if (!a->m)
    /* definitively empty */
    return true;
  else {
    /* no closure => we don't know */
    flag_algo;
    return false;
  }
}

bool octXXX_is_top(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_IS_TOP,0);
  size_t i,j;
  boundXXX_t* m = a->m ? a->m : a->closed;
  if (!m) return false;
  for (i=0;i<2*a->dim;i++)
    for (j=0;j<=(i|1);j++,m++)
      if (!boundXXX_infty(*m) && i!=j)
	return false;
  return true;
}

bool octXXX_is_leq(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_IS_LEQ,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return false;);
  if (pr->option.comparison_test_closure) octXXX_cache_closure(pr,a1);
  if (!a1->closed && !a1->m) {
    /* a1 definitively empty */
    return true;
  }
  else if (!a2->closed && !a2->m) {
    /* a2 definitively empty */
    if (a1->closed) {
      /* a1 not empty on Q */
      if (numXXX_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo; return false; }
  }
  else {
    size_t i,j;
    boundXXX_t *x = a1->closed ? a1->closed : a1->m;
    boundXXX_t *y = a2->closed ? a2->closed : a2->m;
    for (i=0;i<2*a1->dim;i++)
      for (j=0;j<=(i|1);j++,x++,y++)
	if (boundXXX_cmp(*x,*y)>0) {
	  if (a1->closed) {
	    /* not included on Q */
	    if (numXXX_incomplete || a1->intdim) { flag_incomplete; }
	    return false;
	  }
	  else { flag_algo; return false; }
	}
    /* definitively included */
    return true;
  }
}


bool octXXX_is_eq(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_IS_EQ,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return false;);
  if (pr->option.comparison_test_closure) {
    octXXX_cache_closure(pr,a1);
    octXXX_cache_closure(pr,a2);
  }
  if (!a1->closed && !a1->m) {
    if (!a2->closed && !a2->m) {
      /* both are empty */
      return true;
    }
    else if (a2->closed) {
      /* a1 empty, e2 not empty on Q */
      if (numXXX_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo; return false; }
  }
  else if (!a2->closed && !a2->m) {
    if (a1->closed) {
      /* a2 empty, e1 not empty on Q */
      if (numXXX_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo; return false; }
  }
  else {
    size_t i,j;
    boundXXX_t *x = a1->closed ? a1->closed : a1->m;
    boundXXX_t *y = a2->closed ? a2->closed : a2->m;
    for (i=0;i<2*a1->dim;i++)
      for (j=0;j<=(i|1);j++,x++,y++)
	if (boundXXX_cmp(*x,*y)) {
	  if (a1->closed) {
	    /* not equal on Q */
	    if (numXXX_incomplete || a1->intdim)
	      { flag_incomplete; }
	    return false;
	  }
	  else { flag_algo; return false; }
	}
    /* definitively equal */
    return true;
  }
}


bool octXXX_sat_interval(ap_manager_t* man, octXXX_t* a,
			 ap_dim_t dim, ap_coeff_t i)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_SAT_INTERVAL,0);
  arg_assert(dim<a->dim,return false;);
  if (pr->option.sat_test_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    return true;
  }
  else {
    boundXXX_t* m = a->closed ? a->closed : a->m;
    ap_coeff_t b;
    bool r;
    ap_coeff_init(b,i->discr);
    /* get (possibly approximated) bounds */
    coeff_of_boundsXXX(pr,b,
		       m[matpos(2*dim,2*dim+1)],m[matpos(2*dim+1,2*dim)],true);
    /* compare with i */
    r = ap_coeff_is_leq(b,i);
    ap_coeff_clear(b);
    if (r) return true; /* definitively saturates */
    else
      /* definitely does not saturate on Q if closed & no conv error */
      if (numXXX_incomplete || a->intdim) { flag_incomplete; return false; }
      else if (!a->closed) { flag_algo; return false; }
      else if (pr->conv) { flag_conv; return false; }
      else return false;
  }
}


bool octXXX_is_dimension_unconstrained(ap_manager_t* man, octXXX_t* a,
				       ap_dim_t dim)
{
  octXXX_internal_t* pr =
    octXXX_init_from_manager(man,AP_FUNID_IS_DIMENSION_UNCONSTRAINED,0);
  arg_assert(dim<a->dim,return false;);
  if (!a->closed && !a->m)
    /* definitively empty */
    return false;
  else {
    boundXXX_t* b = a->closed ? a->closed : a->m;
    size_t i, d2=2*dim;
    for (i=0;i<2*a->dim;i++) {
      if (!boundXXX_infty(b[matpos2(i,d2)]) && i!=d2) return false;
      if (!boundXXX_infty(b[matpos2(i,d2+1)]) && i!=d2+1) return false;
    }
    return true;
  }
}


/* not very precise for non unit constraints (always top) */
/* the semantics of non-deterministic expressions (i.e., intervals) is
   true  saturates all expressions
   false may saturate some, may not saturate some
*/
bool octXXX_sat_lincons(ap_manager_t* man, octXXX_t* a,
			ap_lincons0_t lincons)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_SAT_LINCONS,
					     2*(a->dim+1));
  if (pr->option.sat_test_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    return true;
  }
  else {
    boundXXX_t* b = a->closed ? a->closed : a->m;
    size_t i, ui, uj;
    ap_constyp_t c = ap_lincons0_get_constyp(lincons);
    uexpr u;
    bool r;

    switch (c) {

      /* skipped */
    case AP_CONS_EQMOD:
    case AP_CONS_DISEQ:
      return false;

      /* handled */
    case AP_CONS_EQ:
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      break;

      /* error */
    default:
      assert(0);
    }

    ap_linexpr0_t linexpr;
    ap_lincons0_linexpr0ref(linexpr,lincons);
    u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,linexpr,a->dim);

    switch (u.type) {

    case EMPTY:
      /* the empty set has all properties */
      return true;

    case ZERO:
      if ((c==AP_CONS_SUPEQ && boundXXX_sgn(pr->tmp[0])<=0) ||
	  /* [-a,b] >= 0 <=> a <= 0 */
	  (c==AP_CONS_SUP && boundXXX_sgn(pr->tmp[0])<0) ||
	  /* [-a,b] > 0 <=> a < 0 */
	  (c==AP_CONS_EQ && !boundXXX_sgn(pr->tmp[0]) && !boundXXX_sgn(pr->tmp[1]))
	  /* [-a,b] = 0 <=> a=b=0 */
	  )
	return true; /* always saturates */
      else {
	/* does not always saturate on Q, if closed and no conv error */
	if (numXXX_incomplete || a->intdim) { flag_incomplete; return false; }
	else if (!a->closed) { flag_algo; return false; }
	else if (pr->conv) { flag_conv; return false; }
	return false;
      }

   case UNARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      boundXXX_mul_2exp(pr->tmp[0],pr->tmp[0],1);
      boundXXX_mul_2exp(pr->tmp[1],pr->tmp[1],1);
      boundXXX_badd(pr->tmp[0],b[matpos(ui,ui^1)]);
      boundXXX_badd(pr->tmp[1],b[matpos(ui^1,ui)]);
      if (boundXXX_sgn(pr->tmp[0])<=0 &&
	  /* c_i X_i + [-a,b] >= 0 <=> -c_i X_i + a <= 0 */
	  (c!=AP_CONS_SUP || boundXXX_sgn(pr->tmp[0])<0) &&
	  /* c_i X_i + [-a,b] >  0 <=> -c_i X_i + a <  0 */
	  (c!=AP_CONS_EQ || boundXXX_sgn(pr->tmp[1])<=0)
	  /* c_i X_i + [-a,b] <= 0 <=>  c_i X_i + b <= 0 */
	  )
	return true; /* always saturates */
      else {
	/* does not always saturate on Q, if closed and no conv error */
	if (numXXX_incomplete || a->intdim) { flag_incomplete; return false; }
	else if (!a->closed) { flag_algo; return false; }
	else if (pr->conv) { flag_conv; return false; }
	return false;
      }

    case BINARY:
      if ( u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if ( u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;
      boundXXX_badd(pr->tmp[0],b[matpos(uj,ui^1)]);
      boundXXX_badd(pr->tmp[1],b[matpos(uj^1,ui)]);
      if (boundXXX_sgn(pr->tmp[0])<=0 &&
	  /* c_i X_i + c_j X_j + [-a,b] >= 0 <=> -c_i X_i - c_j X_j + a <= 0 */
	  (c!=AP_CONS_SUP || boundXXX_sgn(pr->tmp[0])<0) &&
	  /* c_i X_i + c_j X_j + [-a,b] >  0 <=> -c_i X_i - c_j X_j + a <  0 */
	  (c!=AP_CONS_EQ || boundXXX_sgn(pr->tmp[1])<=0)
	  /* c_i X_i + c_j X_j + [-a,b] <= 0 <=>  c_i X_i + c_j X_j + b <= 0 */
	  )
	return true;
      else {
	/* does not saturate on Q, when closed and no conv error */
	if (numXXX_incomplete || a->intdim) { flag_incomplete; return false; }
	else if (!a->closed) { flag_algo; return false; }
	else if (pr->conv) { flag_conv; return false; }
	return false;
      }

    case OTHER:
      /* no clue */
      flag_incomplete;
      return false;

    default:
      assert(0);
      return false; /* unreachable */
    }
  }
}

bool octXXX_sat_tcons(ap_manager_t* man, octXXX_t* a,
		      ap_tcons0_t* cons)
{
  return
    ap_generic_sat_tcons(
	man,a,cons,numXXX_scalar,
	AP_LINEXPR_INTLINEAR,false,
	(bool(*)(ap_manager_t*,void*,ap_lincons0_t))octXXX_sat_lincons
    );
}

/* ============================================================ */
/* Extraction of properties */
/* ============================================================ */

/* not very precise for non unit constraints (interval arithmetics) */
void octXXX_bound_linexpr(ap_manager_t* man,
			  ap_coeff_t r, octXXX_t* a, ap_linexpr0_t expr)
{
  ap_coeff_t coeff;
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_BOUND_LINEXPR,
					     2*(a->dim+5));
  if (pr->option.bound_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    ap_coeff_set_bottom(r);
  }
  else {
    boundXXX_t* b = a->closed ? a->closed : a->m;
    size_t i, ui, uj;
    uexpr u = octXXX_uexpr_of_linexpr0(pr,pr->tmp,expr,a->dim);
    switch (u.type) {

    case EMPTY:
      ap_coeff_set_bottom(r);
      break;

    case ZERO:
      coeff_of_boundsXXX(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* exact on Q if closed and no conversion error */
      if (numXXX_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;

    case UNARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      ap_linexpr0_cstref(coeff,expr);
      boundsXXX_of_coeff(pr,pr->tmp[0],pr->tmp[1],coeff,true);
      boundXXX_badd(pr->tmp[0],b[matpos(ui,ui^1)]);
      boundXXX_badd(pr->tmp[1],b[matpos(ui^1,ui)]);
      coeff_of_boundsXXX(pr,r,pr->tmp[0],pr->tmp[1],true);
      /* exact on Q if closed and no conversion error */
      if (numXXX_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;

    case BINARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if (u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;
      boundXXX_badd(pr->tmp[0],b[matpos2(uj,ui^1)]);
      boundXXX_badd(pr->tmp[1],b[matpos2(uj^1,ui)]);
      coeff_of_boundsXXX(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* exact on Q if closed and no conversion error */
      if (numXXX_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;

    case OTHER:
      /* interval approximation */
      for (i=0;i<a->dim;i++) {
	boundXXX_mul_2exp(pr->tmp[2*i+2],pr->tmp[2*i+2],-1);
	boundXXX_mul_2exp(pr->tmp[2*i+3],pr->tmp[2*i+3],-1);
	boundsXXX_mul(pr->tmp[2*i+2],pr->tmp[2*i+3],
		   b[matpos(2*i,2*i+1)],b[matpos(2*i+1,2*i)],
		   pr->tmp[2*i+2],pr->tmp[2*i+3],pr->tmp+2*(a->dim+1));
	boundXXX_badd(pr->tmp[0],pr->tmp[2*i+2]);
	boundXXX_badd(pr->tmp[1],pr->tmp[2*i+3]);
      }
      coeff_of_boundsXXX(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* not optimal, even when closing a */
      flag_incomplete;
      break;

    default: assert(0);
    }
  }
}

void octXXX_bound_texpr(ap_manager_t* man,
			ap_coeff_t r, octXXX_t* a, ap_texpr0_t* expr)
{
  return
    ap_generic_bound_texpr(
	man,r,a,expr,numXXX_scalar,false,false,
	(void (*)(ap_manager_t*,ap_coeff_t,void*,ap_linexpr0_t))octXXX_bound_linexpr
    );
}

void octXXX_bound_dimension(ap_manager_t* man,
			    ap_coeff_t r, octXXX_t* a, ap_dim_t dim)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_BOUND_DIMENSION,0);
  arg_assert(dim<a->dim,return;);
  if (pr->option.bound_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    ap_coeff_set_bottom(r);
  }
  else if (a->closed) {
    /* optimal in Q */
    coeff_of_boundsXXX(pr,r,
		       a->closed[matpos(2*dim,2*dim+1)],
		       a->closed[matpos(2*dim+1,2*dim)],true);
    if (numXXX_incomplete || a->intdim) flag_incomplete;
    else if (pr->conv) flag_conv;
  }
  else {
    /* not optimal */
    coeff_of_boundsXXX(pr,r,
		       a->m[matpos(2*dim,2*dim+1)],a->m[matpos(2*dim+1,2*dim)],
		       true);
    flag_algo;
  }
}

void octXXX_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t ar, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY,0);
  MACRO_SWITCH(ar->discr) ZZZ {
    ap_linconsZZZ_array_ptr arZ = ar->lincons_array.ZZZ;

    if (!a->closed && !a->m) {
      /* definitively empty */
      ap_linconsZZZ_array_resize(arZ,1);
      ap_linconsZZZ_set_bool(arZ->p[0],false);
    }
    else {
      /* put non-oo constraint bounds only */
      boundXXX_t* m = a->closed ? a->closed : a->m;
      size_t i,j,n=0;
      ap_linconsZZZ_array_resize(arZ,matsize(a->dim));
      for (i=0;i<2*a->dim;i++)
	for (j=0;j<=(i|1);j++,m++) {
	  if (i==j || boundXXX_infty(*m)) continue;
	  linconsZZZ_of_boundXXX(pr,arZ->p[n],i,j,*m);
	  n++;
	}
      ap_linconsZZZ_array_resize(arZ,n);
    }
  }
  ENDMACRO;
  if (pr->conv) flag_conv;
}

ap_tcons0_array_t octXXX_to_tcons_array(ap_manager_t* man, octXXX_t* a)
{
  return ap_generic_to_tcons_array(man,a,numXXX_scalar);
}

void octXXX_to_box(ap_manager_t* man, ap_linexpr0_t box, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_TO_BOX,0);
  size_t i;
  ap_coeff_t coeff;

  ap_linexpr0_set_zero(box);
  if (pr->option.extract_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* definitively empty */
    ap_linexpr0_cstref(coeff,box);
    ap_coeff_set_bottom(coeff);
    if (a->dim>0){
      ap_linexpr0_resize(box,1);
      ap_coeff_t coeff;
      ap_linexpr0_coeffref(coeff,box,0);
      ap_coeff_set_bottom(coeff);
    }
  }
  else {
    /* put variable bounds */
    boundXXX_t* m = a->closed ? a->closed : a->m;
    for (i=0;i<a->dim;i++){
      ap_linexpr0_coeffref(coeff,box,i);
      coeff_of_boundsXXX(pr,coeff,
			 m[matpos(2*i,2*i+1)],m[matpos(2*i+1,2*i)],true);
    }
    man->result.flag_exact = false;
    if (!a->closed) flag_algo;
    else if (numXXX_incomplete || a->intdim) flag_incomplete;
    else if (pr->conv) flag_conv;
  }
}

/* not really implemented (returns either top or bottom) */
void octXXX_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t ar, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_TO_LINGEN_ARRAY,0);
  if (pr->option.extract_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* definitively empty */
    ap_lingen0_array_resize(ar,0);
  }
  else {
    /* not empty => full universe */
    size_t i;
    ap_lingen0_t lingen;
    ap_linexpr0_t linexpr;
    ap_coeff_t coeff;
    /* origin vertex */
    ap_lingen0_array_resize(ar,a->dim+1);

    /* origin vertex */
    ap_lingen0_array_ref_index(lingen,ar,0);
    ap_lingen0_linexpr0ref(linexpr,lingen);
    ap_linexpr0_set_zero(linexpr);
    ap_lingen0_set_gentyp(lingen,AP_GEN_VERTEX);
    /* one line for each dimension */
    for (i=0;i<a->dim;i++) {
      ap_lingen0_array_ref_index(lingen,ar,i+1);
      ap_lingen0_linexpr0ref(linexpr,lingen);
      ap_linexpr0_set_zero(linexpr);
      ap_linexpr0_coeffref(coeff,linexpr,i);
      ap_coeff_set_int(coeff,1);
      ap_lingen0_set_gentyp(lingen,AP_GEN_LINE);
    }
    flag_incomplete;
  }
}
