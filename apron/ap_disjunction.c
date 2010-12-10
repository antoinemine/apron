/* ************************************************************************* */
/* ap_disjunction.c: generic disjunction of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
 read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_global0.h"
#include "ap_disjunction.h"

/* ------------------------------------------------------------ */
/* Allocates an abstract value */
/* ------------------------------------------------------------ */

static inline ap_disjunction_t* ap_disjunction_alloc(size_t size) {
	size_t i;
	ap_disjunction_t* res = malloc(sizeof(ap_disjunction_t) + size
			* sizeof(void*));
	for (i = 0; i < size; i++)
		res->p[i] = NULL;
	return res;
}

/* ------------------------------------------------------------ */
/* Get internal and optionally  */
/* ------------------------------------------------------------ */
static inline ap_disjunction_internal_t* get_internal_init(
		ap_manager_t* manager) {
	return (ap_disjunction_internal_t*) manager->internal;
}

/* ============================================================ */
/*   */
/* ============================================================ */

/*
static
bool ap_interval_meet_with(ap_interval_t* a, ap_interval_t* b)
{
  if (ap_scalar_cmp(a->inf,b->inf)<0){
    ap_scalar_set(a->inf,b->inf);
  }
  if (ap_scalar_cmp(a->sup,b->sup)>0){
    ap_scalar_set(a->sup,b->sup);
  }
  return ap_scalar_cmp(a->inf,a->sup)>0;
}
*/

static
void ap_interval_join_with(ap_interval_t* a, ap_interval_t* b)
{
  if (ap_scalar_cmp(a->inf,b->inf)>0){
    ap_scalar_set(a->inf,b->inf);
  }
  if (ap_scalar_cmp(a->sup,b->sup)<0){
    ap_scalar_set(a->sup,b->sup);
  }
}

static void set_bottom(ap_disjunction_internal_t* intern, ap_disjunction_t* a)
{
	ap_manager_t* man = intern->manager;

	ap_dimension_t (*dimension)(ap_manager_t*, ...) = man->funptr[AP_FUNID_DIMENSION];
	ap_dimension_t dim = dimension(man, a->p[0]);

	void* (*bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_BOTTOM];

	size_t i;
	for (i = 0; i < a->size; i++) {
		if (a->p[i]!=NULL){
			free(man, a->p[i]);
			a->p[i] = NULL;
	    }
	}
	a->p[0] = bottom(man, dim.intdim, dim.realdim);
}

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_copy(ap_manager_t* manager,
						ap_disjunction_t* a)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_COPY];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, a->p[i]);
	}
	return res;
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void ap_disjunction_approximate(ap_manager_t* manager, ap_disjunction_t* a,
		int n) {
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	intern->approximate(manager, (void*) a, n);
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */
/* TO DO */

/* ********************************************************************** */
/* II. Constructor, accessory, tests and property extraction */
/* ********************************************************************** */
/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_of_one(void* abs) {
	ap_disjunction_t* res = ap_disjunction_alloc(1);
	res->p[0] = abs;
	return res;
}

#define BOTTOM_TOP(NAME,FUNID)\
ap_disjunction_t* ap_disjunction_##NAME(ap_manager_t* manager, size_t intdim, size_t realdim) \
{									\
  ap_disjunction_internal_t* intern = get_internal_init(manager);	\
  ap_manager_t* man = intern->manager;	   \
	void* (*ptr)(ap_manager_t*,...) = man->funptr[FUNID];		\
    void* abs = ptr(man,intdim,realdim);				\
                      \
  return ap_disjunction_of_one(abs);								\
}

BOTTOM_TOP(bottom,AP_FUNID_BOTTOM)
BOTTOM_TOP(top,AP_FUNID_TOP)

ap_disjunction_t* ap_disjunction_of_box(ap_manager_t* manager,
					      size_t intdim, size_t realdim,
					      ap_interval_t** tinterval)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
    void* abs;
	ap_disjunction_t* res = ap_disjunction_alloc(1);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_OF_BOX];
	bool (*is_bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];
	abs = ptr(man, intdim, realdim, tinterval);
	return ap_disjunction_of_one(abs);
}

