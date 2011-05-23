/* ********************************************************************** */
/* pkXXX_equality.c: linear equalities on top of NewPolka polyhedra */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkeqXXX.h"
#include "pkXXX_internal.h"
#include "ap_generic.h"
#include "ap_linexpr0.h"
#include "ap_linexprXXX.h"

#define _MARK_XXX_

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

/* Return the abstract size of a set of equalities, which is the number of
   equalities times the dimension */
size_t pkeqXXX_size(ap_manager_t* man, pkeqXXX_t* po)
{
  size_t s;

  assert (po->C ? (po->C->nbrows >=1) : true);

  s = (po->C) ? (po->C->nbrows-1) : 0;
  return s*(po->dim.intd + po->dim.reald);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

static
bool pkeqXXX_check(pkXXX_internal_t* pk, pkeqXXX_t* po)
{
  if (!pkXXX_check(pk,po)) return false;
  if ((po->C && !po->F) || (!po->C && po->F)){
     fprintf(stderr,"pkeqXXX_check: po->C && !po->F || !po->C && po->F: not in canonical form: \n");
     return false;
  }
  if (po->C && po->C->nbrows != po->nbeq+1){
      fprintf(stderr,"pkeqXXX_check: po->C->nbrows != po->nbeq+1\n");
     return false;
  }
  if (po->F && po->F->nbrows != po->nbline+1){
      fprintf(stderr,"pkeqXXX_check: po->F->nbrows != po->nbline+1\n");
     return false;
  }
  return true;
}


static
void matrixXXX_reduce(matrixXXX_t* mat)
{
  if (mat->nbrows + 4 <= mat->_maxrows){
    matrixXXX_minimize(mat);
  }
}

static
void pkeqXXX_reduce(ap_manager_t* man, pkeqXXX_t* po)
{
  assert((!po->C && !po->F) || (po->C && po->F));
  if (po->C && po->F){
    if (po->C->nbrows > po->nbeq + 1){
      pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

      po->C->nbrows = po->nbeq + 1;
      matrixXXX_fill_constraint_top(pk,po->C,po->nbeq);
      matrixXXX_reduce(po->C);
      matrixXXX_free(po->F); po->F = NULL;
      if (po->satC){
	satmat_free(po->satC);
	po->satC = NULL;
      }
      if (po->satF){
	satmat_free(po->satF);
	po->satF = NULL;
      }
      pkXXX_chernikova(man,po,"of result");
      if (pk->exn){
	pk->exn = AP_EXC_NONE;
	pkXXX_set_top(pk,po);
      }
    }
  }
}
void pkeqXXX_canonicalize(ap_manager_t* man, pkeqXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_CANONICALIZE);
  pk->funopt->algorithm = 1;
  return pkXXX_canonicalize(man,po);
}

