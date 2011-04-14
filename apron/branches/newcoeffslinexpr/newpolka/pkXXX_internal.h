/* ************************************************************************* */
/* pkXXX_internal.h: internal manager */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _PKXXX_INTERNAL_H_
#define _PKXXX_INTERNAL_H_

#include "eitvXXX.h"
#include "pkXXX.h"
#include "pk_satmat.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Do not change ! */
static const size_t polka_cst = 1;
static const size_t polka_eps = 2;

/* ********************************************************************** */
/* I. pk_internal_t */
/* ********************************************************************** */

/* These variables are used by various functions.  The prefix XXX_
   indicates that the variable is used by the module XXX. */

struct pkXXX_internal_t {
  num_internal_ptr num;
  enum ap_exc_t exn;

  bool strict;
  size_t dec;

  size_t maxdims;
  size_t maxcols;
  size_t maxrows;

  ap_funid_t funid;
  ap_funopt_t* funopt;

  size_t max_coeff_size; /* Used for overflow exception in vector_combine */
  size_t approximate_max_coeff_size;

  numintXXX_t* vector_numintp; /* of size maxcols */

  mpq_t* vector_mpqp; /* of size maxdims+3 */

  numintXXX_t* vector_tmp;    /* of size 5 */

  ap_dim_t* matrix_dimp;                /* of size maxdims */
  numintXXX_t matrix_acc;
  numintXXX_t matrix_prod;

  /* bitstring_t* cherni_bitstringp; */ /* of size maxrows */
  int* cherni_intp;                /* of size maxcols */
  numintXXX_t cherni_prod;

  eitvXXX_t eitvXXX;
  ap_linexprXXX_t ap_linexprXXX;
  ap_linconsXXX_t ap_linconsXXX;
  ap_lingenXXX_t ap_lingenXXX;
  ap_linexprMPQ_array_t linexprMPQ_array;
  ap_linexprXXX_t envXXX;                  /* of size maxdims */
  numXXX_t numratXXX;
  numintXXX_t* numintXXXp;            /* of size maxcols */
  numintXXX_t* numintXXXp2;           /* of size maxcols */
  /* bitstring_t* poly_bitstringp; */    /* of size maxrows */
  ap_dim_t* dimp;                /* of size maxdims */
  ap_dim_t* dimp2;               /* of size maxdims */
  struct matrixXXX_t* poly_matspecial;
  numintXXX_t poly_prod;
};

pkXXX_internal_t* pkXXX_internal_alloc(bool strict);
  /* Allocates pk and initializes it with a default size */
void pkXXX_internal_free(pkXXX_internal_t* pk);
  /* Clear and free pk */
void pkXXX_internal_realloc_lazy(pkXXX_internal_t* pk, size_t maxdims);
  /* Reallocate pk only if a bigger dimension is required */

static inline pkXXX_internal_t* pkXXX_init_from_manager(ap_manager_t* man, ap_funid_t funid);
  /* Initializes some fields of pk from manager */

/* ********************************************************************** */
/* II. Vectors */
/* ********************************************************************** */

/* Basic Operations */
numintXXX_t*	vectorXXX_alloc(size_t size);
void		vectorXXX_realloc(numintXXX_t** q, size_t size, size_t nsize);
void		vectorXXX_free(numintXXX_t* q, size_t size);
void		vectorXXX_clear(numintXXX_t* q, size_t size);
void		vectorXXX_copy(numintXXX_t* q1, numintXXX_t* q2, size_t size);
void		vectorXXX_print(numintXXX_t* q, size_t size);

/* Normalization */
bool vectorXXX_normalize(pkXXX_internal_t* pk, numintXXX_t* q, size_t size);
bool vectorXXX_normalize_expr(pkXXX_internal_t* pk, numintXXX_t* q, size_t size);
bool vectorXXX_normalize_constraint(pkXXX_internal_t* pk,
				    numintXXX_t* q, ap_dimension_t dim);
bool vectorXXX_normalize_constraint_int(pkXXX_internal_t* pk,
					numintXXX_t* q, ap_dimension_t dim);

/* Comparison */
int vectorXXX_compare(pkXXX_internal_t* pk,
		      numintXXX_t* q1, numintXXX_t* q2, size_t size);

/* Combination and Algebraic Operations */
void vectorXXX_combine(pkXXX_internal_t* pk,
		       numintXXX_t* q1, numintXXX_t* q2,
		       numintXXX_t* q3, size_t k, size_t size);
