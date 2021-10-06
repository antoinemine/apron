/*
 * fpp_print.c
 *
 * Printing and serialization
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

/* ============================================================ */
/* internal  printing */
/* ============================================================ */

/* constraints printing... */
void fpp_constraints_fprint(FILE* stream,ap_manager_t* man, fpp_t* a, char** name_of_dim)
{
  size_t i,j;
  num_t *p;
  numdbl_t q,qInt;

  if (name_of_dim){
    ap_lincons0_array_t cons = fpp_to_lincons_array(man,a);
    ap_lincons0_array_fprint(stream,&cons,name_of_dim);
    ap_lincons0_array_clear(&cons);
  }
  else{
    for (i=0;i<a->ncons;i++) {
      fprintf(stream,"%ld:  ", i+1);
      p=(num_t *)(a->cons+i*(a->dim+1));
      double_set_num(&q,*p);
      qInt=trunc(q);
      if(qInt==q ){ /* integer */
	   fprintf(stream,"%ld", (long int)q); /* the first column is for constants */
      }
      else fprintf(stream,FPRTL, q); /* the first column is for constants */
      fprintf(stream," >=");
      for (j=0;j<a->dim;j++) {
        double_set_num(&q,*(p+j+1));
        if((j!=0)&&(q>=0)) fprintf(stream," +");
        else fprintf(stream," ");
        if(q!=0) {
      	   qInt=trunc(q);
      	   if(qInt==q ){ /* integer */
	   	fprintf(stream,"%ld", (long int)q); /* the first column is for constants */
      	   }
           else fprintf(stream,FPRTL, q);
	   fprintf(stream,"*x%ld", j);
        }
      }
      fprintf(stream,"\n");
    }
   }
}

/* print the bounds, similar to box_fprint() */
void fpp_bounds_fprint(FILE* stream, fpp_t* a, char** name_of_dim)
{
  size_t i;
  size_t nbdims = a->dim;
  numdbl_t p,pInt;

  fprintf(stream,"\n");
  for(i=0; i<nbdims; i++){
     if (name_of_dim){
	fprintf(stream,"%8s in ", name_of_dim[i]);
     } else {
	fprintf(stream,"x%ld in ", (long)i);
     }
     p=*(a->bnds+i*2);
     fprintf(stream,"[");
     if(p==-MAX_VARBND) fprintf(stream,FPRTE,p);
     else{
        pInt=trunc(p);
        if(pInt==p ){ /* integer */
	   fprintf(stream,"%ld", (long int)p); /* the first column is for constants */
        }
        else fprintf(stream,FPRTE,p);
     }
     fprintf(stream," , ");
     p=*(a->bnds+i*2+1);
     if(p==MAX_VARBND) fprintf(stream,FPRTE,p);
     else{
        pInt=trunc(p);
        if(pInt==p ){ /* integer */
	   fprintf(stream,"%ld", (long int)p); /* the first column is for constants */
        }
        else fprintf(stream,FPRTE,p);
     }
     fprintf(stream,"]\n");
  }
}

/* ============================================================ */
/* Apron fppol printing */
/* ============================================================ */

void fpp_fprint(FILE* stream, ap_manager_t* man, fpp_t* a, char** name_of_dim)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_FPRINT,0);
  if (a->flag==EMPTY_POL) {
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else if (a->flag==UNIVERSE_POL) {
    fprintf(stream,"universe polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else { /* GENERALPOL */
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
	/* Constraints */
    fprintf(stream,"Constraints: ");
    fpp_constraints_fprint(stream,man,a,name_of_dim);
    fprintf(stream,"Bounds: ");
    fpp_bounds_fprint(stream,a,name_of_dim);
  }
}

void fpp_fprintdiff(FILE* stream, ap_manager_t* man,
		    fpp_t* a1, fpp_t* a2, char** name_of_dim)
{
  fpp_init_from_manager(man,AP_FUNID_FPRINTDIFF,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_FPRINTDIFF,NULL);
}

void fpp_fdump(FILE* stream, ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_FDUMP,0);
  if (a->flag==EMPTY_POL) {
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else if (a->flag==UNIVERSE_POL) {
    fprintf(stream,"universe polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else { /* GENERALPOL */
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
    fprintf(stream,"Constraints: %lu * %lu\n", (unsigned long)a->ncons, (unsigned long)(a->dim+1));
    fpp_constraints_fprint(stream,man,a,NULL);
    fprintf(stream,"Bounds: ");
    fpp_bounds_fprint(stream,a,NULL);
  }
}

/* ********************************************************************** */
/* Serialization */
/* ********************************************************************** */
ap_membuf_t fpp_serialize_raw(ap_manager_t* man, fpp_t* a)
{
  ap_membuf_t membuf;
  fpp_init_from_manager(man,AP_FUNID_SERIALIZE_RAW,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,NULL);
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}
fpp_t* fpp_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  fpp_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,NULL);
  return NULL;
}
