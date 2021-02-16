/*
 * avo_resize.c
 *
 * Projection, changes of dimension, variable permutation.
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
/* Projections */
/* ============================================================ */

avo_t* avo_forget_array(ap_manager_t* man,
			bool destructive, avo_t* a,
			ap_dim_t* tdim, size_t size,
			bool project)
{
	 //fprintf(stdout, "AP_FUNID_FORGET_ARRAY\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FORGET_ARRAY,0);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m)
    /* definitively empty */
    return avo_set_mat_nsc(pr,a,NULL,NULL,NULL,destructive);
  else {
    bound_t* m = a->closed ? a->closed : a->m;
    bound_t *m1= a->nsc;
    size_t i,k,dim;
    dim = a->dim;
    if (!destructive) m = avo_hmat_copy(pr,m,a->dim);
    for (i=0;i<size;i++) {
      ap_dim_t d2 = 2*tdim[i];
      arg_assert(tdim[i]<a->dim,return NULL;);
      /* binary constraints on tdim[i] */
      for (k=0;k<d2;k++) {
		bound_set_infty(m[avo_matpos2(d2,k)],1);
		bound_set_infty(m[avo_matpos2(d2+1,k)],1);
		bound_set_infty(m[avo_matpos2(d2+ 2 * dim,k)],1);
		bound_set_infty(m[avo_matpos2(d2+1 + 2 * dim,k)],1);
		bound_set_infty(m1[avo_matpos2(d2,k)],1);
		bound_set_infty(m1[avo_matpos2(d2+1,k)],1);
		bound_set_infty(m1[avo_matpos2(d2+ 2 * dim,k)],1);
		bound_set_infty(m1[avo_matpos2(d2+1 + 2 * dim,k)],1);
      }
      for (k=d2+2;k<4*a->dim;k++) {
		bound_set_infty(m[avo_matpos2(k,d2)],1);
		bound_set_infty(m[avo_matpos2(k,d2+1)],1);
		bound_set_infty(m[avo_matpos2(k,d2+2*dim)],1);
		bound_set_infty(m[avo_matpos2(k,d2+1+2*dim)],1);
		bound_set_infty(m1[avo_matpos2(k,d2)],1);
		bound_set_infty(m1[avo_matpos2(k,d2+1)],1);
		bound_set_infty(m1[avo_matpos2(k,d2+2*dim)],1);
		bound_set_infty(m1[avo_matpos2(k,d2+1+2*dim)],1);
      }
      bound_set_infty(m1[avo_matpos2(d2,d2+1)],1);
      bound_set_infty(m1[avo_matpos2(d2+1,d2)],1);
      bound_set_infty(m1[avo_matpos2(d2+2*dim,d2+1+2*dim)],1);
      bound_set_infty(m1[avo_matpos2(d2+1+2*dim,d2+2*dim)],1);

      /* unary constraints on tdim[i] */
	  bound_set_int(m[avo_matpos2(d2,d2)],0);
	  bound_set_int(m[avo_matpos2(d2+1,d2+1)],0);
	  bound_set_int(m[avo_matpos2(d2+2*dim,d2+2*dim)],0);
	  bound_set_int(m[avo_matpos2(d2+2*dim+1,d2+2*dim+1)],0);
      if (project) {
		bound_set_int(m[avo_matpos2(d2,d2+1)],0);
		bound_set_int(m[avo_matpos2(d2+1,d2)],0);
      }
      else {
		bound_set_infty(m[avo_matpos2(d2,d2+1)],1);
		bound_set_infty(m[avo_matpos2(d2+1,d2)],1);
		bound_set_infty(m[avo_matpos2(d2+2*dim,d2+1+2*dim)],1);
		bound_set_infty(m[avo_matpos2(d2+1+2*dim,d2+2*dim)],1);
      }
    }
    if (a->closed) {
      /* result is exact on Q, and closed if forget, not project */
      if (num_incomplete || a->intdim) flag_incomplete;
      if (project) return avo_set_mat_nsc(pr,a,m,NULL,m1,destructive);
      else return avo_set_mat_nsc(pr,a,NULL,m,m1,destructive);
    }
    else {
      /* not exact, not closed */
      flag_algo;
      return avo_set_mat_nsc(pr,a,m,NULL,m1,destructive);
    }
  }
}


