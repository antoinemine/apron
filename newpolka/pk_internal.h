/* ************************************************************************* */
/* pk_internal.h: internal manager */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _PK_INTERNAL_H_
#define _PK_INTERNAL_H_

#include "pk.h"
#include "num_types.h"
#include "eitvMPQ.h"

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

struct pk_internal_t {
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

  numintMPQ_t* vector_numintp; /* of size maxcols */

  mpq_t* vector_mpqp; /* of size maxdims+3 */

  numintMPQ_t* vector_tmp;    /* of size 5 */
  
  ap_dim_t* matrix_dimp;                /* of size maxdims */
  numintMPQ_t matrix_acc;
  numintMPQ_t matrix_prod;

  /* bitstring_t* cherni_bitstringp; */ /* of size maxrows */
  int* cherni_intp;                /* of size maxcols */
  numintMPQ_t cherni_prod;             

  boundMPQ_t poly_bound;
  eitvMPQ_t poly_eitv;
  ap_linexprMPQ_t poly_linexprMPQ;
  ap_linconsMPQ_t poly_linconsMPQ;
  ap_lingenMPQ_t poly_lingenMPQ;
  ap_linexprMPQ_array_t linexprMPQ_array;
  eitvMPQ_t* env;                  /* of size maxdims */
  numMPQ_t poly_numrat;
  numintMPQ_t* poly_numintp;            /* of size maxcols */
  numintMPQ_t* poly_numintp2;           /* of size maxcols */
  /* bitstring_t* poly_bitstringp; */    /* of size maxrows */
  ap_dim_t* poly_dimp;                /* of size maxdims */
  ap_dim_t* poly_dimp2;               /* of size maxdims */
  ap_dim_t* poly_fold_dimp;               /* of size maxdims */
  struct matrix_t* poly_matspecial; 
  numintMPQ_t poly_prod; 
};

pk_internal_t* pk_internal_alloc(bool strict);
  /* Allocates pk and initializes it with a default size */
void pk_internal_free(pk_internal_t* pk);
  /* Clear and free pk */
void pk_internal_realloc_lazy(pk_internal_t* pk, size_t maxdims);
  /* Reallocate pk only if a bigger dimension is required */

static inline pk_internal_t* pk_init_from_manager(ap_manager_t* man, ap_funid_t funid);
  /* Initializes some fields of pk from manager */

/* ********************************************************************** */
/* II. Bitstrings */
/* ********************************************************************** */

typedef unsigned int bitstring_t;
typedef struct bitindex_t {
  size_t index;
  size_t word;
  bitstring_t bit;
} bitindex_t;

#define bitstring_size (sizeof(bitstring_t)*8)
#define bitstring_msb (1U<<(bitstring_size-1))


/* Operations on \verb-bitindex_t- */
void bitindex_print(bitindex_t* bi);
void bitindex_fprint(FILE* stream, bitindex_t* bi);
bitindex_t bitindex_init(size_t col);
void bitindex_inc(bitindex_t*);
void bitindex_dec(bitindex_t*);
size_t bitindex_size(size_t n);

/* Operations on \verb-bitstring_t- */
bitstring_t* bitstring_alloc(size_t n);
bitstring_t* bitstring_realloc(bitstring_t* b, size_t n);
void bitstring_free(bitstring_t* b);
void bitstring_clear(bitstring_t* b, size_t size);
void bitstring_copy(bitstring_t* b2, bitstring_t* b1, size_t size);
int bitstring_cmp(bitstring_t* r1, bitstring_t* r2, size_t size);

void bitstring_print(bitstring_t* b, size_t size);
void bitstring_fprint(FILE* stream, bitstring_t* b, size_t size);

int bitstring_get(bitstring_t* b, bitindex_t ix);
void bitstring_set(bitstring_t* b, bitindex_t ix);
void bitstring_clr(bitstring_t* b, bitindex_t ix);

/* ********************************************************************** */
/* III. Saturation matrices */
/* ********************************************************************** */

typedef struct satmat_t {
  /* public part */
  bitstring_t** p;
  size_t nbrows;
  size_t nbcolumns;
  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
} satmat_t;


