/* ********************************************************************** */
/* pk_equality.c: linear equalities on top of NewPolka polyhedra */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_user.h"
#include "pk_representation.h"
#include "pk_constructor.h"
#include "pk_assign.h"
#include "pk_resize.h"

#include "pkeq.h"

#include "ap_generic.h"


/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

/* Return the abstract size of a set of equalities, which is the number of
   equalities times the dimension */
size_t pkeq_size(ap_manager_t* man, pkeq_t* po)
{
  size_t s;

  assert (po->C ? (po->C->nbrows >=1) : true);

  s = (po->C) ? (po->C->nbrows-1) : 0;
  return s*(po->intdim + po->realdim);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

static
bool equality_check(pk_internal_t* pk, pkeq_t* po)
{
  if (!poly_check(pk,po)) return false;
  if ((po->C && !po->F) || (!po->C && po->F)){
     fprintf(stderr,"equality_check: po->C && !po->F || !po->C && po->F: not in canonical form: \n");
     return false;
  }
  if (po->C && po->C->nbrows != po->nbeq+1){
      fprintf(stderr,"equality_check: po->C->nbrows != po->nbeq+1\n");
     return false;
  }
  if (po->F && po->F->nbrows != po->nbline+1){
      fprintf(stderr,"equality_check: po->F->nbrows != po->nbline+1\n");
     return false;
  }
  return true;
}


static
void pk_matrix_reduce(pk_matrix_t* mat)
{
  if (mat->nbrows + 4 <= mat->_maxrows){
    pk_matrix_minimize(mat);
  }
}

static
void equality_reduce(ap_manager_t* man, pkeq_t* po)
{
  assert((!po->C && !po->F) || (po->C && po->F));
  if (po->C && po->F){
    if (po->C->nbrows > po->nbeq + 1){
      pk_internal_t* pk = (pk_internal_t*)man->internal;

      po->C->nbrows = po->nbeq + 1;
      pk_matrix_fill_constraint_top(pk,po->C,po->nbeq);
      pk_matrix_reduce(po->C);
      pk_matrix_free(po->F); po->F = NULL;
      if (po->satC){
	satmat_free(po->satC);
	po->satC = NULL;
      }
      if (po->satF){
	satmat_free(po->satF);
	po->satF = NULL;
      }
      poly_chernikova(man,po,"of result");
      if (pk->exn){
	pk->exn = AP_EXC_NONE;
	poly_set_top(pk,po);
      }
    }
  }
}
void pkeq_canonicalize(ap_manager_t* man, pkeq_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_CANONICALIZE);
  pk->funopt->algorithm = 1;
  return pk_canonicalize(man,po);
}

void pkeq_approximate(ap_manager_t* man, pkeq_t* po, int algorithm)
{
  return;
}

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t pkeq_serialize_raw(ap_manager_t* man, pkeq_t* a)
{
  ap_membuf_t membuf;
  pk_init_from_manager(man,AP_FUNID_SERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,NULL);
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}
pkeq_t* pkeq_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  pk_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,NULL);
  return NULL;
}

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* Abstract an hypercube defined by the array of intervals of size
   intdim+realdim.  */
pkeq_t* pkeq_of_box(ap_manager_t* man,
			size_t intdim, size_t realdim,
			ap_interval_t** array)
{
  size_t i;
  size_t row,dim;
  pkeq_t* po;
  itv_t itv;
  bool ok;

  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_OF_BOX);
  pk_internal_realloc_lazy(pk,intdim+realdim);

  po = poly_alloc(intdim,realdim);
  po->status =
    pk_status_conseps;

  dim = intdim + realdim;
  po->C = pk_matrix_alloc(pk->dec-1 + dim, pk->dec + dim, false);

  /* constraints */
  row = 0;
  itv_init(itv);
  ok = false;
  for (i=0; i<dim; i++){
    itv_set_ap_interval(pk->itv,itv,array[i]);
    if (itv_is_point(pk->itv,itv)){
      ok = vector_set_dim_bound(pk,po->C->p[row],
				 (ap_dim_t)i, bound_numref(itv->sup), 0,
				 intdim,realdim,
				 true);
      if (!ok){
	pk_matrix_free(po->C);
	po->C = NULL;
	return po;
      }
      row++;
    }
  }
  itv_clear(itv);
  pk_matrix_fill_constraint_top(pk,po->C,row);
  po->C->nbrows = pk->dec - 1 + row;
  pk_matrix_reduce(po->C);
  pk_canonicalize(man,po);
  man->result.flag_exact = man->result.flag_best = true;
  return po;
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool pkeq_is_eq(ap_manager_t* man, pkeq_t* pa, pkeq_t* pb)
{
  pk_init_from_manager(man,AP_FUNID_IS_EQ);
  pkeq_canonicalize(man,pa);
  pkeq_canonicalize(man,pb);

  man->result.flag_exact = man->result.flag_best = true;

  if (pa->C){
    if (pb->C){
      if (pa->nbeq != pb->nbeq || pa->nbline != pb->nbline){
	return false;
      }
      else {
	size_t i,j;

	pk_matrix_t* mata = pa->C;
	pk_matrix_t* matb = pb->C;
	bool res = true;
	for (i=0; i<mata->nbrows; i++){
	  for (j=0; j<matb->nbcolumns; j++){
	    if (numint_cmp(mata->p[i][j],matb->p[i][j])!=0){
	      res = false;
	      goto _pkeq_is_eq_exit;
	    }
	  }
	}
      _pkeq_is_eq_exit:
	return res;
      }
    }
    else {
      return false;
    }
  }
  else {
    return pb->C ? false : true;
  }
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

pkeq_t* pkeq_meet(ap_manager_t* man, bool destructive, pkeq_t* polya, pkeq_t* polyb)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET);
  pk->funopt->algorithm = 1;
  po = pk_meet(man,destructive,polya,polyb);
  return po;
}

