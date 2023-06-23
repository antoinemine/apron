/* ************************************************************************* */
/* ap_reducedproduct.c: generic reduced product of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_global0.h"
#include "ap_reducedproduct.h"

/* ------------------------------------------------------------ */
/* Allocates an abstract value */
static inline
ap_reducedproduct_t* ap_reducedproduct_alloc(size_t size)
{
  size_t i;
  ap_reducedproduct_t* res = malloc(sizeof(ap_reducedproduct_t) + size*sizeof(void*));
  res->reduced = false;
  for (i=0; i<size; i++) res->p[i] = NULL;
  return res;
}

/* ------------------------------------------------------------ */
/* Get internal and optionally reduce the product(s) */
static inline ap_reducedproduct_internal_t* get_internal_init0(ap_manager_t* manager)
{
  return (ap_reducedproduct_internal_t*)manager->internal;
}
static inline ap_reducedproduct_internal_t* get_internal_init1(ap_manager_t* manager,
							       ap_funid_t funid, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  if (a->reduced==false && manager->option.funopt[funid].algorithm & 0x1){
    intern->reduce(manager,a);
  }
  return intern;
}
static inline ap_reducedproduct_internal_t* get_internal_init2(ap_manager_t* manager,
							       ap_funid_t funid,
							       ap_reducedproduct_t* a, ap_reducedproduct_t* b)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  if (manager->option.funopt[funid].algorithm & 0x1){
    if (a->reduced==false) intern->reduce(manager,a);
    if (b && b->reduced==false) intern->reduce(manager,b);
  }
  return intern;
}
static inline
ap_reducedproduct_internal_t* get_internal_init_tab(ap_manager_t* manager,
						    ap_funid_t funid,
						    ap_reducedproduct_t** tab, size_t size)
{
  size_t i;
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  if (manager->option.funopt[funid].algorithm & 0x1){
    for (i=0; i<size; i++){
      if (tab[i]->reduced==false) intern->reduce(manager,tab[i]);
    }
  }
  return intern;
}

/* ------------------------------------------------------------ */
static
void collect_results0(ap_manager_t* manager)
{
  size_t i;
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  ap_result_t* gresult = &manager->result;
  gresult->flag_best = true;
  gresult->flag_exact = true;
  for (i=0;i<intern->size; i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_result_t* result = &man->result;
    gresult->flag_exact = gresult->flag_exact && result->flag_exact;

    if (result->exclog != NULL){
      ap_exclog_t* last = result->exclog;
      while (true){
        size_t sz = strlen(man->library)+(last->msg ? strlen(last->msg) : 0)+3;
	char* buffer = malloc(sz);
	snprintf(buffer,sizeof(sz),"%s: %s",
		man->library,(last->msg ? "" : last->msg));
	if (last->msg) free(last->msg);
	last->msg = buffer;
	if (last->tail)
	  last = last->tail;
	else
	  break;
      }
      last->tail = gresult->exclog;
      gresult->exclog = result->exclog;
      result->exclog = NULL;
    }
  }
}
static
void collect_results1(ap_manager_t* manager, ap_funid_t funid, ap_reducedproduct_t* a)
{
  collect_results0(manager);
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  if (a->reduced==false && manager->option.funopt[funid].algorithm & 0x2){
    intern->reduce(manager,a);
    a->reduced = true;
  }
}

/* ------------------------------------------------------------ */
/* Fills the value with bottom.  It is assumed that a->p[index] is already bottom.
   If destructive is true, then a->p[i] for i>index may be not NULL and is cleaned
   first.
*/
static void set_bottom(ap_reducedproduct_internal_t* intern,
		       bool destructive,
		       ap_reducedproduct_t* a,
		       size_t index            /* index of the already bottom dimension */
		       )
{
  size_t i;

  ap_manager_t* man = intern->tmanagers[index];
  ap_dimension_t (*dimension)(ap_manager_t*,void*) = man->funptr[AP_FUNID_DIMENSION];
  ap_dimension_t dim = dimension(man,a->p[index]);
  for (i=0; i<intern->size; i++){
    if (i!=index){
      man = intern->tmanagers[i];
      if (i<index || destructive){
	void (*free)(ap_manager_t*,void*) = man->funptr[AP_FUNID_FREE];
	free(man,a->p[i]);
      }
      void* (*bottom)(ap_manager_t*,int,int) = man->funptr[AP_FUNID_BOTTOM];
      a->p[i] = bottom(man, dim.intdim, dim.realdim);
    }
  }
  a->reduced = true;
}

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

