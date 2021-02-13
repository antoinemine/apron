/*
 * avo_nary.c
 *
 * N-ary AV octagon functions: meet, join, widening, narrowing & related.
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

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

avo_t* avo_meet(ap_manager_t* man, bool destructive, avo_t* a1, avo_t* a2)
{
	//printf("AP_FUNID_MEET \n");fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_MEET,0);
  bound_t* m;
  bound_t* nsc;

  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
  if ((!a1->closed && !a1->m) || (!a2->closed && !a2->m))
    /* one argument is empty */
    return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,destructive);
  else {
    bound_t* m1 = a1->closed ? a1->closed : a1->m;
    bound_t* m2 = a2->closed ? a2->closed : a2->m;
    size_t i;
    m = destructive ? m1 : avo_hmat_alloc(pr,a1->dim);
    bound_t* nsc1 = a1->nsc;
    bound_t* nsc2 = a2->nsc;
    nsc = destructive ? nsc1 : avo_hmat_alloc(pr,a1->dim);
    for (i=0;i<avo_matsize(a1->dim);i++)
    {
    	bound_set(m[i],m1[i]); bound_set(nsc[i],nsc1[i]);
    	bound_bmin_nsc(m+i,nsc+i,m2[i],nsc2[i]);
    }
    ///////////////////////////////////////
    avo_hmat_s_step(m,nsc,a1->dim);
    avo_t* a12=avo_set_mat_nsc(pr,a1,m,NULL,nsc,destructive);

    /* optimal, but not closed */
    return a12;
  }
}

avo_t* avo_join(ap_manager_t* man, bool destructive, avo_t* a1, avo_t* a2)
{
 avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_JOIN,0);

 arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
 if (pr->funopt->algorithm>=0) {
   avo_cache_closure(pr,a1);
   avo_cache_closure(pr,a2);
 }
 if (!a1->closed && !a1->m) {
   if (!a2->closed && !a2->m)
     /* both empty */
     return avo_set_mat_nsc(pr,a1,NULL,NULL,NULL,destructive);
   else
     /* a1 empty, a2 not empty */
     return avo_set_mat_nsc(pr,a1,avo_hmat_copy(pr,a2->m,a2->dim),
			avo_hmat_copy(pr,a2->closed,a2->dim),avo_hmat_copy(pr,a2->nsc,a2->dim),destructive);
 }
 else if (!a2->closed && !a2->m)
   /* a1 not empty, a2 empty */
   return avo_set_mat_nsc(pr,a1,a1->m,a1->closed,a1->nsc,destructive);
 else {
   /* not empty */
   bound_t* m1 = a1->closed ? a1->closed : a1->m;
   bound_t* m2 = a2->closed ? a2->closed : a2->m;
   bound_t* m = destructive ? m1 : avo_hmat_alloc(pr,a1->dim);
   size_t i;
   bound_t* nsc1 = a1->nsc;
   bound_t* nsc2 = a2->nsc;
   bound_t* nsc = destructive ? nsc1 : avo_hmat_alloc(pr,a1->dim);
   man->result.flag_exact = false;
   for (i=0;i<avo_matsize(a1->dim);i++)
   {
	   bound_set(m[i],m1[i]); bound_set(nsc[i],nsc1[i]);
	   bound_bmax_nsc(m+i,nsc+i,m2[i],nsc2[i]);
   }

   avo_hmat_s_step(m, nsc, a1->dim);

   if (a1->closed && a2->closed) {
     /* result is closed and optimal on Q */
     if (num_incomplete || a1->intdim) flag_incomplete;
     return avo_set_mat_nsc(pr,a1,NULL,m,nsc,destructive);
   }
   else {
     /* not optimal, not closed */
     flag_algo;
     return avo_set_mat_nsc(pr,a1,m,NULL,nsc,destructive);
   }
 }
}

avo_t* avo_meet_array(ap_manager_t* man, avo_t** tab, size_t size)
{
	//printf("AP_FUNID_MEET_ARRAY \n");fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_MEET_ARRAY,0);
  avo_t* r;
  size_t i,k;
  arg_assert(size>0,return NULL;);
  r = avo_alloc_internal(pr,tab[0]->dim,tab[0]->intdim);
  /* check whether there is an empty element */
  for (k=0;k<size;k++)
    if (!tab[k]->m && !tab[k]->closed) return r;
  /* all elements are non-empty */
  r->m = avo_hmat_copy(pr,tab[0]->closed ? tab[0]->closed : tab[0]->m,r->dim);
  for (k=1;k<size;k++) {
    bound_t* x = tab[k]->closed ? tab[k]->closed : tab[k]->m;
    arg_assert(tab[k]->dim==r->dim && tab[k]->intdim==r->intdim,
	       avo_free_internal(pr,r);return NULL;);
    for (i=0;i<avo_matsize(r->dim);i++)
      bound_bmin(r->m[i],x[i]);
  }
  return r;
}

