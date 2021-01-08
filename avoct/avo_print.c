/*
 * avo_print.c
 *
 * Printing and serialization
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
/* Printing */
/* ============================================================ */

/* internal printing... */
static inline void avo_print_bounds(FILE*stream, avo_internal_t* pr,
				    bound_t* m, bound_t* nsc, size_t dim,
				    char** name_of_dim)
{
  size_t i,j;
  for (i=0;i<4*dim;i++)
    for (j=0;j<=(i|1);j++,m++) {
      ap_lincons0_t l;
      if (i==j || bound_infty(*m)) continue;
      l = avo_lincons_of_bound(pr,i,j,*m,*nsc,dim);
      ap_lincons0_fprint(stream,&l,name_of_dim);
      ap_lincons0_clear(&l);
      fprintf(stream,"\n");
    }
}

void avo_fprint(FILE* stream, ap_manager_t* man, avo_t* a, char** name_of_dim)
{
  //fprintf(stdout, "AP_FUNID_FPRINT\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FPRINT,0);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    fprintf(stream,"empty avOctagon of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else {
    bound_t* m = a->closed ? a->closed : a->m;
    fprintf(stream,"avOctagon of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
    /* if not closed, print the original matrix, not the closed cache */
    avo_print_bounds(stream,pr,m,a->nsc, a->dim,name_of_dim);
    if (pr->conv) flag_conv;
  }
}

void avo_fprintdiff(FILE* stream, ap_manager_t* man,
		    avo_t* a1, avo_t* a2, char** name_of_dim)
{
  //fprintf(stdout, "AP_FUNID_FPRINTDIFF\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FPRINTDIFF,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a1);
  if (pr->funopt->algorithm>=0) avo_cache_closure(pr,a2);
  if (!a1->closed && !a1->m) {
    if (a2->closed || a2->m) {
      avo_print_bounds(stream,pr,a2->m?a2->m:a2->closed,a2->nsc,a2->dim,name_of_dim);
      if (pr->conv) flag_conv;
    }
  }
  else if (!a2->closed && !a2->m) {
      avo_print_bounds(stream,pr,a1->m?a1->m:a1->closed,a1->nsc,a1->dim,name_of_dim);
      if (pr->conv) flag_conv;
  }
  else {
    bound_t* m1 = a1->closed ? a1->closed : a1->m;
    bound_t* m2 = a2->closed ? a2->closed : a2->m;
    bound_t* nsc1 = a1->nsc;
    bound_t* nsc2 = a2->nsc;
    size_t i,j,n=0;
    for (i=0;i<2*a1->dim;i++)
      for (j=0;j<=(i|1);j++,m1++,m2++,nsc1++,nsc2++) {
    	ap_lincons0_t l;
    	if (i==j || (!bound_cmp(*m1,*m2) && !bound_cmp(*nsc1,*nsc2)) ) continue;
    	l = avo_lincons_of_bound(pr,i,j,*m1,*nsc1,a1->dim);
		ap_lincons0_fprint(stream,&l,name_of_dim);
		ap_lincons0_clear(&l);
		fprintf(stream," / ");
		l = avo_lincons_of_bound(pr,i,j,*m2,*nsc2,a1->dim);
		ap_lincons0_fprint(stream,&l,name_of_dim);
		ap_lincons0_clear(&l);
		fprintf(stream, "\n");
		n++;
    }
    if (pr->conv) flag_conv;
  }
}