ap_dimension_t ap_disjunction_dimension(ap_manager_t* manager,
		ap_disjunction_t* a)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;
	ap_dimension_t (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_DIMENSION];

	return ptr(man, a->p[0]);
}

/* ============================================================ */
/* II.3 Test Functions */
/* ============================================================ */

// remplacer size==1 && is_bottom(p[0])
bool ap_disjunction_is_bottom(ap_manager_t* manager, ap_disjunction_t* a)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];

	bool gres = true;

	size_t i;
	for (i = 0; i < a->size; i++) {
		bool res = ptr(man, a->p[i]);
		gres = gres && res;
	}
	return gres;
}

bool ap_disjunction_is_top(ap_manager_t* manager, ap_disjunction_t* a)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
			AP_FUNID_IS_TOP, NULL);
	return false;
}

/* ======================================================================= */

bool ap_disjunction_is_eq(ap_manager_t* manager, ap_disjunction_t* a,
		ap_disjunction_t* b)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_IS_EQ,
			NULL);
	return false;
}

bool ap_disjunction_is_leq(ap_manager_t* manager, ap_disjunction_t* a,
		ap_disjunction_t* b)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
			AP_FUNID_IS_LEQ, NULL);
	return false;
}

bool ap_disjunction_sat_lincons(ap_manager_t* manager, ap_disjunction_t* a,
				ap_lincons0_t* lincons)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_SAT_LINCONS];

	bool gres = false;

	size_t i;
	for (i = 0; i < a->size; i++) {
		bool res = ptr(man, a->p[i], lincons);
		gres = gres || res;
		if (gres)
			break;
	}
	return gres;
}

bool ap_disjunction_sat_tcons(ap_manager_t* manager, ap_disjunction_t* a,
		ap_tcons0_t* tcons)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_SAT_TCONS];

	bool gres = false;

	size_t i;
	for (i = 0; i < a->size; i++) {
		bool res = ptr(man, a->p[i], tcons);
		gres = gres || res;
		if (gres)
			break;
	}
	return gres;
}

bool ap_disjunction_sat_interval(ap_manager_t* manager, ap_disjunction_t* a,
			ap_dim_t dim, ap_interval_t* interval)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	bool (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_SAT_INTERVAL];

	bool gres = false;

	size_t i;
	for (i = 0; i < a->size; i++) {
		bool res = ptr(man, a->p[i], dim, interval);
		gres = gres || res;
		if (gres)
			break;
	}
	return gres;
}

bool ap_disjunction_is_dimension_unconstrained(ap_manager_t* manager,
		ap_disjunction_t* a, ap_dim_t dim)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_IS_DIMENSION_UNCONSTRAINED,
				NULL);
	return false;
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* ap_disjunction_bound_linexpr(ap_manager_t* manager,
		ap_disjunction_t* a, ap_linexpr0_t* expr)

{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_interval_t* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_BOUND_LINEXPR];

	ap_interval_t* gres = ap_interval_alloc();
	ap_interval_set_bottom(gres);

	size_t i;
	for (i = 0; i < a->size; i++) {
		ap_interval_t* res = ptr(man, a->p[i], expr);
		ap_interval_join_with(gres, res);
		ap_interval_free(res);
		if (ap_interval_is_bottom(gres))
			break;
	}
	return gres;
}

ap_interval_t* ap_disjunction_bound_texpr(ap_manager_t* manager,
		ap_disjunction_t* a, ap_texpr0_t* expr)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_interval_t* gres = ap_interval_alloc();
	ap_interval_set_bottom(gres);

	ap_interval_t* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_BOUND_TEXPR];

	size_t i;
	for (i = 0; i < a->size; i++) {
		ap_interval_t* res = ptr(man, a->p[i], expr);
		ap_interval_join_with(gres, res);
		ap_interval_free(res);
		if (ap_interval_is_bottom(gres))
			break;
	}
	return gres;
}