satmat_t* satmat_alloc(size_t nbrows, size_t nbcols);
void satmat_resize_rows(satmat_t* sat, size_t nbrows);
void satmat_resize_cols(satmat_t* sat, size_t nbcols);
satmat_t* satmat_copy_resize_cols(satmat_t* sat, size_t nbcols);
void satmat_free(satmat_t* sat);
void satmat_clear(satmat_t* sat);
satmat_t* satmat_copy(satmat_t* sat);
void satmat_print(satmat_t* sat);
void satmat_fprint(FILE* stream, satmat_t* sat);

bitstring_t satmat_get(satmat_t* sat, size_t i, bitindex_t jx);
void satmat_set(satmat_t* sat, size_t i, bitindex_t jx);
void satmat_clr(satmat_t* sat, size_t i, bitindex_t jx);

satmat_t* satmat_transpose(satmat_t* org, size_t nbcols);

void satmat_exch_rows(satmat_t* sat, size_t l1, size_t l2);
void satmat_move_rows(satmat_t* sat, size_t destrow, size_t orgrow, size_t size);


/* ********************************************************************** */
/* IV. Vectors */
/* ********************************************************************** */

/* Basic Operations */
numintMPQ_t*	vector_alloc(size_t size);
void		vector_realloc(numintMPQ_t** q, size_t size, size_t nsize);
void		vector_free(numintMPQ_t* q, size_t size);
void		vector_clear(numintMPQ_t* q, size_t size);
void		vector_copy(numintMPQ_t* q1, numintMPQ_t* q2, size_t size);
void		vector_print(numintMPQ_t* q, size_t size);

/* Normalization */
bool vector_normalize(pk_internal_t* pk, numintMPQ_t* q, size_t size);
bool vector_normalize_expr(pk_internal_t* pk, numintMPQ_t* q, size_t size);
bool vector_normalize_constraint(pk_internal_t* pk,
				 numintMPQ_t* q, ap_dimension_t dim);
bool vector_normalize_constraint_int(pk_internal_t* pk,
				     numintMPQ_t* q, ap_dimension_t dim);

/* Comparison */
int vector_compare(pk_internal_t* pk,
		   numintMPQ_t* q1, numintMPQ_t* q2, size_t size);

/* Combination and Algebraic Operations */
void vector_combine(pk_internal_t* pk,
		    numintMPQ_t* q1, numintMPQ_t* q2,
		    numintMPQ_t* q3, size_t k, size_t size);
void vector_product(pk_internal_t* pk,
		    numintMPQ_t prod,
		    numintMPQ_t* q1, numintMPQ_t* q2, size_t size);
void vector_product_strict(pk_internal_t* pk,
			   numintMPQ_t prod,
			   numintMPQ_t* r1, numintMPQ_t* r2, size_t size);

/* Predicates that can be useful for users */
bool vector_is_null(pk_internal_t* pk, numintMPQ_t* q, size_t size);
bool vector_is_null_strict(pk_internal_t* pk, numintMPQ_t* q, size_t size);
bool vector_is_positivity_constraint(pk_internal_t* pk,numintMPQ_t* q,size_t size);
bool vector_is_dummy_constraint(pk_internal_t* pk, numintMPQ_t* q, size_t size);
bool vector_is_dummy_or_strict_generator(pk_internal_t* pk,
					 numintMPQ_t* q, size_t size);
bool vector_is_integer(pk_internal_t* pk, numintMPQ_t* vec, ap_dimension_t dim);
long vector_hash(pk_internal_t* pk, numintMPQ_t* vec, size_t size);
/* Functions meant to be internal */
numintMPQ_t* _vector_alloc_int(size_t size);
void vector_gcd(pk_internal_t* pk, numintMPQ_t* q, size_t size, numintMPQ_t gcd);

/* ********************************************************************** */
/* V. Matrices */
/* ********************************************************************** */

typedef struct matrix_t {
  /* public part */
  numintMPQ_t** p;     /* array of pointers to rows */
  size_t nbrows;      /* number of effective rows */
  size_t nbcolumns;   /* size of rows */

  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
  bool _sorted;
} matrix_t;

/* Normal functions */

/* information about private part */
static inline size_t matrix_get_maxrows(matrix_t* mat)
{ return mat->_maxrows; }
static inline bool matrix_is_sorted(matrix_t* mat)
{ return mat->_sorted; }

