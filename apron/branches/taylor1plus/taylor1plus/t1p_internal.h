#ifndef _T1P_INTERNAL_H_
#define _T1P_INTERNAL_H_

#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#include "ap_abstract0.h"
#include "itv.h"
#include "mpfr.h"

#include "t1p_itv_utils.h"
#include "t1p_macro_def.h"

extern ap_manager_t* box_manager_alloc(void);
extern ap_manager_t* pk_manager_alloc(bool strict);
extern ap_manager_t* oct_manager_alloc(void);

/**************************************************************************************************/
/* INTERNAL DATA TYPE */
/**************************************************************************************************/

/*****************/
/* Noise symbol */
/****************/
typedef enum nsym_t {
    IN,		/* Classical noise symbol */
    UN,		/* Union noise symbol */
} nsym_t;

/* Noise symbol type */
typedef struct t1p_nsym_t {
    nsym_t	type;		/* type of noise symbol */	/* obsolete */
    size_t	index;		/* global index, noise symbols of the same index are shared */
//    bool	constrained;	/* true if the noise symbol is constrained */
    //ap_dim_t	absdim;		/* the abstract dimension of the epsilon */
} t1p_nsym_t;

/*****************/
/* Taylor1+ term */
/*****************/
/* Taylor1+ affine arithmetic term */
typedef struct _t1p_aaterm_t {
    struct _t1p_aaterm_t*	n;	/* next element */
    t1p_nsym_t*      		pnsym;	/* index of the noise symbol */
    itv_t 			coeff;	/* coeff, encoded as interval */
} t1p_aaterm_t;

/************************/
/* Taylor1+ affine form */
/************************/
struct _t1p_aff_t {
    itv_t		c;	/* center */
    t1p_aaterm_t*	q;	/* first center term (epsilons) aaterm */
    t1p_aaterm_t*	end;	/* quick jump to the last center term : to add a new term for instance */
    t1p_aaterm_t*	lastu;	/* quick jump to the last center term : to add a new term for instance */
    size_t		l;	/* size of q */
    size_t		pby;	/* # pointers to this affine form */
    itv_t		itv;	/* best known interval concretisation */
};
typedef struct _t1p_aff_t t1p_aff_t;

/***************************************/
/* Optimization Problem Data Structure */
/***************************************/
typedef enum indicesSets_t {
    IXY,
    IX,
    IY,
    J
} indicesSets_t;

typedef struct Li {
    int i; /* corresponding index in global tabs */
    itv_t abc[3]; /* coeff of lambda in abc[0], coeff of u0 in abc[1], cst in abc[2] */
} Li;

typedef struct optpr_indexinfo_t {
    int sign; /* tableau de taille n contenant le sign de alphaix - alphaiy */
    indicesSets_t I; /* tableau de taille n indiquant l'indice de chaque eps */
    itv_t alphaixMalphaiy;
    itv_t nsymitvx;
    itv_t nsymitvy;
    itv_t alphaix;
    itv_t alphaiy;
    t1p_nsym_t* pnsym;
} optpr_indexinfo_t; 

/* data structure for feasible point */
typedef struct optpr_point_t {
    itv_t u0;
    itv_t lambda;
    int* ui; /* table of (-1) or (0) or (+1) with size J, if (-1) min{a,b} = a; if (0) min{a,b} = a = b; if (+1), min{a,b} = +1 */
} optpr_point_t;

typedef struct optpr_problem_t {
    itv_t alpha0;
    itv_t beta;	/* |u0| < beta*(1-lambda) */
    itv_t gamma; /* |u0| < gamma*lambda */
    Li* litab; /* table of Li lines and their associated eps */
    optpr_indexinfo_t* T; /* tableau de taille n */
    size_t sizeJ; /* size of litab % détermine la complexité du problème */
    size_t size; /* nb de symboles en tout: le "n" */
    itv_t optval; /* optimal value */
    optpr_point_t optsol; /* optimal point, TODO: pour le moment on prend le dernier qui optimise, mais il peut y en avoir plusieurs */
} optpr_problem_t;

/*******************************/
/* Taylor1+ internal structure */
/*******************************/
typedef struct _t1p_internal_t {
    itv_internal_t*	itv;		/* interval internal representation */
    size_t 		dim;		/* nb of noise symbol used */
    t1p_nsym_t**	epsilon;	/* array of size index of epsilons */
    ap_funid_t 		funid;		/* current function */
    ap_manager_t * 	man;		/* back-pointer */
    ap_manager_t * 	manNS;		/* abstract domain of noise symbols */
    ap_manager_t * 	box;		/* box abstract domain used to compute constraints meet with an hypercube */
    ap_lincons0_array_t	moo;		/* array of constraints -1 <= eps_i <= 1; size = 2index */
    itv_t		muu;		/* [-1,1] (itv_t) */
    ap_interval_t*	ap_muu;		/* [-1,1] (type ap_interval) */
    t1p_aff_t 	*top;		/* top interval */
    t1p_aff_t 	*bot;		/* bottom interval */
    ap_dim_t	*dimtoremove;	/* array to store dimensions to remove after a join */
    ap_dimchange_t*	dimchange;
    ap_abstract0_t*	nsymhypercube;
    clock_t	start;		
    optpr_problem_t* optpr;
} t1p_internal_t;

/***********/
/*** T1+ ***/
/***********/
typedef struct _t1p_t {
    t1p_aff_t**		paf;            /* array of pointers to Taylor1+ expressions of size dims */
    itv_t*		box;		/* reduced product with boxes */
    size_t		intdim;         /* nb of integer variables */
    size_t		dims;           /* intdim + realdim */
    ap_abstract0_t* 	abs;        	/* nsym abstract object */
    ap_dim_t*		nsymcons;       /* array of index of constrained noise symbols */
    ap_interval_t**	gamma;		/* pointer to an array which contains the concretisations of constrained noise symbols if any */
    size_t		size;		/* size of nsymcons and gamma */
    bool		hypercube;	/* true if no constrained nsym */
} t1p_t;

/* special object to store and compute meet with lincons */
typedef struct _obj {
  //  size_t index;
    itv_t itv;
    itv_t coeff;
} obj;

/**************************************************************************************************/
/* Functions prototypes  */
/**************************************************************************************************/

/*****************/
/* Noise symbol */
/****************/
/* memory allocation of a noise symbol (using pr->obs) */
inline static t1p_nsym_t* t1p_nsym_add(t1p_internal_t *pr, nsym_t type);
//inline static t1p_nsym_t* t1p_nsym_alloc(t1p_internal_t *pr, ap_manager_t* man);
//inline static t1p_nsym_t* t1p_nsym_obs_alloc(t1p_internal_t *pr, nsym_t type);

/* free memory used by all noise symbols by freeing only the first element allocated: done when freeing the internal t1p type */
//inline static t1p_nsym_t* t1p_nsym_free(t1p_internal_t *pr, t1p_nsym_t* nsym);
//inline static void t1p_nsym_obs_free(t1p_internal_t *pr);

/* Pretty print a noise Symbol */
inline static void t1p_nsym_fprint(FILE* stream, t1p_nsym_t *eps);
//inline static void t1p_nsym_sprint(char* string, t1p_nsym_t *eps);

/*****************/
/* Taylor1+ term */
/*****************/
//inline static t1p_nsym_t* t1p_nsym_init(t1p_internal_t* pr);
static inline t1p_aaterm_t* t1p_aaterm_alloc_init(void);

/* Free used memory of one linterm. */
static inline void t1p_aaterm_free(t1p_internal_t* pr, t1p_aaterm_t* term);

/* free memory used by a chained list starting from one element (for internal use) */
static inline void t1p_aaterm_list_free(t1p_internal_t *pr, t1p_aaterm_t* head);

/* Pretty print aaterm */
static inline void t1p_aaterm_fprint(t1p_internal_t *pr, FILE* stream, t1p_aaterm_t *ptr);

/************************/
/* Taylor1+ affine form */
/************************/
//t1p_aff_t* t1p_aff_alloc_init(t1p_internal_t *pr);
static t1p_aff_t* t1p_aff_alloc_init(t1p_internal_t *pr)
{
    t1p_aff_t* a = (t1p_aff_t*)malloc(sizeof(t1p_aff_t));
    itv_init(a->c);
    a->q = NULL;
    a->end = NULL;
    a->l = 0;
    a->lastu = NULL;
    a->pby = 0;
    itv_init(a->itv);
    return a;
}

static inline t1p_aff_t * t1p_aff_top(t1p_internal_t* pr);
static inline t1p_aff_t * t1p_aff_bottom(t1p_internal_t* pr);

static inline void t1p_aff_init(t1p_internal_t *pr, t1p_aff_t *a);
static inline void t1p_aff_clear(t1p_internal_t *pr, t1p_aff_t *exp);
static inline void t1p_aff_free(t1p_internal_t *pr, t1p_aff_t *a);
static inline void t1p_aff_check_free(t1p_internal_t *pr, t1p_aff_t *a);
static inline bool t1p_aff_is_zero(t1p_internal_t *pr, t1p_aff_t *a);
static inline bool t1p_aff_is_known_to_be_zero(t1p_internal_t *pr, t1p_aff_t *a);

static inline void t1p_aff_copy(t1p_internal_t *pr, t1p_aff_t *dst, t1p_aff_t *src);
/* create a new noise symbol with coefficient "coeff" */
static inline void t1p_aff_nsym_create(t1p_internal_t *pr, t1p_aff_t *expr, itv_t coeff, nsym_t type);

/* add an existing noise symbol pointed by "nsym" with coefficient "coeff" */
static inline void t1p_aff_add_itv(t1p_internal_t* pr, t1p_aff_t *expr, itv_t itv, nsym_t type);
static inline void t1p_aff_build(t1p_internal_t *pr, t1p_aff_t* expr, itv_t coeff, size_t index);

static inline bool t1p_aff_is_eq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b);
/* test */
static inline bool t1p_aff_is_leq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b, itv_t* gammaa, itv_t* gammab);
static inline bool t1p_aff_is_leq_constrained(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b, t1p_t* enva, t1p_t* envb);
static inline bool t1p_aff_gamma_is_leq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b, itv_t* gammaa, itv_t* gammab);
static inline bool t1p_aff_is_top(t1p_internal_t* pr, t1p_aff_t *a);
static inline bool t1p_aff_is_bottom(t1p_internal_t* pr, t1p_aff_t *a);

static inline void t1p_aff_fprint(t1p_internal_t* pr, FILE* stream, t1p_aff_t *expr);
/* reduce all coefficients from intervals to single point, add a noise symbol if needed (insure a garanteed result) */
static inline bool t1p_aff_reduce(t1p_internal_t* pr, t1p_aff_t *expr);
/* boxize an affine form */
static inline void t1p_aff_boxize(t1p_internal_t* pr, itv_t res, t1p_aff_t *expr, t1p_t* a);
static inline void t1p_aff_bound(t1p_internal_t* pr, itv_t res, t1p_aff_t *expr, t1p_t* a);

/* computes expr-> + Sum(|expr->q_i->coeff|) */
static inline ap_linexpr0_t* t1p_ap_linexpr0_set_aff(t1p_internal_t* pr, t1p_aff_t* aff, t1p_t* a);
static inline void t1p_update_nsymcons_gamma(t1p_internal_t* pr, t1p_t* a);

/*********************/
/* Lattice operators */
/*********************/
/* join of two affine forms without constrained noise symbols (arXiv2008) */
static inline t1p_aff_t * t1p_aff_join(t1p_internal_t* pr, t1p_aff_t *exp1, t1p_aff_t *exp2, t1p_t* a, t1p_t* b);
/* join of two constrained affine forms (draft CAV2010) */
static inline t1p_aff_t* t1p_aff_join_constrained1(t1p_internal_t* pr, t1p_aff_t* expA, t1p_aff_t* expB, t1p_t *a, t1p_t *b, t1p_t *res);

static inline t1p_aff_t* t1p_aff_join_constrained6(t1p_internal_t* pr, t1p_aff_t* expA, t1p_aff_t* expB, t1p_t *a, t1p_t *b, t1p_t *res);

/* calcul du mub ligne par ligne. s'attend a une forme + une perturbation intervalle a calculer.
   retourne une forme + la deviation de la nouvelle perturbation.
 */
static inline t1p_aff_t* t1p_aff_join_constrained7(t1p_internal_t* pr, t1p_aff_t* expA, t1p_aff_t* expB, t1p_t *a, t1p_t *b, t1p_t *res);

static inline t1p_aff_t* t1p_aff_join_constrained8(t1p_internal_t* pr, t1p_aff_t* expA, t1p_aff_t* expB, t1p_t *a, t1p_t *b, t1p_t *res);

static inline t1p_aff_t* t1p_aff_widening_constrained6(t1p_internal_t* pr, t1p_aff_t* exp1, t1p_aff_t* exp2, t1p_t* a, t1p_t* b, t1p_t* ab);

/* compute lambda such that x' = x + lambda(cons), lambda is stored into res as an interval */
static inline void t1p_aff_cons_eq_lambda(t1p_internal_t* pr, itv_t* res, t1p_aff_t* x, t1p_aff_t* cons, t1p_t* a);


/*******************************/
/* Taylor1+ internal structure */
/*******************************/
/* managing internal representation of t1p abstract value */
static inline t1p_internal_t* t1p_internal_alloc(ap_manager_t* manNS);
/* free internal */
static inline void t1p_internal_free(t1p_internal_t* a);
/* Initializes some fields of internal t1p structure from manager */
t1p_internal_t* t1p_init_from_manager(ap_manager_t* man, ap_funid_t funid);

/* alloc T1+ noise symbols manager */
//ap_manager_t* t1p_manager_alloc(ap_manager_t* manNS);
ap_manager_t* t1p_manager_alloc(void);

/**************************************************************************************************/
/* Misc */
/**************************************************************************************************/
/* get the high index of noise symbols in use */
int ap_manager_t1p_get_nsym(ap_manager_t* man);
/* actually not used */
void log_init(void* addr, size_t length, int fd);
void log_sync(void* addr, size_t length, int fd);

/* get the dimension of the constrained noise symbol given its index and the T1+ abstract object */
static inline bool t1p_insert_constrained_nsym(t1p_internal_t *pr, ap_dim_t* res, size_t nsymIndex, t1p_t *a);
/* noy yet defined */
static inline void t1p_delete_constrained_nsym(t1p_internal_t *pr, size_t nsymIndex, t1p_t *a);

/* get the dimension of this noise symbol */
/* if true, then the symbol exist and *dim contains its dimension,
   else, *dim contains where to insert it */