#define VOID_MAN_VAL(NAME,FUNID)					\
void ap_reducedproduct_##NAME(ap_manager_t* manager, ap_reducedproduct_t* a) \
{									\
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,FUNID,a); \
  size_t i;								\
									\
  for (i=0;i<intern->size;i++){						\
    ap_manager_t* man = intern->tmanagers[i];				\
    void (*ptr)(ap_manager_t*,void*) = man->funptr[FUNID];		\
    ptr(man,a->p[i]);							\
  }									\
  collect_results1(manager,FUNID,a);					\
}


/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

ap_reducedproduct_t* ap_reducedproduct_copy(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_COPY,a);
  size_t i;
  ap_reducedproduct_t* res = ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_COPY];
    res->p[i] = ptr(man,a->p[i]);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_COPY,res);
  return res;
}

void ap_reducedproduct_free(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = 
    get_internal_init0(manager);
  size_t i;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_FREE];
    ptr(man,a->p[i]);
  }
  free(a);
  collect_results0(manager);
}

size_t ap_reducedproduct_size(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_ASIZE,a);
  size_t i;
  size_t res = 0;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    size_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_ASIZE];
    res += ptr(man,a->p[i]);
  }
  collect_results0(manager);
  return res;
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

VOID_MAN_VAL(minimize,AP_FUNID_MINIMIZE)
VOID_MAN_VAL(canonicalize,AP_FUNID_CANONICALIZE)
int ap_reducedproduct_hash(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_HASH,a);
  size_t i;
  int res = 0;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    size_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_HASH];
    res += ptr(man,a->p[i]);
  }
  collect_results0(manager);
  return res;
}

void ap_reducedproduct_approximate(ap_manager_t* manager, ap_reducedproduct_t* a, int n)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_APPROXIMATE,a);
  intern->approximate(manager,(void*)a,n);
  collect_results0(manager);
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */
void ap_reducedproduct_fprint(FILE* stream, ap_manager_t* manager,
			      ap_reducedproduct_t* a,
			      char** name_of_dim)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i;

  fprintf(stream,"reduced product of library %s\n",manager->library);
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void (*ptr)(FILE* stream, ap_manager_t*,void*,char**) = man->funptr[AP_FUNID_FPRINT];
    ptr(stream,man,a->p[i],name_of_dim);
  }
  collect_results0(manager);
}
void ap_reducedproduct_fprintdiff(FILE* stream, ap_manager_t* manager,
				  ap_reducedproduct_t* a, ap_reducedproduct_t* b,
				  char** name_of_dim)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i;

  fprintf(stream,"reduced product of library %s\n",manager->library);
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void (*ptr)(FILE* stream, ap_manager_t*,void*,void*,char**) = man->funptr[AP_FUNID_FPRINTDIFF];
    ptr(stream,man,a->p[i],b->p[i],name_of_dim);
  }
  collect_results0(manager);
}
void ap_reducedproduct_fdump(FILE* stream, ap_manager_t* manager,
			     ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i;

  fprintf(stream,"reduced product of library %s\n",manager->library);
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void (*ptr)(FILE* stream, ap_manager_t*,void*) = man->funptr[AP_FUNID_FDUMP];
    ptr(stream,man,a->p[i]);
  }
  collect_results0(manager);
}

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t ap_reducedproduct_serialize_raw(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i, index;
  ap_membuf_t gres;

  gres.ptr = NULL;
  gres.size = 0;
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_membuf_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_SERIALIZE_RAW];
    ap_membuf_t res = ptr(man,a->p[i]);
    gres.ptr = realloc(gres.ptr, gres.size + res.size);
    memcpy(((char*)gres.ptr + gres.size), res.ptr, res.size);
    gres.size += res.size;
    free(res.ptr);
  }
  collect_results0(manager);
  return gres;
}
ap_reducedproduct_t* ap_reducedproduct_deserialize_raw(ap_manager_t* manager, void* p, size_t* size)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i,nb,dummy;
  ap_reducedproduct_t* res = ap_reducedproduct_alloc(intern->size);

  if (size==NULL) size = &dummy;
  *size = 0;
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,char*,size_t*) = man->funptr[AP_FUNID_DESERIALIZE_RAW];
    size_t nb;
    res->p[i] = ptr(man,((char*)p + *size), &nb);
    *size += nb;
  }
  collect_results0(manager);
  return res;
}

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */
/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

#define BOTTOM_TOP(NAME,FUNID)					\
ap_reducedproduct_t* ap_reducedproduct_##NAME(ap_manager_t* manager, size_t intdim, size_t realdim) \
{									\
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);	\
  size_t i;								\
  ap_reducedproduct_t* res = ap_reducedproduct_alloc(intern->size);	\
									\
  for (i=0;i<intern->size;i++){						\
    ap_manager_t* man = intern->tmanagers[i];				\
    void* (*ptr)(ap_manager_t*,size_t,size_t) = man->funptr[FUNID];		\
    res->p[i] = ptr(man,intdim,realdim);				\
  }									\
  res->reduced = true;							\
  collect_results0(manager);						\
  return res;								\
}

