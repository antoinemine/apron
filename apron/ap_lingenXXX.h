/* ********************************************************************** */
/* ap_lingenXXX.h */
/* ********************************************************************** */

#ifndef _AP_LINGENXXX_H_
#define _AP_LINGENXXX_H_

#include "num_types.h"
#include "ap_linexprXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ap_gentyp_t_
#define _ap_gentyp_t_
/* Datatype for type of generators */
typedef enum ap_gentyp_t {
  AP_GEN_LINE,   /* Bidirectional ray (real coefficient) */
  AP_GEN_RAY,    /* Ray (positive real coefficient) */
  AP_GEN_VERTEX, /* Vertex ("barycenter" coefficient) */
  AP_GEN_LINEMOD,/* Bidirectionnal integer ray (integer coefficient) */
  AP_GEN_RAYMOD  /* Integer ray (positive integer coefficient) */
} ap_gentyp_t;
#endif

/* Linear generator */
typedef struct ap_lingenXXX_struct {
  ap_linexprXXX_t linexpr;
  ap_gentyp_t gentyp;
} ap_lingenXXX_struct;
typedef ap_lingenXXX_struct ap_lingenXXX_t[1];
typedef ap_lingenXXX_struct* ap_lingenXXX_ptr;

/* Array of linear generators */
typedef struct ap_lingenXXX_array_struct {
  ap_lingenXXX_t* p;
  size_t size;
} ap_lingenXXX_array_struct;
typedef ap_lingenXXX_array_struct ap_lingenXXX_array_t[1];
typedef ap_lingenXXX_array_struct* ap_lingenXXX_array_ptr;

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void ap_lingenXXX_init(ap_lingenXXX_t gen,size_t size);
static inline void ap_lingenXXX_init_set(ap_lingenXXX_t res, ap_lingenXXX_t gen);
static inline void ap_lingenXXX_init_set_linexpr(ap_lingenXXX_t res, ap_linexprXXX_t expr, ap_gentyp_t gentyp);
static inline void ap_lingenXXX_resize(ap_lingenXXX_t expr, size_t size);
static inline void ap_lingenXXX_minimize(ap_lingenXXX_t e);
static inline void ap_lingenXXX_clear(ap_lingenXXX_t gen);

ap_lingenXXX_ptr ap_lingenXXX_alloc(size_t size);
ap_lingenXXX_ptr ap_lingenXXX_alloc_set(ap_lingenXXX_t gen);
ap_lingenXXX_ptr ap_lingenXXX_alloc_set_linexpr(ap_linexprXXX_t expr, ap_gentyp_t gentyp);
void ap_lingenXXX_free(ap_lingenXXX_ptr gen);

void ap_lingenXXX_fprint(FILE* stream, ap_lingenXXX_t gen, char** name);
static inline void ap_lingenXXX_print(ap_lingenXXX_t gen, char** name);
static inline void ap_lingenXXX_swap(ap_lingenXXX_t a, ap_lingenXXX_t b);

void ap_lingenXXX_array_init(ap_lingenXXX_array_t array, size_t size);
void ap_lingenXXX_array_init_set(ap_lingenXXX_array_t res, ap_lingenXXX_array_t array);
void ap_lingenXXX_array_set(ap_lingenXXX_array_t res, ap_lingenXXX_array_t array);
void ap_lingenXXX_array_resize(ap_lingenXXX_array_t array, size_t size);
void ap_lingenXXX_array_minimize(ap_lingenXXX_array_t array);
void ap_lingenXXX_array_clear(ap_lingenXXX_array_t array);

ap_lingenXXX_array_ptr ap_lingenXXX_array_alloc(size_t size);
ap_lingenXXX_array_ptr ap_lingenXXX_array_alloc_set(ap_lingenXXX_array_t a);
void ap_lingenXXX_array_free(ap_lingenXXX_array_ptr array);