static inline bool t1p_nsymcons_get_dimpos(t1p_internal_t * pr, ap_dim_t* dim, size_t nsymIndex, t1p_t* a);
/* get the dimensions of the abstract object of noise symbols */
static inline size_t t1p_nsymcons_get_dimension(t1p_internal_t * pr, t1p_t* a);
/* get the concretisation of a noise symbol given its index and the abstract object */
static inline void t1p_nsymcons_get_gamma(t1p_internal_t * pr, itv_t res, size_t nsymIndex, t1p_t* a);

/**********************************/
/* Optimization Problem Functions */
/**********************************/

void optpr_init(t1p_internal_t* pr);
void optpr_clear(t1p_internal_t* pr);
void optpr_u0_iszero(t1p_internal_t* pr); /* inject u0 = 0 */
void optpr_build(t1p_internal_t* pr, itv_t alphaix, itv_t alphaiy, t1p_nsym_t* pnsym, itv_t nsymItv1, itv_t nsymItv2, indicesSets_t I); /* construire le problème à résoudre */
void optpr_solve(t1p_internal_t* pr, itv_t alpha0x, itv_t alpha0, itv_t midgx, itv_t midgy, itv_t taux, itv_t tauy, t1p_aff_t* res);

/**************************************************************************************************/
/* Definitions of inline functions */
/**************************************************************************************************/
/*
inline static t1p_nsym_t* t1p_nsym_alloc(t1p_internal_t *pr, ap_manager_t* man)
{
    t1p_nsym_t* res;
    ap_interval_t** interval = ap_interval_array_alloc(1);
    ap_interval_set_double(interval[0], -1.0, 1.0);
    res->inputNoiseSymbols = ap_abstract0_of_box(man, 0, 1, interval);
    res->joinNoiseSymbols = ap_abstract0_of_box(man, 0, 1, interval);
    ap_interval_array_free(interval, 1);
    return res;
}
*/
/*
inline static t1p_nsym_t* t1p_nsym_obs_alloc(t1p_internal_t *pr, nsym_t type)
{
    t1p_nsym_t* res;
    if (1+pr->index < pr->MAX) { 
	pr->obs[1+pr->index] = (t1p_nsym_t*)malloc(sizeof(t1p_nsym_t));
    	res = pr->obs[1+pr->index];
    } else {
	pr->obs = realloc(pr->obs, (pr->MAX+1024)*sizeof(t1p_nsym_t*));
	pr->MAX += 1024;
	pr->obs[1+pr->index] = (t1p_nsym_t*)malloc(sizeof(t1p_nsym_t));
    	res = pr->obs[1+pr->index];
    }

    res->type = type;
    switch (type) {
	case NSYM: {res->index = pr->index++; break;}
	case NSYM_ERR: {res->index = pr->index++; break;}
	case NSYM_U: {res->index = pr->index++; break;}
	case NSYM_TMP: {res->index = pr->index++; break;}
	default: fatal("Unknow noise symbol type, aborting.\n");
    }
    return res;
}
*/
/*
inline static t1p_nsym_t* t1p_nsym_tmp_obs_alloc(t1p_internal_t *pr)
{
    t1p_nsym_t* res;
    if (1+pr->indextmp < pr->MAX_TMP) {
	pr->obstmp[1+pr->indextmp] = (t1p_nsym_t*)malloc(sizeof(t1p_nsym_t));
    	res = pr->obstmp[1+pr->indextmp];
    } else {
	pr->obstmp = realloc(pr->obstmp, (pr->MAX_TMP+1024)*sizeof(t1p_nsym_t*));
	pr->MAX_TMP += 1024;
	pr->obstmp[1+pr->indextmp] = (t1p_nsym_t*)malloc(sizeof(t1p_nsym_t));
    	res = pr->obstmp[1+pr->indextmp];
    }
    res->type = NSYM_TMP;
    res->index = pr->indextmp++;
    return res;
}
*/
inline static void t1p_nsym_fprint(FILE* stream, t1p_nsym_t *eps)
{
    switch (eps->type) {
	case IN: fprintf(stream,"(eps%zu)",eps->index); break;
	//case NSYM_ERR: fprintf(stream,"[r%u]",eps->index); break;
	case UN: fprintf(stream,"(eta%zu)",eps->index); break;
	//case NSYM_TMP: fprintf(stream,"[TMP%u]",eps->index); break;
	default: fprintf(stderr,"error: unknown type of noise symbol, aborting...\n"); abort(); break;
    }
}
/*
inline static void t1p_nsym_sprint(char* string, t1p_nsym_t *eps)
{
    switch (eps->type) {
	case NSYM: sprintf(string,"[%u]",eps->index); break;
	case NSYM_ERR: sprintf(string,"[r%u]",eps->index); break;
	case NSYM_U: sprintf(string,"[u%u]",eps->index); break;
	case NSYM_TMP: sprintf(string,"[TMP%u]",eps->index); break;
	default: fprintf(stderr,"error: unknown type of noise symbol, aborting...\n"); abort(); break;
    }
}
*/
/* memory allocation of affine form term: \alpha_i^x\epsilon_i */
static inline t1p_aaterm_t* t1p_aaterm_alloc_init(void)
{
    t1p_aaterm_t* res = (t1p_aaterm_t*)malloc(sizeof(t1p_aaterm_t));
    res->n = NULL;
    res->pnsym = NULL;
    itv_init(res->coeff);
    return res;
}
/* Free memory used by one aaterm */
static inline void t1p_aaterm_free(t1p_internal_t* pr, t1p_aaterm_t* term)
{
    arg_assert(term, abort(););
    term->n = NULL;
    term->pnsym = NULL;
    itv_clear(term->coeff);
    free(term);
}
/* free memory used by a chained list starting from one term */
static inline void t1p_aaterm_list_free(t1p_internal_t *pr, t1p_aaterm_t* head)
{
    t1p_aaterm_t *p,*q;
    p = q = NULL;
    for (p = head; p; p = q) {
	q = p->n;
	t1p_aaterm_free(pr, p);
    }
}
/* Pretty print an aaterm */
static inline void t1p_aaterm_fprint(t1p_internal_t *pr, FILE* stream, t1p_aaterm_t *ptr)
{
    if (itv_is_point(pr->itv,ptr->coeff)) bound_fprint(stream,ptr->coeff->sup);
    else itv_fprint(stream, ptr->coeff);
    fprintf(stream,".");
    t1p_nsym_fprint(stream,ptr->pnsym);
}
static inline t1p_aff_t * t1p_aff_top(t1p_internal_t* pr)
{
    t1p_aff_t* res = t1p_aff_alloc_init(pr);
    itv_set_top(res->c);
    itv_set_top(res->itv);
    return res;
}
static inline t1p_aff_t * t1p_aff_bottom(t1p_internal_t* pr)
{
    t1p_aff_t* res = t1p_aff_alloc_init(pr);
    itv_set_bottom(res->c);
    itv_set_bottom(res->itv);
    return res;
}
static inline bool t1p_aff_is_zero(t1p_internal_t *pr, t1p_aff_t *a)
{
    if (!itv_is_zero(a->c)) return false;
    else if (a->q != NULL) return false;
    else if (a->end != NULL) return false;
    else if (a->l != 0) return false;
    //else if (a->lastu != NULL) return false;
    else if (!itv_is_zero(a->itv)) return false;
    else return true;
}

static inline bool t1p_aff_is_known_to_be_zero(t1p_internal_t *pr, t1p_aff_t *a)
{
    if (itv_is_zero(a->itv)) return true;
    else return false;
}

static inline void t1p_aff_free(t1p_internal_t *pr, t1p_aff_t *a)
{
    if (a->pby) {
	t1p_aff_fprint(pr, stdout, a);
	fatal("You are about to free a used affine form\n");
    } else {
	a->pby = 0;
	itv_clear(a->c);
	if (a->q) t1p_aaterm_list_free(pr, a->q);
	a->q = NULL;
	a->end = NULL;
	a->lastu = NULL;
	a->l = (size_t)0;
    	itv_clear(a->itv);
	free(a);
	a = NULL;
    }
}
static inline void t1p_aff_check_free(t1p_internal_t *pr, t1p_aff_t *a)
{
    if (a->pby) a->pby--;
    if (a->pby == 0) {
	if ((a != pr->top) && (a != pr->bot)) t1p_aff_free(pr, a);
    }
}
static inline void t1p_aff_init(t1p_internal_t *pr, t1p_aff_t *a)
{
    itv_init(a->c);
    a->q = NULL;
    a->end = NULL;
    a->l = 0;
    a->lastu = NULL;
    a->pby = 0;
    itv_init(a->itv);
}

static inline void t1p_aff_clear(t1p_internal_t *pr, t1p_aff_t *a) 
{
    if (a->pby) {
	t1p_aff_fprint(pr, stdout, a);
	fatal("You are about to clear a used affine form\n"); 
    } else {
	a->pby = 0;
	itv_init(a->c);
	if (a->q) t1p_aaterm_list_free(pr, a->q);
	a->q = NULL;
	a->end = NULL;
	a->lastu = NULL;
	a->l = 0;
	itv_init(a->itv);
    }
}

/* allocate memory and copy the src affine form */
static inline void t1p_aff_copy(t1p_internal_t *pr, t1p_aff_t *dst, t1p_aff_t *src) 
{
    dst = t1p_aff_alloc_init(pr);
    t1p_aaterm_t *p,*q;
    itv_set(dst->c, src->c);
    if (src->q) {
	dst->q = q = t1p_aaterm_alloc_init();
	for (p=src->q; p->n; p=p->n) {
	    itv_set(q->coeff, p->coeff);
	    q->pnsym = p->pnsym;
	    if (p->n) {	
		/* continue */
		q->n = t1p_aaterm_alloc_init();
		q = q->n;
	    } else {
		/* last iteration */
		dst->end = q;
	    }
	}
    }
    dst->l = src->l;
    dst->pby = src->pby;
    itv_set(dst->itv, src->itv);
}
/* 
 *  Insert a new element in the array of constrained noise symbols, 
 *  shall be different from all constrained nsym indices.
 *  Assume that a->nsymcons[0] <= nsymIndex <= a->nsymcons[size-1]
 */
static inline t1p_nsym_t* t1p_nsym_add(t1p_internal_t *pr, nsym_t type)
    /* increment the global index of used noise symbols and add the noise symbol in pr->eps */
{
    size_t dim = pr->dim;
    t1p_nsym_t* res;
    /* resize epsilon array */
    if ((dim+1) % 1024 == 0) pr->epsilon = (t1p_nsym_t**)realloc(pr->epsilon, (dim+1024)*sizeof(t1p_nsym_t*));
    res = pr->epsilon[dim] = (t1p_nsym_t*)malloc(sizeof(t1p_nsym_t));
    res->index = dim;
    res->type = type;
    pr->dim++;
    return res; 
}

/* add a new aaterm with a fresh noise symbol to the affine form expr */
static inline void t1p_aff_nsym_create(t1p_internal_t *pr, t1p_aff_t *expr, itv_t coeff, nsym_t type)
{
    t1p_aaterm_t* ptr = t1p_aaterm_alloc_init();
    itv_set(ptr->coeff, coeff);
    ptr->pnsym = t1p_nsym_add(pr, type);
    if (expr->end) expr->end->n = ptr;
    else expr->q = ptr;
    expr->end = ptr;
    expr->l++;
}

/* add a new aaterm to the affine form with an already existing noise symbol (used to build by hand an affine form) */
static inline void t1p_aff_build(t1p_internal_t *pr, t1p_aff_t* expr, itv_t coeff, size_t index)
{
    nsym_t type;
    t1p_aaterm_t* ptr = t1p_aaterm_alloc_init();
    itv_set(ptr->coeff, coeff);
    ptr->pnsym = pr->epsilon[index];
    if (expr->end) expr->end->n = ptr;
    else expr->q = ptr;
    expr->end = ptr;
    /*
    switch (ptr->pnsym->type) {
	case IN: 
	    if (expr->end) expr->end->n = ptr;
	    else expr->q = expr->end = ptr;
	    ptr->n = expr->lastu;
	    expr->end = ptr;
	    break;
	case UN:
	    if (expr->lastu) expr->lastu->n = ptr;
	    else if (expr->end) expr->end->n = ptr;
	    else expr->q = expr->end = expr->end = ptr;
	    ptr->n = NULL;
	    expr->lastu = ptr;
	    break;
	default: fatal("Unknow noise symbol type, aborting.\n");
    }
    */
    expr->l++;
    /*
    t1p_aaterm_t* end;	
    if (expr->q) {
	end = expr->end;
	end->n = t1p_aaterm_alloc_init();	
	itv_set(end->n->coeff,coeff);		
	end->n->pnsym = pr->epsilon[index];			
	expr->end = end->n;				
    } else {
	expr->q = t1p_aaterm_alloc_init();
	itv_set(expr->q->coeff,coeff);
	expr->q->pnsym = pr->epsilon[index];			
	expr->end = expr->q;
    }
    expr->l++;
    */
}

/* convert an itv to an affine form with a fresh noise symbol then add this form to the affine form expr */
static inline void t1p_aff_add_itv(t1p_internal_t* pr, t1p_aff_t *expr, itv_t itv, nsym_t type)
{
    /* itv is a non point interval with finite bounds */
    itv_t mid, dev;
    itv_init(mid); itv_init(dev);
    if (!itv_is_point(pr->itv,itv)) {
	itv_middev(pr->itv, mid, dev, itv);
	itv_add(expr->c, expr->c, mid);
	t1p_aff_nsym_create(pr, expr, dev, type);
    } else itv_add(expr->c, expr->c, itv);
    itv_clear(mid); itv_clear(dev);
}

/* a->itv and b->itv may be different */
static inline bool t1p_aff_is_eq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b)
{
    if (a == b) return true;
    else if (a->l != b->l) return false;
    else if (!itv_is_eq(a->c, b->c)) return false;
    else {
	t1p_aaterm_t *p, *q;
	for (p=a->q, q=b->q; p && q;) {
	    if (p->pnsym != q->pnsym) return false;
	    else if (!itv_is_eq(p->coeff, q->coeff)) return false;
	    else {
		p = p->n;
		q = q->n;
	    }
	}
	return true;
    }
}

/* equality everywhere (used for regression tests) */
static inline bool t1p_aff_is_perfectly_eq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b)
{
    if (a == b) return true;
    else if (a->l != b->l) return false;
    else if (!itv_is_eq(a->c, b->c)) return false;
    else if (!itv_is_eq(a->itv, b->itv)) return false;
    else {
	t1p_aaterm_t *p, *q;
	for (p=a->q, q=b->q; p && q;) {
	    if (p->pnsym != q->pnsym) return false;
	    else if (!itv_is_eq(p->coeff, q->coeff)) return false;
	    else {
		p = p->n;
		q = q->n;
	    }
	}
	return true;
    }
}