ap_interval_t* ap_disjunction_bound_dimension(ap_manager_t* manager,
		ap_disjunction_t* a, ap_dim_t dim)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_interval_t* gres = ap_interval_alloc();
	ap_interval_set_bottom(gres);

	ap_interval_t* (*ptr)(ap_manager_t*, ...) =	man->funptr[AP_FUNID_BOUND_DIMENSION];

	size_t i;
	for (i = 0; i < a->size; i++) {
		ap_interval_t* res = ptr(man, a->p[i], dim);
		ap_interval_join_with(gres, res);
		ap_interval_free(res);
		if (ap_interval_is_bottom(gres))   /* ?*/
			break;
	}
	return gres;
}

// Convert abstract value 'a' to a disjunction of linear constraints.
/* ===================================================================== */

ap_lincons0_array_t ap_disjunction_to_lincons_array(ap_manager_t* manager,
		ap_disjunction_t* a)
{

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_lincons0_array_t garray;
	garray.p = NULL;
	garray.size = 0;

	ap_lincons0_array_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];

	size_t i,j;
	for (i=0;i<a->size;i++){
	    ap_lincons0_array_t array = ptr(man,a->p[i]);
	    garray.p = realloc(garray.p, (garray.size + array.size) * sizeof(ap_lincons0_t));
	    for (j=0;j<array.size;j++){
	      garray.p[garray.size + j] = array.p[j];
	    }
	    garray.size += array.size;
	    free(array.p);
	  }
	  return garray;
}

ap_tcons0_array_t ap_disjunction_to_tcons_array(ap_manager_t* manager,
		ap_disjunction_t* a)
{
	ap_tcons0_array_t garray;
	garray.p = NULL;
	garray.size = 0;

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_tcons0_array_t (*ptr)(ap_manager_t*,...) = man->funptr[AP_FUNID_TO_TCONS_ARRAY];

	size_t i,j;
	for (i=0;i<a->size;i++){
	  ap_tcons0_array_t array = ptr(man,a->p[i]);
	    garray.p = realloc(garray.p, (garray.size + array.size) * sizeof(ap_tcons0_t));
	    for (j=0;j<array.size;j++){
	      garray.p[garray.size + j] = array.p[j];
	    }
	    garray.size += array.size;
	    free(array.p);
	  }
	  return garray;
}

//Convert 'a' to an interval/hyper-cube.
/* ====================================================================== */

ap_interval_t** ap_disjunction_to_box(ap_manager_t* manager,
						ap_disjunction_t* a)
{

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_interval_t** gbox = NULL;
	size_t nbdims = 0;

    ap_interval_t** (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_TO_BOX];
    ap_dimension_t (*ptrdimension)(ap_manager_t*, ...) = man->funptr[AP_FUNID_DIMENSION];
    ap_dimension_t dimension = ptrdimension(man, a->p[0]);
    nbdims = dimension.intdim + dimension.realdim;

    size_t i, j;
    for (i = 0; i < a->size; i++) {
		ap_interval_t** box = ptr(man, a->p[i]);
		if (i == 0) {
			gbox = box;
		} else {
			for (j = 0; j < nbdims; j++) {
				ap_interval_join_with(gbox[j], box[j]);
				if (ap_interval_is_bottom(gbox[j]))     /* ? */
					break;
			}
			ap_interval_array_free(box, nbdims);
			if (j < nbdims) {
				for (j = 0; j < nbdims; j++)
					ap_interval_set_bottom(gbox[j]);    /* ? */
				break;
			}
		}
	}
	return gbox;
}

//Convert 'a' to an array of generators.
/* ====================================================================== */

ap_generator0_array_t ap_disjunction_to_generator_array(ap_manager_t* manager,
		ap_disjunction_t* a)
{
	ap_generator0_array_t array;

	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,	AP_FUNID_TO_GENERATOR_ARRAY, NULL);

	array.p = NULL;
	array.size = 0;

	return array;
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

/* Extra functions */