BOTTOM_TOP(bottom,AP_FUNID_BOTTOM)
BOTTOM_TOP(top,AP_FUNID_TOP)

ap_reducedproduct_t* ap_reducedproduct_of_box(ap_manager_t* manager,
					      size_t intdim, size_t realdim,
					      ap_interval_t** tinterval)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i;
  ap_reducedproduct_t* res = ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,size_t,size_t,ap_interval_t**) = man->funptr[AP_FUNID_OF_BOX];
    res->p[i] = ptr(man,intdim,realdim,tinterval);
    bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
    if (is_bottom(man,res->p[i])){
      set_bottom(intern,false,res,i);
      break;
    }
  }
  collect_results1(manager,AP_FUNID_OF_BOX,res);
  return res;
}

ap_dimension_t ap_reducedproduct_dimension(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  ap_manager_t* man = intern->tmanagers[0];
  ap_dimension_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_DIMENSION];
  return ptr(man,a->p[0]);
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool ap_reducedproduct_is_bottom(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_IS_BOTTOM,a);
  size_t i;
  bool gres = false;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
    bool res = ptr(man,a->p[i]);
    gres = gres || res;
    if (gres) break;
  }
  collect_results0(manager);
  return gres;
}
bool ap_reducedproduct_is_top(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_IS_TOP,a);
  size_t i;
  bool gres = true;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_TOP];
    bool res = ptr(man,a->p[i]);
    gres = gres && res;
    if (!gres) break;
  }
  collect_results0(manager);
  return gres;
}

#define BOOL_MAN_VAL2(NAME,FUNID)					\
bool ap_reducedproduct_##NAME(ap_manager_t* manager, ap_reducedproduct_t* a,  ap_reducedproduct_t* b) \
{									\
  ap_reducedproduct_internal_t* intern =				\
    get_internal_init2(manager,FUNID,a,b);				\
  size_t i;								\
  bool gres = true;							\
									\
  for (i=0;i<intern->size;i++){						\
    ap_manager_t* man = intern->tmanagers[i];				\
    bool (*ptr)(ap_manager_t*,void*,void*) = man->funptr[FUNID];		\
    bool res = ptr(man,a->p[i],b->p[i]);			       	\
    gres = gres && res;							\
    if (!gres) break;							\
  }									\
  collect_results0(manager);						\
  return gres;								\
}

BOOL_MAN_VAL2(is_leq,AP_FUNID_IS_LEQ)
BOOL_MAN_VAL2(is_eq,AP_FUNID_IS_EQ)

bool ap_reducedproduct_sat_lincons(ap_manager_t* manager, ap_reducedproduct_t* a, ap_lincons0_t* lincons)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_SAT_LINCONS,a);
  size_t i;
  bool gres = false;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*,ap_lincons0_t*) = man->funptr[AP_FUNID_SAT_LINCONS];
    bool res = ptr(man,a->p[i],lincons);
    gres = gres || res;
    if (gres) break;
  }
  collect_results0(manager);
  return gres;
}
bool ap_reducedproduct_sat_tcons(ap_manager_t* manager, ap_reducedproduct_t* a, ap_tcons0_t* tcons)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_SAT_TCONS,a);
  size_t i;
  bool gres = false;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*,ap_tcons0_t*) = man->funptr[AP_FUNID_SAT_TCONS];
    bool res = ptr(man,a->p[i],tcons);
    gres = gres || res;
    if (gres) break;
  }
  collect_results0(manager);
  return gres;
}
bool ap_reducedproduct_sat_interval(ap_manager_t* manager, ap_reducedproduct_t* a,
				       ap_dim_t dim, ap_interval_t* interval)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_SAT_INTERVAL,a);
  size_t i;
  bool gres = false;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*,ap_dim_t,ap_interval_t*) = man->funptr[AP_FUNID_SAT_INTERVAL];
    bool res = ptr(man,a->p[i],dim,interval);
    gres = gres || res;
    if (gres) break;
  }
  collect_results0(manager);
  return gres;
}
bool ap_reducedproduct_is_dimension_unconstrained(ap_manager_t* manager, ap_reducedproduct_t* a,
						ap_dim_t dim)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_IS_DIMENSION_UNCONSTRAINED,a);
  size_t i;
  bool gres = true;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    bool (*ptr)(ap_manager_t*,void*,ap_dim_t) = man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED];
    bool res = ptr(man,a->p[i],dim);
    gres = gres && res;
    if (!gres) break;
  }
  collect_results0(manager);
  return gres;
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