/* a = b + [- eps,+eps] (used for regression tests) */
static inline bool t1p_aff_is_tild_eq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b)
{
    bool res = false;
    itv_t tmp;
    itv_init(tmp);
    itv_t tmp1;
    itv_init(tmp1);
    bound_t c;
    bound_init(c);
    bound_set_int(c, 2);
    bound_mul_2exp(c, c, -32); /* c ~= 2,328306e-10 */
    itv_enlarge_bound(tmp, b->c, c);
    itv_enlarge_bound(tmp1, b->itv, c);
    if (a == b) res = true;
    else if (a->l != b->l) res = false;
    else if (!itv_is_leq(a->c, tmp)) res = false;
    else if (!itv_is_leq(a->itv, tmp1)) res = false;
    else {
	t1p_aaterm_t *p, *q;
	for (p=a->q, q=b->q; p && q;) {
	    itv_enlarge_bound(tmp, q->coeff, c);
	    if (p->pnsym != q->pnsym) {res = false;break;}
	    else if (!itv_is_leq(p->coeff, tmp)) {res = false;break;}
	    else {
		p = p->n;
		q = q->n;
	    }
	}
	res = true;
    }
    itv_clear(tmp);
    itv_clear(tmp1);
    bound_clear(c);
    return res;
}

/*
static inline bool t1p_aff_gamma_is_leq(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b, itv_t* gammaa, itv_t* gammab)
{
    bool res;
    itv_t itvA, itvB;
    itv_init(itvA);
    itv_init(itvB);
    t1p_aff_boxize(pr, itvA, a, gammaa);
    t1p_aff_boxize(pr, itvB, b, gammab);
    itv_print(itvA); printf("\t"); itv_print(itvB); printf("\n");
    res = itv_is_leq(itvA, itvB);
    itv_clear(itvA);
    itv_clear(itvB);
    return res;
}
*/

/* a <= b ? : false means either incomaparable or b <= a */
static inline bool t1p_aff_is_leq_constrained(t1p_internal_t* pr, t1p_aff_t *a, t1p_aff_t *b, t1p_t* enva, t1p_t* envb)
{
    bool res = false;
    size_t i = 0;

    if (!b->q) res = itv_is_leq(a->itv, b->c);
    else if (!a->q) res = false;
    else 
      {
	itv_t mid; itv_init(mid);
	itv_t dev; itv_init(dev);
	itv_t c; itv_init(c);
	itv_t d; itv_init(d);
	itv_t c1; itv_init(c1);
	itv_t d1; itv_init(d1);
	itv_t c2; itv_init(c2);
	itv_t d2; itv_init(d2);
	itv_t betaA; itv_init(betaA);
	itv_t betaB; itv_init(betaB);
	itv_t tmp; itv_init(tmp);
	t1p_aaterm_t *p, *q, *ptr;
	itv_sub(c, a->c, b->c);
	if (a->q || b->q) {
	    ptr = t1p_aaterm_alloc_init();
	    for(p = a->q, q = b->q; p || q;) {
		if (p && q) {
		    if (p->pnsym->index == q->pnsym->index) {
			itv_sub(ptr->coeff, p->coeff, q->coeff);
			ptr->pnsym = p->pnsym;
			if (ptr->pnsym->type == UN) {
			    t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			    itv_mul(pr->itv, ptr->coeff, tmp, p->coeff);
			    itv_add(betaA, betaA, ptr->coeff);
			    itv_mul(pr->itv, ptr->coeff, tmp, q->coeff);
			    itv_add(betaB, betaB, ptr->coeff);
			}
			p = p->n ;
			q = q->n ;
		    } else if (p->pnsym->index < q->pnsym->index) {
			itv_set(ptr->coeff, p->coeff);
			ptr->pnsym = p->pnsym;
			if (ptr->pnsym->type == UN) {
			    t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			    itv_mul(pr->itv, ptr->coeff, tmp, p->coeff);
			    itv_add(betaA, betaA, ptr->coeff);
			}
			p = p->n ;
		    } else {
			itv_neg(ptr->coeff, q->coeff);
			ptr->pnsym = q->pnsym;
			if (ptr->pnsym->type == UN) {
			    t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			    itv_mul(pr->itv, ptr->coeff, tmp, q->coeff);
			    itv_add(betaB, betaB, ptr->coeff);
			}
			q = q->n ;
		    }
		} else if (p) {
		    itv_set(ptr->coeff, p->coeff);
		    ptr->pnsym = p->pnsym;
		    if (ptr->pnsym->type == UN) {
			t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			itv_mul(pr->itv, ptr->coeff, tmp, p->coeff);
			itv_add(betaA, betaA, ptr->coeff);
		    }
		    p = p->n ;
		} else {
		    itv_neg(ptr->coeff, q->coeff);
		    ptr->pnsym = q->pnsym;
		    if (ptr->pnsym->type == UN) {
			t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			itv_mul(pr->itv, ptr->coeff, tmp, q->coeff);
			itv_add(betaB, betaB, ptr->coeff);
		    }
		    q = q->n ;
		}
		if (!itv_is_zero(ptr->coeff)) {
		    if (ptr->pnsym->type == IN) {
			t1p_nsymcons_get_gamma(pr, tmp, ptr->pnsym->index, enva);
			itv_mul(pr->itv, tmp, tmp, ptr->coeff);
			itv_add(c, c, tmp);
			/*
			itv_middev(pr->itv, mid, dev, tmp);
			itv_mul(pr->itv, tmp, mid, ptr->coeff);
			itv_add(c, c, tmp);
			itv_abs(ptr->coeff, ptr->coeff);
			itv_mul(pr->itv, tmp, ptr->coeff, dev);
			itv_add(d, d, tmp);
			*/
		    }
		}
	    }
	    t1p_aaterm_free(pr, ptr);
	}
	itv_middev(pr->itv, c1, d1, betaA);
	itv_middev(pr->itv, c2, d2, betaB);
	itv_sub(tmp,c1,c2);
	itv_add(c,c,tmp);
	itv_abs(c,c);
	//itv_add(c,c,d);
	itv_sub(d, d2, d1);
	/* [c] <= ? [d] */
	res = itv_cmp(c,d);

	itv_clear(tmp);
	itv_clear(c);
	itv_clear(d);
	itv_clear(c1);
	itv_clear(d1);
	itv_clear(c2);
	itv_clear(d2);
	itv_clear(mid);
	itv_clear(dev);
	itv_clear(betaA);
	itv_clear(betaB);
      }
    return res;
}

/* pretty print an AA expression: expr/top/bottom */
static inline void t1p_aff_fprint(t1p_internal_t* pr, FILE* stream, t1p_aff_t *expr)
{
    arg_assert(expr, abort(););
    t1p_aaterm_t* p;
    if (itv_is_point(pr->itv,expr->c)) bound_fprint(stream,expr->c->sup);
    else itv_fprint(stream, expr->c);
    /* Print values */
    for (p=expr->q; p; p=p->n) {
	fprintf(stream," + ");
	t1p_aaterm_fprint(pr, stream, p);
    }
    fprintf(stream,"\t;");
    itv_fprint(stream, expr->itv);
    fprintf(stream,"\t");
    fflush(stream);
}
/* reduce the center and coefficients of the central part (C) to smaller intervals and add a new noise symbol */
static inline bool t1p_aff_reduce(t1p_internal_t* pr, t1p_aff_t *expr)
{
    t1p_aaterm_t *p,*t;
    num_t eps;
    itv_t mid, dev, sum, itv;
    bound_t err;
    num_init(eps);
    itv_init(mid);
    itv_init(dev);
    itv_init(sum);
    itv_init(itv);
    bound_init(err);
    bool ok = false;

    if (itv_has_infty_bound(expr->c)) ok = false;
    else {
	num_set_double(eps,(double)5*1.11022302462515654042e-16);	/* threshold : last bit in the mantissa in double precision */
	itv_range_abs(err,expr->c);
	if (bound_cmp_num(err,eps) > 0) {
	    itv_middev(pr->itv, mid, dev, expr->c);
	    itv_set(expr->c, mid);
	    itv_add(sum, sum, dev);
	}
	for(p = expr->q; p; p=p->n) {
	    if (itv_has_infty_bound(p->coeff)) {
		if (itv_is_top(p->coeff)) {
		    /* reduce to top */
		    if (expr->q) t1p_aaterm_list_free(pr, expr->q);
		    expr->q = NULL;
		    expr->end = NULL;
		    expr->lastu = NULL;
		    expr->l = 0;
		    itv_set_top(expr->c);
		    itv_set_top(expr->itv);
		}
		ok = false;
		itv_set_int(sum,0);
		break;
	    } else {
		itv_range_abs(err,p->coeff);
		if (bound_cmp_num(err,eps) > 0) {
		    itv_middev(pr->itv, mid, dev, p->coeff);
		    itv_set(p->coeff, mid);
		    itv_add(sum, sum, dev);
		}
	    }
	}
	if (itv_is_zero(sum)) ok = false;
	else {
	    bound_set(itv->sup, sum->sup);
	    bound_neg(itv->inf, itv->sup);
	    t1p_aff_nsym_create(pr, expr, itv, IN);
	    ok = true; /* adding new symbol */
	}
    }
    num_clear(eps);
    itv_clear(mid);
    itv_clear(dev);
    itv_clear(sum);
    itv_clear(itv);
    bound_clear(err);
    return ok;
}

/* fetch the interval concretisation of constrained symbol from the abstract object, then compute gamma(expr) and store it in res */
static inline void t1p_aff_boxize(t1p_internal_t* pr, itv_t res, t1p_aff_t *expr, t1p_t* a)
{

    /*
    size_t j = 0;
fprintf(stdout, "\n");
fprintf(stdout, "t1p_t* %x\n", (unsigned int)a);
    size_t size = t1p_nsymcons_get_dimension(pr, a);
ap_abstract0_fprint(stdout, pr->manNS, a->abs, 0x0);
for (j=0; j<size; j++) {
    fprintf(stdout, "a->nsymcons[%d]=%d\n", j, a->nsymcons[j]);
fprintf(stdout, "a->gamma[%d] = %x\n", j, (unsigned int)a->gamma[j]);
    ap_interval_fprint(stdout, a->gamma[j]);}
fprintf(stdout, "\n");
*/

    //if (itv_has_infty_bound(expr->c)) {
//	itv_set(res, expr->c);
//	return;
	if (itv_is_top(expr->c)) {
	    itv_set_top(res);
	return;
    } else {
	ap_dim_t dim;
	itv_t tmp;
	itv_t eps_itv; itv_init(eps_itv);
	itv_init(tmp); itv_init(res);
	t1p_aaterm_t* p;
	itv_set(res,expr->c);
	if (a->hypercube) {
	    for (p=expr->q; p; p=p->n) {
		itv_mul(pr->itv, tmp, p->coeff, pr->muu);
		itv_add(res, res, tmp);
	    }
	} else {
	    for (p=expr->q; p; p=p->n) {
		if (t1p_nsymcons_get_dimpos(pr, &dim, p->pnsym->index, a)) {
		    itv_set_ap_interval(pr->itv, eps_itv, a->gamma[dim]);
		    itv_mul(pr->itv, tmp, p->coeff, eps_itv);
		} else {
		    itv_mul(pr->itv, tmp, p->coeff, pr->muu);
		}
		itv_add(res, res, tmp);
	    }
	}
	itv_clear(tmp); itv_clear(eps_itv);
    }
}

static inline void t1p_aff_bound(t1p_internal_t* pr, itv_t res, t1p_aff_t *expr, t1p_t* a)
{
    if (itv_is_top(expr->c)) {
	itv_set_top(res);
	return;
    } else {
	ap_dim_t dim;
	itv_t tmp;
	itv_t eps_itv; itv_init(eps_itv);
	itv_init(tmp);
	t1p_aaterm_t* p;
	itv_set(res,expr->c);
	if (a->hypercube) {
	    for (p=expr->q; p; p=p->n) {
		itv_mul(pr->itv, tmp, p->coeff, pr->muu);
		itv_add(res, res, tmp);
	    }
	} else {
	    ap_linexpr0_t* linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
	    ap_linexpr0_set_cst_scalar_int(linexpr0, (int)0);
	    /* au maximum, tous les symbole de bruit sont contraints */
	    linexpr0->p.linterm = (ap_linterm_t*)malloc(expr->l*sizeof(ap_linterm_t));
	    size_t k = 0;
	    ap_dim_t dim = 0;
	    for (p=expr->q; p; p=p->n) {
		if (t1p_nsymcons_get_dimpos(pr, &dim, p->pnsym->index, a)) {
		    ap_coeff_init(&linexpr0->p.linterm[k].coeff, AP_COEFF_INTERVAL);
		    ap_coeff_set_itv(pr->itv, &linexpr0->p.linterm[k].coeff, p->coeff);
		    linexpr0->p.linterm[k].dim = dim;
		    k++;
		} else {
		    itv_mul(pr->itv, tmp, p->coeff, pr->muu);
		    itv_add(res, res, tmp);
		}
	    }
	    linexpr0->size = k;
	    ap_interval_t* ap_itv = ap_abstract0_bound_linexpr(pr->manNS, a->abs, linexpr0);
	    itv_set_ap_interval(pr->itv, tmp, ap_itv);
	    itv_add(res, res, tmp);
	    linexpr0->p.linterm = (ap_linterm_t*)realloc((void* )linexpr0->p.linterm, k*sizeof(ap_linterm_t));
	    ap_linexpr0_free(linexpr0);
	    ap_interval_free(ap_itv);
	}
	itv_clear(tmp); itv_clear(eps_itv);
    }
}

/* extension of the argmin operator to intervals:
 * - if "a" or "b" contains 0, then the argmin is 0
 * - if "a" is a pos interval and "b" is a neg interval, then the argmin is 0
 * - if "a" is a neg interval and "b" is a pos interval, then the argmin is 0
 * - if "a" and "b" are pos, then choose the one with the minimum inf
 * - if "a" and "b" are neg, then choose the one with the maximum sup
 */
static inline bool argmin(t1p_internal_t* pr, itv_t res, itv_t a, itv_t b)
    /* IN: a, b */
    /* OUT: res, bool */
{
    itv_t zero; itv_init(zero);
    bool ok = false;
    if (itv_is_leq(zero,a) || itv_is_leq(zero,b)) {
	itv_set_int(res,0);
	ok = false;	/* the coefficient is zero */
    }
    else if (itv_is_pos(a) && itv_is_neg(b)) {
	itv_set_int(res,0);
	ok = false;
    } else if (itv_is_neg(a) && itv_is_pos(b)) {
	itv_set_int(res,0);
	ok = false;
    }
    else {
	/* a and b have the same sign */
	if (itv_is_pos(a)) (bound_cmp(a->inf, b->inf) >= 0) ? itv_set(res, a) : itv_set(res, b);
	else (bound_cmp(a->sup, b->sup) >= 0) ? itv_set(res, a) : itv_set(res, b);
	ok = true;
    }
    itv_clear(zero);
    return ok;
}