avo_t* avo_join_array(ap_manager_t* man, avo_t** tab, size_t size)
{
   //fprintf(stdout, "AP_FUNID_JOIN_ARRAY\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_JOIN_ARRAY,0);
  int algo = pr->funopt->algorithm;
  bool closed = true;
  avo_t* r;
  bound_t* m = NULL;
  bound_t* nsc = NULL;
  size_t i,k;
  arg_assert(size>0,return NULL;);
  r = avo_alloc_internal(pr,tab[0]->dim,tab[0]->intdim);
  for (k=0;k<size;k++) {
    arg_assert(tab[k]->dim==r->dim && tab[k]->intdim==r->intdim,
	       avo_free_internal(pr,r);return NULL;);
    if (algo>=0) avo_cache_closure(pr,tab[k]);
    /* skip definitively empty */
    if (!tab[k]->m && !tab[k]->closed) continue;
    if (!m)
      /* first non-empty */
    {
      m = avo_hmat_copy(pr,tab[k]->closed ? tab[k]->closed : tab[k]->m,r->dim);
      nsc = avo_hmat_copy(pr,tab[k]->nsc,r->dim);
    }
    else {
      /* not first non-empty */
      bound_t* nsc_x = tab[k]->nsc;
      bound_t* x = tab[k]->closed ? tab[k]->closed : tab[k]->m;
      for (i=0;i<avo_matsize(r->dim);i++)
      {
    	  bound_bmax_nsc(m+i,nsc+i,x[i],nsc_x[i]);
      }
    }
    if (!tab[k]->closed) closed = false;
  }

  if (!m) {
    /* empty result */
  }
  else if (closed) { 
    /* closed, optimal result, in Q */
    man->result.flag_exact = false;
    r->closed = m; 
    r->nsc = nsc;
    if (num_incomplete || r->intdim) flag_incomplete;
  }
  else {
    /* non closed, non optimal result */
    r->m = m;
    r->nsc = nsc;
    flag_algo; 
  }
  return r;
}



/* ============================================================ */
/* Widening, Narrowing */
/* ============================================================ */

avo_t* avo_widening(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  int algo = pr->funopt->algorithm;
  avo_t* r;
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
  if (algo>=0) avo_cache_closure(pr,a2);
  if (!a1->closed && !a1->m)
    /* a1 definitively closed */
    r = avo_copy_internal(pr,a2);
  else if (!a2->closed && !a2->m)
   /* a2 definitively closed */
    r = avo_copy_internal(pr,a1);
  else {
    /* work on the origial left matrix, not the closed cache! */
    bound_t* m1 = a1->m ? a1->m : a1->closed;
    bound_t* m2 = a2->closed ? a2->closed : a2->m;
    bound_t* n1 = a1->nsc;
    bound_t* n2 = a2->nsc;
    size_t i;
    r = avo_alloc_internal(pr,a1->dim,a1->intdim);
    r->m = avo_hmat_alloc(pr,r->dim);
    r->nsc = avo_hmat_alloc(pr,r->dim);
    if (algo==avo_pre_widening || algo==-avo_pre_widening) {
      /* degenerate hull: NOT A PROPER WIDENING, use with care */
      for (i=0;i<avo_matsize(r->dim);i++){
    	  bound_max(r->m[i],m1[i],m2[i]);
      	  bound_max(r->nsc[i],n1[i],n2[i]);
      }
    }
    else {
      /* standard widening */
      for (i=0;i<avo_matsize(r->dim);i++)
		if (bound_cmp(m1[i],m2[i])>0)
		{
			bound_set(r->m[i],m1[i]);
			bound_set(r->nsc[i],n1[i]);
		}
		else if (bound_cmp(m1[i],m2[i])==0)
		{
			bound_set(r->m[i],m1[i]);
			bound_max(r->nsc[i],n1[i],n2[i]);
		}
		else
		{
			bound_set_infty(r->m[i],1);
			bound_set_infty(r->nsc[i],1);
		}
	}
  }
  return r;
}