static
void ap_interval_meet_with(ap_interval_t* a, ap_interval_t* b)
{
  if (ap_scalar_cmp(a->inf,b->inf)<0){
    ap_scalar_set(a->inf,b->inf);
  }
  if (ap_scalar_cmp(a->sup,b->sup)>0){
    ap_scalar_set(a->sup,b->sup);
  }
}

ap_interval_t* ap_reducedproduct_bound_linexpr(ap_manager_t* manager,
					       ap_reducedproduct_t* a, ap_linexpr0_t* expr)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_BOUND_LINEXPR,a);
  size_t i;
  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_top(gres);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_interval_t* (*ptr)(ap_manager_t*,void*,ap_linexpr0_t*) = man->funptr[AP_FUNID_BOUND_LINEXPR];
    ap_interval_t* res = ptr(man,a->p[i],expr);
    ap_interval_meet_with(gres,res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))
      break;
  }
  collect_results0(manager);
  return gres;
}
ap_interval_t* ap_reducedproduct_bound_texpr(ap_manager_t* manager,
					       ap_reducedproduct_t* a, ap_texpr0_t* expr)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_BOUND_TEXPR,a);
  size_t i;
  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_top(gres);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_interval_t* (*ptr)(ap_manager_t*,void*,ap_texpr0_t*) = man->funptr[AP_FUNID_BOUND_TEXPR];
    ap_interval_t* res = ptr(man,a->p[i],expr);
    ap_interval_meet_with(gres,res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))
      break;
  }
  collect_results0(manager);
  return gres;
}
ap_interval_t* ap_reducedproduct_bound_dimension(ap_manager_t* manager,
						 ap_reducedproduct_t* a, ap_dim_t dim)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_BOUND_DIMENSION,a);
  size_t i;
  ap_interval_t* gres = ap_interval_alloc();
  ap_interval_set_top(gres);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_interval_t* (*ptr)(ap_manager_t*,void*,ap_dim_t) = man->funptr[AP_FUNID_BOUND_DIMENSION];
    ap_interval_t* res = ptr(man,a->p[i],dim);
    ap_interval_meet_with(gres,res);
    ap_interval_free(res);
    if (ap_interval_is_bottom(gres))
      break;
  }
  collect_results0(manager);
  return gres;
}
ap_lincons0_array_t ap_reducedproduct_to_lincons_array(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_TO_LINCONS_ARRAY,a);
  size_t i,j;
  ap_lincons0_array_t garray;

  garray.p = NULL;
  garray.size = 0;
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_lincons0_array_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_TO_LINCONS_ARRAY];
    ap_lincons0_array_t array = ptr(man,a->p[i]);
    garray.p = realloc(garray.p, (garray.size + array.size) * sizeof(ap_lincons0_t));
    for (j=0;j<array.size;j++){
      garray.p[garray.size + j] = array.p[j];
    }
    garray.size += array.size;
    free(array.p);
  }
  collect_results0(manager);
  return garray;
}
ap_tcons0_array_t ap_reducedproduct_to_tcons_array(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_TO_TCONS_ARRAY,a);
  size_t i,j;
  ap_tcons0_array_t garray;

  garray.p = NULL;
  garray.size = 0;
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_tcons0_array_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_TO_TCONS_ARRAY];
    ap_tcons0_array_t array = ptr(man,a->p[i]);
    garray.p = realloc(garray.p, (garray.size + array.size) * sizeof(ap_tcons0_t));
    for (j=0;j<array.size;j++){
      garray.p[garray.size + j] = array.p[j];
    }
    garray.size += array.size;
    free(array.p);
  }
  collect_results0(manager);
  return garray;
}
ap_interval_t** ap_reducedproduct_to_box(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_TO_BOX,a);
  size_t i,j;
  ap_interval_t** gbox = NULL;
  size_t nbdims = 0;

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    ap_interval_t** (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_TO_BOX];
    ap_interval_t** box = ptr(man,a->p[i]);
    if (i==0){
      gbox = box;
      ap_dimension_t (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_DIMENSION];
      ap_dimension_t dimension = ptr(man,a->p[i]);
      nbdims = dimension.intdim+dimension.realdim;
    }
    else {
      for (j=0; j<nbdims;j++){
	ap_interval_meet_with(gbox[j],box[j]);
	if (ap_interval_is_bottom(gbox[j]))
	  break;
      }
      ap_interval_array_free(box,nbdims);
      if (j<nbdims){
	for (j=0; j<nbdims;j++)
	  ap_interval_set_bottom(gbox[j]);
	break;
      }
    }
  }
  collect_results0(manager);
  return gbox;
}