ap_disjunction_t* ap_disjunction_resize(ap_manager_t* man,
							ap_disjunction_t* a)
{
	void (*free)(ap_manager_t*, ...) = man->funptr[AP_FUNID_FREE];

	size_t i,j;
	i=0; j=0;
	while (i < a->size) {
		if (a->p[i] == NULL){
			if (j == 0) j=i;
			while ((i< a->size) && (a->p[i] == NULL)){
				i=i+1;
			}
			if (i< a->size){
				a->p[j] = a->p[i];
				free(man, a->p[i]);
				a->p[i] = NULL;
				j++;
			}
		} else i++;
	}
	res=realloc(res,i*sizeof(a));

  return res;
}

/* ************************************************************** */

ap_disjunction_t* ap_disjunction_elim_redundant(ap_manager_t* man, ap_disjunction_t* a)
{
	void (*free)(ap_manager_t*, ...) = man->funptr[AP_FUNID_FREE];

	size_t i,j;
	for (i = 0; i < a->size; i++) {
		if (a->p[i] != NULL){
			for (j = i+1; j < a->size; j++){
				if(a->p[i] == a->p[j]) {
					free(man,a->p[j]);
					a->p[j]=NULL;
				}

			}
		}
	}

	return ap_disjunction_resize(man, a);
}

/* ************************************************************** */

ap_disjunction_t* ap_disjunction_reduce_top_bottom(ap_manager_t* man,
							ap_disjunction_t* a)
{
	bool first_top = true;

	bool (*is_bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];
	bool (*is_top)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_TOP];
	void (*free)(ap_manager_t*, ...) = man->funptr[AP_FUNID_FREE];

	size_t i,j;
	for (i = 0; i < a->size; i++) {
		if (a->p[i] != NULL){
				if(is_bottom(man ,a->p[i])) {
					free(man, a->p[i]);
					a->p[i]=NULL;

				}
				if(is_top(man ,a->p[i])) {
					if (first_top == true) first_top = false;
					else {
						free(man, a->p[i]);
						a->p[i]=NULL;
					}

				}
		}
	}
  return ap_disjunction_resize(man, a);
}

//Meet and Join of two abstract values
/* ====================================================================== */

ap_disjunction_t* ap_disjunction_join(ap_manager_t* manager,
                            bool destructive,
                            ap_disjunction_t* a1,
                            ap_disjunction_t* a2)
{

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	size_t i, j, res_size;
    res_size = a1->size + a2->size;

    ap_disjunction_t* res = ap_disjunction_alloc(res_size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_JOIN];


	for (i = 0; i < res_size; i++) {
		res->p[i] = ptr(man, destructive, a1->p[i], a2->p[i]);
	}

	res = ap_disjunction_elim_redundant(man, res);

	return ap_disjunction_reduce_top_bottom(man, res);
}

ap_disjunction_t* ap_disjunction_meet(ap_manager_t* manager, bool destructive,
		ap_disjunction_t* a1, ap_disjunction_t* a2)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_MEET, NULL);
	return NULL;
}

ap_disjunction_t* ap_disjunction_meetjoin_array(ap_funid_t funid,
/* either meet or join */
ap_manager_t* manager, ap_disjunction_t** tab, size_t size)
{

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i, j;
	ap_disjunction_t* res;
	void** a;
	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[funid];
	res = ap_disjunction_alloc(sizeof(tab));
	a = (void**) malloc(size * sizeof(void*));
	bool (*is_bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];
	for (i = 0; i < sizeof(tab); i++) {
		for (j = 0; j < size; j++) {
			a[j] = tab[j]->p[i];
		}
		res->p[i] = ptr(man, a, size);
		if (funid == AP_FUNID_MEET_ARRAY) {
			if (is_bottom(man, res->p[i])) {
				set_bottom(intern, false, res, i);
				goto ap_disjunction_meetjoin_array_exit;
			}
		}
	}

	ap_disjunction_meetjoin_array_exit: free(a);
	return res;
}

ap_disjunction_t* ap_disjunction_meet_array(ap_manager_t* manager,
		ap_disjunction_t** tab, size_t size)
{
	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED, AP_FUNID_MEET_ARRAY, NULL);
	return NULL;
}

