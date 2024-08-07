/* ********************************************************************** */
/* pk_representation.c: General management of polyhedra  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk_cherni.h"
#include "pk.h"
#include "pk_user.h"
#include "pk_representation.h"
#include "pk_extract.h"
#include "pk_constructor.h"

/* ********************************************************************** */
/* Conversions */
/* ********************************************************************** */

pk_t* pk_of_abstract0(ap_abstract0_t* abstract)
{
  ap_manager_t* man = abstract->man;
  if (strncmp(man->library,"polka",5)!=0){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_UNKNOWN,
			       "pk_of_abstract0: attempt to extract a NewPolka polyhedra from an abstract value which is not a wrapper around a NewPolka polyhedra");
    return NULL;
  }
  return (pk_t*)abstract->value;
}

ap_abstract0_t* pk_to_abstract0(ap_manager_t* man, pk_t* poly)
{
  if (strncmp(man->library,"polka",5)!=0){
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,
			       AP_FUNID_UNKNOWN,
			       "pk_to_abstract0: attempt to extract a NewPolka polyhedra from an abstract value which is not a wrapper around a NewPolka polyhedra");
    return ap_abstract0_top(man,poly->intdim,poly->realdim);
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

pk_t* poly_alloc(size_t intdim, size_t realdim)
{
  pk_t* po = (pk_t*)malloc(sizeof(pk_t));
  po->C = NULL;
  po->F = NULL;
  po->satC = NULL;
  po->satF = NULL;
  po->intdim = intdim;
  po->realdim = realdim;
  po->nbeq = 0;
  po->nbline = 0;
  po->status = 0;
  return po;
}

/* Clearing a polyhedron */
void poly_clear(pk_t* po)
{
  if (po->C) pk_matrix_free(po->C);
  if (po->F) pk_matrix_free(po->F);
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
void poly_set(pk_t* pa, pk_t* pb)
{
  if (pa!=pb){
    poly_clear(pa);
    pa->intdim = pb->intdim;
    pa->realdim = pb->realdim;
    pa->C = pb->C ? pk_matrix_copy(pb->C) : NULL;
    pa->F = pb->F ? pk_matrix_copy(pb->F) : NULL;
    pa->satC = pb->satC ? satmat_copy(pb->satC) : NULL;
    pa->satF = pb->satF ? satmat_copy(pb->satF) : NULL;
    pa->status = pb->status;
    pa->nbeq = pb->nbeq;
    pa->nbline = pb->nbline;
    pa->status = pb->status;
  }
}

/* Duplicate (recursively) a polyhedron. */
pk_t* pk_copy(ap_manager_t* man, pk_t* po)
{
  pk_t* npo = poly_alloc(po->intdim,po->realdim);
  npo->C = po->C ? pk_matrix_copy(po->C) : 0;
  npo->F = po->F ? pk_matrix_copy(po->F) : 0;
  npo->satC = po->satC ? satmat_copy(po->satC) : 0;
  npo->satF = po->satF ? satmat_copy(po->satF) : 0;
  npo->nbeq = po->nbeq;
  npo->nbline = po->nbline;
  npo->status = po->status;
  return npo;
}

/* Finalization function for polyhedra, which frees
   recursively the members of the structure. */
void pk_free(ap_manager_t* man, pk_t* po)
{
  poly_clear(po);
  free(po);
}

/* Return the abstract size of a polyhedron, which is the number of
   coefficients of its current representation, possibly redundant. */
size_t pk_size(ap_manager_t* man, pk_t* po)
{
  size_t s1,s2;

  s1 = (po->C) ? po->C->nbrows : 0;
  s2 = (po->F) ? po->F->nbrows : 0;
  return (s1+s2)*(po->intdim + po->realdim);
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

void poly_chernikova(ap_manager_t* man,
		     pk_t* po,
		     char* msg)
{
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  if ((po->C && po->F) || (!po->C && !po->F)){
    return;
  }
  else {
    if (po->C){
      if (!poly_is_conseps(pk,po) ){
	pk_matrix_normalize_constraint(pk,po->C,po->intdim,po->realdim);
      }
      pk_matrix_sort_rows(pk,po->C);
      cherni_minimize(pk,true,po);
      if (pk->exn) goto poly_chernikova_exit0;
      po->status = pk_status_consgauss;
    }
    else {
      po->C = po->F; po->F = NULL;
      pk_matrix_sort_rows(pk,po->C);
      cherni_minimize(pk,false,po);
      poly_dual(po);
      if (pk->exn) goto poly_chernikova_exit0;
      po->status = pk_status_gengauss;
    }
  }
  return;
 poly_chernikova_exit0:
  po->status = 0;
  {
    char str[160];
    snprintf(str,sizeof(str),"conversion from %s %s\n",
	    po->C ? "constraints to generators" : "generators to constraints",
	    msg);
    ap_manager_raise_exception(man,pk->exn,pk->funid,str);
  }
  return;
}

/* Same as poly_chernikova, but if usual is false meaning of constraints and
   matrices exchanged. */
void poly_chernikova_dual(ap_manager_t* man,
			  pk_t* po,
			  char* msg,
			  bool usual)
{
  if ( (!po->C && !po->F) || (po->C && po->F) )
    return;
  else {
    if (!usual) poly_dual(po);
    poly_chernikova(man,po,msg);
    if (!usual) poly_dual(po);
  }
}
/* Ensure minimized and normalized epsilon
   constraints. */
void poly_chernikova2(ap_manager_t* man,
		      pk_t* po,
		      char* msg)
{
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  poly_chernikova(man,po,msg);
  if (pk->exn)
    return;
  if (!po->C && !po->F)
    return;
  assert(po->C && po->F);
  if (!poly_is_minimaleps(pk,po)){
    poly_obtain_satF(po);
    bool change = cherni_minimizeeps(pk,po);
    assert((po->status & pk_status_minimaleps) &&
	   (po->status & pk_status_conseps));
    if (change){
      pk_matrix_sort_rows(pk,po->C);
      cherni_minimize(pk, true, po);
      if (pk->exn) goto poly_chernikova2_exit0;
      assert(po->C && po->F);
      po->status = pk_status_consgauss | pk_status_minimaleps;
    }
  }
  assert(poly_is_minimaleps(pk,po));
  return;
 poly_chernikova2_exit0:
  po->status = 0;
  {
    char str[160];
    snprintf(str,sizeof(str),"conversion from %s %s\n",
	    po->C ? "constraints to generators" : "generators to constraints",
	    msg);
    ap_manager_raise_exception(man,pk->exn,pk->funid,str);  }
  return;
}

/* Same as poly_chernikova2, but in addition normalize matrices by Gauss
   elimination and sorting */
static int counter = 0;
void poly_chernikova3(ap_manager_t* man,
		      pk_t* po,
		      char* msg)
{
  counter++;
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  poly_chernikova2(man,po,msg);
  if (pk->exn)
    return;

  if (po->C){
    assert(po->F);
    size_t rank;
    if (! (po->status & pk_status_consgauss)){
      rank = cherni_gauss(pk,po->C,po->nbeq);
      assert(rank==po->nbeq);
      cherni_backsubstitute(pk,po->C,rank);
      po->C->_sorted = false;
    }
    if (! (po->status & pk_status_gengauss)){
      rank = cherni_gauss(pk,po->F,po->nbline);
      assert(rank==po->nbline);
      cherni_backsubstitute(pk,po->F,rank);
      po->F->_sorted = false;
    }
    poly_obtain_sorted_C(pk,po);
    poly_obtain_sorted_F(pk,po);
    po->status |=
      pk_status_consgauss |
      pk_status_gengauss;
    assert(po->C->_sorted && po->F->_sorted &&
	   (po->status | pk_status_consgauss) &&
	   (po->status | pk_status_gengauss) &&
	   (po->status | pk_status_minimaleps));
    assert(pk_matrix_check_gauss(po->C,po->nbeq));
    assert(pk_matrix_check_gauss(po->F,po->nbline));
  }
}

void poly_obtain_sorted_F(pk_internal_t* pk, pk_t* po)
{
  assert (po->F);

  if (!pk_matrix_is_sorted(po->F)){
    if (po->satC){
      if (po->satF){ satmat_free(po->satF); po->satF = 0; }
      pk_matrix_sort_rows_with_sat(pk,po->F,po->satC);
    }
    else if (po->satF){
      po->satC = satmat_transpose(po->satF,po->F->nbrows);
      satmat_free(po->satF); po->satF = 0;
      pk_matrix_sort_rows_with_sat(pk,po->F,po->satC);
    }
    else {
      pk_matrix_sort_rows(pk,po->F);
    }
  }
}

void poly_obtain_sorted_C(pk_internal_t* pk, pk_t* po)
{
  assert (po->C);

  if (!pk_matrix_is_sorted(po->C)){
    if (po->satF){
      if (po->satC){ satmat_free(po->satC); po->satC = 0; }
      pk_matrix_sort_rows_with_sat(pk,po->C,po->satF);
    }
    else if (po->satC){
      po->satF = satmat_transpose(po->satC,po->C->nbrows);
      satmat_free(po->satC); po->satC = 0;
      pk_matrix_sort_rows_with_sat(pk,po->C,po->satF);
    }
    else {
      pk_matrix_sort_rows(pk,po->C);
    }
  }
}

/* ====================================================================== */
/* II.2 Canonical form */
/* ====================================================================== */

/* Put the polyhedron with minimized constraints and frames.  If in addition
   the integer man->option->canonicalize.algorithm is positive,
   normalize equalities and lines, and also strict constraints */

void pk_canonicalize(ap_manager_t* man, pk_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_CANONICALIZE);

  assert(poly_check(pk,po));
  if (pk->funopt->algorithm >= 0)
    poly_chernikova3(man,po,NULL);
  else
    poly_chernikova(man,po,NULL);

  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_exact = man->result.flag_best = false;
    return;
  }
  assert(poly_check(pk,po));
  man->result.flag_exact = man->result.flag_best =
    po->intdim>0 && (po->C || po->F) ? false : true;
}

int pk_hash(ap_manager_t* man, pk_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_HASH);
  int res,t;
  size_t i;
  ap_funopt_t opt = ap_manager_get_funopt(man,AP_FUNID_CANONICALIZE);

  poly_chernikova3(man,po,NULL);
  assert(poly_check(pk,po));
  res = 5*po->intdim + 7*po->realdim;
  if (po->C!=NULL){
    res += po->C->nbrows*11 +  po->F->nbrows*13;
    for (i=0; i<po->C->nbrows; i += (po->C->nbrows+3)/4){
      res = res*3 + vector_hash(pk,po->C->p[i],po->C->nbcolumns);
    }
    for (i=0; i<po->F->nbrows; i += (po->F->nbrows+3)/4){
      res = res*3 + vector_hash(pk,po->F->p[i],po->F->nbcolumns);
    }
  }
  return res;
}