ap_generator0_array_t ap_reducedproduct_to_generator_array(ap_manager_t* manager, ap_reducedproduct_t* a)
{
  ap_generator0_array_t array;
  (void)a;
  ap_manager_raise_exception(manager,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_TO_GENERATOR_ARRAY,NULL);
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

ap_reducedproduct_t* ap_reducedproduct_meetjoin(ap_funid_t funid,
						/* either meet or join */
						ap_manager_t* manager,
						bool destructive, ap_reducedproduct_t* a1, ap_reducedproduct_t* a2)
{
  ap_reducedproduct_internal_t* intern = get_internal_init2(manager,funid,a1,a2);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a1 : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,void*) = man->funptr[funid];
    res->p[i] = ptr(man,destructive,a1->p[i],a2->p[i]);
    if (funid==AP_FUNID_MEET){
      bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
      if (is_bottom(man,res->p[i])){
	set_bottom(intern,destructive,res,i);
	goto ap_reducedproduct_meetjoin_exit;
      }
    }
  }
  res->reduced = funid==AP_FUNID_JOIN && a1->reduced && a2->reduced;
 ap_reducedproduct_meetjoin_exit:
  collect_results1(manager,funid,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_meet(ap_manager_t* manager,
					    bool destructive, ap_reducedproduct_t* a1, ap_reducedproduct_t* a2)
{
  return ap_reducedproduct_meetjoin(AP_FUNID_MEET,manager,destructive,a1,a2);
}
ap_reducedproduct_t* ap_reducedproduct_join(ap_manager_t* manager,
					    bool destructive, ap_reducedproduct_t* a1, ap_reducedproduct_t* a2)
{
  return ap_reducedproduct_meetjoin(AP_FUNID_JOIN,manager,destructive,a1,a2);
}

ap_reducedproduct_t* ap_reducedproduct_meetjoin_array(ap_funid_t funid,
						      /* either meet or join */
						      ap_manager_t* manager,
						      ap_reducedproduct_t** tab, size_t size)
{
  ap_reducedproduct_internal_t* intern = get_internal_init_tab(manager,funid,tab,size);
  size_t i,j;
  ap_reducedproduct_t* res;
  void** a;

  res = ap_reducedproduct_alloc(intern->size);
  a = (void**)malloc(size*sizeof(void*));
  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,void**,size_t) = man->funptr[funid];
    for (j=0;j<size;j++){
      a[j] = tab[j]->p[i];
    }
    res->p[i] = ptr(man,a,size);
    if (funid==AP_FUNID_MEET_ARRAY){
      bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
      if (is_bottom(man,res->p[i])){
	set_bottom(intern,false,res,i);
	goto ap_reducedproduct_meetjoin_array_exit;
      }
    }
  }
  if (funid==AP_FUNID_JOIN){
    res->reduced = tab[0]->reduced; i = 1;
    while (res->reduced && i<size){
      res->reduced = tab[i]->reduced;
      i++;
    }
  }
  else {
    res->reduced = false;
  }
 ap_reducedproduct_meetjoin_array_exit:
  free(a);
  collect_results1(manager,funid,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_meet_array(ap_manager_t* manager,
						  ap_reducedproduct_t** tab, size_t size)
{
  return ap_reducedproduct_meetjoin_array(AP_FUNID_MEET_ARRAY,manager,tab,size);
}
ap_reducedproduct_t* ap_reducedproduct_join_array(ap_manager_t* manager,
						  ap_reducedproduct_t** tab, size_t size)
{
  return ap_reducedproduct_meetjoin_array(AP_FUNID_JOIN_ARRAY,manager,tab,size);
}

ap_reducedproduct_t* ap_reducedproduct_meet_lincons_array(ap_manager_t* manager,
							  bool destructive, ap_reducedproduct_t* a,
							  ap_lincons0_array_t* array)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_MEET_LINCONS_ARRAY,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_lincons0_array_t*) = man->funptr[AP_FUNID_MEET_LINCONS_ARRAY];
    res->p[i] = ptr(man,destructive,a->p[i],array);
    bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
    if (is_bottom(man,res->p[i])){
      set_bottom(intern,destructive,res,i);
      break;
    }
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_MEET_LINCONS_ARRAY,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_meet_tcons_array(ap_manager_t* manager,
							bool destructive, ap_reducedproduct_t* a,
							ap_tcons0_array_t* array)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_MEET_TCONS_ARRAY,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_tcons0_array_t*) = man->funptr[AP_FUNID_MEET_TCONS_ARRAY];
    res->p[i] = ptr(man,destructive,a->p[i],array);
    bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
    if (is_bottom(man,res->p[i])){
      set_bottom(intern,destructive,res,i);
      break;
    }
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_MEET_TCONS_ARRAY,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_add_ray_array(ap_manager_t* manager,
							   bool destructive, ap_reducedproduct_t* a,
							   ap_lincons0_array_t* array)
{
  ap_reducedproduct_internal_t* intern =
    get_internal_init1(manager,AP_FUNID_ADD_RAY_ARRAY,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_lincons0_array_t*) = man->funptr[AP_FUNID_ADD_RAY_ARRAY];
    res->p[i] = ptr(man,destructive,a->p[i],array);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_ADD_RAY_ARRAY,res);
  return res;
}