/* ============================================================ */
/* Change and permutation of dimensions */
/* ============================================================ */

/* internal helper function */
void avo_hmat_addrem_dimensions(bound_t* dst, bound_t* src,
			    ap_dim_t* pos, size_t nb_pos,
			    size_t mult, size_t dim, bool add)
{
  size_t i,j,new_j,org_j;
  new_j = org_j = pos[0]*2;
  bound_set_array(dst,src,avo_octmatsize(pos[0]));
  for (j=0;j<nb_pos;j++) {
    /* skip lines */
    if (add) new_j += 2*mult; else org_j += 2*mult;
    /* copy lines */
    {
      bound_t* org_c = src + avo_octmatsize(org_j/2);
      bound_t* new_c = dst + avo_octmatsize(new_j/2);
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
		  bound_set_array(new_c+new_i,org_c+org_i,last_org_i-org_i);
		  new_i += last_org_i-org_i;
		  org_i = last_org_i;

		  /* skip elems */
		  if (add) new_i += 2*mult;
		  else     org_i += 2*mult;
		}

		/* copy remaining elems */
		bound_set_array(new_c+new_i,org_c+org_i,size_org_line-org_i);

		/* next line */
		org_c += size_org_line;
		new_c += size_new_line;
     }
   }
  }
}

void avo_hmat_addrem_dimensions_withAV(bound_t* dst, bound_t* src,
	    ap_dim_t* pos, size_t nb_pos,
	    size_t mult, size_t dim, bool add)
{
    size_t i,new_dim;
    bound_t * mm = malloc(sizeof(bound_t)*avo_matsize(dim+nb_pos));
    bound_init_array(mm,avo_matsize(dim+nb_pos));
    for (i=0;i<avo_matsize(dim+nb_pos);i++) 
    	bound_set_infty(mm[i],1);

    avo_hmat_addrem_dimensions(mm, src,pos, nb_pos, mult,2*dim, add);

    for(i = 0 ; i < nb_pos ; i++){
		if(add)	    pos[i] = pos[i]+dim + nb_pos ;
		else	    pos[i] = pos[i]+dim - nb_pos ;
    }

    new_dim = dim * 2;
    if(add) new_dim = new_dim + nb_pos;
    else    new_dim = new_dim - nb_pos;

    avo_hmat_addrem_dimensions(dst, mm,pos, nb_pos, mult,new_dim, add);

    for(i = 0 ; i < nb_pos ; i++){
    	if(add)    	pos[i] = pos[i]-dim - nb_pos ;
    	else   		pos[i] = pos[i]-dim + nb_pos ;
    }
    bound_clear_array(mm,avo_matsize(dim+nb_pos));
    free(mm);
}