ap_disjunction_t* ap_disjunction_join_array(ap_manager_t* manager,
		ap_disjunction_t** tab, size_t size)
{
	return ap_disjunction_meetjoin_array(AP_FUNID_JOIN_ARRAY, manager, tab,
			size);
}

//Meet of the abstract value a with the set of constraints array.
/* ************************************************************************* */

ap_disjunction_t* ap_disjunction_meet_lincons_array(ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_lincons0_array_t* array)
{

	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;


	ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
	bool (*is_bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];
	bool (*is_top)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_TOP];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], array);
		if (is_bottom(man, res->p[i])) {
			set_bottom(intern, destructive, res, i);
			break;
		}
		if(is_top(man, res->p[i])) res->p[i]=NULL;
	}
	return res;
}

ap_disjunction_t* ap_disjunction_meet_tcons_array(ap_manager_t* manager,bool destructive,
								ap_disjunction_t* a, ap_tcons0_array_t* array)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_MEET_TCONS_ARRAY];
	bool (*is_bottom)(ap_manager_t*, ...) = man->funptr[AP_FUNID_IS_BOTTOM];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], array);
		if (is_bottom(man, res->p[i])) {
			set_bottom(intern, destructive, res, i);
			break;
		}
	}
	return res;
}

//  ????????????????????????
/* ************************************************************************ */
ap_disjunction_t* ap_disjunction_add_ray_array(ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_lincons0_array_t* array)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_ADD_RAY_ARRAY];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], array);
	}
	return res;
}

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_disjunction_t*
ap_disjunction_asssub_linexpr_array(ap_funid_t funid,
/* either assign or substitute */
ap_manager_t* manager, bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res= destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[funid];
	bool (*is_bottom)(ap_manager_t*, ...) =	man->funptr[AP_FUNID_IS_BOTTOM];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], tdim, texpr, size,
				dest ? dest->p[i] : NULL);
		if (dest || funid == AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY) {
			if (is_bottom(man, res->p[i])) {
				set_bottom(intern, destructive, res, i);
				break;
			}
		}
	}
	return res;
}

ap_disjunction_t* ap_disjunction_assign_linexpr_array(
		ap_manager_t* man,
		bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
	    return ap_disjunction_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
		man, destructive, a, tdim, texpr, size, dest);
}

ap_disjunction_t* ap_disjunction_substitute_linexpr_array(ap_manager_t* man,
		bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_linexpr0_t** texpr, size_t size, ap_disjunction_t* dest) {
	return ap_disjunction_asssub_linexpr_array(
			AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY, man, destructive, a, tdim,
			texpr, size, dest);
}

ap_disjunction_t*
ap_disjunction_asssub_texpr_array(ap_funid_t funid,
/* either assign or substitute */
ap_manager_t* manager, bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest) {
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i;
	ap_disjunction_t* res;

	res = destructive ? a : ap_disjunction_alloc(a->size);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[funid];
	bool (*is_bottom)(ap_manager_t*, ...) = 	man->funptr[AP_FUNID_IS_BOTTOM];

	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], tdim, texpr, size,
				dest ? dest->p[i] : NULL);
		if (dest || funid == AP_FUNID_SUBSTITUTE_TEXPR_ARRAY) {
			if (is_bottom(man, res->p[i])) {
				set_bottom(intern, destructive, res, i);
				break;
			}
		}
	}
	return res;
}

ap_disjunction_t* ap_disjunction_assign_texpr_array(ap_manager_t* man,
		bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
	return ap_disjunction_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY, man,
			destructive, a, tdim, texpr, size, dest);
}

ap_disjunction_t* ap_disjunction_substitute_texpr_array(ap_manager_t* man,
		bool destructive, ap_disjunction_t* a, ap_dim_t* tdim,
		ap_texpr0_t** texpr, size_t size, ap_disjunction_t* dest)
{
	return ap_disjunction_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
			man, destructive, a, tdim, texpr, size, dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_forget_array(
		ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_dim_t* tdim, size_t size,
		bool project)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i;
	ap_disjunction_t* res;

	res = destructive ? a : ap_disjunction_alloc(a->size);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_FORGET_ARRAY];

	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], tdim, size, project);
	}
	return res;
}