/* ============================================================ */
/* III.2 Assignment and Substitutions */
/* ============================================================ */

ap_reducedproduct_t*
ap_reducedproduct_asssub_linexpr_array(ap_funid_t funid,
				       /* either assign or substitute */
				       ap_manager_t* manager,
				       bool destructive,
				       ap_reducedproduct_t* a,
				       ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size,
				       ap_reducedproduct_t* dest)
{
  ap_reducedproduct_internal_t* intern = get_internal_init2(manager,funid,a,dest);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dim_t*,ap_linexpr0_t**,size_t,ap_reducedproduct_t*) = man->funptr[funid];
    res->p[i] = ptr(man,destructive,a->p[i],tdim,texpr,size, dest ? dest->p[i] : NULL);
    if (dest || funid==AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY){
      bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
      if (is_bottom(man,res->p[i])){
	set_bottom(intern,destructive,res,i);
	break;
      }
    }
  }
  res->reduced = false;
  collect_results1(manager,funid,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_assign_linexpr_array(ap_manager_t* man,
							    bool destructive,
							    ap_reducedproduct_t* a,
							    ap_dim_t* tdim,
							    ap_linexpr0_t** texpr,
							    size_t size,
							    ap_reducedproduct_t* dest)
{
  return ap_reducedproduct_asssub_linexpr_array(AP_FUNID_ASSIGN_LINEXPR_ARRAY,
					   man,destructive,a,tdim,texpr,size,dest);
}
ap_reducedproduct_t* ap_reducedproduct_substitute_linexpr_array(ap_manager_t* man,
								bool destructive,
								ap_reducedproduct_t* a,
								ap_dim_t* tdim,
								ap_linexpr0_t** texpr,
								size_t size,
								ap_reducedproduct_t* dest)
{
  return ap_reducedproduct_asssub_linexpr_array(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
					   man,destructive,a,tdim,texpr,size,dest);
}

ap_reducedproduct_t*
ap_reducedproduct_asssub_texpr_array(ap_funid_t funid,
				       /* either assign or substitute */
				       ap_manager_t* manager,
				       bool destructive,
				       ap_reducedproduct_t* a,
				       ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
				       ap_reducedproduct_t* dest)
{
  ap_reducedproduct_internal_t* intern = get_internal_init2(manager,funid,a,dest);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dim_t*,ap_texpr0_t**,size_t,ap_reducedproduct_t*) = man->funptr[funid];
    res->p[i] = ptr(man,destructive,a->p[i],tdim,texpr,size, dest ? dest->p[i] : NULL);
    if (dest || funid==AP_FUNID_SUBSTITUTE_TEXPR_ARRAY){
      bool (*is_bottom)(ap_manager_t*,void*) = man->funptr[AP_FUNID_IS_BOTTOM];
      if (is_bottom(man,res->p[i])){
	set_bottom(intern,destructive,res,i);
	break;
      }
    }
  }
  res->reduced = false;
  collect_results1(manager,funid,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_assign_texpr_array(ap_manager_t* man,
							    bool destructive,
							    ap_reducedproduct_t* a,
							    ap_dim_t* tdim,
							    ap_texpr0_t** texpr,
							    size_t size,
							    ap_reducedproduct_t* dest)
{
  return ap_reducedproduct_asssub_texpr_array(AP_FUNID_ASSIGN_TEXPR_ARRAY,
					   man,destructive,a,tdim,texpr,size,dest);
}
ap_reducedproduct_t* ap_reducedproduct_substitute_texpr_array(ap_manager_t* man,
								bool destructive,
								ap_reducedproduct_t* a,
								ap_dim_t* tdim,
								ap_texpr0_t** texpr,
								size_t size,
								ap_reducedproduct_t* dest)
{
  return ap_reducedproduct_asssub_texpr_array(AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
					   man,destructive,a,tdim,texpr,size,dest);
}

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

ap_reducedproduct_t* ap_reducedproduct_forget_array(ap_manager_t* manager,
						    bool destructive,
						    ap_reducedproduct_t* a,
						    ap_dim_t* tdim, size_t size,
						    bool project)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_FORGET_ARRAY,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dim_t*,size_t,bool) = man->funptr[AP_FUNID_FORGET_ARRAY];
    res->p[i] = ptr(man,destructive,a->p[i],tdim,size,project);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_FORGET_ARRAY,res);
  return res;
}