avo_t* avo_widening_thresholds(ap_manager_t* man,
			       avo_t* a1, avo_t* a2,
			       ap_scalar_t** array,
			       size_t nb)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,nb+1);
  int algo = pr->funopt->algorithm;
  avo_t* r;
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
  if (algo>=0) avo_cache_closure(pr,a2);
  if (!a1->closed && !a1->m)
    /* a1 definitively closed */
    r = avo_copy_internal(pr,a2);
  else if (!a2->closed && !a2->m)
   /* a2 definitively closed */
    r = avo_copy_internal(pr,a1);
  else {
    /* work on the origial left matrix, not the closed cache! */
    bound_t* m1 = a1->m ? a1->m : a1->closed;
    bound_t* m2 = a2->closed ? a2->closed : a2->m;
    size_t i;
    r = avo_alloc_internal(pr,a1->dim,a1->intdim);
    r->m = avo_hmat_alloc(pr,r->dim);
    r->nsc = avo_hmat_alloc(pr,r->dim);

    /* convert array to bounds */
    for (i=0;i<nb;i++)
      avo_bound_of_scalar(pr,pr->tmp[i],array[i],false,false);
    bound_set_infty(pr->tmp[nb],1);
    /* point-wise loop */
    for (i=0;i<avo_matsize(r->dim);i++)
      if (bound_cmp(m1[i],m2[i])>0){
    	  bound_set(r->m[i],m1[i]);
    	  bound_set(r->nsc[i],a1->nsc[i]);
      }
      else if (bound_cmp(m1[i],m2[i])==0){
    	  bound_set(r->m[i],m1[i]);
    	  if(bound_cmp(a1->nsc[i],a2->nsc[i])>=0)
    		  bound_set(r->nsc[i],a1->nsc[i]);
    	  else
    		  bound_set(r->nsc[i],a2->nsc[i]);
      }
      else {
		size_t j;
		for (j=0;j<nb;j++)
		  if (bound_cmp(m2[i],pr->tmp[j])<=0) break;
		bound_set(r->m[i],pr->tmp[j]);
		bound_set_infty(r->nsc[i],1);
      }
    /* warn user for conv errors in thresolds */
    if (pr->conv) flag_conv;
  }
  return r;
}

avo_t* avo_narrowing(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  avo_t* r;
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
  if (pr->funopt->algorithm>=0) {
    avo_cache_closure(pr,a1);
    avo_cache_closure(pr,a2);
  }
  r = avo_alloc_internal(pr,a1->dim,a1->intdim);
  if ((!a1->closed && !a1->m) || (!a2->closed && !a2->m)) {
    /* a1 or a2 definitively closed */
  }
  else {
    bound_t* m1 = a1->closed ? a1->closed : a1->m;
    bound_t* m2 = a2->closed ? a2->closed : a2->m;
    size_t i;
    r->m = avo_hmat_alloc(pr,r->dim);
    r->nsc = avo_hmat_alloc_top_nsc(pr,r->dim);
    for (i=0;i<avo_matsize(r->dim);i++){
    	if(bound_infty(m1[i])){
    		bound_set(r->m[i], m2[i]);
    		bound_set(r->nsc[i], a2->nsc[i]);
    	}
    	else{
    		bound_set(r->m[i], m1[i]);
    		bound_set(r->nsc[i], a1->nsc[i]);
    	}
    }
  }
  return r;
}

ap_abstract0_t* ap_abstract0_avo_narrowing( ap_manager_t* man,
					    ap_abstract0_t* a1,
					    ap_abstract0_t* a2 )
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  avo_t* a = (avo_t*) (a1->value);
  arg_assert(man->library==a1->man->library &&
	     man->library==a2->man->library,
	     return abstract0_of_avo(man,avo_alloc_top(pr,a->dim,a->intdim)););
  return abstract0_of_avo(man,avo_narrowing
			  (man,a1->value,a2->value));
}

ap_abstract0_t* 
ap_abstract0_avo_widening_thresholds(ap_manager_t* man,
				     ap_abstract0_t* a1, 
				     ap_abstract0_t* a2,
				     ap_scalar_t** array,
				     size_t nb)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  avo_t* a = (avo_t*) (a1->value);
  arg_assert(man->library==a1->man->library &&
	     man->library==a2->man->library,
	     return abstract0_of_avo(man,avo_alloc_top(pr,a->dim,a->intdim)););
  return 
    abstract0_of_avo(man,avo_widening_thresholds
		     (man,a1->value,a2->value,array,nb));
}


