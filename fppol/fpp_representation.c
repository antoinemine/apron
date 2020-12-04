/*
 * fpp_representation.c
 *
 * Simple functions related to the representation of fppols
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
#include "fpp_user.h"
#include "ap_generic.h"
#include <glpk.h> /* linear programming lib */

/* ============================================================ */
/* Memory Related */
/* ============================================================ */

/* generic allocation routine, returns a bottom fppol */
inline fpp_t* fpp_alloc_internal(fpp_internal_t* pr, size_t dim, size_t intdim)
{
  fpp_t* r;
  checked_malloc(pr,r,fpp_t,1, return NULL;);
  r->dim   = dim;
  r->intdim= intdim;
  r->ncons = 0;
  r->cons  = r->bnds = NULL;
  r->flag  = EMPTY_POL;
  return r;
}

inline fpp_t* fpp_alloc_top(fpp_internal_t* pr, size_t dim, size_t intdim)
{
  fpp_t* r = fpp_alloc_internal(pr,dim,intdim);
  r->flag = UNIVERSE_POL;
  return r;
}

inline fpp_t* fpp_alloc_urgent(fpp_internal_t* pr, size_t dim, size_t intdim, size_t ncons)
{
  fpp_t* r;
  checked_malloc(pr,r,fpp_t,1, return NULL;);
  r->dim   = dim;
  r->intdim= intdim;
  r->ncons = ncons;
  if(ncons>0){
    r->flag = GENERAL_POL;
    checked_malloc(pr,r->cons,numdbl_t,ncons*(dim+1), return NULL;);
    checked_malloc(pr,r->bnds,numdbl_t,2*dim, return NULL;);
  }
  else{
    r->flag  = EMPTY_POL;
    checked_malloc(pr,r->bnds,numdbl_t,2*dim, return NULL;);
  }
  return r;
}

inline void fpp_free_internal(fpp_internal_t* pr, fpp_t* a)
{
  if(a!=NULL){
    checked_free(a->cons);
    checked_free(a->bnds);
    checked_free(a);
  }
}

inline fpp_t* fpp_copy_internal(fpp_internal_t* pr, fpp_t* a)
{
  fpp_t* r = fpp_alloc_internal(pr,a->dim,a->intdim);
  r->flag = a->flag;
  r->ncons= a->ncons;
  if(r->ncons >0){
    checked_malloc(pr,r->cons,numdbl_t,r->ncons*(r->dim+1), return NULL;);
    checked_malloc(pr,r->bnds,numdbl_t,2*r->dim, return NULL;);
    memcpy(r->bnds,a->bnds,a->dim*2* sizeof(numdbl_t));
    memcpy(r->cons,a->cons,a->ncons*(a->dim+1)*sizeof(numdbl_t));
  }
  return r;
}

fpp_t* fpp_copy(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_COPY,0);
  return fpp_copy_internal(pr,a);
}

void fpp_free(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_FREE,0);
  fpp_free_internal(pr,a);
}

/* Return the abstract size of a polyhedron, which is the sum of the number of
   coefficients of its constraints and the number of bounds. */
size_t fpp_size(ap_manager_t* man, fpp_t* a)
{
  size_t s;
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_ASIZE,0);
  s=a->ncons*(a->dim+1)+2*a->dim;
  return s;
}

/* ============================================================ */
/* Basic Constructors */
/* ============================================================ */
/*
The empty polyhedron is just defined by the absence of both
constraints and bounds, but with the flag of EMPTY_POL.
*/
fpp_t* fpp_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  fprintf(stdout, "AP_FUNID_BOTTOM\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_BOTTOM,0);
  fpp_t* r = fpp_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

/*
The universe polyhedron is just defined by the absence of both
constraints matrix and bounds, but with the flag of UNIVERSE_POL.
*/
fpp_t* fpp_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  fprintf(stdout, "AP_FUNID_TOP\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_TOP,0);
  fpp_t* r = fpp_alloc_internal(pr,intdim+realdim,intdim);
  r->flag = UNIVERSE_POL;
  return r;
}

/* Abstract an hypercube defined by the array of intervals of size
   intdim+realdim.  */
fpp_t* fpp_of_box(ap_manager_t* man, size_t intdim, size_t realdim,
		  ap_interval_t ** t)
{
  fprintf(stdout, "AP_FUNID_OF_BOX\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_OF_BOX,0);
  fpp_t* r = fpp_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,n2;
  if (!t) return r; /* empty */
  for (i=0;i<r->dim;i++)
    if (ap_scalar_cmp(t[i]->inf,t[i]->sup)>0) return r; /* empty */
  /* bounds */
  checked_malloc(pr,r->bnds,numdbl_t,2*(r->dim), return NULL;);
  for (i=0;i<r->dim;i++){
    *(r->bnds+2*i)  =t[i]->inf->val.dbl; /* lower bound */
    *(r->bnds+2*i+1)=t[i]->sup->val.dbl; /* upper bound */
  }
  /* bounds to constraints */
  r->cons=fpp_bnds_to_cons(pr,r->bnds,r->dim, &r->ncons);
  man->result.flag_exact = man->result.flag_best = true;
  return r;
}


fpp_t* fpp_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* ar)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,0);
  fpp_t* r = fpp_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