ap_reducedproduct_t* ap_reducedproduct_add_dimensions(ap_manager_t* manager,
						      bool destructive,
						      ap_reducedproduct_t* a,
						      ap_dimchange_t* dimchange,
						      bool project)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_ADD_DIMENSIONS,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dimchange_t*,bool) = man->funptr[AP_FUNID_ADD_DIMENSIONS];
    res->p[i] = ptr(man,destructive,a->p[i],dimchange,project);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_ADD_DIMENSIONS,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_remove_dimensions(ap_manager_t* manager,
							 bool destructive,
							 ap_reducedproduct_t* a,
							 ap_dimchange_t* dimchange)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_REMOVE_DIMENSIONS,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dimchange_t*) = man->funptr[AP_FUNID_REMOVE_DIMENSIONS];
    res->p[i] = ptr(man,destructive,a->p[i],dimchange);
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_REMOVE_DIMENSIONS,res);
  return res;
}

ap_reducedproduct_t* ap_reducedproduct_permute_dimensions(ap_manager_t* manager,
							  bool destructive,
							  ap_reducedproduct_t* a,
							  ap_dimperm_t* perm)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_PERMUTE_DIMENSIONS,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dimperm_t*) = man->funptr[AP_FUNID_PERMUTE_DIMENSIONS];
    res->p[i] = ptr(man,destructive,a->p[i],perm);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_PERMUTE_DIMENSIONS,res);
  return res;
}

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

ap_reducedproduct_t* ap_reducedproduct_expand(ap_manager_t* manager,
					      bool destructive,
					      ap_reducedproduct_t* a,
					      ap_dim_t dim,
					      size_t n)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_EXPAND,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dim_t,size_t) = man->funptr[AP_FUNID_EXPAND];
    res->p[i] = ptr(man,destructive,a->p[i],dim,n);
  }
  res->reduced = a->reduced;
  collect_results1(manager,AP_FUNID_EXPAND,res);
  return res;
}
ap_reducedproduct_t* ap_reducedproduct_fold(ap_manager_t* manager,
					    bool destructive,
					    ap_reducedproduct_t* a,
					    ap_dim_t* tdim,
					    size_t size)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_FOLD,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*,ap_dim_t*,size_t) = man->funptr[AP_FUNID_FOLD];
    res->p[i] = ptr(man,destructive,a->p[i],tdim,size);
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_FOLD,res);
  return res;
}

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

ap_reducedproduct_t* ap_reducedproduct_widening(ap_manager_t* manager,
						ap_reducedproduct_t* a1, ap_reducedproduct_t* a2)
{
  ap_reducedproduct_internal_t* intern = get_internal_init2(manager,AP_FUNID_WIDENING,a1,a2);
  size_t i;
  ap_reducedproduct_t* res;

  res = ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,void*,void*) = man->funptr[AP_FUNID_WIDENING];
    res->p[i] = ptr(man,a1->p[i],a2->p[i]);
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_WIDENING,res);
  return res;
}

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

ap_reducedproduct_t* ap_reducedproduct_closure(ap_manager_t* manager, bool destructive, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init1(manager,AP_FUNID_CLOSURE,a);
  size_t i;
  ap_reducedproduct_t* res;

  res = destructive ? a : ap_reducedproduct_alloc(intern->size);

  for (i=0;i<intern->size;i++){
    ap_manager_t* man = intern->tmanagers[i];
    void* (*ptr)(ap_manager_t*,bool,void*) = man->funptr[AP_FUNID_CLOSURE];
    res->p[i] = ptr(man,destructive,a->p[i]);
  }
  res->reduced = false;
  collect_results1(manager,AP_FUNID_CLOSURE,res);
  return res;
}


/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

void ap_reducedproduct_internal_free(void* p)
{
  ap_reducedproduct_internal_t* intern = (ap_reducedproduct_internal_t*)p;
  size_t i;
  for (i=0;i<intern->size;i++){
    ap_manager_free(intern->tmanagers[i]);
    intern->tmanagers[i] = NULL;
  }
  free(intern->version);
  free(intern);
}