/* Basic Operations */
matrix_t* matrix_alloc(size_t nbrows, size_t nbcols, bool s);
void      matrix_resize_rows(matrix_t* mat, size_t nbrows);
void      matrix_resize_rows_lazy(matrix_t* mat, size_t nbrows);
void      matrix_minimize(matrix_t* mat);
void      matrix_free(matrix_t* mat);
void      matrix_clear(matrix_t* mat);
void      matrix_print(matrix_t* mat);
void      matrix_fprint(FILE* stream, matrix_t* mat);
matrix_t* matrix_copy(matrix_t* mat);
bool      matrix_equal(matrix_t* mata, matrix_t* matb);

/* Operations on rows */
void matrix_normalize_row(pk_internal_t* pk,
			  matrix_t* mat, size_t l);
void matrix_combine_rows(pk_internal_t* pk,
			 matrix_t* mat, size_t l1, size_t l2, size_t l3, size_t k);
int matrix_compare_rows(pk_internal_t* pk,
			matrix_t* mat, size_t l1, size_t l2);
void matrix_exch_rows(matrix_t* mat, size_t l1, size_t l2);
void matrix_move_rows(matrix_t* mat, size_t destrow, size_t orgrow, size_t size);

/* Normalization of rows */
bool matrix_normalize_constraint(pk_internal_t* pk,
				   matrix_t* mat, 
				   ap_dimension_t dim);
bool matrix_normalize_constraint_int(pk_internal_t* pk,
				       matrix_t* mat, 
				       ap_dimension_t dim);

/* Sorting & Merging */
void matrix_sort_rows(pk_internal_t* pk,
		      matrix_t* mat);
void matrix_sort_rows_with_sat(pk_internal_t* pk,
			       matrix_t* mat, satmat_t* sat);

matrix_t* matrix_append(matrix_t* ma, matrix_t* mb);
void matrix_append_with(matrix_t* ma, matrix_t* mb);
void matrix_revappend_with(matrix_t* ma, matrix_t* mb);

matrix_t* matrix_merge_sort(pk_internal_t* pk,
			    matrix_t* ma, matrix_t* mb);
void matrix_merge_sort_with(pk_internal_t* pk,
			    matrix_t* mat, matrix_t* cmat);


/* Predicates that can be useful for users */
static inline 
bool matrix_is_row_dummy_constraint(pk_internal_t* pk,
				    matrix_t* mat,
				    size_t l)
{
  return vector_is_dummy_constraint(pk,
				    mat->p[l],mat->nbcolumns);
}


/* Functions meant to be internal */
matrix_t* _matrix_alloc_int(size_t nr, size_t nc, bool s);

/* ********************************************************************** */
/* VI. Other prototypes */
/* ********************************************************************** */

/* Minimization function, in the sense of minimized dual representation This
   function minimizes if not already done the given polyhedron.
   Raise an exception, but still transmit it (pk->exn not reseted).
*/
void poly_chernikova(ap_manager_t* man, pk_t* poly, char* msg);

/* Same as poly_chernikova, but in addition ensure normalized epsilon
   constraints. */
void poly_chernikova2(ap_manager_t* man, pk_t* poly, char* msg);

/* Same as poly_chernikova2, but in addition normalize matrices by Gauss
   elimination and sorting */
void poly_chernikova3(ap_manager_t* man, pk_t* poly, char* msg);

/* Put the polyhedron with minimized constraints and frames.  If in addition
   the integer man->option->canonicalize.algorithm is strictly positive,
   normalize equalities and lines, and also strict constraints */
void poly_canonicalize(ap_manager_t* man, pk_t* poly);

/* Has the polyhedron normalized strict constraints ? */
static inline bool poly_is_conseps(pk_internal_t* pk, pk_t* po);

/* Is the polyhedron under (strong) normal form ? */
bool pk_is_canonical(ap_manager_t* man, pk_t* po);

/* Make available the matrix of constraints (resp. frames). The matrix will
   remain unavailable iff the polyhedron appears to be empty */
static inline void poly_obtain_C(ap_manager_t* man, pk_t* po, char* msg);
static inline void poly_obtain_F(ap_manager_t* man, pk_t* po, char* msg);

/* Assuming the the matrix of constraints (resp. frames) is available, sort it,
   and take care of the saturation matrices. */
void poly_obtain_sorted_C(pk_internal_t* pk, pk_t* poly);
void poly_obtain_sorted_F(pk_internal_t* pk, pk_t* poly);

/* Assuming one of the saturation matrix is available, make satC (resp. satF)
   available. */