void ap_lingenXXX_array_fprint(FILE* stream, ap_lingenXXX_array_t array, char** name);
static inline void ap_lingenXXX_array_print(ap_lingenXXX_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests and Simplifications */
/* ********************************************************************** */

static inline bool ap_lingenXXX_is_integer(ap_lingenXXX_t gen, size_t intdim);
static inline bool ap_lingenXXX_is_linear(ap_lingenXXX_t gen);
  /* Return true iff all involved coefficients are scalars */
bool ap_lingenXXX_array_is_linear(ap_lingenXXX_array_t array);

static inline size_t ap_lingenXXX_supportinterval(ap_lingenXXX_t gen, ap_dim_t* tdim);
size_t ap_lingenXXX_array_supportinterval(ap_lingenXXX_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem, with maxdim1 indicating the maximum possible dimension plus 1 */

/* ********************************************************************** */
/* V. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
static inline
void ap_lingenXXX_add_dimensions(ap_lingenXXX_t res,
				ap_lingenXXX_t gen,
				ap_dimchange_t* dimchange);
void ap_lingenXXX_array_add_dimensions(ap_lingenXXX_array_t res,
				      ap_lingenXXX_array_t array,
				      ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the generator should just be less
   than the size of the permutation. */
static inline
void ap_lingenXXX_permute_dimensions(ap_lingenXXX_t res,
				    ap_lingenXXX_t gen,
				    ap_dimperm_t* perm);
void ap_lingenXXX_array_permute_dimensions(ap_lingenXXX_array_t res,
					  ap_lingenXXX_array_t array,
					  ap_dimperm_t* dimchange);
static inline
void ap_lingenXXX_extend_environment(ap_lingenXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_lingenXXX_t expr,
				      ap_environment_t* env);
void ap_lingenXXX_array_extend_environment(ap_lingenXXX_array_t res,
					    bool* perror,
					    ap_environment_t* nenv,
					    ap_lingenXXX_array_t expr,
					    ap_environment_t* env);

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

static inline int ap_lingenXXX_hash(ap_lingenXXX_t gen);
bool ap_lingenXXX_equal(ap_lingenXXX_t gen1,ap_lingenXXX_t gen2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_lingenXXX_compare(ap_lingenXXX_t gen1, ap_lingenXXX_t gen2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void ap_lingenXXX_init(ap_lingenXXX_t gen,size_t size)
{ ap_linexprXXX_init(gen->linexpr,size); gen->gentyp = AP_GEN_LINE; }
static inline void ap_lingenXXX_set(ap_lingenXXX_t a, ap_lingenXXX_t b)
{ if (a!=b){ ap_linexprXXX_set(a->linexpr,b->linexpr); a->gentyp = b->gentyp; } }
static inline void ap_lingenXXX_init_set(ap_lingenXXX_t a, ap_lingenXXX_t b)
{ assert(a!=b); ap_linexprXXX_init_set(a->linexpr,b->linexpr); a->gentyp = b->gentyp; }
static inline void ap_lingenXXX_init_set_linexpr(ap_lingenXXX_t res, ap_linexprXXX_t expr, ap_gentyp_t gentyp)
{
  ap_linexprXXX_init_set(res->linexpr,expr); res->gentyp = gentyp;
}
static inline void ap_lingenXXX_resize(ap_lingenXXX_t e, size_t size)
{ ap_linexprXXX_resize(e->linexpr,size); }
static inline void ap_lingenXXX_minimize(ap_lingenXXX_t e)
{ ap_linexprXXX_minimize(e->linexpr); }
static inline void ap_lingenXXX_clear(ap_lingenXXX_t gen)
{ ap_linexprXXX_clear(gen->linexpr); }
static inline void ap_lingenXXX_print(ap_lingenXXX_t gen, char** name)
{ ap_lingenXXX_fprint(stdout,gen,name); }
static inline void ap_lingenXXX_swap(ap_lingenXXX_t a, ap_lingenXXX_t b)
{ if (a!=b){ ap_lingenXXX_struct t=*a; *a=*b; *b=t; } }
static inline void ap_lingenXXX_array_print(ap_lingenXXX_array_t array, char** name)
{ ap_lingenXXX_array_fprint(stdout,array,name); }

static inline bool ap_lingenXXX_is_integer(ap_lingenXXX_t gen, size_t intdim)
{
  return ap_linexprXXX_is_integer(gen->linexpr,intdim);
}
static inline bool ap_lingenXXX_is_linear(ap_lingenXXX_t gen)
{
  return ap_linexprXXX_is_linear(gen->linexpr);
}
static inline size_t ap_lingenXXX_supportinterval(ap_lingenXXX_t gen, ap_dim_t* tdim)
{
  return ap_linexprXXX_supportinterval(gen->linexpr,tdim);
}

static inline
void ap_lingenXXX_add_dimensions(ap_lingenXXX_t res,
				ap_lingenXXX_t gen,
				ap_dimchange_t* dimchange)
{ ap_linexprXXX_add_dimensions(res->linexpr,gen->linexpr,dimchange); }

static inline
void ap_lingenXXX_permute_dimensions(ap_lingenXXX_t res,
				    ap_lingenXXX_t gen,
				    ap_dimperm_t* dimperm)
{ ap_linexprXXX_permute_dimensions(res->linexpr,gen->linexpr,dimperm); }
static inline
void ap_lingenXXX_extend_environment(ap_lingenXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_lingenXXX_t expr,
				      ap_environment_t* env)
{ ap_linexprXXX_extend_environment(res->linexpr,perror,nenv,expr->linexpr,env); }

static inline
int ap_lingenXXX_hash(ap_lingenXXX_t gen)
{
  return (int)gen->gentyp + ap_linexprXXX_hash(gen->linexpr);
}

#ifdef __cplusplus
}
#endif

#endif
