/*
 * avo_representation.c
 *
 * Simple functions related to the representation of AV octagons
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

extern void tighten_nsc(bound_t* m, bound_t* nsc, size_t dim);

/* ============================================================ */
/* Memory Related */
/* ============================================================ */

/* generic allocation routine, returns a bottom avOctagon */
inline avo_t* avo_alloc_internal(avo_internal_t* pr, size_t dim, size_t intdim)
{
  avo_t* r;
  checked_malloc(r,avo_t,1,return NULL;);
  r->intdim = intdim;
  r->dim = dim;
  r->closed = r->m = NULL;
  r->nsc = NULL;
  return r;
}

inline avo_t* avo_alloc_top(avo_internal_t* pr, size_t dim, size_t intdim)
{
  avo_t* r = avo_alloc_internal(pr,dim,intdim);
  r->closed = avo_hmat_alloc_top(pr,dim);
  return r;
}

inline void avo_free_internal(avo_internal_t* pr, avo_t* a)
{
  if (a->m) avo_hmat_free(pr,a->m,a->dim);
  if (a->closed) avo_hmat_free(pr,a->closed,a->dim);
  if (a->nsc) avo_hmat_free(pr,a->nsc,a->dim);
  a->closed = a->m = a->nsc = NULL;
  free(a);
}

inline avo_t* avo_copy_internal(avo_internal_t* pr, avo_t* a)
{
  avo_t* r = avo_alloc_internal(pr,a->dim,a->intdim);
  r->m = avo_hmat_copy(pr,a->m,a->dim);
  r->closed = avo_hmat_copy(pr,a->closed,a->dim);
  r->nsc = avo_hmat_copy(pr,a->nsc,a->dim);
  return r;
}

avo_t* avo_copy(ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_COPY\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_COPY,0);
  return avo_copy_internal(pr,a);
}

void avo_free(ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_FREE\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FREE,0);
  avo_free_internal(pr,a);
}

size_t avo_size(ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_ASIZE\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_ASIZE,0);
  if (!a->m) return 1;
  return avo_matsize(a->dim);
}

avo_t* avo_set_mat_nsc(avo_internal_t* pr, avo_t* a, bound_t* m, bound_t* closed,bound_t * nsc,
		   bool destructive)
{
  avo_t* r;
  if (destructive) {
    /* free non-aliased matrices */
    if (a->m && a->m!=m && a->m!=closed)
      avo_hmat_free(pr,a->m,a->dim);
    if (a->closed && a->closed!=m && a->closed!=closed)
      avo_hmat_free(pr,a->closed,a->dim);
    if(a->nsc && a->nsc != nsc)
    	 avo_hmat_free(pr,a->nsc,a->dim);
    r = a;
  }
  else {
    /* copy aliased matrices */
    r = avo_alloc_internal(pr,a->dim,a->intdim);
    if (m && (a->m==m || a->closed==m)) m = avo_hmat_copy(pr,m,a->dim);
    if (closed && (a->m==closed || a->closed==closed))
        closed = avo_hmat_copy(pr,closed,a->dim);
    if (nsc && (a->nsc == nsc))
        nsc = avo_hmat_copy(pr,nsc,a->dim);
  }
  r->nsc = nsc;
  r->m = m;
  r->closed = closed;
  return r;
}


/* ============================================================ */
/* Basic Constructors */
/* ============================================================ */

avo_t* avo_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
	// fprintf(stdout, "AP_FUNID_BOTTOM\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_BOTTOM,0);
  avo_t* r = avo_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

avo_t* avo_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
	//fprintf(stdout, "AP_FUNID_TOP\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_TOP,0);
  avo_t* r = avo_alloc_internal(pr,intdim+realdim,intdim);
  r->closed = avo_hmat_alloc_top(pr,r->dim);
  r->nsc = avo_hmat_alloc_top_nsc(pr,r->dim);
  return r;
}