/* Minimize the size of the representation of the polyhedron */
void pk_minimize(ap_manager_t* man, pk_t* po)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_MINIMIZE);

  if (po->C || po->F){
    poly_chernikova2(man,po,NULL);
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
	pk_matrix_free(po->C);
	po->C = NULL;
	pk_matrix_minimize(po->F);
	po->status &= ~pk_status_consgauss;
      }
      else {
	pk_matrix_free(po->F);
	po->F = NULL;
	pk_matrix_minimize(po->C);
	po->status &= ~pk_status_gengauss;
      }
    }
  }
  assert(poly_check(pk,po));
  man->result.flag_exact = man->result.flag_best =
    po->intdim>0 && (po->C || po->F) ? false : true;
}

/* ====================================================================== */
/* II.4 Minimal representation */
/* ====================================================================== */

/* Is the polyhedron in a minimal representation ? */
bool pk_is_minimal(ap_manager_t* man, pk_t* po)
{
  if ( (!po->C && !po->F) ||
       (po->C && po->F && (po->status & pk_status_minimaleps)) )
    return true;
  else
    return false;
}

/* Is the polyhedron in a canonical representation ?
   (depends on the algorithm option of canonicalize) */
bool pk_is_canonical(ap_manager_t* man, pk_t* po)
{
  bool res;

  if (!po->C && !po->F)
    res = true;
  else if (!po->C || !po->F)
    res = false;
  else {
    pk_internal_t* pk = (pk_internal_t*)man->internal;
    assert(poly_check(pk,po));
    if (po->C->_sorted && po->F->_sorted &&
	po->status & pk_status_consgauss &&
	po->status & pk_status_gengauss &&
	poly_is_minimaleps(pk,po)){
      assert(pk_matrix_check_gauss(po->C,po->nbeq));
      assert(pk_matrix_check_gauss(po->F,po->nbline));
      res = true;
    }
    else
      res = false;
  }
  return res;
}