void vectorXXX_product(pkXXX_internal_t* pk,
		       numintXXX_t prod,
		       numintXXX_t* q1, numintXXX_t* q2, size_t size);
void vectorXXX_product_strict(pkXXX_internal_t* pk,
			      numintXXX_t prod,
			      numintXXX_t* r1, numintXXX_t* r2, size_t size);

/* Predicates that can be useful for users */
bool vectorXXX_is_null(pkXXX_internal_t* pk, numintXXX_t* q, size_t size);
bool vectorXXX_is_null_strict(pkXXX_internal_t* pk, numintXXX_t* q, size_t size);
bool vectorXXX_is_positivity_constraint(pkXXX_internal_t* pk,numintXXX_t* q,size_t size);
bool vectorXXX_is_dummy_constraint(pkXXX_internal_t* pk, numintXXX_t* q, size_t size);
bool vectorXXX_is_dummy_or_strict_generator(pkXXX_internal_t* pk,
					 numintXXX_t* q, size_t size);
bool vectorXXX_is_integer(pkXXX_internal_t* pk, numintXXX_t* vec, ap_dimension_t dim);
long vectorXXX_hash(pkXXX_internal_t* pk, numintXXX_t* vec, size_t size);
/* Functions meant to be internal */
numintXXX_t* _vectorXXX_alloc_int(size_t size);
void vectorXXX_gcd(pkXXX_internal_t* pk, numintXXX_t* q, size_t size, numintXXX_t gcd);

/* ********************************************************************** */
/* III. Matrices */
/* ********************************************************************** */

typedef struct matrixXXX_t {
  /* public part */
  numintXXX_t** p;     /* array of pointers to rows */
  size_t nbrows;      /* number of effective rows */
  size_t nbcolumns;   /* size of rows */

  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
  bool _sorted;
} matrixXXX_t;

/* Normal functions */

/* information about private part */
static inline size_t matrixXXX_get_maxrows(matrixXXX_t* mat)
{ return mat->_maxrows; }
static inline bool matrixXXX_is_sorted(matrixXXX_t* mat)
{ return mat->_sorted; }

/* Basic Operations */
matrixXXX_t* matrixXXX_alloc(size_t nbrows, size_t nbcols, bool s);
void      matrixXXX_resize_rows(matrixXXX_t* mat, size_t nbrows);
void      matrixXXX_resize_rows_lazy(matrixXXX_t* mat, size_t nbrows);
void      matrixXXX_minimize(matrixXXX_t* mat);
void      matrixXXX_free(matrixXXX_t* mat);
void      matrixXXX_clear(matrixXXX_t* mat);
void      matrixXXX_print(matrixXXX_t* mat);
void      matrixXXX_fprint(FILE* stream, matrixXXX_t* mat);
matrixXXX_t* matrixXXX_copy(matrixXXX_t* mat);
bool      matrixXXX_equal(matrixXXX_t* mata, matrixXXX_t* matb);

/* Operations on rows */
void matrixXXX_normalize_row(pkXXX_internal_t* pk,
			     matrixXXX_t* mat, size_t l);
void matrixXXX_combine_rows(pkXXX_internal_t* pk,
			    matrixXXX_t* mat, size_t l1, size_t l2, size_t l3, size_t k);
int matrixXXX_compare_rows(pkXXX_internal_t* pk,
			   matrixXXX_t* mat, size_t l1, size_t l2);
void matrixXXX_exch_rows(matrixXXX_t* mat, size_t l1, size_t l2);
void matrixXXX_move_rows(matrixXXX_t* mat, size_t destrow, size_t orgrow, size_t size);

/* Normalization of rows */
bool matrixXXX_normalize_constraint(pkXXX_internal_t* pk,
				    matrixXXX_t* mat,
				    ap_dimension_t dim);
bool matrixXXX_normalize_constraint_int(pkXXX_internal_t* pk,
					matrixXXX_t* mat,
					ap_dimension_t dim);

/* Sorting & Merging */
void matrixXXX_sort_rows(pkXXX_internal_t* pk,
			 matrixXXX_t* mat);
void matrixXXX_sort_rows_with_sat(pkXXX_internal_t* pk,
				  matrixXXX_t* mat, satmat_t* sat);