avo_t* avo_add_dimensions(ap_manager_t* man,
			  bool destructive, avo_t* a,
			  ap_dimchange_t* dimchange,
			  bool project)
{
	//fprintf(stdout, "AP_FUNID_ADD_DIMENSIONS \n"); fflush(stdout);
	avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS,0);
	bound_t* m = a->closed ? a->closed : a->m;
	bound_t* mm;
	size_t i, nb = dimchange->intdim+dimchange->realdim;
	avo_t* r;
	if (!m)	{	mm = NULL;	}
	else {
	  /* check */
	  for (i=0;i<nb;i++) {
		arg_assert(dimchange->dim[i]<=a->dim,return NULL;);
		arg_assert(!i || dimchange->dim[i-1]<=dimchange->dim[i],return NULL;);
	  }

	  /* insert variables */
	  mm = avo_hmat_alloc_top(pr,a->dim+nb);

	  avo_hmat_addrem_dimensions_withAV(mm,m,dimchange->dim,
				   nb,1,a->dim,true);

	  for (i=0;i<nb;i++) {
			size_t v = 2*(i+dimchange->dim[i]);
			bound_set_int(mm[avo_matpos(v,v)],0);
			bound_set_int(mm[avo_matpos(v+1,v+1)],0);
	  }
	  /* set new variables to 0, if necessary */
	  if (project) {
		for (i=0;i<nb;i++) {
			size_t v = 2*(i+dimchange->dim[i]);
			bound_set_int(mm[avo_matpos(v+1,v)],0);
			bound_set_int(mm[avo_matpos(v,v+1)],0);
		}
	  }
	}
	/* always exact, respect closure if embedding, not projecting */


	////////////////////////////////////////////

	bound_t* m1 = a->nsc;
	bound_t* mm1;

	if (!m1) { mm1 = NULL; }
	else {
	  /* check */
	  for (i=0;i<nb;i++) {
		arg_assert(dimchange->dim[i]<=a->dim,return NULL;);
		arg_assert(!i || dimchange->dim[i-1]<=dimchange->dim[i],return NULL;);
	  }

	  /* insert variables */
	  mm1 = avo_hmat_alloc_top_nsc(pr,a->dim+nb);
	  avo_hmat_addrem_dimensions_withAV(mm1,m1,dimchange->dim,
				   nb,1,a->dim,true);

	  /* set new variables to 0, if necessary */
	}
	/* always exact, respect closure if embedding, not projecting */


	//////////////////////////////////
	if (a->closed && !project) r = avo_set_mat_nsc(pr,a,NULL,mm,mm1,destructive);
	else r = avo_set_mat_nsc(pr,a,mm,NULL,mm1,destructive);

	r->dim += nb;
	r->intdim += dimchange->intdim;
	return r;
}

avo_t* avo_remove_dimensions(ap_manager_t* man,
			     bool destructive, avo_t* a,
			     ap_dimchange_t* dimchange)
{
	//fprintf(stdout, "AP_FUNID_REMOVE_DIMENSIONS\n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS,0);
  bound_t *m, *mm,*nsc, *nnsc;
  size_t i, nb = dimchange->intdim+dimchange->realdim;
  avo_t* r;
  if (pr->funopt->algorithm>=0){
	  avo_cache_closure(pr,a);
  }
  m = a->closed ? a->closed : a->m;
  nsc = a->nsc;
  if (!m) {mm = NULL; nnsc = NULL;}
  else {
    /* check */
    for (i=0;i<nb;i++) {
      arg_assert(dimchange->dim[i]<a->dim,return NULL;);
      arg_assert(!i || dimchange->dim[i-1]<dimchange->dim[i],return NULL;);
    }

    /* remove bounds of variables */
    mm = avo_hmat_alloc(pr,a->dim-nb);
    avo_hmat_addrem_dimensions_withAV(mm,m,dimchange->dim,nb,1,a->dim,false);

	/* remove nsc of variables */
    if (!nsc) nnsc = NULL;
    else{
    	nnsc = avo_hmat_alloc(pr,a->dim-nb);
    	avo_hmat_addrem_dimensions_withAV(nnsc,nsc,dimchange->dim,nb,1,a->dim,false);
    }
  }

  ///////////////////////////////
  if (a->closed) {
    /* result is exact on Q, and closed */
    if (num_incomplete || a->intdim) flag_incomplete;
    r = avo_set_mat_nsc(pr,a,NULL,mm,nnsc,destructive);
  }
  else {
    /* not exact, not closed */
    flag_algo;
    r = avo_set_mat_nsc(pr,a,mm,NULL,nnsc,destructive);
  }
  r->dim -= nb;
  r->intdim -= dimchange->intdim;

  return r;
}