avo_t* avo_of_box(ap_manager_t* man, size_t intdim, size_t realdim,
		  ap_interval_t ** t)
{
	//fprintf(stdout, "AP_FUNID_OF_BOX\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_OF_BOX,0);
  avo_t* r = avo_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,n2;
  if (!t) return r; /* empty */
  for (i=0;i<r->dim;i++)
    if (ap_scalar_cmp(t[i]->inf,t[i]->sup)>0) return r; /* empty */
  r->closed = avo_hmat_alloc_top(pr,r->dim);
  r->nsc = avo_hmat_alloc_top_nsc(pr,r->dim);

  for (i=0;i<r->dim;i++)
    if (avo_bounds_of_interval(pr,
			   r->closed[avo_matpos(2*i,2*i+1)],
			   r->closed[avo_matpos(2*i+1,2*i)],
			   t[i],true)) {
      /* one interval is empty -> the result is empty */
      avo_hmat_free(pr,r->closed,r->dim);
      avo_hmat_free(pr,r->nsc,r->dim);
      r->closed = NULL;
      r->nsc = NULL;
      return r;
    }
  /* tighten + a S step is sufficient to ensure clsoure */
  tighten_nsc(r->closed,r->nsc,r->dim);
  if (avo_hmat_s_step(r->closed,r->nsc,r->dim)){
    /* definitively empty */
    avo_hmat_free(pr,r->closed,r->dim);
    avo_hmat_free(pr,r->nsc,r->dim);
    r->closed = NULL;
    r->nsc = NULL;
  }

  /* exact, except for conversion errors */
  if (pr->conv) flag_conv;

  return r;
}

avo_t* avo_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* ar)
{
  // fprintf(stdout, "AP_FUNID_ADD_RAY_ARRAY\n"); fflush(stdout);
  size_t dim = intdim+realdim;
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,
					     2*(dim+1));
  avo_t* r = avo_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,k;

  /* find one vertex */
  for (i=0;i<ar->size;i++) {
    bound_t* b;
    if (ar->p[i].gentyp!=AP_GEN_VERTEX) continue;
    avo_bounds_of_generator(pr,pr->tmp,ar->p[i].linexpr0,dim);
    r->m = avo_hmat_alloc_top(pr,dim);
    for (b=r->m,j=0;j<2*dim;j++)
      for (k=0;k<=(j|1);k++,b++) bound_sub(*b,pr->tmp[j],pr->tmp[k]);
    break;
  }

  if (r->m) {
    /* not empty */
    avo_hmat_add_generators(pr,r->m,dim,ar);
    /* best in Q and Z, except for conversion errors */
    man->result.flag_exact = false;
    if (pr->conv) flag_conv;
  }
  return r;
}

ap_abstract0_t*
ap_abstract0_avo_of_generator_array(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_generator0_array_t* array)
{
  return abstract0_of_avo(man,avo_of_generator_array(man,intdim,realdim,array));
}


/* ============================================================ */
/* Accessors */
/* ============================================================ */

ap_dimension_t avo_dimension(ap_manager_t* man, avo_t* a)
{
	 //fprintf(stdout, "AP_FUNID_DIMENSION\n"); fflush(stdout);
	avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_DIMENSION,0);
	ap_dimension_t r;
	r.intdim = a->intdim;
	r.realdim = a->dim-a->intdim;
	return r;
}


/* ============================================================ */
/* Closure */
/* ============================================================ */

/* Make sure the closed version of the matrix a->closed is available.
   If a->closed when it returns, this means the avOctagon is empty.
   If the avOctagon is not empty, a->m is not modified.
   => this DOES NOT affect the semantics of functions that rely on a->m
   (e.g., widening)
 */
void avo_cache_closure(avo_internal_t* pr, avo_t* a)
{
  if (a->closed || !a->m) return;
  a->closed = avo_hmat_copy(pr,a->m,a->dim);
  if (avo_hmat_close(a->closed,a->nsc,a->dim)) {
    /* empty! */
    avo_hmat_free(pr,a->m,a->dim);
    avo_hmat_free(pr,a->closed,a->dim);
    avo_hmat_free(pr,a->nsc,a->dim);
    a->m = a->closed = a->nsc =  NULL;
  }
}

/* Unlike avo_cache_closure, this frees the a->m representation, forcing
   all further transfer functions to work on the closed version...
   => this changes the semantics of functions such as widening

*/
void avo_close(avo_internal_t* pr, avo_t* a)
{
  if (!a->m) return;
  if (a->closed) {
    avo_hmat_free(pr,a->m,a->dim);
    a->m = NULL;
    return;
  }
  a->closed = a->m;
  a->m = NULL;
  if (avo_hmat_close(a->closed,a->nsc,a->dim)) {
    avo_hmat_free(pr,a->closed,a->dim);
    avo_hmat_free(pr,a->nsc,a->dim);
    a->closed = NULL;
    a->nsc = NULL;
  }
}


