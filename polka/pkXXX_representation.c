/* ********************************************************************** */
/* pkXXX_representation.c: General management of polyhedra  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"

/* ********************************************************************** */
/* Conversions */
/* ********************************************************************** */

pkXXX_t* pkXXX_of_abstract0(ap_abstract0_t* abstract)
{
  ap_manager_t* man = abstract->man;
  if (strncmp(man->library,"polka",5)!=0){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_UNKNOWN,
			       "pkXXX_of_abstract0: attempt to extract a NewPolka polyhedra from an abstract value which is not a wrapper around a NewPolka polyhedra");
    return NULL;
  }
  return (pkXXX_t*)abstract->value;
}

ap_abstract0_t* pkXXX_to_abstract0(ap_manager_t* man, pkXXX_t* poly)
{
  if (strncmp(man->library,"polka",5)!=0){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_UNKNOWN,
			       "pkXXX_to_abstract0: attempt to extract a NewPolka polyhedra from an abstract value which is not a wrapper around a NewPolka polyhedra");
    return ap_abstract0_top(man,poly->dim);
  }
  else {
    ap_abstract0_t* res = malloc(sizeof(ap_abstract0_t));
    res->man = man;
    res->value = poly;
    return res;
  }
}

/* ********************************************************************** */
/* I. Memory */
/* ********************************************************************** */

/* This internal function allocates a polyhedron and fills its records
   with null values. */

pkXXX_t* pkXXX_alloc(ap_dimension_t dim)
{
  pkXXX_t* po = (pkXXX_t*)malloc(sizeof(pkXXX_t));
  po->C = NULL;
  po->F = NULL;
  po->satC = NULL;
  po->satF = NULL;
  po->dim.intd = dim.intd;
  po->dim.reald = dim.reald;
  po->nbeq = 0;
  po->nbline = 0;
  po->status = 0;
  return po;
}

/* Clearing a polyhedron */
void pkXXX_clear(pkXXX_t* po)
{
  if (po->C) matrixXXX_free(po->C);
  if (po->F) matrixXXX_free(po->F);
  if (po->satC) satmat_free(po->satC);
  if (po->satF) satmat_free(po->satF);
  po->C = NULL;
  po->F = NULL;
  po->satC = NULL;
  po->satF = NULL;
  po->status = 0;
  po->nbeq = 0;
  po->nbline = 0;
}

/* Assignement with GMP semantics */
void pkXXX_set(pkXXX_t* pa, pkXXX_t* pb)
{
  if (pa!=pb){
    pkXXX_clear(pa);
    pa->dim.intd = pb->dim.intd;
    pa->dim.reald = pb->dim.reald;
    pa->C = pb->C ? matrixXXX_copy(pb->C) : NULL;
    pa->F = pb->F ? matrixXXX_copy(pb->F) : NULL;
    pa->satC = pb->satC ? satmat_copy(pb->satC) : NULL;
    pa->satF = pb->satF ? satmat_copy(pb->satF) : NULL;
    pa->status = pb->status;
    pa->nbeq = pb->nbeq;
    pa->nbline = pb->nbline;
    pa->status = pb->status;
  }
}

/* Duplicate (recursively) a polyhedron. */
pkXXX_t* pkXXX_copy(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_t* npo = pkXXX_alloc(po->dim);
  npo->C = po->C ? matrixXXX_copy(po->C) : 0;
  npo->F = po->F ? matrixXXX_copy(po->F) : 0;
  npo->satC = po->satC ? satmat_copy(po->satC) : 0;
  npo->satF = po->satF ? satmat_copy(po->satF) : 0;
  npo->nbeq = po->nbeq;
  npo->nbline = po->nbline;
  npo->status = po->status;
  return npo;
}

/* Finalization function for polyhedra, which frees
   recursively the members of the structure. */
void pkXXX_free(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_clear(po);
  free(po);
}

/* Return the abstract size of a polyhedron, which is the number of
   coefficients of its current representation, possibly redundant. */