void org_avo_hmat_permute(bound_t* dst, bound_t* src,
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
	    bound_set(dst[avo_matpos2(new_ii  ,new_jj  )],src[0]);
	    bound_set(dst[avo_matpos2(new_ii  ,new_jj+1)],src[1]);
	    bound_set(dst[avo_matpos2(new_ii+1,new_jj  )],src[2*(i+1)]);
	    bound_set(dst[avo_matpos2(new_ii+1,new_jj+1)],src[2*(i+1)+1]);
	  }
	  src+=2*(i+1);
	}
}

void avo_hmat_permute(bound_t* dst, bound_t* src,
		  size_t dst_dim, size_t src_dim,
		  ap_dim_t* permutation)
{

       size_t i;
       ap_dim_t* new_per = malloc(2 * src_dim * sizeof(ap_dim_t));

       for(i = 0 ; i < src_dim ; i++)
       {
	      new_per[i] = permutation[i];
	      new_per[i + src_dim] = new_per[i] + dst_dim;
       }
       org_avo_hmat_permute(dst,src,dst_dim * 2,src_dim*2,new_per);

       free(new_per);

}

/* internal helper function: permute & change dimension */
avo_t* avo_permute_dimensions(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_dimperm_t* permutation)
{
//	fprintf(stdout, "AP_FUNID_ADD_DIMENSIONS  permute \n"); fflush(stdout);
  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS,0);
  bound_t* m = a->closed ? a->closed : a->m;
  bound_t* mm;
  arg_assert(permutation->size==a->dim,return NULL;);
  if (!m) mm = NULL;
  else {
    /* check (only bounds, not injectivity) */
    size_t i,j;
    for (i=0;i<a->dim;i++)
      arg_assert(permutation->dim[i]<a->dim,return NULL;);
    
    /* permuted copy */
    mm = avo_hmat_alloc(pr,a->dim);
    avo_hmat_permute(mm,m,a->dim,a->dim,permutation->dim);
  }

  /////////////////////////////
  bound_t* m1= a->nsc;
  bound_t* mm1;
  if (!m1) mm1 = NULL;
  else {
    /* check (only bounds, not injectivity) */
    size_t i,j;
    for (i=0;i<a->dim;i++)
      arg_assert(permutation->dim[i]<a->dim,return NULL;);

    /* permuted copy */
    mm1 = avo_hmat_alloc(pr,a->dim);
    avo_hmat_permute(mm1,m1,a->dim,a->dim,permutation->dim);
  }


  //////////////////////////
  /* always exact, respects closure */
  if (a->closed) return avo_set_mat_nsc(pr,a,NULL,mm,mm1,destructive);
  else return avo_set_mat_nsc(pr,a,mm,NULL,mm1,destructive);
}


/* ============================================================ */
/* Expansion and folding of dimensions */
/* ============================================================ */

