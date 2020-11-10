/*
 * itvp_representation.c
 *
 * Simple functions related to the representation of itvpols
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "itvp.h"
#include "itvp_internal.h"
#include "itvp_user.h"
#include "ap_generic.h"
#include <glpk.h> /* linear programming lib */

/* ============================================================ */
/* Memory Related */
/* ============================================================ */

/* generic allocation routine, returns a bottom itvpol */
inline itvp_t* itvp_alloc_internal(itvp_internal_t* pr, size_t dim, size_t intdim)
{
  itvp_t* r;
  checked_malloc(pr,r,itvp_t,1,return NULL;);
  r->dim   = dim;
  r->intdim= intdim;
  r->ncons = 0;
  r->cons  = r->bnds = NULL;
  r->flag  = EMPTY_POL; 
  return r;
}

inline itvp_t* itvp_alloc_top(itvp_internal_t* pr, size_t dim, size_t intdim)
{
  itvp_t* r = itvp_alloc_internal(pr,dim,intdim);
  r->flag = UNIVERSE_POL; 
  return r;
}

inline itvp_t* itvp_alloc_urgent(itvp_internal_t* pr, size_t dim, size_t intdim, size_t ncons)
{
  itvp_t* r;
  checked_malloc(pr,r,itvp_t,1,return NULL;);
  r->dim   = dim;
  r->intdim= intdim;
  r->ncons = ncons;
  if(ncons>0){
    r->flag = GENERAL_POL;	
    checked_malloc(pr,r->cons,numdbl_t,ncons*(2*dim+1),return NULL;);
    checked_malloc(pr,r->bnds,numdbl_t,2*dim,return NULL;);
  }
  else{
    r->flag  = EMPTY_POL; 
    checked_malloc(pr,r->bnds,numdbl_t,2*dim,return NULL;);
  }   
  return r;
}

inline void itvp_free_internal(itvp_internal_t* pr, itvp_t* a)
{
  if(a!=NULL){
    checked_free(a->cons);
    checked_free(a->bnds);
    checked_free(a);
  }
}

inline itvp_t* itvp_copy_internal(itvp_internal_t* pr, itvp_t* a)
{
  itvp_t* r = itvp_alloc_internal(pr,a->dim,a->intdim);
  r->flag = a->flag;
  r->ncons= a->ncons;
  if(r->ncons >0){
    checked_malloc(pr,r->cons,numdbl_t,r->ncons*(2*r->dim+1),return NULL;);
    checked_malloc(pr,r->bnds,numdbl_t,2*r->dim,return NULL;);
    memcpy(r->bnds,a->bnds,a->dim*2* sizeof(numdbl_t)); 
    memcpy(r->cons,a->cons,a->ncons*(2*a->dim+1)*sizeof(numdbl_t));
  }
  return r;
}

itvp_t* itvp_copy(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_COPY,0);
  return itvp_copy_internal(pr,a);
}

void itvp_free(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_FREE,0);
  itvp_free_internal(pr,a);
}

/* Return the abstract size of a polyhedron, which is the sum of the number of
   coefficients of its constraints and the number of bounds. */
size_t itvp_size(ap_manager_t* man, itvp_t* a)
{
  size_t s;
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_ASIZE,0);
  s=a->ncons*(2*a->dim+1)+2*a->dim;
  return s;
}

/* ============================================================ */
/* Basic Constructors */
/* ============================================================ */
/*
The empty polyhedron is just defined by the absence of both
constraints and bounds, but with the flag of EMPTY_POL.
*/
itvp_t* itvp_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  fprintf(stdout, "AP_FUNID_BOTTOM\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_BOTTOM,0);
  itvp_t* r = itvp_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

/*
The universe polyhedron is just defined by the absence of both
constraints matrix and bounds, but with the flag of UNIVERSE_POL.
*/
itvp_t* itvp_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  fprintf(stdout, "AP_FUNID_TOP\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_TOP,0);
  itvp_t* r = itvp_alloc_internal(pr,intdim+realdim,intdim);
  r->flag = UNIVERSE_POL; 
  return r;
}

/* Abstract an hypercube defined by the array of intervals of size
   intdim+realdim.  */
itvp_t* itvp_of_box(ap_manager_t* man, size_t intdim, size_t realdim,
		  ap_interval_t ** t)
{
  fprintf(stdout, "AP_FUNID_OF_BOX\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_OF_BOX,0);
  itvp_t* r = itvp_alloc_internal(pr,intdim+realdim,intdim);
  size_t i,j,n2;
  if (!t) return r; /* empty */
  for (i=0;i<r->dim;i++)
    if (ap_scalar_cmp(t[i]->inf,t[i]->sup)>0) return r; /* empty */
  /* bounds */
  checked_malloc(pr,r->bnds,numdbl_t,2*(r->dim),return NULL;);
  for (i=0;i<r->dim;i++){
    *(r->bnds+2*i)  =t[i]->inf->val.dbl; /* lower bound */
    *(r->bnds+2*i+1)=t[i]->sup->val.dbl; /* upper bound */
  }
  /* bounds to constraints */
  r->cons=itvp_bnds_to_cons(pr,r->bnds,r->dim, &r->ncons);
  man->result.flag_exact = man->result.flag_best = true;
  return r;
}


itvp_t* itvp_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* ar)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,0);
  itvp_t* r = itvp_alloc_internal(pr,intdim+realdim,intdim);
  return r;
}

