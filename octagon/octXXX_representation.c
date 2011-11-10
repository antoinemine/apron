/*
 * octXXX_representation.c
 *
 * Simple functions related to the representation of octagons
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
/* Memory Related */
/* ============================================================ */

/* generic allocation routine, returns a bottom octagon */
inline octXXX_t* octXXX_alloc_internal(octXXX_internal_t* pr, size_t dim, size_t intdim)
{
  octXXX_t* r;
  checked_malloc(r,octXXX_t,1,return NULL;);
  r->intdim = intdim;
  r->dim = dim;
  r->closed = r->m = NULL;
  return r;
}

inline octXXX_t* octXXX_alloc_top(octXXX_internal_t* pr, size_t dim, size_t intdim)
{
  octXXX_t* r = octXXX_alloc_internal(pr,dim,intdim);
  r->closed = hmatXXX_alloc_top(pr,dim);
  return r;
}

inline void octXXX_free_internal(octXXX_internal_t* pr, octXXX_t* a)
{
  if (a->m) hmatXXX_free(pr,a->m,a->dim);
  if (a->closed) hmatXXX_free(pr,a->closed,a->dim);
  a->closed = a->m = NULL;
  free(a);
}

inline octXXX_t* octXXX_copy_internal(octXXX_internal_t* pr, octXXX_t* a)
{
  octXXX_t* r = octXXX_alloc_internal(pr,a->dim,a->intdim);
  r->m = hmatXXX_copy(pr,a->m,a->dim);
  r->closed = hmatXXX_copy(pr,a->closed,a->dim);
  return r;
}

octXXX_t* octXXX_copy(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_COPY,0);
  return octXXX_copy_internal(pr,a);
}

void octXXX_free(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FREE,0);
  octXXX_free_internal(pr,a);
}

size_t octXXX_size(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_ASIZE,0);
  if (!a->m) return 1;
  return matsize(a->dim);
}

/* If destructive, returns a with fields m and closed updated
   (former fields of a, if not reused in m or closed, are destroyed).
   If not destructive, returns a new octagon with same dimensions as a
   and m and closed as half-matrices.
   m and closed can safely alias fields in a
 */
octXXX_t* octXXX_set_mat(octXXX_internal_t* pr, octXXX_t* a, boundXXX_t* m, boundXXX_t* closed,
		   bool destructive)
{
  octXXX_t* r;
  if (destructive) {
    /* free non-aliased matrices */
    if (a->m && a->m!=m && a->m!=closed)
      hmatXXX_free(pr,a->m,a->dim);
    if (a->closed && a->closed!=m && a->closed!=closed)
      hmatXXX_free(pr,a->closed,a->dim);
    r = a;
  }
  else {
    /* copy aliased matrices */
    r = octXXX_alloc_internal(pr,a->dim,a->intdim);
    if (m && (a->m==m || a->closed==m)) m = hmatXXX_copy(pr,m,a->dim);
    if (closed && (a->m==closed || a->closed==closed))
      closed = hmatXXX_copy(pr,closed,a->dim);
  }
  r->m = m;
  r->closed = closed;
  return r;
}


/* ============================================================ */
/* Basic Constructors */
/* ============================================================ */

octXXX_t* octXXX_bottom(ap_manager_t* man, ap_dimension_t dim)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_BOTTOM,0);
  octXXX_t* r = octXXX_alloc_internal(pr,dim.intd+dim.reald,dim.intd);
  return r;
}

octXXX_t* octXXX_top(ap_manager_t* man, ap_dimension_t dim)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_TOP,0);
  octXXX_t* r = octXXX_alloc_internal(pr, dim.intd+dim.reald,dim.intd);
  r->closed = hmatXXX_alloc_top(pr,r->dim);
  return r;
}

octXXX_t* octXXX_of_box(ap_manager_t* man, ap_dimension_t dim, ap_linexpr0_t box)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_OF_BOX,0);
  size_t size = ap_dimension_size(dim);
  octXXX_t* r = octXXX_alloc_internal(pr,size,dim.intd);
  size_t i,j,n2;
  ap_coeff_t coeff;
  ap_dim_t d,d2,lastd;

  ap_linexpr0_cstref(coeff,box);
  if (ap_coeff_is_bottom(coeff)) return r; /* empty */
  r->closed = hmatXXX_alloc_top(pr,r->dim);
  lastd = 0;
  ap_linexpr0_ForeachLinterm(box,i,d,coeff){
    for (d2=lastd;d2<d;d2++){
      boundXXX_set_int(r->closed[matpos(2*d2,2*d2+1)],0);
      boundXXX_set_int(r->closed[matpos(2*d2+1,2*d2)],0);
    }
    lastd = d+1;
    if (boundsXXX_of_coeff(pr,
			   r->closed[matpos(2*i,2*i+1)],
			   r->closed[matpos(2*i+1,2*i)],
			   coeff,true)) {
      /* one interval is empty -> the result is empty */
      hmatXXX_free(pr,r->closed,r->dim);
      r->closed = NULL;
      return r;
    }
  }
  for (d2=lastd;d2<size;d2++){
    boundXXX_set_int(r->closed[matpos(2*d2,2*d2+1)],0);
    boundXXX_set_int(r->closed[matpos(2*d2+1,2*d2)],0);
  }
  /* a S step is sufficient to ensure clsoure */
  if (hmatXXX_s_step(r->closed,r->dim)){
    /* definitively empty */
    hmatXXX_free(pr,r->closed,r->dim);
    r->closed = NULL;
  }

  /* exact, except for conversion errors */
  if (pr->conv) flag_conv;
  return r;
}

