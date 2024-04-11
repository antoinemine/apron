/* ********************************************************************** */
/* pk_widening.c: widening  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_bit.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk_cherni.h"
#include "pk.h"
#include "pk_representation.h"
#include "pk_user.h"
#include "pk_constructor.h"
#include "pk_test.h"
#include "pk_qsort.h"

typedef struct satmat_row_t {
  bitstring_t* p; 
    /* pointing somewhere in the field pinit of an underlying satmat_t object */
  int index;
} satmat_row_t;


static satmat_row_t* esatmat_of_satmat(satmat_t* sat)
{
  size_t i;
  satmat_row_t* tab;

  tab = malloc(sat->nbrows * sizeof(satmat_row_t));
  for (i=0; i<sat->nbrows; i++){
    tab[i].p = sat->p[i];
    tab[i].index = i;
  }
  return tab;
}

/* Row sorting.

We use here the insertion sort. 
The array tab is supposed to be of size sat->nbrows.
*/
static void esatmat_isort_rows(satmat_row_t* tab, satmat_t* sat)
{
  size_t i,j;

  for (i=1; i<sat->nbrows; i++){
    satmat_row_t row = tab[i];
    j = i;
    while (j > 0 && bitstring_cmp(tab[j-1].p, row.p, sat->nbcolumns) > 0){
      tab[j] = tab[j-1];
      j--;
    }
    tab[j] = row;
  }
}

/* Row sorting.

We use here the quick sort. */
typedef struct qsort_man_t {
  size_t size;
} qsort_man_t;
static int qsort_rows_compar(void* qsort_man, void* p1, void* p2)
{
  qsort_man_t* qm = (qsort_man_t*)qsort_man;
  return (bitstring_cmp( ((satmat_row_t*)p1)->p,
			 ((satmat_row_t*)p2)->p,
			 qm->size));
}

MAKE_SORT(qsort_rows, satmat_row_t, qsort_rows_compar)

static void esatmat_sort_rows(satmat_row_t* tab, satmat_t* sat)
{
  if (sat->nbrows>=6){
    qsort_man_t qsort_man;
    qsort_man.size = sat->nbcolumns;
    qsort_rows(tab, (size_t)sat->nbrows, &qsort_man);
  }
  else {
    esatmat_isort_rows(tab,sat);
  }
}

/* Membership test.

The following function tests if the given row belongs to the sorted saturation
matrix. If it is the case, it returns its rank in the saturation
matrix. Otherwise, it returns -1 */

typedef struct bsearch_man_t {
  bitstring_t* satline;
  satmat_row_t* tab;
  size_t size;
} bsearch_man_t;

static int bsearch2(bsearch_man_t* man, size_t low, size_t high)
{
  if (high - low <= 4){
    size_t i;
    int res=-1;
    for (i=low; i<high; i++){
      int cmp = bitstring_cmp(man->tab[i].p, man->satline, man->size);
      if (cmp==0){
	res=i; break;
      }
      else if (cmp>0) break;
    }
    return res;
  }
  else {
    size_t mid = low+(high-low)/2;
    int cmp = bitstring_cmp(man->tab[mid].p,man->satline,man->size);
    if (cmp<0)
      return (bsearch2(man,mid+1,high));
    else if (cmp>0)
      return (bsearch2(man,low,mid));
    else
      return mid;
  }
}

static
int esatmat_index_in_sorted_rows(bitstring_t* satline, 
				 satmat_row_t* tab, 
				 satmat_t* sat)
{
  bsearch_man_t man;
  man.satline = satline;
  man.tab = tab;
  man.size = sat->nbcolumns;
  return bsearch2(&man,0,sat->nbrows);
}

/* This function defines the standard widening operator.  The resulting
   polyhedron has no frame matrix, unless pa is empty. */
