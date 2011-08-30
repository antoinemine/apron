/*
 * octXXX_resize.c
 *
 * Projection, changes of dimension, variable permutation.
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


/* ============================================================ */
/* Projections */
/* ============================================================ */

octXXX_t* octXXX_forget_array(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_dim_t* tdim, size_t size,
			      bool project)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FORGET_ARRAY,0);
  if (pr->option.project_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m)
    /* definitively empty */
    return octXXX_set_mat(pr,a,NULL,NULL,destructive);
  else {
    boundXXX_t* m = a->closed ? a->closed : a->m;
    size_t i,k;
    if (!destructive) m = hmatXXX_copy(pr,m,a->dim);
    for (i=0;i<size;i++) {
      ap_dim_t d2 = 2*tdim[i];
      arg_assert(tdim[i]<a->dim,return NULL;);
      /* binary constraints on tdim[i] */
      for (k=0;k<d2;k++) {
	boundXXX_set_infty(m[matpos(d2,k)],1);
	boundXXX_set_infty(m[matpos(d2+1,k)],1);
      }
      for (k=d2+2;k<2*a->dim;k++) {
	boundXXX_set_infty(m[matpos(k,d2)],1);
	boundXXX_set_infty(m[matpos(k,d2+1)],1);
      }
      /* unary constraints on tdim[i] */
      if (project) {
	boundXXX_set_int(m[matpos(d2,d2+1)],0);
	boundXXX_set_int(m[matpos(d2+1,d2)],0);
      }
      else {
	boundXXX_set_infty(m[matpos(d2,d2+1)],1);
	boundXXX_set_infty(m[matpos(d2+1,d2)],1);
      }
    }
    if (a->closed) {
      /* result is exact on Q, and closed if forget, not project */
      if (numXXX_incomplete || a->intdim) flag_incomplete;
      if (project) return octXXX_set_mat(pr,a,m,NULL,destructive);
      else return octXXX_set_mat(pr,a,NULL,m,destructive);
    }
    else {
      /* not exact, not closed */
      flag_algo;
      return octXXX_set_mat(pr,a,m,NULL,destructive);
    }
  }
}


/* ============================================================ */
/* Change and permutation of dimensions */
/* ============================================================ */

/* internal helper function */
void hmatXXX_addrem_dimensions(boundXXX_t* dst, boundXXX_t* src,
			    ap_dim_t* pos, size_t nb_pos,
			    size_t mult, size_t dim, bool add)
{
  size_t i,j,new_j,org_j;
  new_j = org_j = pos[0]*2;
  boundXXX_set_array(dst,src,matsize(pos[0]));

  for (j=0;j<nb_pos;j++) {
    /* skip lines */
    if (add) new_j += 2*mult; else org_j += 2*mult;

    /* copy lines */
    {
      boundXXX_t* org_c = src + matsize(org_j/2);
      boundXXX_t* new_c = dst + matsize(new_j/2);
      size_t last_org_j = ((j<nb_pos-1) ? pos[j+1] : dim)*2;
      for (;org_j<last_org_j;org_j++,new_j++) {
	size_t size_org_line = org_j+2-(org_j&1);
	size_t size_new_line = new_j+2-(new_j&1);
	size_t org_i = 0;
	size_t new_i = 0;
	for (i=0;i<nb_pos;i++) {
	  /* copy elems */
	  size_t last_org_i = pos[i]*2;
	  if (last_org_i>=size_org_line) break; /* partial block */
	  boundXXX_set_array(new_c+new_i,org_c+org_i,last_org_i-org_i);
	  new_i += last_org_i-org_i;
	  org_i = last_org_i;

	  /* skip elems */
	  if (add) new_i += 2*mult; else org_i += 2*mult;
	}

	/* copy remaining elems */
	boundXXX_set_array(new_c+new_i,org_c+org_i,size_org_line-org_i);

	/* next line */
	org_c += size_org_line;
	new_c += size_new_line;
      }
    }
  }
}

octXXX_t* octXXX_add_dimensions(ap_manager_t* man,
				bool destructive, octXXX_t* a,
				ap_dimchange_t* dimchange,
				bool project)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS,0);
  boundXXX_t* m = a->closed ? a->closed : a->m;
  boundXXX_t* mm;
  size_t i, nb = ap_dimension_size(dimchange->dim);
  octXXX_t* r;
  if (!m) mm = NULL;
  else {
    /* check */
    for (i=0;i<nb;i++) {
      arg_assert(dimchange->p[i]<=a->dim,return NULL;);
      arg_assert(!i || dimchange->p[i-1]<=dimchange->p[i],return NULL;);
    }

    /* insert variables */
    mm = hmatXXX_alloc_top(pr,a->dim+nb);
    hmatXXX_addrem_dimensions(mm,m,dimchange->p,
			   nb,1,a->dim,true);

    /* set new variables to 0, if necessary */
    if (project) {
      for (i=0;i<nb;i++) {
	size_t v = 2*(i+dimchange->p[i]);
	boundXXX_set_int(mm[matpos(v+1,v)],0);
	boundXXX_set_int(mm[matpos(v,v+1)],0);
      }
    }
  }
  /* always exact, respect closure if embedding, not projecting */
  if (a->closed && !project) r = octXXX_set_mat(pr,a,NULL,mm,destructive);
  else r = octXXX_set_mat(pr,a,mm,NULL,destructive);
  r->dim += nb;
  r->intdim += dimchange->dim.intd;
  return r;
}