/* octXXX_t* octXXX_of_lingen_array(ap_manager_t* man, */
/*			      size_t intdim, size_t realdim, */
/*			      ap_lingen0_array_t ar) */
/* { */
/*   size_t dim = intdim+realdim; */
/*   octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY, */
/*					     2*(dim+1)); */
/*   octXXX_t* r = octXXX_alloc_internal(pr,intdim+realdim,intdim); */
/*   size_t i,j,k; */

/*   /\* find one vertex *\/ */
/*   for (i=0;i<ar->size;i++) { */
/*     boundXXX_t* b; */
/*     if (ar->p[i].gentyp!=AP_GEN_VERTEX) continue; */
/*     bounds_of_lingen(pr,pr->tmp,ar->p[i].linexpr0,dim); */
/*     r->m = hmatXXX_alloc_top(pr,dim); */
/*     for (b=r->m,j=0;j<2*dim;j++) */
/*       for (k=0;k<=(j|1);k++,b++) boundXXX_sub(*b,pr->tmp[j],pr->tmp[k]); */
/*     break; */
/*   } */

/*   if (r->m) { */
/*     /\* not empty *\/ */
/*     hmatXXX_add_lingen0_array(pr,r->m,dim,ar); */
/*     /\* best in Q and Z, except for conversion errors *\/ */
/*     man->result.flag_exact = false; */
/*     if (pr->conv) flag_conv; */
/*   } */
/*   return r; */
/* } */

/* ap_abstract0_t* */
/* ap_abstract0_octXXX_of_lingen_array(ap_manager_t* man, */
/*				    size_t intdim, size_t realdim, */
/*				    ap_lingen0_array_t array) */
/* { */
/*   return abstract0_of_oct(man,octXXX_of_lingen_array(man,intdim,realdim,array)); */
/* } */


/* ============================================================ */
/* Accessors */
/* ============================================================ */

ap_dimension_t octXXX_dimension(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_DIMENSION,0);
  ap_dimension_t r;
  r.intd = a->intdim;
  r.reald = a->dim-a->intdim;
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
void octXXX_cache_closure(octXXX_internal_t* pr, octXXX_t* a)
{
  if (a->closed || !a->m) return;
  a->closed = hmatXXX_copy(pr,a->m,a->dim);
  if (hmatXXX_close(a->closed,a->dim)) {
    /* empty! */
    hmatXXX_free(pr,a->m,a->dim);
    hmatXXX_free(pr,a->closed,a->dim);
    a->m = a->closed = NULL;
  }
}

/* Unlike octXXX_cache_closure, this frees the a->m representation, forcing
   all further transfer functions to work on the closed version...
   => this changes the semantics of functions such as widening

*/
void octXXX_close(octXXX_internal_t* pr, octXXX_t* a)
{
  if (!a->m) return;
  if (a->closed) {
    hmatXXX_free(pr,a->m,a->dim);
    a->m = NULL;
    return;
  }
  a->closed = a->m;
  a->m = NULL;
  if (hmatXXX_close(a->closed,a->dim)) {
    hmatXXX_free(pr,a->closed,a->dim);
    a->closed = NULL;
  }
}


/* ============================================================ */
/* Control of internal representation: NOT IMPLEMENTED */
/* ============================================================ */

/* NOT IMPLEMENTED: do nothing */
void octXXX_minimize(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_MINIMIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

void octXXX_canonicalize(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_CANONICALIZE,0);
  octXXX_cache_closure(pr,a);
  /*  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  */
}

int octXXX_hash(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_HASH,0);
  octXXX_cache_closure(pr,a);
  if (a->closed || a->m) {
    int r = 0;
    boundXXX_t *m = a->closed ? a->closed : a->m;
    size_t i,j;
    for (i=0;i<2*a->dim;i++)
      for (j=0;j<=(i|1);j++,m)
	r = r*37 + boundXXX_hash(*m);
    return r;
  }
  return 0;
}