ap_disjunction_t* ap_disjunction_add_dimensions(
		ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_dimchange_t* dimchange,
		bool project)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i;
	ap_disjunction_t* res;

	res = destructive ? a : ap_disjunction_alloc(a->size);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_ADD_DIMENSIONS];

	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], dimchange, project);
	}
	return res;
}

ap_disjunction_t* ap_disjunction_remove_dimensions(ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_dimchange_t* dimchange)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i;
	ap_disjunction_t* res;

	res = destructive ? a : ap_disjunction_alloc(a->size);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_REMOVE_DIMENSIONS];

	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], dimchange);
	}
	return res;
}

ap_disjunction_t* ap_disjunction_permute_dimensions(ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a, ap_dimperm_t* perm)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	size_t i;
	ap_disjunction_t* res;

	res = destructive ? a : ap_disjunction_alloc(a->size);

	ap_manager_t* man = intern->manager;
	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_PERMUTE_DIMENSIONS];

	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], perm);
	}
	return res;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

//expanding
/* ***************************************************************************** */
ap_disjunction_t* ap_disjunction_expand(ap_manager_t* manager,
						bool destructive,
						ap_disjunction_t* a,
						ap_dim_t dim,
						size_t n)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_EXPAND];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], dim, n);
	}
	return res;
}

//folding
/* ***************************************************************************** */
ap_disjunction_t* ap_disjunction_fold(ap_manager_t* manager,
						bool destructive,
						ap_disjunction_t* a,
						ap_dim_t* tdim,
						size_t size)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_FOLD];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i], tdim, size);
	}
	return res;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

// disjunctive widening
/* *************************************************************************** */
void* ap_disjunction_widening(ap_manager_t* manager,
		ap_disjunction_t* a1, ap_disjunction_t* a2)
{

	ap_manager_raise_exception(manager, AP_EXC_NOT_IMPLEMENTED,
		AP_FUNID_WIDENING, NULL);
    return false;

}

/* ============================================================ */
/* III.6 Merge function */
/* ============================================================ */

void* ap_disjunction_merge (ap_manager_t* manager, ap_disjunction_t* a)

{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = ap_disjunction_alloc(a->size);

    res = ap_disjunction_elim_redundant(man, a);

    return ap_disjunction_reduce_top_bottom(man, res);
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

ap_disjunction_t* ap_disjunction_closure(ap_manager_t* manager,
		bool destructive, ap_disjunction_t* a)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = destructive ? a : ap_disjunction_alloc(a->size);

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_CLOSURE];

	size_t i;
	for (i = 0; i < a->size; i++) {
		res->p[i] = ptr(man, destructive, a->p[i]);
	}
	return res;
}

/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

void ap_disjunction_internal_free(void* p)
{
	ap_disjunction_internal_t* intern = (ap_disjunction_internal_t*) p;

	ap_manager_free(intern->manager);
	intern->manager = NULL;

	free(intern->version);
	free(intern);
}