pk_t* pk_widening(ap_manager_t* man, pk_t* pa, pk_t* pb)
{
  pk_internal_t* pk = pk_init_from_manager(man,AP_FUNID_WIDENING);
  bool widening_affine = pk->funopt->algorithm<=0;
  poly_chernikova2(man,pa,"of the first argument");
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    return pk_top(man,pa->intdim,pa->realdim);
  }
  poly_chernikova2(man,pb,"of the second argument");
  if (pk->exn){
    pk->exn = AP_EXC_NONE;
    man->result.flag_best = man->result.flag_exact = false;
    return pk_top(man,pa->intdim,pa->realdim);
  }
  assert(pk_is_leq(man,pa,pb));
  if (!pa->C && !pa->F) /* pa is empty */
    return pk_copy(man,pb);
  else {
    size_t sat_nbcols;
    size_t nbrows,i;
    int index;
    pk_t* po;
    bitstring_t* bitstringp;
    satmat_row_t* tab;
 
    /* copy saturation pa->satF, and sort it */
    poly_obtain_satF(pa);
    tab = esatmat_of_satmat(pa->satF);
    esatmat_sort_rows(tab,pa->satF);
    sat_nbcols = pa->satF->nbcolumns;

    po = poly_alloc(pa->intdim,pa->realdim);

    po->C = pk_matrix_alloc(pk->dec-1+pb->C->nbrows, pb->C->nbcolumns, false);
    pk_matrix_fill_constraint_top(pk,po->C,0);
    nbrows = pk->dec-1;

    /* Adding constraints of pb mutually redundant with some of pa, except if
       it is mutually redundant with the positivity constraint of pa only. */
    bitstringp = bitstring_alloc(sat_nbcols);
    for (i=0; i<pb->C->nbrows; i++){
      bitstring_clear(bitstringp,sat_nbcols);
      cherni_buildsatline(pk, pa->F, pb->C->p[i], bitstringp);
      index = esatmat_index_in_sorted_rows(bitstringp,tab,pa->satF);
      if (index>=0){
	index = tab[index].index;
	if (!widening_affine ||
	    !vector_is_positivity_constraint(pk, 
					     pa->C->p[index],
					     pa->C->nbcolumns)){
	  /* Belongs to saturation matrix, and does not correspond to the
	     positivity constraint. */
	  vector_copy(po->C->p[nbrows],pb->C->p[i], 
		      pa->C->nbcolumns);
	  nbrows++;
	}
      }
    }
    free(tab);
    bitstring_free(bitstringp);
    po->C->nbrows = nbrows;
    man->result.flag_best = man->result.flag_exact = false;
    assert(poly_check(pk,po));
    return po;
  }
}

/* This second one implements a version parametrized by a set of constraints:
   when a constraint of this set is verified by both polyhedra, it is kept in
the result. */
pk_t* pk_widening_threshold(ap_manager_t* man, 
			    pk_t* pa, pk_t* pb, 
			    ap_lincons0_array_t* array)
{
  pk_t* po;
  size_t i,nbrows,nbcols;
  size_t* tab;
  size_t size,nb;
  pk_internal_t* pk = (pk_internal_t*)man->internal;

  po = pk_widening(man,pa,pb);
  if (!po->C && !po->F)
    return po;

  /* We assume that both pa and pb are minimized, and that po->F==NULL */
  nbcols = po->C->nbcolumns;
  nbrows = po->C->nbrows;
  pk_matrix_resize_rows_lazy(po->C, nbrows + array->size);
  for (i=0; i<array->size; i++){
    switch(array->p[i].constyp){
    case AP_CONS_EQ:
      break;
    case AP_CONS_SUPEQ:
    case AP_CONS_SUP:
      if (ap_linexpr0_is_linear(array->p[i].linexpr0)){
	itv_lincons_set_ap_lincons0(pk->itv,
				    &pk->poly_itv_lincons,
				    &array->p[i]);
	vector_set_itv_lincons(pk,
			       pk->poly_numintp,
			       &pk->poly_itv_lincons,
			       pa->intdim,pa->realdim,true);
	if (do_generators_sat_vector(pk,pb->F,
				     pk->poly_numintp,
				     pk->strict && 
				     numint_sgn(pk->poly_numintp[polka_eps])<0))
	  {
	    /* if the constraint is satisfied by pb, add it */
	    vector_copy(po->C->p[nbrows],pk->poly_numintp,nbcols);
	    nbrows++;
	  }
      }
    default:
      break;
    }
  }
  po->C->nbrows = nbrows;
  pk_matrix_minimize(po->C);
  assert(poly_check(pk,po));
  return po;
}