matrixXXX_t* matrixXXX_append(matrixXXX_t* ma, matrixXXX_t* mb);
void matrixXXX_append_with(matrixXXX_t* ma, matrixXXX_t* mb);
void matrixXXX_revappend_with(matrixXXX_t* ma, matrixXXX_t* mb);

matrixXXX_t* matrixXXX_merge_sort(pkXXX_internal_t* pk,
				  matrixXXX_t* ma, matrixXXX_t* mb);
void matrixXXX_merge_sort_with(pkXXX_internal_t* pk,
			       matrixXXX_t* mat, matrixXXX_t* cmat);


/* Predicates that can be useful for users */
static inline
bool matrixXXX_is_row_dummy_constraint(pkXXX_internal_t* pk,
				       matrixXXX_t* mat,
				       size_t l)
{
  return vectorXXX_is_dummy_constraint(pk,
				       mat->p[l],mat->nbcolumns);
}


/* Functions meant to be internal */
matrixXXX_t* _matrixXXX_alloc_int(size_t nr, size_t nc, bool s);

/* ********************************************************************** */
/* IV. Other prototypes */
/* ********************************************************************** */

/* Minimization function, in the sense of minimized dual representation This
   function minimizes if not already done the given polyhedron.
   Raise an exception, but still transmit it (pk->exn not reseted).
*/
void pkXXX_chernikova(ap_manager_t* man, pkXXX_t* poly, char* msg);

/* Same as pkXXX_chernikova, but in addition ensure normalized epsilon
   constraints. */
void pkXXX_chernikova2(ap_manager_t* man, pkXXX_t* poly, char* msg);

/* Same as pkXXX_chernikova2, but in addition normalize matrices by Gauss
   elimination and sorting */
void pkXXX_chernikova3(ap_manager_t* man, pkXXX_t* poly, char* msg);

/* Has the polyhedron normalized strict constraints ? */
static inline bool pkXXX_is_conseps(pkXXX_internal_t* pk, pkXXX_t* po);

/* Is the polyhedron under (strong) normal form ? */
bool pkXXX_is_canonical(ap_manager_t* man, pkXXX_t* po);

/* Make available the matrix of constraints (resp. frames). The matrix will
   remain unavailable iff the polyhedron appears to be empty */
static inline void pkXXX_obtain_C(ap_manager_t* man, pkXXX_t* po, char* msg);
static inline void pkXXX_obtain_F(ap_manager_t* man, pkXXX_t* po, char* msg);

/* Assuming the the matrix of constraints (resp. frames) is available, sort it,
   and take care of the saturation matrices. */
void pkXXX_obtain_sorted_C(pkXXX_internal_t* pk, pkXXX_t* poly);
void pkXXX_obtain_sorted_F(pkXXX_internal_t* pk, pkXXX_t* poly);

/* Assuming one of the saturation matrix is available, make satC (resp. satF)
   available. */
static inline void pkXXX_obtain_satC(pkXXX_t* poly);
static inline void pkXXX_obtain_satF(pkXXX_t* poly);

/* Exchange C and F, sat C and satF, nbeq and nbline */
static inline void pkXXX_dual(pkXXX_t* po);

/* Like pkXXX_chernikova, etc, but if usual is false, take into account the
   fact that pkXXX_dual has been applied */
void pkXXX_chernikova_dual(ap_manager_t* man, pkXXX_t* poly,char* msg, bool usual);
static inline
void pkXXX_obtain_C_dual(ap_manager_t* man,pkXXX_t* po, char* msg, bool usual);
static inline
void pkXXX_obtain_F_dual(ap_manager_t* man,pkXXX_t* po, char* msg, bool usual);

/* Perform rather detailed and costly checks on a polyhedron, to detect
   inconsistencies */
bool pkXXX_check(pkXXX_internal_t* pk, pkXXX_t* po);
/* Same as previous function, but usual==false mean that pkXXX_dual has been
   applied */
bool pkXXX_check_dual(pkXXX_internal_t* pk, pkXXX_t* po, bool usual);

/* Allocates a polyedron and fills its structure with null values, which
   corresponds to a bottom polyhedron. */
pkXXX_t* pkXXX_alloc(ap_dimension_t dim);

/* Free all the members of the polyhedron structure (GMP semantics) */
void pkXXX_clear(pkXXX_t* po);

/*  Assignement with GMP semantics */
void pkXXX_set(pkXXX_t* pa, pkXXX_t* pb);