/* NOT IMPLEMENTED: do nothing */
void octXXX_approximate(ap_manager_t* man, octXXX_t* a, int algorithm)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_APPROXIMATE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* ============================================================ */
/* Topological losure operation */
/* ============================================================ */

/* actually identity */
octXXX_t* octXXX_closure(ap_manager_t* man, bool destructive, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_CLOSURE,0);
  if (destructive) return a;
  else return octXXX_copy_internal(pr,a);
}



/* ============================================================ */
/* Managers */
/* ============================================================ */

void octXXX_internal_free(octXXX_internal_t* pr)
{
  eitvXXX_clear(pr->eitv);
  boundXXX_clear_array(pr->tmp,pr->tmp_size);
  free(pr->tmp);
  free(pr->tmp2);
  free(pr);
}

ap_manager_t* octXXX_manager_alloc(void)
{
  size_t i;
  ap_manager_t* man;
  octXXX_internal_t* pr;

  if (!ap_fpu_init()) {
    fprintf(stderr,"octXXX_manager_alloc cannot change the FPU rounding mode\n");
  }

  pr = (octXXX_internal_t*)malloc(sizeof(octXXX_internal_t));
  assert(pr);
  eitvXXX_init(pr->eitv);
  pr->tmp_size = 10;
  pr->tmp = malloc(sizeof(boundXXX_t)*pr->tmp_size);
  assert(pr->tmp);
  boundXXX_init_array(pr->tmp,pr->tmp_size);
  pr->tmp2 = malloc(sizeof(long)*pr->tmp_size);
  assert(pr->tmp2);

  man = ap_manager_alloc(strdup("oct" "XXX"),strdup("1.0"), pr,
			 (void (*)(void*))octXXX_internal_free);

  pr->man = man;
  pr->num = man->num;
  pr->option.print_closure = true;
  pr->option.printdiff_closure = true;
  pr->option.meet_lincons_array_closure = true;
  pr->option.add_ray_array_closure = true;
  pr->option.assign_closure = true;
  pr->option.substitute_closure = true;
  pr->option.join_closure = true;
  pr->option.narrowing_closure = true;
  pr->option.widening_closure = true;
  pr->option.comparison_test_closure = true;
  pr->option.sat_test_closure = true;
  pr->option.bound_closure = true;
  pr->option.extract_closure = true;
  pr->option.project_closure = true;
  pr->option.fold_closure = true;
  pr->option.widening_kind = oct_widening;

  man->funptr[AP_FUNID_COPY] = &octXXX_copy;
  man->funptr[AP_FUNID_FREE] = &octXXX_free;
  man->funptr[AP_FUNID_ASIZE] = &octXXX_size;
  man->funptr[AP_FUNID_MINIMIZE] = &octXXX_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = &octXXX_canonicalize;
  man->funptr[AP_FUNID_HASH] = &octXXX_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = &octXXX_approximate;
  man->funptr[AP_FUNID_FPRINT] = &octXXX_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = &octXXX_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = &octXXX_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = &octXXX_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = &octXXX_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = &octXXX_bottom;
  man->funptr[AP_FUNID_TOP] = &octXXX_top;
  man->funptr[AP_FUNID_OF_BOX] = &octXXX_of_box;
  man->funptr[AP_FUNID_DIMENSION] = &octXXX_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = &octXXX_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = &octXXX_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = &octXXX_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = &octXXX_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &octXXX_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = &octXXX_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = &octXXX_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = &octXXX_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = &octXXX_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = &octXXX_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = &octXXX_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = &octXXX_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = &octXXX_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = &octXXX_to_tcons_array;
  man->funptr[AP_FUNID_TO_LINGEN_ARRAY] = &octXXX_to_lingen_array;
  man->funptr[AP_FUNID_MEET] = &octXXX_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = &octXXX_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &octXXX_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = &octXXX_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = &octXXX_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = &octXXX_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = &octXXX_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &octXXX_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &octXXX_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &octXXX_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &octXXX_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = &octXXX_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = &octXXX_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &octXXX_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = &octXXX_forget_array;
  man->funptr[AP_FUNID_EXPAND] = &octXXX_expand;
  man->funptr[AP_FUNID_FOLD] = &octXXX_fold;
  man->funptr[AP_FUNID_WIDENING] = &octXXX_widening;
  man->funptr[AP_FUNID_CLOSURE] = &octXXX_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,i,false);

  return man;
}

octXXX_t* octXXX_of_abstract0(ap_abstract0_t* a)
{
  return (octXXX_t*)a->value;
}

ap_abstract0_t* abstract0_of_octXXX(ap_manager_t* man, octXXX_t* oct)
{
  ap_abstract0_t* r = malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = oct;
  r->man = ap_manager_copy(man);
  return r;
}