pkeq_t* pkeq_meet_array(ap_manager_t* man, pkeq_t** po, size_t size)
{
  pkeq_t* poly;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET);
  pk->funopt->algorithm = 1;
  poly = pk_meet_array(man,po,size);
  return poly;
}

pkeq_t* pkeq_meet_lincons_array(ap_manager_t* man, bool destructive, pkeq_t* pa, ap_lincons0_array_t* array)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY);
  pk->funopt->algorithm = 1;
  po = pk_meet_lincons_array(man,destructive,pa,array);
  equality_reduce(man,po);
  assert(equality_check(pk,po));
  return po;
}

pkeq_t* pkeq_meet_tcons_array(ap_manager_t* man, bool destructive, pkeq_t* pa, ap_tcons0_array_t* array)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MEET_TCONS_ARRAY);
  pk->funopt->algorithm = 1;
  po = pk_meet_tcons_array(man,destructive,pa,array);
  equality_reduce(man,po);
  assert(equality_check(pk,po));
  return po;
}

pkeq_t* pkeq_join(ap_manager_t* man, bool destructive, pkeq_t* polya, pkeq_t* polyb)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_JOIN);
  pk->funopt->algorithm = 1;
  po = pk_join(man,destructive,polya,polyb);
  equality_reduce(man,po);
  return po;
}

pkeq_t* pkeq_join_array(ap_manager_t* man, pkeq_t** po, size_t size)
{
  pkeq_t* poly;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_JOIN);
  pk->funopt->algorithm = 1;
  poly = pk_join_array(man,po,size);
  equality_reduce(man,poly);
  return poly;
}

pkeq_t* pkeq_add_ray_array(ap_manager_t* man, bool destructive, pkeq_t* pa, ap_generator0_array_t* array)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY);
  pk->funopt->algorithm = 1;
  po = pk_add_ray_array(man,destructive,pa,array);
  equality_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

/* Assignement/Substitution by a linear expression */
static
pkeq_t* equality_asssub_linexpr(bool assign,
				ap_manager_t* man,
				bool destructive,
				pkeq_t* pa,
				ap_dim_t dim, ap_linexpr0_t* linexpr)
{
  pkeq_t* po;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  pk_internal_realloc_lazy(pk,pa->intdim+pa->realdim);
  
  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->intdim,pa->realdim);
  }
  /* Choose the right technique */
  switch (linexpr->cst.discr){
  case AP_COEFF_SCALAR:
    {
      po = poly_asssub_linexpr_det(assign,man,destructive,pa,dim,linexpr);
      poly_chernikova(man,po,"of the result");
      if (pk->exn) goto _equality_asssub_linexpr_error;
      equality_reduce(man,po);
      if (pk->exn) goto _equality_asssub_linexpr_error;
      /* Is the result exact or best ? */
      if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
	man->result.flag_best = man->result.flag_exact = 
	  (dim < pa->intdim || !ap_linexpr0_is_real(linexpr, pa->intdim)) ?
	  false :
	  true;
      }
      else {
	man->result.flag_best = man->result.flag_exact = (pa->intdim==0);
      }
    }
    break;
  case AP_COEFF_INTERVAL:
    po = pkeq_forget_array(man,destructive,pa,&dim,1,false);
    break;
  default:
    abort();
  }
  return po;
 _equality_asssub_linexpr_error:
  pk->exn = AP_EXC_NONE;
  poly_set_top(pk,po);
  man->result.flag_best = man->result.flag_exact = false;
  return po;
}