ap_abstract0_t*
ap_abstract0_itvp_of_generator_array(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_generator0_array_t* array)
{
  return abstract0_of_itvp(man,itvp_of_generator_array(man,intdim,realdim,array));
}

/* ============================================================ */
/* Accessors */
/* ============================================================ */

ap_dimension_t itvp_dimension(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_DIMENSION,0);
  ap_dimension_t r;
  r.intdim = a->intdim;
  r.realdim = a->dim-a->intdim;
  return r;
}

/* ============================================================ */
/* Control of internal representation: NOT IMPLEMENTED */
/* ============================================================ */

/* NOT IMPLEMENTED: do nothing */
void itvp_minimize(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_MINIMIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
void itvp_canonicalize(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_CANONICALIZE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* NOT IMPLEMENTED: do nothing */
int itvp_hash(ap_manager_t* man, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_HASH,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  return 0;
}

/* NOT IMPLEMENTED: do nothing */
void itvp_approximate(ap_manager_t* man, itvp_t* a, int algorithm)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_APPROXIMATE,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
}

/* ============================================================ */
/* Topological losure operation */
/* ============================================================ */

/* actually identity */
itvp_t* itvp_closure(ap_manager_t* man, bool destructive, itvp_t* a)
{
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_CLOSURE,0);
  if (destructive) return a;
  else return itvp_copy_internal(pr,a);
}


/* ============================================================ */
/* Managers */
/* ============================================================ */
int itvp_printme(void* info, const char* msg) {
    return 1;
}


void itvp_internal_free(itvp_internal_t* pr)
{
  free(pr);
}

ap_manager_t* itvp_manager_alloc(void)
{
  size_t i;
  ap_manager_t* man;
  itvp_internal_t* pr;

#if num_fpu
  if (!ap_fpu_init()) {
    fprintf(stderr,"itvp_manager_alloc cannot change the FPU rounding mode\n");
    /*return NULL;*/
  }
#endif

  pr = (itvp_internal_t*)malloc(sizeof(itvp_internal_t));
  assert(pr);
	/* intialize the mark for differences between test meet or assign meet*/
  pr->newdims=0;
	/* only support num_name = 'NUM_DOUBLE' at present */
  man = ap_manager_alloc("itvp","1.0 with " num_name,pr,
			 (void (*)(void*))itvp_internal_free);

  pr->man = man;

  man->funptr[AP_FUNID_COPY] = &itvp_copy;
  man->funptr[AP_FUNID_FREE] = &itvp_free;
  man->funptr[AP_FUNID_ASIZE] = &itvp_size;
  man->funptr[AP_FUNID_MINIMIZE] = &itvp_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = &itvp_canonicalize;
  man->funptr[AP_FUNID_HASH] = &itvp_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = &itvp_approximate;
  man->funptr[AP_FUNID_FPRINT] = &itvp_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = &itvp_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = &itvp_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = &itvp_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = &itvp_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = &itvp_bottom;
  man->funptr[AP_FUNID_TOP] = &itvp_top;
  man->funptr[AP_FUNID_OF_BOX] = &itvp_of_box;
  man->funptr[AP_FUNID_DIMENSION] = &itvp_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = &itvp_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = &itvp_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = &itvp_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = &itvp_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &itvp_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = &itvp_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = &itvp_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = &itvp_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = &itvp_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = &itvp_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = &itvp_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = &itvp_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = &itvp_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = &itvp_to_tcons_array;
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &itvp_to_generator_array;
  man->funptr[AP_FUNID_MEET] = &itvp_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = &itvp_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &itvp_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = &itvp_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = &itvp_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = &itvp_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = &itvp_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &itvp_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &itvp_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &itvp_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &itvp_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = &itvp_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = &itvp_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &itvp_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = &itvp_forget_array;
  man->funptr[AP_FUNID_EXPAND] = &itvp_expand;
  man->funptr[AP_FUNID_FOLD] = &itvp_fold;
  man->funptr[AP_FUNID_WIDENING] = &itvp_widening;
  man->funptr[AP_FUNID_CLOSURE] = &itvp_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,i,false);

  return man;
}

itvp_t* itvp_of_abstract0(ap_abstract0_t* a)
{
  return (itvp_t*)a->value;
}

ap_abstract0_t* abstract0_of_itvp(ap_manager_t* man, itvp_t* itvp)
{
  ap_abstract0_t* r = malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = itvp;
  r->man = ap_manager_copy(man);
  return r;
}