void pkeqXXX_approximate(ap_manager_t* man, pkeqXXX_t* po, int algorithm)
{
  return;
}

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t pkeqXXX_serialize_raw(ap_manager_t* man, pkeqXXX_t* a)
{
  ap_membuf_t membuf;
  pkXXX_init_from_manager(man,AP_FUNID_SERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,NULL);
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}
pkeqXXX_t* pkeqXXX_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  pkXXX_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW);
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
pkeqXXX_t* pkeqXXX_of_box(ap_manager_t* man,
			  ap_dimension_t dimen,
			  ap_linexpr0_t box)
{
  size_t i;
  size_t row,dim;
  pkeqXXX_t* po;
  bool ok,exact;

  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_OF_BOX);
  pkXXX_internal_realloc_lazy(pk,dimen.intd+dimen.reald);

  po = pkXXX_alloc(dimen);
  po->status = pk_status_conseps;

  dim = dimen.intd + dimen.reald;
  po->C = matrixXXX_alloc(pk->dec-1 + dim, pk->dec + dim, false);

  /* constraints */
  exact = true;
  row = 0;
  ok = false;
  for (i=0; i<dim; i++){
    ap_coeff_t coeff;
    ap_linexpr0_coeffref(coeff,box, i);
    exact = eitvXXX_set_ap_coeff(pk->eitvXXX,coeff,pk->num) && exact;
    if (eitvXXX_is_point(pk->eitvXXX)){
      ok = vectorXXX_set_dim_bound(
	  pk,po->C->p[row],
	  (ap_dim_t)i, boundXXX_numref(pk->eitvXXX->itv->sup), 0, dimen, true);
      if (!ok){
	matrixXXX_free(po->C);
	po->C = NULL;
	return po;
      }
      row++;
    }
  }
  matrixXXX_fill_constraint_top(pk,po->C,row);
  po->C->nbrows = pk->dec - 1 + row;
  matrixXXX_reduce(po->C);
  pkXXX_canonicalize(man,po);
  man->result.flag_exact = exact;
  man->result.flag_best = true;
  return po;
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool pkeqXXX_is_eq(ap_manager_t* man, pkeqXXX_t* pa, pkeqXXX_t* pb)
{
  pkXXX_init_from_manager(man,AP_FUNID_IS_EQ);
  pkeqXXX_canonicalize(man,pa);
  pkeqXXX_canonicalize(man,pb);

  man->result.flag_exact = man->result.flag_best = true;

  if (pa->C){
    if (pb->C){
      if (pa->nbeq != pb->nbeq || pa->nbline != pb->nbline){
	return false;
      }
      else {
	size_t i,j;

	matrixXXX_t* mata = pa->C;
	matrixXXX_t* matb = pb->C;
	bool res = true;
	for (i=0; i<mata->nbrows; i++){
	  for (j=0; j<matb->nbcolumns; j++){
	    if (numintXXX_cmp(mata->p[i][j],matb->p[i][j])!=0){
	      res = false;
	      goto _pkeqXXX_is_eq_exit;
	    }
	  }
	}
      _pkeqXXX_is_eq_exit:
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

pkeqXXX_t* pkeqXXX_meet(ap_manager_t* man, bool destructive, pkeqXXX_t* polya, pkeqXXX_t* polyb)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET);
  pk->funopt->algorithm = 1;
  po = pkXXX_meet(man,destructive,polya,polyb);
  return po;
}

pkeqXXX_t* pkeqXXX_meet_array(ap_manager_t* man, pkeqXXX_t** po, size_t size)
{
  pkeqXXX_t* poly;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET);
  pk->funopt->algorithm = 1;
  poly = pkXXX_meet_array(man,po,size);
  return poly;
}

pkeqXXX_t* pkeqXXX_meet_lincons_array(ap_manager_t* man, bool destructive, pkeqXXX_t* pa, ap_lincons0_array_t array)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY);
  pk->funopt->algorithm = 1;
  po = pkXXX_meet_lincons_array(man,destructive,pa,array);
  pkeqXXX_reduce(man,po);
  assert(pkeqXXX_check(pk,po));
  return po;
}

pkeqXXX_t* pkeqXXX_meet_tcons_array(ap_manager_t* man, bool destructive, pkeqXXX_t* pa, ap_tcons0_array_t* array)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MEET_TCONS_ARRAY);
  pk->funopt->algorithm = 1;
  po = pkXXX_meet_tcons_array(man,destructive,pa,array);
  pkeqXXX_reduce(man,po);
  assert(pkeqXXX_check(pk,po));
  return po;
}

pkeqXXX_t* pkeqXXX_join(ap_manager_t* man, bool destructive, pkeqXXX_t* polya, pkeqXXX_t* polyb)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_JOIN);
  pk->funopt->algorithm = 1;
  po = pkXXX_join(man,destructive,polya,polyb);
  pkeqXXX_reduce(man,po);
  return po;
}

pkeqXXX_t* pkeqXXX_join_array(ap_manager_t* man, pkeqXXX_t** po, size_t size)
{
  pkeqXXX_t* poly;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_JOIN);
  pk->funopt->algorithm = 1;
  poly = pkXXX_join_array(man,po,size);
  pkeqXXX_reduce(man,poly);
  return poly;
}

pkeqXXX_t* pkeqXXX_add_ray_array(ap_manager_t* man, bool destructive, pkeqXXX_t* pa, ap_lingen0_array_t array)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY);
  pk->funopt->algorithm = 1;
  po = pkXXX_add_ray_array(man,destructive,pa,array);
  pkeqXXX_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