size_t pkXXX_size(ap_manager_t* man, pkXXX_t* po)
{
  size_t s1,s2;

  s1 = (po->C) ? po->C->nbrows : 0;
  s2 = (po->F) ? po->F->nbrows : 0;
  return (s1+s2)*(po->dim.intd + po->dim.reald);
}

/* ********************************************************************** */
/* II. Control of internal representation */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Dual representation */
/* ====================================================================== */

/* Minimization function, in the sense of minimized dual representation
   This function minimizes if not already done the given polyhedron.

   Transmit exception
*/

void pkXXX_chernikova(ap_manager_t* man,
		     pkXXX_t* po,
		     char* msg)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  if ((po->C && po->F) || (!po->C && !po->F)){
    return;
  }
  else {
    if (po->C){
      if (!pkXXX_is_conseps(pk,po) ){
	matrixXXX_normalize_constraint(pk,po->C,po->dim);
      }
      matrixXXX_sort_rows(pk,po->C);
      cherniXXX_minimize(pk,true,po);
      if (pk->exn) goto pkXXX_chernikova_exit0;
      po->status = pk_status_consgauss;
    }
    else {
      po->C = po->F; po->F = NULL;
      matrixXXX_sort_rows(pk,po->C);
      cherniXXX_minimize(pk,false,po);
      pkXXX_dual(po);
      if (pk->exn) goto pkXXX_chernikova_exit0;
      po->status = pk_status_gengauss;
    }
  }
  return;
 pkXXX_chernikova_exit0:
  po->status = 0;
  {
    char str[160];
    sprintf(str,"conversion from %s %s\n",
	    po->C ? "constraints to generators" : "generators to constraints",
	    msg);
    ap_manager_raise_exception(man,pk->exn,pk->funid,str);
  }
  return;
}

/* Same as pkXXX_chernikova, but if usual is false meaning of constraints and
   matrices exchanged. */
void pkXXX_chernikova_dual(ap_manager_t* man,
			  pkXXX_t* po,
			  char* msg,
			  bool usual)
{
  if ( (!po->C && !po->F) || (po->C && po->F) )
    return;
  else {
    if (!usual) pkXXX_dual(po);
    pkXXX_chernikova(man,po,msg);
    if (!usual) pkXXX_dual(po);
  }
}
/* Ensure minimized and normalized epsilon
   constraints. */
void pkXXX_chernikova2(ap_manager_t* man,
		      pkXXX_t* po,
		      char* msg)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pkXXX_chernikova(man,po,msg);
  if (pk->exn)
    return;
  if (!po->C && !po->F)
    return;
  assert(po->C && po->F);
  if (!poly_is_minimaleps(pk,po)){
    pkXXX_obtain_satF(po);
    bool change = cherniXXX_minimizeeps(pk,po);
    assert((po->status & pk_status_minimaleps) &&
	   (po->status & pk_status_conseps));
    if (change){
      matrixXXX_sort_rows(pk,po->C);
      cherniXXX_minimize(pk, true, po);
      if (pk->exn) goto pkXXX_chernikova2_exit0;
      assert(po->C && po->F);
      po->status = pk_status_consgauss | pk_status_minimaleps;
    }
  }
  assert(poly_is_minimaleps(pk,po));
  return;
 pkXXX_chernikova2_exit0:
  po->status = 0;
  {
    char str[160];
    sprintf(str,"conversion from %s %s\n",
	    po->C ? "constraints to generators" : "generators to constraints",
	    msg);
    ap_manager_raise_exception(man,pk->exn,pk->funid,str);  }
  return;
}

/* Same as pkXXX_chernikova2, but in addition normalize matrices by Gauss
   elimination and sorting */