avo_t* avo_expand(ap_manager_t* man,
		  bool destructive, avo_t* a,
		  ap_dim_t dim,
		  size_t n)
{
//	fprintf(stdout, "AP_FUNID_EXPAND\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_EXPAND,0);
  bound_t* m = a->closed ? a->closed : a->m;
  size_t i, j;
  ap_dim_t pos = (dim < a->intdim) ? a->intdim : a->dim;
  bound_t* mm;
  bound_t* nsc;
  avo_t* r;
  arg_assert(dim<a->dim,return NULL;);
  if (!m) {mm = NULL;  nsc = NULL; }
  else {
    /* insert n variables at pos */
    mm = avo_hmat_alloc_top(pr,a->dim+n);
    nsc= avo_hmat_alloc_top_nsc(pr,a->dim+n);
    avo_hmat_addrem_dimensions(mm,m,&pos,1,n,a->dim,true);
    avo_hmat_addrem_dimensions(nsc,a->nsc,&pos,1,n,a->dim,true);
    for (i=0;i<n;i++) {
      /* copy binary constraints */
      for (j=0;j<2*dim;j++) {
		bound_set(mm[avo_matpos2(2*(pos+i)  ,j)],mm[avo_matpos(2*dim  ,j)]);
		bound_set(mm[avo_matpos2(2*(pos+i)+1,j)],mm[avo_matpos(2*dim+1,j)]);
		bound_set(nsc[avo_matpos2(2*(pos+i)  ,j)],nsc[avo_matpos(2*dim  ,j)]);
		bound_set(nsc[avo_matpos2(2*(pos+i)+1,j)],nsc[avo_matpos(2*dim+1,j)]);
      }
      for (j=2*dim+2;j<2*(a->dim+n);j++) {
		bound_set(mm[avo_matpos2(2*(pos+i)  ,j)],mm[avo_matpos(j^1,2*dim+1)]);
		bound_set(mm[avo_matpos2(2*(pos+i)+1,j)],mm[avo_matpos(j^1,2*dim)]);
		bound_set(nsc[avo_matpos2(2*(pos+i)  ,j)],nsc[avo_matpos(j^1,2*dim+1)]);
		bound_set(nsc[avo_matpos2(2*(pos+i)+1,j)],nsc[avo_matpos(j^1,2*dim)]);
      }
      /* copy unary constraints */
      bound_set(mm[avo_matpos2(2*(pos+i),2*(pos+i)+1)],mm[avo_matpos2(2*dim,2*dim+1)]);
      bound_set(mm[avo_matpos2(2*(pos+i)+1,2*(pos+i))],mm[avo_matpos2(2*dim+1,2*dim)]);
      bound_set(nsc[avo_matpos2(2*(pos+i),2*(pos+i)+1)],nsc[avo_matpos2(2*dim,2*dim+1)]);
      bound_set(nsc[avo_matpos2(2*(pos+i)+1,2*(pos+i))],nsc[avo_matpos2(2*dim+1,2*dim)]);
    }
  }
  
  /*  exact, generally not closed */
  r = avo_set_mat_nsc(pr,a,mm,NULL,nsc,destructive);
  r->dim += n;
  if (dim<a->intdim) r->intdim += n;
  return r;
}

