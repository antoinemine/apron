/*
 * oct_representation.c
 *
 * Simple functions related to the representation of octagons
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#include "oct.h"
#include "oct_internal.h"
#include "ap_generic.h"


/* ============================================================ */
/* Memory Related */
/* ============================================================ */

/* generic allocation routine, returns a bottom octagon */
inline oct_t* oct_alloc_internal(oct_internal_t* pr, size_t dim, size_t intdim)
{
  oct_t* r;
  checked_malloc(r,oct_t,1,return NULL;);
  r->intdim = intdim;
  r->dim = dim;
  r->closed = r->m = NULL;
  return r;
}

inline oct_t* oct_alloc_top(oct_internal_t* pr, size_t dim, size_t intdim)
{
  oct_t* r = oct_alloc_internal(pr,dim,intdim);
  r->closed = hmat_alloc_top(pr,dim);
  return r;
}

inline void oct_free_internal(oct_internal_t* pr, oct_t* a)
{
  if (a->m) hmat_free(pr,a->m,a->dim);
  if (a->closed) hmat_free(pr,a->closed,a->dim);
  a->closed = a->m = NULL;
  free(a);
}

inline oct_t* oct_copy_internal(oct_internal_t* pr, oct_t* a)
{
  oct_t* r = oct_alloc_internal(pr,a->dim,a->intdim);
  r->m = hmat_copy(pr,a->m,a->dim);
  r->closed = hmat_copy(pr,a->closed,a->dim);
  return r;
}

oct_t* oct_copy(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_COPY,0);
  return oct_copy_internal(pr,a);
}

void oct_free(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_FREE,0);
  oct_free_internal(pr,a);
}

size_t oct_size(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_ASIZE,0);
  if (!a->m) return 1;
  return matsize(a->dim);
}

/* If destructive, returns a with fields m and closed updated
   (former fields of a, if not reused in m or closed, are destroyed).
   If not destructive, returns a new octagon with same dimensions as a
   and m and closed as half-matrices.
   m and closed can safely alias fields in a
 */
oct_t* oct_set_mat(oct_internal_t* pr, oct_t* a, bound_t* m, bound_t* closed,
		   bool destructive)
{
  oct_t* r;
  if (destructive) {
    /* free non-aliased matrices */
    if (a->m && a->m!=m && a->m!=closed)
      hmat_free(pr,a->m,a->dim);
    if (a->closed && a->closed!=m && a->closed!=closed)
      hmat_free(pr,a->closed,a->dim);
    r = a;
  }
  else {
    /* copy aliased matrices */
    r = oct_alloc_internal(pr,a->dim,a->intdim);
    if (m && (a->m==m || a->closed==m)) m = hmat_copy(pr,m,a->dim);
    if (closed && (a->m==closed || a->closed==closed))
      closed = hmat_copy(pr,closed,a->dim);
  }
  r->m = m;
  r->closed = closed;
  return r;
}


/* ============================================================ */
/* Basic Constructors */
/* ============================================================ */

oct_t* oct_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_BOTTOM,0);
  oct_t* r = oct_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

oct_t* oct_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_TOP,0);
  oct_t* r = oct_alloc_internal(pr,intdim+realdim,intdim);
  r->closed = hmat_alloc_top(pr,r->dim);
  return r;
}

oct_t* oct_of_box(ap_manager_t* man, size_t intdim, size_t realdim,
		  ap_interval_t ** t)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_OF_BOX,0);
  oct_t* r = oct_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,n2;
  if (!t) return r; /* empty */
  for (i=0;i<r->dim;i++)
    if (ap_scalar_cmp(t[i]->inf,t[i]->sup)>0) return r; /* empty */
  r->closed = hmat_alloc_top(pr,r->dim);
  for (i=0;i<r->dim;i++)
    if (bounds_of_interval(pr,
			   r->closed[matpos(2*i,2*i+1)],
			   r->closed[matpos(2*i+1,2*i)],
			   t[i],true)) {
      /* one interval is empty -> the result is empty */
      hmat_free(pr,r->closed,r->dim);
      r->closed = NULL;
      return r;
    }
  /* a S step is sufficient to ensure clsoure */
  if (hmat_s_step(r->closed,r->dim)){
    /* definitively empty */
    hmat_free(pr,r->closed,r->dim);
    r->closed = NULL;
  }

  /* exact, except for conversion errors */
  if (pr->conv) flag_conv;
  return r;
}

oct_t* oct_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* ar)
{
  size_t dim = intdim+realdim;
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,
					     2*(dim+1));
  oct_t* r = oct_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,k;

  /* find one vertex */
  for (i=0;i<ar->size;i++) {
    bound_t* b;
    if (ar->p[i].gentyp!=AP_GEN_VERTEX) continue;
    bounds_of_generator(pr,pr->tmp,ar->p[i].linexpr0,dim);
    r->m = hmat_alloc_top(pr,dim);
    for (b=r->m,j=0;j<2*dim;j++)
      for (k=0;k<=(j|1);k++,b++) bound_sub(*b,pr->tmp[j],pr->tmp[k]);
    break;
  }

  if (r->m) {
    /* not empty */
    hmat_add_generators(pr,r->m,dim,ar);
    /* best in Q and Z, except for conversion errors */
    man->result.flag_exact = false;
    if (pr->conv) flag_conv;
  }
  return r;
}

ap_abstract0_t*
ap_abstract0_oct_of_generator_array(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_generator0_array_t* array)
{
  return abstract0_of_oct(man,oct_of_generator_array(man,intdim,realdim,array));
}


/* ============================================================ */
/* Accessors */
/* ============================================================ */

ap_dimension_t oct_dimension(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_DIMENSION,0);
  ap_dimension_t r;
  r.intdim = a->intdim;
  r.realdim = a->dim-a->intdim;
  return r;
}