void pkXXX_chernikova3(ap_manager_t* man,
		      pkXXX_t* po,
		      char* msg)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pkXXX_chernikova2(man,po,msg);
  if (pk->exn)
    return;

  if (po->C){
    assert(po->F);
    size_t rank;
    if (! (po->status & pk_status_consgauss)){
      rank = cherniXXX_gauss(pk,po->C,po->nbeq);
      assert(rank==po->nbeq);
      cherniXXX_backsubstitute(pk,po->C,rank);
      po->C->_sorted = false;
    }
    if (! (po->status & pk_status_gengauss)){
      rank = cherniXXX_gauss(pk,po->F,po->nbline);
      assert(rank==po->nbline);
      cherniXXX_backsubstitute(pk,po->F,rank);
      po->F->_sorted = false;
    }
    pkXXX_obtain_sorted_C(pk,po);
    pkXXX_obtain_sorted_F(pk,po);
    po->status |=
      pk_status_consgauss |
      pk_status_gengauss;
    assert(po->C->_sorted && po->F->_sorted &&
	   (po->status | pk_status_consgauss) &&
	   (po->status | pk_status_gengauss) &&
	   (po->status | pk_status_minimaleps));
  }
}

void pkXXX_obtain_sorted_F(pkXXX_internal_t* pk, pkXXX_t* po)
{
  assert (po->F);

  if (!matrixXXX_is_sorted(po->F)){
    if (po->satC){
      if (po->satF){ satmat_free(po->satF); po->satF = 0; }
      matrixXXX_sort_rows_with_sat(pk,po->F,po->satC);
    }
    else if (po->satF){
      po->satC = satmat_transpose(po->satF,po->F->nbrows);
      satmat_free(po->satF); po->satF = 0;
      matrixXXX_sort_rows_with_sat(pk,po->F,po->satC);
    }
    else {
      matrixXXX_sort_rows(pk,po->F);
    }
  }
}

void pkXXX_obtain_sorted_C(pkXXX_internal_t* pk, pkXXX_t* po)
{
  assert (po->C);

  if (!matrixXXX_is_sorted(po->C)){
    if (po->satF){
      if (po->satC){ satmat_free(po->satC); po->satC = 0; }
      matrixXXX_sort_rows_with_sat(pk,po->C,po->satF);
    }
    else if (po->satC){
      po->satF = satmat_transpose(po->satC,po->C->nbrows);
      satmat_free(po->satC); po->satC = 0;
      matrixXXX_sort_rows_with_sat(pk,po->C,po->satF);
    }
    else {
      matrixXXX_sort_rows(pk,po->C);
    }
  }
}

/* ====================================================================== */
/* II.2 Canonical form */
/* ====================================================================== */

/* Put the polyhedron with minimized constraints and frames.  If in addition
   the integer man->option->canonicalize.algorithm is positive,
   normalize equalities and lines, and also strict constraints */

void pkXXX_canonicalize(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_CANONICALIZE);

  assert(pkXXX_check(pk,po));
  if (pk->option.strong_normalization >= 0)
    pkXXX_chernikova3(man,po,NULL);
  else
    pkXXX_chernikova(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    return;
  }
  assert(pkXXX_check(pk,po));
  man->result.flag_exact = man->result.flag_best =
    po->dim.intd>0 && (po->C || po->F) ? false : true;
}

int pkXXX_hash(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_HASH);
  int res,t;
  size_t i;
  ap_funopt_t opt = ap_manager_get_funopt(man,AP_FUNID_CANONICALIZE);

  pkXXX_chernikova3(man,po,NULL);
  assert(pkXXX_check(pk,po));
  res = 5*po->dim.intd + 7*po->dim.reald;
  if (po->C!=NULL){
    res += po->C->nbrows*11 +  po->F->nbrows*13;
    for (i=0; i<po->C->nbrows; i += (po->C->nbrows+3)/4){
      res = res*3 + vectorXXX_hash(pk,po->C->p[i],po->C->nbcolumns);
    }
    for (i=0; i<po->F->nbrows; i += (po->F->nbrows+3)/4){
      res = res*3 + vectorXXX_hash(pk,po->F->p[i],po->F->nbcolumns);
    }
  }
  return res;
}

