/*
 * itvp_nary.c
 *
 * N-ary itvpol functions: meet, join, widening, narrowing & related.
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include "itvp.h"
#include "itvp_internal.h"
#include "itvp_domain.h"
#include "ap_generic.h"

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

itvp_t* itvp_meet(ap_manager_t* man, bool destructive, itvp_t* a1, itvp_t* a2)
{
  itvp_t* a3;
  fprintf(stdout, "AP_FUNID_MEET\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_MEET,0);
  if(destructive==true)  a3=itvpol_intersection(pr,true, a1, a2);
  else a3=itvpol_intersection(pr,false, a1, a2);
  return a3;
}

itvp_t* itvp_join(ap_manager_t* man, bool destructive, itvp_t* a1, itvp_t* a2)
{
  fprintf(stdout, "AP_FUNID_JOIN\n");
  fflush(stdout);

 itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_JOIN,0);
 arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
 if (pr->funopt->algorithm >= 0) {/* polyhedral strong join (convex hull) */
 }
 else{ /* week join (unimplemented at present)*/
 }
 itvp_t* a3=itvpol_weak_join(pr,a1,a2);

 if(destructive==true)  itvp_free_internal(pr,a1);
 return a3;
}

itvp_t* itvp_meet_array(ap_manager_t* man, itvp_t** tab, size_t size)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_MEET_ARRAY,0);

  fprintf(stdout, "AP_FUNID_MEET_ARRAY\n");
  fflush(stdout);

  itvp_t* r;
  size_t i,k;
  arg_assert(size>0,return NULL;);
  r = itvp_alloc_internal(pr,tab[0]->dim,tab[0]->intdim);
  return r;
}

itvp_t* itvp_join_array(ap_manager_t* man, itvp_t** tab, size_t size)
{
  itvp_t* a;
  fprintf(stdout, "AP_FUNID_JOIN_ARRAY\n");
  fflush(stdout);

  a=(itvp_t* )ap_generic_meetjoin_array(false,man,(void **)tab, size);

  return a;
}



/* ============================================================ */
/* Widening, Narrowing */
/* ============================================================ */

itvp_t* itvp_widening(ap_manager_t* man, itvp_t* a1, itvp_t* a2)
{
  itvp_t* a3;

  fprintf(stdout, "AP_FUNID_WIDENING1\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);

#if WIDENBND
  a3=itvpol_widen_standard_withBound(pr,false,a1,a2);
#else
  a3=itvpol_widen_standard(pr,false,a1,a2);
#endif

  return a3;
}

itvp_t* itvp_widening_thresholds(ap_manager_t* man,
			       itvp_t* a1, itvp_t* a2,
			       ap_scalar_t** array,
			       size_t nb)
{
  fprintf(stdout, "AP_FUNID_WIDENING2\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,nb+1);
  return a1;
}

itvp_t* itvp_narrowing(ap_manager_t* man, itvp_t* a1, itvp_t* a2)
{
  fprintf(stdout, "AP_FUNID_WIDENING3\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);
  return a1;
}

ap_abstract0_t* ap_abstract0_itvp_narrowing( ap_manager_t* man,
					    ap_abstract0_t* a1,
					    ap_abstract0_t* a2 )
{
  fprintf(stdout, "AP_FUNID_WIDENING4\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);
  itvp_t* a = (itvp_t*) (a1->value);
  return a1;
}

ap_abstract0_t* 
ap_abstract0_itvp_widening_thresholds(ap_manager_t* man,
				     ap_abstract0_t* a1, 
				     ap_abstract0_t* a2,
				     ap_scalar_t** array,
				     size_t nb)
{
  fprintf(stdout, "AP_FUNID_WIDENING5\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);
  return a1;
}


/* ============================================================ */
/* Perturbation */
/* ============================================================ */

itvp_t* itvp_add_epsilon(ap_manager_t* man, itvp_t* a, ap_scalar_t* epsilon)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,2);
  itvp_t* r = itvp_alloc_internal(pr,a->dim,a->intdim);
  return r;
}

ap_abstract0_t* 
ap_abstract0_itvp_add_epsilon(ap_manager_t* man, 
			     ap_abstract0_t* a1, 
			     ap_scalar_t* epsilon)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);
  itvp_t* a = (itvp_t*) (a1->value);
  return a1;
}


itvp_t* itvp_add_epsilon_bin(ap_manager_t* man, itvp_t* a1, itvp_t* a2, 
			   ap_scalar_t* epsilon)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,2);
  itvp_t* r;
  return a1;
}

ap_abstract0_t* 
ap_abstract0_itvp_add_epsilon_bin(ap_manager_t* man, 
				 ap_abstract0_t* a1, 
				 ap_abstract0_t* a2, 
				 ap_scalar_t* epsilon)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_WIDENING,0);
  itvp_t* a = (itvp_t*) (a1->value);
  return a1;
}