/* ============================================================ */
/* Closure */
/* ============================================================ */

/* Make sure the closed version of the matrix a->closed is available.
   If a->closed when it returns, this means the octagon is empty.
   If the octagon is not empty, a->m is not modified.
   => this DOES NOT affect the semantics of functions that rely on a->m
   (e.g., widening)
 */
void oct_cache_closure(oct_internal_t* pr, oct_t* a)
{
  if (a->closed || !a->m) return;
  a->closed = hmat_copy(pr,a->m,a->dim);
  if (hmat_close(a->closed,a->dim)) {
    /* empty! */
    hmat_free(pr,a->m,a->dim);
    hmat_free(pr,a->closed,a->dim);
    a->m = a->closed = NULL;
  }
}

/* Unlike oct_cache_closure, this frees the a->m representation, forcing
   all further transfer functions to work on the closed version...
   => this changes the semantics of functions such as widening

*/
void oct_close(oct_internal_t* pr, oct_t* a)
{
  if (!a->m) return;
  if (a->closed) {
    hmat_free(pr,a->m,a->dim);
    a->m = NULL;
    return;
  }
  a->closed = a->m;
  a->m = NULL;
  if (hmat_close(a->closed,a->dim)) {
    hmat_free(pr,a->closed,a->dim);
    a->closed = NULL;
  }
}


/* ============================================================ */
/* Control of internal representation: NOT IMPLEMENTED */
/* ============================================================ */

/* NOT IMPLEMENTED: do nothing */
void oct_minimize(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_MINIMIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
void oct_canonicalize(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_CANONICALIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
int oct_hash(ap_manager_t* man, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_HASH,0);
  if (pr->funopt->algorithm>=0) oct_cache_closure(pr,a);
  if (a->closed || a->m) {
    int r = 0;
    bound_t *m = a->closed ? a->closed : a->m;
    size_t i,j;
    for (i=0;i<2*a->dim;i++)
      for (j=0;j<=(i|1);j++,m)
	r = r*37 + bound_hash(*m);
    return r;
  }
  return 0;
}

/* NOT IMPLEMENTED: do nothing */
void oct_approximate(ap_manager_t* man, oct_t* a, int algorithm)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_APPROXIMATE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* ============================================================ */
/* Topological losure operation */
/* ============================================================ */

/* actually identity */
oct_t* oct_closure(ap_manager_t* man, bool destructive, oct_t* a)
{
  oct_internal_t* pr = oct_init_from_manager(man,AP_FUNID_CLOSURE,0);
  if (destructive) return a;
  else return oct_copy_internal(pr,a);
}



/* ============================================================ */
/* Managers */
/* ============================================================ */

void oct_internal_free(oct_internal_t* pr)
{
  bound_clear_array(pr->tmp,pr->tmp_size);
  free(pr->tmp);
  free(pr->tmp2);
  free(pr);
}

ap_manager_t* oct_manager_alloc(void)
{
  size_t i;
  ap_manager_t* man;
  oct_internal_t* pr;

  if (!ap_fpu_init()) {
    fprintf(stderr,"oct_manager_alloc cannot change the FPU rounding mode\n");
  }

  pr = (oct_internal_t*)malloc(sizeof(oct_internal_t));
  assert(pr);
  pr->tmp_size = 10;
  pr->tmp = malloc(sizeof(bound_t)*pr->tmp_size);
  assert(pr->tmp);
  bound_init_array(pr->tmp,pr->tmp_size);
  pr->tmp2 = malloc(sizeof(long)*pr->tmp_size);
  assert(pr->tmp2);

  man = ap_manager_alloc("oct","1.0 with " NUM_NAME, pr,
			 (void (*)(void*))oct_internal_free);

  pr->man = man;

  man->funptr[AP_FUNID_COPY] = &oct_copy;
  man->funptr[AP_FUNID_FREE] = &oct_free;
  man->funptr[AP_FUNID_ASIZE] = &oct_size;
  man->funptr[AP_FUNID_MINIMIZE] = &oct_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = &oct_canonicalize;
  man->funptr[AP_FUNID_HASH] = &oct_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = &oct_approximate;
  man->funptr[AP_FUNID_FPRINT] = &oct_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = &oct_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = &oct_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = &oct_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = &oct_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = &oct_bottom;
  man->funptr[AP_FUNID_TOP] = &oct_top;
  man->funptr[AP_FUNID_OF_BOX] = &oct_of_box;
  man->funptr[AP_FUNID_DIMENSION] = &oct_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = &oct_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = &oct_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = &oct_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = &oct_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &oct_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = &oct_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = &oct_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = &oct_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = &oct_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = &oct_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = &oct_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = &oct_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = &oct_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = &oct_to_tcons_array;
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &oct_to_generator_array;
  man->funptr[AP_FUNID_MEET] = &oct_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = &oct_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &oct_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = &oct_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = &oct_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = &oct_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = &oct_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &oct_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &oct_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &oct_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &oct_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = &oct_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = &oct_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &oct_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = &oct_forget_array;
  man->funptr[AP_FUNID_EXPAND] = &oct_expand;
  man->funptr[AP_FUNID_FOLD] = &oct_fold;
  man->funptr[AP_FUNID_WIDENING] = &oct_widening;
  man->funptr[AP_FUNID_CLOSURE] = &oct_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,i,false);

  return man;
}

oct_t* oct_of_abstract0(ap_abstract0_t* a)
{
  return (oct_t*)a->value;
}

ap_abstract0_t* abstract0_of_oct(ap_manager_t* man, oct_t* oct)
{
  ap_abstract0_t* r = malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = oct;
  r->man = ap_manager_copy(man);
  return r;
}