/* Minimize the size of the representation of the polyhedron */
void pkXXX_minimize(ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_MINIMIZE);

  if (po->C || po->F){
    pkXXX_chernikova2(man,po,NULL);
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      man->result.flag_exact = man->result.flag_best = false;
      return;
    }
    if (po->C || po->F){
      if (po->satC) satmat_free(po->satC);
      if (po->satF) satmat_free(po->satF);
      po->satC = po->satF = NULL;
      if (po->C->nbrows > po->F->nbrows){
	matrixXXX_free(po->C);
	po->C = NULL;
	matrixXXX_minimize(po->F);
	po->status &= ~pk_status_consgauss;
      }
      else {
	matrixXXX_free(po->F);
	po->F = NULL;
	matrixXXX_minimize(po->C);
	po->status &= ~pk_status_gengauss;
      }
    }
  }
  assert(pkXXX_check(pk,po));
  man->result.flag_exact = man->result.flag_best =
    po->dim.intd>0 && (po->C || po->F) ? false : true;
}

/* ====================================================================== */
/* II.4 Minimal representation */
/* ====================================================================== */

/* Is the polyhedron in a minimal representation ? */
bool pkXXX_is_minimal(ap_manager_t* man, pkXXX_t* po)
{
  if ( (!po->C && !po->F) ||
       (po->C && po->F && (po->status & pk_status_minimaleps)) )
    return true;
  else
    return false;
}

/* Is the polyhedron in a canonical representation ?
   (depends on the algorithm option of canonicalize) */
bool pkXXX_is_canonical(ap_manager_t* man, pkXXX_t* po)
{
  bool res;

  if (!po->C && !po->F)
    res = true;
  else if (!po->C || !po->F)
    res = false;
  else {
    pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
    assert(pkXXX_check(pk,po));
    if (po->C->_sorted && po->F->_sorted &&
	po->status & pk_status_consgauss &&
	po->status & pk_status_gengauss &&
	poly_is_minimaleps(pk,po))
      res = true;
    else
      res = false;
  }
  return res;
}

/* ********************************************************************** */
/* III Printing */
/* ********************************************************************** */

void pkXXX_fprint(FILE* stream, ap_manager_t* man, pkXXX_t* po,
		  char** name_of_dim)
{
  pkXXX_internal_t* pk = pkXXX_init_from_manager(man,AP_FUNID_FPRINT);

  pkXXX_chernikova(man,po,NULL);
  if (!po->C && !po->F){
    assert(pk->exn == AP_EXC_NONE);
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->dim.intd,(unsigned long)po->dim.reald);
  }
  else {
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->dim.intd,(unsigned long)po->dim.reald);
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      fprintf(stream,"cannot display due to exception\n");
    }
    else {
      ap_lincons0_array_t cons;
      ap_lincons0_array_init(cons,AP_SCALAR_MPQ,0);
      pkXXX_to_lincons_array(man,cons,po);
      ap_lincons0_array_fprint(stream,cons,name_of_dim);
      ap_lincons0_array_clear(cons);
    }
  }
}

void pkXXX_fprintdiff(FILE* stream, ap_manager_t* man,
		      pkXXX_t* po1, pkXXX_t* po2,
		      char** name_of_dim)
{
  pkXXX_init_from_manager(man,AP_FUNID_FPRINTDIFF);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_FPRINTDIFF,NULL);
}

/* Raw printing function. */
void pkXXX_fdump(FILE* stream, ap_manager_t* man, pkXXX_t* po)
{
  pkXXX_init_from_manager(man,AP_FUNID_FDUMP);
  if (!po->C && !po->F)
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->dim.intd,(unsigned long)po->dim.reald);
  else {
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->dim.intd,(unsigned long)po->dim.reald);
    if (po->C){
      fprintf(stream,"Constraints: ");
      matrixXXX_fprint(stream, po->C);
    }
    if (po->F){
      fprintf(stream,"Frames: ");
      matrixXXX_fprint(stream, po->F);
    }
    if (po->satC){
      fprintf(stream,"satC: ");
      satmat_fprint(stream, po->satC);
    }
    if (po->satF){
      fprintf(stream,"satF: ");
      satmat_fprint(stream, po->satF);
    }
  }
}

/* ********************************************************************** */
/* IV. Serialization */
/* ********************************************************************** */

ap_membuf_t pkXXX_serialize_raw(ap_manager_t* man, pkXXX_t* a)
{
  ap_membuf_t membuf;
  pkXXX_init_from_manager(man,AP_FUNID_SERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,NULL);
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}
pkXXX_t* pkXXX_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  pkXXX_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,NULL);
  return NULL;
}