/* Assignement/Substitution by a linear expression */
static
pkeqXXX_t* pkeqXXX_asssub_linexpr(bool assign,
				  ap_manager_t* man,
				  bool destructive,
				  pkeqXXX_t* pa,
				  ap_dim_t dim, ap_linexpr0_t linexpr0)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pkXXX_internal_realloc_lazy(pk,pa->dim.intd+pa->dim.reald);

  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pkXXX_bottom(man,pa->dim);
  }
  /* Choose the right technique */
  if (ap_linexpr0_is_linear(linexpr0)){
#ifdef _MARK_MPQ_
    if (linexpr0->discr==AP_SCALAR_MPQ)
      po = pkXXX_asssub_linexprXXX_det(assign,man,destructive,pa,dim,linexpr0->linexpr.MPQ);
    else
#endif
      {
	bool exact = ap_linexprXXX_set_linexpr0(pk->ap_linexprXXX,linexpr0,pk->num);
	po = pkXXX_asssub_linexprXXX_det(assign,man,destructive,pa,dim,pk->ap_linexprXXX);
      }
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn) goto _pkeqXXX_asssub_linexpr_error;
    pkeqXXX_reduce(man,po);
    if (pk->exn) goto _pkeqXXX_asssub_linexpr_error;
    /* Is the result exact or best ? */
    if (pk->funopt->flag_best_wanted || pk->funopt->flag_exact_wanted){
      man->result.flag_best = man->result.flag_exact =
	(dim < pa->dim.intd || !ap_linexpr0_is_real(linexpr0, pa->dim.intd)) ?
	false :
	true;
    }
    else {
      man->result.flag_best = man->result.flag_exact = (pa->dim.intd==0);
    }
  }
  else {
    po = pkeqXXX_forget_array(man,destructive,pa,&dim,1,false);
  }
  return po;
 _pkeqXXX_asssub_linexpr_error:
  pk->exn = AP_EXC_NONE;
  pkXXX_set_top(pk,po);
  man->result.flag_best = man->result.flag_exact = false;
  return po;
}

/* Assignement/Substitution by an array of linear expressions */
static
pkeqXXX_t* pkeqXXX_asssub_linexpr_array(bool assign,
					ap_manager_t* man,
					bool destructive,
					pkeqXXX_t* pa,
					ap_dim_t* tdim,
					ap_linexpr0_array_t array)
{
  size_t i;
  pkeqXXX_t* po;
  ap_dim_t* tdimp;
  ap_linexprXXX_array_t arrayXXX;
  size_t sizep;
  ap_dim_t* tdimforget;
  size_t sizeforget;

  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;

  /* Return empty if empty */
  if (!pa->C && !pa->F){
    man->result.flag_best = man->result.flag_exact = true;
    return destructive ? pa : pkXXX_bottom(man,pa->dim);
  }
  /* Choose the right technique */
  size_t size = ap_linexpr0_array_size(array);
  tdimp = malloc(size*sizeof(ap_dim_t));
  ap_linexprXXX_array_init(arrayXXX,size);
  sizep = 0;
  tdimforget = malloc(size*sizeof(ap_dim_t));
  sizeforget = 0;
  for (i=0; i<size; i++){
    ap_linexpr0_t linexpr0ref;
    ap_linexpr0_array_ref_index(linexpr0ref,array,i);
    if (ap_linexpr0_is_linear(linexpr0ref)){
      tdimp[sizep] = tdim[i];
      ap_linexprXXX_set_linexpr0(arrayXXX->p[sizep],linexpr0ref,pk->num);
      sizep++;
    } else {
      tdimforget[sizeforget] = tdim[i];
      sizeforget++;
    }
  }
  if (sizep>0){
    ap_linexprXXX_array_resize(arrayXXX,sizep);
    po = pkXXX_asssub_linexprXXX_array_det(assign,man,destructive,pa,tdimp,arrayXXX);
    pkXXX_chernikova(man,po,"of the result");
    if (pk->exn) goto _pkeqXXX_asssub_linexpr_array_error;
    pkeqXXX_reduce(man,po);
    if (pk->exn) goto _pkeqXXX_asssub_linexpr_array_error;
    if (sizeforget>0){
      po = pkeqXXX_forget_array(man,true,po,tdimforget,sizeforget,false);
    }
  }
  else {
    po = pkeqXXX_forget_array(man,destructive,pa,tdimforget,sizeforget,false);
  }
  free(tdimp);
  ap_linexprXXX_array_clear(arrayXXX);
  free(tdimforget);
  /* Is the result exact or best ? */
  man->result.flag_best = true;
  man->result.flag_exact = (pa->dim.intd==0) && sizeforget==0;
  return po;
 _pkeqXXX_asssub_linexpr_array_error:
  free(tdimp);
  ap_linexprXXX_array_clear(arrayXXX);
  free(tdimforget);
  pk->exn = AP_EXC_NONE;
  pkXXX_set_top(pk,po);
  man->result.flag_best = man->result.flag_exact = false;
  return po;
}