void avo_fdump(FILE* stream, ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_FDUMP\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_FDUMP,0);
  fprintf(stream,"avOctagon of dim (%lu,%lu)\n",
	  (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  if (a->m) {
    fprintf(stream,"matrix:\n");
    avo_hmat_fdump(stream,a->m,a->dim);
  }
  else fprintf(stream,"matrix: NULL\n");
  if (a->closed) {
    fprintf(stream,"closed matrix:\n");
    avo_hmat_fdump(stream,a->closed,a->dim);
  }
  else fprintf(stream,"closed matrix: NULL\n");
  if (a->nsc) {
    fprintf(stream,"strictness matrix:\n");
    avo_hmat_fdump(stream,a->nsc,a->dim);
  }
  else fprintf(stream,"strictness matrix: NULL\n");
}



/* ============================================================ */
/* Serialization */
/* ============================================================ */

/* raw format:
   0: uchar:  num_serialize_id
   1: uchar:  state (0=empty, 1=not closed, 2=closed)
   2: uint32: intdim
   6: uint32: realdim
  10:   -   : half matrix, as flat array of bound_t
 */

ap_membuf_t avo_serialize_raw(ap_manager_t* man, avo_t* a)
{
  //fprintf(stdout, "AP_FUNID_SERIALIZE_RAW\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_SERIALIZE_RAW,0);
  ap_membuf_t buf;
  size_t n = 10;
  if (a->m) 
    n += bound_serialized_size_array(a->m,avo_matsize(a->dim));
  else if (a->closed) 
    n += bound_serialized_size_array(a->closed,avo_matsize(a->dim));
  if (a->nsc)
    n += bound_serialized_size_array(a->nsc,avo_matsize(a->dim));
  checked_malloc(buf.ptr,char,n,buf.size=0;return buf;);
  ((unsigned char*)buf.ptr)[0] = num_serialize_id();
  num_dump_word32((char*)buf.ptr+2,a->dim);
  num_dump_word32((char*)buf.ptr+6,a->intdim);
  n = 10;
  if (a->m && a->nsc == NULL) {
    ((char*)buf.ptr)[1] = 1;
    buf.size = 10 + bound_serialize_array((char*)buf.ptr+10,a->m,avo_matsize(a->dim));
    
  }
  else if (a->m && a->nsc) {
	    ((char*)buf.ptr)[1] = 2;
	    buf.size = 10 + bound_serialize_array((char*)buf.ptr+10,a->m,avo_matsize(a->dim));
	    buf.size = buf.size + bound_serialize_array((char*)buf.ptr+buf.size,a->nsc,avo_matsize(a->dim));
  }
  else if (a->closed && a->nsc == NULL) {
    ((char*)buf.ptr)[1] = 3;
    buf.size = 10 + bound_serialize_array((char*)buf.ptr+10,a->closed,avo_matsize(a->dim));
  }
  else if (a->closed && a->nsc) {
    ((char*)buf.ptr)[1] = 4;
    buf.size = 10 + bound_serialize_array((char*)buf.ptr+10,a->closed,avo_matsize(a->dim));
    buf.size = buf.size + bound_serialize_array((char*)buf.ptr+buf.size,a->nsc,avo_matsize(a->dim));
  }
  else {
    ((char*)buf.ptr)[1] = 0;
    buf.size = 10;
  }
  return buf;
}

avo_t* avo_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  //fprintf(stdout, "AP_FUNID_DESERIALIZE_RAW\n"); fflush(stdout);

  avo_internal_t* pr = avo_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW,0);
  unsigned char id = ((unsigned char*)ptr)[0];
  char state = ((char*)ptr)[1];
  size_t dim = num_undump_word32((char*)ptr+2);
  size_t intdim = num_undump_word32((char*)ptr+6);
  size_t dummy;
  avo_t* r = avo_alloc_internal(pr,dim,intdim);
  arg_assert(id==num_serialize_id(),avo_free_internal(pr,r);return NULL;);
  arg_assert(state<=4,avo_free_internal(pr,r);return NULL;);
  if (!size) size = &dummy;
  switch (state) {
	  case 0:
		*size = 10;
		break;
	  case 1:
		r->m = avo_hmat_alloc(pr,dim);
		*size = 10 + bound_deserialize_array(r->m,(char*)ptr+10,avo_matsize(dim));
		break;
	  case 2:
		r->m = avo_hmat_alloc(pr,dim);
		*size = 10 + bound_deserialize_array(r->m,(char*)ptr+10,avo_matsize(dim));
		r->nsc = avo_hmat_alloc(pr,dim);
		*size = *size + bound_deserialize_array(r->nsc,(char*)ptr+(*size),avo_matsize(dim));
		break;
	  case 3:
		r->closed = avo_hmat_alloc(pr,dim);
		*size = 10 + bound_deserialize_array(r->closed,(char*)ptr+10,avo_matsize(dim));
		break;
	  case 4:
		r->closed = avo_hmat_alloc(pr,dim);
		*size = 10 + bound_deserialize_array(r->closed,(char*)ptr+10,avo_matsize(dim));
		r->nsc = avo_hmat_alloc(pr,dim);
		*size = 10 + bound_deserialize_array(r->nsc,(char*)ptr+(*size),avo_matsize(dim));
		break;
	  default: assert(0);
  }
  return r;
}