/* ********************************************************************** */
/* V. Checking */
/* ********************************************************************** */

/* Should be true
   If false, implies that there is no positivity constraint  */
static bool matrixXXX_check1cons(pkXXX_internal_t* pk, matrixXXX_t* mat)
{
  size_t i;
  bool res;
  res = false;
  for (i = 0; i<mat->nbrows; i++){
    if (numintXXX_sgn(mat->p[i][pk->dec-1])>0){
      res = true;
      break;
    }
  }
  return res;
}
/* true <=> \xi or \epsilon always positive */
static bool matrixXXX_check1ray(pkXXX_internal_t* pk, matrixXXX_t* mat)
{
  size_t i;
  bool res;
  res = true;
  for (i = 0; i<mat->nbrows; i++){
    if (numintXXX_sgn(mat->p[i][pk->dec-1])<0){
      res = false;
      break;
    }
  }
  return res;
}

/* Return false if not normalized constraint */
static bool matrixXXX_check2(pkXXX_internal_t* pk, matrixXXX_t* mat)
{
  size_t i;
  bool res;
  numintXXX_t gcd;
  numintXXX_init(gcd);

  res = true;
  for (i=0; i<mat->nbrows; i++){
    vectorXXX_gcd(pk, &mat->p[i][1], mat->nbcolumns-1, gcd);
    if (numintXXX_cmp_int(gcd,1)>0){
      res = false;
      break;
    }
  }
  numintXXX_clear(gcd);
  return res;
}

/* If false, _sorted is true without sorted rows */
static bool matrixXXX_check3(pkXXX_internal_t* pk, matrixXXX_t* mat)
{
  size_t i;
  bool res;

  if (mat->_sorted==false)
    return true;

  res = true;
  for (i=0; i<mat->nbrows-1; i++){
    if (matrixXXX_compare_rows(pk,mat,i,i+1)>0){
      res = false;
      break;
    }
  }
  return res;
}