/* arXiv Goubault Putot 2008 *
 * using argmin for generic and non generic positions.
 * TODO: could do better than argmin
 */
static inline t1p_aff_t * t1p_aff_join(t1p_internal_t* pr, t1p_aff_t *exp1, t1p_aff_t *exp2, t1p_t* a, t1p_t* b)
{
    arg_assert(exp1 && exp2, abort(););
    t1p_aff_t * res = t1p_aff_alloc_init(pr);
    itv_t mid, dev;
    itv_init(mid); itv_init(dev);
    itv_t tmp, abscoeff, beta;
    itv_init(tmp); itv_init(abscoeff); itv_init(beta);
    t1p_aaterm_t *lastu = NULL;
    t1p_aaterm_t *p, *q, *ptr;

    itv_t argminpq; itv_init(argminpq);
    ptr = NULL;
    if (exp1->q && exp2->q) {
	for(p = exp1->q, q = exp2->q; p && q;) {
	    if (p->pnsym->index == q->pnsym->index) {				/* coeff of the same noise symbol */
		if (argmin(pr, argminpq, p->coeff, q->coeff)) {			/* true if the argmin is not zero */
		    if (res->q == NULL) {
			res->q = ptr = t1p_aaterm_alloc_init();			/* allocate the first element of res->q */
			res->l++;
		    }
		    if (exp1->lastu && exp2->lastu) {				/* keep track of the perturbation noise symbol */
			if (exp1->lastu == p && exp2->lastu == q) lastu = ptr;	/* only if exp1->lastu and exp2->lastu resist to argmin formula */
		    } 
		    ptr->pnsym = p->pnsym;						/* set the index of the noise symbol */
		    itv_set(ptr->coeff, argminpq);					/* set the coefficient as the argmin of the two other coefficients */
		}		 
		p = p->n ;
		q = q->n ;
	    } else if (p->pnsym->index < q->pnsym->index) {
		p = p->n ;
	    } else {
		q = q->n ;
	    }
	    if (p && q) {
		if (p->pnsym->index == q->pnsym->index && res->q) {
		    if (argmin(pr, argminpq, p->coeff, q->coeff)) {
			ptr->n = t1p_aaterm_alloc_init();			/* allocate the next element of res->q */
			res->l++;
			ptr=ptr->n;
		    }
		}
	    }
	}
	if (ptr) {
	    ptr->n = NULL;
	    res->end = ptr;
	}
    }

    if (lastu) {
	itv_init(lastu->coeff);
    }
    itv_join(res->itv, exp1->itv, exp2->itv);
    itv_middev(pr->itv, mid, dev, res->itv);
    itv_set(beta, dev);
    itv_set(res->c, mid);
    for (q=res->q; q; q=q->n) {
	itv_abs(abscoeff, q->coeff);
	itv_sub(beta, beta, abscoeff);
    }
    if (itv_is_pos(beta)) {
	t1p_aff_nsym_create(pr, res, beta, UN);
	res->lastu = res->end;
    } else {
	fatal("Join without constrained, Perturbation coeff is not positive\n");
    }
    itv_clear(mid); itv_clear(dev);
    itv_clear(tmp); itv_clear(abscoeff); itv_clear(beta);
    return res;
}

/* version rapide qui calcule l'argmin pour i>= 1 puis complete par alpha0 et tau telque
   - z soit upper bound
   - beta^z soit minimale
 */