/* Fill the first (pk->dec-1) rows of the matrix with the constraints of the
   universe polyhedron */
void matrixXXX_fill_constraint_top(pkXXX_internal_t* pk, matrixXXX_t* C, size_t start);

/* Assign with GMP semantics the given polyhedron with the empty
   (resp. universe) polyhedron, of same dimensions */
void pkXXX_set_bottom(pkXXX_internal_t* pk, pkXXX_t* po);
void pkXXX_set_top(pkXXX_internal_t* pk, pkXXX_t* po);

/*
  F is suppposed to be a valid matrix of ray (i.e., corresponding to a non
  empty polyhedron.

  The epsilon component of the constraint is not taken into account.  The
  constraint is considered as strict only if the is_strict paramater telles
  so.  This enables to test the satisfiability of a strict constraint in
  non-strict mode for the library.
*/
bool do_generators_sat_vectorXXX(
    pkXXX_internal_t* pk, matrixXXX_t* F, numintXXX_t* tab, bool is_strict
);

/* Bounding by a itv box a matrix of generators. */
void matrixXXX_to_box(pkXXX_internal_t* pk, ap_linexprXXX_t res, matrixXXX_t* F);

/* Bounding the value of a dimension in a matrix of generators.
   mode == 1: sup bound
   mode == -1: inf bound
*/
void matrixXXX_bound_dimension(
    pkXXX_internal_t* pk, eitvXXX_t eitv, ap_dim_t dim, matrixXXX_t* F
);

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/
void matrixXXX_bound_vector(
    pkXXX_internal_t* pk, eitvXXX_t eitv, numintXXX_t* vec,matrixXXX_t* F
);

/* In all the following functions, for the structure poly_t:

   - If meet is true, standard meaning of everything
   - If meet is false,
     - matrices C and F, satC and satF have been exchanged,
     - nbeq and nbline have been exchanged
     - in status, nothing has changed
*/

/* Meet/Join of a polyhedronwith an array of constraints/generators.

  The polyhedron is supposed:
   - to have constraints,
   - if lazy is false, to be minimized, and to have satC
   - possible emptiness not detected,

   Matrix of constraints is supposed:
   - to be sorted
   - to be canonical too.

   Return true if exception
 */
bool pkXXX_meet_matrix(
    bool meet, bool lazy, ap_manager_t* man, pkXXX_t* po, pkXXX_t* pa, matrixXXX_t* mat
);
void pkXXX_meet_ap_linconsXXX_array(
    bool lazy, ap_manager_t* man, pkXXX_t* po, pkXXX_t* pa, ap_linconsXXX_array_t array
);
void pkXXX_meetjoin(
    bool meet, bool lazy, ap_manager_t* man, pkXXX_t* po, pkXXX_t* pa, pkXXX_t* pb
);

void vectorXXX_remove_dimensions(
    pkXXX_internal_t* pk, numintXXX_t* newq, numintXXX_t* q, size_t size,
    ap_dimchange_t* dimchange
);
matrixXXX_t* matrixXXX_add_dimensions(
    pkXXX_internal_t* pk, bool destructive, matrixXXX_t* mat,
    ap_dimchange_t* dimchange
);

/* From ITV to PK */

/* Fills the vector with the constraint:
   dim <= bound if sgn>0,
   dim = bound if sgn=0
   dim >= bound if sgn<0

   bound is assumed <> oo.

   Returns false if equality of an integer dimension with a non-integer numbers
*/

bool vectorXXX_set_dim_bound(
    pkXXX_internal_t* pk,
    numintXXX_t* vec, ap_dim_t dim, numXXX_t numrat,
    int mode, ap_dimension_t dimen, bool integer);

bool vectorXXX_set_linexpr_bound(
    pkXXX_internal_t* pk,
    numintXXX_t* vec,numintXXX_t* vec2,numXXX_t numrat,int mode,
    ap_dimension_t dim, bool integer);

/* Fills the vector with the quasi-linear expression (ap_linexprXXX) */
void vectorXXX_set_ap_linexprXXX(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_linexprXXX_t expr, ap_dimension_t dim, int mode);

/* Fills the vector(s) with the linear constraint cons */
void vectorXXX_set_ap_linconsXXX(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_linconsXXX_t cons, ap_dimension_t dim, bool integer);

/* Fills the vector(s) with the linear constraint cons for testing
   satisfiability. Returns false if unsatisfiable
 */