ap_abstract0_t*
ap_abstract0_fpp_of_generator_array(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_generator0_array_t* array)
{
  return abstract0_of_fpp(man,fpp_of_generator_array(man,intdim,realdim,array));
}

/* ============================================================ */
/* Accessors */
/* ============================================================ */

ap_dimension_t fpp_dimension(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_DIMENSION,0);
  ap_dimension_t r;
  r.intdim = a->intdim;
  r.realdim = a->dim-a->intdim;
  return r;
}

/* ============================================================ */
/* Control of internal representation: NOT IMPLEMENTED */
/* ============================================================ */

/* NOT IMPLEMENTED: do nothing */
void fpp_minimize(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_MINIMIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
void fpp_canonicalize(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_CANONICALIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
int fpp_hash(ap_manager_t* man, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_HASH,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  return 0;
}

/* NOT IMPLEMENTED: do nothing */
void fpp_approximate(ap_manager_t* man, fpp_t* a, int algorithm)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_APPROXIMATE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* ============================================================ */
/* Topological losure operation */
/* ============================================================ */

/* actually identity */
fpp_t* fpp_closure(ap_manager_t* man, bool destructive, fpp_t* a)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_CLOSURE,0);
  if (destructive) return a;
  else return fpp_copy_internal(pr,a);
}


/* ============================================================ */
/* Managers */
/* ============================================================ */
int printme(void* info, const char* msg) {
    return 1;
}


void fpp_internal_free(fpp_internal_t* pr)
{
  free(pr);
}

ap_manager_t* fpp_manager_alloc(void)
{
  size_t i;
  ap_manager_t* man;
  fpp_internal_t* pr;

#if num_fpu
  if (!ap_fpu_init()) {
    fprintf(stderr,"fpp_manager_alloc cannot change the FPU rounding mode\n");
    /*return NULL;*/
  }
#endif

#if DISABLE_LP_OUTPUT==1
  glp_term_hook(printme,NULL); /*disable output*/
#endif

  pr = (fpp_internal_t*)malloc(sizeof(fpp_internal_t));
  assert(pr);
	/* intialize the mark for differences between test meet or assign meet*/
  pr->newdims=0;
	/* only support num_name = 'NUM_DOUBLE' at present */
  man = ap_manager_alloc("fpp","1.0 with " num_name,pr,
			 (void (*)(void*))fpp_internal_free);

  pr->man = man;

  man->funptr[AP_FUNID_COPY] = &fpp_copy;
  man->funptr[AP_FUNID_FREE] = &fpp_free;
  man->funptr[AP_FUNID_ASIZE] = &fpp_size;
  man->funptr[AP_FUNID_MINIMIZE] = &fpp_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = &fpp_canonicalize;
  man->funptr[AP_FUNID_HASH] = &fpp_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = &fpp_approximate;
  man->funptr[AP_FUNID_FPRINT] = &fpp_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = &fpp_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = &fpp_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = &fpp_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = &fpp_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = &fpp_bottom;
  man->funptr[AP_FUNID_TOP] = &fpp_top;
  man->funptr[AP_FUNID_OF_BOX] = &fpp_of_box;
  man->funptr[AP_FUNID_DIMENSION] = &fpp_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = &fpp_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = &fpp_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = &fpp_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = &fpp_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &fpp_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = &fpp_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = &fpp_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = &fpp_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = &fpp_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = &fpp_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = &fpp_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = &fpp_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = &fpp_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = &fpp_to_tcons_array;
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &fpp_to_generator_array;
  man->funptr[AP_FUNID_MEET] = &fpp_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = &fpp_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &fpp_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = &fpp_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = &fpp_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = &fpp_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = &fpp_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &fpp_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &fpp_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &fpp_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &fpp_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = &fpp_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = &fpp_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &fpp_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = &fpp_forget_array;
  man->funptr[AP_FUNID_EXPAND] = &fpp_expand;
  man->funptr[AP_FUNID_FOLD] = &fpp_fold;
  man->funptr[AP_FUNID_WIDENING] = &fpp_widening;
  man->funptr[AP_FUNID_CLOSURE] = &fpp_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,i,false);

  return man;
}

fpp_t* fpp_of_abstract0(ap_abstract0_t* a)
{
  return (fpp_t*)a->value;
}

ap_abstract0_t* abstract0_of_fpp(ap_manager_t* man, fpp_t* fpp)
{
  ap_abstract0_t* r = malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = fpp;
  r->man = ap_manager_copy(man);
  return r;
}