octXXX_t* octXXX_remove_dimensions(ap_manager_t* man,
			     bool destructive, octXXX_t* a,
			     ap_dimchange_t* dimchange)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS,0);
  boundXXX_t *m, *mm;
  size_t i, nb = ap_dimension_size(dimchange->dim);
  octXXX_t* r;
  if (pr->option.project_closure) octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) mm = NULL;
  else {
    /* check */
    for (i=0;i<nb;i++) {
      arg_assert(dimchange->p[i]<a->dim,return NULL;);
      arg_assert(!i || dimchange->p[i-1]<dimchange->p[i],return NULL;);
    }

    /* remove variables */
    mm = hmatXXX_alloc(pr,a->dim-nb);
    hmatXXX_addrem_dimensions(mm,m,dimchange->p,
			   nb,1,a->dim,false);
  }

  if (a->closed) {
    /* result is exact on Q, and closed */
    if (numXXX_incomplete || a->intdim) flag_incomplete;
    r = octXXX_set_mat(pr,a,NULL,mm,destructive);
  }
  else {
    /* not exact, not closed */
    flag_algo;
    r = octXXX_set_mat(pr,a,mm,NULL,destructive);
  }
  r->dim -= nb;
  r->intdim -= dimchange->dim.intd;
  return r;
}

/* internal helper function: permute & change dimension */
void hmatXXX_permute(boundXXX_t* dst, boundXXX_t* src,
		     size_t dst_dim, size_t src_dim,
		     ap_dim_t* permutation)
{
  size_t i,j;
  for (i=0;i<src_dim;i++) {
    size_t new_ii = 2*permutation[i];
    if (new_ii >= 2*dst_dim)  { src+=4*(i+1); continue; }
    for (j=0;j<=i;j++,src+=2) {
      size_t new_jj = 2*permutation[j];
      if (new_jj >= 2*dst_dim) continue;
      boundXXX_set(dst[matpos2(new_ii  ,new_jj  )],src[0]);
      boundXXX_set(dst[matpos2(new_ii  ,new_jj+1)],src[1]);
      boundXXX_set(dst[matpos2(new_ii+1,new_jj  )],src[2*(i+1)]);
      boundXXX_set(dst[matpos2(new_ii+1,new_jj+1)],src[2*(i+1)+1]);
    }
    src+=2*(i+1);
  }
}

octXXX_t* octXXX_permute_dimensions(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_dimperm_t* permutation)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS,0);
  boundXXX_t* m = a->closed ? a->closed : a->m;
  boundXXX_t* mm;
  arg_assert(permutation->size==a->dim,return NULL;);
  if (!m) mm = NULL;
  else {
    /* check (only bounds, not injectivity) */
    size_t i,j;
    for (i=0;i<a->dim;i++)
      arg_assert(permutation->p[i]<a->dim,return NULL;);

    /* permuted copy */
    mm = hmatXXX_alloc(pr,a->dim);
    hmatXXX_permute(mm,m,a->dim,a->dim,permutation->p);
  }
  /* always exact, respects closure */
  if (a->closed) return octXXX_set_mat(pr,a,NULL,mm,destructive);
  else return octXXX_set_mat(pr,a,mm,NULL,destructive);
}


/* ============================================================ */
/* Expansion and folding of dimensions */
/* ============================================================ */

