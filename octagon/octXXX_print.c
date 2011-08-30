/*
 * octXXX_print.c
 *
 * Printing and serialization
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
#include "eitvXXX.h"

/* ============================================================ */
/* Printing */
/* ============================================================ */

/* internal printing... */
static inline void octXXX_print_bounds(FILE*stream, octXXX_internal_t* pr,
				       boundXXX_t* m, size_t dim,
				       char** name_of_dim)
{
  size_t i,j;
  ap_lincons0_t l;

  ap_lincons0_init(l,numXXX_scalar,2);
  for (i=0;i<2*dim;i++)
    for (j=0;j<=(i|1);j++,m++) {
      if (i==j || boundXXX_infty(*m)) continue;
      lincons0_of_boundXXX(pr,l,i,j,*m);
      ap_lincons0_fprint(stream,l,name_of_dim);
      fprintf(stream,"\n");
    }
  ap_lincons0_clear(l);
}

void octXXX_fprint(FILE* stream, ap_manager_t* man, octXXX_t* a, char** name_of_dim)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FPRINT,0);
  if (pr->option.print_closure) octXXX_cache_closure(pr,a);
  if (!a->closed && !a->m) {
    fprintf(stream,"empty octagon of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else {
    boundXXX_t* m = a->closed ? a->closed : a->m;
    fprintf(stream,"octagon of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
    /* if not closed, print the original matrix, not the closed cache */
    octXXX_print_bounds(stream,pr,m,a->dim,name_of_dim);
    if (pr->conv) flag_conv;
  }
}

void octXXX_fprintdiff(FILE* stream, ap_manager_t* man,
		    octXXX_t* a1, octXXX_t* a2, char** name_of_dim)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FPRINTDIFF,0);
  arg_assert(a1->dim==a2->dim && a1->intdim==a2->intdim,);
  if (pr->option.printdiff_closure){
    octXXX_cache_closure(pr,a1);
    octXXX_cache_closure(pr,a2);
  }
  if (!a1->closed && !a1->m) {
    if (a2->closed || a2->m) {
      fprintf(stream,"octagon1 = empty\noctagon2 =\n");
      octXXX_print_bounds(stream,pr,a2->m?a2->m:a2->closed,a2->dim,name_of_dim);
      if (pr->conv) flag_conv;
    }
    else fprintf(stream,"octagon1 = octagon2 = empty\n");
  }
  else if (!a2->closed && !a2->m) {
      fprintf(stream,"octagon1 =\n");
      octXXX_print_bounds(stream,pr,a1->m?a1->m:a1->closed,a1->dim,name_of_dim);
      fprintf(stream,"octagon2 = empty\n");
      if (pr->conv) flag_conv;
  }
  else {
    boundXXX_t* m1 = a1->closed ? a1->closed : a1->m;
    boundXXX_t* m2 = a2->closed ? a2->closed : a2->m;
    size_t i,j,n=0;
    ap_lincons0_t l;

    ap_lincons0_init(l,numXXX_scalar,2);
    for (i=0;i<2*a1->dim;i++)
      for (j=0;j<=(i|1);j++,m1++,m2++) {
	if (i==j || !boundXXX_cmp(*m1,*m2)) continue;

	lincons0_of_boundXXX(pr,l,i,j,*m1);
	ap_lincons0_fprint(stream,l,name_of_dim);

	fprintf(stream," / ");

	lincons0_of_boundXXX(pr,l,i,j,*m2);
	ap_lincons0_fprint(stream,l,name_of_dim);

	fprintf(stream, "\n");

	n++;
      }
    if (!n) fprintf(stream,"octagon1 = octagon2\n");
    if (pr->conv) flag_conv;
    ap_lincons0_clear(l);
  }
}

void octXXX_fdump(FILE* stream, ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_FDUMP,0);
  fprintf(stream,"octagon of dim (%lu,%lu)\n",
	  (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  if (a->m) {
    fprintf(stream,"matrix:\n");
    hmatXXX_fdump(stream,pr,a->m,a->dim);
  }
  else fprintf(stream,"matrix: NULL\n");
  if (a->closed) {
    fprintf(stream,"closed matrix:\n");
    hmatXXX_fdump(stream,pr,a->closed,a->dim);
  }
  else fprintf(stream,"closed matrix: NULL\n");
}



/* ============================================================ */
/* Serialization */
/* ============================================================ */

/* raw format:
   0: uchar:  numXXX_serialize_id
   1: uchar:  state (0=empty, 1=not closed, 2=closed)
   2: uint32: intdim
   6: uint32: realdim
  10:   -   : half matrix, as flat array of boundXXX_t
 */

ap_membuf_t octXXX_serialize_raw(ap_manager_t* man, octXXX_t* a)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_SERIALIZE_RAW,0);
  ap_membuf_t buf;
  size_t n = 10;
  if (a->m)
    n += boundXXX_serialized_size_array(a->m,matsize(a->dim));
  else if (a->closed)
    n += boundXXX_serialized_size_array(a->closed,matsize(a->dim));
  checked_malloc(buf.ptr,char,n,buf.size=0;return buf;);
  ((unsigned char*)buf.ptr)[0] = numXXX_serialize_id();
  num_dump_word32((char*)buf.ptr+2,a->dim);
  num_dump_word32((char*)buf.ptr+6,a->intdim);
  n = 10;
  if (a->m) {
    ((char*)buf.ptr)[1] = 1;
    buf.size = 10 + boundXXX_serialize_array
      ((char*)buf.ptr+10,a->m,matsize(a->dim));

  }
  else if (a->closed) {
    ((char*)buf.ptr)[1] = 2;
    buf.size = 10 + boundXXX_serialize_array
      ((char*)buf.ptr+10,a->closed,matsize(a->dim));
  }
  else {
    ((char*)buf.ptr)[1] = 0;
    buf.size = 10;
  }
  return buf;
}

octXXX_t* octXXX_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  octXXX_internal_t* pr = octXXX_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW,0);
  unsigned char id = ((unsigned char*)ptr)[0];
  char state = ((char*)ptr)[1];
  size_t dim = num_undump_word32((char*)ptr+2);
  size_t intdim = num_undump_word32((char*)ptr+6);
  size_t dummy;
  octXXX_t* r = octXXX_alloc_internal(pr,dim,intdim);
  arg_assert(id==numXXX_serialize_id(),octXXX_free_internal(pr,r);return NULL;);
  arg_assert(state<3,octXXX_free_internal(pr,r);return NULL;);
  if (!size) size = &dummy;
  switch (state) {
  case 0:
    *size = 10;
    break;
  case 1:
    r->m = hmatXXX_alloc(pr,dim);
    *size = 10 + boundXXX_deserialize_array(r->m,(char*)ptr+10,matsize(dim));
    break;
  case 2:
    r->closed = hmatXXX_alloc(pr,dim);
    *size = 10 + boundXXX_deserialize_array(r->closed,(char*)ptr+10,matsize(dim));
    break;
  default: assert(0);
  }
  return r;
}