static inline t1p_aff_t * t1p_aff_join_constrained6(t1p_internal_t* pr, t1p_aff_t *exp1, t1p_aff_t *exp2, t1p_t* a, t1p_t* b, t1p_t* ab) 
{
    arg_assert(exp1 && exp2, abort(););

    t1p_aff_t * res = t1p_aff_alloc_init(pr);
    itv_t mid, dev, tmp, tmp1, tmp2, betaA, betaB;
    itv_t c1, c2, d1, d2, c, d;
    itv_init(tmp); itv_init(tmp1); itv_init(tmp2);
    itv_init(betaA); itv_init(betaB);
    itv_init(c);
    itv_init(c1);
    itv_init(c2);
    itv_init(d);
    itv_init(d1);
    itv_init(d2);
    itv_init(mid);
    itv_init(dev);
    itv_t nsymItv1; itv_init(nsymItv1);
    itv_t nsymItv2; itv_init(nsymItv2);
    itv_t pmptr; itv_init(pmptr);
    itv_t qmptr; itv_init(qmptr);
    itv_t argminpq; itv_init(argminpq);

    t1p_aaterm_t *p, *q, *ptr;

    itv_join(res->itv, exp1->itv, exp2->itv);
    ptr = NULL;

    if (exp1->q || exp2->q) {
	itv_set(c1, exp1->c);
	itv_set(c2, exp2->c);
	ptr = t1p_aaterm_alloc_init();
	for(p = exp1->q, q = exp2->q; p || q;) {
	    if (p && q) {
		if (p->pnsym->index == q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (p->pnsym->type == IN) {
			argmin(pr, ptr->coeff, p->coeff, q->coeff);
			ptr->pnsym = p->pnsym;
			itv_sub(pmptr, p->coeff, ptr->coeff);
			itv_sub(qmptr, q->coeff, ptr->coeff);
		    }
		    p = p->n ;
		    q = q->n ;
		} else if (p->pnsym->index < q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
		    } else if (p->pnsym->type == IN) {
			itv_set(pmptr,p->coeff);
			itv_set_int(qmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		    p = p->n;
		} else {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		    if (q->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (q->pnsym->type == IN) {
			itv_set(qmptr,q->coeff);
			itv_set_int(pmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		    q = q->n;
		}
	    } else if (p) {
		t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		if (p->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
		    itv_add(betaA, betaA, tmp);
		} else if (p->pnsym->type == IN) {
		    itv_set(pmptr,p->coeff);
		    itv_set_int(qmptr,0);
		}
		t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		p = p->n;
	    } else {
		t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		if (q->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
		    itv_add(betaB, betaB, tmp);
		} else if (q->pnsym->type == IN) {
		    itv_set(qmptr,q->coeff);
		    itv_set_int(pmptr,0);
		}
		t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		q = q->n;
	    }
	    itv_mul(pr->itv, tmp1, pmptr, nsymItv1);
	    itv_add(c1, c1, tmp1);
	    itv_mul(pr->itv, tmp2, qmptr, nsymItv2);
	    itv_add(c2, c2, tmp2);
	    itv_set_int(pmptr,0);
	    itv_set_int(qmptr,0);
	    if (itv_is_zero(ptr->coeff)) {
		if (!(p||q)) {
		    /* the last iteration */
		    t1p_aaterm_free(pr, ptr);
		    if (res->end) res->end->n = NULL;
		}
	    } else {
		/* keep this term */
		if (!res->q) res->q = ptr;
		res->end = ptr;
		res->l++;
		if (p||q) {
		    /* continuing */
		    ptr->n = t1p_aaterm_alloc_init();
		    ptr=ptr->n;
		} else {
		    /* the last iteration */
		}
	    }
	}

	itv_middev(pr->itv, mid, dev, betaA);
	itv_set(d1, dev);
	itv_add(c1, c1, mid);

	itv_middev(pr->itv, mid, dev, betaB);
	itv_set(d2, dev);
	itv_add(c2, c2, mid);

	num_t c0, min, max, dmin, dmax;
	num_init(c0); num_init(min); num_init(max); num_init(dmin); num_init(dmax);
	itv_t beta; itv_init(beta);

	num_t array[4];
	num_init(array[0]);
	num_init(array[1]);
	num_init(array[2]);
	num_init(array[3]);

	num_set(array[0],bound_numref(c1->inf));
	num_neg(array[0],array[0]);
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_neg(array[1],array[1]);
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_add(array[3],array[3],bound_numref(d2->sup));

	int i = 0;
	num_set(dmax, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmax,array[i]) < 0) num_set(dmax, array[i]);

	num_set(array[0],bound_numref(c1->inf));
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_neg(array[2],array[2]);
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_neg(array[3],array[3]);
	num_add(array[3],array[3],bound_numref(d2->sup));

	num_set(dmin, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmin,array[i]) < 0) num_set(dmin, array[i]);

	num_add(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(beta,c0);

	num_sub(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(res->c,c0);

	if (num_cmp(bound_numref(c1->inf),bound_numref(c2->inf)) < 0) {
	    num_set(min, bound_numref(c2->inf));
	} else {
	    num_set(min, bound_numref(c1->inf));
	}
	num_neg(min,min);

	if (num_cmp(bound_numref(c1->sup),bound_numref(c2->sup)) < 0) {
	    num_set(max, bound_numref(c2->sup));
	} else {
	    num_set(max, bound_numref(c1->sup));
	}

	if (num_cmp(c0,min) <= 0 || num_cmp(c0,max) >= 0) {
	    t1p_aff_free(pr, res);
	    itv_sub(tmp,d2,d1);
	    if (itv_is_pos(tmp)) res = exp2;
	    else res = exp1;
	} else t1p_aff_nsym_create(pr, res, beta, UN);

	num_clear(c0); num_clear(min); num_clear(max); num_clear(dmin); num_clear(dmax);
	num_clear(array[0]);
	num_clear(array[1]);
	num_clear(array[2]);
	num_clear(array[3]);
	itv_clear(beta);
    } else {
	t1p_aff_add_itv(pr, res, res->itv, UN);
    }
    itv_clear(tmp); itv_clear(tmp1); itv_clear(tmp2);
    itv_clear(betaA); itv_clear(betaB);
    itv_clear(c);
    itv_clear(c1);
    itv_clear(c2);
    itv_clear(d);
    itv_clear(d1);
    itv_clear(d2);
    itv_clear(nsymItv1);
    itv_clear(nsymItv2);
    itv_clear(argminpq);
    itv_clear(pmptr);
    itv_clear(qmptr);
    itv_clear(mid);
    itv_clear(dev);
    return res;
}

static inline t1p_aff_t * t1p_aff_join_constrained7(t1p_internal_t* pr, t1p_aff_t *exp1, t1p_aff_t *exp2, t1p_t* a, t1p_t* b, t1p_t* ab) 
/* 
   - inputs: (alpha^x, tau^x) ; (alpha^y, tau^y)
   - output: (alpha^z, tau^z)
   - algo: 1/calculer tau^x et tau^y
   	   2/calculer lambda et u_0 (autre procedure)
	   3/deduire les u_i restants
	   4/ecrire le sys d'equation verifie par les alpha^z_i, puis le resoudre (autre procedure)
*/   
{
    arg_assert(exp1 && exp2, abort(););

    t1p_aff_t * res = t1p_aff_alloc_init(pr);

    itv_t mid, dev, tmp, tmp1, tmp2, betaA, betaB;
    itv_t c1, c2, d1, d2, c, d;
    itv_init(tmp); itv_init(tmp1); itv_init(tmp2);
    itv_init(betaA); itv_init(betaB);
    itv_init(c);
    itv_init(c1);
    itv_init(c2);
    itv_init(d);
    itv_init(d1);
    itv_init(d2);
    itv_init(mid);
    itv_init(dev);
    itv_t nsymItv1; itv_init(nsymItv1);
    itv_t nsymItv2; itv_init(nsymItv2);
    itv_t pmptr; itv_init(pmptr);
    itv_t qmptr; itv_init(qmptr);
    itv_t argminpq; itv_init(argminpq);
    itv_t midgx, midgy;

    itv_init(midgx); itv_init(midgy); /* TODO: to clear later */
    /* !!!!!!!!!!!!!!!!! SUPPOSES THAT exp->itv contains always the interval concretisation of the form, not its interval in Reduced Product !!!!!!!!!!!!!!!!!!!!!!!!!! */
    itv_middev(pr->itv, midgx, dev, exp1->itv);
    itv_middev(pr->itv, midgy, dev, exp2->itv);

    itv_t alpha0; itv_init(alpha0); /* TODO: clear alpha0 */

    itv_t(alphaix); itv_t(alphaiy); itv_init(alphaix); itv_init(alphaiy);

    t1p_nsym_t* pnsym = NULL;

    t1p_aaterm_t *p, *q;

    if (exp1->q || exp2->q) {
	optpr_init(pr);
	for(p = exp1->q, q = exp2->q; p || q;) {
	    if (p && q) {
		if (p->pnsym->index == q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			pnsym = NULL;
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (p->pnsym->type == IN) {
			pnsym = p->pnsym;
			itv_set(alphaix, p->coeff);
			itv_set(alphaiy, q->coeff);
		    }
		    p = p->n ;
		    q = q->n ;
		} else if (p->pnsym->index < q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			pnsym = NULL;
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
		    } else if (p->pnsym->type == IN) {
			pnsym = p->pnsym;
			itv_set(alphaix,p->coeff);
			itv_set_int(alphaiy,0);
		    }
		    p = p->n;
		} else {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		    if (q->pnsym->type == UN) {
			pnsym = NULL;
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (q->pnsym->type == IN) {
			pnsym = q->pnsym;
			itv_set_int(alphaix,0);
			itv_set(alphaiy,q->coeff);
		    }
		    q = q->n;
		}
	    } else if (p) {
		t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		if (p->pnsym->type == UN) {
		    pnsym = NULL;
		    itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
		    itv_add(betaA, betaA, tmp);
		} else if (p->pnsym->type == IN) {
		    pnsym = p->pnsym;
		    itv_set(alphaix,p->coeff);
		    itv_set_int(alphaiy,0);
		}
		p = p->n;
	    } else {
		t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		if (q->pnsym->type == UN) {
		    pnsym = NULL;
		    itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
		    itv_add(betaB, betaB, tmp);
		} else if (q->pnsym->type == IN) {
		    pnsym = q->pnsym;
		    itv_set_int(alphaix,0);
		    itv_set(alphaiy,q->coeff);
		}
		q = q->n;
	    }
	    if (pnsym) {
		if (itv_is_point(pr->itv, nsymItv1) && itv_is_point(pr->itv, nsymItv2)) {
		    /* I^{xy} */
		    if (itv_is_eq(nsymItv1,nsymItv2)) {
			/* u0 != 0 */
			//itv_sub(tmp, alphaix, alphaiy); /* alpha_i^x - alpha_i^y */
			//itv_mul(pr->itv, tmp, tmp, nsymItv1); /* (alpha_i^x - alpha_i^y)mid(eps_i^x) */
			//itv_add(alpha0,alpha0,tmp);
			optpr_build(pr, alphaix, alphaiy, pnsym, nsymItv1, nsymItv2, IXY);
		    } else {
			/* u0 = 0 */
			itv_set_int(alpha0,0);
			optpr_u0_iszero(pr);
			// solve a much simpler problem
		    }
		} else if (itv_is_point(pr->itv, nsymItv1)) {
		    /* I^x */
		    //itv_sub(tmp, alphaix, alphaiy); /* alpha_i^x - alpha_i^y */
		    //itv_mul(pr->itv, tmp, tmp, nsymItv1); /* (alpha_i^x - alpha_i^y)mid(eps_i^x) */
		    //itv_add(alpha0,alpha0,tmp);
		    optpr_build(pr, alphaix, alphaiy, pnsym, nsymItv1, nsymItv2, IX);
		} else if (itv_is_point(pr->itv, nsymItv2)) {
		    /* I^y */
		    //itv_sub(tmp, alphaix, alphaiy); /* alpha_i^x - alpha_i^y */
		    //itv_mul(pr->itv, tmp, tmp, nsymItv2); /* (alpha_i^x - alpha_i^y)mid(eps_i^x) */
		    //itv_add(alpha0,alpha0,tmp);
		    optpr_build(pr, alphaix, alphaiy, pnsym, nsymItv1, nsymItv2, IY);
		} else {
		    /* J */
		    //itv_sub(tmp, alphaix, alphaiy); /* alpha_i^x - alpha_i^y */
		    optpr_build(pr, alphaix, alphaiy, pnsym, nsymItv1, nsymItv2, J); /* construire le problème à résoudre */
		}
		pnsym = NULL;
	    }
	}

	itv_middev(pr->itv, mid, dev, betaA);
	itv_t taux; itv_init(taux);
	itv_set(taux, dev);
	itv_t cx; itv_init(cx);
	itv_add(cx, exp1->c, mid);

	itv_middev(pr->itv, mid, dev, betaB);
	itv_t tauy; itv_init(tauy);
	itv_set(tauy, dev);
	itv_t cy; itv_init(cy);
	itv_add(cy, exp2->c, mid);

	itv_sub(tmp, cx, cy);
	itv_clear(cx); itv_clear(cy);
	itv_add(alpha0, alpha0, tmp);

	optpr_solve(pr, exp1->c, alpha0, midgx, midgy, taux, tauy, res);
	itv_clear(taux);
	itv_clear(tauy);

	optpr_clear(pr); pr->optpr = NULL;
    } else {
	t1p_aff_add_itv(pr, res, res->itv, UN);
    }
    itv_clear(tmp); itv_clear(tmp1); itv_clear(tmp2);
    itv_clear(betaA); itv_clear(betaB);
    itv_clear(c);
    itv_clear(c1);
    itv_clear(c2);
    itv_clear(d);
    itv_clear(d1);
    itv_clear(d2);
    itv_clear(nsymItv1);
    itv_clear(nsymItv2);
    itv_clear(argminpq);
    itv_clear(pmptr);
    itv_clear(qmptr);
    itv_clear(mid);
    itv_clear(dev);
    return res;
}

/* mub global */
static inline t1p_aff_t * t1p_aff_join_constrained8(t1p_internal_t* pr, t1p_aff_t *exp1, t1p_aff_t *exp2, t1p_t* a, t1p_t* b, t1p_t* ab) 
{
    arg_assert(exp1 && exp2, abort(););

    t1p_aff_t * res = t1p_aff_alloc_init(pr);
    itv_t mid, dev, tmp, tmp1, tmp2, betaA, betaB;
    itv_t c1, c2, d1, d2, c, d;
    itv_init(tmp); itv_init(tmp1); itv_init(tmp2);
    itv_init(betaA); itv_init(betaB);
    itv_init(c);
    itv_init(c1);
    itv_init(c2);
    itv_init(d);
    itv_init(d1);
    itv_init(d2);
    itv_init(mid);
    itv_init(dev);
    itv_t nsymItv1; itv_init(nsymItv1);
    itv_t nsymItv2; itv_init(nsymItv2);
    itv_t pmptr; itv_init(pmptr);
    itv_t qmptr; itv_init(qmptr);
    itv_t argminpq; itv_init(argminpq);

    t1p_aaterm_t *p, *q, *ptr;

    itv_join(res->itv, exp1->itv, exp2->itv);
    ptr = NULL;

    if (exp1->q || exp2->q) {
	itv_set(c1, exp1->c);
	itv_set(c2, exp2->c);
	ptr = t1p_aaterm_alloc_init();
	for(p = exp1->q, q = exp2->q; p || q;) {
	    if (p && q) {
		if (p->pnsym->index == q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (p->pnsym->type == IN) {
			argmin(pr, ptr->coeff, p->coeff, q->coeff);
			ptr->pnsym = p->pnsym;
			itv_sub(pmptr, p->coeff, ptr->coeff);
			itv_sub(qmptr, q->coeff, ptr->coeff);

		    }
		    p = p->n ;
		    q = q->n ;
		} else if (p->pnsym->index < q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
		    } else if (p->pnsym->type == IN) {
			itv_set(pmptr,p->coeff);
			itv_set_int(qmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		    p = p->n;
		} else {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		    if (q->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (q->pnsym->type == IN) {
			itv_set(qmptr,q->coeff);
			itv_set_int(pmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		    q = q->n;
		}
	    } else if (p) {
		t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		if (p->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
		    itv_add(betaA, betaA, tmp);
		} else if (p->pnsym->type == IN) {
		    itv_set(pmptr,p->coeff);
		    itv_set_int(qmptr,0);
		}
		t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		p = p->n;
	    } else {
		t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		if (q->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
		    itv_add(betaB, betaB, tmp);
		} else if (q->pnsym->type == IN) {
		    itv_set(qmptr,q->coeff);
		    itv_set_int(pmptr,0);
		}
		t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		q = q->n;
	    }
	    itv_mul(pr->itv, tmp1, pmptr, nsymItv1);
	    itv_add(c1, c1, tmp1);
	    itv_mul(pr->itv, tmp2, qmptr, nsymItv2);
	    itv_add(c2, c2, tmp2);
	    itv_set_int(pmptr,0);
	    itv_set_int(qmptr,0);
	    if (itv_is_zero(ptr->coeff)) {
		if (!(p||q)) {
		    /* the last iteration */
		    t1p_aaterm_free(pr, ptr);
		    if (res->end) res->end->n = NULL;
		}
	    } else {
		/* keep this term */
		if (!res->q) res->q = ptr;
		res->end = ptr;
		res->l++;
		if (p||q) {
		    /* continuing */
		    ptr->n = t1p_aaterm_alloc_init();
		    ptr=ptr->n;
		} else {
		    /* the last iteration */
		}
	    }
	}

	itv_middev(pr->itv, mid, dev, betaA);
	itv_set(d1, dev);
	itv_add(c1, c1, mid);

	itv_middev(pr->itv, mid, dev, betaB);
	itv_set(d2, dev);
	itv_add(c2, c2, mid);

	num_t c0, min, max, dmin, dmax;
	num_init(c0); num_init(min); num_init(max); num_init(dmin); num_init(dmax);
	itv_t beta; itv_init(beta);

	num_t array[4];
	num_init(array[0]);
	num_init(array[1]);
	num_init(array[2]);
	num_init(array[3]);

	num_set(array[0],bound_numref(c1->inf));
	num_neg(array[0],array[0]);
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_neg(array[1],array[1]);
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_add(array[3],array[3],bound_numref(d2->sup));

	int i = 0;
	num_set(dmax, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmax,array[i]) < 0) num_set(dmax, array[i]);

	num_set(array[0],bound_numref(c1->inf));
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_neg(array[2],array[2]);
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_neg(array[3],array[3]);
	num_add(array[3],array[3],bound_numref(d2->sup));

	num_set(dmin, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmin,array[i]) < 0) num_set(dmin, array[i]);

	num_add(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(beta,c0);

	num_sub(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(res->c,c0);

	if (num_cmp(bound_numref(c1->inf),bound_numref(c2->inf)) < 0) {
	    num_set(min, bound_numref(c2->inf));
	} else {
	    num_set(min, bound_numref(c1->inf));
	}
	num_neg(min,min);

	if (num_cmp(bound_numref(c1->sup),bound_numref(c2->sup)) < 0) {
	    num_set(max, bound_numref(c2->sup));
	} else {
	    num_set(max, bound_numref(c1->sup));
	}

	if (num_cmp(c0,min) <= 0 || num_cmp(c0,max) >= 0) {
	    t1p_aff_free(pr, res);
	    itv_sub(tmp,d2,d1);
	    if (itv_is_pos(tmp)) res = exp2;
	    else res = exp1;
	} else t1p_aff_nsym_create(pr, res, beta, UN);

	num_clear(c0); num_clear(min); num_clear(max); num_clear(dmin); num_clear(dmax);
	num_clear(array[0]);
	num_clear(array[1]);
	num_clear(array[2]);
	num_clear(array[3]);
	itv_clear(beta);
    } else {
	t1p_aff_add_itv(pr, res, res->itv, UN);
    }
    itv_clear(tmp); itv_clear(tmp1); itv_clear(tmp2);
    itv_clear(betaA); itv_clear(betaB);
    itv_clear(c);
    itv_clear(c1);
    itv_clear(c2);
    itv_clear(d);
    itv_clear(d1);
    itv_clear(d2);
    itv_clear(nsymItv1);
    itv_clear(nsymItv2);
    itv_clear(argminpq);
    itv_clear(pmptr);
    itv_clear(qmptr);
    itv_clear(mid);
    itv_clear(dev);
    return res;
}
/* draft CAV 2010 *
 * computes mub with minimal interval concretisation
 */
//static inline t1p_aff_t * t1p_aff_join_constrained1(t1p_internal_t* pr, t1p_aff_t *exp_a, t1p_aff_t *exp_b, t1p_t* a, t1p_t* b, t1p_t* ab) 
//{
//    t1p_aff_t *exp1 = exp_a;
//    t1p_aff_t *exp2 = exp_b;
//    itv_t *gammaa = gamma_a;
//    itv_t *gammab = gamma_b;	
//    itv_t *gamma = gamma_ab;
//    ap_dim_t* nsymconsa = nsymcons_a;
//    ap_dim_t* nsymconsb = nsymcons_b;
//    ap_dim_t* nsymcons = nsymcons_res;
//
//    arg_assert(exp1 && exp2, abort(););
//    t1p_aff_t * res = t1p_aff_alloc_init(pr);
//    void * ptmp = NULL;	/* needed for swap */
//    itv_t mid, dev;
//    itv_t mid1, dev1; itv_init(mid1); itv_init(dev1);	/* TODO: verifier les clear apres */
//    itv_t mid2, dev2; itv_init(mid2); itv_init(dev2);
//    itv_t itv, itv1, itv2;
//    itv_init(itv); itv_init(itv1); itv_init(itv2);
//    itv_t tmp, abscoeff, beta;
//    itv_init(tmp); itv_init(abscoeff); itv_init(beta);
//    t1p_aaterm_t *lastu = NULL;
//    t1p_aaterm_t *p, *q, *ptr;
//
//    t1p_aff_boxize(pr, itv1, exp1, a);			/* gamma(exp1) */
//    t1p_aff_boxize(pr, itv2, exp2, b);			/* gamma(exp2) */
//    itv_middev(pr->itv, mid1, dev1, itv1);
//    itv_middev(pr->itv, mid2, dev2, itv2);
//    itv_join(itv, itv1, itv2);
//    itv_middev(pr->itv, mid, dev, itv);
//    if (itv_cmp(mid1, mid2)) {;} 
//    else {
//	/* swap exp1 and exp2 */
//	ptmp = (void*)exp1;
//	exp1 = exp2;
//	exp2 = (t1p_aff_t*)ptmp;
//	/* swap gammaa and gammab */
//	ptmp = (void*)a;
//	a = b;
//	b = (itv_t*)ptmp;
//	itv_swap(itv1, itv2);
//	itv_swap(mid1, mid2);
//	itv_swap(dev1, dev2);
//    }
//    if (!(itv_is_generic(itv1, itv2))) {
//	itv_set(beta, dev);
//	itv_set(res->c, mid);
//	t1p_aff_nsym_create(pr, res, beta);
//	res->lastu = res->end;
//	return res;
//    } else {
//	/* exp1 and exp2 are in generic position and mid(exp1) \leq mid(exp2) */
//	itv_t argminpq; itv_init(argminpq);
//	ptr = NULL;
//
//	if (exp1->q && exp2->q) {
//	    itv_t nsym_gammaa, nsym_gammab, nsym_gamma_res;
//	    itv_init(nsym_gammaa); itv_init(nsym_gammab); itv_init(nsym_gamma_res);
//	    itv_t suma, sumb; itv_init(suma); itv_init(sumb);
//	    itv_t mida, midb, midab, deva, devb, devab, tmpa, tmpb;
//	    itv_init(mida); itv_init(midb); itv_init(midab); itv_init(deva); itv_init(devb); itv_init(devab);
//	    itv_init(tmpa);itv_init(tmpb);
//	    itv_t* hypercube = itv_array_alloc(exp1->l + exp2->l);
//	    ap_linexpr0_t* a_linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
//	    ap_linexpr0_t* b_linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
//	    a_linexpr0->p.linterm = (ap_linterm_t*)malloc((exp1->l + exp2->l)*sizeof(ap_linterm_t));
//	    b_linexpr0->p.linterm = (ap_linterm_t*)malloc((exp1->l + exp2->l)*sizeof(ap_linterm_t));
//	    a_linexpr0->size = exp1->l + exp2->l;
//	    b_linexpr0->size = exp1->l + exp2->l;
//	    ap_coeff_t* coeff = ap_coeff_alloc(AP_COEFF_INTERVAL);
//	    itv_sub(tmp, mid, mid1);
//	    ap_coeff_set_itv(pr->itv, coeff, tmp);
//	    itv_set(suma, tmp);
//	    ap_linexpr0_set_cst(a_linexpr0, coeff);
//	    itv_sub(tmp, mid2, mid);
//	    ap_coeff_set_itv(pr->itv, coeff, tmp);
//	    itv_set(sumb, tmp);
//	    ap_linexpr0_set_cst(b_linexpr0, coeff);
//	    ap_dim_t dim_nsym_a, dim_nsym_b, dim_nsym_res;
//	    for(p = exp1->q, q = exp2->q; p && q;) {
//		if (p->pnsym->index == q->pnsym->index) {				/* coeff of the same noise symbol */
//		    if (t1p_nsymcons_get_dim(pr, &dim_nsym_a, p->pnsym->index, nsymconsa, sizeofa_abs)) itv_set(nsym_gammaa, gammaa[dim_nsym_a]);
//		    else itv_set(nsym_gammaa, pr->muu);
//		    if (t1p_nsymcons_get_dim(pr, &dim_nsym_b, p->pnsym->index, nsymconsb, sizeofb_abs)) itv_set(nsym_gammab, gammab[dim_nsym_b]);
//		    else itv_set(nsym_gammab, pr->muu);
//		    //if (dim_nsym_a == -1) {itv_set(nsym_gammaa, pr->muu);}
//		    //else {itv_set(nsym_gammaa, gammaa[dim_nsym_a]);}
//		    //if (dim_nsym_b == -1) {itv_set(nsym_gammab, pr->muu);}
//		    //else {itv_set(nsym_gammab, gammab[dim_nsym_b]);}
//		    if (itv_is_generic(nsym_gammaa, nsym_gammab)) {
//			/* eps_i^a et eps_i^b sont en pos generique */
//			if (t1p_nsymcons_get_dim(pr, &dim_nsym_res, p->pnsym->index, nsymcons, sizeofres_abs)) itv_set(nsym_gamma_res, gamma[dim_nsym_res]);
//			else itv_set(nsym_gamma_res, pr->muu);
//			//dim_nsym_res = t1p_nsymcons_get_dim(pr, p->pnsym->index, nsymcons, sizeofres_abs);
//			//if (dim_nsym_res == -1) {itv_set(nsym_gamma_res, pr->muu);}
//			//else {itv_set(nsym_gamma_res, gamma[dim_nsym_res]);}
//			itv_middev(pr->itv, mida, deva, nsym_gammaa);
//			itv_middev(pr->itv, midb, devb, nsym_gammab);
//			itv_middev(pr->itv, midab, devab, nsym_gamma_res);
//			itv_sub(tmpa, mida, midab);
//			itv_sub(tmpb, midb, midab);
//			if (itv_is_pos(p->coeff) && itv_is_pos(q->coeff)) {
//			    /* \alpha_i^a et \alpha_i^b sont pos -> \alpha_i^c est pos */
//			    if (itv_is_neg(tmpa) && itv_is_pos(tmpb)) {
//				/* \alpha_i^c <> 0 */
//				if (argmin(pr, argminpq, p->coeff, q->coeff)) {
//				    if (res->q == NULL) {
//					res->q = ptr = t1p_aaterm_alloc_init();			/* allocate the first element of res->q */
//					res->l++;
//				    } else {
//					ptr->n = t1p_aaterm_alloc_init();
//					res->l++;
//					ptr = ptr->n;
//				    }
//				    if (exp1->lastu && exp2->lastu) {				/* keep track of the perturbation noise symbol */
//					if (exp1->lastu == p && exp2->lastu == q) lastu = ptr;	/* only if exp1->lastu and exp2->lastu resist to argmin formula */
//				    } 
//				    ptr->pnsym = p->pnsym;						/* set the index of the noise symbol */
//				    itv_set(ptr->coeff, argminpq);					/* set the coefficient as the argmin of the two other coefficients */
//				    itv_set(hypercube[-1 + res->l],argminpq);
//				    bound_init(hypercube[-1 + res->l]->inf);			/* store [0, argmin] in hypercube[-1 + res->l] */
//				    ap_coeff_init(&a_linexpr0->p.linterm[-1 + res->l].coeff, AP_COEFF_INTERVAL);
//				    ap_coeff_set_itv(pr->itv, &a_linexpr0->p.linterm[-1 + res->l].coeff, tmpa);
//				    a_linexpr0->p.linterm[-1 + res->l].dim = -1 + res->l;
//				    itv_mul(pr->itv, tmp, ptr->coeff, tmpa);
//				    itv_add(suma, suma, tmp);
//				    ap_coeff_init(&b_linexpr0->p.linterm[-1 + res->l].coeff, AP_COEFF_INTERVAL);
//				    itv_neg(tmpb,tmpb);
//				    ap_coeff_set_itv(pr->itv, &b_linexpr0->p.linterm[-1 + res->l].coeff, tmpb);
//				    b_linexpr0->p.linterm[-1 + res->l].dim = -1 + res->l;
//				    itv_mul(pr->itv, tmp, ptr->coeff, tmpb);
//				    itv_add(sumb, sumb, tmp);
//				} else {
//				    add_dim_to_remove(pr, p->pnsym->index);
//				}
//			    } else {
//				add_dim_to_remove(pr, p->pnsym->index);
//			    }
//			} else if (itv_is_neg(p->coeff) && itv_is_neg(q->coeff)) {
//			    /* \alpha_i^a et \alpha_i^b sont neq -> \alpha_i^c est neg */
//			    if (itv_is_pos(tmpa) && itv_is_neg(tmpb)) {
//				/* \alpha_i^c <> 0 */
//				if (argmin(pr, argminpq, p->coeff, q->coeff)) {
//				    if (res->q == NULL) {
//					res->q = ptr = t1p_aaterm_alloc_init();			/* allocate the first element of res->q */
//					res->l++;
//				    } else {
//					ptr->n = t1p_aaterm_alloc_init();
//					res->l++;
//					ptr = ptr->n;
//				    }
//				    if (exp1->lastu && exp2->lastu) {				/* keep track of the perturbation noise symbol */
//					if (exp1->lastu == p && exp2->lastu == q) lastu = ptr;	/* only if exp1->lastu and exp2->lastu resist to argmin formula */
//				    } 
//				    ptr->pnsym = p->pnsym;					/* set the index of the noise symbol */
//				    itv_set(ptr->coeff, argminpq);				/* set the coefficient as the argmin of the two other coefficients */
//				    itv_set(hypercube[-1 + res->l], argminpq);
//				    bound_init(hypercube[-1 + res->l]->sup);				/* store [argmin, 0] in hypercube[-1 + res->l] */
//				    ap_coeff_init(&a_linexpr0->p.linterm[-1 + res->l].coeff, AP_COEFF_INTERVAL);
//				    ap_coeff_set_itv(pr->itv, &a_linexpr0->p.linterm[-1 + res->l].coeff, tmpa);
//				    a_linexpr0->p.linterm[-1 + res->l].dim = -1 + res->l;
//				    itv_mul(pr->itv, tmp, ptr->coeff, tmpa);
//				    itv_add(suma, suma, tmp);
//				    ap_coeff_init(&b_linexpr0->p.linterm[-1 + res->l].coeff, AP_COEFF_INTERVAL);
//				    itv_neg(tmpb,tmpb);
//				    ap_coeff_set_itv(pr->itv, &b_linexpr0->p.linterm[-1 + res->l].coeff, tmpb);
//				    b_linexpr0->p.linterm[-1 + res->l].dim = -1 + res->l;
//				    itv_mul(pr->itv, tmp, ptr->coeff, tmpb);
//				    itv_add(sumb, sumb, tmp);
//				} else {
//				    add_dim_to_remove(pr, p->pnsym->index);
//				}
//			    } else {
//				add_dim_to_remove(pr, p->pnsym->index);
//			    }
//			} else {
//			    add_dim_to_remove(pr, p->pnsym->index);
//			}
//		    } else {
//			add_dim_to_remove(pr, p->pnsym->index);
//		    }
//		    p = p->n ;
//		    q = q->n ;
//		} else if (p->pnsym->index < q->pnsym->index) {
//		    p = p->n ;
//		    add_dim_to_remove(pr, p->pnsym->index);
//		} else {
//		    q = q->n ;
//		    add_dim_to_remove(pr, q->pnsym->index);
//		}
//		/*
//		   if (p && q) {
//		   if (p->pnsym->index == q->pnsym->index && res->q) {
//		   if (argmin(pr, argminpq, p->coeff, q->coeff)) {
//		   ptr->n = t1p_aaterm_alloc_init();			//allocate the next element of res->q
//		   res->l++;
//		   ptr=ptr->n;
//		   }
//		   }
//		   }*/
//	    }
//	    if (ptr) {
//		ptr->n = NULL;
//		res->end = ptr;
//	    }
//	    /* Delete the join symbol if exists.
//	     * This is done simply by setting the coeff to zero, hence this will not appear while computing the norm.
//	     * TODO: what if the perturbation coeff isupports constraints ?
//	     */
//	    if (lastu) {
//		itv_init(lastu->coeff);
//	    }
//	    ap_lincons0_array_t hyperplane = ap_lincons0_array_make(1);
//	    //itv_fprint(stdout,suma); printf("\n");
//	    //itv_fprint(stdout,sumb); printf("\n");
//	    num_t eps;
//	    itv_t err; itv_init(err);
//	    num_set_double(eps,(double)1000*1.11022302462515654042e-16); /* threshold : 1000*last bit in the mantissa in double precision */
//	    itv_set_unit_num(err, eps);
//	    //printf("err: ");itv_fprint(stdout,err); printf("\n");
//	    size_t k = 0;
//	    if (bound_cmp(suma->inf,eps) <= 0) {
//		//if (itv_is_pos(suma)) 
//		if (bound_cmp(sumb->inf,eps)) {
//		    //if (itv_is_pos(sumb)) 
//		    //printf("here\n");
//		    /* argmin solution is ok */
//		    /* compute beta and alpha_0 */
//		} else {
//		    /* compute intersection with cons b_linexpr0 */
//		    itv_t midi, devi;
//		    itv_init(midi); itv_init(devi);
//		    b_linexpr0->p.linterm = (ap_linterm_t*)realloc(b_linexpr0->p.linterm, (res->l)*sizeof(ap_linterm_t));
//		    b_linexpr0->size = res->l;
//		    hyperplane.p[0] = ap_lincons0_make(AP_CONS_EQ, b_linexpr0, NULL);
//		    ap_interval_t** tinterval = ap_interval_array_alloc(res->l);
//		    for (k=0; k<res->l; k++) {
//			ap_interval_set_itv(pr->itv, tinterval[k], hypercube[k]);
//		    }
//		    ap_abstract0_t* abs = ap_abstract0_of_box(pr->box, 0, res->l, tinterval);
//		    ap_abstract0_t* abs1 = ap_abstract0_meet_lincons_array(pr->box, false, abs, &hyperplane);
//		    ap_interval_t** tinterval2 = ap_abstract0_to_box(pr->box, abs1);
//		    k = 0;
//		    for (p=res->q; p; p=p->n) {
//			itv_set_ap_interval(pr->itv, tmp, tinterval2[k]);
//			itv_middev(pr->itv, midi, devi, tmp);
//			itv_set(p->coeff, midi);
//			k++;
//		    }
//		    itv_clear(midi); itv_clear(devi);
//		    ap_abstract0_free(pr->box, abs1);
//		    ap_abstract0_free(pr->box, abs);
//		    ap_interval_array_free(tinterval2, res->l);
//		    ap_interval_array_free(tinterval, res->l);
//		}
//	    } else {
//		//if (itv_is_pos(sumb)) 
//		if (bound_cmp(sumb->inf, eps)) {
//		    /* compute intersection with cons c_x */
//		    itv_t midi, devi;
//		    itv_init(midi); itv_init(devi);
//		    a_linexpr0->p.linterm = (ap_linterm_t*)realloc(a_linexpr0->p.linterm, (res->l)*sizeof(ap_linterm_t));
//		    a_linexpr0->size = res->l;
//		    hyperplane.p[0] = ap_lincons0_make(AP_CONS_EQ, a_linexpr0, NULL);
//		    ap_interval_t** tinterval = ap_interval_array_alloc(res->l);
//		    for (k=0; k<res->l; k++) {
//			ap_interval_set_itv(pr->itv, tinterval[k], hypercube[k]);
//		    }
//		    ap_abstract0_t* abs = ap_abstract0_of_box(pr->box, 0, res->l, tinterval);
//		    ap_abstract0_t* abs1 = ap_abstract0_meet_lincons_array(pr->box, false, abs, &hyperplane);
//		    ap_interval_t** tinterval2 = ap_abstract0_to_box(pr->box, abs1);
//		    k = 0;
//		    for (p=res->q; p; p=p->n) {
//			itv_set_ap_interval(pr->itv, tmp, tinterval2[k]);
//			itv_middev(pr->itv, midi, devi, tmp);
//			itv_set(p->coeff, midi);
//			k++;
//		    }
//		    itv_clear(suma); itv_clear(sumb);
//		    itv_clear(mida); itv_clear(midb); itv_clear(midab); itv_clear(deva); itv_clear(devb); itv_clear(devab);
//		    itv_clear(tmpa);itv_clear(tmpb);
//		    itv_clear(midi); itv_clear(devi);
//		    ap_abstract0_free(pr->box, abs1);
//		    ap_abstract0_free(pr->box, abs);
//		    ap_interval_array_free(tinterval2, res->l);
//		    ap_interval_array_free(tinterval, res->l);
//		} else {
//		    /* both intersects return degenerate mub */
//		    t1p_aff_clear(pr, res);
//		    itv_set(beta, dev);
//		    itv_set(res->c, mid);
//		    t1p_aff_nsym_create(pr, res, beta);
//		    res->lastu = res->end;
//		    return res;
//		}
//	    }
//	    /* compute \beta and \alpha_0^c */
//	    itv_set(beta, dev);
//	    itv_set(res->c, mid);
//	    //if (gamma_ab == NULL) {		/* use the hypercube [-1,1]^dim */
//	    //	for (q=res->q; q; q=q->n) {
//	    //	    itv_abs(abscoeff, q->coeff);
//	    //	    itv_sub(beta, beta, abscoeff);
//	    //	}
//	    //  } else {
//	    itv_t midi, devi;
//	    itv_init(midi);
//	    itv_init(devi);
//	    for (q=res->q; q; q=q->n) {
//		if (t1p_nsymcons_get_dim(pr, &dim_nsym_res, q->pnsym->index, nsymcons, sizeofres_abs)) itv_set(nsym_gamma_res, gamma[dim_nsym_res]);
//		else itv_set(nsym_gamma_res, pr->muu);
//		//if (dim_nsym_res == -1) {itv_set(nsym_gamma_res, pr->muu);}
//		//else {itv_set(nsym_gamma_res, gamma[dim_nsym_res]);}
//		itv_middev(pr->itv, midi, devi, nsym_gamma_res);    /* supposes that gamma has the newest interval concretisations */
//		itv_mul(pr->itv, tmp, midi, q->coeff);       		/* alpha_i mid(epsilon_i) */
//		itv_sub(res->c, res->c, tmp);               		/* center - alpha_i mid(epsilon_i) */
//		itv_abs(abscoeff, q->coeff);                		/* |alpha_i| */
//		itv_mul(pr->itv, tmp, devi, abscoeff);       		/* |alpha_i| dev(epsilon_i) */
//		itv_sub(beta, beta, tmp);
//	    }
//	    itv_clear(midi);
//	    itv_clear(devi);
//	    //}
//	    if (itv_is_pos(beta)) {
//		if (!(itv_is_zero(beta))) {
//		    t1p_aff_nsym_create(pr, res, beta);
//		    res->lastu = res->end;
//		}
//	    } else {
//		fatal("aïe, aïe, aïe, le beta est négatif !\n");
//	    }
//	    itv_array_free(hypercube,exp1->l+exp2->l);
//	    ap_lincons0_array_clear(&hyperplane);
//	    //free(Uhypercube);
//	    return res;
//	} else {
//	    itv_set(beta, dev);
//	    itv_set(res->c, mid);
//	    t1p_aff_nsym_create(pr, res, beta);
//	    res->lastu = res->end;
//	    return res;
//	}
//    }
//}

/* CAV 2010 
 * computes lambda such as z = x = y with minimal interval range, if the constraint is "y - x = 0"
 */
static inline void t1p_aff_cons_eq_lambda(t1p_internal_t* pr, itv_t* res, t1p_aff_t* x, t1p_aff_t* cons, t1p_t *a)
{
    t1p_aaterm_t *p, *q; 
    p = q = NULL;
    /* TODO: mettre array dans pr ? */
    obj** array = NULL;
    if (cons->l + x->l > pr->dim)  array = (obj**)calloc(pr->dim,sizeof(obj*)); // pr->dim est le max qu'on peut avoir a trier
    else array = (obj**)calloc((cons->l + x->l),sizeof(obj*)); // une vache sur-approximation
    size_t max = 0;
    size_t i = 0;
    itv_t tmp, mid, dev, dim_itv; 
    itv_init(mid);
    itv_init(dev);
    itv_init(tmp);
    itv_init(dim_itv);
    ap_dim_t dim;
    /* parcourir la contrainte */
    for (p=cons->q, q=x->q; p || q;) {
	if (p && q) {
	    if (p->pnsym->index == q->pnsym->index) {
		/* ajouter -q->coeff/p->coeff avec l'indice d'epsilon */
		if (itv_is_zero(p->coeff)) {;}
		else {
		    array[i] = (obj*)calloc(1,sizeof(obj));
		    itv_init(array[i]->coeff);
		    itv_init(array[i]->itv);
		    if (a->hypercube) {
			itv_abs(tmp, p->coeff);
			itv_set(array[i]->coeff,tmp);
		    } else {
			if (t1p_nsymcons_get_dimpos(pr, &dim, p->pnsym->index, a)) {
			    itv_set_ap_interval(pr->itv, dim_itv, a->gamma[dim]);
			    itv_middev(pr->itv, mid, dev, dim_itv);
			    itv_abs(tmp, p->coeff);
			    itv_mul(pr->itv, array[i]->coeff, tmp, dev);
			} else {
			    itv_abs(tmp, p->coeff);
			    itv_set(array[i]->coeff,tmp);
			}
		    }
		    itv_div(pr->itv, tmp, q->coeff, p->coeff);
		    if (itv_is_point(pr->itv, tmp)) itv_neg(array[i]->itv, tmp);
		    else {
			bound_neg(tmp->inf, tmp->sup);
			itv_set_num(array[i]->itv, bound_numref(tmp->inf));
		    }
		    i++;
		}
		p = p->n ;
		q = q->n ;
	    } else if (p->pnsym->index < q->pnsym->index) {
		/* ajouter 0 dans les valeurs à trier avec l'indice d'epsilon */
		array[i] = (obj*)calloc(1,sizeof(obj));
		    itv_init(array[i]->coeff);
		    itv_init(array[i]->itv);
		if (a->hypercube) {
		    itv_abs(tmp, p->coeff);
		    itv_set(array[i]->coeff,tmp);
		} else {
		    if (t1p_nsymcons_get_dimpos(pr, &dim, p->pnsym->index, a)) {
			itv_set_ap_interval(pr->itv, dim_itv, a->gamma[dim]);
			itv_middev(pr->itv, mid, dev, dim_itv);
			itv_abs(tmp, p->coeff);
			itv_mul(pr->itv, array[i]->coeff, tmp, dev);
		    } else {
			itv_abs(tmp, p->coeff);
			itv_set(array[i]->coeff,tmp);
		    }
		}
		itv_set_int(array[i]->itv,0);
		i++;
		p = p->n ;
	    } else {
		q = q->n ;
	    }
	} else if (p) {
	    /* ajouter 0 dans les valeurs à trier avec l'indice d'epsilon */
	    array[i] = (obj*)calloc(1,sizeof(obj));
		    itv_init(array[i]->coeff);
		    itv_init(array[i]->itv);
	    if (a->hypercube) {
		itv_abs(tmp, p->coeff);
		itv_set(array[i]->coeff,tmp);
	    } else {
		if (t1p_nsymcons_get_dimpos(pr, &dim, p->pnsym->index, a)) {
		    itv_set_ap_interval(pr->itv, dim_itv, a->gamma[dim]);
		    itv_middev(pr->itv, mid, dev, dim_itv);
		    itv_abs(tmp, p->coeff);
		    itv_mul(pr->itv, array[i]->coeff, tmp, dev);
		} else {
		    itv_abs(tmp, p->coeff);
		    itv_set(array[i]->coeff,tmp);
		}
	    }
	    itv_set_int(array[i]->itv,0);
	    i++;
	    p = p->n ;
	} else {
	    /* ces termes n'intervienderont pas dans la minimisation de la concretisation */
	    /* on peut arreter la boucle ici */
	    break;
	}
    }
    max = i; // taille du tableau a trier
    /* trouver le min dans array et retourner l'indice correspondant */
    /* algo de tri */
    i = 0;
    obj* obj;
    if (max > 0) {
	while (i<max-1) {
	    if (itv_cmp(array[i]->itv, array[i+1]->itv)) {
		i++;
	    } else {
		obj = array[i];
		array[i] = array[i+1];
		array[i+1] = obj;
		if (i) i--;
	    }
	}
	/* une fois trie */
	itv_t sum, q1, q2;
	itv_init(sum); itv_init(q1); itv_init(q2);
	itv_set(sum, array[0]->coeff);
	for (i=1; i<max; i++) itv_add(sum, sum, array[i]->coeff);
	itv_set(q2, sum);
	i = 0;
	if (max == 1) {
	    itv_set(*res, array[i]->itv);
	} else {
	    do {
		itv_add(q1, q1, array[i]->coeff);
		itv_sub(q2,q2,array[i]->coeff);
		i++;
	    } while (itv_cmp(q1,q2));
	    /* we shall have q1 >= q2 */
	    if (itv_is_eq(q1,q2)) {
		/* all lambda between [array[i-1].itv, array[i].itv] are solutions */
		/* TODO: appliquer l'heuristique de l'inclusion dans l'intersection des concretisations ... */
		itv_set(*res, array[i-1]->itv);
	    }
	    else {
		/* array[i-1].itv is the only possible solution */
		itv_set(*res, array[i-1]->itv);
	    }
	}
	itv_clear(sum); itv_clear(q1); itv_clear(q2);
    }
    itv_clear(mid);
    itv_clear(dev);
    itv_clear(tmp);
    itv_clear(dim_itv);
    for (i=0;i<max;i++) {
	itv_clear(array[i]->coeff);
	itv_clear(array[i]->itv);
	free(array[i]);
    }
    free(array);
}

/* same as t1p_aff_join_constrained6 while zeroing all epsilons */
static inline t1p_aff_t* t1p_aff_widening_constrained6(t1p_internal_t* pr, t1p_aff_t* exp1, t1p_aff_t* exp2, t1p_t* a, t1p_t* b, t1p_t* ab)
{
    arg_assert(exp1 && exp2, abort(););

    t1p_aff_t * res = t1p_aff_alloc_init(pr);
    itv_t mid, dev, tmp, tmp1, tmp2, betaA, betaB;
    itv_t c1, c2, d1, d2, c, d;
    itv_init(tmp); itv_init(tmp1); itv_init(tmp2);
    itv_init(betaA); itv_init(betaB);
    itv_init(c);
    itv_init(c1);
    itv_init(c2);
    itv_init(d);
    itv_init(d1);
    itv_init(d2);
    itv_init(mid);
    itv_init(dev);
    itv_t nsymItv1; itv_init(nsymItv1);
    itv_t nsymItv2; itv_init(nsymItv2);
    itv_t pmptr; itv_init(pmptr);
    itv_t qmptr; itv_init(qmptr);
    itv_t argminpq; itv_init(argminpq);

    t1p_aaterm_t *p, *q, *ptr;

    itv_join(res->itv, exp1->itv, exp2->itv);
    ptr = NULL;

    if (exp1->q || exp2->q) {
	itv_set(c1, exp1->c);
	itv_set(c2, exp2->c);
	ptr = t1p_aaterm_alloc_init();
	for(p = exp1->q, q = exp2->q; p || q;) {
	    if (p && q) {
		if (p->pnsym->index == q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (p->pnsym->type == IN) {
			//argmin(pr, ptr->coeff, p->coeff, q->coeff);
			ptr->pnsym = p->pnsym;
			itv_sub(pmptr, p->coeff, ptr->coeff);
			itv_sub(qmptr, q->coeff, ptr->coeff);
		    }
		    p = p->n ;
		    q = q->n ;
		} else if (p->pnsym->index < q->pnsym->index) {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		    if (p->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
			itv_add(betaA, betaA, tmp);
		    } else if (p->pnsym->type == IN) {
			itv_set(pmptr,p->coeff);
			itv_set_int(qmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		    p = p->n;
		} else {
		    t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		    t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		    if (q->pnsym->type == UN) {
			itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
			itv_add(betaB, betaB, tmp);
		    } else if (q->pnsym->type == IN) {
			itv_set(qmptr,q->coeff);
			itv_set_int(pmptr,0);
		    }
		    t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		    q = q->n;
		}
	    } else if (p) {
		t1p_nsymcons_get_gamma(pr, nsymItv1, p->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, p->pnsym->index, b);
		if (p->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv1, p->coeff);
		    itv_add(betaA, betaA, tmp);
		} else if (p->pnsym->type == IN) {
		    itv_set(pmptr,p->coeff);
		    itv_set_int(qmptr,0);
		}
		t1p_delete_constrained_nsym(pr, p->pnsym->index, ab);
		p = p->n;
	    } else {
		t1p_nsymcons_get_gamma(pr, nsymItv1, q->pnsym->index, a);
		t1p_nsymcons_get_gamma(pr, nsymItv2, q->pnsym->index, b);
		if (q->pnsym->type == UN) {
		    itv_mul(pr->itv, tmp, nsymItv2, q->coeff);
		    itv_add(betaB, betaB, tmp);
		} else if (q->pnsym->type == IN) {
		    itv_set(qmptr,q->coeff);
		    itv_set_int(pmptr,0);
		}
		t1p_delete_constrained_nsym(pr, q->pnsym->index, ab);
		q = q->n;
	    }
	    itv_mul(pr->itv, tmp1, pmptr, nsymItv1);
	    itv_add(c1, c1, tmp1);
	    itv_mul(pr->itv, tmp2, qmptr, nsymItv2);
	    itv_add(c2, c2, tmp2);
	    itv_set_int(pmptr,0);
	    itv_set_int(qmptr,0);
	    if (itv_is_zero(ptr->coeff)) {
		if (!(p||q)) {
		    /* the last iteration */
		    t1p_aaterm_free(pr, ptr);
		    if (res->end) res->end->n = NULL;
		}
	    } else {
		/* keep this term */
		if (!res->q) res->q = ptr;
		res->end = ptr;
		res->l++;
		if (p||q) {
		    /* continuing */
		    ptr->n = t1p_aaterm_alloc_init();
		    ptr=ptr->n;
		} else {
		    /* the last iteration */
		}
	    }
	}

	itv_middev(pr->itv, mid, dev, betaA);
	itv_set(d1, dev);
	itv_add(c1, c1, mid);

	itv_middev(pr->itv, mid, dev, betaB);
	itv_set(d2, dev);
	itv_add(c2, c2, mid);

	num_t c0, min, max, dmin, dmax;
	num_init(c0); num_init(min); num_init(max); num_init(dmin); num_init(dmax);
	itv_t beta; itv_init(beta);

	num_t array[4];
	num_init(array[0]);
	num_init(array[1]);
	num_init(array[2]);
	num_init(array[3]);

	num_set(array[0],bound_numref(c1->inf));
	num_neg(array[0],array[0]);
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_neg(array[1],array[1]);
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_add(array[3],array[3],bound_numref(d2->sup));

	int i = 0;
	num_set(dmax, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmax,array[i]) < 0) num_set(dmax, array[i]);

	num_set(array[0],bound_numref(c1->inf));
	num_add(array[0],array[0],bound_numref(d1->sup));
	num_set(array[1],bound_numref(c2->inf));
	num_add(array[1],array[1],bound_numref(d2->sup));
	num_set(array[2],bound_numref(c1->sup));
	num_neg(array[2],array[2]);
	num_add(array[2],array[2],bound_numref(d1->sup));
	num_set(array[3],bound_numref(c2->sup));
	num_neg(array[3],array[3]);
	num_add(array[3],array[3],bound_numref(d2->sup));

	num_set(dmin, array[0]);
	for (i=1;i<4;i++)
	    if (num_cmp(dmin,array[i]) < 0) num_set(dmin, array[i]);

	num_add(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(beta,c0);

	num_sub(c0,dmax,dmin);
	num_div_2(c0, c0);
	itv_set_num(res->c,c0);

	if (num_cmp(bound_numref(c1->inf),bound_numref(c2->inf)) < 0) {
	    num_set(min, bound_numref(c2->inf));
	} else {
	    num_set(min, bound_numref(c1->inf));
	}
	num_neg(min,min);

	if (num_cmp(bound_numref(c1->sup),bound_numref(c2->sup)) < 0) {
	    num_set(max, bound_numref(c2->sup));
	} else {
	    num_set(max, bound_numref(c1->sup));
	}

	if (num_cmp(c0,min) <= 0 || num_cmp(c0,max) >= 0) {
	    t1p_aff_free(pr, res);
	    itv_sub(tmp,d2,d1);
	    if (itv_is_pos(tmp)) res = exp2;
	    else res = exp1;
	} else t1p_aff_nsym_create(pr, res, beta, UN);

	num_clear(c0); num_clear(min); num_clear(max); num_clear(dmin); num_clear(dmax);
	num_clear(array[0]);
	num_clear(array[1]);
	num_clear(array[2]);
	num_clear(array[3]);
	itv_clear(beta);
    } else {
	t1p_aff_add_itv(pr, res, res->itv, UN);
    }
    itv_clear(tmp); itv_clear(tmp1); itv_clear(tmp2);
    itv_clear(betaA); itv_clear(betaB);
    itv_clear(c);
    itv_clear(c1);
    itv_clear(c2);
    itv_clear(d);
    itv_clear(d1);
    itv_clear(d2);
    itv_clear(nsymItv1);
    itv_clear(nsymItv2);
    itv_clear(argminpq);
    itv_clear(pmptr);
    itv_clear(qmptr);
    itv_clear(mid);
    itv_clear(dev);
    return res;
}

static inline bool t1p_aff_is_top(t1p_internal_t* pr, t1p_aff_t *a)
{
    if (a == pr->top) return true;
    else if (!itv_is_top(a->c)) return false;
    else if (!itv_is_top(a->itv)) return false;
    else if (a->q != NULL) return false;
    else return true;
}
static inline bool t1p_aff_is_bottom(t1p_internal_t* pr, t1p_aff_t *a)
{
    if (a == pr->bot) return true;
    else if (!itv_is_bottom(pr->itv, a->c)) return false;
    else if (!itv_is_bottom(pr->itv, a->itv)) return false;
    else if (a->q != NULL) return false;
    else return true;
}

static inline t1p_internal_t* t1p_internal_alloc(ap_manager_t* manNS)
{
    CALL();
    t1p_internal_t* pr = (t1p_internal_t*)malloc(sizeof(t1p_internal_t));
    pr->itv = itv_internal_alloc();
    pr->dim = 0;
    pr->funid = AP_FUNID_UNKNOWN;
    pr->man = NULL;
    pr->manNS = manNS;
    pr->box = box_manager_alloc();
    itv_init(pr->muu); itv_set_int2(pr->muu, (long int)-1, (long int)1);
    pr->ap_muu = ap_interval_alloc();
    ap_interval_set_itv(pr->itv, pr->ap_muu, pr->muu);
    pr->moo = ap_lincons0_array_make(2);
    pr->epsilon = (t1p_nsym_t**)calloc(1024, sizeof(t1p_nsym_t*));	/* starts with a limit of 1024 noise symbols */
    pr->top = t1p_aff_top(pr);
    pr->bot = t1p_aff_bottom(pr);
    pr->nsymhypercube = ap_abstract0_top(pr->manNS, 0,0);
    pr->dimtoremove = (ap_dim_t*)calloc(128, sizeof(ap_dim_t));
    pr->dimchange = ap_dimchange_alloc(0,1);
    /* -eps + 1 */
    ap_linexpr0_t* mnspone = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);	/* mnspone : (m)inus (n)oise (s)ymbol (p)lus (o)ne */
    ap_linexpr0_set_cst_scalar_double(mnspone, 1.0);
    ap_linexpr0_set_coeff_scalar_double(mnspone, (ap_dim_t)0, (double)-1.0);
    /* eps + 1 */
    ap_linexpr0_t* nspone = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, (size_t)1);
    ap_linexpr0_set_cst_scalar_double(nspone, 1.0);
    ap_linexpr0_set_coeff_scalar_double(nspone, (ap_dim_t)0, (double)1.0);

    /* 0 <= -eps + 1 */
    pr->moo.p[0] = ap_lincons0_make(AP_CONS_SUPEQ, mnspone, NULL);
    /* 0 <= eps + 1 */
    pr->moo.p[1] = ap_lincons0_make(AP_CONS_SUPEQ, nspone, NULL);

    pr->start = clock();
    pr->optpr = NULL;
    return pr;
}

static inline void t1p_internal_free(t1p_internal_t* pr)
{
    CALL();
    unsigned int i = 0;
    clock_t end = clock();
    double cpu_time_used = ((double) (end - pr->start)) / CLOCKS_PER_SEC;
#ifdef _GET_CPU_TIME
    fprintf(stdout, "\n ### cpu_time = %g ### \n", cpu_time_used);
#endif
    if (pr) {
	itv_internal_free(pr->itv);
	t1p_aff_free(pr, pr->top);
	t1p_aff_free(pr, pr->bot);
	pr->start = 0;
	pr->itv = NULL;
	for (i=0;i<pr->dim;i++) free(pr->epsilon[i]);
	pr->dim = (size_t)0;
	free(pr->epsilon);
	pr->epsilon= NULL;
	pr->funid = AP_FUNID_UNKNOWN;
	pr->man = NULL;
	ap_abstract0_free(pr->manNS, pr->nsymhypercube);
	pr->nsymhypercube = NULL;
	ap_manager_free(pr->manNS);
	pr->manNS = NULL;
	ap_manager_free(pr->box);
	pr->box = NULL;
	itv_clear(pr->muu);
	ap_interval_free(pr->ap_muu);
	pr->ap_muu = NULL;
	ap_lincons0_array_clear(&(pr->moo));
	free(pr->dimtoremove);
	ap_dimchange_free(pr->dimchange);
	pr->dimchange = NULL;
	pr->optpr = NULL;
	free(pr);
    }
}

static inline void t1p_set_lincons_dim(t1p_internal_t* pr, ap_dim_t dim) {
    pr->moo.p[0].linexpr0->p.linterm[0].dim = dim;
    pr->moo.p[1].linexpr0->p.linterm[0].dim = dim;
}

static inline void t1p_nsymcons_set_hypercube(t1p_internal_t* pr, t1p_t* a) 
{
    size_t size = t1p_nsymcons_get_dimension(pr, a);
    ap_interval_t** tinterval = (ap_interval_t**)malloc(size*sizeof(ap_interval_t*));
    size_t i = 0;
    for (i=0; i<size; i++) tinterval[i] = pr->ap_muu;

    ap_abstract0_free(pr->manNS, pr->nsymhypercube);
    pr->nsymhypercube = ap_abstract0_of_box(pr->manNS, 0, size, tinterval);
    free(tinterval);
}


static inline bool findKR(size_t *res, size_t x, size_t* tab, size_t size)
{
    assert(size >= 1);
    int low = 0;
    int high = size - 1;
    int mid = -1;
    while (low <= high) {
	mid = (high + low) / 2;
	if (x < tab[mid]) high = mid - 1;
	else if (x > tab[mid]) low = mid + 1;
	else {
	    *res = mid;
	    return true;
	}
    }
    *res = mid;
    return false;
}

static inline bool t1p_nsymcons_get_dimpos(t1p_internal_t* pr, ap_dim_t* dim, size_t nsymIndex, t1p_t* a)
{
    size_t size = t1p_nsymcons_get_dimension(pr, a);
    if (size) return (ap_dim_t)findKR((size_t*)dim, (size_t)nsymIndex, (size_t*)a->nsymcons, size);
    else return false;
}

static inline size_t t1p_nsymcons_get_dimension(t1p_internal_t *pr, t1p_t *a)
{
    ap_dimension_t dimension = ap_abstract0_dimension(pr->manNS, a->abs);
    return (dimension.intdim + dimension.realdim);
}


static inline void t1p_nsymcons_get_gamma(t1p_internal_t * pr, itv_t res, size_t nsymIndex, t1p_t* a) 
{
    if (a->hypercube) {
	itv_set(res, pr->muu);
    } else {
	ap_dim_t dim;
	if (t1p_nsymcons_get_dimpos(pr, &dim, nsymIndex, a)) itv_set_ap_interval(pr->itv, res, a->gamma[dim]);
	else itv_set(res, pr->muu);
    }
}

static inline bool t1p_insert_constrained_nsym(t1p_internal_t *pr, ap_dim_t* res, size_t nsymIndex, t1p_t *a)
{
    size_t j = 0;
    void* dst = NULL;
    size_t size = t1p_nsymcons_get_dimension(pr, a);
    size_t dim = 0;
    bool addconsnsym = false;

    /* resize nsymcons array if needed */
    if ((size + 1) % 128 == 0) {
	a->size += 128;
	a->nsymcons = (ap_dim_t*)realloc(a->nsymcons, (a->size)*sizeof(ap_dim_t));
	a->gamma = (ap_interval_t**)realloc(a->gamma, (a->size)*sizeof(ap_interval_t*));
    }
    if (size == 0) {
	a->nsymcons[size] = nsymIndex;
	a->gamma[size] = pr->ap_muu;
	*res = size;
	pr->dimchange->dim[0] = size;
	ap_abstract0_add_dimensions(pr->manNS, true, a->abs, pr->dimchange, false);
	addconsnsym = true;
    } else if (nsymIndex > a->nsymcons[size-1]) {
	a->nsymcons[size] = nsymIndex;
	a->gamma[size] = pr->ap_muu;
	*res = size;
	pr->dimchange->dim[0] = size;
	ap_abstract0_add_dimensions(pr->manNS, true, a->abs, pr->dimchange, false);
	addconsnsym= true;
    } else {
	if (!findKR((size_t *)&dim, nsymIndex, (size_t*)a->nsymcons, size)) {
	    dst = (void *)(&a->nsymcons[dim+1]);
	    memmove(dst,(void*)(&a->nsymcons[dim]),(size-dim)*sizeof(ap_dim_t));
	    a->nsymcons[dim] = nsymIndex;
	    dst = (void *)(&a->gamma[dim+1]);
	    memmove(dst,(void*)(&a->gamma[dim]),(size-dim)*sizeof(ap_interval_t*));
	    a->gamma[dim] = pr->ap_muu;
	    *res = dim;
	    pr->dimchange->dim[0] = dim;
	    ap_abstract0_add_dimensions(pr->manNS, true, a->abs, pr->dimchange, false);
	    addconsnsym = true;
	} else {
	    *res = dim;
	    addconsnsym = false;
	}
    }
    if (addconsnsym) {
	t1p_set_lincons_dim(pr, pr->dimchange->dim[0]);
	ap_abstract0_meet_lincons_array(pr->manNS, true, a->abs, &pr->moo);
    }

    return addconsnsym;
}

/* uses the internal structure pr->dimtoremove */
/* starts with a memory zone setted to zero  (memset) */
static inline void t1p_delete_constrained_nsym(t1p_internal_t *pr, size_t nsymIndex, t1p_t *a)
{
    CALL();
    size_t size = t1p_nsymcons_get_dimension(pr, a);
    if (size == 0) {
	/* the constrained noise symbol abstract object is already empty */
    } else {
	size_t dim;
	/* worst case : all constrained nsym have to be removed */
	if (findKR(&dim, nsymIndex, (size_t*)a->nsymcons, size)) {
	    pr->dimtoremove[dim]++;
	    if (pr->dimtoremove[dim] == a->dims) {
		void* dst = NULL;
		//fprintf(stdout, "remove %d\n", dim);
		pr->dimchange->dim[0] = dim;
		ap_abstract0_remove_dimensions(pr->manNS, true, a->abs, pr->dimchange);
		if (size-1 == 0) a->hypercube = true;
		/* UPDATE a->nsymcons and a->gamma */
		dst = (void *)(&a->nsymcons[dim]);
		memmove(dst,(void*)(&a->nsymcons[dim+1]),(size-dim-1)*sizeof(ap_dim_t));
		//fprintf(stdout, "toto %d #################### \n",size-1-1);
		if (size > 1) a->nsymcons[size-1-1] = 0; else a->nsymcons[0] = 0;
		if (a->gamma[dim]) {
		    if (a->gamma[dim] != pr->ap_muu) {ap_interval_free(a->gamma[dim]);a->gamma[dim] = NULL;}
		}
		dst = (void *)(&a->gamma[dim]);
		memmove(dst,(void*)(&a->gamma[dim+1]),(size-dim-1)*sizeof(ap_interval_t*));
	    }
	}
    }
}

/* 
   - build a SPARSE ap_linexpr0_t from t1p_aff_t
   - update a->abs with new constrained noise symbols
 */
static inline ap_linexpr0_t* t1p_ap_linexpr0_set_aff(t1p_internal_t* pr, t1p_aff_t* aff, t1p_t* a)
{
    ap_interval_t *cst = ap_interval_alloc();
    ap_linexpr0_t* linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
    ap_interval_set_itv(pr->itv, cst ,aff->c);
    ap_linexpr0_set_cst_interval(linexpr0, cst);

    t1p_aaterm_t *p;
    linexpr0->p.linterm = (ap_linterm_t*)malloc(aff->l*sizeof(ap_linterm_t));
    linexpr0->size = aff->l;
    size_t k = 0;
    ap_dim_t dim = 0;
    for (p=aff->q; p; p=p->n) {
	ap_coeff_init(&linexpr0->p.linterm[k].coeff, AP_COEFF_INTERVAL);
	ap_coeff_set_itv(pr->itv, &linexpr0->p.linterm[k].coeff, p->coeff);
	/* update a->abs with new constrained noise symbols */
	t1p_insert_constrained_nsym(pr, &dim, p->pnsym->index, a);
	linexpr0->p.linterm[k].dim = dim;
	k++;
    }
    ap_interval_free(cst);
    return linexpr0;
}

static inline void t1p_update_nsymcons_gamma(t1p_internal_t* pr, t1p_t* a)
{
    size_t i = 0;
    size_t nsymcons_size = t1p_nsymcons_get_dimension(pr, a);
    ap_interval_t* bound = NULL;
    t1p_nsymcons_set_hypercube(pr, a);
    if (ap_abstract0_is_leq(pr->manNS, pr->nsymhypercube, a->abs)) {
	a->hypercube = true;
	ap_abstract0_free(pr->manNS, a->abs);
	a->abs = ap_abstract0_top(pr->manNS, 0,0);
	for (i=0; i<nsymcons_size; i++) {
	    if (a->gamma[i] != pr->ap_muu) ap_interval_free(a->gamma[i]);
	    a->gamma[i] = NULL;
	}
	//memset((void*)a->gamma, 0, nsymcons_size*sizeof(ap_interval_t*));
	memset((void*)a->nsymcons, 0, nsymcons_size*sizeof(ap_dim_t));
    } else {
	a->hypercube = false;
	for (i=0; i<nsymcons_size; i++) {
	    bound = ap_abstract0_bound_dimension(pr->manNS, a->abs, i);
	    if (a->gamma[i] == NULL) {
		a->gamma[i] = bound;
	    } else if (ap_interval_is_leq(a->gamma[i], bound)) {
		ap_interval_free(bound);
	    } else {
		if (a->gamma[i] != pr->ap_muu) ap_interval_free(a->gamma[i]);
		a->gamma[i] = bound;
	    }
	    bound = NULL;
	    /*
	    if (ap_interval_is_leq(a->gamma[i], bound)) {
		ap_interval_free(bound);
	    } else {
		if (a->gamma[i] != pr->ap_muu) ap_interval_free(a->gamma[i]);
		a->gamma[i] = bound;
	    }
	    bound = NULL;
	    */
	}
    }
}


#endif