/* ********************************************************************** */
/* III Printing */
/* ********************************************************************** */

void pk_fprint(FILE* stream, ap_manager_t* man, pk_t* po,
	       char** name_of_dim)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_FPRINT);

  poly_chernikova(man,po,NULL);
  if (!po->C && !po->F){
    assert(pk->exn == AP_EXC_NONE);
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->intdim,(unsigned long)po->realdim);
  }
  else {
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->intdim,(unsigned long)po->realdim);
    if (pk->exn){
      pk->exn = AP_EXC_NONE;
      fprintf(stream,"cannot display due to exception\n");
    }
    else {
      ap_lincons0_array_t cons = pk_to_lincons_array(man,po);
      ap_lincons0_array_fprint(stream,&cons,name_of_dim);
      ap_lincons0_array_clear(&cons);
    }
  }
}
void pk_print(ap_manager_t* man, pk_t* po, char** name_of_dim)
{ pk_fprint(stdout,man,po,name_of_dim); }

void pk_fprintdiff(FILE* stream, ap_manager_t* man,
		   pk_t* po1, pk_t* po2,
		   char** name_of_dim)
{
  pk_init_from_manager(man,AP_FUNID_FPRINTDIFF);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_FPRINTDIFF,NULL);
}

/* Raw printing function. */
void pk_fdump(FILE* stream, ap_manager_t* man, pk_t* po)
{
  pk_init_from_manager(man,AP_FUNID_FDUMP);
  if (!po->C && !po->F)
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->intdim,(unsigned long)po->realdim);
  else {
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)po->intdim,(unsigned long)po->realdim);
    if (po->C){
      fprintf(stream,"Constraints: ");
      pk_matrix_fprint(stream, po->C);
    }
    if (po->F){
      fprintf(stream,"Frames: ");
      pk_matrix_fprint(stream, po->F);
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

ap_membuf_t pk_serialize_raw(ap_manager_t* man, pk_t* a)
{
  ap_membuf_t membuf;
  pk_init_from_manager(man,AP_FUNID_SERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,NULL);
  membuf.ptr = NULL;
  membuf.size = 0;
  return membuf;
}
pk_t* pk_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  pk_init_from_manager(man,AP_FUNID_DESERIALIZE_RAW);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,NULL);
  return NULL;
}

