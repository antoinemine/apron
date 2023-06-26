/*
 * avo_predicate.c
 *
 * Predicates on AV octagons, extraction functions
 *
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"
#include "ap_generic.h"


int abs_print_hint_flag = 0; 
  /* print 11x to denote abs(x), i.e., |x| */

/* ============================================================ */
/* Tests */
/* ============================================================ */

bool avo_is_bottom(ap_manager_t* man, avo_t* a)
{
   //fprintf(stdout, "AP_FUNID_IS_BOTTOM\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_IS_BOTTOM,0);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (a->closed) {
    /* definitively non empty on Q */
    if (num_incomplete || a->intdim) { flag_incomplete; }
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

bool avo_is_top(ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_IS_TOP\n"); fflush(stdout);
    avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_IS_TOP,0);
    size_t i,j;
    bound_t* m = a->m ? a->m : a->closed;
    if (!m) return false;
    for (i=0;i<4*a->dim;i++)
      for (j=0;j<=(i|1);j++,m++)
        if (!bound_infty(*m) && i!=j)
  	return false;
    return true;
}

bool avo_is_leq(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
  //fprintf(stdout, "AP_FUNID_IS_LEQ\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_IS_LEQ,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return false;);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a1);
  if (!a1->closed && !a1->m) {
    /* a1 definitively empty */
    return true;   
  }
  else if (!a2->closed && !a2->m) {
    /* a2 definitively empty */
    if (a1->closed) {
      /* a1 not empty on Q */
      if (num_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo;    return false; }
  }
  else {
    size_t i,j;
    bound_t *x = a1->closed ? a1->closed : a1->m;
    bound_t *y = a2->closed ? a2->closed : a2->m;
    bound_t *nx = a1->nsc;
    bound_t *ny = a2->nsc;
    bound_t *sx = x;
    bound_t *sy = y;
    size_t dim = a1->dim;
    for (i=0;i<4*a1->dim;i++)
      for (j=0;j<=(i|1);j++,x++,y++,nx++,ny++)
      {
    	  if(i > 2 * dim &&( bound_cmp_int(sx[avo_matpos2( i - 2 * dim  + 1, i - 2 * dim )],0)>= 0||bound_cmp_int(sx[avo_matpos2( i - 2 * dim  , i - 2 * dim + 1)],0)<= 0))
    	  	continue;
    	  if(j > 2 * dim &&( bound_cmp_int(sx[avo_matpos2( j - 2 * dim  + 1, j - 2 * dim )],0)>= 0||bound_cmp_int(sx[avo_matpos2( j - 2 * dim  , j - 2 * dim + 1)],0)<= 0))
    	  	continue;

    	  if(i > 2 * dim &&( bound_cmp_int(sy[avo_matpos2( i - 2 * dim  + 1, i - 2 * dim )],0)>= 0||bound_cmp_int(sy[avo_matpos2( i - 2 * dim  , i - 2 * dim + 1)],0)<= 0))
    	  	continue;
    	  if(j > 2 * dim &&( bound_cmp_int(sy[avo_matpos2( j - 2 * dim  + 1, j - 2 * dim )],0)>= 0||bound_cmp_int(sy[avo_matpos2( j - 2 * dim  , j - 2 * dim + 1)],0)<= 0))
    	  	continue;
		  if (bound_cmp(*x,*y)>0 || (bound_cmp(*x,*y)==0 && bound_cmp(*nx,*ny)>0)) {
			if (a1->closed) {
				  /* not included on Q */
				if (num_incomplete || a1->intdim) { flag_incomplete; }
				return false;
			}
			else {
				flag_algo;
				return false;
			}
		  }
      }
    return true;
  }
}


bool avo_is_eq(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
	//fprintf(stdout, "AP_FUNID_IS_EQ\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_IS_EQ,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return false;);
  if (pr->funopt->algorithm>=0) {
    avo_cache_closure(pr,a1);
    avo_cache_closure(pr,a2);
  }
  if (!a1->closed && !a1->m) {
    if (!a2->closed && !a2->m) {
      /* both are empty */
      return true;
    }
    else if (a2->closed) {
      /* a1 empty, e2 not empty on Q */
      if (num_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo; return false; }
  }
  else if (!a2->closed && !a2->m) {
    if (a1->closed) {
      /* a2 empty, e1 not empty on Q */
      if (num_incomplete || a1->intdim) { flag_incomplete; }
      return false;
    }
    else { flag_algo; return false; }
  }
  else {
    size_t i,j,dim;
    bound_t *x = a1->closed ? a1->closed : a1->m;
    bound_t *y = a2->closed ? a2->closed : a2->m;
    bound_t *nx = a1->nsc;
    bound_t *ny = a2->nsc;
    bound_t *sx = x;
    bound_t *sy = y;
    dim = a1->dim;
    for (i=0;i<2*a1->dim;i++)
      for (j=0;j<=(i|1);j++,x++,y++,nx++,ny++)
      {
    	  if(i > 2 * dim &&( bound_cmp_int(sx[avo_matpos2( i - 2 * dim  + 1, i - 2 * dim )],0)>= 0||bound_cmp_int(sx[avo_matpos2( i - 2 * dim  , i - 2 * dim + 1)],0)<= 0))
    	    	  continue;
    	  if(j > 2 * dim &&( bound_cmp_int(sx[avo_matpos2( j - 2 * dim  + 1, j - 2 * dim )],0)>= 0||bound_cmp_int(sx[avo_matpos2( j - 2 * dim  , j - 2 * dim + 1)],0)<= 0))
    	    	  continue;
    	  if(i > 2 * dim &&( bound_cmp_int(sy[avo_matpos2( i - 2 * dim  + 1, i - 2 * dim )],0)>= 0||bound_cmp_int(sy[avo_matpos2( i - 2 * dim  , i - 2 * dim + 1)],0)<= 0))
    	    	  continue;
    	  if(j > 2 * dim &&( bound_cmp_int(sy[avo_matpos2( j - 2 * dim  + 1, j - 2 * dim )],0)>= 0||bound_cmp_int(sy[avo_matpos2( j - 2 * dim  , j - 2 * dim + 1)],0)<= 0))
    	    	  continue;
          if (bound_cmp(*x,*y)||bound_cmp(*nx,*ny)!=0) {
			if (a1->closed) {
			  /* not equal on Q */
			  if (num_incomplete || a1->intdim)  { flag_incomplete; }
			  return false;
			}
			else { flag_algo; return false; }
          }
      }
    /* definitively equal */
    return true;
  }
}


bool avo_sat_interval(ap_manager_t* man, avo_t* a,
		      ap_dim_t dim, ap_interval_t* i)
{
     //	 fprintf(stdout, "AP_FUNID_SAT_INTERVAL\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_SAT_INTERVAL,0);
  arg_assert(dim<a->dim,return false;);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    return true;
  }
  else {
    bound_t* m = a->closed ? a->closed : a->m;
    ap_interval_t* b = ap_interval_alloc();
    bool r;
    /* get (possibly approximated) bounds */
    avo_interval_of_bounds(pr,b,
		       m[avo_matpos(2*dim,2*dim+1)],m[avo_matpos(2*dim+1,2*dim)],true);
    /* compare with i */
    r = (ap_scalar_cmp(b->inf,i->inf)>=0) && (ap_scalar_cmp(b->sup,i->sup)<=0);
    ap_interval_free(b);
    if (r) return true; /* definitively saturates */
    else
      /* definitely does not saturate on Q if closed & no conv error */
      if (num_incomplete || a->intdim) { flag_incomplete; return false; } 
      else if (!a->closed) { flag_algo; return false; } 
      else if (pr->conv) { flag_conv; return false; }
      else return false;
  }
}


bool avo_is_dimension_unconstrained(ap_manager_t* man, avo_t* a,
				    ap_dim_t dim)
{
  //fprintf(stdout, "AP_FUNID_IS_DIMENSION_UNCONSTRAINED\n"); fflush(stdout);
  avo_internal_t* pr =
    avo_init_from_manager(man,AP_FUNID_IS_DIMENSION_UNCONSTRAINED,0);
  arg_assert(dim<a->dim,return false;);
  if (!a->closed && !a->m)
    /* definitively empty */
    return false;
  else {
    bound_t* b = a->closed ? a->closed : a->m;
    size_t i, d2=2*dim;
    for (i=0;i<2*a->dim;i++) {
      if (!bound_infty(b[avo_matpos2(i,d2)]) && i!=d2) return false;
      if (!bound_infty(b[avo_matpos2(i,d2+1)]) && i!=d2+1) return false;
    }
    return true;
  }
}


/* not very precise for non unit constraints (always top) */
/* the semantics of non-deterministic expressions (i.e., intervals) is
   true  saturates all expressions
   false may saturate some, may not saturate some
*/
bool avo_sat_lincons(ap_manager_t* man, avo_t* a,
		     ap_lincons0_t* lincons)
{
  //fprintf(stdout, "AP_FUNID_SAT_LINCONS\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_SAT_LINCONS,
					     2*(a->dim+1));
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    return true;
  }
  else {
    bound_t* b = a->closed ? a->closed : a->m;
    size_t i, ui, uj;
    ap_constyp_t c = lincons->constyp;
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
    u = avo_uexpr_of_linexpr(pr,pr->tmp,lincons->linexpr0,a->dim);
    switch (u.type) {      

    case EMPTY:
      /* the empty set has all properties */
      return true;

    case ZERO:
      if ((c==AP_CONS_SUPEQ && bound_sgn(pr->tmp[0])<=0) ||
	  /* [-a,b] >= 0 <=> a <= 0 */
	  (c==AP_CONS_SUP && bound_sgn(pr->tmp[0])<0) ||
	  /* [-a,b] > 0 <=> a < 0 */
	  (c==AP_CONS_EQ && !bound_sgn(pr->tmp[0]) && !bound_sgn(pr->tmp[1]))
  	  /* [-a,b] = 0 <=> a=b=0 */
	  )
    	return true; /* always saturates */
      else {
		/* does not always saturate on Q, if closed and no conv error */
		if (num_incomplete || a->intdim) { flag_incomplete; return false; }
		else if (!a->closed) { flag_algo; return false; }
		else if (pr->conv) { flag_conv; return false; }
		return false;
      }
      
   case UNARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      bound_mul_2(pr->tmp[0],pr->tmp[0]);
      bound_mul_2(pr->tmp[1],pr->tmp[1]);
      bound_badd(pr->tmp[0],b[avo_matpos(ui,ui^1)]);
      bound_badd(pr->tmp[1],b[avo_matpos(ui^1,ui)]);
      if (bound_sgn(pr->tmp[0])<=0 &&
	  /* c_i X_i + [-a,b] >= 0 <=> -c_i X_i + a <= 0 */
	  (c!=AP_CONS_SUP || bound_sgn(pr->tmp[0])<0) &&
	  /* c_i X_i + [-a,b] >  0 <=> -c_i X_i + a <  0 */
	  (c!=AP_CONS_EQ || bound_sgn(pr->tmp[1])<=0)
	  /* c_i X_i + [-a,b] <= 0 <=>  c_i X_i + b <= 0 */
	  ) 
    	  return true; /* always saturates */
      else {
		/* does not always saturate on Q, if closed and no conv error */
		if (num_incomplete || a->intdim) { flag_incomplete; return false; }
		else if (!a->closed) { flag_algo; return false; }
		else if (pr->conv) { flag_conv; return false; }
		return false;
      }
      
    case BINARY:
      if ( u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if ( u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;
      bound_badd(pr->tmp[0],b[avo_matpos(uj,ui^1)]);
      bound_badd(pr->tmp[1],b[avo_matpos(uj^1,ui)]);
      if (bound_sgn(pr->tmp[0])<=0 &&
	  /* c_i X_i + c_j X_j + [-a,b] >= 0 <=> -c_i X_i - c_j X_j + a <= 0 */
	  (c!=AP_CONS_SUP || bound_sgn(pr->tmp[0])<0) &&
	  /* c_i X_i + c_j X_j + [-a,b] >  0 <=> -c_i X_i - c_j X_j + a <  0 */
	  (c!=AP_CONS_EQ || bound_sgn(pr->tmp[1])<=0)
	  /* c_i X_i + c_j X_j + [-a,b] <= 0 <=>  c_i X_i + c_j X_j + b <= 0 */
	  )
    	return true;
      else {
		/* does not saturate on Q, when closed and no conv error */
		if (num_incomplete || a->intdim) { flag_incomplete; return false; }
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

bool avo_sat_tcons(ap_manager_t* man, avo_t* a,
		   ap_tcons0_t* cons)
{
  return ap_generic_sat_tcons(man,a,cons,
#if defined(NUM_NUMFLT)
			      AP_SCALAR_DOUBLE
#else
			      AP_SCALAR_MPQ
#endif
			      ,
			      false);
}

/* ============================================================ */
/* Extraction of properties */
/* ============================================================ */

/* not very precise for non unit constraints (interval arithmetics) */
ap_interval_t* avo_bound_linexpr(ap_manager_t* man,
				 avo_t* a, ap_linexpr0_t* expr)
{
  //fprintf(stdout, "AP_FUNID_BOUND_LINEXPR\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_BOUND_LINEXPR,
					     2*(a->dim+5));
  ap_interval_t* r = ap_interval_alloc();
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    ap_interval_set_bottom(r);
  }
  else {
    bound_t* b = a->closed ? a->closed : a->m;
    size_t i, ui, uj;
    uexpr u = avo_uexpr_of_linexpr(pr,pr->tmp,expr,a->dim);
    switch (u.type) {

     case EMPTY:
      ap_interval_set_bottom(r);
      break;

     case ZERO:
      avo_interval_of_bounds(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* exact on Q if closed and no conversion error */
      if (num_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;
      
     case UNARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      avo_bounds_of_coeff(pr,pr->tmp[0],pr->tmp[1],expr->cst,true);
      bound_badd(pr->tmp[0],b[avo_matpos(ui,ui^1)]);
      bound_badd(pr->tmp[1],b[avo_matpos(ui^1,ui)]);
      avo_interval_of_bounds(pr,r,pr->tmp[0],pr->tmp[1],true);
      /* exact on Q if closed and no conversion error */
      if (num_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;
      
     case BINARY:
      if (u.coef_i==1) ui = 2*u.i; else ui = 2*u.i+1;
      if (u.coef_j==1) uj = 2*u.j; else uj = 2*u.j+1;
      bound_badd(pr->tmp[0],b[avo_matpos2(uj,ui^1)]);
      bound_badd(pr->tmp[1],b[avo_matpos2(uj^1,ui)]);
      avo_interval_of_bounds(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* exact on Q if closed and no conversion error */
      if (num_incomplete || a->intdim) flag_incomplete;
      else if (!a->closed) flag_algo;
      else if (pr->conv) flag_conv;
      break;
      
     case OTHER:
      /* interval approximation */
      for (i=0;i<a->dim;i++) {
		bound_div_2(pr->tmp[2*i+2],pr->tmp[2*i+2]);
		bound_div_2(pr->tmp[2*i+3],pr->tmp[2*i+3]);
		bounds_mul(pr->tmp[2*i+2],pr->tmp[2*i+3],
			   b[avo_matpos(2*i,2*i+1)],b[avo_matpos(2*i+1,2*i)],
			   pr->tmp[2*i+2],pr->tmp[2*i+3],pr->tmp+2*(a->dim+1));
		bound_badd(pr->tmp[0],pr->tmp[2*i+2]);
		bound_badd(pr->tmp[1],pr->tmp[2*i+3]);
      }
      avo_interval_of_bounds(pr,r,pr->tmp[0],pr->tmp[1],false);
      /* not optimal, even when closing a */
      flag_incomplete;
      break;

     default: assert(0);
    }
  }
  return r;
}

ap_interval_t* avo_bound_texpr(ap_manager_t* man,
			       avo_t* a, ap_texpr0_t* expr)
{
  return ap_generic_bound_texpr(man,a,expr,
#if defined(NUM_NUMFLT)
				AP_SCALAR_DOUBLE
#else
				AP_SCALAR_MPQ
#endif
				,
				false);
}

ap_interval_t* avo_bound_dimension(ap_manager_t* man,
				   avo_t* a, ap_dim_t dim)
{
  //fprintf(stdout, "AP_FUNID_BOUND_DIMENSION\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_BOUND_DIMENSION,0);
  ap_interval_t* r = ap_interval_alloc();
  arg_assert(dim<a->dim,ap_interval_free(r);return NULL;);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* really empty */
    ap_interval_set_bottom(r);
  }
  else if (a->closed) {
    /* optimal in Q */
    avo_interval_of_bounds(pr,r,
		       a->closed[avo_matpos(2*dim,2*dim+1)],
		       a->closed[avo_matpos(2*dim+1,2*dim)],true);
    if (num_incomplete || a->intdim) flag_incomplete;
    else if (pr->conv) flag_conv;
  }
  else {
    /* not optimal */
    avo_interval_of_bounds(pr,r,
		       a->m[avo_matpos(2*dim,2*dim+1)],a->m[avo_matpos(2*dim+1,2*dim)],
		       true);
    flag_algo;
  }
  return r;
}

ap_lincons0_array_t avo_to_lincons_array(ap_manager_t* man, avo_t* a)
{
	//fprintf(stdout, "AP_FUNID_TO_LINCONS_ARRAY\n"); fflush(stdout);
  ap_lincons0_array_t ar;
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY,0);
  if (!a->closed && !a->m) {
    /* definitively empty */
    ar = ap_lincons0_array_make(1);
    ar.p[0] = ap_lincons0_make_unsat();
  }
  else {
    /* put non-oo constraint bounds only */
    bound_t* m = a->closed ? a->closed : a->m;
    bound_t* nsc = a->nsc;
    bound_t* m1 = m;
    bound_t* nsc1 = nsc;
    size_t i,j,n=0;
    ar = ap_lincons0_array_make(avo_matsize(a->dim*2));

    for (i=0;i<4*a->dim;i++)
      for (j=0;j<=(i|1);j++,m1++,nsc1++) {
		if (i==j || bound_infty(*m1)  || ( j>=a->dim*2 && j%2==0 ) ||( i>=a->dim *2 && i%2!=0 ))
			continue;

			/* no terms between -|Vi| and Vi */
		if (i>=2*a->dim && (i == j+2*a->dim || i == j+2*a->dim-1))
			continue;

			/* no |Vi| terms if postive or negative */
		if(i>2*a->dim &&( bound_cmp_int(m[avo_matpos2( i-i%2-2*a->dim+1, i-i%2-2*a->dim )],0)<= 0||bound_cmp_int(m[avo_matpos2( i-i%2-2*a->dim, i-i%2-2*a->dim+1)],0)<= 0))
			continue;
		if(j>2*a->dim &&( bound_cmp_int(m[avo_matpos2( j-j%2-2*a->dim+1, j-j%2-2*a->dim )],0)<= 0||bound_cmp_int(m[avo_matpos2( j-j%2-2*a->dim, j-j%2-2*a->dim+1)],0)<= 0))
			continue;


			/*  -|Vi|, -|Vj|, -|Vi| -|Vj| <=0 */
		if (i>=2*a->dim && i % 2 == 0 && j>=2*a->dim && j % 2 == 1){
			int order = bound_cmp_int(*m1,0);
			if((order>0)||(order==0 && bound_cmp_int(*nsc1,1)>=0))
				continue;
		}
		ar.p[n] = avo_lincons_of_bound(pr,i,j,*m1,*nsc1,a->dim);
		n++;
      }

    ar.size = n;
    if (pr->conv) flag_conv;
  }
  return ar;
}

ap_tcons0_array_t avo_to_tcons_array(ap_manager_t* man, avo_t* a)
{
  return ap_generic_to_tcons_array(man,a);
}

ap_interval_t** avo_to_box(ap_manager_t* man, avo_t* a)
{
   //fprintf(stdout, "AP_FUNID_TO_BOX\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_TO_BOX,0);
  ap_interval_t** in = ap_interval_array_alloc(a->dim);
  size_t i;
  //if (pr->funopt->algorithm>=0)
  avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    /* definitively empty */
    for (i=0;i<a->dim;i++)
      ap_interval_set_bottom(in[i]);
  }
  else {
    /* put variable bounds */
    bound_t* m = a->closed ? a->closed : a->m;
    for (i=0;i<a->dim;i++)
      avo_interval_of_bounds(pr,in[i],
			 m[avo_matpos(2*i,2*i+1)],m[avo_matpos(2*i+1,2*i)],true);
    man->result.flag_exact = false;
    if (!a->closed) flag_algo;
    else if (num_incomplete || a->intdim) flag_incomplete;
    else if (pr->conv) flag_conv;
  }
  return in;
}

/* not really implemented (returns either top or bottom) */
ap_generator0_array_t avo_to_generator_array(ap_manager_t* man, avo_t* a)
{
  ap_generator0_array_t ar;
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_TO_GENERATOR_ARRAY,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid, "not implemented");
  ar = ap_generator0_array_make(0);
  return ar;
}