/* Assignement/Substitution by an array of linear expressions */
static
pkeq_t* equality_asssub_linexpr_array(bool assign, 
				      ap_manager_t* man,
				      bool destructive,
				      pkeq_t* pa,
				      ap_dim_t* tdim,
				      ap_linexpr0_t** texpr,
				      size_t size)
{
  size_t i;
  pkeq_t* po;
  ap_dim_t* tdimp;
  ap_linexpr0_t** texprp;
  size_t sizep;
  ap_dim_t* tdimforget;
  size_t sizeforget;

  pk_internal_t* pk = (pk_internal_t*)man->internal;

  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pk_bottom(man,pa->intdim,pa->realdim);
  }
  /* Choose the right technique */
  tdimp = malloc(size*sizeof(ap_dim_t));
  texprp = malloc(size*sizeof(ap_linexpr0_t*));
  sizep = 0;
  tdimforget = malloc(size*sizeof(ap_dim_t));
  sizeforget = 0;
  for (i=0; i<size; i++){
    if (texpr[i]->cst.discr==AP_COEFF_SCALAR){
      tdimp[sizep] = tdim[i];
      texprp[sizep] = texpr[i];
      sizep++;
    } else {
      tdimforget[sizeforget] = tdim[i];
      sizeforget++;
    }
  }
  if (sizep>0){
    po = poly_asssub_linexpr_array_det(assign,man,destructive,pa,tdimp,texprp,sizep);
    poly_chernikova(man,po,"of the result");
    if (pk->exn) goto _equality_asssub_linexpr_array_error;
    equality_reduce(man,po);
    if (pk->exn) goto _equality_asssub_linexpr_array_error;
    if (sizeforget){
      po = pkeq_forget_array(man,true,po,tdimforget,sizeforget,false);
    }
  }
  else {
    po = pkeq_forget_array(man,destructive,pa,tdimforget,sizeforget,false);
  }
  /* Is the result exact or best ? */
  if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
    man->result.flag_best = true;
    for (i=0;i<size;i++){
      if (tdim[i] < pa->intdim || !ap_linexpr0_is_real(texpr[i], pa->intdim)){
	man->result.flag_best = false;
	break;
      }
    }
    man->result.flag_exact = man->result.flag_best;
  }
  else {
    man->result.flag_best = man->result.flag_exact = (pa->intdim==0);
  }
  free(tdimp);
  free(texprp); 
  free(tdimforget);
  return po;
 _equality_asssub_linexpr_array_error:
  free(tdimp);
  free(texprp); 
  free(tdimforget);
  pk->exn = AP_EXC_NONE;
  poly_set_top(pk,po);
  man->result.flag_best = man->result.flag_exact = false;
  return po;
}

pkeq_t* pkeq_assign_linexpr_array(ap_manager_t* man,
				      bool destructive, pkeq_t* pa,
				      ap_dim_t* tdim, ap_linexpr0_t** texpr,
				      size_t size,
				      pkeq_t* pb)
{
  pk_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY);
  pkeq_t* po;
  po = 
    size==1 ?
    equality_asssub_linexpr(true,
			    man,destructive,pa,tdim[0],texpr[0])
    :
    equality_asssub_linexpr_array(true,
				  man,destructive,pa,tdim,texpr,size);
  if (pb!=NULL){
    po = pkeq_meet(man,true,po,pb);
  }
  return po;
}

pkeq_t* pkeq_substitute_linexpr_array(ap_manager_t* man,
					  bool destructive, pkeq_t* pa,
					  ap_dim_t* tdim, ap_linexpr0_t** texpr,
					  size_t size,
					  pkeq_t* pb)
{
  pk_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY);
  pkeq_t* po;
  po = 
    size==1 ?
    equality_asssub_linexpr(false,
			    man,destructive,pa,tdim[0],texpr[0])
    :
    equality_asssub_linexpr_array(false,
				  man,destructive,pa,tdim,texpr,size);
  if (pb!=NULL){
    po = pkeq_meet(man,true,po,pb);
  }
  return po;
}
pkeq_t* pkeq_assign_texpr_array(ap_manager_t* man,
				bool destructive, pkeq_t* a,
				ap_dim_t* tdim,
				ap_texpr0_t** texpr,
				size_t size,
				pkeq_t* dest)
{
  return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
pkeq_t* pkeq_substitute_texpr_array(ap_manager_t* man,
				    bool destructive, pkeq_t* a,
				    ap_dim_t* tdim,
				    ap_texpr0_t** texpr,
				    size_t size,
				    pkeq_t* dest)
{
  return ap_generic_substitute_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}


/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

pkeq_t* pkeq_forget_array(ap_manager_t* man,
			      bool destructive, pkeq_t* a,
			      ap_dim_t* tdim, size_t size,
			      bool project)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_FORGET_ARRAY);
  pk->funopt->algorithm = 1;
  po = pk_forget_array(man,destructive,a,tdim,size,project);
  equality_reduce(man,po);
  return po;
}


/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