/* ********************************************************************** */
/* V. Checking */
/* ********************************************************************** */

/* Should be true
   If false, implies that there is no positivity constraint  */
static bool pk_matrix_check1cons(pk_internal_t* pk, pk_matrix_t* mat)
{
  size_t i;
  bool res;
  res = false;
  for (i = 0; i<mat->nbrows; i++){
    if (numint_sgn(mat->p[i][pk->dec-1])>0){
      res = true;
      break;
    }
  }
  return res;
}
/* true <=> \xi or \epsilon always positive */
static bool pk_matrix_check1ray(pk_internal_t* pk, pk_matrix_t* mat)
{
  size_t i;
  bool res;
  res = true;
  for (i = 0; i<mat->nbrows; i++){
    if (numint_sgn(mat->p[i][pk->dec-1])<0){
      res = false;
      break;
    }
  }
  return res;
}

/* Return false if not normalized constraint */
static bool pk_matrix_check2(pk_internal_t* pk, pk_matrix_t* mat)
{
  size_t i;
  bool res;
  numint_t gcd;
  numint_init(gcd);

  res = true;
  for (i=0; i<mat->nbrows; i++){
    vector_gcd(pk, &mat->p[i][1], mat->nbcolumns-1, gcd);
    if (numint_cmp_int(gcd,1)>0){
      res = false;
      break;
    }
  }
  numint_clear(gcd);
  return res;
}