pkeqXXX_t* pkeqXXX_assign_linexpr_array(ap_manager_t* man,
					bool destructive, pkeqXXX_t* pa,
					ap_dim_t* tdim, ap_linexpr0_array_t array,
					pkeqXXX_t* pb)
{
  pkXXX_init_from_manager(man,AP_FUNID_ASSIGN_LINEXPR_ARRAY);
  pkeqXXX_t* po;
  size_t size = ap_linexpr0_array_size(array);
  if (size==1){
    ap_linexpr0_t linexpr0ref;
    ap_linexpr0_array_ref_index(linexpr0ref,array,0);
    po = pkeqXXX_asssub_linexpr(true,
				man,destructive,pa,tdim[0],linexpr0ref);
  } else {
    po = pkeqXXX_asssub_linexpr_array(true,
				      man,destructive,pa,tdim,array);
  }
  if (pb!=NULL){
    po = pkeqXXX_meet(man,true,po,pb);
  }
  return po;
}

pkeqXXX_t* pkeqXXX_substitute_linexpr_array(ap_manager_t* man,
					    bool destructive, pkeqXXX_t* pa,
					    ap_dim_t* tdim, ap_linexpr0_array_t array,
					    pkeqXXX_t* pb)

{
  pkXXX_init_from_manager(man,AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY);
  pkeqXXX_t* po;
  size_t size = ap_linexpr0_array_size(array);
  if (size==1){
    ap_linexpr0_t linexpr0ref;
    ap_linexpr0_array_ref_index(linexpr0ref,array,0);
    po = pkeqXXX_asssub_linexpr(false,
				man,destructive,pa,tdim[0],linexpr0ref);
  } else {
    po = pkeqXXX_asssub_linexpr_array(false,
				      man,destructive,pa,tdim,array);
  }
  if (pb!=NULL){
    po = pkeqXXX_meet(man,true,po,pb);
  }
  return po;
}
pkeqXXX_t* pkeqXXX_assign_texpr_array(ap_manager_t* man,
				      bool destructive, pkeqXXX_t* a,
				      ap_dim_t* tdim,
				      ap_texpr0_array_t* array,
				      pkeqXXX_t* dest)
{
  return ap_generic_asssub_texpr_array(true,man,destructive,a,tdim,array,dest);
}
pkeqXXX_t* pkeqXXX_substitute_texpr_array(ap_manager_t* man,
					  bool destructive, pkeqXXX_t* a,
					  ap_dim_t* tdim,
					  ap_texpr0_array_t* array,
					  pkeqXXX_t* dest)
{
  return ap_generic_asssub_texpr_array(false,man,destructive,a,tdim,array,dest);
}


/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

pkeqXXX_t* pkeqXXX_forget_array(ap_manager_t* man,
			      bool destructive, pkeqXXX_t* a,
			      ap_dim_t* tdim, size_t size,
			      bool project)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_FORGET_ARRAY);
  pk->funopt->algorithm = 1;
  po = pkXXX_forget_array(man,destructive,a,tdim,size,project);
  pkeqXXX_reduce(man,po);
  return po;
}


/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

pkeqXXX_t* pkeqXXX_remove_dimensions(ap_manager_t* man,
			    bool destructive, pkeqXXX_t* a,
			    ap_dimchange_t* dimchange)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS);
  pk->funopt->algorithm = 1;
  po = pkXXX_remove_dimensions(man,destructive,a,dimchange);
  pkeqXXX_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */
pkeqXXX_t* pkeqXXX_expand(ap_manager_t* man,
			bool destructive, pkeqXXX_t* a,
			ap_dim_t dim,
			size_t n)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_EXPAND);
  pk->funopt->algorithm = 1;
  po = pkXXX_expand(man,destructive,a,dim,n);
  pkeqXXX_reduce(man,po);
  return po;
}
pkeqXXX_t* pkeqXXX_fold(ap_manager_t* man,
		      bool destructive, pkeqXXX_t* a,
		      ap_dim_t* tdim, size_t size)
{
  pkeqXXX_t* po;
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_FOLD);
  pk->funopt->algorithm = 1;
  po = pkXXX_fold(man,destructive,a,tdim,size);
  pkeqXXX_reduce(man,po);
  return po;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