/* ============================================================ */
/* Perturbation */
/* ============================================================ */

avo_t* avo_add_epsilon(ap_manager_t* man, avo_t* a, ap_scalar_t* epsilon)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,2);
  avo_t* r = avo_alloc_internal(pr,a->dim,a->intdim);
  bound_t* m;
  m = a->m ? a->m : a->closed;
  if (m) {
    size_t i;
    /* compute max of finite bounds */
    bound_set_int(pr->tmp[0],0);
    for (i=0;i<avo_matsize(a->dim);i++) {
      if (bound_infty(m[i])) continue;
      if (bound_sgn(m[i])>=0) 
	bound_max(pr->tmp[0],pr->tmp[0],m[i]);
      else {
	bound_neg(pr->tmp[1],m[i]);
	bound_max(pr->tmp[0],pr->tmp[0],pr->tmp[1]);
      }
    }
    /* multiply by epsilon */
    avo_bound_of_scalar(pr,pr->tmp[1],epsilon,false,false);
    bound_mul(pr->tmp[0],pr->tmp[0],pr->tmp[1]);
    /* enlarge bounds */
    r->m = avo_hmat_alloc(pr,r->dim);
    r->nsc = avo_hmat_alloc_top_nsc(pr,r->dim);
    for (i=0;i<avo_matsize(r->dim);i++)
      bound_add(r->m[i],m[i],pr->tmp[0]);
  }
  return r;
}

ap_abstract0_t* 
ap_abstract0_avo_add_epsilon(ap_manager_t* man,
			     ap_abstract0_t* a1, 
			     ap_scalar_t* epsilon)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  avo_t* a = (avo_t*) (a1->value);
  arg_assert(man->library==a1->man->library,
	     return abstract0_of_avo(man,avo_alloc_top(pr,a->dim,a->intdim)););
  return abstract0_of_avo(man,avo_add_epsilon(man,a,epsilon));
}


avo_t* avo_add_epsilon_bin(ap_manager_t* man, avo_t* a1, avo_t* a2,
			   ap_scalar_t* epsilon)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,2);
  avo_t* r;
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
  if (!a1->closed && !a1->m)
    /* a1 definitively closed */
    r = avo_copy_internal(pr,a2);
  else if (!a2->closed && !a2->m)
   /* a2 definitively closed */
    r = avo_copy_internal(pr,a1);
  else {
    bound_t* m1 = a1->m ? a1->m : a1->closed;
    bound_t* m2 = a2->m ? a2->m : a2->closed;
    size_t i;
    r = avo_alloc_internal(pr,a1->dim,a1->intdim);
    r->m = avo_hmat_alloc(pr,r->dim);
    r->nsc = avo_hmat_alloc_top_nsc(pr,r->dim);
    /* get max abs of non +oo coefs in m2, times epsilon */
    bound_set_int(pr->tmp[0],0);
    for (i=0;i<avo_matsize(a1->dim);i++) {
      if (bound_infty(m2[i])) continue;
      if (bound_sgn(m2[i])>=0) 
    	  bound_max(pr->tmp[0],pr->tmp[0],m2[i]);
      else {
		bound_neg(pr->tmp[1],m2[i]);
		bound_max(pr->tmp[0],pr->tmp[0],pr->tmp[1]);
      }
    }
    /* multiply by epsilon */
    avo_bound_of_scalar(pr,pr->tmp[1],epsilon,false,false);
    bound_mul(pr->tmp[0],pr->tmp[0],pr->tmp[1]);
    /* enlarge unstable coefficients in a1 */
    for (i=0;i<avo_matsize(a1->dim);i++)
      if (bound_cmp(m1[i],m2[i])<0) bound_add(r->m[i],m2[i],pr->tmp[0]);
      else bound_set(r->m[i],m1[i]);
  }
  return r;
}

ap_abstract0_t* 
ap_abstract0_avo_add_epsilon_bin(ap_manager_t* man,
				 ap_abstract0_t* a1, 
				 ap_abstract0_t* a2, 
				 ap_scalar_t* epsilon)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_WIDENING,0);
  avo_t* a = (avo_t*) (a1->value);
  arg_assert(man->library==a1->man->library &&
	     man->library==a2->man->library,
	     return abstract0_of_avo(man,avo_alloc_top(pr,a->dim,a->intdim)););
  return abstract0_of_avo(man,avo_add_epsilon_bin(man,a1->value,a2->value,epsilon));
}