/* If false, _sorted is true without sorted rows */
static bool pk_matrix_check3(pk_internal_t* pk, pk_matrix_t* mat)
{
  size_t i;
  bool res;

  if (mat->_sorted==false)
    return true;

  res = true;
  for (i=0; i<mat->nbrows-1; i++){
    if (pk_matrix_compare_rows(pk,mat,i,i+1)>0){
      res = false;
      break;
    }
  }
  return res;
}

bool pk_matrix_check_gauss(pk_matrix_t* mat, size_t nbeq)
{
  size_t i,j,k;

  for (k=0; k<nbeq; k++){
    /* Look for the first non zero coeff from the right */
    for (j=mat->nbcolumns-1; j>=2; j--){
      if (numint_sgn(mat->p[k][j])!=0)
	break;
    }
    if (j<=1){
      fprintf(stderr,"pk_matrix_check_gauss: equality with all std coefficients set to zero !\n");
      return false;
    }
    /* Check that this coeff is zero on all other rows */
    for (i=0; i<mat->nbrows; i++){
      if (i != k && numint_sgn(mat->p[i][j])!=0){
	fprintf(stderr,"pk_matrix_check_gauss: row %llu col %llu should be zero !\n",(unsigned long long)i,(unsigned long long)j);
	return false;
      }
    }
  }
  return true;
}