ap_manager_t* ap_reducedproduct_manager_alloc
(
 char* library,
 ap_manager_t** tab,
 size_t size,
 void (*reduce)(ap_manager_t*, ap_reducedproduct_t*),            /* reduce function */
 void (*approximate)(ap_manager_t*, ap_reducedproduct_t*, int n) /* approximate function */
)
{
  char* version;
  size_t i,index, length;
  ap_reducedproduct_internal_t* internal;
  ap_manager_t* man;
  ap_funid_t funid;
  void** funptr;

  assert(size>=2);
  if (size<2) return NULL;



  /* Creating libray name and version */
  length = 100 + 2 * size;
  for (i=0;i<size;i++) length += strlen(tab[i]->version);
  version = malloc(length);
  index = snprintf(version,length,"%s, %s",
                   tab[0]->version,tab[1]->version);
  for (i=2; i<size; i++){
    index += snprintf(version+index,length-index,", %s",tab[i]->version);
  }
  /* creating internal */
  internal = malloc(sizeof(ap_reducedproduct_internal_t) +
		    size * sizeof(ap_manager_t*));
  internal->size = size;
  for (i=0; i<size; i++){
    internal->tmanagers[i] = ap_manager_copy(tab[i]);
  }
  internal->reduce = reduce;
  internal->approximate = approximate;
  internal->library = library;
  internal->version = strdup(version);
  free(version);

  /* allocating managers */
  man = ap_manager_alloc(internal->library,internal->version,
			 internal,
			 &ap_reducedproduct_internal_free);
  /* default options */
  for (funid=0; funid<AP_FUNID_SIZE; funid++){
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

  funptr[AP_FUNID_COPY] = &ap_reducedproduct_copy;
  funptr[AP_FUNID_FREE] = &ap_reducedproduct_free;
  funptr[AP_FUNID_ASIZE] = &ap_reducedproduct_size;
  funptr[AP_FUNID_MINIMIZE] = &ap_reducedproduct_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &ap_reducedproduct_canonicalize;
  funptr[AP_FUNID_HASH] = &ap_reducedproduct_hash;
  funptr[AP_FUNID_APPROXIMATE] = &ap_reducedproduct_approximate;
  funptr[AP_FUNID_FPRINT] = &ap_reducedproduct_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &ap_reducedproduct_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &ap_reducedproduct_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &ap_reducedproduct_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &ap_reducedproduct_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &ap_reducedproduct_bottom;
  funptr[AP_FUNID_TOP] = &ap_reducedproduct_top;
  funptr[AP_FUNID_OF_BOX] = &ap_reducedproduct_of_box;
  funptr[AP_FUNID_DIMENSION] = &ap_reducedproduct_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &ap_reducedproduct_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &ap_reducedproduct_is_top;
  funptr[AP_FUNID_IS_LEQ] = &ap_reducedproduct_is_leq;
  funptr[AP_FUNID_IS_EQ] = &ap_reducedproduct_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &ap_reducedproduct_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &ap_reducedproduct_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &ap_reducedproduct_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &ap_reducedproduct_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &ap_reducedproduct_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &ap_reducedproduct_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &ap_reducedproduct_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &ap_reducedproduct_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &ap_reducedproduct_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &ap_reducedproduct_to_tcons_array;
  funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &ap_reducedproduct_to_generator_array;
  funptr[AP_FUNID_MEET] = &ap_reducedproduct_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &ap_reducedproduct_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &ap_reducedproduct_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &ap_reducedproduct_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &ap_reducedproduct_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &ap_reducedproduct_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &ap_reducedproduct_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &ap_reducedproduct_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &ap_reducedproduct_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &ap_reducedproduct_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &ap_reducedproduct_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &ap_reducedproduct_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &ap_reducedproduct_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &ap_reducedproduct_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &ap_reducedproduct_forget_array;
  funptr[AP_FUNID_EXPAND] = &ap_reducedproduct_expand;
  funptr[AP_FUNID_FOLD] = &ap_reducedproduct_fold;
  funptr[AP_FUNID_WIDENING] = &ap_reducedproduct_widening;
  funptr[AP_FUNID_CLOSURE] = &ap_reducedproduct_closure;

  return man;
}

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

void** ap_reducedproduct_decompose(ap_manager_t* manager, bool destructive, ap_reducedproduct_t* a)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  size_t i;
  void** res = (void**)malloc(intern->size*sizeof(void*));

  for (i=0;i<intern->size;i++){
    if (destructive){
      res[i] = a->p[i];
    }
    else {
      ap_manager_t* man = intern->tmanagers[i];
      void* (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_COPY];
      res[i] = ptr(man,a->p[i]);
    }
  }
  if (destructive) free(a);
  collect_results0(manager);
  return res;
}

ap_reducedproduct_t* ap_reducedproduct_compose(ap_manager_t* manager, bool destructive, void** tabs)
{
  ap_reducedproduct_internal_t* intern = get_internal_init0(manager);
  ap_reducedproduct_t* res = ap_reducedproduct_alloc(intern->size);
  size_t i;
  
  for (i=0;i<intern->size;i++){
    if (destructive){
      res->p[i] = tabs[i];
    }
    else {
      ap_manager_t* man = intern->tmanagers[i];
      void* (*ptr)(ap_manager_t*,void*) = man->funptr[AP_FUNID_COPY];
      res->p[i] = ptr(man,tabs[i]);
    }
  }
  collect_results0(manager);
  return res;
}