pkeq_t* pkeq_remove_dimensions(ap_manager_t* man,
			    bool destructive, pkeq_t* a,
			    ap_dimchange_t* dimchange)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS);
  pk->funopt->algorithm = 1;
  po = pk_remove_dimensions(man,destructive,a,dimchange);
  equality_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */
pkeq_t* pkeq_expand(ap_manager_t* man,
			bool destructive, pkeq_t* a,
			ap_dim_t dim,
			size_t n)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_EXPAND);
  pk->funopt->algorithm = 1;
  po = pk_expand(man,destructive,a,dim,n);
  equality_reduce(man,po);
  return po;
}
pkeq_t* pkeq_fold(ap_manager_t* man,
		      bool destructive, pkeq_t* a,
		      ap_dim_t* tdim, size_t size)
{
  pkeq_t* po;
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_FOLD);
  pk->funopt->algorithm = 1;
  po = pk_fold(man,destructive,a,tdim,size);
  equality_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

pkeq_t* pkeq_widening(ap_manager_t* man,
			  pkeq_t* a1, pkeq_t* a2)
{
  return pk_copy(man,a2);
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

pkeq_t* pkeq_closure(ap_manager_t* man, bool destructive, pkeq_t* a)
{
  return destructive ? a : pk_copy(man,a);
}

pk_internal_t* pkeq_manager_get_internal(ap_manager_t* man)
{
  return pk_manager_get_internal(man);
}


ap_manager_t* pkeq_manager_alloc(void)
{
  ap_manager_t* man;
  void** funptr;

  man = pk_manager_alloc(false);
  man->library = "polka, equalities mode";
  funptr = man->funptr;
  // funptr[AP_FUNID_COPY] = &poly_copy;
  // funptr[AP_FUNID_FREE] = &poly_free;
  funptr[AP_FUNID_ASIZE] = &pkeq_size;
  // funptr[AP_FUNID_MINIMIZE] = &poly_minimize;
  // funptr[AP_FUNID_CANONICALIZE] = &poly_canonicalize;
  // funptr[AP_FUNID_HASH] = &poly_hash;
  funptr[AP_FUNID_APPROXIMATE] = &pkeq_approximate;
  // funptr[AP_FUNID_FPRINT] = &poly_fprint;
  // funptr[AP_FUNID_FPRINTDIFF] = &poly_fprintdiff;
  // funptr[AP_FUNID_FDUMP] = &poly_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &pkeq_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &pkeq_deserialize_raw;
  // funptr[AP_FUNID_BOTTOM] = &poly_bottom;
  // funptr[AP_FUNID_TOP] = &poly_top;
  funptr[AP_FUNID_OF_BOX] = &pkeq_of_box;
  // funptr[AP_FUNID_DIMENSION] = &poly_dimension;
  // funptr[AP_FUNID_IS_BOTTOM] = &poly_is_bottom;
  // funptr[AP_FUNID_IS_TOP] = &poly_is_top;
  // funptr[AP_FUNID_IS_LEQ] = &poly_is_leq;
  funptr[AP_FUNID_IS_EQ] = &pkeq_is_eq;
  // funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &poly_is_dimension_unconstrained;
  // funptr[AP_FUNID_SAT_INTERVAL] = &poly_sat_interval;
  // funptr[AP_FUNID_SAT_LINCONS] = &poly_sat_lincons;
  // funptr[AP_FUNID_SAT_TCONS] = &poly_sat_tcons;
  // funptr[AP_FUNID_BOUND_DIMENSION] = &poly_bound_dimension;
  // funptr[AP_FUNID_BOUND_LINEXPR] = &poly_bound_linexpr;
  // funptr[AP_FUNID_BOUND_TEXPR] = &poly_bound_texpr;
  // funptr[AP_FUNID_TO_BOX] = &poly_to_box;
  // funptr[AP_FUNID_TO_LINCONS_ARRAY] = &poly_to_lincons_array;
  // funptr[AP_FUNID_TO_TCONS_ARRAY] = &poly_to_tcons_array;
  // funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &poly_to_generator_array;
  funptr[AP_FUNID_MEET] = &pkeq_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &pkeq_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &pkeq_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &pkeq_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &pkeq_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &pkeq_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &pkeq_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &pkeq_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &pkeq_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &pkeq_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &pkeq_substitute_texpr_array;
  //funptr[AP_FUNID_ADD_DIMENSIONS] = &poly_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &pkeq_remove_dimensions;
  //funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &poly_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &pkeq_forget_array;
  funptr[AP_FUNID_EXPAND] = &pkeq_expand;
  funptr[AP_FUNID_FOLD] = &pkeq_fold;
  funptr[AP_FUNID_WIDENING] = &pkeq_widening;
  funptr[AP_FUNID_CLOSURE] = &pkeq_closure;

  return man;
}