bool poly_check(pk_internal_t* pk, pk_t* po)
{
  bool res;
  size_t nbdim,nbcols;
  nbdim = po->intdim + po->realdim;
  if (po->nbline+po->nbeq>nbdim){
    fprintf(stderr,"poly_check: nbline+nbeq>intdim+realdim\n");
    return false;
  }
  if (!po->C && !po->F)
    return true;

  nbcols = po->C ? po->C->nbcolumns : po->F->nbcolumns;
  if (nbcols != pk->dec+nbdim){
    fprintf(stderr,"poly_check: pk->dec+intdim+realdim != nbcols\n");
    return false;
  }
  if (po->C){
    res = pk_matrix_check1cons(pk,po->C);
    if (!res){ /* should not arise */
      fprintf(stderr,"poly_check: unvalid constraint system, does not imply the positivity constraint\n");
      return false;
    }
    res = pk_matrix_check2(pk,po->C);
    if (!res){
      fprintf(stderr,"poly_check: C not normalized\n");
      return false;
    }
    res = pk_matrix_check3(pk,po->C);
    if (!res){
      fprintf(stderr,"poly_check: C not sorted although _sorted=true\n");
      return false;
    }
  }
  if (po->F){
    res = pk_matrix_check1ray(pk,po->F);
    if (!res){ /* should not arise */
      fprintf(stderr,"poly_check: unvalid generator system, does not imply the positivity constraint\n");
      return false;
    }
    res = pk_matrix_check2(pk,po->F);
    if (!res){
      fprintf(stderr,"poly_check: F not normalized\n");
      return false;
    }
    res = pk_matrix_check3(pk,po->F);
    if (!res){
      fprintf(stderr,"poly_check: F not sorted although _sorted=true\n");
      return false;
    }
    /* In strict mode, check that it satisfies \xi-\epsilon>=0 */
    if (pk->strict){
      numint_t* tab = pk->vector_numintp;
      pk_matrix_t* F = po->F;
      vector_clear(tab,F->nbcolumns);
      numint_set_int(tab[0],1);
      numint_set_int(tab[polka_cst],1);
      numint_set_int(tab[polka_eps],-1);
      bool res = true;
      int sign;      /* sign of the scalar product */
      size_t i;

      for (i=0; i<F->nbrows; i++){
	vector_product(pk,pk->poly_prod,
		       F->p[i],
		       tab,F->nbcolumns);
	sign = numint_sgn(pk->poly_prod);
	if (sign<0){
	  res = false; break;
	}
	else {
	  if (numint_sgn(F->p[i][0])==0){
	    /* line */
	    if (sign!=0){ res = false; break; }
	  }
	}
      }
      if (!res){
	fprintf(stderr,"poly_check: F does not satisfy epsilon<=1");
	return false;
      }
    }
  }
  if (po->C && po->F){
    if (!po->satC && !po->satF){
      fprintf(stderr,"poly_check: we have both constraints and generators, but no saturation matrix !\n");
      return false;
    }
    if (po->status & pk_status_consgauss && po->status & pk_status_gengauss){
      assert(pk_matrix_check_gauss(po->C,po->nbeq));
      assert(pk_matrix_check_gauss(po->F,po->nbline));
    }
  }
  else
    return true;

  if (po->C->nbcolumns != nbcols || po->F->nbcolumns != nbcols){
    fprintf(stderr,"poly_check: po->C->nbcolumns==%lu, po->F->nbcolumns==%lu\n",
	    (unsigned long)po->C->nbcolumns, (unsigned long)po->F->nbcolumns);
    return false;
  }
  if (po->satC){
    if (po->satC->nbrows!=po->F->nbrows){
      fprintf(stderr,"poly_check: po->satC->nbrows==%lu, po->F->nbrows==%lu\n",
	      (unsigned long)po->satC->nbrows,(unsigned long)po->F->nbrows);
      return false;
    }
    if (!cherni_checksat(pk,true,
			 po->C,po->nbeq,
			 po->F,po->nbline,
			 po->satC)){
      fprintf(stderr,"poly_check: bad saturation numbers\n");
      return false;
    }
    res = cherni_checksatmat(pk,true,po->C,po->F,po->satC);
    if (!res){
      fprintf(stderr,"poly_check: bad satC\n");
      return false;
    }
  }
  if (po->satF){
    if (po->satF->nbrows!=po->C->nbrows){
      fprintf(stderr,"poly_check: po->satF->nbrows==%lu, po->C->nbrows==%lu\n",
	      (unsigned long)po->satF->nbrows, (unsigned long)po->C->nbrows);
      return false;
    }
    if (!cherni_checksat(pk,false,
			 po->F,po->nbline,
			 po->C,po->nbeq,
			 po->satF)){
      fprintf(stderr,"poly_check: bad saturation numbers\n");
      return false;
    }
    res = cherni_checksatmat(pk,false,po->F,po->C,po->satF);
    if (!res){
      fprintf(stderr,"poly_check: bad satF\n");
      return false;
    }
  }
  /* Check status: conseps */
  if (pk->strict && (po->status & pk_status_conseps) && po->C){
    pk_matrix_t* mat = po->C;
    size_t i;
    numint_t* vec = vector_alloc(mat->nbcolumns);
    res = true;
    for (i=0; i<mat->nbrows; i++){
      if (numint_sgn(mat->p[i][polka_eps])<=0){
	vector_copy(vec,mat->p[i],mat->nbcolumns);
	if (vector_normalize_constraint(pk,vec,po->intdim,po->realdim)){
	  res = false;
	  break;
	}
      }
    }
    vector_free(vec,mat->nbcolumns);
    if (!res){
      fprintf(stderr,"poly_check: pk_status_conseps true but the matrix is not normalized\n");
      return false;
    }
  }
  return true;
}

bool poly_check_dual(pk_internal_t* pk, pk_t* po, bool usual)
{
  bool res;
  if (!usual) poly_dual(po);
  res = poly_check(pk,po);
  if (!usual) poly_dual(po);
  return res;
}