static inline void poly_obtain_satC(pk_t* poly);
static inline void poly_obtain_satF(pk_t* poly);

/* Exchange C and F, sat C and satF, nbeq and nbline */
static inline void poly_dual(pk_t* po);

/* Like poly_chernikova, etc, but if usual is false, take into account the
   fact that poly_dual has been applied */ 
void poly_chernikova_dual(ap_manager_t* man, pk_t* poly,char* msg, bool usual);
static inline 
void poly_obtain_C_dual(ap_manager_t* man,pk_t* po, char* msg, bool usual);
static inline 
void poly_obtain_F_dual(ap_manager_t* man,pk_t* po, char* msg, bool usual);

/* Perform rather detailed and costly checks on a polyhedron, to detect
   inconsistencies */
bool poly_check(pk_internal_t* pk, pk_t* po);
/* Same as previous function, but usual==false mean that poly_dual has been
   applied */
bool poly_check_dual(pk_internal_t* pk, pk_t* po, bool usual);

/* Allocates a polyedron and fills its structure with null values, which
   corresponds to a bottom polyhedron. */
pk_t* poly_alloc(ap_dimension_t dim);

/* Free all the members of the polyhedron structure (GMP semantics) */
void poly_clear(pk_t* po);

/*  Assignement with GMP semantics */
void poly_set(pk_t* pa, pk_t* pb);

/* Fill the first (pk->dec-1) rows of the matrix with the constraints of the
   universe polyhedron */
void matrix_fill_constraint_top(pk_internal_t* pk, matrix_t* C, size_t start);

/* Assign with GMP semantics the given polyhedron with the empty
   (resp. universe) polyhedron, of same dimensions */
void poly_set_bottom(pk_internal_t* pk, pk_t* po);
void poly_set_top(pk_internal_t* pk, pk_t* po);

/*
  F is suppposed to be a valid matrix of ray (i.e., corresponding to a non
  empty polyhedron.
  
  The epsilon component of the constraint is not taken into account.  The
  constraint is considered as strict only if the is_strict paramater telles
  so.  This enables to test the satisfiability of a strict constraint in
  non-strict mode for the library.
*/
bool do_generators_sat_vector(
    pk_internal_t* pk, matrix_t* F, numintMPQ_t* tab, bool is_strict
);

/* Bounding by a itv box a matrix of generators. */
void matrix_to_box(pk_internal_t* pk, eitvMPQ_t* res, matrix_t* F);

/* Bounding the value of a dimension in a matrix of generators.
   mode == 1: sup bound
   mode == -1: inf bound
*/
void matrix_bound_dimension(
    pk_internal_t* pk, eitvMPQ_t eitv, ap_dim_t dim, matrix_t* F
);