pkeqXXX_t* pkeqXXX_widening(ap_manager_t* man,
			    pkeqXXX_t* a1, pkeqXXX_t* a2)
{
  return pkXXX_copy(man,a2);
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

pkeqXXX_t* pkeqXXX_closure(ap_manager_t* man, bool destructive, pkeqXXX_t* a)
{
  return destructive ? a : pkXXX_copy(man,a);
}

pkXXX_internal_t* pkeqXXX_manager_get_internal(ap_manager_t* man)
{
  return pkXXX_manager_get_internal(man);
}


ap_manager_t* pkeqXXX_manager_alloc(void)
{
  ap_manager_t* man;
  void** funptr;

  man = pkXXX_manager_alloc(false);
  man->library = "polka, equalities mode, numXXX";
  funptr = man->funptr;
  // funptr[AP_FUNID_COPY] = &pkXXX_copy;
  // funptr[AP_FUNID_FREE] = &pkXXX_free;
  funptr[AP_FUNID_ASIZE] = &pkeqXXX_size;
  // funptr[AP_FUNID_MINIMIZE] = &pkXXX_minimize;
  // funptr[AP_FUNID_CANONICALIZE] = &pkXXX_canonicalize;
  // funptr[AP_FUNID_HASH] = &pkXXX_hash;
  funptr[AP_FUNID_APPROXIMATE] = &pkeqXXX_approximate;
  // funptr[AP_FUNID_FPRINT] = &pkXXX_fprint;
  // funptr[AP_FUNID_FPRINTDIFF] = &pkXXX_fprintdiff;
  // funptr[AP_FUNID_FDUMP] = &pkXXX_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &pkeqXXX_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &pkeqXXX_deserialize_raw;
  // funptr[AP_FUNID_BOTTOM] = &pkXXX_bottom;
  // funptr[AP_FUNID_TOP] = &pkXXX_top;
  funptr[AP_FUNID_OF_BOX] = &pkeqXXX_of_box;
  // funptr[AP_FUNID_DIMENSION] = &pkXXX_dimension;
  // funptr[AP_FUNID_IS_BOTTOM] = &pkXXX_is_bottom;
  // funptr[AP_FUNID_IS_TOP] = &pkXXX_is_top;
  // funptr[AP_FUNID_IS_LEQ] = &pkXXX_is_leq;
  funptr[AP_FUNID_IS_EQ] = &pkeqXXX_is_eq;
  // funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &pkXXX_is_dimension_unconstrained;
  // funptr[AP_FUNID_SAT_INTERVAL] = &pkXXX_sat_interval;
  // funptr[AP_FUNID_SAT_LINCONS] = &pkXXX_sat_lincons;
  // funptr[AP_FUNID_SAT_TCONS] = &pkXXX_sat_tcons;
  // funptr[AP_FUNID_BOUND_DIMENSION] = &pkXXX_bound_dimension;
  // funptr[AP_FUNID_BOUND_LINEXPR] = &pkXXX_bound_linexpr;
  // funptr[AP_FUNID_BOUND_TEXPR] = &pkXXX_bound_texpr;
  // funptr[AP_FUNID_TO_BOX] = &pkXXX_to_box;
  // funptr[AP_FUNID_TO_LINCONS_ARRAY] = &pkXXX_to_lincons_array;
  // funptr[AP_FUNID_TO_TCONS_ARRAY] = &pkXXX_to_tcons_array;
  // funptr[AP_FUNID_TO_LINGEN_ARRAY] = &pkXXX_to_lingen_array;
  funptr[AP_FUNID_MEET] = &pkeqXXX_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &pkeqXXX_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &pkeqXXX_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &pkeqXXX_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &pkeqXXX_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &pkeqXXX_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &pkeqXXX_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &pkeqXXX_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &pkeqXXX_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &pkeqXXX_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &pkeqXXX_substitute_texpr_array;
  //funptr[AP_FUNID_ADD_DIMENSIONS] = &pkXXX_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &pkeqXXX_remove_dimensions;
  //funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &pkXXX_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &pkeqXXX_forget_array;
  funptr[AP_FUNID_EXPAND] = &pkeqXXX_expand;
  funptr[AP_FUNID_FOLD] = &pkeqXXX_fold;
  funptr[AP_FUNID_WIDENING] = &pkeqXXX_widening;
  funptr[AP_FUNID_CLOSURE] = &pkeqXXX_closure;

  return man;
}