octXXX_t* octXXX_expand(ap_manager_t* man,
		  bool destructive, octXXX_t* a,
		  ap_dim_t dim,
		  size_t n)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_EXPAND,0);
  boundXXX_t* m = a->closed ? a->closed : a->m;
  size_t i, j;
  ap_dim_t pos = (dim < a->intdim) ? a->intdim : a->dim;
  boundXXX_t* mm;
  octXXX_t* r;
  arg_assert(dim<a->dim,return NULL;);
  if (!m) mm = NULL;
  else {
    /* insert n variables at pos */
    mm = hmatXXX_alloc_top(pr,a->dim+n);
    hmatXXX_addrem_dimensions(mm,m,&pos,1,n,a->dim,true);

    for (i=0;i<n;i++) {

      /* copy binary constraints */

      for (j=0;j<2*dim;j++) {
	boundXXX_set(mm[matpos2(2*(pos+i)  ,j)],mm[matpos(2*dim  ,j)]);
	boundXXX_set(mm[matpos2(2*(pos+i)+1,j)],mm[matpos(2*dim+1,j)]);
      }
      for (j=2*dim+2;j<2*(a->dim+n);j++) {
	boundXXX_set(mm[matpos2(2*(pos+i)  ,j)],mm[matpos(j^1,2*dim+1)]);
	boundXXX_set(mm[matpos2(2*(pos+i)+1,j)],mm[matpos(j^1,2*dim)]);
      }

      /* copy unary constraints */
      boundXXX_set(mm[matpos2(2*(pos+i),2*(pos+i)+1)],mm[matpos2(2*dim,2*dim+1)]);
      boundXXX_set(mm[matpos2(2*(pos+i)+1,2*(pos+i))],mm[matpos2(2*dim+1,2*dim)]);
    }
  }

  /*  exact, generally not closed */
  r = octXXX_set_mat(pr,a,mm,NULL,destructive);
  r->dim += n;
  if (dim<a->intdim) r->intdim += n;
  return r;
}

octXXX_t* octXXX_fold(ap_manager_t* man,
		bool destructive, octXXX_t* a,
		ap_dim_t* tdim,
		size_t size)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FOLD,matsize(a->dim));
  boundXXX_t* m;
  boundXXX_t* mm;
  octXXX_t* r;
  if (pr->option.fold_closure) octXXX_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) mm = NULL;
  else {
    /* check, assuming tdim[0..(size-1)] is strictly increasing */
    size_t i,j;
    arg_assert(size>0,return NULL;);
    for (i=1;i<size;i++) {
      arg_assert(tdim[i-1]<tdim[i],return NULL;);
    }
    arg_assert(tdim[size-1]<a->dim,return NULL;);

    boundXXX_set_array(pr->tmp,m,matsize(a->dim));

    /* merge binary constraints */
    for (j=0;j<2*tdim[0];j++) {
      boundXXX_t* mm1 = pr->tmp+matpos2(tdim[0]*2  ,j);
      boundXXX_t* mm2 = pr->tmp+matpos2(tdim[0]*2+1,j);
      for (i=1;i<size;i++) {
	boundXXX_max(*mm1,*mm1,m[matpos2(tdim[i]*2  ,j)]);
	boundXXX_max(*mm2,*mm2,m[matpos2(tdim[i]*2+1,j)]);
      }
    }
    for (j=2*(tdim[0]+1);j<2*a->dim;j++) {
      boundXXX_t* mm1 = pr->tmp+matpos2(tdim[0]*2  ,j);
      boundXXX_t* mm2 = pr->tmp+matpos2(tdim[0]*2+1,j);
      for (i=1;i<size;i++) {
	boundXXX_max(*mm1,*mm1,m[matpos2(tdim[i]*2  ,j)]);
	boundXXX_max(*mm2,*mm2,m[matpos2(tdim[i]*2+1,j)]);
      }
    }

    /* merge unary constraints */
    {
      boundXXX_t* mm1 = pr->tmp+matpos2(tdim[0]*2  ,tdim[0]*2+1);
      boundXXX_t* mm2 = pr->tmp+matpos2(tdim[0]*2+1,tdim[0]*2  );
      for (i=1;i<size;i++) {
	boundXXX_max(*mm1,*mm1,m[matpos2(tdim[i]*2,tdim[i]*2+1)]);
	boundXXX_max(*mm2,*mm2,m[matpos2(tdim[i]*2+1,tdim[i]*2)]);
      }
    }

    /* destroy all dimensions in tdim except the first one */
    mm = hmatXXX_alloc_top(pr,a->dim-size+1);
    hmatXXX_addrem_dimensions(mm,pr->tmp,tdim+1,size-1,1,a->dim,false);

    /* reset diagonal elements */
    boundXXX_set_int(mm[matpos(tdim[0]*2  ,tdim[0]*2  )],0);
    boundXXX_set_int(mm[matpos(tdim[0]*2+1,tdim[0]*2+1)],0);

    man->result.flag_exact = false;
  }

  if (a->closed) {
    /* result is optimal on Q, not closed */
    if (numXXX_incomplete || a->intdim) flag_incomplete;
    r = octXXX_set_mat(pr,a,mm,NULL,destructive);
  }
  else {
    /* not exact, not closed */
    flag_algo;
    r = octXXX_set_mat(pr,a,mm,NULL,destructive);
  }
  r->dim -= size-1;
  if (tdim[0]<r->intdim) r->intdim -= size-1;
  return r;
}