/* Bounding the value of a linear expression (vector) in a matrix of
   generators.  vec is supposed to be of size F->nbcolumns.
*/
void matrix_bound_vector(
    pk_internal_t* pk, eitvMPQ_t itv, numintMPQ_t* vec,matrix_t* F
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
bool poly_meet_matrix(
    bool meet, bool lazy, ap_manager_t* man, pk_t* po, pk_t* pa, matrix_t* mat
);
void poly_meet_ap_linconsMPQ_array(
    bool lazy, ap_manager_t* man, pk_t* po, pk_t* pa, ap_linconsMPQ_array_t array
);
void poly_meet(
    bool meet, bool lazy, ap_manager_t* man, pk_t* po, pk_t* pa, pk_t* pb
);

void vector_remove_dimensions(
    pk_internal_t* pk, numintMPQ_t* newq, numintMPQ_t* q, size_t size,
    ap_dimchange_t* dimchange
);
matrix_t* matrix_add_dimensions(
    pk_internal_t* pk, bool destructive, matrix_t* mat,
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

bool vector_set_dim_bound(
    pk_internal_t* pk,
    numintMPQ_t* vec, ap_dim_t dim, numMPQ_t numrat,
    int mode, ap_dimension_t dimen, bool integer);

bool vector_set_linexpr_bound(
    pk_internal_t* pk,
    numintMPQ_t* vec,numintMPQ_t* vec2,numMPQ_t numrat,int mode,
    ap_dimension_t dim, bool integer);

/* Fills the vector with the quasi-linear expression (ap_linexprMPQ) */
void vector_set_ap_linexprMPQ(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_linexprMPQ_t expr, ap_dimension_t dim, int mode);

/* Fills the vector(s) with the linear constraint cons */
void vector_set_ap_linconsMPQ(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_linconsMPQ_t cons, ap_dimension_t dim, bool integer);

/* Fills the vector(s) with the linear constraint cons for testing
   satisfiability. Returns false if unsatisfiable
 */
bool vector_set_ap_linconsMPQ_sat(
    pk_internal_t* pk, numintMPQ_t* vec,
    ap_linconsMPQ_t cons, ap_dimension_t dim, bool integer);

/* From APRON to PK */
/* Fills the vector with the generator gen. */
bool vector_set_ap_lingen0(
    pk_internal_t* pk, numintMPQ_t* vec, ap_lingen0_t gen, 
    ap_dimension_t dim
);
bool matrix_set_ap_lingen0_array(
    pk_internal_t* pk, matrix_t** matrix, ap_lingen0_array_t array,
    ap_dimension_t dim
);
/* From ITV to PK */
bool matrix_set_ap_linconsMPQ_array(
    pk_internal_t* pk, matrix_t** mat, ap_linconsMPQ_array_t array,
    ap_dimension_t dim, bool integer
);
/* From PK to APRON */
bool lincons0_set_vector(pk_internal_t* pk, ap_lincons0_t cons, numintMPQ_t* q, size_t size);
bool lingen0_set_vector(pk_internal_t* pk, ap_lingen0_t gen, numintMPQ_t* q, size_t size);

/* ********************************************************************** */
/* ********************************************************************** */
/* INLINE DEFINITIONS */
/* ********************************************************************** */

static inline pk_internal_t* pk_init_from_manager(ap_manager_t* man, ap_funid_t funid)
{
  pk_internal_t* pk = (pk_internal_t*)man->internal;
  pk->funid = funid;
  pk->funopt = &man->option.funopt[funid];
  man->result.flag_exact = man->result.flag_best = false;
  return pk;
}

static inline bool poly_is_conseps(pk_internal_t* pk, pk_t* po)
{
  return (!pk->strict || 
	  po->dim.reald==0 || 
	  (po->status & pk_status_conseps));;
}
  static inline bool poly_is_minimaleps(pk_internal_t* pk, pk_t* po)
{
  return (!pk->strict ||
	  po->dim.reald==0 ||
	  (po->status & pk_status_minimaleps));
}

static inline void poly_obtain_C(ap_manager_t* man, pk_t* po, char* msg)
{
  if (!po->C) poly_chernikova(man,po,msg);
}
static inline void poly_obtain_F(ap_manager_t* man, pk_t* po, char* msg)
{
  if (!po->F) poly_chernikova(man,po,msg);
}
static inline void poly_obtain_satF(pk_t* poly)
{
  if (!poly->satF){
    assert(poly->C && poly->satC);
    poly->satF = satmat_transpose(poly->satC,poly->C->nbrows);
  }
}
static inline void poly_obtain_satC(pk_t* poly)
{
  if (!poly->satC){
    assert(poly->F && poly->satF);
    poly->satC = satmat_transpose(poly->satF,poly->F->nbrows);
  }
}

static inline void poly_dual(pk_t* po)
{
  void* ptr;
  size_t nb;
  ptr = po->C; po->C = po->F; po->F = ptr;
  ptr = po->satC; po->satC = po->satF; po->satF = ptr;
  nb = po->nbeq; po->nbeq = po->nbline; po->nbline = nb;
}

static inline void poly_obtain_C_dual(ap_manager_t* man, pk_t* po, char* msg, bool usual)
{
  if (!po->C) poly_chernikova_dual(man,po,msg,usual);
}
static inline void poly_obtain_F_dual(ap_manager_t* man, pk_t* po, char* msg, bool usual)
{
  if (!po->F) poly_chernikova_dual(man,po,msg,usual);
}

pk_t* poly_asssub_linexprMPQ_det(
    bool assign, ap_manager_t* man, bool destructive,
    pk_t* pa, ap_dim_t dim, ap_linexprMPQ_t linexpr
);
pk_t* poly_asssub_linexprMPQ_array_det(
    bool assign, ap_manager_t* man, bool destructive,
    pk_t* pa, ap_dim_t* tdim, ap_linexprMPQ_array_t texpr
);
  
#ifdef __cplusplus
}
#endif

#endif