bool vectorXXX_set_ap_linconsXXX_sat(
    pkXXX_internal_t* pk, numintXXX_t* vec,
    ap_linconsXXX_t cons, ap_dimension_t dim, bool integer);

/* From APRON to PK */
/* Fills the vector with the generator gen. */
bool vectorXXX_set_ap_lingen0(
    pkXXX_internal_t* pk, numintXXX_t* vec, ap_lingen0_t gen,
    ap_dimension_t dim
);
bool matrixXXX_set_ap_lingen0_array(
    pkXXX_internal_t* pk, matrixXXX_t** matrix, ap_lingen0_array_t array,
    ap_dimension_t dim
);
/* From ITV to PK */
bool matrixXXX_set_ap_linconsXXX_array(
    pkXXX_internal_t* pk, matrixXXX_t** mat, ap_linconsXXX_array_t array,
    ap_dimension_t dim, bool integer
);
/* From PK to APRON */
bool lincons0_set_vector(pkXXX_internal_t* pk, ap_lincons0_t cons, numintXXX_t* q, size_t size);
bool lingen0_set_vector(pkXXX_internal_t* pk, ap_lingen0_t gen, numintXXX_t* q, size_t size);

/* ********************************************************************** */
/* ********************************************************************** */
/* INLINE DEFINITIONS */
/* ********************************************************************** */

static inline pkXXX_internal_t* pkXXX_init_from_manager(ap_manager_t* man, ap_funid_t funid)
{
  pkXXX_internal_t* pk = (pkXXX_internal_t*)man->internal;
  pk->funid = funid;
  pk->funopt = &man->option.funopt[funid];
  man->result.flag_exact = man->result.flag_best = false;
  return pk;
}

static inline bool pkXXX_is_conseps(pkXXX_internal_t* pk, pkXXX_t* po)
{
  return (!pk->strict ||
	  po->dim.reald==0 ||
	  (po->status & pk_status_conseps));;
}
  static inline bool poly_is_minimaleps(pkXXX_internal_t* pk, pkXXX_t* po)
{
  return (!pk->strict ||
	  po->dim.reald==0 ||
	  (po->status & pk_status_minimaleps));
}

static inline void pkXXX_obtain_C(ap_manager_t* man, pkXXX_t* po, char* msg)
{
  if (!po->C) pkXXX_chernikova(man,po,msg);
}
static inline void pkXXX_obtain_F(ap_manager_t* man, pkXXX_t* po, char* msg)
{
  if (!po->F) pkXXX_chernikova(man,po,msg);
}
static inline void pkXXX_obtain_satF(pkXXX_t* poly)
{
  if (!poly->satF){
    assert(poly->C && poly->satC);
    poly->satF = satmat_transpose(poly->satC,poly->C->nbrows);
  }
}
static inline void pkXXX_obtain_satC(pkXXX_t* poly)
{
  if (!poly->satC){
    assert(poly->F && poly->satF);
    poly->satC = satmat_transpose(poly->satF,poly->F->nbrows);
  }
}

static inline void pkXXX_dual(pkXXX_t* po)
{
  void* ptr;
  size_t nb;
  ptr = po->C; po->C = po->F; po->F = ptr;
  ptr = po->satC; po->satC = po->satF; po->satF = ptr;
  nb = po->nbeq; po->nbeq = po->nbline; po->nbline = nb;
}

static inline void pkXXX_obtain_C_dual(ap_manager_t* man, pkXXX_t* po, char* msg, bool usual)
{
  if (!po->C) pkXXX_chernikova_dual(man,po,msg,usual);
}
static inline void pkXXX_obtain_F_dual(ap_manager_t* man, pkXXX_t* po, char* msg, bool usual)
{
  if (!po->F) pkXXX_chernikova_dual(man,po,msg,usual);
}

pkXXX_t* pkXXX_asssub_linexprXXX_det(
    bool assign, ap_manager_t* man, bool destructive,
    pkXXX_t* pa, ap_dim_t dim, ap_linexprXXX_t linexpr
);
pkXXX_t* pkXXX_asssub_linexprXXX_array_det(
    bool assign, ap_manager_t* man, bool destructive,
    pkXXX_t* pa, ap_dim_t* tdim, ap_linexprXXX_array_t texpr
);

#ifdef __cplusplus
}
#endif

#endif