ap_manager_t* ap_disjunction_manager_alloc(char* library, ap_manager_t** tab,
		size_t size, void(*merge)(ap_manager_t*, ap_disjunction_t*), /* disjunction function */
		void(*widening)(ap_manager_t*, ap_disjunction_t*, ap_disjunction_t*), /* widening function */
		void(*approximate)(ap_manager_t*, ap_disjunction_t*, int n) /* approximate function */
)
{
	char* version;
	size_t i, index, length;
	ap_disjunction_internal_t* internal;
	ap_manager_t* man;
	ap_funid_t funid;
	void** funptr;

	assert(size>=2);
	if (size < 2)
		return NULL;

	/* Creating libray name and version */
	length = 100 + 2 * size;
	for (i = 0; i < size; i++)
		length += strlen(tab[i]->version);
	version = malloc(length * sizeof(char));
	index = sprintf(version, "%s, %s", tab[0]->version, tab[1]->version);
	for (i = 2; i < size; i++) {
		index += sprintf(version + index, ", %s", tab[i]->version);
	}
	/* creating internal */
	internal = malloc(sizeof(ap_disjunction_internal_t) + size
			* sizeof(ap_manager_t*));

	internal->manager = ap_manager_copy(tab[i]);

	internal->approximate = approximate;
	internal->library = library;
	internal->version = strdup(version);
	free(version);

	/* allocating managers */
	man = ap_manager_alloc(internal->library, internal->version, internal,
			&ap_disjunction_internal_free);
	/* default options */
	for (funid = 0; funid < AP_FUNID_SIZE; funid++) {
		man->option.funopt[funid].algorithm = 0x2;
	}
	man->option.funopt[AP_FUNID_COPY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_FREE].algorithm = 0x0;
	man->option.funopt[AP_FUNID_ASIZE].algorithm = 0x0;
	man->option.funopt[AP_FUNID_COPY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_MINIMIZE].algorithm = 0x1;
	man->option.funopt[AP_FUNID_IS_BOTTOM].algorithm = 0x1;
	man->option.funopt[AP_FUNID_IS_TOP].algorithm = 0x1;
	man->option.funopt[AP_FUNID_IS_LEQ].algorithm = 0x1;
	man->option.funopt[AP_FUNID_IS_EQ].algorithm = 0x1;
	man->option.funopt[AP_FUNID_SAT_LINCONS].algorithm = 0x1;
	man->option.funopt[AP_FUNID_SAT_TCONS].algorithm = 0x1;
	man->option.funopt[AP_FUNID_SAT_INTERVAL].algorithm = 0x1;
	man->option.funopt[AP_FUNID_IS_DIMENSION_UNCONSTRAINED].algorithm = 0x1;
	man->option.funopt[AP_FUNID_BOUND_LINEXPR].algorithm = 0x1;
	man->option.funopt[AP_FUNID_BOUND_TEXPR].algorithm = 0x1;
	man->option.funopt[AP_FUNID_BOUND_DIMENSION].algorithm = 0x1;
	man->option.funopt[AP_FUNID_TO_LINCONS_ARRAY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_TO_TCONS_ARRAY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_TO_BOX].algorithm = 0x1;
	man->option.funopt[AP_FUNID_TO_GENERATOR_ARRAY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_JOIN].algorithm = 0x1;
	man->option.funopt[AP_FUNID_JOIN_ARRAY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_ADD_RAY_ARRAY].algorithm = 0x1;
	man->option.funopt[AP_FUNID_FORGET_ARRAY].algorithm = 0x3;
	man->option.funopt[AP_FUNID_ADD_DIMENSIONS].algorithm = 0x0;
	man->option.funopt[AP_FUNID_PERMUTE_DIMENSIONS].algorithm = 0x0;
	man->option.funopt[AP_FUNID_EXPAND].algorithm = 0x1;
	man->option.funopt[AP_FUNID_WIDENING].algorithm = 0x0;
	man->option.funopt[AP_FUNID_CLOSURE].algorithm = 0x1;

	/* Virtual table */
	funptr = man->funptr;

	funptr[AP_FUNID_COPY] = &ap_disjunction_copy;

	funptr[AP_FUNID_APPROXIMATE] = &ap_disjunction_approximate;
	funptr[AP_FUNID_BOTTOM] = &ap_disjunction_bottom;
	funptr[AP_FUNID_TOP] = &ap_disjunction_top;
	funptr[AP_FUNID_OF_BOX] = &ap_disjunction_of_box;
	funptr[AP_FUNID_DIMENSION] = &ap_disjunction_dimension;
	funptr[AP_FUNID_IS_BOTTOM] = &ap_disjunction_is_bottom;
	funptr[AP_FUNID_IS_TOP] = &ap_disjunction_is_top;
	funptr[AP_FUNID_IS_LEQ] = &ap_disjunction_is_leq;
	funptr[AP_FUNID_IS_EQ] = &ap_disjunction_is_eq;
	funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED]
			= &ap_disjunction_is_dimension_unconstrained;
	funptr[AP_FUNID_SAT_INTERVAL] = &ap_disjunction_sat_interval;
	funptr[AP_FUNID_SAT_LINCONS] = &ap_disjunction_sat_lincons;
	funptr[AP_FUNID_SAT_TCONS] = &ap_disjunction_sat_tcons;
	funptr[AP_FUNID_BOUND_DIMENSION] = &ap_disjunction_bound_dimension;
	funptr[AP_FUNID_BOUND_LINEXPR] = &ap_disjunction_bound_linexpr;
	funptr[AP_FUNID_BOUND_TEXPR] = &ap_disjunction_bound_texpr;
	funptr[AP_FUNID_TO_BOX] = &ap_disjunction_to_box;
	funptr[AP_FUNID_TO_LINCONS_ARRAY] = &ap_disjunction_to_lincons_array;
	funptr[AP_FUNID_TO_TCONS_ARRAY] = &ap_disjunction_to_tcons_array;
	funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &ap_disjunction_to_generator_array;
	funptr[AP_FUNID_MEET] = &ap_disjunction_meet;
	funptr[AP_FUNID_MEET_ARRAY] = &ap_disjunction_meet_array;
	funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &ap_disjunction_meet_lincons_array;
	funptr[AP_FUNID_MEET_TCONS_ARRAY] = &ap_disjunction_meet_tcons_array;
	funptr[AP_FUNID_JOIN] = &ap_disjunction_join;
	funptr[AP_FUNID_JOIN_ARRAY] = &ap_disjunction_join_array;
	funptr[AP_FUNID_ADD_RAY_ARRAY] = &ap_disjunction_add_ray_array;
	funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY]
			= &ap_disjunction_assign_linexpr_array;
	funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY]
			= &ap_disjunction_substitute_linexpr_array;
	funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &ap_disjunction_assign_texpr_array;
	funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY]
			= &ap_disjunction_substitute_texpr_array;
	funptr[AP_FUNID_ADD_DIMENSIONS] = &ap_disjunction_add_dimensions;
	funptr[AP_FUNID_REMOVE_DIMENSIONS] = &ap_disjunction_remove_dimensions;
	funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &ap_disjunction_permute_dimensions;
	funptr[AP_FUNID_FORGET_ARRAY] = &ap_disjunction_forget_array;
	funptr[AP_FUNID_EXPAND] = &ap_disjunction_expand;
	funptr[AP_FUNID_FOLD] = &ap_disjunction_fold;
	funptr[AP_FUNID_WIDENING] = &ap_disjunction_widening;
	funptr[AP_FUNID_CLOSURE] = &ap_disjunction_closure;

	return man;
}

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

/* Decompose a disjunctive abstract value into an array of abstract values */
void** ap_disjunction_decompose(ap_manager_t* manager, bool destructive,
		ap_disjunction_t* a, size_t* psize)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_COPY];

	void** res = (void**) malloc(a->size * (*psize));

	size_t i;
	for (i = 0; i < a->size; i++) {
		if (destructive) {
			res[i] = a->p[i];
		} else {
			res[i] = ptr(man, a->p[i]);
		}
	}
	if (destructive)
		free(a);
	return res;
}

/* Create a disjunctive abstract value from an array */
ap_disjunction_t* ap_disjunction_compose(ap_manager_t* manager,
		bool destructive, void** tabs)
{
	ap_disjunction_internal_t* intern = get_internal_init(manager);
	ap_manager_t* man = intern->manager;

	ap_disjunction_t* res = ap_disjunction_alloc(sizeof(tabs));

	void* (*ptr)(ap_manager_t*, ...) = man->funptr[AP_FUNID_COPY];

	size_t i;
	for (i = 0; i < sizeof(tabs); i++) {
		if (destructive) {
			res->p[i] = tabs[i];
		} else {
			res->p[i] = ptr(man, tabs[i]);
		}
	}
	return res;
}