avo_t* avo_fold(ap_manager_t* man,
		bool destructive, avo_t* a,
		ap_dim_t* tdim,
		size_t size)
{
//	fprintf(stdout, "AP_FUNID_FOLD\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FOLD,avo_matsize(a->dim));
  bound_t* m;
  bound_t* mm;
  bound_t* nsc;
  avo_t* r;
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  m = a->closed ? a->closed : a->m;
  if (!m) {mm = NULL; nsc = NULL;}
  else {
    /* check, assuming tdim[0..(size-1)] is strictly increasing */
    size_t i,j;
    arg_assert(size>0,return NULL;);
    for (i=1;i<size;i++) {
      arg_assert(tdim[i-1]<tdim[i],return NULL;);
    }
    arg_assert(tdim[size-1]<a->dim,return NULL;);
    
    ///////////////update m//////////////
    bound_set_array(pr->tmp,m,avo_matsize(a->dim));

    /* merge binary constraints */
    for (j=0;j<2*tdim[0];j++) {
      bound_t* mm1 = pr->tmp+avo_matpos2(tdim[0]*2  ,j);
      bound_t* mm2 = pr->tmp+avo_matpos2(tdim[0]*2+1,j);
      for (i=1;i<size;i++) {
		bound_max(*mm1,*mm1,m[avo_matpos2(tdim[i]*2  ,j)]);
		bound_max(*mm2,*mm2,m[avo_matpos2(tdim[i]*2+1,j)]);
      }
    }
    for (j=2*(tdim[0]+1);j<2*a->dim;j++) {
      bound_t* mm1 = pr->tmp+avo_matpos2(tdim[0]*2  ,j);
      bound_t* mm2 = pr->tmp+avo_matpos2(tdim[0]*2+1,j);
      for (i=1;i<size;i++) {
		bound_max(*mm1,*mm1,m[avo_matpos2(tdim[i]*2  ,j)]);
		bound_max(*mm2,*mm2,m[avo_matpos2(tdim[i]*2+1,j)]);
      }
    }

    /* merge unary constraints */
    {
      bound_t* mm1 = pr->tmp+avo_matpos2(tdim[0]*2  ,tdim[0]*2+1);
      bound_t* mm2 = pr->tmp+avo_matpos2(tdim[0]*2+1,tdim[0]*2  );
      for (i=1;i<size;i++) {
		bound_max(*mm1,*mm1,m[avo_matpos2(tdim[i]*2,tdim[i]*2+1)]);
		bound_max(*mm2,*mm2,m[avo_matpos2(tdim[i]*2+1,tdim[i]*2)]);
      }
    }

    /* destroy all dimensions in tdim except the first one */
    mm = avo_hmat_alloc_top(pr,a->dim-size+1);
    avo_hmat_addrem_dimensions(mm,pr->tmp,tdim+1,size-1,1,a->dim,false);

    /* reset diagonal elements */
    bound_set_int(mm[avo_matpos(tdim[0]*2  ,tdim[0]*2  )],0);
    bound_set_int(mm[avo_matpos(tdim[0]*2+1,tdim[0]*2+1)],0);

    ///////////////update nsc//////////////
	bound_set_array(pr->tmp,a->nsc,avo_matsize(a->dim));

	/* merge binary constraints */
	for (j=0;j<2*tdim[0];j++) {
	  bound_t* nsc1 = pr->tmp+avo_matpos2(tdim[0]*2  ,j);
	  bound_t* nsc2 = pr->tmp+avo_matpos2(tdim[0]*2+1,j);
	  for (i=1;i<size;i++) {
		bound_max(*nsc1,*nsc1,a->nsc[avo_matpos2(tdim[i]*2  ,j)]);
		bound_max(*nsc2,*nsc2,a->nsc[avo_matpos2(tdim[i]*2+1,j)]);
	  }
	}
	for (j=2*(tdim[0]+1);j<2*a->dim;j++) {
	  bound_t* nsc1 = pr->tmp+avo_matpos2(tdim[0]*2  ,j);
	  bound_t* nsc2 = pr->tmp+avo_matpos2(tdim[0]*2+1,j);
	  for (i=1;i<size;i++) {
		bound_max(*nsc1,*nsc1,a->nsc[avo_matpos2(tdim[i]*2  ,j)]);
		bound_max(*nsc2,*nsc2,a->nsc[avo_matpos2(tdim[i]*2+1,j)]);
	  }
	}

	/* merge unary constraints */
	{
	  bound_t* nsc1 = pr->tmp+avo_matpos2(tdim[0]*2  ,tdim[0]*2+1);
	  bound_t* nsc2 = pr->tmp+avo_matpos2(tdim[0]*2+1,tdim[0]*2  );
	  for (i=1;i<size;i++) {
		bound_max(*nsc1,*nsc1,a->nsc[avo_matpos2(tdim[i]*2,tdim[i]*2+1)]);
		bound_max(*nsc2,*nsc2,a->nsc[avo_matpos2(tdim[i]*2+1,tdim[i]*2)]);
	  }
	}

	/* destroy all dimensions in tdim except the first one */
	nsc = avo_hmat_alloc_top_nsc(pr,a->dim-size+1);
	avo_hmat_addrem_dimensions(nsc,pr->tmp,tdim+1,size-1,1,a->dim,false);

	/* reset diagonal elements */
	bound_set_int(nsc[avo_matpos(tdim[0]*2  ,tdim[0]*2  )],0);
	bound_set_int(nsc[avo_matpos(tdim[0]*2+1,tdim[0]*2+1)],0);

    man->result.flag_exact = false;
  }

  if (a->closed) {
    /* result is optimal on Q, not closed */
    if (num_incomplete || a->intdim) flag_incomplete;
    r = avo_set_mat_nsc(pr,a,mm,NULL,nsc,destructive);
  }
  else {
    /* not exact, not closed */
    flag_algo;
    r = avo_set_mat_nsc(pr,a,mm,NULL,nsc,destructive);
  }
  r->dim -= size-1;
  if (tdim[0]<r->intdim) r->intdim -= size-1;
  return r;
}