bool pkXXX_check(pkXXX_internal_t* pk, pkXXX_t* po)
{
  bool res;
  size_t nbdim,nbcols;
  nbdim = po->dim.intd + po->dim.reald;
  if (po->nbline+po->nbeq>nbdim){
    fprintf(stderr,"pkXXX_check: nbline+nbeq>dim.intd+dim.reald\n");
    return false;
  }
  if (!po->C && !po->F)
    return true;

  nbcols = po->C ? po->C->nbcolumns : po->F->nbcolumns;
  if (nbcols != pk->dec+nbdim){
    fprintf(stderr,"pkXXX_check: pk->dec+dim.intd+dim.reald != nbcols\n");
    return false;
  }
  if (po->C){
    res = matrixXXX_check1cons(pk,po->C);
    if (!res){ /* should not arise */
      fprintf(stderr,"pkXXX_check: unvalid constraint system, does not imply the positivity constraint\n");
      return false;
    }
    res = matrixXXX_check2(pk,po->C);
    if (!res){
      fprintf(stderr,"pkXXX_check: C not normalized\n");
      return false;
    }
    res = matrixXXX_check3(pk,po->C);
    if (!res){
      fprintf(stderr,"pkXXX_check: C not sorted although _sorted=true\n");
      return false;
    }
  }
  if (po->F){
    res = matrixXXX_check1ray(pk,po->F);
    if (!res){ /* should not arise */
      fprintf(stderr,"pkXXX_check: unvalid generator system, does not imply the positivity constraint\n");
      return false;
    }
    res = matrixXXX_check2(pk,po->F);
    if (!res){
      fprintf(stderr,"pkXXX_check: F not normalized\n");
      return false;
    }
    res = matrixXXX_check3(pk,po->F);
    if (!res){
      fprintf(stderr,"pkXXX_check: F not sorted although _sorted=true\n");
      return false;
    }
    /* In strict mode, check that it satisfies \xi-\epsilon>=0 */
    if (pk->strict){
      numintXXX_t* tab = pk->vector_numintp;
      matrixXXX_t* F = po->F;
      vectorXXX_clear(tab,F->nbcolumns);
      numintXXX_set_int(tab[0],1);
      numintXXX_set_int(tab[polka_cst],1);
      numintXXX_set_int(tab[polka_eps],-1);
      bool res = true;
      int sign;      /* sign of the scalar product */
      size_t i;

      for (i=0; i<F->nbrows; i++){
	vectorXXX_product(pk,pk->poly_prod,
			  F->p[i],
			  tab,F->nbcolumns);
	sign = numintXXX_sgn(pk->poly_prod);
	if (sign<0){
	  res = false; break;
	}
	else {
	  if (numintXXX_sgn(F->p[i][0])==0){
	    /* line */
	    if (sign!=0){ res = false; break; }
	  }
	}
      }
      if (!res){
	fprintf(stderr,"pkXXX_check: F does not satisfy epsilon<=1");
	return false;
      }
    }
  }
  if (po->C && po->F){
    if (!po->satC && !po->satF){
      fprintf(stderr,"pkXXX_check: we have both constraints and generators, but no saturation matrix !\n");
      return false;
    }
  }
  else
    return true;

  if (po->C->nbcolumns != nbcols || po->F->nbcolumns != nbcols){
    fprintf(stderr,"pkXXX_check: po->C->nbcolumns==%lu, po->F->nbcolumns==%lu\n",
	    (unsigned long)po->C->nbcolumns, (unsigned long)po->F->nbcolumns);
    return false;
  }
  if (po->satC){
    if (po->satC->nbrows!=po->F->nbrows){
      fprintf(stderr,"pkXXX_check: po->satC->nbrows==%lu, po->F->nbrows==%lu\n",
	      (unsigned long)po->satC->nbrows,(unsigned long)po->F->nbrows);
      return false;
    }
    if (!cherniXXX_checksat(pk,true,
			    po->C,po->nbeq,
			    po->F,po->nbline,
			    po->satC)){
      fprintf(stderr,"pkXXX_check: bad saturation numbers\n");
      return false;
    }
    res = cherniXXX_checksatmat(pk,true,po->C,po->F,po->satC);
    if (!res){
      fprintf(stderr,"pkXXX_check: bad satC\n");
      return false;
    }
  }
  if (po->satF){
    if (po->satF->nbrows!=po->C->nbrows){
      fprintf(stderr,"pkXXX_check: po->satF->nbrows==%lu, po->C->nbrows==%lu\n",
	      (unsigned long)po->satF->nbrows, (unsigned long)po->C->nbrows);
      return false;
    }
    if (!cherniXXX_checksat(pk,false,
			    po->F,po->nbline,
			    po->C,po->nbeq,
			    po->satF)){
      fprintf(stderr,"pkXXX_check: bad saturation numbers\n");
      return false;
    }
    res = cherniXXX_checksatmat(pk,false,po->F,po->C,po->satF);
    if (!res){
      fprintf(stderr,"pkXXX_check: bad satF\n");
      return false;
    }
  }
  /* Check status: conseps */
  if (pk->strict && (po->status & pk_status_conseps) && po->C){
    matrixXXX_t* mat = po->C;
    size_t i;
    numintXXX_t* vec = vectorXXX_alloc(mat->nbcolumns);
    res = true;
    for (i=0; i<mat->nbrows; i++){
      if (numintXXX_sgn(mat->p[i][polka_eps])<=0){
	vectorXXX_copy(vec,mat->p[i],mat->nbcolumns);
	if (vectorXXX_normalize_constraint(pk,vec,po->dim)){
	  res = false;
	  break;
	}
      }
    }
    vectorXXX_free(vec,mat->nbcolumns);
    if (!res){
      fprintf(stderr,"pkXXX_check: pk_status_conseps true but the matrix is not normalized\n");
      return false;
    }
  }
  return true;
}

bool pkXXX_check_dual(pkXXX_internal_t* pk, pkXXX_t* po, bool usual)
{
  bool res;
  if (!usual) pkXXX_dual(po);
  res = pkXXX_check(pk,po);
  if (!usual) pkXXX_dual(po);
  return res;
}
