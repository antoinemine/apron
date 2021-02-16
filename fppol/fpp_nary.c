/*
 * fpp_nary.c
 *
 * N-ary fppol functions: meet, join, widening, narrowing & related.
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */


/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#include "fpp.h"
#include "fpp_internal.h"
#include "fppol_domain.h"
#include "ap_generic.h"

/* ============================================================ */
/* Meet and Join */
/* ============================================================ */

fpp_t* fpp_meet(ap_manager_t* man, bool destructive, fpp_t* a1, fpp_t* a2)
{
  fpp_t* a3;
  fprintf(stdout, "AP_FUNID_MEET\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_MEET,0);
  if(destructive==true)  a3=fppol_intersection(pr,true, a1, a2);
  else a3=fppol_intersection(pr,false, a1, a2);
  return a3;
}

fpp_t* fpp_join(ap_manager_t* man, bool destructive, fpp_t* a1, fpp_t* a2)
{
  fprintf(stdout, "AP_FUNID_JOIN\n");
  fflush(stdout);
int nlp0=nlp;
/* gettimeofday(&tpstart,NULL); */

 fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_JOIN,0);
 arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,return NULL;);
 if (pr->funopt->algorithm >= 0) {/* polyhedral strong join (convex hull) */
 }
 else{ /* week join (unimplemented at present)*/
 }
#if STRONG_JOIN
  fpp_t* a3=fppol_strong_join(pr,a1,a2);
#else
  fpp_t* a3=fppol_joins(pr,a1,a2);
#endif

nlpjoin=nlpjoin+nlp-nlp0;

 if(destructive==true)  fpp_free_internal(pr,a1);
 return a3;
}

fpp_t* fpp_meet_array(ap_manager_t* man, fpp_t** tab, size_t size)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_MEET_ARRAY,0);

  fprintf(stdout, "AP_FUNID_MEET_ARRAY\n");
  fflush(stdout);

  fpp_t* r;
  size_t i,k;
  arg_assert(size>0,return NULL;);
  r = fpp_alloc_internal(pr,tab[0]->dim,tab[0]->intdim);
  return r;
}

fpp_t* fpp_join_array(ap_manager_t* man, fpp_t** tab, size_t size)
{
  fpp_t* a;
  fprintf(stdout, "AP_FUNID_JOIN_ARRAY\n");
  fflush(stdout);
  a=ap_generic_meetjoin_array(false,man,(void **)tab, size);
  return a;
}



/* ============================================================ */
/* Widening, Narrowing */
/* ============================================================ */

fpp_t* fpp_widening(ap_manager_t* man, fpp_t* a1, fpp_t* a2)
{
  fpp_t* a3;

  fprintf(stdout, "AP_FUNID_WIDENING1\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);

#if WIDENBND
  a3=fppol_widen_standard_withBound(pr,false,a1,a2);
#else
  a3=fppol_widen_standard(pr,false,a1,a2);
#endif

  return a3;
}

fpp_t* fpp_widening_thresholds(ap_manager_t* man,
			       fpp_t* a1, fpp_t* a2,
			       ap_scalar_t** array,
			       size_t nb)
{
  fprintf(stdout, "AP_FUNID_WIDENING2\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,nb+1);
  return a1;
}

fpp_t* fpp_narrowing(ap_manager_t* man, fpp_t* a1, fpp_t* a2)
{
  fprintf(stdout, "AP_FUNID_WIDENING3\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);
  return a1;
}

ap_abstract0_t* ap_abstract0_fpp_narrowing( ap_manager_t* man,
					    ap_abstract0_t* a1,
					    ap_abstract0_t* a2 )
{
  fprintf(stdout, "AP_FUNID_WIDENING4\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);
  fpp_t* a = (fpp_t*) (a1->value);
  return a1;
}

ap_abstract0_t*
ap_abstract0_fpp_widening_thresholds(ap_manager_t* man,
				     ap_abstract0_t* a1,
				     ap_abstract0_t* a2,
				     ap_scalar_t** array,
				     size_t nb)
{
  fprintf(stdout, "AP_FUNID_WIDENING5\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);
  return a1;
}


/* ============================================================ */
/* Perturbation */
/* ============================================================ */

fpp_t* fpp_add_epsilon(ap_manager_t* man, fpp_t* a, ap_scalar_t* epsilon)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,2);
  fpp_t* r = fpp_alloc_internal(pr,a->dim,a->intdim);
  return r;
}

ap_abstract0_t*
ap_abstract0_fpp_add_epsilon(ap_manager_t* man,
			     ap_abstract0_t* a1,
			     ap_scalar_t* epsilon)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);
  fpp_t* a = (fpp_t*) (a1->value);
  return a1;
}


fpp_t* fpp_add_epsilon_bin(ap_manager_t* man, fpp_t* a1, fpp_t* a2,
			   ap_scalar_t* epsilon)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,2);
  fpp_t* r;
  return a1;
}

ap_abstract0_t*
ap_abstract0_fpp_add_epsilon_bin(ap_manager_t* man,
				 ap_abstract0_t* a1,
				 ap_abstract0_t* a2,
				 ap_scalar_t* epsilon)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_WIDENING,0);
  fpp_t* a = (fpp_t*) (a1->value);
  return a1;
}