/* ============================================================ */
/* Control of internal representation: NOT IMPLEMENTED */
/* ============================================================ */

/* NOT IMPLEMENTED: do nothing */
void avo_minimize(ap_manager_t* man, avo_t* a)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_MINIMIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
void avo_canonicalize(ap_manager_t* man, avo_t* a)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_CANONICALIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
int avo_hash(ap_manager_t* man, avo_t* a)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_HASH,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  return 0;
}

/* NOT IMPLEMENTED: do nothing */
void avo_approximate(ap_manager_t* man, avo_t* a, int algorithm)
{
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_APPROXIMATE,0);
  return;
}

/* ============================================================ */
/* Topological losure operation */
/* ============================================================ */

/* actually identity */
avo_t* avo_closure(ap_manager_t* man, bool destructive, avo_t* a)
{
	//fprintf(stdout, "AP_FUNID_CLOSURE\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_CLOSURE,0);
  if (destructive) return a;
  else return avo_copy_internal(pr,a);
}



/* ============================================================ */
/* Managers */
/* ============================================================ */

void avo_internal_free(avo_internal_t* pr)
{
  bound_clear_array(pr->tmp,pr->tmp_size);
  free(pr->tmp);
  free(pr->tmp2);
  free(pr);
}

ap_manager_t* avo_manager_alloc(void)
{
  size_t i;
  ap_manager_t* man;
  avo_internal_t* pr;

#if num_fpu
  if (!ap_fpu_init()) {
    fprintf(stderr,"avo_manager_alloc cannot change the FPU rounding mode\n");
    return NULL;
  }
#endif
  pr = (avo_internal_t*)malloc(sizeof(avo_internal_t));
  assert(pr);
  pr->tmp_size = 10;
  pr->tmp = malloc(sizeof(bound_t)*pr->tmp_size);
  assert(pr->tmp);
  bound_init_array(pr->tmp,pr->tmp_size);
  pr->tmp2 = malloc(sizeof(long)*pr->tmp_size);
  assert(pr->tmp2);

  man = ap_manager_alloc("avo","1.0 with " num_name,pr,
			 (void (*)(void*))avo_internal_free);

  pr->man = man;

  man->funptr[AP_FUNID_COPY] = &avo_copy;
  man->funptr[AP_FUNID_FREE] = &avo_free;
  man->funptr[AP_FUNID_ASIZE] = &avo_size;
  man->funptr[AP_FUNID_MINIMIZE] = &avo_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = &avo_canonicalize;
  man->funptr[AP_FUNID_HASH] = &avo_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = &avo_approximate;
  man->funptr[AP_FUNID_FPRINT] = &avo_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = &avo_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = &avo_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = &avo_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = &avo_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = &avo_bottom;
  man->funptr[AP_FUNID_TOP] = &avo_top;
  man->funptr[AP_FUNID_OF_BOX] = &avo_of_box;
  man->funptr[AP_FUNID_DIMENSION] = &avo_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = &avo_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = &avo_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = &avo_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = &avo_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &avo_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = &avo_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = &avo_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = &avo_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = &avo_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = &avo_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = &avo_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = &avo_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = &avo_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = &avo_to_tcons_array;
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &avo_to_generator_array;
  man->funptr[AP_FUNID_MEET] = &avo_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = &avo_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &avo_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = &avo_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = &avo_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = &avo_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = &avo_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &avo_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &avo_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &avo_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &avo_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = &avo_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = &avo_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &avo_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = &avo_forget_array;
  man->funptr[AP_FUNID_EXPAND] = &avo_expand;
  man->funptr[AP_FUNID_FOLD] = &avo_fold;
  man->funptr[AP_FUNID_WIDENING] = &avo_widening;
  man->funptr[AP_FUNID_CLOSURE] = &avo_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,i,false);

  return man;
}

avo_t* avo_of_abstract0(ap_abstract0_t* a)
{
  return (avo_t*)a->value;
}

ap_abstract0_t* abstract0_of_avo(ap_manager_t* man, avo_t* avo)
{
  ap_abstract0_t* r = malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = avo;
  r->man = ap_manager_copy(man);
  return r;
}
